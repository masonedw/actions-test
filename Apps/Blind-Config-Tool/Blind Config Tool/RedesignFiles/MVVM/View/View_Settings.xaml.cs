using Blind_Config_Tool.RedesignFiles.MVVM.ViewModel;
using System;
using System.Collections.Generic;
using System.IO;
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
    /// Interaction logic for View_Settings.xaml
    /// </summary>
    public partial class View_Settings : UserControl
    {
        public View_Settings()
        {
            InitializeComponent();
            VM_Settings.ErrorDisplayed += HighlightError;
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
                case "Serial Connection":
                    brush = (SolidColorBrush)SerialConnectionErrorMessage.Background;
                    break;
                case "IP Configuration":
                    brush = (SolidColorBrush)IPConfigError.Background;
                    break;
                case "IP List":
                    brush = (SolidColorBrush)IPListError.Background;
                    break;
                case "MAC Password":
                    brush = (SolidColorBrush)IncorrectPassword.Background;
                    break;
                case "Ethernet Connection":
                    brush = (SolidColorBrush)EthernetConnectionErrorMessage.Background;
                    break;
            }

            // Begin the animation on the SolidColorBrush color property
            brush.BeginAnimation(SolidColorBrush.ColorProperty, colorAnimation);
        }

        private void IP_1_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (IP_Part1.Text.Length == 3)
            {
                IP_Part2.Focus();
            }
        }

        private void IP_2_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (IP_Part2.Text.Length == 3)
            {
                IP_Part3.Focus();
            }
        }

        private void IP_3_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (IP_Part3.Text.Length == 3)
            {
                IP_Part4.Focus();
            }
        }

        private void IP_4_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (IP_Part4.Text.Length == 3)
            {
                Subnet_Part1.Focus();
            }
        }

        private void EthernetConnection_IP_Part_1_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (EthernetConnection_IP_Part_1.Text.Length == 3)
            {
                EthernetConnection_IP_Part_2.Focus();
            }
        }

        private void EthernetConnection_IP_Part_2_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (EthernetConnection_IP_Part_2.Text.Length == 3)
            {
                EthernetConnection_IP_Part_3.Focus();
            }
        }

        private void EthernetConnection_IP_Part_3_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (EthernetConnection_IP_Part_3.Text.Length == 3)
            {
                EthernetConnection_IP_Part_4.Focus();
            }
        }

        private void EthernetConnection_IP_Part_4_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (EthernetConnection_IP_Part_3.Text.Length == 3)
            {
                Ethernet_Port_Number.Focus();
            }
        }



        private void Subnet_1_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (Subnet_Part1.Text.Length == 3)
            {
                Subnet_Part2.Focus();
            }
        }

        private void Subnet_2_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (Subnet_Part2.Text.Length == 3)
            {
                Subnet_Part3.Focus();
            }
        }

        private void Subnet_3_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (Subnet_Part3.Text.Length == 3)
            {
                Subnet_Part4.Focus();
            }
        }
        private void Subnet_4_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (Subnet_Part4.Text.Length == 3)
            {
                PortNum.Focus();
            }
        }

        private void IPList_IP_1_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (IPList_IP_Part1.Text.Length == 3)
            {
                IPList_IP_Part2.Focus();
            }
        }

        private void IPList_IP_2_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (IPList_IP_Part2.Text.Length == 3)
            {
                IPList_IP_Part3.Focus();
            }
        }

        private void IPList_IP_3_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (IPList_IP_Part3.Text.Length == 3)
            {
                IPList_IP_Part4.Focus();
            }
        }

        private void IPList_IP_4_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (IPList_IP_Part4.Text.Length == 3)
            {
                IPList_Subnet_Part1.Focus();
            }
        }

        private void IPList_Subnet_1_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (IPList_Subnet_Part1.Text.Length == 3)
            {
                IPList_Subnet_Part2.Focus();
            }
        }

        private void IPList_Subnet_2_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (IPList_Subnet_Part2.Text.Length == 3)
            {
                IPList_Subnet_Part3.Focus();
            }
        }

        private void IPList_Subnet_3_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (IPList_Subnet_Part3.Text.Length == 3)
            {
                IPList_Subnet_Part4.Focus();
            }
        }

        private void IPList_Subnet_4_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (IPList_Subnet_Part4.Text.Length == 3)
            {
                IPList_PortNum.Focus();
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

        private void OnlyNumeric(object sender, TextCompositionEventArgs e)
        {
            Regex regex = new Regex("[^0-9]+");
            e.Handled = regex.IsMatch(e.Text);
        }

        private void OnlyTo255(object sender, TextCompositionEventArgs e)
        {
            Regex regex = new Regex("[^0-9\\s]+");  // \\s represents any whitespace character
            e.Handled = regex.IsMatch(e.Text);

            TextBox tbx = (TextBox)sender;

            if (int.TryParse(e.Text + tbx.Text, out int val))
            {
                if (val > 255)
                {
                    tbx.Text = "255";
                    e.Handled = true;
                }
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

        private void OnlyTo255(object sender, RoutedEventArgs e)
        {
            TextBox tbx = (TextBox)sender;

            if (int.TryParse(tbx.Text, out int val))
            {
                if (val > 255)
                {
                    tbx.Text = "255";
                    e.Handled = true;
                }
            }
        }

        private void OnlyTo65535(object sender, RoutedEventArgs e)
        {
            TextBox tbx = (TextBox)sender;

            if (int.TryParse(tbx.Text, out int val))
            {
                if (val > 65535)
                {
                    tbx.Text = "65535";
                    e.Handled = true;
                }
            }
        }

        private void ListOfIPs_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            var selectedItem = (sender as ListBox)?.SelectedItem;

            // Call the ViewModel's command to handle the selection change
            if (DataContext is VM_Settings viewModel && viewModel.ChangeSelectedIPListEntry.CanExecute(selectedItem))
            {
                viewModel.ChangeSelectedIPListEntry.Execute(selectedItem);
            }
        }
    }
}
