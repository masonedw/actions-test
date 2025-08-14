using Blind_Config_Tool.RedesignFiles;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace Blind_Config_Tool
{
    /// <summary>
    /// Interaction logic for Test.xaml
    /// </summary>
    public partial class View_Main : Window
    {
        AppData _appData;
        bool errorDisplayed = false;
        public View_Main()
        {
            InitializeComponent();
            _appData = AppData.Instance;

            _appData.OnErrorDisplay += ShowErrorBox;
            _appData.OnHideError += HideErrorBox;
        }

        void ShowErrorBox(string caption, string title)
        {
            ErrorBoxTitle.Dispatcher.Invoke(() =>
            {
                ErrorBoxTitle.Content = title;
            });

            ErrorBoxCaption.Dispatcher.Invoke(() =>
            {
                ErrorBoxCaption.Text = caption;
            });

            if (!errorDisplayed)
            {
                ErrorBoxTitle.Dispatcher.Invoke(() =>
                {
                    Storyboard st = (Storyboard)Resources["DisplayErrorBox"];
                    st.Begin();
                    errorDisplayed = true;
                });
            }
            

            _appData.SetTimer(AppData.Timers.HIDE_ERROR_DISPLAY, 8000);
        }

        void HideErrorBox()
        {
            ErrorBoxTitle.Dispatcher.Invoke(() =>
            {
                Storyboard st = (Storyboard)Resources["HideErrorBox"];
                st.Begin();
                errorDisplayed = false;
                _appData.CancelTimer(AppData.Timers.HIDE_ERROR_DISPLAY);
            });
        }
    }
}
