using Blind_Config_Tool.Core;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics.Contracts;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media.Animation;
using System.Windows.Media;
using System.ComponentModel;
using System.IO;
using System.Windows.Forms;

namespace Blind_Config_Tool.RedesignFiles.MVVM.ViewModel
{
    internal class VM_Keypad : ObservableObject
    {
        #region Declarations
        ScriptHandler _scriptHandler = ScriptHandler.Instance;
        AppData _appData = AppData.Instance;

        ObservableCollection<Keypad> _keypads;
        int _keypadIndex = 0;

        string _newAddressPt1;
        string _newAddressPt2;
        string _newAddressPt3;
        string _keypadAddressError = "";

        string _newMotorAllAddressPt1;
        string _newMotorAllAddressPt2;
        string _newMotorAllAddressPt3;
        string _motorAllTitle = "'Motor All' Address: FF-FF-FF";
        string _motorAllError = "";

        ObservableCollection<KeypadCommand> _commands;
        int _selectedCommand;
        string _selectedButton = "Button 1";
        string _buttonConfigError;
        Visibility _saveCommandDoneVisibility = Visibility.Hidden;

        private string[] _deviceTypesList = new string[2] { "KEYPAD (8 Commands)", "BOARD (100 Commands)" };
        private int _deviceTypeIndex;

        private KeypadButtonAction _currentCommandOnPressAction;
        private string _currentCommandOnPressTarget = "0";
        private KeypadButtonAction _currentCommandOnHoldAction;
        private string _currentCommandOnHoldTarget = "0";
        private KeypadButtonAction _currentCommandOnReleaseAction;
        private string _currentCommandOnReleaseTarget = "0";
        private int _currentCommand;
        private TargetMotor _targetMotor;
        private string _currentCommandTargetAddress;
        private bool _targetAddressEnabled = false;
        private string _keypadName = "";
        private string _keypadNameError = "";

        private Visibility _uploadBoxVisibility = Visibility.Hidden;
        #endregion

        #region Events
        public static event EventHandler<string> ErrorDisplayed;
        ActionTimer doneLabelTimer;
        #endregion

        #region Constructor
        public VM_Keypad()
        {
            _appData.OnKeypadListUpdated += UpdateKeypadList;
            _appData.OnKeypadCommandsListUpdated += UpdateCommandsList;
            PreprocessActions.OnRequestNewKeypadAddress += RetrieveNewKeypadAddress;

            KeypadList = new ObservableCollection<Keypad>(_appData.Keypads);
            UpdateCommandListSize();

            doneLabelTimer = new ActionTimer(AppData.Timers.PRIVATE_TIMER, HideDoneLabel);
        }
        #endregion

        #region Finding & Displaying Keypads
        private RelayCommand SearchForKeypads1;
        public ICommand SearchForKeypads
        {
            get
            {
                if (SearchForKeypads1 == null)
                {
                    SearchForKeypads1 = new RelayCommand(o =>
                    {
                        if (_appData.ResetValidationOnSearch)
                        {
                            foreach (Keypad keypad in _appData.Keypads)
                            {
                                keypad.Validated = false;
                            }
                        }

                        _appData.SetTimer(AppData.Timers.VALIDATION_CHECK, _appData.ValidationCheckTime);
                        _scriptHandler.StartScript(Messages.SEARCH_FOR_KEYPADS);
                    });
                }

                return SearchForKeypads1;
            }
        }

        void UpdateKeypadList()
        {
            KeypadList = new ObservableCollection<Keypad>(_appData.Keypads);
            SelectedKeypad = SelectedKeypad;
        }
        #endregion

        #region Setting New Keypad Address/Name
        void RetrieveNewKeypadAddress(object sender, PreprocessActions.StringRequestEventArgs e)
        {
            e.StringValue = $"{NewKeypadAddress_Part1}{NewKeypadAddress_Part2}{NewKeypadAddress_Part3}";
        }

        private RelayCommand SetNewKeypadAddress1;
        public ICommand SetNewKeypadAddress
        {
            get
            {
                if (SetNewKeypadAddress1 == null)
                {
                    SetNewKeypadAddress1 = new RelayCommand(o =>
                    {
                        if (KeypadList.Count <= 0)
                        {
                            KeypadAddressError = "ⓘ No keypad selected";
                            ErrorDisplayed?.Invoke(this, "Keypad Address");
                        }
                        else if (AddressIsValid(NewKeypadAddress))
                        {
                            KeypadAddressError = "";
                            _scriptHandler.StartScript(Messages.SAVE_NEW_KEYPAD_ADDRESS);
                        }
                        else
                        {
                            KeypadAddressError = "ⓘ Invalid keypad address";
                            ErrorDisplayed?.Invoke(this, "Keypad Address");
                        }
                    });
                }

                return SetNewKeypadAddress1;
            }
        }

        bool AddressIsValid(string addr)
        {
            return addr.Length == 6;
        }
        #endregion

        #region Setting Motor All Address
        private RelayCommand SetMotorAllAddress1;
        public ICommand SetMotorAllAddress
        {
            get
            {
                if(SetMotorAllAddress1 == null)
                {
                    SetMotorAllAddress1 = new RelayCommand(o =>
                    {
                        if (AddressIsValid(NewMotorAllAddress))
                        {
                            MotorAllError = "";
                            _appData.MotorAllAddress = _appData.StringToByteArray(NewMotorAllAddress);
                            MotorAllTitle = NewMotorAllAddress;
                            CurrentCommandTargetAddress = NewMotorAllAddress;

                            for (int i = 0; i < _appData.KeypadCommandsList.Count; i++)
                            {
                                if (_appData.KeypadCommandsList[i].TargetMotor == TargetMotor.ALL_MOTORS)
                                {
                                    _appData.KeypadCommandsList[i].TargetAddress = NewMotorAllAddress;
                                }
                            }
                        }
                        else
                        {
                            MotorAllError = "ⓘ Invalid motor address";
                            ErrorDisplayed?.Invoke(this, "Motor All");
                        }
                    });
                }

                return SetMotorAllAddress1;
            }
        }

        private RelayCommand SetKeypadName1;
        public ICommand SetKeypadName
        {
            get
            {
                if (SetKeypadName1 == null)
                {
                    SetKeypadName1 = new RelayCommand(o =>
                    {
                        if (KeypadList.Count <= 0)
                        {
                            SetNameError = "ⓘ No keypad selected";
                            ErrorDisplayed?.Invoke(this, "Set Name Button");
                            return;
                        }

                        if (KeypadName == "")
                        {
                            SetNameError = "ⓘ Invalid name";
                            ErrorDisplayed?.Invoke(this, "Set Name Button");
                            return;
                        }

                        _appData.NewKeypadName = KeypadName;
                        _scriptHandler.StartScript(Messages.SET_KEYPAD_NAME);
                    });
                }

                return SetKeypadName1;
            }
        }
        #endregion

        #region Saving Command Settings
        private RelayCommand SaveCommandSettings1;
        public ICommand SaveCommandSettings
        {
            get 
            { 
                if(SaveCommandSettings1 == null)
                {
                    SaveCommandSettings1 = new RelayCommand(o =>
                    {
                        if(SelectedCommand < 0)
                        {
                            ButtonConfigError = "ⓘ No button selected";
                            ErrorDisplayed?.Invoke(this, "Save Button Config");
                            return;
                        }

                        if(CurrentCommandTargetAddress.Length != 6)
                        {
                            ButtonConfigError = "ⓘ Address must be 6 characters long";
                            ErrorDisplayed?.Invoke(this, "Save Button Config");
                            return;
                        }

                        if (CurrentCommandOnPressTarget.StartsWith("0"))
                        {
                            CurrentCommandOnPressTarget = GetMinTarget(CurrentCommandOnPressAction);
                        }

                        if (CurrentCommandOnHoldTarget.StartsWith("0"))
                        {
                            CurrentCommandOnHoldTarget = GetMinTarget(CurrentCommandOnPressAction);
                        }

                        if (CurrentCommandOnReleaseTarget.StartsWith("0"))
                        {
                            CurrentCommandOnReleaseTarget = GetMinTarget(CurrentCommandOnPressAction);
                        }

                        ButtonConfigError = "";
                        KeypadCommand updatedCommand = new KeypadCommand(SelectedCommand);
                        updatedCommand.OnPressAction = CurrentCommandOnPressAction;
                        updatedCommand.OnPressTarget = CurrentCommandOnPressTarget;
                        updatedCommand.OnHoldAction = CurrentCommandOnHoldAction;
                        updatedCommand.OnHoldTarget = CurrentCommandOnHoldTarget;
                        updatedCommand.OnReleaseAction = CurrentCommandOnReleaseAction;
                        updatedCommand.OnReleaseTarget = CurrentCommandOnReleaseTarget;
                        updatedCommand.TargetMotor = CurrentCommandTargetMotor;
                        updatedCommand.TargetAddress = CurrentCommandTargetAddress;
                        updatedCommand.Changed = true;

                        int cmd = SelectedCommand;
                        _appData.AddNewKeypadCommand(updatedCommand);
                        SelectedCommand = cmd;
                        SaveCommandDoneVisibility = Visibility.Visible;
                        doneLabelTimer.Start(3000);
                    });
                }
                return SaveCommandSettings1; 
            }
        }
        #endregion

        #region Uploading/Retrieving Commands
        void UpdateCommandsList()
        {
            Commands = new ObservableCollection<KeypadCommand>(_appData.KeypadCommandsList);
        }

        private RelayCommand UploadCommands1;
        public ICommand UploadCommands
        {
            get
            {
                if (UploadCommands1 == null)
                {
                    UploadCommands1 = new RelayCommand(o =>
                    {
                        if(KeypadList.Count <= 0 || SelectedKeypad < 0)
                        {
                            _appData.DisplayError("No keypad selected. Search for keypads and select one using the listbox above.", "No Keypad Selected");
                            return;
                        }

                        UploadBoxVisibility = Visibility.Visible;
                    });
                }

                return UploadCommands1;
            }
        }

        private RelayCommand RetrieveCommands1;
        public ICommand RetrieveCommands
        {
            get
            {
                if (RetrieveCommands1 == null)
                {
                    RetrieveCommands1 = new RelayCommand(o =>
                    {
                        if (KeypadList.Count <= 0 || SelectedKeypad < 0)
                        {
                            _appData.DisplayError("No keypad selected. Search for keypads and select one using the listbox above.", "No Keypad Selected");
                            return;
                        }

                        _scriptHandler.StartScript(Messages.GET_BUTTON_DATA_FROM_KEYPAD);
                    });
                }

                return RetrieveCommands1;
            }
        }
        #endregion

        #region Changing The Number Of Commands In The List
        void UpdateCommandListSize()
        {
            switch (_deviceTypeIndex)
            {
                case 0:
                    _appData.ChangeNumberOfCommands(8);
                    break;
                case 1:
                    _appData.ChangeNumberOfCommands(100);
                    break;
            }
        }
        #endregion

        #region Change Selected Button
        private RelayCommand ChangeSelectedButton1;
        public ICommand ChangeSelectedButton
        {
            get
            {
                if (ChangeSelectedButton1 == null)
                {
                    ChangeSelectedButton1 = new RelayCommand(o =>
                    {
                        SelectedButtonName = (SelectedCommand + 1).ToString();
                    });
                }

                return ChangeSelectedButton1;
            }
        }

        private void UpdateSelectedButtonDetails()
        {
            KeypadCommand cmd = Commands[SelectedCommand];

            CurrentCommandOnPressAction = cmd.OnPressAction;
            CurrentCommandOnPressTarget = cmd.OnPressTarget;

            CurrentCommandOnHoldAction = cmd.OnHoldAction;
            CurrentCommandOnHoldTarget = cmd.OnHoldTarget;

            CurrentCommandOnReleaseAction = cmd.OnReleaseAction;
            CurrentCommandOnReleaseTarget = cmd.OnReleaseTarget;

            CurrentCommandTargetMotor = cmd.TargetMotor;
            CurrentCommandTargetAddress = cmd.TargetAddress;
        }
        #endregion

        #region Getting The Min Value For A Target
        private string GetMinTarget(KeypadButtonAction action)
        {
            switch (action)
            {
                case KeypadButtonAction.GO_TO_IP:
                case KeypadButtonAction.SET_IP:
                    return $"{1}";
                default:
                    return $"{0}";
            }
        }
        #endregion

        #region Hide the done label
        void HideDoneLabel()
        {
            SaveCommandDoneVisibility = Visibility.Hidden;
        }
        #endregion

        #region Upload Options
        private RelayCommand UploadAll1;
        public ICommand UploadAll
        {
            get
            {
                if (UploadAll1 == null)
                {
                    UploadAll1 = new RelayCommand(o =>
                    {
                        _appData.UploadType = AppData.Upload_Type.UPLOAD_ALL;
                        _scriptHandler.StartScript(Messages.PROGRAM_KEYPAD_COMMANDS);
                        UploadBoxVisibility = Visibility.Hidden;
                    });
                }

                return UploadAll1;
            }
        }

        private RelayCommand UploadWithValues1;
        public ICommand UploadWithValues
        {
            get
            {
                if (UploadWithValues1 == null)
                {
                    UploadWithValues1 = new RelayCommand(o =>
                    {
                        bool canProgram = false;

                        foreach(KeypadCommand cmd in _appData.KeypadCommandsList)
                        {
                            if(cmd.OnPressAction != KeypadButtonAction.NO_ACTION || cmd.OnHoldAction != KeypadButtonAction.NO_ACTION || cmd.OnReleaseAction != KeypadButtonAction.NO_ACTION)
                            {
                                canProgram = true;
                                break;
                            }
                        }

                        if (canProgram)
                        {
                            _appData.UploadType = AppData.Upload_Type.UPLOAD_BUTTONS_WITH_VALUES;
                            _scriptHandler.StartScript(Messages.PROGRAM_KEYPAD_COMMANDS);
                        }
                        else
                        {
                            _appData.DisplayError("There are no buttons that can be programmed with this option. Please modify a button and try again.", "No Valid Buttons");
                        }

                        UploadBoxVisibility = Visibility.Hidden;
                    });
                }

                return UploadWithValues1;
            }
        }

        private RelayCommand UploadModifiedOnly1;
        public ICommand UploadModifiedOnly
        {
            get
            {
                if (UploadModifiedOnly1 == null)
                {
                    UploadModifiedOnly1 = new RelayCommand(o =>
                    {
                        bool canProgram = false;

                        foreach (KeypadCommand cmd in _appData.KeypadCommandsList)
                        {
                            if (cmd.Changed)
                            {
                                canProgram = true;
                                break;
                            }
                        }

                        if (canProgram)
                        {
                            _appData.UploadType = AppData.Upload_Type.UPLOAD_MODIFIED;
                            _scriptHandler.StartScript(Messages.PROGRAM_KEYPAD_COMMANDS);
                        }
                        else
                        {
                            _appData.DisplayError("There are no buttons that can be programmed with this option. Please modify a button and try again.", "No Valid Buttons");
                        }
                        
                        UploadBoxVisibility = Visibility.Hidden;
                    });
                }

                return UploadModifiedOnly1;
            }
        }

        private RelayCommand CancelUpload1;
        public ICommand CancelUpload
        {
            get
            {
                if (CancelUpload1 == null)
                {
                    CancelUpload1 = new RelayCommand(o =>
                    {
                        UploadBoxVisibility = Visibility.Hidden;
                    });
                }

                return CancelUpload1;
            }
        }
        #endregion

        #region Saving/Loading From Excel Files
        private RelayCommand SaveToFile1;
        public ICommand SaveToFile
        {
            get
            {
                if (SaveToFile1 == null)
                {
                    SaveToFile1 = new RelayCommand(o =>
                    {
                        using (var dialog = new System.Windows.Forms.FolderBrowserDialog())
                        {
                            dialog.SelectedPath = Directory.GetCurrentDirectory();
                            System.Windows.Forms.DialogResult result = dialog.ShowDialog();

                            if (result == System.Windows.Forms.DialogResult.OK)
                            {
                                if (System.Windows.MessageBox.Show("Are you sure you want to save to \"" + dialog.SelectedPath + "\"?", "Confirm Save Location", MessageBoxButton.YesNo) == MessageBoxResult.Yes)
                                {
                                    ExcelFileCreator.SaveToExcelFile(_appData.KeypadCommandsList, dialog.SelectedPath);
                                }
                            }
                        }
                    });
                }

                return SaveToFile1;
            }
        }

        private RelayCommand LoadFromFile1;
        public ICommand LoadFromFile
        {
            get
            {
                if (LoadFromFile1 == null)
                {
                    LoadFromFile1 = new RelayCommand(o =>
                    {
                        // Create a new instance of the OpenFileDialog class
                        var openFileDialog = new System.Windows.Forms.OpenFileDialog();

                        // Set the initial directory and filter for the dialog box
                        openFileDialog.InitialDirectory = Directory.GetCurrentDirectory();
                        openFileDialog.Filter = "Excel files (*.xlsx)|*.xlsx|All files (*.*)|*.*";

                        // Show the dialog box and get the result
                        var result = openFileDialog.ShowDialog();

                        // If the user clicked OK, get the selected file path
                        if (result == DialogResult.OK)
                        {
                            List<KeypadCommand> loadedCommands = ExcelFileCreator.LoadFromExcelFile(openFileDialog.FileName);

                            if (loadedCommands.Count > 0)
                            {
                                if (loadedCommands.Count > 8)
                                {
                                    //Change to board on dropdown menu
                                    DeviceTypeIndex = (int)DeviceType.TYPE_BOARD;
                                }
                                else
                                {
                                    //Change to keypad on dropdown menu
                                    DeviceTypeIndex = (int)DeviceType.TYPE_KEYPAD;
                                }

                                for (int i = 0; i < loadedCommands.Count; i++)
                                {
                                    _appData.KeypadCommandsList[i].OnPressAction = loadedCommands[i].OnPressAction;
                                    _appData.KeypadCommandsList[i].OnPressTarget = loadedCommands[i].OnPressTarget;

                                    _appData.KeypadCommandsList[i].OnHoldAction = loadedCommands[i].OnHoldAction;
                                    _appData.KeypadCommandsList[i].OnHoldTarget = loadedCommands[i].OnHoldTarget;

                                    _appData.KeypadCommandsList[i].OnReleaseAction = loadedCommands[i].OnReleaseAction;
                                    _appData.KeypadCommandsList[i].OnReleaseTarget = loadedCommands[i].OnReleaseTarget;

                                    _appData.KeypadCommandsList[i].TargetMotor = loadedCommands[i].TargetMotor;
                                    _appData.KeypadCommandsList[i].TargetAddress = loadedCommands[i].TargetAddress;

                                    _appData.KeypadCommandsList[i].Changed = loadedCommands[i].Changed;
                                }

                                System.Windows.MessageBox.Show("Commands successfully loaded from file.");
                            }
                        }
                    });
                }

                return LoadFromFile1;
            }
        }
        #endregion

        #region Accessors
        public ObservableCollection<Keypad> KeypadList { get { return _keypads; } set { _keypads = value; OnPropertyChanged(); } }
        public int SelectedKeypad { get { return _keypadIndex; } 
            set 
            { 
                _keypadIndex = value; 
                _appData.KeypadIndex = value; 
                KeypadName = KeypadList[SelectedKeypad].NameAsString;
                NewKeypadAddress_Part1 = KeypadList[SelectedKeypad].AddressAsString.Substring(0, 2);
                NewKeypadAddress_Part2 = KeypadList[SelectedKeypad].AddressAsString.Substring(3, 2);
                NewKeypadAddress_Part3 = KeypadList[SelectedKeypad].AddressAsString.Substring(6, 2);
                OnPropertyChanged(); 
            } 
        }

        public string SelectedButtonName { get { return _selectedButton; } 
            set 
            { 
                if(value == "0")
                {
                    _selectedButton = "No Button Selected";
                }
                else
                {
                    _selectedButton = $"Button {value}";
                    UpdateSelectedButtonDetails();
                }

                OnPropertyChanged(); 
            } 
        }

        public string NewKeypadAddress_Part1 { get { return _newAddressPt1; } set { _newAddressPt1 = value; OnPropertyChanged(); } }
        public string NewKeypadAddress_Part2 { get { return _newAddressPt2; } set { _newAddressPt2 = value; OnPropertyChanged(); } }
        public string NewKeypadAddress_Part3 { get { return _newAddressPt3; } set { _newAddressPt3 = value; OnPropertyChanged(); } }
        public string NewKeypadAddress 
        { 
            get { return $"{_newAddressPt1}{_newAddressPt2}{_newAddressPt3}"; } 

            set 
            {
                _newAddressPt1 = value.Substring(0, 2);
                _newAddressPt2 = value.Substring(2, 2);
                _newAddressPt3 = value.Substring(4, 2);
                OnPropertyChanged(); 
            } 
        }
        public string KeypadAddressError { get { return _keypadAddressError; } set { _keypadAddressError = value; OnPropertyChanged(); } }

        public string NewMotorAllAddress_Part1 { get { return _newMotorAllAddressPt1; } set { _newMotorAllAddressPt1 = value; OnPropertyChanged(); } }
        public string NewMotorAllAddress_Part2 { get { return _newMotorAllAddressPt2; } set { _newMotorAllAddressPt2 = value; OnPropertyChanged(); } }
        public string NewMotorAllAddress_Part3 { get { return _newMotorAllAddressPt3; } set { _newMotorAllAddressPt3 = value; OnPropertyChanged(); } }
        public string NewMotorAllAddress
        {
            get { return $"{_newMotorAllAddressPt1}{_newMotorAllAddressPt2}{_newMotorAllAddressPt3}"; }

            set
            {
                _newMotorAllAddressPt1 = value.Substring(0, 2);
                _newMotorAllAddressPt2 = value.Substring(2, 2);
                _newMotorAllAddressPt3 = value.Substring(4, 2);
                OnPropertyChanged();
            }
        }
        public string MotorAllTitle { get { return _motorAllTitle; } set { _motorAllTitle = $"'Motor All' Address: {value.Substring(0,2)}-{value.Substring(2, 2)}-{value.Substring(4, 2)}"; OnPropertyChanged(); } }
        public string MotorAllError { get { return _motorAllError; } set { _motorAllError = value; OnPropertyChanged(); } }
        public ObservableCollection<KeypadCommand> Commands { get { return _commands;} set { _commands = value; OnPropertyChanged(); } }
        public int SelectedCommand { get { return _selectedCommand; } set { _selectedCommand = value; OnPropertyChanged(); } }

        public string[] DeviceTypesList { get { return _deviceTypesList; } }
        public int DeviceTypeIndex { get { return _deviceTypeIndex; } set { _deviceTypeIndex = value; UpdateCommandListSize(); OnPropertyChanged(); } }

        public KeypadButtonAction CurrentCommandOnPressAction { get { return _currentCommandOnPressAction; } set { _currentCommandOnPressAction = value; OnPropertyChanged(); } }
        public string CurrentCommandOnPressTarget { get { return _currentCommandOnPressTarget; } set { _currentCommandOnPressTarget = value; OnPropertyChanged(); } }
        public KeypadButtonAction CurrentCommandOnHoldAction { get { return _currentCommandOnHoldAction; } set { _currentCommandOnHoldAction = value; OnPropertyChanged(); } }
        public string CurrentCommandOnHoldTarget { get { return _currentCommandOnHoldTarget; } set { _currentCommandOnHoldTarget = value; OnPropertyChanged(); } }
        public KeypadButtonAction CurrentCommandOnReleaseAction { get { return _currentCommandOnReleaseAction; } set { _currentCommandOnReleaseAction = value; OnPropertyChanged(); } }
        public string CurrentCommandOnReleaseTarget { get { return _currentCommandOnReleaseTarget; } set { _currentCommandOnReleaseTarget = value; OnPropertyChanged(); } }
        public TargetMotor CurrentCommandTargetMotor { get { return _targetMotor; } 
            set 
            { 
                _targetMotor = value;
                
                if(value == TargetMotor.ALL_MOTORS)
                {
                    TargetAddressEnabled = false;
                    CurrentCommandTargetAddress = "FFFFFF";
                }
                else
                {
                    TargetAddressEnabled = true;
                }

                OnPropertyChanged(); 
            } 
        }

        public string CurrentCommandTargetAddress { get { return _currentCommandTargetAddress; } set { _currentCommandTargetAddress = value; OnPropertyChanged(); } }
        public string ButtonConfigError { get { return _buttonConfigError; } set { _buttonConfigError = value; OnPropertyChanged(); } }
        public Visibility SaveCommandDoneVisibility { get { return _saveCommandDoneVisibility; } set { _saveCommandDoneVisibility = value; OnPropertyChanged(); } }

        public bool TargetAddressEnabled { get { return _targetAddressEnabled;  } set { _targetAddressEnabled = value; OnPropertyChanged(); } }
        public string KeypadName { get { return _keypadName; } set { _keypadName = value; OnPropertyChanged(); } }
        public string SetNameError { get { return _keypadNameError; } set { _keypadNameError = value; OnPropertyChanged(); } }
        public Visibility UploadBoxVisibility { get { return _uploadBoxVisibility; } set { _uploadBoxVisibility = value; OnPropertyChanged(); } }
        #endregion
    }
}