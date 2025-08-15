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

namespace FA_1To100.MVVM.ViewModel
{
    internal class MainViewModel : ObservableObject
    {
        string _serialConnectionStatus = "Connect";
        string _messageText = "1";
        string _comPort;
        string _baudRate = "4800";
        ParityType _parity = ParityType.Odd;
        StopBitsType _stopBits = StopBitsType.One;

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
                        }
                        else
                        {
                            SerialConnectionStatus = "Connect";
                        }
                    });
                }

                return ConnectToSerial1;
            }
        }

        private RelayCommand SendMessage1;
        public ICommand SendMessage
        {
            get
            {
                if(SendMessage1 == null)
                {
                    SendMessage1 = new RelayCommand(o =>
                    {
                        if(int.TryParse(_messageText, out int res))
                        {
                            if(Serial.Instance.Serial_Status == false)
                            {
                                MessageBox.Show("Connect to serial first", "No Connection");
                                return;
                            }

                            Serial.Instance.SendMessage(Encoding.UTF8.GetBytes("fa_" + _messageText));
                        }
                        else
                        {
                            MessageBox.Show("Invalid number. Please enter a number between 1 and 100", "Invalid Number");
                        }
                    });
                }

                return SendMessage1;
            }
        }

        public string SerialConnectionStatus { get { return _serialConnectionStatus; } set { _serialConnectionStatus = value; OnPropertyChanged(); } }
        public string MessageText { get { return _messageText;} set { _messageText = value; OnPropertyChanged(); } }
        public string COMPort { get { return _comPort; } set { _comPort = value; OnPropertyChanged(); } }
        public string BaudRate { get { return _baudRate; } set { _baudRate= value; OnPropertyChanged(); } }
        public ParityType Parity { get { return _parity; } set { _parity = value;OnPropertyChanged(); } }
        public StopBitsType StopBits { get { return _stopBits; } set { _stopBits = value; OnPropertyChanged(); } }
    }
}
