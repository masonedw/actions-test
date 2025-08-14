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
    class KeypadButtonActionConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture) //Converts a KeypadButtonAction enum into a text string
        {
            KeypadButtonAction val = (KeypadButtonAction)value;

            string retVal = "N/A";

            switch(val) 
            {
                case KeypadButtonAction.JOG_DOWN_MM:
                    retVal = "Jog Up (MM)";
                    break;
                case KeypadButtonAction.JOG_UP_MM:
                    retVal = "Jog Up (UP)";
                    break;
                case KeypadButtonAction.VISUAL_FEEDBACK:
                    retVal = "Visual Feedback";
                    break;
                case KeypadButtonAction.MOVE_TOGGLE:
                    retVal = "Move Toggle";
                    break;
                case KeypadButtonAction.GO_TO_IP:
                    retVal = "To IP";
                    break;
                case KeypadButtonAction.GO_TO_PERCENT:
                    retVal = "To Percent #";
                    break;
                case KeypadButtonAction.JOG_UP_PULSE:
                    retVal = "Jog Up (pulse)";
                    break;
                case KeypadButtonAction.JOG_DOWN_PULSE:
                    retVal = "Jog Down (pulse)";
                    break;
                case KeypadButtonAction.STOP:
                    retVal = "Stop";
                    break;
                case KeypadButtonAction.MOVE_UP:
                    retVal = "Up";
                    break;
                case KeypadButtonAction.MOVE_DOWN:
                    retVal = "Down";
                    break;
                case KeypadButtonAction.NEXT_IP_DOWN:
                    retVal = "Next IP Down";
                    break;
                case KeypadButtonAction.NEXT_IP_UP:
                    retVal = "Next IP Up";
                    break;
                case KeypadButtonAction.NO_ACTION:
                    retVal = "N/A";
                    break;
            }

            return retVal;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture) //Converts a string into a KeypadButtonAction
        {
            string strValue = value as string;
            KeypadButtonAction retVal = KeypadButtonAction.NO_ACTION;

            switch(strValue)
            {
                case "To IP":
                    retVal = KeypadButtonAction.GO_TO_IP;
                    break;
                case "To Percent #":
                    retVal = KeypadButtonAction.GO_TO_PERCENT;
                    break;
                case "Jog Up (pulse)":
                    retVal = KeypadButtonAction.JOG_UP_PULSE;
                    break;
                case "Jog Down (pulse)":
                    retVal = KeypadButtonAction.JOG_DOWN_PULSE;
                    break;
                case "Jog Up (MM)":
                    retVal = KeypadButtonAction.JOG_UP_MM;
                    break;
                case "Jog Down (MM)":
                    retVal = KeypadButtonAction.JOG_DOWN_MM;
                    break;
                case "Stop":
                    retVal = KeypadButtonAction.STOP;
                    break;
                case "Up":
                    retVal = KeypadButtonAction.MOVE_UP;
                    break;
                case "Down":
                    retVal = KeypadButtonAction.MOVE_DOWN;
                    break;
                case "Next IP Down":
                    retVal = KeypadButtonAction.NEXT_IP_DOWN;
                    break;
                case "Next IP Up":
                    retVal = KeypadButtonAction.NEXT_IP_UP;
                    break;
                case "N/A":
                    retVal = KeypadButtonAction.NO_ACTION;
                    break;
                case "Visual Feedback":
                    retVal = KeypadButtonAction.VISUAL_FEEDBACK;
                    break;
                case "Move Toggle":
                    retVal = KeypadButtonAction.MOVE_TOGGLE;
                    break;
            }

            return retVal;
        }
    }
}
