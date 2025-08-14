using Blind_Config_Tool.Core;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Net;
using System.Runtime.CompilerServices;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Markup;

namespace Blind_Config_Tool.RedesignFiles
{
    public class DeviceData : INotifyPropertyChanged
    {
        protected byte[] _addressBytes;   //The device's address in bytes
        protected string _addressString;  //The device's address shown as a string
        protected string _nameString;     //The device's name as a string
        protected bool _validated;        //Set to true when we confirm that the device is responding to our messages
        protected int _validationAttempts;  //How many times the device has tried to be validated and failed

        public void GetAddressFromMsg(byte[] msg, int startPos, bool reverseBytes = false)   //Retrieves the address from the bytes and stores it
        {
            byte[] newAddr = new byte[Constants.AddressLength];   //Create an array to store the address

            for (int i = 0; i < Constants.AddressLength; i++) //Loop through the message to get the address
            {
                if (reverseBytes)   //If the address should be reversed, count backwards. Otherwise count forwards
                {
                    newAddr[i] = msg[(Constants.AddressLength - 1) - i + startPos];
                }
                else
                {
                    newAddr[i] = msg[i + startPos];
                }
            }

            _addressBytes = newAddr;    //Store the address
            _addressString = BitConverter.ToString(_addressBytes).Replace("-", "");      //Convert address to a string
        }

        public bool TryValidate(byte[] msg)
        {
            for (int i = 0; i < msg.Length; i++)  //Loop through the message to check if it's an ack or nack
            {
                if (msg[i] == Constants.AckByte && (i + Constants.ackNackLength) < msg.Length)
                {
                    _validated = true;
                    return true;
                }
                else if (msg[i] == Constants.NackByte && (i + Constants.ackNackLength) < msg.Length)
                {
                    _validationAttempts++;
                    return false;
                }
            }

            return false;
        }

        #region Comparison Functions
        public bool CompareAddress(byte[] address)
        { 
            return address.SequenceEqual(_addressBytes);
        }
        public bool CompareAddress(string address)
        {
            return address == _addressString;
        }

        public bool CompareName(string name)
        {
            return name == _nameString;
        }
        #endregion

        public void ChangeAddress(byte[] newAddress)
        {
            AddressAsBytes = newAddress;
            AddressAsString = BitConverter.ToString(newAddress);
        }

        public event PropertyChangedEventHandler PropertyChanged;

        protected virtual void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        public byte[] AddressAsBytes { get { return _addressBytes; } set { _addressBytes = value; OnPropertyChanged(); } }
        public string AddressAsString { get { return _addressString; } set { _addressString = value; OnPropertyChanged(); OnPropertyChanged(nameof(FullName)); } }
        public string NameAsString { get { return _nameString; } set { _nameString = value; OnPropertyChanged(); OnPropertyChanged(nameof(FullName)); } }
        public string FullName { get { return $"{_nameString} ({_addressString})"; } }
        public bool Validated { get { return _validated; } set { _validated = value; OnPropertyChanged(); } }
        public int ValidationAttempts { get { return _validationAttempts; } set { _validationAttempts = value; OnPropertyChanged(); } }
    }
}
