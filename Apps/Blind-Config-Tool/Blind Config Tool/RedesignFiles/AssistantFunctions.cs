using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
//test
namespace Blind_Config_Tool.RedesignFiles
{
    public static class AssistantFunctions
    {
        public static byte[] StringToByteArray(string str)
        {
            byte[] arr = new byte[str.Length / 2];    //Create a new array that is half the length of the string (due to each byte being 2 chars)

            for (int i = 0; i < str.Length; i += 2)
            {
                arr[i / 2] = Convert.ToByte(str.Substring(i, 2), 16);
            }

            return arr;
        }

        public static byte[] InvertBytes(byte[] msg)                   //Flip all bytes in the message (e.g. 0xFF becomes 0x00)
        {
            for (int i = 0; i < msg.Length; i++)                  //Loop through all bytes in the message
            {
                byte byt = (byte)(~msg[i]);                      //Flip the byte
                msg[i] = byt;                                    //Replace the byte with the flipped version
            }

            return msg;                                          //Return the message
        }

        public static MessageStructure InvertMessage(MessageStructure msg)  //Flip the bytes in the message (e.g. 0xFF becomes 0x00)
        {
            for(int i = 0; i < msg.Command.Length; i++)
            {
                msg.Command[i] = (byte)(~msg.Command[i]);
            }

            for (int i = 0; i < msg.Length.Length; i++)
            {
                msg.Length[i] = (byte)(~msg.Length[i]);
            }

            for (int i = 0; i < 3; i++)
            {
                msg.Sender[i] = (byte)(~msg.Sender[i]);
                msg.Receiver[i] = (byte)(~msg.Receiver[i]);
            }

            for (int i = 0; i < msg.Data.Length; i++)
            {
                msg.Data[i] = (byte)(~msg.Data[i]);
            }

            return msg;                                          //Return the message
        }

        public static byte ReverseHexCharacters(byte input) //Reverses the characters in a hex value (so 6F would become F6)
        {
            byte highNibble = (byte)((input >> 4) & 0x0F); // Get the high nibble (D)
            byte lowNibble = (byte)(input & 0x0F);         // Get the low nibble (0)

            // Swap the nibbles by shifting and combining them
            byte flippedByte = (byte)((lowNibble << 4) | highNibble); // Swap nibbles

            return flippedByte;
        }
    }
}
