using System.Windows;
using System.Windows.Input;
using WPFWeather.ViewModel;
using System.Windows.Controls;

namespace WPFWeather.Windows
{
    public partial class MainWindow : Window
    {
        private WeatherViewModel viewmodel = new WeatherViewModel();

        public MainWindow()
        {
            InitializeComponent();

            // Set DataContext of this window to main ViewModel of application
            viewmodel.DownloadWeather("London");
            DataContext = viewmodel;
        }
        private void changeCity_Click(object sender, RoutedEventArgs e)
        {
            string objname = ((MenuItem)sender).Name;
            //MessageBox.Show(objname);
            viewmodel.DownloadWeather(objname);
            DataContext = viewmodel;
        }
        private void enableMenu_Click(object sender, RoutedEventArgs e)
        {
            (sender as Button).ContextMenu.IsEnabled = true;
            (sender as Button).ContextMenu.IsOpen = true;
            
        }
    }
}
