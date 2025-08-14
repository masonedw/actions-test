using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Blind_Config_Tool.Core
{
    public enum ScriptCommands
    {
        //FLOW/FREE
        FLOW_BUS_CONTROL = 0x01,
        FLOW_VALIDATE_DEVICE = 0x02,

        //SET COMMANDS
        MOTOR_SET_ID = 0x10,
        MOTOR_SET_LABEL = 0x11,
        MOTOR_SET_DIRECTION = 0x12,
        MOTOR_SET_LIMITS = 0x13,
        MOTOR_SET_SPEED = 0x14,
        MOTOR_SET_GROUP_ID = 0x15,
        MOTOR_SET_INTERNAL_POSITION = 0x16,
        MOTOR_SET_CURRENT_POSITION = 0x17,

        KEYPAD_SET_ID = 0x20,
        KEYPAD_SET_LABEL = 0x21,
        KEYPAD_SET_COMMAND = 0x22,
        KEYPAD_SET_GROUP_ID = 0x23,

        SYS_SET_MAC_ADDRESS = 0x31,
        SYS_SET_IP_ADDRESS = 0x32,

        SYS_SET_SERVER_IP = 0x33,
        
        SYS_SET_CUSTOM = 0x3F,

        PLC_SET_MASTER = 0x40,

        //GET COMMANDS
        MOTOR_GET_ID = 0x50,
        MOTOR_GET_LABEL = 0x51,
        MOTOR_GET_DIRECTION = 0x52,
        MOTOR_GET_LIMITS = 0x53,
        MOTOR_GET_SPEED = 0x54,
        MOTOR_GET_GROUP_ID = 0x55,
        MOTOR_GET_INTERNAL_POSITION = 0x56,
        MOTOR_GET_CURRENT_POSITION = 0x57,

        KEYPAD_GET_ID = 0x60,
        KEYPAD_GET_LABEL = 0x61,
        KEYPAD_GET_COMMAND = 0x62,
        KEYPAD_GET_GROUP_ID = 0x63,

        SYS_GET_SOFTWARE_VERSION = 0x70,
        SYS_GET_MAC_ADDRESS = 0x71,
        SYS_GET_IP_ADDRESS = 0x72,

        SYS_GET_CUSTOM = 0x7F,

        PLC_GET_SLAVE_STATUS = 0x81,

        //CONTROL COMMANDS
        CMD_STOP = 0x90,
        CMD_MOVE = 0x91,
        CMD_TO_INTERNAL_POS = 0x92,
        CMD_NEXT_INTERNAL_POS = 0x93,
        CMD_JOG_MM = 0x94,
        CMD_JOG_PULSE = 0x95,
        CMD_TO_PERCENT = 0x96,
        CMD_VISUAL_FEEDBACK = 0x97,
        CMD_MOVE_TOGGLE = 0x98,

        CMD_CUSTOM = 0x9F,

        //ACK/NACK
        ACK = 0xA0,
        
        NACK = 0xB0,

        //POST/RESPONSE COMMANDS
        MOTOR_POST_ID = 0xC0,
        MOTOR_POST_LABEL = 0xC1,
        MOTOR_POST_DIRECTION = 0xC2,
        MOTOR_POST_LIMITS = 0xC3,
        MOTOR_POST_SPEED = 0xC4,
        MOTOR_POST_GROUP_ID = 0xC5,
        MOTOR_POST_INTERNAL_POSITION = 0xC6,
        MOTOR_POST_CURRENT_POSITION = 0xC7,

        KEYPAD_POST_ID = 0xD0,
        KEYPAD_POST_LABEL = 0xD1,
        KEYPAD_POST_COMMAND = 0xD2,
        KEYPAD_POST_GROUP_ID = 0xD3,

        SYS_POST_SOFTWARE_VERSION = 0xE0,
        SYS_POST_MAC_ADDRESS = 0xE1,
        SYS_POST_IP_ADDRESS = 0xE2,

        SYS_POST_CUSTOM = 0xEF,

        PLC_POST_MASTER = 0xF0,
        PLC_POST_SLAVE_STATUS = 0xF1
    }
}
