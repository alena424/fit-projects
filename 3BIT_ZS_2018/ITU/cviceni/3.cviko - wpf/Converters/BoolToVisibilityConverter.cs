using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Data;
using System.Windows;

namespace WPFWeather.Converters
{
    public class BoolToVisibilityConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            bool boolValue = (bool)value;
            bool boolParameter = parameter != null ? bool.Parse((string)parameter) : false;

            if (boolParameter) return boolValue ? Visibility.Hidden : Visibility.Visible;
            else return boolValue ? Visibility.Visible : Visibility.Hidden;
            
        }

        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            return null;
        }
    }
}
