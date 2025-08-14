using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Blind_Config_Tool.RedesignFiles
{
    public class Motor : DeviceData
    {
        #region Declarations
        private byte[] _motorType;                            //The type of motor
        private string[] _internalPositions = new string[16];   //The preset positions to move the motor to
        private string[] _groups = new string[16];          //The groups that this motor is a part of (the motor will move when a group movement command is used with one of these groups)
        private byte _movementDirection;           //The direction the motor moves
        private int _slowSpeed;                             //The slow movement speed
        private int _upSpeed;                               //The up movement speed
        private int _downSpeed;                             //The down movement speed
        private int _downLimit;                             //The max downward position the motor can move to
        private int _upLimit;                               //The max upward position the motor can move to
        #endregion

        public Motor(byte[] address, byte[] node)
        {
            _nameString = "";
            _addressBytes = address;
            _addressString = BitConverter.ToString(address);
            //_motorType = AssistantFunctions.ReverseHexCharacters((byte)~node);
            _motorType = node;

            for(int i = 0; i < 16; i++)
            {
                _internalPositions[i] = "2";
                _groups[i] = "000000";
            }
        }

        public void SetSpeeds(int upSpeed, int downSpeed, int slowSpeed)
        {
            _upSpeed = upSpeed;
            _downSpeed = downSpeed;
            _slowSpeed= slowSpeed;
        }

        #region Accessors
        public byte[] Type { get { return _motorType; } }
        public string[] InternalPositions { get { return _internalPositions; } }
        public string[] Groups { get { return _groups;} }
        public byte MovementDirection { get { return _movementDirection; } set { _movementDirection = value; } }
        public int SlowSpeed { get { return _slowSpeed; } set { _slowSpeed = value; } }
        public int UpSpeed { get { return _upSpeed;} set { _upSpeed = value; } }
        public int DownSpeed { get { return _downSpeed;} set { _downSpeed = value; } }
        public int DownLimit { get { return _downLimit; } set { _upLimit = value; } }
        public int UpLimit { get { return _upLimit; } set { _downLimit = value; } }
        #endregion
    }
}
