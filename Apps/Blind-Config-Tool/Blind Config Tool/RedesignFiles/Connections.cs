using Blind_Config_Tool.Core;
using System;
using System.Threading.Tasks;
using System.Windows.Threading;

namespace Blind_Config_Tool.RedesignFiles
{
    public enum ConnectionMode
    {
        SERIAL,
        ETHERNET
    }

    internal class Connections
    {
        private static Connections _instance;
        private static readonly object _lock = new object();    //The lock (used to ensure that only one instance of this class is created)

        Serial _serial;
        Ethernet _tcp;
        private int dotCount = 0;
        private DispatcherTimer _animate_text_timer;

        ConnectionMode ConnectionMode = ConnectionMode.SERIAL;

        public event Action<string> OnConnectionOpened;
        public event Action OnConnectionClosed;

        #region Singleton Instantiation
        public static Connections Instance
        {
            get
            {
                if (_instance == null)   //Check whether an instance has been created and if not, try to create one
                {
                    lock (_lock) //Prevent the thread from continuing on if another thread is in the process of creating an instance
                    {
                        if (_instance == null)   //Second check inside the lock
                        {
                            _instance = new Connections();   //Create the instance
                        }
                    }
                }

                return _instance;
            }
        }
        #endregion

        private Connections() 
        {
            _serial = Serial.Instance;
            _tcp = Ethernet.Instance;
        }

        public void ChangeConnectionMode(ConnectionMode mode)
        {
            Disconnect();
            ConnectionMode = mode;
        }

        public async Task<bool> ConnectToDevice(string ip, int port)
        {
            ConnectionMode = ConnectionMode.ETHERNET;
            if (_serial.Serial_Status)
            {
                _serial.Close_Serial_Port();
            }

            // Animate dots to show the app hasnt frozen
            _animate_text_timer = new DispatcherTimer
            {
                Interval = TimeSpan.FromMilliseconds(500)
            };

            _animate_text_timer.Tick += (sender, e) =>
            {
                dotCount = (dotCount + 1) % 3; 
                OnConnectionOpened?.Invoke("Attempting to connect." + new string('.', dotCount));
            };

            _animate_text_timer.Start();

            await _tcp.TryConnect(ip, port);

            if (_tcp.IsConnected)
            {
                _animate_text_timer.Stop();
                OnConnectionOpened?.Invoke($"Connected to device via TCP/IP (IP: {ip} | Port: {port})");
            }
            else
            {
                _animate_text_timer.Stop();
                OnConnectionClosed?.Invoke();
            }

            return IsConnected;
        }

        public bool ConnectToDevice(string port, int baud, ParityType parity, StopBitsType stopBits)
        {
            ConnectionMode = ConnectionMode.SERIAL;

            if (_tcp.IsConnected)
            {
                _tcp.CloseConnection();
            }

            _serial.Open_Serial_Port(port, baud, parity, stopBits);

            if (_serial.Serial_Status)
            {
                OnConnectionOpened?.Invoke($"Connected to device via serial (port: {port})");
            }

            return IsConnected;
        }

        public void Disconnect()
        {
            switch(ConnectionMode)
            {
                case ConnectionMode.SERIAL:
                    _serial.Close_Serial_Port();
                    break;
                case ConnectionMode.ETHERNET:
                    _tcp.CloseConnection();
                    break;
            }

            OnConnectionClosed?.Invoke();
        }

        public void ClearData()
        {
            switch (ConnectionMode)
            {
                case ConnectionMode.SERIAL:
                    _serial.ClearBuffers();
                    break;
            }
        }

        public void SendMessage(byte[] msg)
        {
            switch (ConnectionMode)
            {
                case ConnectionMode.SERIAL:
                    _serial.SendMessage(msg);
                    break;
                case ConnectionMode.ETHERNET:
                    _tcp.SendMessage(msg);
                    break;
            }
        }

        public byte[] ReadResponse()
        { 
            switch (ConnectionMode)
            {
                case ConnectionMode.SERIAL:
                    return _serial.ReadResponse();
                case ConnectionMode.ETHERNET:
                    return _tcp.ReadResponse();
                default: 
                    return null;
            }
        }

        public bool IsConnected { get { return _serial.Serial_Status || _tcp.IsConnected; } }
    }
}
