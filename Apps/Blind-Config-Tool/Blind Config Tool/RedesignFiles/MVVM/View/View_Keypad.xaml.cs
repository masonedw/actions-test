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
using System.Windows.Forms.VisualStyles;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Blind_Config_Tool.RedesignFiles.MVVM.View
{
    /// <summary>
    /// Interaction logic for View_Keypad.xaml
    /// </summary>
    public partial class View_Keypad : UserControl
    {
        public View_Keypad()
        {
            VM_Keypad.ErrorDisplayed += HighlightError;
            InitializeComponent();
        }

        private void NewKeypadAddressTextbox_1_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (NewKeypadAddressTextbox_1.Text.Length == 2)
            {
                NewKeypadAddressTextbox_2.Focus();
            }
        }

        private void NewKeypadAddressTextbox_2_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (NewKeypadAddressTextbox_2.Text.Length == 2)
            {
                NewKeypadAddressTextbox_3.Focus();
            }
        }

        private void NewKeypadAddressTextbox_3_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (NewKeypadAddressTextbox_3.Text.Length == 2)
            {
                SetNewKeypadAddressButton.Focus();
            }
        }

        private void NewMotorAllAddressTextbox_1_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (NewMotorAllAddressTextbox_1.Text.Length == 2)
            {
                NewMotorAllAddressTextbox_2.Focus();
            }
        }

        private void NewMotorAllAddressTextbox_2_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (NewMotorAllAddressTextbox_2.Text.Length == 2)
            {
                NewMotorAllAddressTextbox_3.Focus();
            }
        }

        private void NewMotorAllAddressTextbox_3_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (NewMotorAllAddressTextbox_3.Text.Length == 2)
            {
                SetMotorAllAddressButton.Focus();
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
            if(sender is TextBox textBox)
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
                case "Motor All":
                    brush = (SolidColorBrush)MotorAllAddressErrorMessage.Background;
                    break;
                case "Keypad Address":
                    brush = (SolidColorBrush)KeypadAddressErrorMessage.Background;
                    break;
                case "Save Button Config":
                    brush = (SolidColorBrush)SaveButtonConfigErrorMessage.Background;
                    break;
                case "Set Name Button":
                    brush = (SolidColorBrush)SetNameErrorMessage.Background;
                    break;
            }

            // Begin the animation on the SolidColorBrush color property
            brush.BeginAnimation(SolidColorBrush.ColorProperty, colorAnimation);
        }

        private void ListOfCommands_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            var selectedItem = (sender as ListBox)?.SelectedItem;

            // Call the ViewModel's command to handle the selection change
            if (DataContext is VM_Keypad viewModel && viewModel.ChangeSelectedButton.CanExecute(selectedItem))
            {
                viewModel.ChangeSelectedButton.Execute(selectedItem);
            }
        }

        private void CheckMinMax(object sender, RoutedEventArgs e)
        {
            TextBox tbx = (TextBox)sender;
            Regex regex = new Regex("[^0-9\\s]+");  // \\s represents any whitespace character
            e.Handled = regex.IsMatch(tbx.Text);

            KeypadButtonAction action = (KeypadButtonAction)OnPressActionComboBox.SelectedItem;

            if(tbx.Name == OnHoldTargetValue.Name)
            {
                action = (KeypadButtonAction)OnHoldActionComboBox.SelectedItem;
            }
            else if (tbx.Name == OnReleaseTargetValue.Name)
            {
                action = (KeypadButtonAction)OnReleaseActionComboBox.SelectedItem;
            }

            CanEnableTextBox(action, out int min, out int max);

            if (!long.TryParse(tbx.Text, out long val))
            {
                val = 0;
                tbx.Text = "0";
            }

            if (val < min)
            {
                tbx.Text = min.ToString();
                e.Handled = true;
            }

            if (val > max)
            {
                tbx.Text = max.ToString();
                e.Handled = true;
            }
        }

        private void EnableTargetBox(object sender, SelectionChangedEventArgs e)
        {
            ComboBox cbx = (ComboBox)sender;

            if(cbx == null || cbx.SelectedItem == null)
            {
                return;
            }

            KeypadButtonAction action = (KeypadButtonAction)cbx.SelectedItem;

            if (cbx.Name == OnPressActionComboBox.Name)
            {
                OnPressTargetValue.IsEnabled = CanEnableTextBox(action, out int min, out int max);
                OnPressTargetValue.Text = min.ToString();
            }
            else if(cbx.Name == OnHoldActionComboBox.Name)
            {
                OnHoldTargetValue.IsEnabled = CanEnableTextBox(action, out int min, out int max);
                OnHoldTargetValue.Text = min.ToString();
            }
            else if(cbx.Name == OnReleaseActionComboBox.Name)
            {
                OnReleaseTargetValue.IsEnabled = CanEnableTextBox(action, out int min, out int max);
                OnReleaseTargetValue.Text = min.ToString();
            }
        }

        private bool CanEnableTextBox(KeypadButtonAction action, out int minValue, out int maxValue)
        {
            minValue = 0;
            maxValue = 0;

            switch(action)
            {
                case KeypadButtonAction.GO_TO_IP:
                    minValue = 1;
                    maxValue = 16;
                    break;
                case KeypadButtonAction.GO_TO_PULSE:
                    maxValue = 65000;
                    break;
                case KeypadButtonAction.JOG_UP_MS:
                    maxValue = 250;
                    break;
                case KeypadButtonAction.JOG_DOWN_MS:
                    maxValue = 250;
                    break;
                case KeypadButtonAction.JOG_UP_PULSE:
                    maxValue = 999;
                    break;
                case KeypadButtonAction.JOG_DOWN_PULSE:
                    maxValue = 999;
                    break;
                case KeypadButtonAction.GO_TO_PERCENT:
                    maxValue = 100;
                    break;
                case KeypadButtonAction.SET_IP:
                    minValue = 1;
                    maxValue = 16;
                    break;
            }

            if(maxValue > 0)
            {
                return true;
            }

            return false;
        }

        private void OnlyNumeric(object sender, TextCompositionEventArgs e)
        {
            Regex regex = new Regex("[^0-9]+");
            e.Handled = regex.IsMatch(e.Text);
        }

        private void NoSpace(object sender, KeyEventArgs e)
        {
            // If the user presses the spacebar, prevent it
            if (e.Key == Key.Space || e.Key == Key.Decimal || (e.Key == Key.V && (Keyboard.Modifiers & ModifierKeys.Control) == ModifierKeys.Control))
            {
                e.Handled = true;  // Block space from being entered
            }
        }

        private void NoDecimal(object sender, KeyEventArgs e)
        {
            // If the user presses the spacebar, prevent it
            if (e.Key == Key.Decimal || (e.Key == Key.V && (Keyboard.Modifiers & ModifierKeys.Control) == ModifierKeys.Control))
            {
                e.Handled = true;  // Block space from being entered
            }
        }

        private void OnlyLetters(object sender, TextCompositionEventArgs e)
        {
            // Create a Regex to allow only alphabetic characters (A-Z, a-z)
            Regex regex = new Regex("^[a-zA-Z]$");

            // Check if the input character matches the regex
            e.Handled = !regex.IsMatch(e.Text);  // If it doesn't match, prevent the input
        }

        private void OnlyAlphanumeric(object sender, TextCompositionEventArgs e)
        {
            // Create a Regex to allow only alphabetic characters (A-Z, a-z)
            Regex regex = new Regex("^[a-zA-Z0-9]$");

            // Check if the input character matches the regex
            e.Handled = !regex.IsMatch(e.Text);  // If it doesn't match, prevent the input
        }
    }
}
