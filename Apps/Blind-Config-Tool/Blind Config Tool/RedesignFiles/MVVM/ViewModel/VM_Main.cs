using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using Blind_Config_Tool.Core;
using Blind_Config_Tool.RedesignFiles;
using System.Windows;
using Blind_Config_Tool.RedesignFiles.MVVM.ViewModel;
using System.Windows.Media;
using System.IO.Ports;

namespace Blind_Config_Tool.RedesignFiles.MVVM
{
    class VM_Main : ObservableObject
    {
        #region Declarations
        Connections _connector = Connections.Instance;
        const string _connectionStatusImageLocation = "/Images/";                                       //The file for the images
        string _connectionStatusLabel = "Not Connected";                                                //Text showing whether or not the program is connected to a serial port
        string _connectButtonText = "Connect";
        string _connectionStatusImageSource = _connectionStatusImageLocation + "No Connection.png";     //The location of the image showing whether we're connected or not

        //App Data
        AppData _appData = AppData.Instance;

        //Page Handling
        object _currentView;    //The page we are currently looking at

        //Command description
        string _commandDescription = "";    //Description of the serial command that is currently being performed

        //Pages
        VM_Keypad _viewModelKeypad;
        VM_Settings _viewModelSettings;
        VM_Motor _viewModelMotor;

        SolidColorBrush _keypadPageColour = new SolidColorBrush(Color.FromArgb(0, 0, 0, 0));
        SolidColorBrush _motorPageColour = new SolidColorBrush(Color.FromArgb(0, 0, 0, 0));
        SolidColorBrush _settingsPageColour = new SolidColorBrush(Color.FromArgb(0, 0, 0, 0));
        #endregion

        #region Constructor
        public VM_Main()
        {
            _connector.OnConnectionOpened += SetConnectionLabel;
            _connector.OnConnectionClosed += SetConnectionLabel;
            _appData.OnCommandDescriptionUpdated += UpdateCommandDescription;
            ScriptHandler.Instance.OnCommandDescriptionUpdated += UpdateCommandDescription;
            _viewModelKeypad= new VM_Keypad();
            _viewModelSettings= new VM_Settings();
            _viewModelMotor= new VM_Motor();

            SelectPage("Motor");
        }
        #endregion

        #region Serial Connection Functions
        private RelayCommand ConnectToSerial1;
        public ICommand ConnectToSerial
        {
            get
            {
                if (ConnectToSerial1 == null)
                {
                    ConnectToSerial1 = new RelayCommand(o =>
                    {
                        _viewModelSettings.SerialConnectErrorMessage = "";

                        if (_viewModelSettings.SelectedCOMPort == null)
                        {
                            //QuickConnect();
                            return;
                        }

                        if (int.TryParse(_viewModelSettings.BaudRate, out int baud) == false)
                        {
                            _viewModelSettings.SerialConnectErrorMessage = "ⓘ Invalid baud rate";
                            return;
                        }

                        if (_connector.IsConnected)
                        {
                            _connector.Disconnect();
                        }
                        else
                        {
                            _connector.ConnectToDevice(_viewModelSettings.SelectedCOMPort, baud, _viewModelSettings.ParityType, _viewModelSettings.StopBits);

                            if (_connector.IsConnected)
                            {
                                _appData.DisplayError("Could not connect to the port.", "Could Not Connect");
                            }
                        }
                    });
                }
                
                return ConnectToSerial1;
            }
        }

        //public void QuickConnect()  //Tries to connect to the most recent port and failing that, tries to connect to any available port
        //{
        //    string[] ports = System.IO.Ports.SerialPort.GetPortNames();

        //    try
        //    {
        //        string loadedCOM = Application.GetCookie(new Uri(System.IO.Path.Combine(System.AppDomain.CurrentDomain.BaseDirectory, @"Properties"))); //If the system has a favourite port, then use that

        //        if (loadedCOM != null)
        //        {
        //            loadedCOM = loadedCOM.Substring(loadedCOM.LastIndexOf('=') + 1);
        //            _connector.ConnectToDevice(loadedCOM, 9600, ParityType.None, StopBitsType.One); //Try to connect to the serial port using the default settings
        //        }
        //    }
        //    catch
        //    {
        //        foreach (string port in ports)
        //        {
        //            _connector.ConnectToDevice(port, 9600, ParityType.None, StopBitsType.One);
        //        }

        //        if (_connector.IsConnected == false)
        //        {
        //            ErrorMessages.DisplayError(ErrorCode.SERIAL_NO_PORT);
        //        }
        //    }
        //}

        void SetConnectionLabel(string label)    //Triggers whenever the serial port successfully connects
        {
            ConnectionStatusLabel = label;
            ConnectionStatusImageSource = _connectionStatusImageLocation + "Connected.png";
        }

        void SetConnectionLabel()    //Triggers whenever the serial port successfully disconnects
        {
            ConnectionStatusLabel = "Not connected";
            ConnectionStatusImageSource = _connectionStatusImageLocation + "No Connection.png";
        }

        #endregion

        #region Page Buttons
        private RelayCommand SelectKeypadPage1;
        public ICommand SelectKeypadPage
        {
            get
            {
                if (SelectKeypadPage1 == null)
                {
                    SelectKeypadPage1 = new RelayCommand(o =>
                    {
                        SelectPage("Keypad");

                    });
                }

                return SelectKeypadPage1;
            }
        }

        private RelayCommand SelectMotorPage1;
        public ICommand SelectMotorPage
        {
            get
            {
                if (SelectMotorPage1 == null)
                {
                    SelectMotorPage1 = new RelayCommand(o =>
                    {
                        SelectPage("Motor");
                    });
                }

                return SelectMotorPage1;
            }
        }

        private RelayCommand SelectSettingsPage1;
        public ICommand SelectSettingsPage
        {
            get
            {
                if (SelectSettingsPage1 == null)
                {
                    SelectSettingsPage1 = new RelayCommand(o =>
                    {
                        SelectPage("Settings");
                    });
                }

                return SelectSettingsPage1;
            }
        }

        void SelectPage(string page)
        {
            KeypadButtonColour = new SolidColorBrush(Color.FromArgb(0, 0, 0, 0));
            MotorButtonColour = new SolidColorBrush(Color.FromArgb(0, 0, 0, 0));
            SettingsButtonColour = new SolidColorBrush(Color.FromArgb(0, 0, 0, 0));

            switch (page)
            {
                case "Keypad":
                    CurrentView = _viewModelKeypad;
                    KeypadButtonColour = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#012359"));
                    break;
                case "Motor":
                    CurrentView = _viewModelMotor;
                    MotorButtonColour = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#012359"));
                    break;
                case "Settings":
                    CurrentView = _viewModelSettings;
                    SettingsButtonColour = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#012359"));
                    break;
            }
        }
        #endregion

        #region Command Description Functions
        void UpdateCommandDescription(string description)
        {
            if(description == "")
            {
                CommandDescription = "";
                return;
            }

            CommandDescription = $"Process: {description}";
        }
        #endregion

        #region Errors
        private RelayCommand HideError1;
        public ICommand HideError
        {
            get
            {
                if (HideError1 == null)
                {
                    HideError1 = new RelayCommand(o =>
                    {
                        _appData.HideErrorDisplay();
                    });
                }

                return HideError1;
            }
        }
        #endregion

        #region Accessors
        public string ConnectionStatusLabel { get => _connectionStatusLabel; set { _connectionStatusLabel = value; OnPropertyChanged(); } }
        public string ConnectionStatusImageSource { get => _connectionStatusImageSource; set { _connectionStatusImageSource = value; OnPropertyChanged(); } }
        public object CurrentView { get { return _currentView; } set { _currentView = value; OnPropertyChanged(); } }
        public string CommandDescription { get => _commandDescription; set { _commandDescription = value; OnPropertyChanged(); } }
        public SolidColorBrush KeypadButtonColour { get { return _keypadPageColour; } set { _keypadPageColour = value; OnPropertyChanged();} }
        public SolidColorBrush MotorButtonColour { get { return _motorPageColour; } set { _motorPageColour = value; OnPropertyChanged();} }
        public SolidColorBrush SettingsButtonColour { get { return _settingsPageColour; } set { _settingsPageColour = value; OnPropertyChanged();} }
        public string ConnectButtonText { get { return _connectButtonText; } set { _connectButtonText = value;OnPropertyChanged(); } }
        #endregion
    }
}
