using Blind_Config_Tool.Core;
using System;
using System.IO.Ports;
using System.Threading;
using System.Windows;

namespace Blind_Config_Tool.RedesignFiles
{
    public class Serial
    {
        #region Declarations
        private static Serial _instance;    //The single instance of this class
        private static readonly object _lock = new object();    //The lock (used to ensure that only one instance of this class is created)

        public SerialPort SerialPort = new SerialPort();    //The serial port object
        byte[] _rxBytes = new byte[300];    //The buffer for the bytes that come in from the serial port
        int _rxIndex = 0;
        int _lastRX = 0;
        #endregion

        #region Events
        public event Action<bool, string> OnConnectionStateChanged;
        #endregion

        #region Singleton Instantiation
        private Serial()    //Private constructor to prevent outside instantiation
        {
            Console.WriteLine("Serial instantiated");
        }

        public static Serial Instance
        {
            get
            {
                if (_instance == null)   //Check whether an instance has been created and if not, try to create one
                {
                    lock (_lock) //Prevent the thread from continuing on if another thread is in the process of creating an instance
                    {
                        if (_instance == null)   //Second check inside the lock
                        {
                            _instance = new Serial();   //Create the instance
                        }
                    }
                }

                return _instance;
            }
        }
        #endregion

        #region Opening/Closing The Port
        public bool Open_Serial_Port(string comPort, int baud, ParityType parity, StopBitsType stopBits)
        {
            try
            {
                string portName = comPort;                                      //Set up all the data for the port that we're trying to open
                SerialPort.PortName = portName;
                SerialPort.BaudRate = baud;
                SerialPort.Parity = ParityTypeToParity(parity);
                SerialPort.StopBits = StopBitsTypeToStopBits(stopBits);
                SerialPort.WriteTimeout = 500;
                SerialPort.ReadTimeout = 500;
                SerialPort.Handshake = Handshake.None;
                SerialPort.Encoding = System.Text.Encoding.GetEncoding(28591);

                SerialPort.Open();  //Attempt to open the port

                SerialPort.DataReceived += new SerialDataReceivedEventHandler(Receive); //Subscribe to the dataReceived handler

                SerialPort.DiscardInBuffer();   //Clear the buffers
                SerialPort.DiscardOutBuffer();

                OnConnectionStateChanged?.Invoke(true, comPort);   //Fire off the event showing that we've connected to a port

                return true;
            }
            catch
            {
                return false;
            }
        }

        public bool Close_Serial_Port()
        {
            if (SerialPort.IsOpen)
            {
                try
                {
                    SerialPort.Close();
                    SerialPort.DataReceived -= Receive; //Unsubscribe to the dataReceived handler

                    OnConnectionStateChanged?.Invoke(false, "");   //Fire off the event showing that we've disconnected from the port

                    return true;
                }
                catch (Exception ex)
                {
                    AppData.Instance.DisplayError(ex.Message, "Close Serial Failed");
                    return false;
                }
            }

            return false;
        }

        public void ClearBuffers()
        {
            try
            {
                SerialPort.DiscardInBuffer();
                SerialPort.DiscardOutBuffer();

                for (int i = 0; i < _rxBytes.Length; i++)
                {
                    _rxBytes[i] = 0;
                }

                _rxIndex = 0;
            }
            catch
            {

            }
        }
        #endregion

        #region Receiving Serial Data
        private void Receive(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            while (SerialPort.BytesToRead > 0)
            {
                try
                {
                    _rxBytes[_rxIndex] = (byte)SerialPort.ReadByte();
                    _rxIndex++;
                    if (_rxIndex >= _rxBytes.Length)
                    {
                        _rxIndex = 0;
                    }
                }
                catch (System.IndexOutOfRangeException)
                {
                    _rxIndex = 0;
                }
            }
        }

        public bool IsSerialDataAvailable()
        {
            for (int i = 0; i < _rxBytes.Length; i++)
            {
                if (_rxBytes[i] != 0)
                {
                    return true;
                }
            }
            return false;
        }
        #endregion

        #region Reading Serial Data
        public byte[] GetSerialData(bool clearData)
        {
            byte[] byteRetVal = _rxBytes;

            if (clearData) { Array.Clear(_rxBytes, 0, _rxBytes.Length); }

            return byteRetVal;
        }

        public void RemoveFromRxBuffer(int start, byte length)
        {
            byte[] dest = new byte[_rxBytes.Length];
            if (start > 0)
            {
                Array.Copy(_rxBytes, 0, dest, 0, start);
            }

            if (start < _rxBytes.Length - 1)
            {
                Array.Copy(_rxBytes, start + length, dest, start, _rxBytes.Length - (start + length));
            }

            bool zeroCheck = false;

            for (int i = 0; i < length; i++)
            {
                if (dest[i] != 0)
                {
                    zeroCheck = true;
                }
            }

            if (zeroCheck == false)
            {
                zeroCheck = false;
            }

            _rxBytes = dest;
        }

        public byte GetRxByte(int index)
        {
            return _rxBytes[index];
        }

        public byte[] ReadResponse()
        {
            int difference = ((_rxIndex + (_rxBytes.Length - _lastRX)) % _rxBytes.Length);
            byte[] retVal = new byte[difference];

            for (int i = 0; i < difference; i++)
            {
                if (i + _lastRX >= _rxBytes.Length)
                {
                    retVal[i] = _rxBytes[(i + _lastRX) - _rxBytes.Length];
                }
                else
                {
                    retVal[i] = _rxBytes[i + _lastRX];
                }
            }

            return _rxBytes;
        }
        #endregion

        #region Sending Serial Data
        public void SendMessage(byte[] sendData)
        {
            if (SerialPort.IsOpen)
            {
                try
                {
                    SerialPort.Write(sendData, 0, sendData.Length);
                    Thread.Sleep(1);
                }
                catch (Exception ex)
                {
                    AppData.Instance.DisplayError(ex.Message, "Serial Write Error");
                }
            }
            else
            {
                Console.WriteLine("Port not open");
            }
        }
        #endregion

        #region Conversions
        private Parity ParityTypeToParity(ParityType parity)
        {
            Parity retVal = Parity.None;

            switch (parity)
            {
                case ParityType.None:
                    retVal = Parity.None;
                    break;
                case ParityType.Odd:
                    retVal = Parity.Odd;
                    break;
                case ParityType.Even:
                    retVal = Parity.Even;
                    break;
                case ParityType.Space:
                    retVal = Parity.Space;
                    break;
                case ParityType.Mark:
                    retVal = Parity.Mark;
                    break;
            }

            return retVal;
        }

        private StopBits StopBitsTypeToStopBits(StopBitsType stopBits)
        {
            StopBits retVal = StopBits.None;

            switch (stopBits)
            {
                case StopBitsType.None:
                    retVal = StopBits.None;
                    break;
                case StopBitsType.One:
                    retVal = StopBits.One;
                    break;
                case StopBitsType.OnePointFive:
                    retVal = StopBits.OnePointFive;
                    break;
                case StopBitsType.Two:
                    retVal = StopBits.Two;
                    break;
            }

            return retVal;
        }
        #endregion

        #region Accessors
        public bool Serial_Status
        {
            get { return SerialPort.IsOpen; }
        }
        #endregion
    }
}
