using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Data;

namespace Blind_Config_Tool.Core.Converters
{
    public class SortTypeToStringConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            switch (value)
            {
                case SortType.NAME_ASCENDING:
                    return "Name (A - Z)";
                case SortType.NAME_DESCENDING:
                    return "Name (Z - A)";
                case SortType.ADDRESS_ASCENDING:
                    return "Address (A - Z)";
                case SortType.ADDRESS_DESCENDING:
                    return "Address (Z - A)";
                default:
                    return string.Empty;
            }
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value == null || string.IsNullOrEmpty(value.ToString()))
                return DependencyProperty.UnsetValue;

            string enumString = value.ToString();

            // Handle the conversion back from the human-readable string to the enum
            foreach (SortType enumValue in Enum.GetValues(typeof(SortType)))
            {
                string enumStringValue = enumValue.ToString();
                string friendlyString = string.Join(" ", enumStringValue.Split(new[] { '_' }, StringSplitOptions.RemoveEmptyEntries)
                                                                     .Select(word => char.ToUpper(word[0]) + word.Substring(1).ToLower()));
                if (friendlyString == enumString)
                    return enumValue;
            }

            // If no match is found, return a default or throw an exception (depending on your needs)
            return DependencyProperty.UnsetValue;
        }
    }
}
