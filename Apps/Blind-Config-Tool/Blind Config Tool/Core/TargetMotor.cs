using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Blind_Config_Tool.Core
{
    public enum TargetMotor
    {
        ALL_MOTORS = 0x01,
        SPECIFIC_GROUP = 0x80,
        SPECIFIC_MOTOR = 0x02,
        NEW_ADDRESS = 0x03
    }
}
