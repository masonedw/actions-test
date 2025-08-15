using Blind_Config_Tool.Core;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading;
using System.Windows;
using System.Windows.Input;
using System.Windows.Interop;
using static OfficeOpenXml.ExcelErrorValue;

namespace Blind_Config_Tool.RedesignFiles
{
    public class ScriptHandler
    {
        #region Declarations
        //Singleton
        private static ScriptHandler _instance;    //The single instance of this class
        private static readonly object _lock = new object();    //The lock (used to ensure that only one instance of this class is created)

        //Serial
        Connections _connector = Connections.Instance;

        //App Data
        AppData _appData = AppData.Instance;

        //Script Variables
        Dictionary<Messages, List<CommunicationAction>> _scriptDictionary;      //Dictionary that holds all the scripts
        Messages _script;       //The script we're using
        CommunicationAction _action;    //The action we're using
        int _scriptIdx = -1;    //Denotes where we are in the current script
        int _failCounter = 0;   //How many times we've failed the current message
        int _repeatCounter = 0;  //How many times we've repeated the a message (used for messages like getting all keypad commands or motor groups)
        bool _validationPerformed = false;  //If true, a motor has been validated during a response function (triggers the system to repeat a search message)
        int _timesSearched = 0; //How many times the motors have been searched for (used to repeat the searching a set amount of times)
        Messages _currentMessage;   //The message that is being processed
        int _messageRepeatCounter;  //How many times the same message has been run
        ElementActions _lastReceiveAction; //The previous receive action we performed (used for checking if we're repeating a command)
        byte[] _lastSentMessage;    //The final state of the last message just before it was sent out. Used for resending the last message
        bool sentDuplicateRepeatMessage;
        #endregion

        #region Events
        public event Action<string> OnCommandDescriptionUpdated;

        class RepeatAddress
        {
            public byte[] Address;
            public int Entries;

            public RepeatAddress(byte[] _addr)
            {
                Address = _addr;
                Entries = 1;
            }
        }

        public event Action<bool> OnToggleCanClearState;
        #endregion

        #region Constructor
        private ScriptHandler()    //Private constructor to prevent outside instantiation
        {
            _appData.OnIntervalTimerDone += ReadMessage;
            _scriptDictionary = new MessageScripts().DictionaryOfMessages;
            Console.WriteLine("Scripts instantiated");
        }
        #endregion

        #region Singleton Instantiation
        public static ScriptHandler Instance
        {
            get
            {
                if (_instance == null)   //Check whether an instance has been created and if not, try to create one
                {
                    lock (_lock) //Prevent the thread from continuing on if another thread is in the process of creating an instance
                    {
                        if (_instance == null)   //Second check inside the lock
                        {
                            _instance = new ScriptHandler();   //Create the instance
                        }
                    }
                }

                return _instance;
            }
        }
        #endregion

        #region Starting A New Script
        public void StartScript(Messages msg)
        {
            _validationPerformed = false;
            _timesSearched = 0;
            OnToggleCanClearState?.Invoke(false);

            if (msg != Messages.MOVE_MOTOR_UP && msg != Messages.MOVE_MOTOR_DOWN && msg != Messages.START_IDENTIFICATION_MOVEMENT)
            {
                _appData.CancelTimer(AppData.Timers.UP_MOVEMENT);
                _appData.CancelTimer(AppData.Timers.DOWN_MOVEMENT);
                _appData.CancelTimer(AppData.Timers.VISUAL_FEEDBACK);
            }

            if (!_connector.IsConnected)
            {
                _appData.DisplayError("There is no open device connection. Please connect to a device and try again.", "No Open connection");
                OnToggleCanClearState?.Invoke(true);
                return;
            }

            if(_currentMessage == msg)
            {
                _messageRepeatCounter++;
            }
            else
            {
                _messageRepeatCounter = 0;
            }

            _scriptIdx = -1;        //Set the current script index to the beginning
            _script = msg;
            _currentMessage = msg;

            SendNextMessage();      //Send the first message in the script
        }
        #endregion

        #region Performing A Script Element
        void SendNextMessage()
        {
            _scriptIdx++;                                               //Move to the next part of the script
            _failCounter = 0;

            if (_scriptIdx >= _scriptDictionary[_script].Count) //If we've reached the end of the script then we can just 
            {
                OnCommandDescriptionUpdated?.Invoke("Done!");
                _appData.SetTimer(AppData.Timers.HIDE_COMMAND_DESC, Constants.CommandDescVisibleTime);
                _currentMessage = Messages.NO_MESSAGE;
                OnToggleCanClearState(true);
                return;
            }

            BuildMessage();
        }

        private void BuildMessage()
        {
            _action = _scriptDictionary[_script][_scriptIdx];           //Retrieves the next action to be performed
            _repeatCounter = _lastReceiveAction == _action.ElementName ? _repeatCounter + 1 : 0;  //If we're doing the same action as last time, increment the repeat counter

            MessageStructure message = new MessageStructure(_action.Message);   //Copy the structure from the message script
            _connector.ClearData();  //Clears the buffers of the open connection

            _lastReceiveAction = _action.ElementName;   //Set the lastOnReceive to the current command's onReceive action                                   

            Array.Copy(_appData.AppAddress, message.Sender, 3);   //Add the app address
            message.Receiver = AddTargetAddressToMessage(_action.TargetType, out bool validTarget);   //Get the target address

            if (validTarget == false)
            {
                _appData.DisplayError("Attempted to target an invalid address. The action could not be completed.", "Invalid Address");
                CancelScript();
                return;
            }

            _action.PreprocessAction?.Invoke(message);  //Perform the custom preprocess function if this message has one

            if (_action.TargetType == Target.SPECIFIC_MOTOR)             //Either get the type of the selected motor or get the next one in the list
            {
                message.NodeID = _appData.SelectedMotor.Type;
            }
            else if (_action.TargetType == Target.ALL_MOTORS_IN_LIST)
            {
                message.NodeID = _appData.Motors[_repeatCounter].Type;
            }
            else
            {
                message.NodeID[0] = 0x80;
                message.NodeID[1] = 0xF0;
            }

            message.Length[0] = CalculateMessageLength(message, _action);                                    //Calculate the length of the message  
            message.RawData = ConvertToRaw(message);                                                     //Convert to a single message to send

            UpdateProcessLabel(_action);                                //Display on the command label what action we're about to perform
            _connector.SendMessage(message.RawData);                                                            //Send the message
            _appData.SetTimer(AppData.Timers.INTERVAL_DONE, AppData.SERIAL_READ_INTERVAL_TIME/*GetIntervalTime(_action.ElementName)*/);    //Set the timer to wait for the response
            _lastSentMessage = message.RawData;
        }

        byte[] ConvertToRaw(MessageStructure msg)
        {
            byte[] retVal = new byte[msg.Data.Length + MessageStructure.staticBytes];

            retVal[0] = msg.Command[0];
            retVal[1] = msg.Length[0];
            retVal[2] = msg.NodeID[0];
            retVal[3] = msg.NodeID[1];

            for(int i = 0; i < 3; i++)
            {
                retVal[4 + i] = msg.Sender[i];
                retVal[7 + i] = msg.Receiver[i];
            }

            for(int i = 0; i < msg.Data.Length; i++)
            {
                retVal[10+i] = msg.Data[i];
            }

            ushort crc = CalculateChecksum(retVal, false);

            retVal[retVal.Length - 2] = (byte)(crc >> 8);
            retVal[retVal.Length - 1] = (byte)(crc & 0xFF);

            return retVal;
        }

        public void CancelScript()
        {
            OnCommandDescriptionUpdated?.Invoke($"Error during command: {_lastReceiveAction}");
            _appData.SetTimer(AppData.Timers.HIDE_COMMAND_DESC, Constants.CommandDescVisibleTime);
            _currentMessage = Messages.NO_MESSAGE;
            OnToggleCanClearState(true);
            TimesRepeated = 0;
        }

        public void ResendLastMessage()
        {
            BuildMessage();
        }
        #endregion

        #region Building Script Actions Before Sending A New Message
        private void UpdateProcessLabel(CommunicationAction action)
        {
            string description = action.Description;

            switch (action.ElementName)
            {
                case ElementActions.KEYPAD_SET_COMMANDS:
                    description = $"Programming commands ({_repeatCounter + 1} of {_appData.KeypadCommandsList.Count})";
                    break;
                case ElementActions.KEYPAD_GET_COMMANDS:
                    description = $"Retrieving data for command {_repeatCounter + 1}";
                    break;
                case ElementActions.KEYPAD_SET_GROUP_ADDRESSES:
                    description = $"Programming groups ({_repeatCounter * 10 + 1} to {_repeatCounter * 10 + 10})";
                    break;
                case ElementActions.KEYPAD_GET_GROUP_ADDRESSES:
                    description = $"Retrieving groups ({_repeatCounter * 10 + 1} to {_repeatCounter * 10 + 10})";
                    break;
                case ElementActions.MOTOR_GET_GROUP_IDS:
                    description = $"Retrieving groups ({_repeatCounter+1} of 16)";
                    break;
                case ElementActions.MOTOR_SET_GROUP_ID:
                    description = $"Setting groups ({_repeatCounter + 1} of 16)";
                    break;
                case ElementActions.MOTOR_GET_INTERNAL_POSITIONS:
                    description = $"Retrieving IPs ({_repeatCounter + 1} of 16)";
                    break;
                case ElementActions.MOTOR_SET_INTERNAL_POSITION:
                    description = $"Setting IPs ({_repeatCounter + 1} of 16)";
                    break;
            }

            Console.WriteLine($"SENDING MESSAGE: {description}");
            OnCommandDescriptionUpdated?.Invoke(description);
            _appData.CancelTimer(AppData.Timers.HIDE_COMMAND_DESC);
        }

        private byte CalculateMessageLength(MessageStructure msg, CommunicationAction action)                     //Calculating the length of a given message
        {            
            return (byte)(action.RequiresAcknowledgement ? 0x80 + (msg.Data.Length + MessageStructure.staticBytes) : (byte)(msg.Data.Length + MessageStructure.staticBytes));
        }

        private ushort CalculateChecksum(byte[] msg, bool includeChecksum)                                //Calculating the checksum of a message
        {
            //int total = 0;                                                                              //The addition of all the bytes in the message
            //byte[] retVal = new byte[2];                                                                //array to hold the bytes

            //total += msg.Command[0];
            //total += msg.Length[0];
            //total += msg.NodeID[0];
            //total += msg.NodeID[1];

            //for(int i = 0; i < 3; i++)
            //{
            //    total += msg.Sender[i];
            //    total += msg.Receiver[i];
            //}

            //for(int i = 0; i < msg.Data.Length; i++)
            //{
            //    total += msg.Data[i];
            //}

            //retVal[0] = (byte)(total >> 8);                                             //Add the checksum bytes to the end of the message
            //retVal[1] = (byte)(total & 0xFF);                                           //Add the checksum bytes to the end of the message

            //return retVal;                                                              //Return the newly created message

            ushort crc = 0xFFFF;

            int checkLength = msg.Length;

            if (!includeChecksum)
            {
                checkLength -= 2;
            }

            for(int i = 0; i < checkLength; i++)
            {
                byte tableIndex = (byte)((crc >> 8) ^ msg[i]);
                crc = (ushort)((crc << 8) ^ CRCTable.crc16CcittTable[tableIndex]);
            }

            //foreach (byte b in msg)
            //{
            //    byte tableIndex = (byte)((crc >> 8) ^ b);
            //    crc = (ushort)((crc << 8) ^ CRCTable.crc16CcittTable[tableIndex]);
            //}

            return crc;
        }

        private byte[] AddTargetAddressToMessage(Target target, out bool validTarget)
        {
            validTarget = true;
            byte[] targetAddress = new byte[3] { 0xFF, 0xFF, 0xFF};

            switch (target)
            {
                case Target.ALL_MOTORS_IN_LIST:
                    if(_repeatCounter < _appData.Motors.Count)
                    { 
                        targetAddress[0] = _appData.Motors[_repeatCounter].AddressAsBytes[0];
                        targetAddress[1] = _appData.Motors[_repeatCounter].AddressAsBytes[1];
                        targetAddress[2] = _appData.Motors[_repeatCounter].AddressAsBytes[2];
                    }
                    else
                    {
                        validTarget = false;
                    }
                    break;
                case Target.ALL_KEYPADS_IN_LIST:
                    if (_repeatCounter < _appData.Keypads.Count)
                    {
                        targetAddress[0] = _appData.Keypads[_repeatCounter].AddressAsBytes[0];
                        targetAddress[1] = _appData.Keypads[_repeatCounter].AddressAsBytes[1];
                        targetAddress[2] = _appData.Keypads[_repeatCounter].AddressAsBytes[2];
                    }
                    else
                    {
                        validTarget = false;
                    }
                    break;
                case Target.SPECIFIC_KEYPAD:
                    targetAddress[0] = _appData.SelectedKeypad.AddressAsBytes[0];
                    targetAddress[1] = _appData.SelectedKeypad.AddressAsBytes[1];
                    targetAddress[2] = _appData.SelectedKeypad.AddressAsBytes[2];
                    break;
                case Target.SPECIFIC_MOTOR:
                    targetAddress[0] = _appData.SelectedMotor.AddressAsBytes[0];
                    targetAddress[1] = _appData.SelectedMotor.AddressAsBytes[1];
                    targetAddress[2] = _appData.SelectedMotor.AddressAsBytes[2];
                    break;
                case Target.SPECIFIC_DEVICE:
                    targetAddress[0] = _appData.SelectedDevice.AddressAsBytes[0];
                    targetAddress[1] = _appData.SelectedDevice.AddressAsBytes[1];
                    targetAddress[2] = _appData.SelectedDevice.AddressAsBytes[2];
                    break;
            }

            return targetAddress;
        }

        //int GetIntervalTime(ElementActions receiveAction)
        //{
        //    int _interval;

        //    switch (receiveAction)
        //    {
        //        //case ElementActions.DO_NOTHING:                                                    //Start the timer that checks for a response to this message
        //        //    _interval = 50;
        //        //    break;
        //        default:
        //            _interval = 75;
        //            break;
        //    }

        //    return _interval;
        //}
        #endregion

        #region Reading The Response
        void ReadMessage()
        {
            _appData.CancelTimer(AppData.Timers.INTERVAL_DONE);

            if(_lastReceiveAction == ElementActions.DO_NOTHING)
            {
                SendNextMessage();
                return;
            }

            List<MessageStructure> responses = GetAllResponses(_action.Length);

            if (responses.Count == 0)   //If there are no responses
            {
                if (MessageNeedsResponses())
                {
                    if (_failCounter >= 3)
                    {
                        _appData.DisplayError($"No valid responses received for message: {_lastReceiveAction}. Possible faulty connection or incorrect command type.", "No Responses Received");
                        CancelScript();
                        return;
                    }
                    else
                    {
                        _failCounter++;
                        ResendLastMessage();
                        return;
                    }
                }
            }
            else
            {
                if (_lastReceiveAction == ElementActions.KEYPAD_SEARCH || _lastReceiveAction == ElementActions.MOTOR_SEARCH)   //For the search messages, get a list of what addresses respond to check if theres's any duplicates
                {
                    List<RepeatAddress> repeats = CheckForDuplicateAddresses(responses, out bool hasDuplicates, out List<MessageStructure> validResponses);    //Get a list of all addresses and how many times they've appeared
                    responses = validResponses;

                    if (hasDuplicates)  //If duplicates are present, display an error and cancel the script
                    {
                        string duplicationMessage = "Multiple devices found to be using the same address. Each device must have a unique address. Please unplug duplicate devices and change each address individually. Duplicate devices will be ignored. \n\nDuplicate addresses:\n";

                        foreach (RepeatAddress rep in repeats)
                        {
                            if (rep.Entries > 1)
                            {
                                duplicationMessage += $"\n{BitConverter.ToString(rep.Address)}: {rep.Entries} devices found";
                            }
                        }

                        _appData.DisplayError(duplicationMessage, "Duplicate Addresses");

                        if (sentDuplicateRepeatMessage == false)
                        {
                            ResendLastMessage();
                        }
                        else
                        {
                            return;
                        }
                    }
                    else
                    {
                        sentDuplicateRepeatMessage = false;
                    }
                }

                foreach (MessageStructure response in responses)
                {
                    if (_action.ResponseAction != null)
                    {
                        _action.ResponseAction(response);    //Execute the receive action and pass over the response
                    }
                }
            }

            if ((_action.ElementName == ElementActions.MOTOR_VALIDATE || _action.ElementName == ElementActions.KEYPAD_VALIDATE) && responses.Count <= 0)
            {
                DeviceData dev = _appData.GetDeviceByAddress(new byte[] { _lastSentMessage[7], _lastSentMessage[8], _lastSentMessage[9] });

                if(dev != null)
                {
                    if(dev.ValidationAttempts < 3)
                    {
                        dev.ValidationAttempts++;
                    }
                    else
                    {
                        _appData.RemoveDeviceFromList(dev);
                    }
                }
            }

            if (_action.TargetType == Target.ALL_MOTORS_IN_LIST && _repeatCounter < _appData.Motors.Count-1)
            {
                ResendLastMessage();
                return;
            }

            else if(_action.TargetType == Target.ALL_KEYPADS_IN_LIST && _repeatCounter < _appData.Keypads.Count-1)
            {
                ResendLastMessage();
                return;
            }

            if (_action.ElementName == ElementActions.KEYPAD_SEARCH && _appData.Keypads.Count <= 0)
            {
                if(TimesRepeated > 3)
                {
                    if (_currentMessage == Messages.SEARCH_FOR_ALL_DEVICES)
                    {
                        _scriptIdx += 2;
                    }
                    else
                    {
                        _appData.DisplayError("No keypads found. Check the connections and try again.", "No Keypads Found");
                        CancelScript();
                        return;
                    }
                }
                else
                {
                    ResendLastMessage();
                    return;
                }
            }
            else if(_action.ElementName == ElementActions.MOTOR_SEARCH && _appData.Motors.Count <= 0)
            {
                if (TimesRepeated > 3)
                {
                    if (_currentMessage == Messages.SEARCH_FOR_ALL_DEVICES)
                    {
                        _scriptIdx += 2;

                        if (_appData.Motors.Count == 0 && _appData.Keypads.Count == 0)
                        {
                            _appData.DisplayError("No devices found. Check the connections and try again.", "No Devices Found");
                            CancelScript();
                            return;
                        }
                    }
                    else
                    {
                        _appData.DisplayError("No motors found. Check the connections and try again.", "No Motors Found");
                        CancelScript();
                        return;
                    }
                }
                else
                {
                    ResendLastMessage();
                }
            }

            if(_repeatCounter < 15 && (_action.ElementName == ElementActions.MOTOR_GET_GROUP_IDS || _action.ElementName == ElementActions.MOTOR_GET_INTERNAL_POSITIONS || _action.ElementName == ElementActions.MOTOR_SET_GROUP_ID || _action.ElementName == ElementActions.MOTOR_SET_INTERNAL_POSITION))
            {
                ResendLastMessage();
                return;
            }

            if ((_action.ElementName == ElementActions.MOTOR_VALIDATE && _repeatCounter >= _appData.Motors.Count-1) || (_action.ElementName == ElementActions.KEYPAD_VALIDATE && _repeatCounter >= _appData.Keypads.Count - 1))    //If we've performed a validation on all the motors/keypads in the list
            {
                if (_validationPerformed)   //If any of them were validated for the first time this loop
                {
                    _timesSearched = 0; //Reset the search counter

                    if(_script == Messages.SEARCH_FOR_ALL_DEVICES && _action.ElementName == ElementActions.MOTOR_VALIDATE)
                    {
                        _scriptIdx = 4;
                    }
                    else
                    {
                        _scriptIdx = -1;
                    }
                }
                else
                {
                    if (_timesSearched < 3) //Perform the search a few times after the last validation just to ensure there aren't any motors left
                    {
                        if (_script == Messages.SEARCH_FOR_ALL_DEVICES)
                        {
                            _scriptIdx = 5;
                        }
                        else
                        {
                            _scriptIdx = -1;
                        }

                        _timesSearched++;
                    }
                    else
                    {
                        _timesSearched = 0; //Reset the search counter for future use
                    }
                }

                _validationPerformed = false;
            }

            SendNextMessage();  //Move on to the next message
        }

        private List<RepeatAddress> CheckForDuplicateAddresses(List<MessageStructure> responses, out bool hasDuplicates, out List<MessageStructure> ValidResponses)
        {
            hasDuplicates = false;
            List<RepeatAddress> repeats = new List<RepeatAddress>();
            ValidResponses = new List<MessageStructure>();

            foreach (MessageStructure resp in responses)
            {
                if (repeats.Count <= 0)
                {
                    repeats.Add(new RepeatAddress(resp.Sender));
                    ValidResponses.Add(resp);
                    continue;
                }

                bool isDuplicate = false;

                for (int i = 0; i < repeats.Count; i++)
                {
                    if (resp.Sender[0] == repeats[i].Address[0] && resp.Sender[1] == repeats[i].Address[1] && resp.Sender[2] == repeats[i].Address[2])
                    {
                        repeats[i].Entries++;
                        hasDuplicates = true;
                        isDuplicate = true;
                        break;
                    }
                }

                if (!isDuplicate)
                {
                    repeats.Add(new RepeatAddress(resp.Sender));
                    ValidResponses.Add(resp);
                }
            }

            return repeats;
        }

        bool MessageNeedsResponses()
        {
            switch (_action.ElementName)
            {
                case ElementActions.KEYPAD_SEARCH:
                case ElementActions.MOTOR_SEARCH:
                    return false;
                default:
                    return true;
            }
        }

        private List<MessageStructure> GetAllResponses(byte[] actionLength) //Get a list of responses from all the devices
        {
            int longestLength = 0;

            List<MessageStructure> responses = new List<MessageStructure>();
            //byte[] allBytes = AssistantFunctions.InvertBytes(_serial.ReadResponse());
            byte[] allBytes = _connector.ReadResponse();

            for (int i = 0; i < _action.Length.Length; i++)
            {
                if ((byte)actionLength[i] > longestLength)
                {
                    longestLength = (byte)actionLength[i];
                }
            }
                
            for (int i = 0; i < allBytes.Length - longestLength; i++)
            {
                bool validType = false;

                for(int j = 0; j < _action.ResponseType.Length; j++)
                {
                    if (allBytes[i] == (byte)_action.ResponseType[j])
                    {
                        validType = true;
                    }
                }

                if (validType)
                {
                    try
                    {
                        byte[] potentialMessage = new byte[allBytes[i + 1]];
                        Array.Copy(allBytes, i, potentialMessage, 0, allBytes[i + 1]);    //Copy the data from the serial buffer to the new array
                        MessageStructure response = new MessageStructure(potentialMessage);   //Split the message into its individual components

                        if (MessageIsValid(response, _action.ResponseType, actionLength))
                        {
                            responses.Add(response);    //It's a valid message
                            i += allBytes[i + 1] - 1;
                        }
                    }
                    catch(Exception e)
                    {
                        _appData.DisplayError("There may be too much disturbance in the response signal. Remove any faulty devices from the network and try again.", "Could Not Verify Message");
                    }
                }

                if (allBytes[i] == (byte)ScriptCommands.NACK && allBytes[i+1] == 0x0C && _action.ElementName != ElementActions.MOTOR_VALIDATE && _action.ElementName != ElementActions.KEYPAD_VALIDATE)   //If it's a nack message
                {
                    byte[] addr = new byte[3] { allBytes[i+6], allBytes[i + 7], allBytes[i + 8] };
                    bool sameAddress = true;

                    for(int j = 0; j < 3; j++)
                    {
                        if (allBytes[j+6] != _appData.AppAddress[j])
                        {
                            sameAddress = false;
                            break;
                        }
                    }

                    for(int j = 0; j < _action.Length.Length; j++)
                    {
                        if (_action.Length[j] == 0x90)
                        {
                            sameAddress = false;
                        }
                    }

                    if(sameAddress)
                    {
                        string msg;
                        switch(allBytes[i + 9])
                        {
                            case 0x01:
                                msg = "NACK 0x01: Data out of range.\nThe data was not within the allowable range (e.g. IP greater than 16 or percent greater than 100)";
                                break;
                            case 0x10:
                                msg = "NACK 0x10: Unknown message.\nSent an unknown message identifier (message byte 1)";
                                break;
                            case 0x11:
                                msg = "NACK 0x11: Message length error.\nThe given length of the sent message did not match the actual length";
                                break;
                            case 0x12:
                                msg = "NACK 0x12: Message checksum error.\nThe given checksum of the sent message did not match the actual checksum";
                                break;
                            case 0xFF:
                                msg = "NACK 0xFF: Cannot process message.\nThe device cannot process the received message";
                                break;
                            default:
                                msg = $"Unknown NACK error (code: {allBytes[i+9]})";
                                break;
                        }

                        _appData.DisplayError(msg, "NACK Error");
                        return null;
                    }
                }
            }

            return responses;
        }

        bool MessageIsValid(MessageStructure msg, byte[] messageTypes, byte[] messageLengths) //Check that the message type and length match what we expect to see
        {
            bool validType = false;
            bool validLength = false;

            for(int i = 0; i < messageTypes.Length; i++)
            {
                if (msg.Command[0] == (byte)messageTypes[i])
                {
                    validType = true;
                }
            }

            for (int i = 0; i < messageLengths.Length; i++)
            {
                if (msg.Length[0] == (byte)messageLengths[i])
                {
                    validLength = true;
                }
            }

            if(!validType || !validLength)
            {
                return false;
            }

            if(!IsChecksumValid(msg))
            {
                //_appData.DisplayError($"Invalid checksum. Expected checksum total of {total} but received {receivedChecksum} instead.", "Unexpected checksum length");
                return false;
            }

            return true;
        }

        private bool IsChecksumValid(MessageStructure _messageToCheck)//Check if the checksum is accurate for this message
        {
            int total = CalculateChecksum(_messageToCheck.RawData, true);

            return total == 0;
        }
        #endregion

        #region Accessors
        public int CurrentElement { get { return _scriptIdx; } set { _scriptIdx = value; } }
        public int TimesRepeated { get { return _repeatCounter; } set { _repeatCounter = value;} }
        public bool ValidationRequired { get { return _validationPerformed; } set { _validationPerformed = value; } }
        public CommunicationAction CurrentAction { get { return _action; } }
        #endregion
    }
}