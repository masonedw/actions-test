using Blind_Config_Tool.RedesignFiles.MVVM.ViewModel;
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

namespace Blind_Config_Tool.RedesignFiles.MVVM.View
{
    /// <summary>
    /// Interaction logic for View_MotPage2.xaml
    /// </summary>
    public partial class View_MotPage2 : UserControl
    {
        public View_MotPage2()
        {
            InitializeComponent();
            VM_MotPage2.ErrorDisplayed += HighlightError;
        }

        private void HexOnly(object sender, TextCompositionEventArgs e)
        {
            // Check if the character is a valid hexadecimal digit (0-9, A-F, a-f)
            if (!Regex.IsMatch(e.Text, @"^[0-9A-Fa-f]$"))
            {
                // If the character is not a valid hex digit, mark the event as handled (reject input)
                e.Handled = true;
            }
        }

        private void SelectAll(object sender, RoutedEventArgs e)
        {
            if (sender is TextBox textBox)
            {
                textBox.SelectAll();
            }
        }

        private void HandleMouseDown(object sender, MouseButtonEventArgs e)
        {
            // Check if the sender is a TextBox and ensure selection is maintained
            if (sender is TextBox textBox)
            {
                // Select all text again when the TextBox is clicked
                SelectAll(textBox, new RoutedEventArgs());

                // Set focus on the TextBox without removing the selection
                textBox.Focus();

                // Prevent the default mouse event handling to stop cursor placement
                e.Handled = true;
            }
        }

        private void NoSpace(object sender, KeyEventArgs e)
        {
            // If the user presses the spacebar, prevent it
            if (e.Key == Key.Space || e.Key == Key.Decimal || (e.Key == Key.V && (Keyboard.Modifiers & ModifierKeys.Control) == ModifierKeys.Control))
            {
                e.Handled = true;  // Block space from being entered
            }
        }

        private void HighlightError(object sender, string label)
        {
            // Create the ColorAnimation
            var colorAnimation = new ColorAnimation
            {
                From = (Color)ColorConverter.ConvertFromString("#FDA172"),         // Starting color
                To = Colors.Transparent,      // Ending color
                Duration = new Duration(TimeSpan.FromSeconds(0.5)), // Duration of 0.5 seconds
                AutoReverse = true,
                RepeatBehavior = new RepeatBehavior(2.5)  // It will play only once
            };

            // Apply the animation to the TextBox's background color
            SolidColorBrush brush = new SolidColorBrush();

            switch (label)
            {
                case "Group IDs":
                    brush = (SolidColorBrush)SetGroupIDsErrorMessage.Background;
                    break;
                case "IPs":
                    brush = (SolidColorBrush)SetIPsErrorMessage.Background;
                    break;
            }

            // Begin the animation on the SolidColorBrush color property
            brush.BeginAnimation(SolidColorBrush.ColorProperty, colorAnimation);
        }

        private void TwoTo98(object sender, TextCompositionEventArgs e)
        {
            TextBox tbx = (TextBox)sender;

            if (int.TryParse(tbx.Text + e.Text, out int value))
            {
                if (value > 98)
                {
                    tbx.Text = "98";
                    e.Handled = true;
                }

                if (value < 2)
                {
                    tbx.Text = "2";
                    e.Handled = true;
                }
            }
            else
            {
                e.Handled = true;
            }
        }

        private void OneTo16(object sender, TextCompositionEventArgs e)
        {
            TextBox tbx = (TextBox)sender;

            if (int.TryParse(tbx.Text + e.Text, out int value))
            {
                if (value > 16)
                {
                    tbx.Text = "16";
                    e.Handled = true;
                }

                if (value < 1)
                {
                    tbx.Text = "1";
                    e.Handled = true;
                }
            }
            else
            {
                e.Handled = true;
            }
        }
    }
}
