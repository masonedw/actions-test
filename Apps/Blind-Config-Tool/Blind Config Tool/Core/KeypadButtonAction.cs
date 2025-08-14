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
        JOG_UP_MM = 0x07,
        JOG_DOWN_MM = 0x08,
        JOG_UP_PULSE = 0x09,
        JOG_DOWN_PULSE = 0x0A,
        GO_TO_PERCENT = 0x0B,
        VISUAL_FEEDBACK = 0xC,
        MOVE_TOGGLE = 0x0D
    }
}
