using Blind_Config_Tool.Core.Converters;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;

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

        /// <summary>
        /// KeypadCommand Constructor that takes the values directly as parameters to construct the KeypadCommand
        /// </summary>
        /// <param name="CommandIndex">The index into the array of keypad commands. The value must without 1 minused from it.
        ///     from it.</param>
        /// <param name="OnPressActionValue">The on press action value in as a <see cref="MotorMoveCommand">MotorMoveCommand</see> enum.</param>
        /// <param name="OnPressTargetValue">The on press targeting value. Should be a 16-bit value.</param>
        /// <param name="OnHoldActionValue">The on hold action value in as a <see cref="MotorMoveCommand">MotorMoveCommand</see> enum.</param>
        /// <param name="OnHoldTargetValue">The on hold targeting value. Should be a 16-bit value.</param>
        /// <param name="OnReleaseActionValue">The on release action value in as a <see cref="MotorMoveCommand">MotorMoveCommand</see> enum.</param>
        /// <param name="OnReleaseTargetValue">The on hold targeting value. Should be a 16-bit value.</param>
        /// <param name="ChangedValue">A bool representing if the KeypadCommand should be listed as "Modified".</param>
        /// <param name="TargetAddressValue">The address of the target.</param>
        public KeypadCommand(int CommandIndex, MotorMoveCommand OnPressActionValue, string OnPressTargetValue,
                             MotorMoveCommand OnHoldActionValue, string OnHoldTargetValue,
                             MotorMoveCommand OnReleaseActionValue, string OnReleaseTargetValue,
                             bool ChangedValue, string TargetAddressValue)
        {
            ID = CommandIndex;

            OnPressTarget = OnPressTargetValue.Equals("65535") ? "0" : OnPressTargetValue;
            OnPressAction = MotorMoveCommandConverter.ConvertBack(OnPressActionValue, _onPressTarget);

            OnHoldTarget = OnHoldTargetValue.Equals("65535") ? "0" : OnHoldTargetValue;
            OnHoldAction = MotorMoveCommandConverter.ConvertBack(OnHoldActionValue, _onHoldTarget);

            OnReleaseTarget = OnHoldTargetValue.Equals("65535") ? "0" : OnHoldTargetValue;
            OnReleaseAction = MotorMoveCommandConverter.ConvertBack(OnReleaseActionValue, _onReleaseTarget);

            Changed = ChangedValue;
            TargetAddress = TargetAddressValue;
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
