using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Blind_Config_Tool.Core
{
    public class KeypadCommand : INotifyPropertyChanged
    {
        private int _id;
        private bool _changed;

        private KeypadButtonAction _onPressAction;
        private string _onPressTarget;
        private byte[] _onPressTargetBytes;

        private KeypadButtonAction _onHoldAction;
        private string _onHoldTarget;
        private byte[] _onHoldTargetBytes;

        private KeypadButtonAction _onReleaseAction;
        private string _onReleaseTarget;
        private byte[] _onReleaseTargetBytes;

        private TargetMotor _targetMotor;
        private string _targetAddress;

        public KeypadCommand(int  _index) 
        { 
            _id = _index+1;
            _onPressTarget = "0";
            _onPressTargetBytes = new byte[2];
            _onHoldTarget = "0";
            _onHoldTargetBytes = new byte[2];
            _onReleaseTarget = "0";
            _onReleaseTargetBytes = new byte[2];
            _targetAddress = "FFFFFF";
            _targetMotor = TargetMotor.ALL_MOTORS;
        }

        public event PropertyChangedEventHandler PropertyChanged;
        protected void OnPropertyChanged(string propertyName)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        public bool CompareCommands(KeypadCommand cmd)
        {
            return _id == cmd.ID &&
                    _onPressAction == cmd.OnPressAction &&
                    _onPressTarget == cmd.OnPressTarget &&
                    _onHoldAction == cmd.OnHoldAction &&
                    _onHoldTarget == cmd.OnHoldTarget &&
                    _onReleaseAction == cmd.OnReleaseAction &&
                    _onReleaseTarget == cmd.OnReleaseTarget &&
                    _targetMotor == cmd._targetMotor;
        }

        public int ID { get { return _id; } set { _id = value; OnPropertyChanged("ID"); } }
        public bool Changed { get { return _changed;} set { _changed = value; OnPropertyChanged("Changed"); } }
        public KeypadButtonAction OnPressAction { get { return _onPressAction; } set { _onPressAction = value; OnPropertyChanged("OnPressAction"); } }
        public string OnPressTarget { get { return _onPressTarget; } set { _onPressTarget = value; OnPropertyChanged("OnPressTarget"); _onPressTargetBytes = StringIntToBytes(value); } }
        public byte[] OnPressTargetAsBytes { get { return _onPressTargetBytes; } }
        public KeypadButtonAction OnHoldAction { get { return _onHoldAction; } set { _onHoldAction = value; OnPropertyChanged("OnHoldAction"); } }
        public string OnHoldTarget { get { return _onHoldTarget; } set { _onHoldTarget = value; OnPropertyChanged("OnHoldTarget"); _onHoldTargetBytes = StringIntToBytes(value); } }
        public byte[] OnHoldTargetAsBytes { get { return _onHoldTargetBytes; } }
        public KeypadButtonAction OnReleaseAction { get { return _onReleaseAction; } set { _onReleaseAction = value; OnPropertyChanged("OnReleaseAction"); } }
        public string OnReleaseTarget { get { return _onReleaseTarget; } set { _onReleaseTarget = value; OnPropertyChanged("OnReleaseTarget"); _onReleaseTargetBytes = StringIntToBytes(value); } }
        public byte[] OnReleaseTargetAsBytes { get { return _onReleaseTargetBytes; } }
        public TargetMotor TargetMotor { get { return _targetMotor; } set { _targetMotor = value; OnPropertyChanged("TargetMotor"); } }
        public string TargetAddress { get { return _targetAddress;} set { _targetAddress = value; OnPropertyChanged("TargetAddress"); } }

        private byte[] StringIntToBytes(string value)
        {
            if(!int.TryParse(value, out int val))
            {
                val = 0;
            }

            byte[] retVal = BitConverter.GetBytes(val);

            // Take the first two bytes
            byte[] result = new byte[2];
            result[0] = retVal[0];
            result[1] = retVal[1];

            return result;
        }
    }
}
