using System;
using System.ComponentModel;
using System.IO;
using System.Net;
using System.Windows;
using System.Windows.Media.Imaging;
using Newtonsoft.Json.Linq;
using System.Windows.Media;
using System.Threading;
using System.Windows.Threading;

namespace WPFWeather.ViewModel
{
    /// <summary>
    /// Main ViewModel for application.
    /// </summary>
    public class WeatherViewModel : INotifyPropertyChanged
    {
        // AppId for OpenWeatherMap
        private readonly string OpenWeatherMapAppId = "5174c48b50122686367191e785842895";

        // Weather downloader background worker
        private BackgroundWorker weatherWorker;

        /// <summary>
        /// True when weather is currently being downloaded from online service.
        /// </summary>
        public bool DownloadingWeather { get; private set; }

        /// <summary>
        /// Indicates if the download was successful.
        /// </summary>
        public bool IsWeatherValid { get; private set; }

        /// <summary>
        /// Error message returned by server.
        /// </summary>
        public string ErrorMessage { get; private set; }

        /// <summary>
        /// Weather info.
        /// </summary>
        public WeatherInfo Weather { get; private set; }

        public WeatherViewModel()
        {
            weatherWorker = new BackgroundWorker();
            weatherWorker.DoWork += new DoWorkEventHandler(weatherWorker_DoWork);
            weatherWorker.RunWorkerCompleted += new RunWorkerCompletedEventHandler(weatherWorker_RunWorkerCompleted);

            IsWeatherValid = false;
            DownloadingWeather = false;
        }

        public void DownloadWeather(string city)
        {
            DownloadingWeather = true;
            OnPropertyChanged("DownloadingWeather");

            weatherWorker.RunWorkerAsync(city);
        }

        void weatherWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            // Get city name from argument
            string city = (string)e.Argument;
            city = city.ToLower();

            // Add city to address line
            /*string address = string.Format(@"http://api.openweathermap.org/data/2.5/weather?q={0}&APPID={1}&mode=json&units=metric", city, OpenWeatherMapAppId);

            // Create HTTP request
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(address);
            request.Method = "GET";

            // Get response
            HttpWebResponse response = (HttpWebResponse)request.GetResponse();

            // Read response to string
            string jsonString = new StreamReader(response.GetResponseStream()).ReadToEnd();*/

            // Simulate downloading from website
            Thread.Sleep(250);

            // Check if file exists
            if (File.Exists(string.Format("Data/{0}.json", city)))
            {

                // Get data from file
                string jsonString;
                using (FileStream stream = File.OpenRead(string.Format("Data/{0}.json", city)))
                {
                    using (StreamReader reader = new StreamReader(stream))
                    {
                        jsonString = reader.ReadToEnd();
                    }
                }


                // Parse json string and get relevant data
                JObject root = JObject.Parse(jsonString);

                // Get response code
                JToken codToken = root["cod"];

                if (root.Value<int>("cod") == 200)
                {
                    // Get temperature and humidity
                    JToken main = root["main"];
                    decimal temperature = main.Value<decimal>("temp");
                    decimal humidity = main.Value<decimal>("humidity");

                    // Get city name
                    string cityName = root.Value<string>("name");

                    // Get weather description and icon
                    JArray weatherArray = (JArray)root["weather"];
                    string desc = weatherArray[0].Value<string>("main");

                    string iconName = weatherArray[0].Value<string>("icon");

                    ImageSource icon = null;
                    DispatcherOperation op = Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            icon = new BitmapImage(new Uri("/Icons/" + iconName + ".png", UriKind.RelativeOrAbsolute));
                        }
                    ));

                    while (op.Status != DispatcherOperationStatus.Completed) { op.Wait(TimeSpan.FromMilliseconds(500)); }

                    // Get sunrise and sunset time
                    JToken sys = root["sys"];
                    DateTime sunrise = UnixTimeStampToDateTime(sys.Value<long>("sunrise"));
                    DateTime sunset = UnixTimeStampToDateTime(sys.Value<long>("sunset"));

                    // Get wind speed and direction
                    JToken wind = root["wind"];
                    decimal windSpeed = wind.Value<decimal>("speed");
                    decimal windDirection = wind.Value<decimal>("deg");

                    IsWeatherValid = true;

                    Weather = new WeatherInfo(cityName, temperature, humidity, windSpeed, windDirection, desc, icon, sunrise, sunset);
                }
                else
                {
                    // Some error occured
                    JToken message = root["message"];
                    ErrorMessage = message.Value<string>();
                    OnPropertyChanged("ErrorMessage");

                    IsWeatherValid = false;
                }
            }
            else
            {
                ErrorMessage = "You can choose only Brno or Praha :( Sorry!";
                OnPropertyChanged("ErrorMessage");

                IsWeatherValid = false;
            }
        }

        void weatherWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            DownloadingWeather = false;
            OnPropertyChanged("DownloadingWeather");
            OnPropertyChanged("IsWeatherValid");
            OnPropertyChanged("Weather");
        }

        public static DateTime UnixTimeStampToDateTime(long unixTimeStamp)
        {
            // Unix timestamp is seconds past epoch
            DateTime dtDateTime = new DateTime(1970, 1, 1, 0, 0, 0, 0, DateTimeKind.Utc);
            dtDateTime = dtDateTime.AddSeconds(unixTimeStamp).ToLocalTime();

            return dtDateTime;
        }

        #region INotifyPropertyChanged members

        public event PropertyChangedEventHandler PropertyChanged;

        private void OnPropertyChanged(string propertyName)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }

        #endregion
    }
}
