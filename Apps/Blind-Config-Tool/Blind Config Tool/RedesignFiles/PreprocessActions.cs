using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Blind_Config_Tool.Core.Converters;

namespace Blind_Config_Tool.RedesignFiles
{
    public static class PreprocessActions
    {
        static AppData _appData;
        static ScriptHandler _scriptHandler;

        static int validationCheckTime = 100;

        #region Events
        public static event EventHandler<StringRequestEventArgs> OnRequestNewMotorName;
        public static event EventHandler<StringRequestEventArgs> OnRequestNewMotorAddress;
        public static event EventHandler<StringRequestEventArgs> OnRequestNewKeypadAddress;
        public static event EventHandler<SpeedsRequestEventArgs> OnRequestSpeeds;
        public static event EventHandler<DirectionRequestEventArgs> OnRequestDirection;
        public static event EventHandler<TargetRequestEventArgs> OnTargetRequest;
        public static event EventHandler<GroupIDRequestArgs> OnGroupIDRequest;
        public static event EventHandler<IPRequestArgs> OnIPRequest;
        public static event EventHandler<CurrentIPRequestArgs> OnCurrentIPRequest;
        public static event EventHandler<CurrentIPRequestArgs> OnIPMoveTargetRequest;
        public static event EventHandler<ServerIPRequestArgs> OnServerIPRequest;

        public class StringRequestEventArgs : EventArgs
        {
            public string StringValue { get; set; }
        }
        public class SpeedsRequestEventArgs : EventArgs
        {
            public int UpValue { get; set; }
            public int DownValue { get; set; }
            public int SlowValue { get; set; }
        }

        
        public class DirectionRequestEventArgs : EventArgs
        {
            public byte Direction { get; set; }
        }

        public class TargetRequestEventArgs : EventArgs
        {
            public int target { get; set; }
        }

        public class GroupIDRequestArgs : EventArgs
        {
            public int ID { get; set;}
            public string address { get; set; }
        }

        public class IPRequestArgs : EventArgs
        {
            public int ID { get; set; }
            public string IP { get; set; }
        }

        public class CurrentIPRequestArgs : EventArgs
        {
            public int IP { get; set; }
        }

        public class ServerIPRequestArgs : EventArgs
        {
            public int[] IP { get; set; }
            public int[] Subnet { get; set; }
            public int Port { get; set; }
        }
        #endregion

        #region Constructor
        static PreprocessActions()
        {
            _appData = AppData.Instance;
            _scriptHandler = ScriptHandler.Instance;
        }
        #endregion

        /// <summary>
        /// MOTOR FUNCTIONS
        /// </summary>
        /// 

        public static MessageStructure Preprocess_Search_For_Motors(MessageStructure msg)
        {
            if (_appData.ResetValidationOnSearch)
            {
                foreach(Motor mot in _appData.Motors)
                {
                    mot.Validated = false;
                }
            }

            _appData.SetTimer(AppData.Timers.VALIDATION_CHECK, validationCheckTime);

            return msg;
        }

        public static MessageStructure Preprocess_Add_Motors_To_List(MessageStructure msg)
        {
            _scriptHandler.TimesRepeated = 0;

            foreach (Motor mot in _appData.Motors)
            {
                mot.Validated = false;
            }

            return msg;
        }

        public static MessageStructure Preprocess_Set_Motor_Address(MessageStructure msg)
        {
            var args = new StringRequestEventArgs();
            OnRequestNewMotorAddress?.Invoke(null, args);  //Send the event to request the data from the textbox

            //Add the new address to the message
            msg.Data = AssistantFunctions.StringToByteArray(args.StringValue);

            return msg;
        }

        public static MessageStructure Preprocess_Set_Label(MessageStructure msg)
        {
            var args = new StringRequestEventArgs();
            OnRequestNewMotorName?.Invoke(null, args);  //Send the event to request the data from the textbox

            byte[] name = new byte[0];

            if (args.StringValue != null)
            {
                name = System.Text.Encoding.UTF8.GetBytes(args.StringValue);
            }

            for (int i = 0; i < 16; i++)
            {
                if (name.Length > i)
                {
                    msg.Data[i] = name[i];
                }
                else
                {
                    msg.Data[i] = 0xFF;
                }
            }

            return msg;
        }

        public static MessageStructure Preprocess_Set_Speeds(MessageStructure msg)
        {
            var args = new SpeedsRequestEventArgs();
            OnRequestSpeeds?.Invoke(null, args);

            switch (msg.NodeID)
            {
                default:
                    msg.Data[0] = (byte)args.UpValue;
                    msg.Data[1] = (byte)args.UpValue;
                    msg.Data[2] = (byte)args.SlowValue;
                    break;
            }

            return msg;
        }

        public static MessageStructure Preprocess_Set_Orientation(MessageStructure msg)
        {
            var args = new DirectionRequestEventArgs();
            OnRequestDirection?.Invoke(null, args);

            msg.Data[0] = args.Direction;

            return msg;
        }

        public static MessageStructure Preprocess_Move_To_Target(MessageStructure msg)
        {
            var args = new TargetRequestEventArgs();
            OnTargetRequest?.Invoke(null, args);

            msg.Data[0] = (byte)args.target;

            return msg;
        }

        public static MessageStructure Preprocess_Move_To_Selected_Ip(MessageStructure msg)
        {
            var args = new CurrentIPRequestArgs();
            OnIPMoveTargetRequest?.Invoke(null, args);

            msg.Data[0] = (byte)args.IP;

            return msg;
        }

        public static MessageStructure Preprocess_Get_Group_IDs(MessageStructure msg)
        {
            msg.Data[0] = (byte)(_scriptHandler.TimesRepeated);

            return msg;
        }

        public static MessageStructure Preprocess_Set_Group_ID(MessageStructure msg)
        {
            GroupIDRequestArgs args = new GroupIDRequestArgs();
            args.ID = _scriptHandler.TimesRepeated;
            OnGroupIDRequest?.Invoke(null, args);

            msg.Data[0] = (byte)_scriptHandler.TimesRepeated;

            byte[] addr = AssistantFunctions.StringToByteArray(args.address);

            msg.Data[1] = addr[0];
            msg.Data[2] = addr[1];
            msg.Data[3] = addr[2];

            return msg;
        }

        public static MessageStructure Preprocess_Get_Internal_Positions(MessageStructure msg)
        {
            msg.Data[0] = (byte)(_scriptHandler.TimesRepeated + 1);

            return msg;
        }

        public static MessageStructure Preprocess_Set_Internal_Positions(MessageStructure msg)
        {
            IPRequestArgs args = new IPRequestArgs();
            args.ID = _scriptHandler.TimesRepeated;
            OnIPRequest?.Invoke(null, args);

            msg.Data[1] = (byte)(_scriptHandler.TimesRepeated+1);
            msg.Data[2] = (byte)int.Parse(args.IP);
            return msg;
        }

        public static MessageStructure Preprocess_Set_Ip_To_Current_Position(MessageStructure msg)
        {
            CurrentIPRequestArgs args = new CurrentIPRequestArgs();
            OnCurrentIPRequest?.Invoke(null, args);

            msg.Data[1] = (byte)(args.IP);

            return msg;
        }

        public static MessageStructure Preprocess_Get_Single_Internal_Position(MessageStructure msg)
        {
            CurrentIPRequestArgs args = new CurrentIPRequestArgs();
            OnCurrentIPRequest?.Invoke(null, args);

            msg.Data[0] = (byte)(args.IP);

            return msg;
        }
        /// <summary>
        /// KEYPAD FUNCTIONS
        /// </summary>
        /// 

        public static MessageStructure Preprocess_Search_For_Keypads(MessageStructure msg)
        {
            if (_appData.ResetValidationOnSearch)
            {
                foreach (Keypad keypad in _appData.Keypads)
                {
                    keypad.Validated = false;
                }
            }

            _appData.SetTimer(AppData.Timers.VALIDATION_CHECK, validationCheckTime);

            return msg;
        }

        public static MessageStructure Preprocess_Add_Keypads_To_List(MessageStructure msg)
        {
            foreach (Keypad pad in _appData.Keypads)
            {
                pad.Validated = false;
            }

            return msg;
        }

        public static MessageStructure Preprocess_Set_Keypad_Address(MessageStructure msg)
        {
            var args = new StringRequestEventArgs();
            OnRequestNewKeypadAddress?.Invoke(null, args);  //Send the event to request the data from the textbox

            //Add the new address to the message
           msg.Data = AssistantFunctions.StringToByteArray(args.StringValue);

            return msg;
        }

        public static MessageStructure Preprocess_Program_Keypad_Commands(MessageStructure msg)
        {
            // Start at the correct keypad command index.
            // Set _scriptHandler.TimesRepeated to either the first modified value or the first button with value.
            if (_scriptHandler.TimesRepeated == 0 && (_appData.UploadType == AppData.Upload_Type.UPLOAD_MODIFIED || _appData.UploadType == AppData.Upload_Type.UPLOAD_BUTTONS_WITH_VALUES))
            {
                for(int i = 0; i < _appData.KeypadCommandsList.Count; i++)
                {
                    if(_appData.UploadType == AppData.Upload_Type.UPLOAD_MODIFIED && _appData.KeypadCommandsList[i].Changed == true)
                    {
                        _scriptHandler.TimesRepeated = i;
                        break;
                    }

                    if (_appData.UploadType == AppData.Upload_Type.UPLOAD_BUTTONS_WITH_VALUES && (_appData.KeypadCommandsList[i].OnPressAction != Core.KeypadButtonAction.NO_ACTION || _appData.KeypadCommandsList[i].OnHoldAction != Core.KeypadButtonAction.NO_ACTION || _appData.KeypadCommandsList[i].OnReleaseAction != Core.KeypadButtonAction.NO_ACTION))
                    {
                        _scriptHandler.TimesRepeated = i;
                        break;
                    }
                }
            }

            // Button number
            msg.Data[0] = (byte)(_scriptHandler.TimesRepeated + 1);

            //ON PRESS
            msg.Data[1] = (byte)MotorMoveCommandConverter.ConvertTo(_appData.KeypadCommandsList[_scriptHandler.TimesRepeated].OnPressAction); 
            msg.Data[2] = _appData.KeypadCommandsList[_scriptHandler.TimesRepeated].OnPressTargetAsBytes[0];
            msg.Data[3] = _appData.KeypadCommandsList[_scriptHandler.TimesRepeated].OnPressTargetAsBytes[1];

            //ON HOLD
            msg.Data[4] = (byte)MotorMoveCommandConverter.ConvertTo(_appData.KeypadCommandsList[_scriptHandler.TimesRepeated].OnHoldAction);
            msg.Data[5] = _appData.KeypadCommandsList[_scriptHandler.TimesRepeated].OnHoldTargetAsBytes[0];
            msg.Data[6] = _appData.KeypadCommandsList[_scriptHandler.TimesRepeated].OnHoldTargetAsBytes[1];

            //ON RELEASE
            msg.Data[7] = (byte)MotorMoveCommandConverter.ConvertTo(_appData.KeypadCommandsList[_scriptHandler.TimesRepeated].OnReleaseAction);
            msg.Data[8] = _appData.KeypadCommandsList[_scriptHandler.TimesRepeated].OnReleaseTargetAsBytes[0];
            msg.Data[9] = _appData.KeypadCommandsList[_scriptHandler.TimesRepeated].OnReleaseTargetAsBytes[1];
                
            byte[] addr = new byte[3] { 0x00, 0x00, 0x00 };

            //SETTING TARGET ADDRESS//
            if (_appData.KeypadCommandsList[_scriptHandler.TimesRepeated].TargetAddress != null && _appData.KeypadCommandsList[_scriptHandler.TimesRepeated].TargetAddress.Length == 6)
            {
                addr = AssistantFunctions.StringToByteArray(_appData.KeypadCommandsList[_scriptHandler.TimesRepeated].TargetAddress);
            }

            msg.Data[10] = addr[0];
            msg.Data[11] = addr[1];
            msg.Data[12] = addr[2];

            return msg;

            //SETTING COMMAND TYPES (e.g. up, down, go to IP, etc.)//
            //msg.Data[1] = (byte)_appData.KeypadCommandsList[_scriptHandler.TimesRepeated].OnPressAction;      //Press
            //msg.Data[8] = (byte)_appData.KeypadCommandsList[_scriptHandler.TimesRepeated].OnHoldAction;       //Hold
            //msg.Data[15] = (byte)_appData.KeypadCommandsList[_scriptHandler.TimesRepeated].OnReleaseAction;    //Release

            //SETTING DISTANCE OR POS//
            //if (_appData.KeypadCommandsList[_scriptHandler.TimesRepeated].OnPressTargetAsBytes[0] == 0 && _appData.KeypadCommandsList[_scriptHandler.TimesRepeated].OnPressTargetAsBytes[1] == 0)
            //{
            //    msg.Data[2] = 0x01;
            //}
            //else
            //{
            //    msg.Data[2] = _appData.KeypadCommandsList[_scriptHandler.TimesRepeated].OnPressTargetAsBytes[0];       //Press
            //}
            //msg.Data[3] = _appData.KeypadCommandsList[_scriptHandler.TimesRepeated].OnPressTargetAsBytes[1];

            //if (_appData.KeypadCommandsList[_scriptHandler.TimesRepeated].OnHoldTargetAsBytes[0] == 0 && _appData.KeypadCommandsList[_scriptHandler.TimesRepeated].OnHoldTargetAsBytes[1] == 0)
            //{
            //    msg.Data[9] = 0x01;
            //}
            //else
            //{
            //    msg.Data[9] = _appData.KeypadCommandsList[_scriptHandler.TimesRepeated].OnHoldTargetAsBytes[1];        //Hold
            //}
            //msg.Data[10] = _appData.KeypadCommandsList[_scriptHandler.TimesRepeated].OnHoldTargetAsBytes[0];

            //if (_appData.KeypadCommandsList[_scriptHandler.TimesRepeated].OnReleaseTargetAsBytes[0] == 0 && _appData.KeypadCommandsList[_scriptHandler.TimesRepeated].OnReleaseTargetAsBytes[1] == 0)
            //{
            //    msg.Data[16] = 0x01;
            //}
            //else
            //{
            //    msg.Data[16] = _appData.KeypadCommandsList[_scriptHandler.TimesRepeated].OnReleaseTargetAsBytes[1];     //Release
            //}
            //msg.Data[17] = _appData.KeypadCommandsList[_scriptHandler.TimesRepeated].OnReleaseTargetAsBytes[0];

            //SETTING TARGET TYPE//
            //msg.Data[4] = (byte)_appData.KeypadCommandsList[_scriptHandler.TimesRepeated].TargetMotor;
            //msg.Data[11] = (byte)_appData.KeypadCommandsList[_scriptHandler.TimesRepeated].TargetMotor;
            //msg.Data[18] = (byte)_appData.KeypadCommandsList[_scriptHandler.TimesRepeated].TargetMotor;

            //byte[] addr = new byte[3] { 0x00, 0x00, 0x00 };

            ////SETTING TARGET ADDRESS//
            //if (_appData.KeypadCommandsList[_scriptHandler.TimesRepeated].TargetAddress != null && _appData.KeypadCommandsList[_scriptHandler.TimesRepeated].TargetAddress.Length == 6)
            //{
            //    addr = AssistantFunctions.StringToByteArray(_appData.KeypadCommandsList[_scriptHandler.TimesRepeated].TargetAddress);
            //}

            //msg.Data[5] = addr[2];
            //msg.Data[6] = addr[1];
            //msg.Data[7] = addr[0];

            //msg.Data[12] = addr[2];
            //msg.Data[13] = addr[1];
            //msg.Data[14] = addr[0];

            //msg.Data[19] = addr[2];
            //msg.Data[20] = addr[1];
            //msg.Data[21] = addr[0];

            //return msg;
        }

        public static MessageStructure Preprocess_Program_Keypad_Groups(MessageStructure msg)
        {
            if (_scriptHandler.TimesRepeated == 0 && (_appData.UploadType == AppData.Upload_Type.UPLOAD_MODIFIED || _appData.UploadType == AppData.Upload_Type.UPLOAD_BUTTONS_WITH_VALUES))
            {
                for (int i = 0; i < _appData.KeypadCommandsList.Count; i++)
                {
                    if (_appData.UploadType == AppData.Upload_Type.UPLOAD_MODIFIED && _appData.KeypadCommandsList[i].Changed == true)
                    {
                        _scriptHandler.TimesRepeated = i / 10;
                        break;
                    }

                    if (_appData.UploadType == AppData.Upload_Type.UPLOAD_BUTTONS_WITH_VALUES && (_appData.KeypadCommandsList[i].OnPressAction != Core.KeypadButtonAction.NO_ACTION || _appData.KeypadCommandsList[i].OnHoldAction != Core.KeypadButtonAction.NO_ACTION || _appData.KeypadCommandsList[i].OnReleaseAction != Core.KeypadButtonAction.NO_ACTION))
                    {
                        _scriptHandler.TimesRepeated = i / 10;
                        break;
                    }
                }
            }

            if (_appData.KeypadCommandsList.Count == 8)
            {
                for (int i = 0; i < 8; i++)
                {
                    byte[] addr = new byte[3] { 0x00, 0x00, 0x00 };

                    //SETTING TARGET ADDRESS//
                    if (_appData.KeypadCommandsList[i].TargetAddress != null && _appData.KeypadCommandsList[i].TargetAddress.Length == 6)
                    {
                        addr = AssistantFunctions.StringToByteArray(_appData.KeypadCommandsList[i].TargetAddress);
                    }

                    msg.Data[(i * 3)] = (byte)(addr[2]);
                    msg.Data[(i * 3) + 1] = (byte)(addr[1]);
                    msg.Data[(i * 3) + 2] = (byte)(addr[0]);
                }
            }
            else if (_appData.KeypadCommandsList.Count == 100)
            {
                msg.Command = new byte[] { 0x85 };

                msg.Data = new byte[31];

                msg.Data[0] = (byte)(_scriptHandler.TimesRepeated + 1);

                for (int i = 0; i < 10; i++)
                {
                    byte[] addr = new byte[3] { 0x00, 0x00, 0x00 };

                    //SETTING TARGET ADDRESS//
                    if (_appData.KeypadCommandsList[(10 * _scriptHandler.TimesRepeated) + i].TargetAddress != null && _appData.KeypadCommandsList[(10 * _scriptHandler.TimesRepeated) + i].TargetAddress.Length == 6)
                    {
                        addr = AssistantFunctions.StringToByteArray(_appData.KeypadCommandsList[(10 * _scriptHandler.TimesRepeated) + i].TargetAddress);
                    }

                    msg.Data[1 + (i * 3)] = (byte)(addr[2]);
                    msg.Data[2 + (i * 3)] = (byte)(addr[1]);
                    msg.Data[3 + (i * 3)] = (byte)(addr[0]);
                }
            }

            return msg;
        }

        public static MessageStructure Preprocess_Retrieve_Keypad_Commands(MessageStructure msg)
        {
            msg.Data[0] = (byte)(_scriptHandler.TimesRepeated + 1);

            return msg;
        }

        public static MessageStructure Preprocess_Retrieve_Group_Addresses(MessageStructure msg)
        {
            if (_appData.KeypadCommandsList.Count >= 10)
            {
                msg.Command[0] = 0x84;
                msg.Data[0] = (byte)(_scriptHandler.TimesRepeated + 1);
            }

            return msg;
        }

        public static MessageStructure Preprocess_Set_Keypad_Name(MessageStructure msg)
        {
            byte[] name = Encoding.UTF8.GetBytes(_appData.NewKeypadName);

            for (int i = 0; i < 16; i++)
            {
                if (name.Length > i)
                {
                    msg.Data[i] = name[i];
                }
                else
                {
                    msg.Data[i] = 0x00;
                }
            }

            return msg;
        }

        /// <summary>
        /// SERIAL FUNCTIONS
        /// </summary>
        /// 
        public static MessageStructure Preprocess_Set_Ip_Address(MessageStructure msg)
        {
            msg.Data[0] = 0x00;

            if (_appData.LastSentDHCP)
            {
                msg.Data[0] = 0x01;
            }

            msg.Data[2] = ((byte)((_appData.LastSentPort >> 8) & 0xFF));
            msg.Data[1] = ((byte)(_appData.LastSentPort & 0xFF));
            
            for (int i = 0; i < 4; i++)
            {
                msg.Data[6 - i] = (byte)_appData.LastSentIP[i];
                msg.Data[10 - i] = (byte)_appData.LastSentSubnet[i];
            }

            return msg;
        }

        public static MessageStructure Preprocess_Set_Server_Ip(MessageStructure msg)
        {
            var args = new ServerIPRequestArgs();
            OnServerIPRequest?.Invoke(null, args);  //Send the event to request the data from the textbox

            msg.Data[0] = (byte)_scriptHandler.TimesRepeated;

            for (int i = 0; i < 4; i++)
            {
                msg.Data[1 + i] = (byte)args.IP[i];
                msg.Data[5 + i] = (byte)args.Subnet[i];
            }

            msg.Data[9] = ((byte)(args.Port & 0xFF));
            msg.Data[10] = ((byte)((args.Port >> 8) & 0xFF));

            return msg;
        }
    }
}
