using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using FA_1To100.Core;
using System.Windows.Media.Media3D;
using System.IO.Ports;
using System.Windows.Threading;
using System.Windows.Markup;
using System.Text.RegularExpressions;

namespace FA_1To100.MVVM.ViewModel
{
    internal class MainViewModel : ObservableObject
    {
        private List<byte> messageBuffer = new List<byte>();
        private const byte LineTerminator = 0x0D;   //ASCII value for \d (carriage return)
        private const byte LineTerminator2 = 0x0A; // ASCII value for '\n' (Line Feed)
        public static Action OnScrollRequest;
        string _serialConnectionStatus = "Connect";
        string _comPort;
        string _connectedPort = "";
        string _baudRate = "4800";
        ParityType _parity = ParityType.Odd;
        StopBitsType _stopBits = StopBitsType.One;

        bool _prependFA = true;
        bool _appendCR = true;
        bool _appendComma = false;

        string _customText;
        string _outputLog;

        public MainViewModel()
        {
            Serial.Instance.SerialPort.DataReceived += ReadOutput;
            MainWindow.PortsRefreshed += CheckCurrentPortState;
        }

        private RelayCommand ConnectToSerial1;
        public ICommand ConnectToSerial
        {
            get
            {
                if (ConnectToSerial1 == null)
                {
                    ConnectToSerial1 = new RelayCommand(o =>
                    {
                        if(Serial.Instance.Serial_Status)
                        {
                            Serial.Instance.Close_Serial_Port();
                        }
                        else
                        {
                            if(!int.TryParse(_baudRate, out int result))
                            {
                                MessageBox.Show("Enter a valid baud rate", "Invalid Baud Rate");
                                return;
                            }

                            Serial.Instance.Open_Serial_Port(_comPort, result, _parity, _stopBits);
                        }

                        if(Serial.Instance.Serial_Status)
                        {
                            SerialConnectionStatus = "Disconnect";
                            _connectedPort = Serial.Instance.SerialPort.PortName;
                        }
                        else
                        {
                            SerialConnectionStatus = "Connect";
                            _connectedPort = "";
                        }
                    });
                }

                return ConnectToSerial1;
            }
        }

        #region Buttons
        private RelayCommand In1;
        public ICommand In
        {
            get
            {
                if (In1 == null)
                {
                    In1 = new RelayCommand(o =>
                    {
                        SendMessage("in");
                    });
                }

                return In1;
            }
        }

        private RelayCommand Out1;
        public ICommand Out
        {
            get
            {
                if (Out1 == null)
                {
                    Out1 = new RelayCommand(o =>
                    {
                        SendMessage("out");
                    });
                }

                return Out1;
            }
        }

        private RelayCommand Stop1;
        public ICommand Stop
        {
            get
            {
                if (Stop1 == null)
                {
                    Stop1 = new RelayCommand(o =>
                    {
                        SendMessage("stop");
                    });
                }

                return Stop1;
            }
        }

        private RelayCommand Preset1;
        public ICommand Preset
        {
            get
            {
                if (Preset1 == null)
                {
                    Preset1 = new RelayCommand(o =>
                    {
                        SendMessage("preset");
                    });
                }

                return Preset1;
            }
        }

        private RelayCommand Store1;
        public ICommand Store
        {
            get
            {
                if (Store1 == null)
                {
                    Store1 = new RelayCommand(o =>
                    {
                        SendMessage("store");
                    });
                }

                return Store1;
            }
        }

        private RelayCommand Home1;
        public ICommand Home
        {
            get
            {
                if (Home1 == null)
                {
                    Home1 = new RelayCommand(o =>
                    {
                        SendMessage("home");
                    });
                }

                return Home1;
            }
        }

        private RelayCommand A1;
        public ICommand A
        {
            get
            {
                if (A1 == null)
                {
                    A1 = new RelayCommand(o =>
                    {
                        SendMessage("a");
                    });
                }

                return A1;
            }
        }

        private RelayCommand B1;
        public ICommand B
        {
            get
            {
                if (B1 == null)
                {
                    B1 = new RelayCommand(o =>
                    {
                        SendMessage("b");
                    });
                }

                return B1;
            }
        }

        private RelayCommand C1;
        public ICommand C
        {
            get
            {
                if (C1 == null)
                {
                    C1 = new RelayCommand(o =>
                    {
                        SendMessage("c");
                    });
                }

                return C1;
            }
        }

        private RelayCommand D1;
        public ICommand D
        {
            get
            {
                if (D1 == null)
                {
                    D1 = new RelayCommand(o =>
                    {
                        SendMessage("d");
                    });
                }

                return D1;
            }
        }

        private RelayCommand E1;
        public ICommand E
        {
            get
            {
                if (E1 == null)
                {
                    E1 = new RelayCommand(o =>
                    {
                        SendMessage("e");
                    });
                }

                return E1;
            }
        }

        private RelayCommand F1;
        public ICommand F
        {
            get
            {
                if (F1 == null)
                {
                    F1 = new RelayCommand(o =>
                    {
                        SendMessage("f");
                    });
                }

                return F1;
            }
        }

        private RelayCommand Left1;
        public ICommand Left
        {
            get
            {
                if (Left1 == null)
                {
                    Left1 = new RelayCommand(o =>
                    {
                        SendMessage("left");
                    });
                }

                return Left1;
            }
        }

        private RelayCommand Right1;
        public ICommand Right
        {
            get
            {
                if (Right1 == null)
                {
                    Right1 = new RelayCommand(o =>
                    {
                        SendMessage("right");
                    });
                }

                return Right1;
            }
        }

        private RelayCommand StoreA1;
        public ICommand StoreA
        {
            get
            {
                if (StoreA1 == null)
                {
                    StoreA1 = new RelayCommand(o =>
                    {
                        SendMessage("store a");
                    });
                }

                return StoreA1;
            }
        }

        private RelayCommand StoreB1;
        public ICommand StoreB
        {
            get
            {
                if (StoreB1 == null)
                {
                    StoreB1 = new RelayCommand(o =>
                    {
                        SendMessage("store b");
                    });
                }

                return StoreB1;
            }
        }

        private RelayCommand StoreC1;
        public ICommand StoreC
        {
            get
            {
                if (StoreC1 == null)
                {
                    StoreC1 = new RelayCommand(o =>
                    {
                        SendMessage("store c");
                    });
                }

                return StoreC1;
            }
        }

        private RelayCommand StoreD1;
        public ICommand StoreD
        {
            get
            {
                if (StoreD1 == null)
                {
                    StoreD1 = new RelayCommand(o =>
                    {
                        SendMessage("store d");
                    });
                }

                return StoreD1;
            }
        }

        private RelayCommand StoreE1;
        public ICommand StoreE
        {
            get
            {
                if (StoreE1 == null)
                {
                    StoreE1 = new RelayCommand(o =>
                    {
                        SendMessage("store e");
                    });
                }

                return StoreE1;
            }
        }

        private RelayCommand StoreF1;
        public ICommand StoreF
        {
            get
            {
                if (StoreF1 == null)
                {
                    StoreF1 = new RelayCommand(o =>
                    {
                        SendMessage("store f");
                    });
                }

                return StoreF1;
            }
        }

        private RelayCommand SendCustom1;
        public ICommand SendCustom
        {
            get
            {
                if (SendCustom1 == null)
                {
                    SendCustom1 = new RelayCommand(o =>
                    {
                        if(_customText == "FLASH" && !PrependFA && !AppendCR)
                        {
                            OnScrollRequest?.Invoke();
                            return;
                        }

                        SendMessage(CustomText);
                    });
                }

                return SendCustom1;
            }
        }
        #endregion

        private RelayCommand ClearOutput1;
        public ICommand ClearOutput
        {
            get
            {
                if (ClearOutput1 == null)
                {
                    ClearOutput1 = new RelayCommand(o =>
                    {
                        OutputLog = "";
                    });
                }

                return ClearOutput1;
            }
        }
        void SendMessage(string message)
        {
            if (Serial.Instance.Serial_Status == false)
            {
                MessageBox.Show("Connect to serial first", "No Connection");
                return;
            }

            if (_prependFA)
            {
                message = "fa_" + message;
            }

            if (_appendCR)
            {
                message = message + (char)0x0D;
            }
            else if (_appendComma)
            {
                message = message + (char)0x2C;
            }

            Serial.Instance.SendMessage(Encoding.UTF8.GetBytes(message));
        }

        private void ReadOutput(object sender, SerialDataReceivedEventArgs e)
        {
            string output = Serial.Instance.SerialPort.ReadExisting();

            // Check if the data is alphanumeric
            if (IsValid(output))
            {
                OutputLog += output;
            }
            else
            {
                // Convert the data to hexadecimal
                OutputLog += $"{ConvertToHex(output)}\r";
            }
        }

        private bool IsValid(string data)
        {
            // Check if all characters are alphanumeric (letters or digits)
            foreach (char c in data)
            {
                if (!char.IsLetterOrDigit(c) && c != '_' && c != '\r' && c != '\n')
                {
                    return false;
                }
            }
            return true;
        }

        private string ConvertToHex(string data)
        {
            // Convert the data (bytes) to a hexadecimal string
            StringBuilder hexString = new StringBuilder();
            foreach (char c in data)
            {
                hexString.AppendFormat("{0:X2} ", (int)c);  // Format each character as hex
            }
            return Regex.Replace(hexString.ToString().Trim(), @"[^0-9a-fA-F]", "", System.Text.RegularExpressions.RegexOptions.IgnoreCase);
        }

        void CheckCurrentPortState()
        {
            if(_connectedPort == "")
            {
                return;
            }

            foreach(string port in SerialPort.GetPortNames())
            {
                if(port == _connectedPort)
                {
                    return;
                }
            }

            MessageBox.Show($"port {Serial.Instance.SerialPort.PortName} has been disconnected.", "Disconnected port");
            Serial.Instance.Close_Serial_Port();
            SerialConnectionStatus = "Connect";
            _connectedPort = "";
        }

        public string SerialConnectionStatus { get { return _serialConnectionStatus; } set { _serialConnectionStatus = value; OnPropertyChanged(); } }
        public string CustomText { get { return _customText;} set { _customText = value; OnPropertyChanged(); } }
        public string COMPort { get { return _comPort; } set { _comPort = value; OnPropertyChanged(); } }
        public string BaudRate { get { return _baudRate; } set { _baudRate= value; OnPropertyChanged(); } }
        public ParityType Parity { get { return _parity; } set { _parity = value;OnPropertyChanged(); } }
        public StopBitsType StopBits { get { return _stopBits; } set { _stopBits = value; OnPropertyChanged(); } }
        public bool PrependFA { get { return _prependFA; } set { _prependFA = value; OnPropertyChanged();} }
        public bool AppendCR { get { return _appendCR;} set { _appendCR = value; OnPropertyChanged(); } }
        public bool AppendComma { get { return _appendComma; } set { _appendComma = value; OnPropertyChanged(); } }
        public string OutputLog { get { return _outputLog; } set { _outputLog = value; OnPropertyChanged(); } }
    }
}
