using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Blind_Config_Tool.RedesignFiles
{
    public static class Constants
    {
        public static readonly byte AckByte = 0x7F;     //Denotes an acknowledgement message
        public static readonly byte NackByte = 0x6F;    //Denotes a non-acknowledgement message
        public static readonly int ackNackLength = 11;  //The length of the acknowledgement/non-acknowledgement message
        public static readonly int AddressLength = 3;   //The length of a device address

        public static readonly int CommandDescVisibleTime = 3000;   //How long the command description stays visible after it's just received a command
    }
}
