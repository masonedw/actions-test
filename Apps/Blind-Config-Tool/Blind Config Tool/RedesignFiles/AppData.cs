using Blind_Config_Tool.Core;
using System;
using System.Collections.Generic;
using System.Windows;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.TaskbarClock;
using System.Threading;
using System.Collections.ObjectModel;
using System.Security.RightsManagement;
using System.Linq;
using System.Data;
using System.Security.Cryptography;

namespace Blind_Config_Tool.RedesignFiles
{
    public class AppData
    {
        #region Declarations
        //Singleton
        private static AppData _instance;    //The single instance of this class
        private static readonly object _lock = new object();    //The lock (used to ensure that only one instance of this class is created)

        //App Info
        byte[] _appAddress = new byte[3] { 0x0FA, 0x00, 0x01 };
        bool _connection_serial = true;

        //Devices
        List<Motor> _motors;   //The list of motors that have been found by searching
        int _selectedMotor;     //The motor that has been selected
        string _newMotorName;

        List<Keypad> _keypads;  //The list of keypads that have been found by searching
        int _selectedKeypad;    //The keypad that has been selected
        ObservableCollection<KeypadCommand> _keypadCommands;
        Upload_Type _uploadType = Upload_Type.UPLOAD_MODIFIED;    //If true, only keypad commands that have been changed will be uploaded
        byte[] _motorAllAddress;   //The motor all address
        string _newKeypadName;  //The new keypad name that will be set

        List<DeviceData> _devices;  //The list of all devices
        int _selectedDevice;    //The currently selected device
        bool _resetValidationOnSearch = true;

        //Timers
        ActionTimer _timer_hideCommandDescription;              //The timer that hides the label displaying the current process 
        ActionTimer _timer_intervalTimer;                       //Timer to handle when we want to begin reqding response messages
        ActionTimer _timer_errorDisplayTimer;                   //Timer to show the error box
        ActionTimer _timer_MotorUpMovementRepeatTimer;          //Timer to repeat the up command
        ActionTimer _timer_MotorDownMovementRepeatTimer;        //Timer to repeat the down command
        ActionTimer _timer_MotorVisualFeedbackTimer;            //Timer to repeat the visual feedback movement
        ActionTimer _timer_ValidationCheckTimer;                //Timer for deciding whether to reset device validation
        bool _autoHideErrors;                                   //If true, the system will automatically hide the error box after a set amount of time has passed

        //IP Info
        int[] _lastSentIP;
        int[] _lastSentSubnet;
        bool _lastSentDHCPState;
        int _lastSentPort;

        int _selectedSortType;

        public static int SERIAL_READ_INTERVAL_TIME = 80;

        public enum Timers
        {
            PRIVATE_TIMER,
            HIDE_COMMAND_DESC,
            INTERVAL_DONE,
            HIDE_ERROR_DISPLAY,
            UP_MOVEMENT,
            DOWN_MOVEMENT,
            VISUAL_FEEDBACK,
            VALIDATION_CHECK,
            TEST
        }

        public enum Upload_Type
        {
            UPLOAD_ALL,
            UPLOAD_MODIFIED,
            UPLOAD_BUTTONS_WITH_VALUES
        }
        #endregion

        #region Events
        public event Action<Timer> OnTimerComplete;

        public event Action<string> OnCommandDescriptionUpdated;
        public event Action OnIntervalTimerDone;

        public event Action OnKeypadListUpdated;
        public event Action OnKeypadCommandsListUpdated;

        public event Action OnMotorListUpdated;
        public event Action OnRefreshSelectedMotor;
        public event Action OnRefreshGroupsAndIPs;

        public event Action OnDeviceListUpdated;

        public delegate void ErrorDisplayEventArgs(string title, string caption);
        public event ErrorDisplayEventArgs OnErrorDisplay;

        public event Action OnHideError;

        public event Action OnRetriggerUpMovement;
        public event Action OnRetriggerDownMovement;

        public event Action OnValidationTimerDone;
        #endregion

        #region Singleton Instantiation
        public static AppData Instance
        {
            get
            {
                if (_instance == null)   //Check whether an instance has been created and if not, try to create one
                {
                    lock (_lock) //Prevent the thread from continuing on if another thread is in the process of creating an instance
                    {
                        if (_instance == null)   //Second check inside the lock
                        {
                            _instance = new AppData();   //Create the instance
                        }
                    }
                }

                return _instance;
            }
        }
        #endregion

        #region Constructor
        public AppData()
        {
            _motors = new List<Motor>();
            _keypads = new List<Keypad>();
            _keypadCommands= new ObservableCollection<KeypadCommand>();

            Serial.Instance.SerialPort.DataReceived += ResetIntervalTimer;

            _timer_intervalTimer = new ActionTimer(Timers.INTERVAL_DONE, IntervalDone);
            _timer_hideCommandDescription = new ActionTimer(Timers.HIDE_COMMAND_DESC, HideCommandDescription);
            _timer_errorDisplayTimer = new ActionTimer(Timers.HIDE_ERROR_DISPLAY, HideErrorDisplay);
            _timer_MotorUpMovementRepeatTimer = new ActionTimer(Timers.UP_MOVEMENT, RestartUpTimer);
            _timer_MotorDownMovementRepeatTimer = new ActionTimer(Timers.DOWN_MOVEMENT, RestartDownTimer);
            _timer_MotorVisualFeedbackTimer = new ActionTimer(Timers.VISUAL_FEEDBACK, RestartVisualFeedbackTimer);
            _timer_ValidationCheckTimer = new ActionTimer(Timers.VALIDATION_CHECK, ValidationTimerDone);

            _lastSentIP = new int[4];
            _lastSentSubnet = new int[4];
        }
        #endregion

        #region Changing the app's address
        public void ChangeAppAddress(byte[] newAddress)
        {
            if(newAddress.Length != 3)
            {
                DisplayError($"New app address has wrong length.\n\nExpected length: 3\nGiven length: {newAddress.Length}", "Invalid Address Length");
                return;
            }

            _appAddress = newAddress;
        }
        #endregion

        #region Timer Functions
        public void SetTimer(Timers timer, int time)
        {
            if (IsTimerRunning(timer))
            {
                return;
            }

            switch (timer)
            {
                case Timers.HIDE_COMMAND_DESC:
                    _timer_hideCommandDescription.Start(time);
                    break;
                case Timers.INTERVAL_DONE:
                    _timer_intervalTimer.Start(time);
                    break;
                case Timers.HIDE_ERROR_DISPLAY:
                    _timer_errorDisplayTimer.Start(time);
                    break;
                case Timers.VISUAL_FEEDBACK:
                    _timer_MotorVisualFeedbackTimer.Start(time);
                    break;
                case Timers.UP_MOVEMENT:
                    _timer_MotorUpMovementRepeatTimer.Start(time);
                    break;
                case Timers.DOWN_MOVEMENT:
                    _timer_MotorDownMovementRepeatTimer.Start(time);
                    break;
                case Timers.VALIDATION_CHECK:
                    //_resetValidationOnSearch = false;
                    //_timer_ValidationCheckTimer.Start(time);
                    break;
                default:
                    DisplayError($"Invalid timer ({timer})", "Invalid Timer");
                    return;
            }
        }

        public void ResetIntervalTimer(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            if (_timer_intervalTimer.IsRunning)
            {
                //if(ScriptHandler.Instance.CurrentAction.ElementName == ElementActions.MOTOR_SEARCH || ScriptHandler.Instance.CurrentAction.ElementName == ElementActions.KEYPAD_SEARCH)
                //{
                //    _timer_intervalTimer.Start(500);
                //    return;
                //}

                _timer_intervalTimer.Start(SERIAL_READ_INTERVAL_TIME);
            }
        }

        public void CancelTimer(Timers timer)
        {
            switch(timer)
            {
                case Timers.HIDE_COMMAND_DESC:
                    _timer_hideCommandDescription.Stop();
                    break;
                case Timers.INTERVAL_DONE:
                    _timer_intervalTimer.Stop();
                    break;
                case Timers.HIDE_ERROR_DISPLAY:
                    _timer_errorDisplayTimer.Stop();
                    break;
                case Timers.VISUAL_FEEDBACK:
                    _timer_MotorVisualFeedbackTimer.Stop();
                    break;
                case Timers.UP_MOVEMENT:
                    _timer_MotorUpMovementRepeatTimer.Stop();
                    break;
                case Timers.DOWN_MOVEMENT:
                    _timer_MotorDownMovementRepeatTimer.Stop();
                    break;
                case Timers.VALIDATION_CHECK:
                    _resetValidationOnSearch = true;
                    _timer_ValidationCheckTimer.Stop();
                    break;
                default:
                    DisplayError($"Invalid timer ({timer})", "Invalid Timer");
                    break;
            }
        }

        public bool IsTimerRunning(Timers timer)
        {
            switch (timer)
            {
                case Timers.HIDE_COMMAND_DESC:
                    return _timer_hideCommandDescription.IsRunning;
                case Timers.INTERVAL_DONE:
                    return _timer_intervalTimer.IsRunning;
                case Timers.HIDE_ERROR_DISPLAY:
                    return _timer_errorDisplayTimer.IsRunning;
                case Timers.VISUAL_FEEDBACK:
                    return _timer_MotorVisualFeedbackTimer.IsRunning;
                case Timers.UP_MOVEMENT:
                    return _timer_MotorUpMovementRepeatTimer.IsRunning;
                case Timers.DOWN_MOVEMENT:
                    return _timer_MotorDownMovementRepeatTimer.IsRunning;
                case Timers.VALIDATION_CHECK:
                    return _timer_ValidationCheckTimer.IsRunning;
                default:
                    DisplayError($"Invalid timer ({timer})", "Invalid Timer");
                    return false;
            }
        }

        public void RestartUpTimer()
        {
            OnRetriggerUpMovement?.Invoke();
        }

        public void RestartDownTimer()
        {
            OnRetriggerDownMovement?.Invoke();
        }

        public void RestartVisualFeedbackTimer()
        {
            ScriptHandler.Instance.StartScript(Messages.START_IDENTIFICATION_MOVEMENT);
            SetTimer(Timers.VISUAL_FEEDBACK, 3000);
        }

        void HideCommandDescription()
        {
            OnCommandDescriptionUpdated?.Invoke("");
        }

        void IntervalDone()
        {
            OnIntervalTimerDone?.Invoke();
        }

        void ValidationTimerDone()
        {
            _resetValidationOnSearch = true;
        }

        public void HideErrorDisplay()
        {
            OnHideError?.Invoke();
        }
        #endregion

        #region Motor Functions
        public void AddNewMotor(byte[] address, byte[] node)
        {

            Motors.Add(new Motor(address, node));    //Create a new motor object and add it to the list
            SortMotorList();
        }

        public void RemoveMotor(int motorToRemove)
        {
            Motors.RemoveAt(motorToRemove);
            SortMotorList();
        }

        public void UpdateMotorList()
        {
            OnMotorListUpdated?.Invoke();
            OnDeviceListUpdated?.Invoke();
            _devices = Keypads.Cast<DeviceData>().Concat(Motors.Cast<DeviceData>()).ToList();
        }

        public void RefreshSelectedMotor()
        {
            OnRefreshSelectedMotor?.Invoke();
            _devices = Keypads.Cast<DeviceData>().Concat(Motors.Cast<DeviceData>()).ToList();
        }

        public void RefreshGroupsAndIPs()
        {
            OnRefreshGroupsAndIPs?.Invoke();
        }

        public void ClearMotorList()
        {
            while(Motors.Count > 0)
            {
                RemoveMotor(0);
            }

            RefreshSelectedMotor();
        }

        public void SortMotorList()
        {
            switch((SortType)SelectedSortType)
            {
                case SortType.NAME_ASCENDING:
                    Motors.Sort((x, y) => x.NameAsString.CompareTo(y.NameAsString));
                    break;
                case SortType.NAME_DESCENDING:
                    Motors.Sort((x, y) => y.NameAsString.CompareTo(x.NameAsString));
                    break;
                case SortType.ADDRESS_ASCENDING:
                    Motors.Sort((x, y) => x.AddressAsString.CompareTo(y.AddressAsString));
                    break;
                case SortType.ADDRESS_DESCENDING:
                    Motors.Sort((x, y) => y.AddressAsString.CompareTo(x.AddressAsString));
                    break;
            }

            UpdateMotorList();
        }
        #endregion

        #region Keypad Functions
        public void AddNewKeypad(byte[] address)
        {
            Keypads.Add(new Keypad(address));    //Create a new keypad object and add it to the list
            UpdateKeypadList();
        }

        public void RemoveKeypad(int keypadToRemove)
        {
            Keypads.RemoveAt(keypadToRemove);
            UpdateKeypadList();
        }

        public void UpdateKeypadList()
        {
            OnKeypadListUpdated?.Invoke();
            OnDeviceListUpdated?.Invoke();
            _devices = Keypads.Cast<DeviceData>().Concat(Motors.Cast<DeviceData>()).ToList();
        }

        public void AddNewKeypadCommand(KeypadCommand cmd)
        {
            while (KeypadCommandsList.Count < cmd.ID)
            {
                KeypadCommandsList.Add(new KeypadCommand(KeypadCommandsList.Count));
            }

            KeypadCommandsList[cmd.ID-1] = cmd;

            UpdateCommandList();
        }

        public void UpdateCommandGroupAddress(int id, string addr)
        {
            KeypadCommandsList[id].TargetAddress = addr;
            UpdateCommandList();
        }

        public void UpdateCommandList()
        {
            OnKeypadCommandsListUpdated?.Invoke();
        }

        public void ChangeNumberOfCommands(int number)
        {
            while(KeypadCommandsList.Count != number)
            {
                if(KeypadCommandsList.Count > number)
                {
                    KeypadCommandsList.RemoveAt(KeypadCommandsList.Count - 1);
                }
                else
                {
                    KeypadCommandsList.Add(new KeypadCommand(KeypadCommandsList.Count));
                }
            }

            UpdateCommandList();
        }
        #endregion

        public DeviceData GetDeviceByAddress(byte[] address)
        {
            foreach (DeviceData dev in Motors)
            {
                if (address[0] == dev.AddressAsBytes[0] && address[1] == dev.AddressAsBytes[1] && address[2] == dev.AddressAsBytes[2])
                {
                    return dev;
                }
            }

            foreach (DeviceData dev in Keypads)
            {
                if (address[0] == dev.AddressAsBytes[0] && address[1] == dev.AddressAsBytes[1] && address[2] == dev.AddressAsBytes[2])
                {
                    return dev;
                }
            }

            return null;
        }

        public void RemoveDeviceFromList(DeviceData dev)
        {
            if(dev == null)
            {
                return;
            }

            _devices.Remove(dev);

            if (dev is Motor motor)
            {
                _motors.Remove(motor);
                UpdateMotorList();
                _selectedMotor = 0;
            }
            else if (dev is Keypad keypad)
            {
                _keypads.Remove(keypad);
                UpdateKeypadList();
                _selectedKeypad = 0;
            }

            _selectedDevice = 0;
        }
        #region Error Display
        public void DisplayError(string title, string caption)
        {
            OnErrorDisplay?.Invoke(title, caption);
        }
        #endregion

        #region Conversions
        public byte[] StringToByteArray(string _string)                                                         //Converts a string to an array of bytes
        {
            try
            {
                if (_string.Length % 2 == 1)                                                                    //If the string has an odd number of characters
                {
                    _string = "0" + _string;                                                                    //Add an extra 0 onto it
                }

                byte[] byt = new byte[_string.Length];                                                          //Create a new byte array the size of the string
                byte[] hexval = new byte[_string.Length / 2];                                                   //Create a new byte array to hold the hex value (half the size of the normal string as each hex is 2 characters

                for (int i = 0; i < _string.Length; i++)                                                        //loop through all characters in the string
                {
                    byt[i] = Convert.ToByte(_string[i]);                                                        //Convert each character to a byte
                }

                for (int i = 0; i < hexval.Length; i++)                                                         //Loop through each hex value
                {
                    int a = i * 2;                                                                              //Go to the next hex value
                    hexval[i] = (byte)((byte)(ConvertASCIIToHex(byt[a]) * 16) + ConvertASCIIToHex(byt[a + 1])); //Convert each part to hex
                }

                return hexval;
            }
            catch
            {
                DisplayError("Could not convert string to byte array.", "Invalid Operation");

                return null;
            }
        }

        private byte ConvertASCIIToHex(byte _data)                                                      //Converts ASCII values to hex values
        {
            if (_data >= 48 && _data <= 57)                                                             //If the character is lowercase
            {
                return (byte)(_data - 48);
            }
            if (_data >= 65 && _data <= 70)                                                             //If the character is uppercase
            {
                return (byte)(_data - 55);
            }
            if (_data >= 97 && _data <= 102)                                                            //If the character is a special character
            {
                return (byte)(_data - 87);
            }
            else
            {
                return 0;
            }
        }
        #endregion

        #region Accessors
        public byte[] AppAddress { get { return _appAddress; } }
        public List<Motor> Motors { get { return _motors; } }
        public Motor SelectedMotor { get { return _motors[_selectedMotor >= 0 ? _selectedMotor : 0]; } }
        public int MotorIndex { get { return _selectedMotor; } set { _selectedMotor = value; } }
        public List<Keypad> Keypads { get { return _keypads; } }
        public Keypad SelectedKeypad { get { return _keypads[_selectedKeypad >= 0 ? _selectedKeypad : 0]; } }
        public int KeypadIndex { get { return _selectedKeypad; } set { _selectedKeypad = value; } }
        public ObservableCollection<KeypadCommand> KeypadCommandsList { get { return _keypadCommands; } set { _keypadCommands = value; } }
        public byte[] MotorAllAddress { get { return _motorAllAddress; } set { _motorAllAddress = value; } }
        public List<DeviceData> Devices { get { return _devices; } set { _devices = value; } }
        public int DeviceIndex { get { return _selectedDevice; } set { _selectedDevice = value; } }
        public DeviceData SelectedDevice { get { return _devices[_selectedDevice >= 0 ? _selectedDevice : 0]; } }
        
        public int[] LastSentIP { get { return _lastSentIP; } set { _lastSentIP = value; } }
        public int[] LastSentSubnet { get { return _lastSentSubnet; } set { _lastSentSubnet = value; } }
        public bool LastSentDHCP { get { return _lastSentDHCPState; } set { _lastSentDHCPState = value; } }
        public int LastSentPort { get { return _lastSentPort; } set { _lastSentPort = value; } }
        public string NewKeypadName { get { return _newKeypadName; } set { _newKeypadName = value; } }
        public Upload_Type UploadType { get { return _uploadType; } set { _uploadType = value; } }
        public int SelectedSortType { get { return _selectedSortType; } 
            set 
            { 
                _selectedSortType = value;
                SortMotorList();
            } 
        }
        public bool ResetValidationOnSearch { get { return _resetValidationOnSearch; } set { _resetValidationOnSearch = value; } }
        public int ValidationCheckTime { get { return 150; } }
        public bool Connection_Serial { get { return _connection_serial; } set { _connection_serial = value; } }
        #endregion
    }
}
