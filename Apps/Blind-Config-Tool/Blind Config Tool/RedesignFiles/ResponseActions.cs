using Blind_Config_Tool.Core;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Blind_Config_Tool.RedesignFiles
{
    public static class ResponseActions
    {
        static AppData _appData;
        static ScriptHandler _scriptHandler;

        static ResponseActions()
        {
            _appData = AppData.Instance;
            _scriptHandler = ScriptHandler.Instance;
        }

        #region Events
        public static event EventHandler<int> OnRequestIPListCount;

        public static event EventHandler<IPRetrieveEventArgs> OnIPRetrieved;
        public class IPRetrieveEventArgs : EventArgs
        {
            int[] ip;
            int[] subnet;
            bool dhcp;
            int port;

            public IPRetrieveEventArgs(int[] newIP, int[] newSubnet, bool newDHCP, int newPort)
            {
                ip = newIP;
                subnet = newSubnet;
                dhcp = newDHCP;
                port = newPort;
            }

            public int[] IP { get { return ip; } set { ip = value; } }
            public int[] Subnet { get { return subnet; } set { subnet = value; } }
            public bool DHCP { get { return dhcp; } set { dhcp = value; } }
            public int Port { get { return port; } set { port = value; } }
        }

        public static event Action<bool> OnToggleOrientation;
        public static event Action<bool> OnToggleSpeeds;
        public static event Action OnCheckUpButtonHeld;
        public static event Action OnCheckDownButtonHeld;

        public static void AddOnCheckUpButtonListener(Action listener)
        {
            if (OnCheckUpButtonHeld == null || !OnCheckUpButtonHeld.GetInvocationList().Any(d => d.Method == listener.Method))
            {
                OnCheckUpButtonHeld += listener;
            }
        }

        public static void AddOnCheckDownButtonListener(Action listener)
        {

            if (OnCheckDownButtonHeld == null || !OnCheckDownButtonHeld.GetInvocationList().Any(d => d.Method == listener.Method))
            {
                OnCheckDownButtonHeld += listener;
            }
        }
        #endregion

        /// <summary>
        /// MOTOR_RELATED FUNCTIONS
        /// </summary>
        ///
        public static void Response_Add_Motors_To_List(MessageStructure msg)
        {

            if (_appData.Motors.Count > 0) //If there are motors in the list then we need to check them before adding the new one
            {
                for (int i = 0; i < _appData.Motors.Count; i++) //Check the list to see if it's already been added
                {
                    if (_appData.Motors[i].CompareAddress(msg.Sender))
                    {
                        return;
                    }
                }
            }

            _appData.AddNewMotor(msg.Sender, msg.NodeID);
        }

        public static void Response_Validate_Motor(MessageStructure msg)
        {
            _scriptHandler.ValidationRequired = false;

            if (_appData.Motors.Count > 0) //If there are motors in the list then we need to check them before adding the new one
            {
                for (int i = 0; i < _appData.Motors.Count; i++) //Check the list to see if it's already been added
                {
                    if (!_appData.Motors[i].Validated)
                    {
                        _scriptHandler.ValidationRequired = true;
                    }

                    if (_appData.Motors[i].CompareAddress(msg.Sender))  //If this motor is the one that responded
                    {
                        _appData.Motors[i].Validated = true;
                    }
                }
            }
            else
            {
                _appData.DisplayError($"The system attempted to validate a motor that doesn't exist.", "Non-Existent Motor");
                _scriptHandler.CancelScript();
            }
        }

        public static void Response_Get_Motor_Label(MessageStructure msg)
        {
            string motorName = "";
            List<byte> label = new List<byte>();
            bool labelPresent = false;

            for (int i = 0; i < 16; i++)
            {
                if (msg.Data[i] != 0x00 && msg.Data[i] != 0xFF)
                {
                    label.Add(msg.Data[i]);
                    labelPresent = true;
                }
            }

            if (labelPresent == false)
            {
                motorName = "Unnamed Motor";
            }
            else
            {
                motorName = System.Text.Encoding.ASCII.GetString(label.ToArray());
            }

            _appData.GetDeviceByAddress(msg.Sender).NameAsString = motorName;
            _appData.RefreshSelectedMotor();

            if(_scriptHandler.TimesRepeated >= _appData.Motors.Count-1 || _scriptHandler.CurrentAction.TargetType == Target.SPECIFIC_MOTOR)
            {
                _appData.SortMotorList();
            }
        }

        public static void Response_Set_Motor_Address(MessageStructure msg)
        {
            byte[] newAddress = msg.Sender;
            _appData.SelectedMotor.ChangeAddress(newAddress);
            _appData.UpdateMotorList();
        }

        public static void Response_Get_Speeds(MessageStructure msg)
        {
            if (msg.Length[0] == 0xF1) //If the command is a nack
            {
                OnToggleOrientation?.Invoke(false); //Disallow the user to change the orientation
                return;
            }
            else
            {
                OnToggleOrientation?.Invoke(true);
            }

            int up = msg.Data[0];
            int down = msg.Data[1];
            int slow = msg.Data[2];

            _appData.SelectedMotor.SetSpeeds(up, down, slow);
            _appData.RefreshSelectedMotor();
        }

        public static void Response_Get_Motor_Orientation(MessageStructure msg)
        {
            if (msg.Command[0] == (byte)ScriptCommands.NACK) //If the command is a nack
            {
                OnToggleSpeeds(false); //Toggle which speed display is shown
                return;
            }
            else
            {
                OnToggleSpeeds(true);
            }

            _appData.SelectedMotor.MovementDirection = (byte)msg.Data[0];
            _appData.RefreshSelectedMotor();
        }

        public static void Response_Set_Orientation(MessageStructure msg)
        {
            return;
        }

        public static void Response_Move_Motor_Up(MessageStructure msg)
        {
            OnCheckUpButtonHeld?.Invoke();
        }

        public static void Response_Move_Motor_Down(MessageStructure msg)
        {
            OnCheckDownButtonHeld?.Invoke();
        }

        public static void Response_Get_Motor_Limits(MessageStructure msg)
        {
            _appData.SelectedMotor.DownLimit = (msg.Data[1] << 8) | msg.Data[0];
            _appData.SelectedMotor.UpLimit = (msg.Data[3] << 8) | msg.Data[2];
            _appData.RefreshSelectedMotor();
        }

        public static void Response_Get_Group_IDs(MessageStructure msg)
        {
            _appData.SelectedMotor.Groups[msg.Data[0]] = BitConverter.ToString(new byte[] { msg.Data[3], msg.Data[2], msg.Data[1] }).Replace("-", "");
            _appData.RefreshGroupsAndIPs();
        }

        public static void Response_Get_Internal_Positions(MessageStructure msg)
        {
            if (msg.Data[3] > 98)
            {
                _appData.SelectedMotor.InternalPositions[msg.Data[0] - 1] = "2";
            }
            else if (msg.Data[3] < 2)
            {
                _appData.SelectedMotor.InternalPositions[msg.Data[0] - 1] = "2";
            }
            else
            {
                _appData.SelectedMotor.InternalPositions[msg.Data[0] - 1] = msg.Data[3].ToString();
            }
            
            _appData.RefreshGroupsAndIPs();
        }

        public static void Response_Get_Single_Internal_Position(MessageStructure msg)
        {
            if (msg.Data[3] > 98)
            {
                _appData.SelectedMotor.InternalPositions[msg.Data[0] - 1] = "2";
            }
            else if (msg.Data[3] < 2)
            {
                _appData.SelectedMotor.InternalPositions[msg.Data[0] - 1] = "2";
            }
            else
            {
                _appData.SelectedMotor.InternalPositions[msg.Data[0] - 1] = msg.Data[3].ToString();
            }

            _appData.RefreshGroupsAndIPs();
        }

        /// <summary>
        /// KEYPAD-RELATED FUNCTIONS
        /// </summary>
        /// 
        public static void Response_Add_Keypad_To_List(MessageStructure msg)
        {
            byte[] _addr = msg.Sender;  //Get the keypad's address from the message

            if (_appData.Keypads.Count > 0) //If there are keypads in the list then we need to check them before adding the new one
            {
                for (int i = 0; i < _appData.Keypads.Count; i++) //Check the list to see if it's already been added
                {
                    if (_appData.Keypads[i].CompareAddress(_addr))  //If we've already added the keypad then don't do anything else
                    {
                        if (_appData.Keypads[i].CompareAddress(_addr))  //If we've already added and validated the motor then don't do anything else
                        {
                            if (_appData.Keypads[i].Validated == true)  //If we've already added and validated the motor then don't do anything else
                            {
                                _appData.DisplayError($"Device ({_appData.Keypads[i].AddressAsString}) has responded even though it has been validated. This could indicate two or more devices share this address. Make sure that all devices have a unique address.", $"Multiple Validations ({_appData.Keypads[i].AddressAsString})");
                            }

                            return;
                        }
                    }
                }
            }

            _appData.AddNewKeypad(_addr);    //Create a new keypad object and add it to the list
        }

        public static void Response_Validate_Keypad(MessageStructure msg)
        {
            _scriptHandler.ValidationRequired = false;

            if (_appData.Keypads.Count > 0) //If there are keypads in the list then we need to check them before adding the new one
            {
                for (int i = 0; i < _appData.Keypads.Count; i++) //Check the list to see if it's already been added
                {
                    if (!_appData.Keypads[i].Validated)
                    {
                        _scriptHandler.ValidationRequired = true;
                    }

                    if (_appData.Keypads[i].CompareAddress(msg.Sender))  //If this motor is the one that responded
                    {
                        _appData.Keypads[i].Validated = true;
                    }
                }
            }
            else
            {
                _appData.DisplayError($"The system attempted to validate a keypad that doesn't exist.", "Non-Existent Keypads");
                _scriptHandler.CancelScript();
            }
        }

        public static void Response_Set_Keypad_Address(MessageStructure msg)
        {
            byte[] newAddress = msg.Sender;
            _appData.SelectedKeypad.ChangeAddress(newAddress);
            _appData.UpdateKeypadList();
        }

        public static void Response_Program_Keypad_Commands(MessageStructure msg)
        {
            if (_scriptHandler.TimesRepeated < _appData.KeypadCommandsList.Count)
            {
                switch(_appData.UploadType)
                {
                    case AppData.Upload_Type.UPLOAD_ALL:
                        if (_scriptHandler.TimesRepeated < _appData.KeypadCommandsList.Count-1)
                        {
                            _scriptHandler.CurrentElement--;
                        }
                        break;
                    case AppData.Upload_Type.UPLOAD_BUTTONS_WITH_VALUES:
                        for (int i = _scriptHandler.TimesRepeated + 1; i < _appData.KeypadCommandsList.Count; i++)
                        {
                            if (_appData.KeypadCommandsList[i].OnPressAction != KeypadButtonAction.NO_ACTION || _appData.KeypadCommandsList[i].OnHoldAction != KeypadButtonAction.NO_ACTION || _appData.KeypadCommandsList[i].OnReleaseAction != KeypadButtonAction.NO_ACTION)
                            {
                                _scriptHandler.TimesRepeated = i - 1;
                                _scriptHandler.CurrentElement -= 1;
                                return;
                            }
                        }
                        break;
                    default:
                        for (int i = _scriptHandler.TimesRepeated + 1; i < _appData.KeypadCommandsList.Count; i++)
                        {
                            if (_appData.KeypadCommandsList[i].Changed)
                            {
                                _scriptHandler.TimesRepeated = i - 1;
                                _scriptHandler.CurrentElement -= 1;
                                return;
                            }
                        }
                        break;
                }

                _appData.KeypadCommandsList[_scriptHandler.TimesRepeated].Changed = false;
            }
        }

        //public static void Response_Program_Keypad_Commands(MessageStructure msg)
        //{
        //    if (_scriptHandler.TimesRepeated < _appData.KeypadCommandsList.Count)
        //    {
        //        if (_appData.UploadOnlyModified == true)
        //        {
        //            for (int i = _scriptHandler.TimesRepeated + 1; i < _appData.KeypadCommandsList.Count; i++)
        //            {
        //                if (_appData.KeypadCommandsList[i].Changed)
        //                {
        //                    _scriptHandler.TimesRepeated = i - 1;
        //                    _scriptHandler.CurrentElement -= 1;
        //                    return;
        //                }
        //            }
        //        }
        //        else
        //        {
        //            _scriptHandler.CurrentElement -= 1;
        //            return;
        //        }
        //    }
        //}

        public static void Response_Program_Keypad_Groups(MessageStructure msg)
        {
            switch (_appData.UploadType)
            {
                case AppData.Upload_Type.UPLOAD_ALL:
                    if ((_scriptHandler.TimesRepeated+1) * 10 < _appData.KeypadCommandsList.Count)
                        {
                            _scriptHandler.CurrentElement -= 1;
                            return;
                        }
                        break;
                case AppData.Upload_Type.UPLOAD_BUTTONS_WITH_VALUES:
                    for (int i = (_scriptHandler.TimesRepeated + 1) * 10; i < _appData.KeypadCommandsList.Count; i++)
                    {
                        if (_appData.KeypadCommandsList[i].OnPressAction != KeypadButtonAction.NO_ACTION || _appData.KeypadCommandsList[i].OnHoldAction != KeypadButtonAction.NO_ACTION || _appData.KeypadCommandsList[i].OnReleaseAction != KeypadButtonAction.NO_ACTION)
                        {
                            _scriptHandler.TimesRepeated = (i / 10) - 1;
                            _scriptHandler.CurrentElement -= 1;
                            return;
                        }
                    }
                    break;
                default:
                    for (int i = (_scriptHandler.TimesRepeated + 1) * 10; i < _appData.KeypadCommandsList.Count; i++)
                    {
                        if (_appData.KeypadCommandsList[i].Changed)
                        {
                            _scriptHandler.TimesRepeated = (i / 10) - 1;
                            _scriptHandler.CurrentElement -= 1;
                            return;
                        }
                    }
                    break;
            }

            foreach (KeypadCommand cmd in _appData.KeypadCommandsList)
            {
                cmd.Changed = false;
            }
        }

        public static void Response_Retrieve_Keypad_Commands(MessageStructure msg)
        {
            if (msg.Data[0] - 1 < 0)
            {
                _appData.DisplayError($"Retrieved invalid button number {msg.Data[0]}. Button number must be greater than 0", "Invalid Button Number");
                _scriptHandler.CancelScript();
                return;
            }

            KeypadCommand newCommand = new KeypadCommand(msg.Data[0] - 1);

            //ON PRESS
            newCommand.OnPressAction = (KeypadButtonAction)(msg.Data[1]);    //On press

            if (msg.Data[2] != 0xFF && msg.Data[3] != 0xFF)
            {
                newCommand.OnPressTarget = ((msg.Data[3] << 8) | msg.Data[2]).ToString();
            }
            else
            {
                newCommand.OnPressTarget = "0";
            }

            if (newCommand.OnPressAction == KeypadButtonAction.NO_ACTION)
            {
                newCommand.OnPressTarget = "0";
            }

            //ON HOLD
            newCommand.OnHoldAction = (KeypadButtonAction)(msg.Data[4]);    //On Hold

            if (msg.Data[5] != 0xFF && msg.Data[6] != 0xFF)
            {
                newCommand.OnHoldTarget = ((msg.Data[6] << 8) | msg.Data[5]).ToString();
            }
            else
            {
                newCommand.OnHoldTarget = "0";
            }

            if (newCommand.OnHoldAction == KeypadButtonAction.NO_ACTION)
            {
                newCommand.OnHoldTarget = "0";
            }

            //ON RELEASE
            newCommand.OnReleaseAction = (KeypadButtonAction)(msg.Data[7]);    //On Release

            if (msg.Data[8] != 0xFF && msg.Data[9] != 0xFF)
            {
                newCommand.OnReleaseTarget = ((msg.Data[9] << 8) | msg.Data[8]).ToString();
            }
            else
            {
                newCommand.OnReleaseTarget = "0";
            }

            if (newCommand.OnReleaseAction == KeypadButtonAction.NO_ACTION)
            {
                newCommand.OnReleaseTarget = "0";
            }

            //GETTING THE TARGET ADDRESS//
            if (newCommand.TargetMotor == TargetMotor.SPECIFIC_MOTOR)
            {
                newCommand.TargetAddress = BitConverter.ToString(new byte[] { msg.Data[10], msg.Data[11], msg.Data[12] }).Replace("-", "");
            }

            if (msg.Data[0] - 1 < _appData.KeypadCommandsList.Count)
            {
                newCommand.Changed = true;
            }

            _appData.AddNewKeypadCommand(newCommand);

            if (_scriptHandler.TimesRepeated < _appData.KeypadCommandsList.Count - 1)
            {
                _scriptHandler.CurrentElement -= 1;
            }

            //KeypadCommand newCommand = new KeypadCommand(msg.Data[0] - 1);
            //GETTING BUTTON FUNCTION//
            //newCommand.OnPressAction = (KeypadButtonAction)(msg.Data[1]);    //On press
            //newCommand.OnHoldAction = (KeypadButtonAction)(msg.Data[8]);    //On Hold
            //newCommand.OnReleaseAction = (KeypadButtonAction)(msg.Data[15]);    //On Release

            //GETTING DIS OR POS//
            //if (msg.Data[2] != 0xFF && msg.Data[3] != 0xFF)
            //{
            //    newCommand.OnPressTarget = ((msg.Data[3] << 8) | msg.Data[2]).ToString();
            //}
            //else
            //{
            //    newCommand.OnPressTarget = "0";
            //}

            //if (msg.Data[16] != 0xFF && msg.Data[17] != 0xFF)
            //{
            //    newCommand.OnHoldTarget = ((msg.Data[10] << 8) | msg.Data[9]).ToString();
            //}
            //else
            //{
            //    newCommand.OnHoldTarget = "0";
            //}

            //if (msg.Data[9] != 0xFF && msg.Data[10] != 0xFF)
            //{
            //    newCommand.OnReleaseTarget = ((msg.Data[17] << 8) | msg.Data[16]).ToString();
            //}
            //else
            //{
            //    newCommand.OnReleaseTarget = "0";
            //}

            //if (newCommand.OnPressAction == KeypadButtonAction.NO_ACTION)
            //{
            //    newCommand.OnPressTarget = "0";
            //}

            //if (newCommand.OnHoldAction == KeypadButtonAction.NO_ACTION)
            //{
            //    newCommand.OnHoldTarget = "0";
            //}

            //if (newCommand.OnReleaseAction == KeypadButtonAction.NO_ACTION)
            //{
            //    newCommand.OnReleaseTarget = "0";
            //}

            //GETTING TARGET TYPE//
            //newCommand.TargetMotor = (TargetMotor)msg.Data[4];

            ////GETTING THE TARGET ADDRESS//
            //if (newCommand.TargetMotor == TargetMotor.SPECIFIC_MOTOR)
            //{
            //    newCommand.TargetAddress = BitConverter.ToString(new byte[] { msg.Data[7], msg.Data[6], msg.Data[5] }).Replace("-", "");
            //}

            //if (msg.Data[0] - 1 < _appData.KeypadCommandsList.Count)
            //{
            //    newCommand.Changed = true;
            //}

            //_appData.AddNewKeypadCommand(newCommand);

            //if (_scriptHandler.TimesRepeated < _appData.KeypadCommandsList.Count - 1)
            //{
            //    _scriptHandler.CurrentElement -= 1;
            //}
        }

        public static void Response_Retrieve_Group_Addresses(MessageStructure msg)
        {
            if (msg.Command[0] == 0x5C || _appData.KeypadCommandsList.Count < 10)
            {
                for (int i = 0; i < 8; i++)
                {
                    if (_appData.KeypadCommandsList[i + (10 * _scriptHandler.TimesRepeated)].TargetMotor == TargetMotor.SPECIFIC_GROUP)
                    {
                        _appData.KeypadCommandsList[i + (10 * _scriptHandler.TimesRepeated)].TargetAddress = BitConverter.ToString(new byte[] { (msg.Data[(i * 3) + 2]), msg.Data[(i * 3) + 1], msg.Data[(i * 3)] }).Replace("-", "");
                    }
                }
            }
            else
            {
                for (int i = 0; i < 10; i++)
                {
                    if (_appData.KeypadCommandsList[i + (10 * _scriptHandler.TimesRepeated)].TargetMotor == TargetMotor.SPECIFIC_GROUP)
                    {
                        _appData.KeypadCommandsList[i + (10 * _scriptHandler.TimesRepeated)].TargetAddress = BitConverter.ToString(new byte[] { (msg.Data[(i * 3) + 3]), msg.Data[(i * 3) + 2], msg.Data[(i * 3) + 1] }).Replace("-", "");
                    }
                }
            }

            if (10 * (_scriptHandler.TimesRepeated + 1) < _appData.KeypadCommandsList.Count - 1)
            {
                _scriptHandler.CurrentElement -= 1;
            }
        }

        public static void Response_Set_Keypad_Name(MessageStructure msg)
        {
            return;
        }

        public static void Response_Get_Keypad_Name(MessageStructure msg)
        {
            string keypadName = "";
            List<byte> label = new List<byte>();
            bool labelPresent = false;

            for (int i = 0; i < 16; i++)
            {
                if (msg.Data[i] != 0x00)
                {
                    label.Add(msg.Data[i]);
                    labelPresent = true;
                }
            }

            if (labelPresent == false)
            {
                keypadName = "Unnamed Keypad";
            }
            else
            {
                keypadName = System.Text.Encoding.ASCII.GetString(label.ToArray());
            }

            _appData.GetDeviceByAddress(msg.Sender).NameAsString = keypadName;
            _appData.UpdateKeypadList();
        }

        ///<summary>
        /// SETTINGS-RELATED FUNCTIONS
        /// </summary>
        /// 

        public static void Response_Retrieve_Ip_Address(MessageStructure msg)
        {
            int[] ip = new int[4];
            int[] subnet = new int[4];
            bool isDhcp = false;
            int port;

            if (msg.Data[0] == 0x01)
            {
                isDhcp = true;
            }

            port = (msg.Data[2] << 8) + msg.Data[1];

            for (int i = 0; i < 4; i++)
            {
                ip[i] = msg.Data[6-i];
                subnet[i] = msg.Data[10 - i];
            }

            OnIPRetrieved?.Invoke(null, new IPRetrieveEventArgs(ip, subnet, isDhcp, port));
        }

        public static void Response_Set_Server_Ip(MessageStructure msg)
        {
            int e = new int();
            OnRequestIPListCount?.Invoke(null, e);

            if(e >= _scriptHandler.TimesRepeated - 1)
            {
                _scriptHandler.CurrentElement -= 1;
            }
        }
    }
}
