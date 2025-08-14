using Blind_Config_Tool.Core;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;

namespace Blind_Config_Tool.RedesignFiles.MVVM.ViewModel
{
    internal class VM_MotPage1 : ObservableObject
    {
        #region Events
        public static event EventHandler<string> ErrorDisplayed;
        public static event Action OnMotorPageChange;
        #endregion

        ScriptHandler _scriptHandler;
        AppData _appData;

        ObservableCollection<Motor> _motors;
        int _selectedMotor = -1;
        int _lastSelectedMotor = -1;
        bool _validMotor = false;

        string _motorLabel = "No Motor Selected";
        string _nodeType = "No Type";
        string _firmware = "Firmware: 0.0";

        string _newMotorName;

        string _newAddress_part1;
        string _newAddress_part2;
        string _newAddress_part3;
        string _setAddressError;

        int _upRPM_Value = 0;
        int _upRPM_Min = 0;
        int _upRPM_Max = 0;
        int _last_UpRPM_Value = 0;

        int _downRPM_Value = 0;
        int _downRPM_Min = 0;
        int _downRPM_Max = 0;
        int _last_DownRPM_Value = 0;

        int _slowRPM_Value = 0;
        int _slowRPM_Min = 0;
        int _slowRPM_Max = 0;
        int _last_SlowRPM_Value = 0;

        bool _direction_Forward;
        bool _direction_Reverse;
        bool directionChanged;

        int _upLimit = 0;
        int _downLimit = 0;

        string _TargetPosition = "0";
        string _setTargetPositionError = "";

        string _moveTargetIP = "1";

        Visibility _speedConfig1_Visibility = Visibility.Visible;
        Visibility _speedConfig2_Visibility = Visibility.Hidden;

        bool _orientationEnabled = true;

        bool _addressChangeEnabled = true;

        bool _stopRequested;

        public VM_MotPage1()
        {
            _scriptHandler = ScriptHandler.Instance;
            _appData = AppData.Instance;

            Motors = new ObservableCollection<Motor>();

            _appData.OnMotorListUpdated += UpdateMotorList; //Called whenever a new motor is added to the list of available motors
            _appData.OnRefreshSelectedMotor += UpdatePageDetails;   //Called whenever data is changed on the selected motor

            PreprocessActions.OnRequestNewMotorName += RetrieveNewMotorName;   //Called when the system is requesting the new motor name that the user has entered
            PreprocessActions.OnRequestNewMotorAddress += RetrieveNewMotorAddress; //Called when the system is requesting the new motor address that the user has entered
            PreprocessActions.OnRequestSpeeds += RetrieveSpeeds;   //Called when the system is requesting the speeds the user has entered
            PreprocessActions.OnRequestDirection += RetrieveOrientation;    //Called when the system is requesting the motor direction the user entered
            PreprocessActions.OnTargetRequest += RetrieveTarget;    //Called when the system is requesting the target for the move to position function
            PreprocessActions.OnIPMoveTargetRequest += RetrieveIPMoveTarget;

            ResponseActions.OnToggleOrientation += ToggleOrientationEnable;
            ResponseActions.OnToggleSpeeds += ToggleSpeedDisplay;

            ResponseActions.AddOnCheckUpButtonListener(CheckUpStillPressed);
            ResponseActions.AddOnCheckDownButtonListener(CheckDownStillPressed);
            View.View_MotPage1.OnUpPressed += UpPressed;
            View.View_MotPage1.OnDownPressed += DownPressed;
            View.View_MotPage1.OnButtonReleased += ButtonReleased;
        }

        private RelayCommand SearchForMotors1;
        public ICommand SearchForMotors
        {
            get
            {
                if (SearchForMotors1 == null)
                {
                    SearchForMotors1 = new RelayCommand(o =>
                    {
                        if (_appData.ResetValidationOnSearch)
                        {
                            foreach (Motor mot in _appData.Motors)
                            {
                                mot.Validated = false;
                            }
                        }

                        _appData.SetTimer(AppData.Timers.VALIDATION_CHECK, _appData.ValidationCheckTime);
                        _scriptHandler.StartScript(Messages.SEARCH_FOR_MOTORS);
                    });
                }

                return SearchForMotors1;
            }
        }

        private RelayCommand ChangeName1;
        public ICommand ChangeName
        {
            get
            {
                if (ChangeName1 == null)
                {
                    ChangeName1 = new RelayCommand(o =>
                    {
                        if (Motors.Count <= 0 || SelectedMotor < 0)
                        {
                            SetAddressError = "ⓘ No motor selected";
                            ErrorDisplayed?.Invoke(this, "Motor Address");
                        }
                        else
                        {
                            _scriptHandler.StartScript(Messages.SET_MOTOR_NAME);
                        }
                    });
                }

                return ChangeName1;
            }
        }

        private RelayCommand ChangeAddress1;
        public ICommand ChangeAddress
        {
            get
            {
                if (ChangeAddress1 == null)
                {
                    ChangeAddress1 = new RelayCommand(o =>
                    {
                        if (Motors.Count <= 0 || SelectedMotor < 0)
                        {
                            SetAddressError = "ⓘ No motor selected";
                            ErrorDisplayed?.Invoke(this, "Motor Address");
                        }
                        else if (AddressIsValid(NewAddress))
                        {
                            SetAddressError = "";
                            _scriptHandler.StartScript(Messages.SET_MOTOR_ADDRESS);
                        }
                        else
                        {
                            SetAddressError = "ⓘ Invalid motor address";
                            ErrorDisplayed?.Invoke(this, "Motor Address");
                        }
                    });
                }

                return ChangeAddress1;
            }
        }

        private RelayCommand SetDirection1;
        public ICommand SetDirection
        {
            get
            {
                if (SetDirection1 == null)
                {
                    SetDirection1 = new RelayCommand(o =>
                    {
                        if (Motors.Count <= 0 || SelectedMotor < 0)
                        {
                            return;
                        }

                        _scriptHandler.StartScript(Messages.SET_MOVEMENT_DIRECTION);
                    });
                }

                return SetDirection1;
            }
        }

        bool AddressIsValid(string addr)
        {
            return addr.Length == 6;
        }

        private RelayCommand SetSpeeds1;
        public ICommand SetSpeeds
        {
            get
            {
                if (SetSpeeds1 == null)
                {
                    SetSpeeds1 = new RelayCommand(o =>
                    {
                        if (Motors.Count <= 0 || SelectedMotor < 0)
                        {
                            return;
                        }

                        _scriptHandler.StartScript(Messages.SET_SPEEDS);
                    });
                }

                return SetSpeeds1;
            }
        }

        private RelayCommand FactoryReset1;
        public ICommand FactoryReset
        {
            get
            {
                if (FactoryReset1 == null)
                {
                    FactoryReset1 = new RelayCommand(o =>
                    {
                        if (Motors.Count <= 0 || SelectedMotor < 0)
                        {
                            return;
                        }

                        if (MessageBox.Show("Are you sure you want to perform a factory reset? You will lose all saved data for this motor.", "Perform Factory Reset?", MessageBoxButton.YesNo) == MessageBoxResult.Yes)
                        {
                            _scriptHandler.StartScript(Messages.FACTORY_RESET_MOTOR);
                        }
                    });
                }

                return FactoryReset1;
            }
        }

        private RelayCommand SetUpLimit1;
        public ICommand SetUpLimit
        {
            get
            {
                if (SetUpLimit1 == null)
                {
                    SetUpLimit1 = new RelayCommand(o =>
                    {
                        _scriptHandler.StartScript(Messages.SET_UP_LIMIT);
                    });
                }

                return SetUpLimit1;
            }
        }

        private RelayCommand SetDownLimit1;
        public ICommand SetDownLimit
        {
            get
            {
                if (SetDownLimit1 == null)
                {
                    SetDownLimit1 = new RelayCommand(o =>
                    {
                        _scriptHandler.StartScript(Messages.SET_DOWN_LIMIT);
                    });
                }

                return SetDownLimit1;
            }
        }

        private RelayCommand MoveUp1;
        public ICommand MoveUp
        {
            get
            {
                if (MoveUp1 == null)
                {
                    MoveUp1 = new RelayCommand(o =>
                    {
                        _scriptHandler.StartScript(Messages.MOVE_MOTOR_UP);
                        _appData.SetTimer(AppData.Timers.UP_MOVEMENT, 500);
                    });
                }

                return MoveUp1;
            }
        }

        private RelayCommand MoveStop1;
        public ICommand MoveStop
        {
            get
            {
                if (MoveStop1 == null)
                {
                    MoveStop1 = new RelayCommand(o =>
                    {
                        _scriptHandler.StartScript(Messages.STOP_MOTOR_MOVEMENT);
                    });
                }

                return MoveStop1;
            }
        }

        void UpPressed()
        {
            StopRequested = false;
            CheckUpStillPressed();
        }

        void DownPressed()
        {
            StopRequested = false;
            CheckDownStillPressed();
        }

        void ButtonReleased()
        {
            StopRequested = true;
        }

        private void CheckUpStillPressed()
        {
            if (StopRequested)
            {
                ScriptHandler.Instance.StartScript(Messages.STOP_MOTOR_MOVEMENT);
            }
            else
            {
                ScriptHandler.Instance.StartScript(Messages.MOVE_MOTOR_UP);
            }
        }

        private void CheckDownStillPressed()
        {
            if (StopRequested)
            {
                ScriptHandler.Instance.StartScript(Messages.STOP_MOTOR_MOVEMENT);
            }
            else
            {
                ScriptHandler.Instance.StartScript(Messages.MOVE_MOTOR_DOWN);
            }
        }

        private RelayCommand StartVisualFeedback1;
        public ICommand StartVisualFeedback
        {
            get
            {
                if (StartVisualFeedback1 == null)
                {
                    StartVisualFeedback1 = new RelayCommand(o =>
                    {
                        _scriptHandler.StartScript(Messages.START_IDENTIFICATION_MOVEMENT);
                        _appData.SetTimer(AppData.Timers.VISUAL_FEEDBACK, 3000);
                    });
                }

                return StartVisualFeedback1;
            }
        }

        private RelayCommand MoveToPos1;
        public ICommand MoveToPos
        {
            get
            {
                if (MoveToPos1 == null)
                {
                    MoveToPos1 = new RelayCommand(o =>
                    {
                        if (!int.TryParse(TargetPosition, out int target))
                        {
                            SetTargetPositionError = "ⓘ Invalid target";
                            ErrorDisplayed?.Invoke(this, "Set Target");
                            return;
                        }

                        if(_selectedMotor < 0)
                        {
                            SetTargetPositionError = "ⓘ No motor selected";
                            ErrorDisplayed?.Invoke(this, "Set Target");
                            return;
                        }

                        SetTargetPositionError = "";
                        _scriptHandler.StartScript(Messages.MOVE_TO_POSITION);
                    });
                }

                return MoveToPos1;
            }
        }

        private RelayCommand ChangePage1;
        public ICommand ChangePage
        {
            get
            {
                if (ChangePage1 == null)
                {
                    ChangePage1 = new RelayCommand(o =>
                     {
                        OnMotorPageChange?.Invoke();
                    });
                }

                return ChangePage1;
            }
        }

        private RelayCommand MoveToUpLimit1;
        public ICommand MoveToUpLimit
        {
            get
            {
                if (MoveToUpLimit1 == null)
                {
                    MoveToUpLimit1 = new RelayCommand(o =>
                    {
                        _scriptHandler.StartScript(Messages.MOVE_TO_UP_LIMIT);
                    });
                }

                return MoveToUpLimit1;
            }
        }

        private RelayCommand MoveToDownLimit1;
        public ICommand MoveToDownLimit
        {
            get
            {
                if (MoveToDownLimit1 == null)
                {
                    MoveToDownLimit1 = new RelayCommand(o =>
                    {
                        _scriptHandler.StartScript(Messages.MOVE_TO_DOWN_LIMIT);
                    });
                }

                return MoveToDownLimit1;
            }
        }

        private RelayCommand MoveToSelectedIP1;
        public ICommand MoveToSelectedIP
        {
            get
            {
                if (MoveToSelectedIP1 == null)
                {
                    MoveToSelectedIP1 = new RelayCommand(o =>
                    {
                        if(!int.TryParse(_moveTargetIP, out int res))
                        {
                            _appData.DisplayError("Invalid Internal Position", "The chosen IP is not valid. Please enter number between 1 and 16");
                        }
                        _scriptHandler.StartScript(Messages.MOVE_TO_SELECTED_IP);
                    });
                }

                return MoveToSelectedIP1;
            }
        }

        void UpdateMotorList()
        {
            Motors = new ObservableCollection<Motor>(_appData.Motors);

            if (SelectedMotor < 0)
            {
                SelectedMotor = 0;
            }
        }

        public void UpdatePageDetails()
        {
            try
            {
                if (_appData.Motors.Count <= 0)
                {
                    MotorLabel = "No Motor Selected";
                    NewName = String.Empty;
                    NodeType = "No Type";

                    UpRpm_Min = 0;
                    UpRpm_Max = 0;
                    DownRpm_Min = 0;
                    DownRpm_Max = 0;
                    SlowRpm_Min = 0;
                    SlowRpm_Max = 0;

                    UpRpm_Value = 0;
                    DownRpm_Value = 0;
                    SlowRpm_Value = 0;

                    UpLimit = 0;
                    DownLimit = 0;

                    SelectedMotor = -1;
                    return;
                }

                if (_appData.SelectedMotor.NameAsString == "")
                {
                    MotorLabel = $"Unnamed Motor ({_appData.SelectedMotor.AddressAsString})";   //Display the heading of the page as the motor name + address
                }
                else
                {
                    MotorLabel = $"{_appData.SelectedMotor.NameAsString} ({_appData.SelectedMotor.AddressAsString})";   //Display the heading of the page as the motor name + address
                }

                NewName = _appData.SelectedMotor.NameAsString;  //Add the name to the new name field

                //Get the limits based on the type of motor it is
                MotorTypeParams parameters = MotorConfigs.GetParams(_appData.SelectedMotor.Type[1]);
                NodeType = $"Node: {parameters.NodeName}";

                UpRpm_Min = parameters.UpRPM_Min;
                UpRpm_Max = parameters.UpRPM_Max;
                DownRpm_Min = parameters.DownRPM_Min;
                DownRpm_Max = parameters.DownRPM_Max;
                SlowRpm_Min = parameters.SlowRPM_Min;
                SlowRpm_Max = parameters.SlowRPM_Max;

                //Get the address
                NewAddress_Part1 = _appData.SelectedMotor.AddressAsString.Substring(0, 2);
                NewAddress_Part2 = _appData.SelectedMotor.AddressAsString.Substring(3, 2);
                NewAddress_Part3 = _appData.SelectedMotor.AddressAsString.Substring(6, 2);

                //Get the speeds
                UpRpm_Value = _appData.SelectedMotor.UpSpeed > UpRpm_Max ? UpRpm_Max : _appData.SelectedMotor.UpSpeed;
                DownRpm_Value = _appData.SelectedMotor.DownSpeed > DownRpm_Max ? DownRpm_Max : _appData.SelectedMotor.DownSpeed;
                SlowRpm_Value = _appData.SelectedMotor.SlowSpeed > SlowRpm_Max ? SlowRpm_Max : _appData.SelectedMotor.SlowSpeed;

                //Get limits
                UpLimit = _appData.SelectedMotor.UpLimit;
                DownLimit = _appData.SelectedMotor.DownLimit;

                if(UpLimit == 65535)
                {
                    UpLimit = 0;
                }

                if (DownLimit == 65535)
                {
                    DownLimit = 0;
                }

                if (_appData.SelectedMotor.MovementDirection == 0)
                {
                    Direction_Forward = true;
                    Direction_Reverse = false;
                }
                else
                {
                    Direction_Forward = false;
                    Direction_Reverse = true;
                }

                AddressChangeEnabled = parameters.ModifiableAddress;

                if(parameters.TwoFastSpeeds)
                {
                    SpeedConfig1_Visibility = Visibility.Hidden;
                    SpeedConfig2_Visibility = Visibility.Visible;
                }
                else
                {
                    SpeedConfig1_Visibility = Visibility.Visible;
                    SpeedConfig2_Visibility = Visibility.Hidden;
                }
            }
            catch
            {

            }
        }

        void RetrieveNewMotorAddress(object sender, PreprocessActions.StringRequestEventArgs e)
        {
            e.StringValue = NewAddress;
        }

        void RetrieveNewMotorName(object sender, PreprocessActions.StringRequestEventArgs e)
        {
            e.StringValue = NewName;
        }

        void RetrieveSpeeds(object sender, PreprocessActions.SpeedsRequestEventArgs e)
        {
            e.UpValue = UpRpm_Value;
            e.DownValue = DownRpm_Value;
            e.SlowValue = SlowRpm_Value;
        }

        void RetrieveOrientation(object sender, PreprocessActions.DirectionRequestEventArgs e)
        {
            if (Direction_Forward == true)
            {
                e.Direction = 0x00;
            }
            else
            {
                e.Direction = 0x01;
            }
        }

        void RetrieveTarget(object sender, PreprocessActions.TargetRequestEventArgs e)
        {
            e.target = int.Parse(TargetPosition);
        }

        void RetrieveIPMoveTarget(object sender, PreprocessActions.CurrentIPRequestArgs e)
        {
            e.IP = int.Parse(_moveTargetIP);
        }

        void GetNewMotorDetails()
        {
            _scriptHandler.StartScript(Messages.UPDATE_MOTOR_PAGE);
        }

        void ToggleOrientationEnable(bool toggle)
        {
            OrientationEnabled = toggle;
        }

        void ToggleSpeedDisplay(bool showThreeSpeeds)
        {
            if(showThreeSpeeds)
            {
                SpeedConfig1_Visibility= Visibility.Visible;
                SpeedConfig2_Visibility= Visibility.Hidden;
            }
            else
            {
                SpeedConfig1_Visibility = Visibility.Hidden;
                SpeedConfig2_Visibility = Visibility.Visible;
            }
        }

        public ObservableCollection<Motor> Motors { get { return _motors; } set { _motors = value; OnPropertyChanged(); } }
        public int SelectedMotor
        {
            get { return _selectedMotor; }
            set
            {
                _selectedMotor = value;
                _appData.MotorIndex = value;

                if (_selectedMotor != _lastSelectedMotor && _selectedMotor >= 0)
                {
                    ValidMotor = true;
                    UpdatePageDetails();

                    if (_lastSelectedMotor >= 0)
                    {
                        GetNewMotorDetails();
                    }
                }

                if(_selectedMotor < 0)
                {
                    ValidMotor = false;
                }

                _lastSelectedMotor = _selectedMotor;
                OnPropertyChanged();
            }
        }
        public bool ValidMotor { get { return _validMotor; } set { _validMotor = value; OnPropertyChanged(); } }
        public string MotorLabel { get { return _motorLabel; } set { _motorLabel = value; OnPropertyChanged(); } }
        public string NodeType { get { return _nodeType; } set { _nodeType = value; OnPropertyChanged(); } }
        public string Firmware { get { return _firmware; } set { _firmware = value; OnPropertyChanged(); } }

        public string NewName { get { return _newMotorName; } set { _newMotorName = value; OnPropertyChanged(); } }

        public string NewAddress_Part1 { get { return _newAddress_part1; } set { _newAddress_part1 = value; OnPropertyChanged(); } }
        public string NewAddress_Part2 { get { return _newAddress_part2; } set { _newAddress_part2 = value; OnPropertyChanged(); } }
        public string NewAddress_Part3 { get { return _newAddress_part3; } set { _newAddress_part3 = value; OnPropertyChanged(); } }
        public string NewAddress
        {
            get { return $"{_newAddress_part1}{_newAddress_part2}{_newAddress_part3}"; }

            set
            {
                _newAddress_part1 = value.Substring(0, 2);
                _newAddress_part2 = value.Substring(2, 2);
                _newAddress_part3 = value.Substring(4, 2);
                OnPropertyChanged();
            }
        }
        public string SetAddressError { get { return _setAddressError; } set { _setAddressError = value; OnPropertyChanged(); } }

        public int UpRpm_Value { get { return _upRPM_Value; } set { _upRPM_Value = value; OnPropertyChanged(); } }
        public int UpRpm_Min { get { return _upRPM_Min; } set { _upRPM_Min = value; OnPropertyChanged(); } }
        public int UpRpm_Max { get { return _upRPM_Max; } set { _upRPM_Max = value; OnPropertyChanged(); } }

        public int DownRpm_Value { get { return _downRPM_Value; } set { _downRPM_Value = value; OnPropertyChanged(); } }
        public int DownRpm_Min { get { return _downRPM_Min; } set { _downRPM_Min = value; OnPropertyChanged(); } }
        public int DownRpm_Max { get { return _downRPM_Max; } set { _downRPM_Max = value; OnPropertyChanged(); } }

        public int SlowRpm_Value { get { return _slowRPM_Value; } set { _slowRPM_Value = value; OnPropertyChanged(); } }
        public int SlowRpm_Min { get { return _slowRPM_Min; } set { _slowRPM_Min = value; OnPropertyChanged(); } }
        public int SlowRpm_Max { get { return _slowRPM_Max; } set { _slowRPM_Max = value; OnPropertyChanged(); } }

        public bool Direction_Forward { get { return _direction_Forward; } set { _direction_Forward = value; OnPropertyChanged(); } }
        public bool Direction_Reverse { get { return _direction_Reverse; } set { _direction_Reverse = value; OnPropertyChanged(); } }

        public int UpLimit { get { return _upLimit; } set { _upLimit = value; OnPropertyChanged(); } }
        public int DownLimit { get { return _downLimit; } set { _downLimit = value; OnPropertyChanged(); } }

        public string TargetPosition { get { return _TargetPosition; } set { _TargetPosition = value; OnPropertyChanged(); } }
        public string SetTargetPositionError { get { return _setTargetPositionError; } set { _setTargetPositionError = value; OnPropertyChanged(); } }

        public Visibility SpeedConfig1_Visibility { get { return _speedConfig1_Visibility; } set { _speedConfig1_Visibility = value; OnPropertyChanged(); } }
        public Visibility SpeedConfig2_Visibility { get { return _speedConfig2_Visibility; } set { _speedConfig2_Visibility = value; OnPropertyChanged(); } }

        public bool OrientationEnabled { get { return _orientationEnabled; } set { _orientationEnabled = value; OnPropertyChanged(); } }

        public bool AddressChangeEnabled { get { return _addressChangeEnabled; } set { _addressChangeEnabled = value; OnPropertyChanged(); } }

        public string MoveTargetIP { get { return _moveTargetIP; } set { _moveTargetIP = value; OnPropertyChanged(); } }

        public bool StopRequested { get { return _stopRequested; } set { _stopRequested = value; OnPropertyChanged(); } }
    }
}