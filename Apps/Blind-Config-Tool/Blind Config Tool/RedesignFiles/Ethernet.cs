using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using OfficeOpenXml.FormulaParsing.Excel.Functions.Logical;

namespace Blind_Config_Tool.RedesignFiles
{
    public class Ethernet
    {
        private struct UdpState
        {
            public UdpClient udpClient;
            public IPEndPoint endpoint;
        }

        private enum AnnouncePacketFields
        {
            None = 0,
            Truncated,
            MACAddr,
            MACName,
            HostName,
            IPv4Addr,
            IPv6Unicast,
            IPv6Multicast,
            IPv6DefaultRouter,
            IPv6DefaultGateway,
        }

        private static Ethernet _instance;    //The single instance of this class
        private static readonly object _lock = new object();    //The lock (used to ensure that only one instance of this class is created)

        public TcpClient TCPClient = new TcpClient();    //The TCP object
        public NetworkStream DataStream;

        public UdpClient udpClient;

        #region Events
        public event Action<bool, string, int> OnConnectionStateChanged;
        public event Action<EthernetDevice> OnAddNewEthernetDevice;
        #endregion 

        public static Ethernet Instance
        {
            get
            {
                if (_instance == null)   //Check whether an instance has been created and if not, try to create one
                {
                    lock (_lock) //Prevent the thread from continuing on if another thread is in the process of creating an instance
                    {
                        if (_instance == null)   //Second check inside the lock
                        {
                            _instance = new Ethernet();   //Create the instance
                        }
                    }
                }

                return _instance;
            }
        }

        public Ethernet()
        {
            TCPClient.ReceiveTimeout = 3000;
            UDP_DiscoverDevices();
        }

        public async Task<bool> TryConnect(string ip, int port)
        {
            try
            {
                TCPClient = new TcpClient();
                await TCPClient.ConnectAsync(ip, port);
                OnConnectionStateChanged?.Invoke(true, ip, port);
                return true;  // Connection succeeded
            }
            catch (Exception)
            {
                OnConnectionStateChanged?.Invoke(false, ip, port);
                return false;  // Connection failed
            }
        }

        public void CloseConnection()
        {
            if (TCPClient.GetStream() != null)
            {
                TCPClient.GetStream().Close();
                TCPClient.Close();   
            }
            OnConnectionStateChanged?.Invoke(false, "", 0);
        }

        public bool AreTCPSettingsValid(string ip, int port)
        {
            return IsValidIPAddress(ip) && IsValidPort(port);
        }

        private bool IsValidIPAddress(string ip)
        {
            return IPAddress.TryParse(ip, out _);
        }

        private bool IsValidPort(int port)
        {
            return port >= 1 && port <= 65535;
        }

        public void SendMessage(byte[] msg)
        {
            if (TCPClient.Connected)
            {
                try
                {
                    DataStream = TCPClient.GetStream();
                    DataStream.Write(msg, 0, msg.Length);
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"Error sending message: {ex.Message}");
                }
            }
        }

        public byte[] ReadResponse()
        {
            if (TCPClient.Connected)
            {
                try
                {
                    byte[] buffer = new byte[300];

                    if (!TCPClient.GetStream().DataAvailable)
                    {
                        return buffer;
                    }

                    int bytesRead = TCPClient.GetStream().Read(buffer, 0, buffer.Length);

                    return buffer;
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"Error reading response: {ex.Message}");
                }
            }
            return null;
        }

        public void UDP_DiscoverDevices()
        {
            udpClient = new UdpClient(30303);
            udpClient.EnableBroadcast = true;

            IPEndPoint broadcast = new IPEndPoint(IPAddress.Broadcast, 30303);
            IPEndPoint any = new IPEndPoint(IPAddress.Any, 0);

            byte[] sendBytes = Encoding.ASCII.GetBytes("Discovery: Who is out there?\0\n");
            udpClient.Send(sendBytes, sendBytes.Length, broadcast);

            UdpState udpState = new UdpState { udpClient = udpClient, endpoint = any };
            udpClient.BeginReceive(new AsyncCallback(UDP_ReceiveData), udpState);
        }

        public void UDP_ReceiveData(IAsyncResult ar)
        {
            if (!(ar.AsyncState is UdpState udpState))
                return;

            UdpClient udpClient = udpState.udpClient;
            IPEndPoint endpoint = new IPEndPoint(IPAddress.Any, 0); // Ensure the endpoint is properly initialized.

            var result = udpClient.EndReceive(ar, ref endpoint);

            string returnData = Encoding.ASCII.GetString(result);

            ProcessUdpPacket(result, endpoint);

            udpClient.BeginReceive(new AsyncCallback(UDP_ReceiveData), udpState);
        }

        private void ProcessUdpPacket(byte[] packet, IPEndPoint endpoint)
        {
            // If the broadcast message doesnt contain "FA_DEVICE" then ignore that packet.
            if (!Encoding.ASCII.GetString(packet).Contains("FA_DEVICE"))
            {
                return;
            }

            Dictionary<byte, byte[]> fields = new Dictionary<byte, byte[]>();

            int i = 0;

            while (i < packet.Length)
            {
                byte type = packet[i++];
                List<byte> data = new List<byte>();

                while (i < packet.Length - 1)
                {
                    // Field terminator
                    if (packet[i] == 0x0D && packet[i + 1] == 0x0A)
                    {
                        i += 2;
                        break;
                    }

                    data.Add(packet[i++]);
                }

                fields[type] = data.ToArray();
            }

            string mac = "";
            string ip = "";
            string name = "";

            foreach (var kvp in fields)
            {
                switch (kvp.Key)
                {
                    case 0x02: // MAC address
                        mac = BitConverter.ToString(kvp.Value);
                        break;
                    case 0x03: // Hostname
                    case 0x04: // Device name or ID
                        name = Encoding.ASCII.GetString(kvp.Value).Trim();
                        break;
                    case 0x05: // IP address
                        ip = string.Join(".", kvp.Value);
                        break;
                }
            }

            EthernetDevice dev = new EthernetDevice(name, ip, mac);
            OnAddNewEthernetDevice?.Invoke(dev);
        }

        public bool IsConnected { get { return TCPClient.Client != null && TCPClient.Connected; } }
    }
}