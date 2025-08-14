using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;

namespace Blind_Config_Tool.Core.Converters
{
    class BooleanToYesOrNoConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture) //Converts boolean values into Yes or No
        {
            bool bValue = (bool)value;
            return bValue ? "Yes" : "No";   //If true, return "Yes", else, return "No
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture) //Converts Yes and No into boolean
        {
            string strValue = value as string;
            if (strValue == "Yes")
                return true;    //If Yes, return true
            else
                return false;
        }
    }
}
