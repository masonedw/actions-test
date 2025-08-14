using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Blind_Config_Tool.Core
{
    public enum MotorMoveCommand
    {
        NO_ACTION = 0x0,
        STOP = 0x90,
        MOVE_UP_DOWN = 0x91,
        GO_TO_IP = 0x92,
        NEXT_IP_UP_DOWN = 0x93,
        JOG_UP_MM = 0x94,
        JOG_DOWN_MM = 0x95,
        JOG_UP_PULSE = 0x96,
        JOG_DOWN_PULSE = 0x97,
        GO_TO_PERCENT = 0x98,
        VISUAL_FEEDBACK = 0x99,
        MOVE_TOGGLE = 0x9A
    }
}
