using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;

namespace Blind_Config_Tool.Core.Converters
{
    class TargetMotorConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture) //Converts a TargetMotor enum into a string
        {
            TargetMotor val = (TargetMotor)value;
            string retVal = "All Motors";


            switch(val)
            {
                case TargetMotor.ALL_MOTORS:
                    retVal = "All Motors";
                    break;
                case TargetMotor.SPECIFIC_GROUP:
                    retVal = "Specific Group";
                    break;
                case TargetMotor.SPECIFIC_MOTOR:
                    retVal = "Specific Motor";
                    break;
                case TargetMotor.NEW_ADDRESS:
                    retVal = "New Address";
                    break;
            }

            return retVal;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture) //Converts a string into a TargetMotor enum
        {
            string strValue = value as string;
            TargetMotor retVal = TargetMotor.SPECIFIC_GROUP;

            switch(strValue)
            {
                case "All Motors":
                    retVal = TargetMotor.ALL_MOTORS;
                    break;
                case "Specific Group":
                    retVal = TargetMotor.SPECIFIC_GROUP;
                    break;
                case "Specific Motor":
                    retVal = TargetMotor.SPECIFIC_MOTOR;
                    break;
                case "New Address":
                    retVal = TargetMotor.NEW_ADDRESS;
                    break;
            }

            return retVal;
        }
    }
}
