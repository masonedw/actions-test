using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Blind_Config_Tool.RedesignFiles
{
    public class EthernetDevice : INotifyPropertyChanged
    {
        public string deviceName;
        public string ip;
        public string macAddress;

        public EthernetDevice(string _name, string ip, string mac)
        {
            deviceName = _name;
            this.ip = ip;
            macAddress = mac;
        }

        public event PropertyChangedEventHandler PropertyChanged;

        protected virtual void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        public string DeviceName { get { return deviceName; } set { deviceName = value; OnPropertyChanged(); OnPropertyChanged(nameof(FullName)); } }
        public string IP { get { return ip; } set { ip = value; OnPropertyChanged(); OnPropertyChanged(nameof(FullName)); } }
        public string FullName { get { return $"{deviceName} ({ip})"; } }
    }
}
