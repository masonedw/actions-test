using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections.ObjectModel;
using System.Threading.Tasks;
using Blind_Config_Tool.Core;
using System.Windows.Input;
using System.Globalization;

namespace Blind_Config_Tool.RedesignFiles.MVVM.ViewModel
{
    internal class VM_MotPage2 : ObservableObject
    {
        public static event EventHandler<string> ErrorDisplayed;
        public static event Action OnMotorPageChange;
        ScriptHandler _scriptHandler;
        AppData _appData;

        string _motorLabel = "No Motor Selected";
        string _nodeType = "No Type";
        string _firmware = "Firmware: 0.0";
        string[] _groupIDs;
        string _setGroupIDError;
        string[] _intermediatePositions;
        string _ipForSetCurrentPos = "1";
        string _setIPError;

        public VM_MotPage2()
        {
            _scriptHandler = ScriptHandler.Instance;
            _appData = AppData.Instance;

            _groupIDs = new string[16];
            _intermediatePositions = new string[16];

            for(int i = 0; i < 16; i++)
            {
                _groupIDs[i] = "000000";
                _intermediatePositions[i] = "0";
            }

            _appData.OnRefreshGroupsAndIPs += UpdateDetails;
            PreprocessActions.OnGroupIDRequest += RetrieveGroupID;
            PreprocessActions.OnIPRequest += RetrieveIP;
            PreprocessActions.OnCurrentIPRequest += RetrieveCurrentSetIP;
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

        private RelayCommand SetGroupIDs1;
        public ICommand SetGroupIDs
        {
            get
            {
                if (SetGroupIDs1 == null)
                {
                    SetGroupIDs1 = new RelayCommand(o =>
                    {
                        if(_appData.SelectedMotor == null)
                        {
                            SetGroupIDError = "ⓘ No motor selected";
                            ErrorDisplayed?.Invoke(this, "Group IDs");
                            return;
                        }

                        for(int i = 0; i < 16; i++)
                        {
                            if (!IsValidHex(GroupIDs[i]))
                            {
                                SetGroupIDError = $"ⓘ ID {i+1} is invalid";
                                ErrorDisplayed?.Invoke(this, "Group IDs");
                                return;
                            }
                        }

                        SetGroupIDError = "";
                        _scriptHandler.StartScript(Messages.SAVE_GROUPS_AND_IPS);
                    });
                }

                return SetGroupIDs1;
            }
        }

        private RelayCommand SetIPs1;
        public ICommand SetIPs
        {
            get
            {
                if (SetIPs1 == null)
                {
                    SetIPs1 = new RelayCommand(o =>
                    {
                        if (_appData.SelectedMotor == null)
                        {
                            SetIPError = "ⓘ No motor selected";
                            ErrorDisplayed?.Invoke(this, "IPs");
                            return;
                        }

                        for (int i = 0; i < 16; i++)
                        {
                            if (!int.TryParse(IntermediatePositions[i], out int val))
                            {
                                SetIPError = $"ⓘ IP {i + 1} is invalid";
                                ErrorDisplayed?.Invoke(this, "IPs");
                                return;
                            }

                            if(val < 2 || val > 98)
                            {
                                SetIPError = $"ⓘ IP {i + 1} is out of range (2 - 98)";
                                ErrorDisplayed?.Invoke(this, "IPs");
                                return;
                            }
                        }

                        SetIPError = "";
                        _scriptHandler.StartScript(Messages.SAVE_GROUPS_AND_IPS);
                    });
                }

                return SetIPs1;
            }
        }

        private RelayCommand SetIPToCurrentPosition1;
        public ICommand SetIPToCurrentPosition
        {
            get
            {
                if (SetIPToCurrentPosition1 == null)
                {
                    SetIPToCurrentPosition1 = new RelayCommand(o =>
                    {
                        if (int.TryParse(_ipForSetCurrentPos, out int res))
                        {
                            _scriptHandler.StartScript(Messages.SET_IP_TO_CURRENT_POSITION);
                        }
                        else
                        {
                            _appData.DisplayError("Please enter a valid IP to change, then press the button again.", "Invalid IP Selected");
                        }
                    });
                }

                return SetIPToCurrentPosition1;
            }
        }

        bool IsValidHex(string hex)
        {
            if (hex.Length != 6)
            {
                return false;
            }

                // Try parsing the string as a hex number
                return int.TryParse(hex, NumberStyles.HexNumber, CultureInfo.InvariantCulture, out _);
        }

        void RetrieveGroupID(object sender, PreprocessActions.GroupIDRequestArgs e)
        {
            e.address = GroupIDs[e.ID];
        }

        void RetrieveIP(object sender, PreprocessActions.IPRequestArgs e)
        {
            e.IP = IntermediatePositions[e.ID];
        }

        void RetrieveCurrentSetIP(object sender, PreprocessActions.CurrentIPRequestArgs e)
        {
            e.IP = int.Parse(IPForSetCurrentPos);
        }

        void UpdateDetails()
        {
            //Get the limits based on the type of motor it is
            MotorTypeParams parameters = MotorConfigs.GetParams(_appData.SelectedMotor.Type[1]);
            NodeType = $"Node: {parameters.NodeName}";

            if (_appData.SelectedMotor.NameAsString == "")
            {
                MotorLabel = $"Unnamed Motor ({_appData.SelectedMotor.AddressAsString})";   //Display the heading of the page as the motor name + address
            }
            else
            {
                MotorLabel = $"{_appData.SelectedMotor.NameAsString} ({_appData.SelectedMotor.AddressAsString})";   //Display the heading of the page as the motor name + address
            }

            for (int i = 0; i < 16; i++)
            {
                GroupIDs[i] = _appData.SelectedMotor.Groups[i];
                IntermediatePositions[i] = _appData.SelectedMotor.InternalPositions[i];
                OnPropertyChanged(nameof(GroupIDs));
                OnPropertyChanged(nameof(IntermediatePositions));
            }
        }

        public string MotorLabel { get { return _motorLabel; } set { _motorLabel = value; OnPropertyChanged(); } }
        public string NodeType { get { return _nodeType; } set { _nodeType = value; OnPropertyChanged(); } }
        public string Firmware { get { return _firmware; } set { } }
        public string[] GroupIDs { get { return _groupIDs; } set { _groupIDs = value; OnPropertyChanged(); } }
        public string SetGroupIDError { get { return _setGroupIDError; } set { _setGroupIDError = value; OnPropertyChanged(); } }
        public string[] IntermediatePositions { get { return _intermediatePositions; } set { _intermediatePositions = value; OnPropertyChanged(); } }
        public string IPForSetCurrentPos { get { return _ipForSetCurrentPos; } set { _ipForSetCurrentPos = value; OnPropertyChanged(); } }
        public string SetIPError { get { return _setIPError; } set { _setIPError = value; OnPropertyChanged(); } }
    }
}
