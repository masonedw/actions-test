using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace Blind_Config_Tool.RedesignFiles
{
    public class Keypad : DeviceData
    {
        #region Declarations
        byte[] _firmwareVersion;
        string _firmwareString;
        #endregion

        #region Constructor
        public Keypad(byte[] address)
        {
            _addressBytes = address;
            _addressString = BitConverter.ToString(address);
            _firmwareVersion = new byte[] { 0x00, 0x00 };
            _firmwareString = BitConverter.ToString(_firmwareVersion);
        }
        #endregion

        #region Accessors
        byte[] FirmwareVersion { get { return _firmwareVersion; } }
        string FirmwareAsString { get { return _firmwareString; } }
        #endregion
    }
}
