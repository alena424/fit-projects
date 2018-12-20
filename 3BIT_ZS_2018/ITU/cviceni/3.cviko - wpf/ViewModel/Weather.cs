using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Media;

namespace WPFWeather.ViewModel
{
    public struct WeatherInfo
    {
        public string CityName { get; private set; }
        public decimal Temperature { get; private set; }
        public decimal Humidity { get; private set; }
        public decimal WindSpeed { get; private set; }
        public decimal WindDirection { get; private set; }
        public string Description { get; private set; }
        public ImageSource Icon { get; private set; }
        public DateTime Sunrise { get; private set; }
        public DateTime Sunset { get; private set; }

        public WeatherInfo(string cityName, decimal temp, decimal humidity, decimal windSpeed, decimal windDirection, string desc, ImageSource icon, DateTime sunrise, DateTime sunset)
            : this()
        {
            CityName = cityName;
            Temperature = temp;
            Humidity = humidity;
            WindSpeed = windSpeed;
            WindDirection = windDirection;
            Description = desc;
            Icon = icon;
            Sunrise = sunrise;
            Sunset = sunset;
        }
    }
}
