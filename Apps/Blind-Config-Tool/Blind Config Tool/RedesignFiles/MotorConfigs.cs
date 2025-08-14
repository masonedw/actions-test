using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Blind_Config_Tool.RedesignFiles
{
    public static class MotorConfigs
    {
        static MotorTypeParams defaultParams = new MotorTypeParams("Unknown", 10, 25, 10, 25, 10, 25, true);
        static MotorTypeParams Node_40AC = new MotorTypeParams("Ø40 AC Series RS485", 10, 25, 10, 25, 10, 25, true);
        static MotorTypeParams Node_50DC = new MotorTypeParams("Ø50 DC Series RS485", 10, 25, 10, 25, 10, 25, false);
        static MotorTypeParams Node_50AC = new MotorTypeParams("Ø50 AC Series RS485", 10, 25, 10, 25, 10, 25, false);
        static MotorTypeParams Node_Glydea = new MotorTypeParams("Glydea RS485", 10, 25, 10, 25, 10, 25, false);
        static MotorTypeParams Node_RTS = new MotorTypeParams("RS485 RTS Transmitter", 10, 25, 10, 25, 10, 25, false);
        static MotorTypeParams Node_30DC = new MotorTypeParams("Ø30 DC Series RS485", 6, 28, 6, 28, 6, 28, false, true);

        public static MotorTypeParams GetParams(byte type)
        {
            switch(type)
            {
                case 0x09:
                    return Node_40AC;
                case 0x08:
                    return Node_50DC;
                case 0x07:
                    return Node_50AC;
                case 0x06:
                    return Node_Glydea;
                case 0x05:
                    return Node_RTS;
                case 0x0D:
                    return Node_30DC;
            default:
                    return defaultParams;
            }
        }
    }

    public class MotorTypeParams
    {
        public readonly string NodeName;
        public readonly int UpRPM_Min;
        public readonly int UpRPM_Max;

        public readonly int DownRPM_Min;
        public readonly int DownRPM_Max;

        public readonly int SlowRPM_Min;
        public readonly int SlowRPM_Max;

        public readonly bool TwoFastSpeeds;

        public readonly bool ModifiableAddress;

        public MotorTypeParams(string name, int upRPM_min = 10, int upRPM_Max = 25, int downRPM_min = 10, int downRPM_max = 25, int slowRPM_Min = 10, int slowRPM_Max = 25, bool modifiableAddress = true, bool twoFastSpeeds = false)
        {
            NodeName = name;

            UpRPM_Min= upRPM_min;
            UpRPM_Max = upRPM_Max;

            DownRPM_Min= downRPM_min;
            DownRPM_Max= downRPM_max;

            SlowRPM_Min = slowRPM_Min;
            SlowRPM_Max = slowRPM_Max;

            ModifiableAddress = modifiableAddress;
            TwoFastSpeeds = twoFastSpeeds;
        }
    }
}
