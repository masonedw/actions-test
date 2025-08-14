using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Blind_Config_Tool.Core
{
    internal class IPListEntry : INotifyPropertyChanged
    {
        string _ip_Data;
        string _subnet_Data;
        string _port_Data;

        public IPListEntry(string ip = "0.0.0.0", string subnet = "255.255.255.0", string port = "0") 
        {
            IP_Data = ip;
            Subnet_Data = subnet;

            Port_Data = port;
        }

        public event PropertyChangedEventHandler PropertyChanged;
        protected void OnPropertyChanged(string propertyName)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        public string IP_Data { get { return _ip_Data; } set { _ip_Data = value; OnPropertyChanged("IP_Data"); } }
        public string Subnet_Data { get { return _subnet_Data; } set { _subnet_Data = value; OnPropertyChanged("Subnet_Data"); } }
        public string Port_Data { get { return _port_Data; } set { _port_Data = value; OnPropertyChanged("Port_Data"); } }
    }
}
