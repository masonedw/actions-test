using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;
using Blind_Config_Tool.Core;

namespace Blind_Config_Tool.Core.Converters
{
    static class MotorMoveCommandConverter {
        public static MotorMoveCommand ConvertTo(KeypadButtonAction keypadButtonAction)
        {
            MotorMoveCommand retVal = MotorMoveCommand.NO_ACTION;

            switch(keypadButtonAction) 
            {
                case KeypadButtonAction.NO_ACTION:
                    retVal = MotorMoveCommand.NO_ACTION;
                    break;
                case KeypadButtonAction.MOVE_UP:
                case KeypadButtonAction.MOVE_DOWN:
                    retVal = MotorMoveCommand.MOVE_UP_DOWN;
                    break;
                case KeypadButtonAction.STOP:
                    retVal = MotorMoveCommand.STOP;
                    break;
                case KeypadButtonAction.GO_TO_IP:
                    retVal = MotorMoveCommand.GO_TO_IP;
                    break;
                case KeypadButtonAction.NEXT_IP_UP:
                case KeypadButtonAction.NEXT_IP_DOWN:
                    retVal = MotorMoveCommand.NEXT_IP_UP_DOWN;
                    break;
                case KeypadButtonAction.JOG_UP_MM:
                    retVal = MotorMoveCommand.JOG_UP_MM;
                    break;
                case KeypadButtonAction.JOG_DOWN_MM:
                    retVal = MotorMoveCommand.JOG_DOWN_MM;
                    break;
                case KeypadButtonAction.JOG_UP_PULSE:
                    retVal = MotorMoveCommand.JOG_UP_PULSE;
                    break;
                case KeypadButtonAction.JOG_DOWN_PULSE:
                    retVal = MotorMoveCommand.JOG_DOWN_PULSE;
                    break;
                case KeypadButtonAction.GO_TO_PERCENT:
                    retVal = MotorMoveCommand.GO_TO_PERCENT;
                    break;
                case KeypadButtonAction.VISUAL_FEEDBACK:
                    retVal = MotorMoveCommand.VISUAL_FEEDBACK;
                    break;
                case KeypadButtonAction.MOVE_TOGGLE:
                    retVal = MotorMoveCommand.MOVE_TOGGLE;
                    break;
            }

            return retVal;
        }

        public static KeypadButtonAction ConvertBack(MotorMoveCommand motorMoveCommand, string Target)
        {
            if (motorMoveCommand == MotorMoveCommand.MOVE_UP_DOWN || motorMoveCommand == MotorMoveCommand.NEXT_IP_UP_DOWN)
            {
                if (Target == "1")
                {
                    return motorMoveCommand == MotorMoveCommand.MOVE_UP_DOWN ? KeypadButtonAction.MOVE_UP : KeypadButtonAction.NEXT_IP_UP;
                }
                else if (Target == "0")
                {
                    return motorMoveCommand == MotorMoveCommand.MOVE_UP_DOWN ? KeypadButtonAction.MOVE_DOWN : KeypadButtonAction.NEXT_IP_DOWN;
                }
            }

            var commandMapping = new Dictionary<MotorMoveCommand, KeypadButtonAction>
                {
                    { MotorMoveCommand.STOP, KeypadButtonAction.STOP },
                    { MotorMoveCommand.GO_TO_IP, KeypadButtonAction.GO_TO_IP },
                    { MotorMoveCommand.JOG_UP_MM, KeypadButtonAction.JOG_UP_MM },
                    { MotorMoveCommand.JOG_DOWN_MM, KeypadButtonAction.JOG_DOWN_MM },
                    { MotorMoveCommand.JOG_UP_PULSE, KeypadButtonAction.JOG_UP_PULSE },
                    { MotorMoveCommand.JOG_DOWN_PULSE, KeypadButtonAction.JOG_DOWN_PULSE },
                    { MotorMoveCommand.GO_TO_PERCENT, KeypadButtonAction.GO_TO_PERCENT },
                    { MotorMoveCommand.VISUAL_FEEDBACK, KeypadButtonAction.VISUAL_FEEDBACK },
                    { MotorMoveCommand.MOVE_TOGGLE, KeypadButtonAction.MOVE_TOGGLE }
                };

            return commandMapping.TryGetValue(motorMoveCommand, out var action) ? action : KeypadButtonAction.NO_ACTION;
        }
    }
}
