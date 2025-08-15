using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Blind_Config_Tool.Core
{
    public enum KeypadButtonAction
    {
        NO_ACTION = 0x00,
        MOVE_UP = 0x01,
        MOVE_DOWN = 0x02,
        STOP = 0x03,
        GO_TO_IP = 0x04,
        NEXT_IP_UP = 0x05,
        NEXT_IP_DOWN = 0x06,
        GO_TO_PULSE = 0x08,
        JOG_UP_MS = 0x0A,
        JOG_DOWN_MS = 0x0B,
        JOG_UP_PULSE = 0x0C,
        JOG_DOWN_PULSE = 0x0D,
        GO_TO_PERCENT = 0x10,
        TOGGLE = 0x24,
        SET_IP = 0x25
    }
}
