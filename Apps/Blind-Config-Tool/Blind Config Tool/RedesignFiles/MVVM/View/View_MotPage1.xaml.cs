using Blind_Config_Tool.Core;
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
    /// Interaction logic for View_MotPage1.xaml
    /// </summary>
    public partial class View_MotPage1 : UserControl
    {
        bool stopRequested;
        public static event Action OnUpPressed;
        public static event Action OnDownPressed;
        public static event Action OnButtonReleased;

        public View_MotPage1()
        {
            VM_MotPage1.ErrorDisplayed += HighlightError;
            InitializeComponent();
        }

        private void NewAddressTextbox_1_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (NewAddressTextbox_1.Text.Length == 2)
            {
                NewAddressTextbox_2.Focus();
            }
        }

        private void NewAddressTextbox_2_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (NewAddressTextbox_2.Text.Length == 2)
            {
                NewAddressTextbox_3.Focus();
            }
        }

        private void NewAddressTextbox_3_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (NewAddressTextbox_3.Text.Length == 2)
            {
                SetNewAddressButton.Focus();
            }
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
                case "Motor Address":
                    brush = (SolidColorBrush)SetAddressErrorMessage.Background;
                    break;
                case "Set Target":
                    brush = (SolidColorBrush)SetTargetPositionErrorMessage.Background;
                    break;
            }

            // Begin the animation on the SolidColorBrush color property
            brush.BeginAnimation(SolidColorBrush.ColorProperty, colorAnimation);
        }

        private void ZeroTo100(object sender, TextCompositionEventArgs e)
        {
            TextBox tbx = (TextBox)sender;

            if (int.TryParse(tbx.Text + e.Text, out int value))
            {
                if (value > 100)
                {
                    tbx.Text = "100";
                    e.Handled = true;
                }

                if (value < 0)
                {
                    tbx.Text = "0";
                    e.Handled = true;
                }
            }
            else
            {
                e.Handled = true;
            }
        }

        private void MouseDown_MoveUp(object sender, MouseButtonEventArgs e)
        {
            //stopRequested = false;
            //CheckUpStillPressed();
            OnUpPressed?.Invoke();
        }

        private void MouseDown_MoveDown(object sender, MouseButtonEventArgs e)
        {
            //stopRequested = false;
            //CheckDownStillPressed();
            OnDownPressed?.Invoke();
        }

        private void MouseUp_Movement(object sender, MouseButtonEventArgs e)
        {
            //stopRequested = true;
            OnButtonReleased?.Invoke();
        }

        private void CheckUpStillPressed()
        {
            if (stopRequested)
            {
                ScriptHandler.Instance.StartScript(Messages.STOP_MOTOR_MOVEMENT);
            }
            else
            {
                ScriptHandler.Instance.StartScript(Messages.MOVE_MOTOR_UP);
            }
        }

        private void CheckDownStillPressed()
        {
            if (stopRequested)
            {
                ScriptHandler.Instance.StartScript(Messages.STOP_MOTOR_MOVEMENT);
            }
            else
            {
                ScriptHandler.Instance.StartScript(Messages.MOVE_MOTOR_DOWN);
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
