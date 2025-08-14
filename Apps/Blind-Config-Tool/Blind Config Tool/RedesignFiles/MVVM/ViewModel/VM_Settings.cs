using Blind_Config_Tool.Core;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;

namespace Blind_Config_Tool.RedesignFiles.MVVM.ViewModel
{
    internal class VM_Settings : ObservableObject
    {
        #region Declarations
        ScriptHandler _scriptHandler;
        AppData _appData;
        Connections _connector = Connections.Instance;

        //SERIAL SETTINGS
        string _selectedCOMPort;
        string _baudRate = "9600";
        ParityType _parityType = ParityType.None;
        StopBitsType _stopBits = StopBitsType.One;
        string _conToggleButtonName = "Connect";
        string _serialConnectErrorMessage;

        //TCP SETTINGS
        string _ethernetConnection_IP_1;
        string _ethernetConnection_IP_2;
        string _ethernetConnection_IP_3;
        string _ethernetConnection_IP_4;
        string _ethernetConnection_Port = "9670";
        string _ethernetConnectErrorMessage;
        ObservableCollection<EthernetDevice> _ethernetDevices = new ObservableCollection<EthernetDevice>();
        int _ethernetDevices_SelectedIndex = -1;

        //IP CONFIGURATION
        ObservableCollection<DeviceData> _devices;
        int _ipDeviceIndex;
        string _ipConfigErrorMessage;
        string _ipPart1 = "0";
        string _ipPart2 = "0";
        string _ipPart3 = "0";
        string _ipPart4 = "0";

        string _subnetPart1 = "255";
        string _subnetPart2 = "255";
        string _subnetPart3 = "255";
        string _subnetPart4 = "0";

        string _portNumber = "0";
        bool _isDHCP = true;
        bool _isStatic;
        //MAC ADDRESS
        string _password;
        string _correctPassword;
        string _macPasswordError;

        int _macKeypadIndex;
        string _macAddressErrorMessage;
        string _macAddressPart1;
        string _macAddressPart2;
        string _macAddressPart3;
        string _macAddressPart4;
        string _macAddressPart5;
        string _macAddressPart6;

        //IP LIST
        string _addIPEntry_IP_part1 = "0";
        string _addIPEntry_IP_part2 = "0";
        string _addIPEntry_IP_part3 = "0";
        string _addIPEntry_IP_part4 = "0";

        string _addIPEntry_Subnet_part1 = "255";
        string _addIPEntry_Subnet_part2 = "255";
        string _addIPEntry_Subnet_part3 = "255";
        string _addIPEntry_Subnet_part4 = "0";

        string _addIPEntry_Port = "9670";

        string _ipListConfigErrorMessage;
        int _selectedEntry;
        ObservableCollection<IPListEntry> _ipList;
        Visibility _moreIPSettingsButtonVisibility = Visibility.Collapsed;
        Visibility _lessIPSettingsButtonVisibility = Visibility.Visible;
        bool _canPerformActions = true;

        //COMM SETTINGS
        bool _commType_Serial = true;
        Visibility _commType_Serial_Visibility = Visibility.Visible;
        bool _commType_Ethernet;
        Visibility _commType_Ethernet_Visibility = Visibility.Collapsed;

        bool _tcpTargetType_Manual = true;
        Visibility _tcpTargetType_Manual_Visibility = Visibility.Visible;
        bool _tcpTargetType_FromList;
        Visibility _tcpTargetType_FromList_Visibility = Visibility.Collapsed;
        #endregion

        #region Events
        public static event EventHandler<string> ErrorDisplayed;
        #endregion

        #region Constructor
        public VM_Settings()
        {
            _scriptHandler = ScriptHandler.Instance;
            _appData = AppData.Instance;
            _appData.OnDeviceListUpdated += UpdateDeviceList;
            _connector.OnConnectionOpened += SetConnectButtonDisconnect;
            _connector.OnConnectionClosed += SetConnectButtonConnect;
            ResponseActions.OnIPRetrieved += UpdateIPDetails;
            _scriptHandler.OnToggleCanClearState += UpdateEnableState;
            PreprocessActions.OnServerIPRequest += RetrieveServerIPDetails;
            ResponseActions.OnRequestIPListCount += RetrieveIPListCount;
            _ipList = new ObservableCollection<IPListEntry>() { new IPListEntry() };
            Ethernet.Instance.OnAddNewEthernetDevice += AddEthernetDeviceToList;
        }
        #endregion

        #region Serial Connection Settings
        private RelayCommand ConnectToPort1;
        public ICommand ConnectToPort
        {
            get
            {
                if (ConnectToPort1 == null)
                {
                    ConnectToPort1 = new RelayCommand(o =>
                    {
                        SerialConnectErrorMessage = "";

                        if (SelectedCOMPort == null)
                        {
                            SerialConnectErrorMessage = "ⓘ Invalid COM port";
                            ErrorDisplayed?.Invoke(this, "Serial Connection");
                            return;
                        }

                        if (int.TryParse(BaudRate, out int baud) == false)
                        {
                            SerialConnectErrorMessage = "ⓘ Invalid baud rate";
                            ErrorDisplayed?.Invoke(this, "Serial Connection");
                            return;
                        }

                        if (_connector.IsConnected)
                        {
                            _connector.Disconnect();
                        }
                        else
                        {
                            _connector.ConnectToDevice(SelectedCOMPort, baud, ParityType, StopBits);

                            if (!_connector.IsConnected)
                            {
                                _appData.DisplayError("Could not connect to the port.", "Could Not Connect");
                            }
                        }
                    });
                }

                return ConnectToPort1;
            }
        }

        void SetConnectButtonConnect()
        {
            ConToggleButtonName = "Connect";
        }

        void SetConnectButtonDisconnect(string _)
        {
            ConToggleButtonName = "Disconnect";
        }
        #endregion

        #region Ethernet Connection Settings
        private RelayCommand ConnectToEthernet1;
        public ICommand ConnectToEthernet
        {
            get
            {
                if (ConnectToEthernet1 == null)
                {
                    ConnectToEthernet1 = new RelayCommand(async o =>
                    {
                        EthernetConnectErrorMessage = "";

                        if (!int.TryParse(EthernetConnection_Port, out int port) || port < 0 || port > 65535)
                        {
                            EthernetConnectErrorMessage = "ⓘ Invalid port";
                            ErrorDisplayed?.Invoke(this, "Ethernet Connection");
                            return;
                        }

                        if (TCPTargetType_Manual)
                        {
                            await ConnectToManualIP(port);
                        }
                        else
                        {
                            await ConnectToIPListElement(port);
                        }
                    });
                }

                return ConnectToEthernet1;
            }
        }

        async Task ConnectToManualIP(int port)
        {
            if (!int.TryParse(EthernetConnection_IP_1, out int ip1) ||
                !int.TryParse(EthernetConnection_IP_2, out int ip2) ||
                !int.TryParse(EthernetConnection_IP_3, out int ip3) ||
                !int.TryParse(EthernetConnection_IP_4, out int ip4))
            {
                EthernetConnectErrorMessage = "ⓘ Invalid IP target";
                ErrorDisplayed?.Invoke(this, "Ethernet Connection");
                return;
            }

            string ip = $"{ip1}.{ip2}.{ip3}.{ip4}";

            if (_connector.IsConnected)
            {
                _connector.Disconnect();
            }
            else
            {
                bool connected = await _connector.ConnectToDevice(ip, port);

                if (!connected)
                {
                    _appData.DisplayError("Could not connect to the port.", "Could Not Connect");
                }
            }
        }

        async Task ConnectToIPListElement(int port)
        {
            if (_ethernetDevices.Count <= 0 || _ethernetDevices_SelectedIndex < 0)
            {
                EthernetConnectErrorMessage = "ⓘ No Target Selected";
                ErrorDisplayed?.Invoke(this, "Ethernet Connection");
                return;
            }

            if (_connector.IsConnected)
            {
                _connector.Disconnect();
            }
            else
            {
                bool connected = await _connector.ConnectToDevice(_ethernetDevices[_ethernetDevices_SelectedIndex].IP, port);

                if (!connected)
                {
                    _appData.DisplayError("Could not connect to the port.", "Could Not Connect");
                }
            }
        }
        #endregion

        #region IP Configuration
        private RelayCommand SearchForDevices1;
        public ICommand SearchForDevices
        {
            get
            {
                if (SearchForDevices1 == null)
                {
                    SearchForDevices1 = new RelayCommand(o =>
                    {
                        if (_appData.ResetValidationOnSearch)
                        {
                            if(_appData.Devices == null)
                            {
                                _appData.Devices = new List<DeviceData>();
                            }

                            foreach (DeviceData dev in _appData.Devices)
                            {
                                dev.Validated = false;
                            }
                        }

                        _appData.SetTimer(AppData.Timers.VALIDATION_CHECK, _appData.ValidationCheckTime);
                        _scriptHandler.StartScript(Messages.SEARCH_FOR_ALL_DEVICES);
                    });
                }

                return SearchForDevices1;
            }
        }

        void UpdateDeviceList()
        {
            DeviceList = new ObservableCollection<DeviceData>(_appData.Keypads);

            foreach (Motor motor in _appData.Motors)
            {
                DeviceList.Add(motor);
            }
        }

        private RelayCommand SaveIPSettings1;
        public ICommand SaveIPSettings
        {
            get
            {
                if (SaveIPSettings1 == null)
                {
                    SaveIPSettings1 = new RelayCommand(o =>
                    {
                        IPConfigErrorMessage = "";

                        if (_devices == null || _devices.Count <= 0 || _ipDeviceIndex < 0)
                        {
                            IPConfigErrorMessage = "ⓘ No target selected";
                            ErrorDisplayed?.Invoke(this, "IP Configuration");
                            return;
                        }

                        if (IsStatic)
                        {
                            if (long.TryParse(IPAddress, out long ip) == false)
                            {
                                IPConfigErrorMessage = "ⓘ Invalid IP address";
                                ErrorDisplayed?.Invoke(this, "IP Configuration");
                                return;
                            }

                            if (long.TryParse(Subnet, out long subnet) == false)
                            {
                                IPConfigErrorMessage = "ⓘ Invalid subnet mask";
                                ErrorDisplayed?.Invoke(this, "IP Configuration");
                                return;
                            }
                        }

                        if (int.TryParse(PortNumber, out int port) == false)
                        {
                            port = 0;
                        }

                        _appData.LastSentIP[0] = int.Parse(IPAddressPart1);
                        _appData.LastSentIP[1] = int.Parse(IPAddressPart2);
                        _appData.LastSentIP[2] = int.Parse(IPAddressPart3);
                        _appData.LastSentIP[3] = int.Parse(IPAddressPart4);

                        _appData.LastSentSubnet[0] = int.Parse(SubnetPart1);
                        _appData.LastSentSubnet[1] = int.Parse(SubnetPart2);
                        _appData.LastSentSubnet[2] = int.Parse(SubnetPart3);
                        _appData.LastSentSubnet[3] = int.Parse(SubnetPart4);

                        _appData.LastSentDHCP = IsDHCP;
                        _appData.LastSentPort = int.Parse(PortNumber);

                        _scriptHandler.StartScript(Messages.SET_IP_ADDRESS);    //Send the IP address
                    });
                }

                return SaveIPSettings1;
            }
        }

        private RelayCommand RetrieveIPSettings1;
        public ICommand RetrieveIPSettings
        {
            get
            {
                if (RetrieveIPSettings1 == null)
                {
                    RetrieveIPSettings1 = new RelayCommand(o =>
                    {
                        IPConfigErrorMessage = "";

                        if (_devices == null || _devices.Count <= 0 || _ipDeviceIndex < 0)
                        {
                            IPConfigErrorMessage = "ⓘ No target selected";
                            ErrorDisplayed?.Invoke(this, "IP Configuration");
                            return;
                        }

                        MessageBoxResult res = MessageBox.Show("This will overwrite the current values. Are you sure you want to retrieve the IP settings?", "Retrieve IP Settings", System.Windows.MessageBoxButton.YesNo);

                        if (res == MessageBoxResult.Yes)
                        {
                            IPAddressPart1 = "0";
                            IPAddressPart2 = "0";
                            IPAddressPart3 = "0";
                            IPAddressPart4 = "0";

                            SubnetPart1 = "0";
                            SubnetPart2 = "0";
                            SubnetPart3 = "0";
                            SubnetPart4 = "0";

                            PortNumber = "0";

                            _scriptHandler.StartScript(Messages.GET_IP_ADDRESS);    //Retrieve the IP address
                        }
                    });
                }

                return RetrieveIPSettings1;
            }
        }

        private void UpdateIPDetails(object sender, ResponseActions.IPRetrieveEventArgs args)
        {
            IPAddressPart1 = args.IP[0].ToString();
            IPAddressPart2 = args.IP[1].ToString();
            IPAddressPart3 = args.IP[2].ToString();
            IPAddressPart4 = args.IP[3].ToString();

            SubnetPart1 = args.Subnet[0].ToString();
            SubnetPart2 = args.Subnet[1].ToString();
            SubnetPart3 = args.Subnet[2].ToString();
            SubnetPart4 = args.Subnet[3].ToString();

            IsDHCP = args.DHCP;
            IsStatic = !args.DHCP;

            PortNumber = args.Port.ToString();
        }

        private void UpdateEnableState(bool state)
        {
            CanPerformActions = state;
        }
        #endregion

        #region MAC Address

        #endregion

        #region Additional IP Entry
        private RelayCommand AddListItem1;
        public ICommand AddListItem
        {
            get
            {
                if (AddListItem1 == null)
                {
                    AddListItem1 = new RelayCommand(o =>
                    {
                        //Add new list item
                        ChangeNumberOfIPEntries(IPList.Count + 1);
                    });
                }

                return AddListItem1;
            }
        }

        private RelayCommand RemoveListItem1;
        public ICommand RemoveListItem
        {
            get
            {
                if (RemoveListItem1 == null)
                {
                    RemoveListItem1 = new RelayCommand(o =>
                    {
                        //Remove selected list item or last list item if none selected
                        ChangeNumberOfIPEntries(IPList.Count - 1);
                    });
                }

                return RemoveListItem1;
            }
        }

        public void ChangeNumberOfIPEntries(int number)
        {
            while (IPList.Count != number)
            {
                if (IPList.Count > number)
                {
                    IPList.RemoveAt(IPList.Count - 1);
                }
                else
                {
                    IPList.Add(new IPListEntry());
                }
            }

            UpdateIPList();
        }

        void UpdateIPList()
        {
            IPList = new ObservableCollection<IPListEntry>(IPList);
        }

        private RelayCommand ChangeSelectedIPListEntry1;
        public ICommand ChangeSelectedIPListEntry
        {
            get
            {
                if (ChangeSelectedIPListEntry1 == null)
                {
                    ChangeSelectedIPListEntry1 = new RelayCommand(o =>
                    {
                        string[] ip = IPList[SelectedEntry].IP_Data.Split('.');
                        string[] subnet = IPList[SelectedEntry].Subnet_Data.Split('.');
                        string port = IPList[SelectedEntry].Port_Data;

                        IPListEntry_IP1 = ip[0];
                        IPListEntry_IP2 = ip[1];
                        IPListEntry_IP3 = ip[2];
                        IPListEntry_IP4 = ip[3];

                        IPListEntry_Subnet1 = subnet[0];
                        IPListEntry_Subnet2 = subnet[1];
                        IPListEntry_Subnet3 = subnet[2];
                        IPListEntry_Subnet4 = subnet[3];

                        IPListEntry_Port = port;
                    });
                }

                return ChangeSelectedIPListEntry1;
            }
        }

        private RelayCommand AddNewIPToList1;
        public ICommand AddNewIPToList
        {
            get
            {
                if (AddNewIPToList1 == null)
                {
                    AddNewIPToList1 = new RelayCommand(o =>
                    {
                        IPConfigErrorMessage = "";

                        if (long.TryParse(IPAddress, out long ip) == false)
                        {
                            IPConfigErrorMessage = "ⓘ Invalid IP address";
                            ErrorDisplayed?.Invoke(this, "IP List");
                            return;
                        }

                        if (long.TryParse(Subnet, out long subnet) == false)
                        {
                            IPConfigErrorMessage = "ⓘ Invalid subnet mask";
                            ErrorDisplayed?.Invoke(this, "IP List");
                            return;
                        }

                        if (int.TryParse(PortNumber, out int port) == false)
                        {
                            IPConfigErrorMessage = "ⓘ Invalid port number";
                            ErrorDisplayed?.Invoke(this, "IP List");
                        }

                        IPList[SelectedEntry].IP_Data = IPListEntry_FullIP;
                        IPList[SelectedEntry].Subnet_Data = IPListEntry_FullSubnet;
                        IPList[SelectedEntry].Port_Data = IPListEntry_Port;
                        UpdateIPList();
                    });
                }

                return AddNewIPToList1;
            }
        }

        private RelayCommand HideExtraIPSettings1;
        public ICommand HideExtraIPSettings
        {
            get
            {
                if (HideExtraIPSettings1 == null)
                {
                    HideExtraIPSettings1 = new RelayCommand(o =>
                    {
                        MoreIPSettingsVisibility = Visibility.Collapsed;
                        LessIPSettingsVisibility = Visibility.Visible;
                    });
                }

                return HideExtraIPSettings1;
            }
        }

        private RelayCommand ShowExtraIPSettings1;
        public ICommand ShowExtraIPSettings
        {
            get
            {
                if (ShowExtraIPSettings1 == null)
                {
                    ShowExtraIPSettings1 = new RelayCommand(o =>
                    {
                        MoreIPSettingsVisibility = Visibility.Visible;
                        LessIPSettingsVisibility = Visibility.Collapsed;
                    });
                }

                return ShowExtraIPSettings1;
            }
        }

        private RelayCommand SendListToDevice1;
        public ICommand SendListToDevice
        {
            get
            {
                if (SendListToDevice1 == null)
                {
                    SendListToDevice1 = new RelayCommand(o =>
                    {
                        if (_devices == null || _devices.Count <= 0 || _ipDeviceIndex < 0)
                        {
                            IPListConfigErrorMessage = "ⓘ No target selected";
                            ErrorDisplayed?.Invoke(this, "IP List");
                            return;
                        }

                        _scriptHandler.StartScript(Messages.SET_SERVER_IPS);
                    });
                }

                return SendListToDevice1;
            }
        }

        void RetrieveServerIPDetails(object sender, PreprocessActions.ServerIPRequestArgs e)
        {
            string[] ip = IPList[_scriptHandler.TimesRepeated].IP_Data.Split('.');
            string[] subnet = IPList[_scriptHandler.TimesRepeated].Subnet_Data.Split('.');
            string port = IPList[_scriptHandler.TimesRepeated].Port_Data;

            e.IP = new int[4] { int.Parse(ip[0]), int.Parse(ip[1]), int.Parse(ip[2]), int.Parse(ip[3]) };
            e.Subnet = new int[4] { int.Parse(subnet[0]), int.Parse(subnet[1]), int.Parse(subnet[2]), int.Parse(subnet[3]) };
            e.Port = int.Parse(port);
        }

        void RetrieveIPListCount(object sender, int e)
        {
            e = IPList.Count;
        }
        #endregion

        private RelayCommand SetCommType_Serial1;
        public ICommand SetCommType_Serial
        {
            get
            {
                if (SetCommType_Serial1 == null)
                {
                    SetCommType_Serial1 = new RelayCommand(o =>
                    {
                        CommType_Serial_Visibility = Visibility.Visible;
                        CommType_Ethernet_Visibility = Visibility.Collapsed;
                        _appData.Connection_Serial = true;

                        if (_connector.IsConnected)
                        {
                            _connector.Disconnect();
                        }
                    });
                }

                return SetCommType_Serial1;
            }
        }

        private RelayCommand SetCommType_Ethernet1;
        public ICommand SetCommType_Ethernet
        {
            get
            {
                if (SetCommType_Ethernet1 == null)
                {
                    SetCommType_Ethernet1 = new RelayCommand(o =>
                    {
                        CommType_Ethernet_Visibility = Visibility.Visible;
                        CommType_Serial_Visibility = Visibility.Collapsed;
                        _appData.Connection_Serial = false;

                        if (_connector.IsConnected)
                        {
                            _connector.Disconnect();
                        }
                    });
                }

                return SetCommType_Ethernet1;
            }
        }

        private RelayCommand SetTCPTargetType_Manual1;
        public ICommand SetTCPTargetType_Manual
        {
            get
            {
                if (SetTCPTargetType_Manual1 == null)
                {
                    SetTCPTargetType_Manual1 = new RelayCommand(o =>
                    {
                        TCPTargetType_Manual_Visibility = Visibility.Visible;
                        TCPTargetType_FromList_Visibility = Visibility.Collapsed;
                    });
                }

                return SetTCPTargetType_Manual1;
            }
        }

        private RelayCommand SetTCPTargetType_FromList1;
        public ICommand SetTCPTargetType_FromList
        {
            get
            {
                if (SetTCPTargetType_FromList1 == null)
                {
                    SetTCPTargetType_FromList1 = new RelayCommand(o =>
                    {
                        TCPTargetType_FromList_Visibility = Visibility.Visible;
                        TCPTargetType_Manual_Visibility = Visibility.Collapsed;
                    });
                }

                return SetTCPTargetType_FromList1;
            }
        }

        private void AddEthernetDeviceToList(EthernetDevice device)
        {
            foreach(EthernetDevice existingDevice in _ethernetDevices)
            {
                if (existingDevice.IP == device.IP)
                {
                    return;
                }
            }

            Application.Current.Dispatcher.Invoke(() =>
            {
                _ethernetDevices.Add(device);
            });
        }

        #region Accessors
        public string SelectedCOMPort { get { return _selectedCOMPort; } set { _selectedCOMPort = value; OnPropertyChanged(); } }
        public string BaudRate { get { return _baudRate; } set { _baudRate = value; OnPropertyChanged(); } }
        public ParityType ParityType { get { return _parityType; } set { _parityType = value; OnPropertyChanged(); } }
        public StopBitsType StopBits { get { return _stopBits; } set { _stopBits = value; OnPropertyChanged(); } }
        public string ConToggleButtonName { get { return _conToggleButtonName; } set { _conToggleButtonName = value; OnPropertyChanged(); } }
        public string SerialConnectErrorMessage { get { return _serialConnectErrorMessage; } set { _serialConnectErrorMessage = value; OnPropertyChanged(); } }
        public string EthernetConnectErrorMessage { get { return _ethernetConnectErrorMessage; } set { _ethernetConnectErrorMessage = value; OnPropertyChanged(); } }
        public bool CanPerformActions { get { return _canPerformActions; } set { _canPerformActions = value; OnPropertyChanged(); } }
        public ObservableCollection<DeviceData> DeviceList { get { return _devices; } set { _devices = value; OnPropertyChanged(); } }
        public int SelectedDevice { get { return _ipDeviceIndex; } set { _ipDeviceIndex = value; _appData.DeviceIndex = value; OnPropertyChanged(); } }
        public string IPConfigErrorMessage { get { return _ipConfigErrorMessage; } set { _ipConfigErrorMessage = value; OnPropertyChanged(); } }
        public string IPAddressPart1 { get { return _ipPart1; } set { _ipPart1 = value; OnPropertyChanged(); } }
        public string IPAddressPart2 { get { return _ipPart2; } set { _ipPart2 = value; OnPropertyChanged(); } }
        public string IPAddressPart3 { get { return _ipPart3; } set { _ipPart3 = value; OnPropertyChanged(); } }
        public string IPAddressPart4 { get { return _ipPart4; } set { _ipPart4 = value; OnPropertyChanged(); } }
        public string IPAddress
        {
            get { return $"{_ipPart1}{_ipPart2}{_ipPart3}{_ipPart4}"; }
            set
            {
                _ipPart1 = value.Substring(0, 2);
                _ipPart2 = value.Substring(2, 2);
                _ipPart3 = value.Substring(4, 2);
                _ipPart4 = value.Substring(6, 2);
                OnPropertyChanged();
            }
        }
        public string SubnetPart1 { get { return _subnetPart1; } set { _subnetPart1 = value; OnPropertyChanged(); } }
        public string SubnetPart2 { get { return _subnetPart2; } set { _subnetPart2 = value; OnPropertyChanged(); } }
        public string SubnetPart3 { get { return _subnetPart3; } set { _subnetPart3 = value; OnPropertyChanged(); } }
        public string SubnetPart4 { get { return _subnetPart4; } set { _subnetPart4 = value; OnPropertyChanged(); } }
        public string Subnet
        {
            get { return $"{_subnetPart1}{_subnetPart2}{_subnetPart3}{_subnetPart4}"; }
            set
            {
                _subnetPart1 = value.Substring(0, 2);
                _subnetPart2 = value.Substring(2, 2);
                _subnetPart3 = value.Substring(4, 2);
                _subnetPart4 = value.Substring(6, 2);
                OnPropertyChanged();
            }
        }
        public string PortNumber { get { return _portNumber; } set { _portNumber = value; OnPropertyChanged(); } }
        public bool IsDHCP { get { return _isDHCP; } set { _isDHCP = value; OnPropertyChanged(); } }
        public bool IsStatic { get { return _isStatic; } set { _isStatic = value; OnPropertyChanged(); } }

        public string IPListEntry_IP1 { get { return _addIPEntry_IP_part1; } set { _addIPEntry_IP_part1 = value; OnPropertyChanged(); } }
        public string IPListEntry_IP2 { get { return _addIPEntry_IP_part2; } set { _addIPEntry_IP_part2 = value; OnPropertyChanged(); } }
        public string IPListEntry_IP3 { get { return _addIPEntry_IP_part3; } set { _addIPEntry_IP_part3 = value; OnPropertyChanged(); } }
        public string IPListEntry_IP4 { get { return _addIPEntry_IP_part4; } set { _addIPEntry_IP_part4 = value; OnPropertyChanged(); } }
        public string IPListEntry_FullIP
        {
            get { return $"{_addIPEntry_IP_part1}.{_addIPEntry_IP_part2}.{_addIPEntry_IP_part3}.{_addIPEntry_IP_part4}"; }
            set
            {
                _addIPEntry_IP_part1 = value.Substring(0, 2);
                _addIPEntry_IP_part2 = value.Substring(2, 2);
                _addIPEntry_IP_part3 = value.Substring(4, 2);
                _addIPEntry_IP_part4 = value.Substring(6, 2);
                OnPropertyChanged();
            }
        }

        public string IPListEntry_Subnet1 { get { return _addIPEntry_Subnet_part1; } set { _addIPEntry_Subnet_part1 = value; OnPropertyChanged(); } }
        public string IPListEntry_Subnet2 { get { return _addIPEntry_Subnet_part2; } set { _addIPEntry_Subnet_part2 = value; OnPropertyChanged(); } }
        public string IPListEntry_Subnet3 { get { return _addIPEntry_Subnet_part3; } set { _addIPEntry_Subnet_part3 = value; OnPropertyChanged(); } }
        public string IPListEntry_Subnet4 { get { return _addIPEntry_Subnet_part4; } set { _addIPEntry_Subnet_part4 = value; OnPropertyChanged(); } }
        public string IPListEntry_FullSubnet
        {
            get { return $"{_addIPEntry_Subnet_part1}.{_addIPEntry_Subnet_part2}.{_addIPEntry_Subnet_part3}.{_addIPEntry_Subnet_part4}"; }
            set
            {
                _addIPEntry_Subnet_part1 = value.Substring(0, 2);
                _addIPEntry_Subnet_part2 = value.Substring(2, 2);
                _addIPEntry_Subnet_part3 = value.Substring(4, 2);
                _addIPEntry_Subnet_part4 = value.Substring(6, 2);
                OnPropertyChanged();
            }
        }

        public string IPListEntry_Port { get { return _addIPEntry_Port; } set { _addIPEntry_Port = value; OnPropertyChanged(); } }
        public int SelectedEntry { get { return _selectedEntry; } set { _selectedEntry = value; OnPropertyChanged(); } }
        public string IPListConfigErrorMessage { get { return _ipListConfigErrorMessage; } set { _ipListConfigErrorMessage = value; OnPropertyChanged(); } }
        public ObservableCollection<IPListEntry> IPList { get { return _ipList; } set { _ipList = value; OnPropertyChanged(); } }
        public Visibility MoreIPSettingsVisibility { get { return _moreIPSettingsButtonVisibility; } set { _moreIPSettingsButtonVisibility = value; OnPropertyChanged(); } }
        public Visibility LessIPSettingsVisibility { get { return _lessIPSettingsButtonVisibility; } set { _lessIPSettingsButtonVisibility = value; OnPropertyChanged(); } }

        public bool CommType_Serial { get { return _commType_Serial; } set { _commType_Serial = value; OnPropertyChanged(); } }
        public Visibility CommType_Serial_Visibility { get { return _commType_Serial_Visibility; } set { _commType_Serial_Visibility = value; OnPropertyChanged(); } }
        public Visibility CommType_Ethernet_Visibility { get { return _commType_Ethernet_Visibility; } set { _commType_Ethernet_Visibility = value; OnPropertyChanged(); } }

        public bool CommType_Ethernet { get { return _commType_Ethernet; } set { _commType_Ethernet = value; OnPropertyChanged(); } }

        public bool TCPTargetType_Manual { get { return _tcpTargetType_Manual; } set { _tcpTargetType_Manual = value; OnPropertyChanged(); } }
        public Visibility TCPTargetType_Manual_Visibility { get { return _tcpTargetType_Manual_Visibility; } set { _tcpTargetType_Manual_Visibility = value; OnPropertyChanged(); } }
        public Visibility TCPTargetType_FromList_Visibility { get { return _tcpTargetType_FromList_Visibility; } set { _tcpTargetType_FromList_Visibility = value; OnPropertyChanged(); } }

        public bool TCPTargetType_FromList { get { return _tcpTargetType_FromList; } set { _tcpTargetType_FromList = value; OnPropertyChanged(); } }

        public string EthernetConnection_IP_1 { get { return _ethernetConnection_IP_1; } set { _ethernetConnection_IP_1 = value; OnPropertyChanged(); } }
        public string EthernetConnection_IP_2 { get { return _ethernetConnection_IP_2; } set { _ethernetConnection_IP_2 = value; OnPropertyChanged(); } }
        public string EthernetConnection_IP_3 { get { return _ethernetConnection_IP_3; } set { _ethernetConnection_IP_3 = value; OnPropertyChanged(); } }
        public string EthernetConnection_IP_4 { get { return _ethernetConnection_IP_4; } set { _ethernetConnection_IP_4 = value; OnPropertyChanged(); } }
        public string EthernetConnection_Port { get { return _ethernetConnection_Port; } set { _ethernetConnection_Port = value; OnPropertyChanged(); } }

        public ObservableCollection<EthernetDevice> EthernetDevices { get { return _ethernetDevices; } set { _ethernetDevices= value; OnPropertyChanged(); } }
        public int EthernetDevices_SelectedIndex { get { return _ethernetDevices_SelectedIndex; } set { _ethernetDevices_SelectedIndex = value; OnPropertyChanged(); } }
        #endregion
    }
}
