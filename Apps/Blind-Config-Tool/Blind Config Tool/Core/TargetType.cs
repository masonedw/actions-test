using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Blind_Config_Tool.Core
{
    public enum Target
    {
        BROADCAST_ALL,         //Target address will be set to FFFFFF
        ALL_MOTORS_IN_LIST,     //The target address will be set to one of the motors in the list of motors (which one is targeted will depend on the repeatCounter value in SerialControl)
        ALL_KEYPADS_IN_LIST,    //The target address will be set to one of the keypads in the list of keypads
        ALL_DEVICES_IN_LIST,    //The target will be set to one of the devices in the list of devices
        SPECIFIC_KEYPAD,    //The target address will be set to the currently selected keypad
        SPECIFIC_MOTOR,      //The target address will be set to the currently selected motor
        SPECIFIC_DEVICE     //The target address will be set to the currently selected device
    }
}
