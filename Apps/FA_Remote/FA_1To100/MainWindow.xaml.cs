using FA_1To100.MVVM.ViewModel;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace FA_1To100
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        DispatcherTimer _checkPortTimer;
        public static Action PortsRefreshed;

        public MainWindow()
        {
            InitializeComponent();
            MainViewModel.OnScrollRequest += StartAnimation;

            _checkPortTimer = new DispatcherTimer();
            _checkPortTimer.Interval = TimeSpan.FromSeconds(2);
            _checkPortTimer.Tick += CheckPorts;
            _checkPortTimer.Start();
        }

        private void TextBox_LostFocus(object sender, RoutedEventArgs e)
        {
            TextBox tbx = (TextBox)sender;

            if(int.TryParse(tbx.Text, out int value))
            {
                if(value < 1)
                {
                    tbx.Text = "1";
                }
                else if(value > 100)
                {
                    tbx.Text = "100";
                }
            }
        }

        private void TextBox_PreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            Regex regex = new Regex("[^0-9]+");
            e.Handled = regex.IsMatch(e.Text);
        }

        private void TextBox_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            // If the user presses the spacebar, prevent it
            if (e.Key == Key.Space || e.Key == Key.Decimal || (e.Key == Key.V && (Keyboard.Modifiers & ModifierKeys.Control) == ModifierKeys.Control))
            {
                e.Handled = true;  // Block space from being entered
            }
        }

        private void StartAnimation()
        {
            // Create the animation
            DoubleAnimation animation = new DoubleAnimation
            {
                From = 410,                   // Start position (right)
                To = -500,                   // End position (left)
                Duration = TimeSpan.FromSeconds(5),  // Duration of the animation
                AutoReverse = false,        // No auto reverse (it won't move back)
            };

            ScrollMessage.Text = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
            // Apply the animation to the X property of the TranslateTransform
            ScrollMessageTranslateTransform.BeginAnimation(TranslateTransform.XProperty, animation);

            ColorAnimationUsingKeyFrames colorAnimation = new ColorAnimationUsingKeyFrames
            {
                Duration = TimeSpan.FromSeconds(5),
                RepeatBehavior = RepeatBehavior.Forever
            };

            // Add the rainbow color keyframes
            colorAnimation.KeyFrames.Add(new LinearColorKeyFrame(Colors.Red, KeyTime.FromTimeSpan(TimeSpan.FromSeconds(0))));
            colorAnimation.KeyFrames.Add(new LinearColorKeyFrame(Colors.Orange, KeyTime.FromTimeSpan(TimeSpan.FromSeconds(0.7))));
            colorAnimation.KeyFrames.Add(new LinearColorKeyFrame(Colors.Yellow, KeyTime.FromTimeSpan(TimeSpan.FromSeconds(1.4))));
            colorAnimation.KeyFrames.Add(new LinearColorKeyFrame(Colors.Green, KeyTime.FromTimeSpan(TimeSpan.FromSeconds(2.1))));
            colorAnimation.KeyFrames.Add(new LinearColorKeyFrame(Colors.Blue, KeyTime.FromTimeSpan(TimeSpan.FromSeconds(2.8))));
            colorAnimation.KeyFrames.Add(new LinearColorKeyFrame(Colors.Indigo, KeyTime.FromTimeSpan(TimeSpan.FromSeconds(3.5))));
            colorAnimation.KeyFrames.Add(new LinearColorKeyFrame(Colors.Violet, KeyTime.FromTimeSpan(TimeSpan.FromSeconds(4.2))));

            // Animate the Foreground property
            ScrollMessage.Foreground = new SolidColorBrush(Colors.Red); // Initial color
            ScrollMessage.Foreground.BeginAnimation(SolidColorBrush.ColorProperty, colorAnimation);

            animation.Completed += (s, args) =>
            {
                // Change the text once the animation is finished
                ScrollMessage.Text = "";
            };
        }

        void CheckPorts(object sender, EventArgs e)
        {
            var provider = (ObjectDataProvider)Resources["portNames"];
            provider.Refresh();
            PortsRefreshed?.Invoke();
        }
    }
}
