using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Blind_Config_Tool.Core
{
    public enum ElementActions
    {
        DO_NOTHING,

        //=== MOTOR COMMANDS ===//
        MOTOR_SEARCH,
        MOTOR_VALIDATE,
        MOTOR_FACTORY_RESET,
        MOTOR_GET_ADDRESS,

        MOTOR_GET_NAME,
        MOTOR_GET_GROUP_IDS,
        MOTOR_GET_INTERNAL_POSITIONS,
        MOTOR_GET_SINGLE_INTERNAL_POSITION,
        MOTOR_GET_ORIENTATION,
        MOTOR_GET_LIMITS,
        MOTOR_GET_SPEEDS,

        MOTOR_SET_NAME,
        MOTOR_SET_ADDRESS,
        MOTOR_SET_UP_LIMIT,
        MOTOR_SET_DOWN_LIMIT,
        MOTOR_SET_ORIENTATION,
        MOTOR_SET_SPEEDS,
        MOTOR_SET_GROUP_ID,
        MOTOR_SET_INTERNAL_POSITION,
        MOTOR_SET_IP_TO_CURRENT_POSITION,

        MOTOR_MOVE_UP,
        MOTOR_MOVE_DOWN,
        MOTOR_STOP,
        MOTOR_START_ID_MOVEMENT,
        MOTOR_MOVE_TO_POSITION,
        MOTOR_MOVE_TO_SELECTED_IP,
        MOTOR_MOVE_TO_UP_LIMIT,
        MOTOR_MOVE_TO_DOWN_LIMIT,

        //=== KEYPAD COMMANDS ===//
        KEYPAD_SEARCH,
        KEYPAD_VALIDATE,
        KEYPAD_GET_NAME,
        KEYPAD_GET_COMMANDS,
        KEYPAD_GET_GROUP_ADDRESSES,
        KEYPAD_GET_ADDRESS,

        KEYPAD_SET_COMMANDS,
        KEYPAD_SET_ADDRESS,
        KEYPAD_SET_GROUP_ADDRESSES,
        KEYPAD_SET_NAME,

        //=== SERIAL COMMANDS ===//
        GET_IP_ADDRESS,
        GET_MAC_ADDRESS,

        SET_IP_ADDRESS,
        SET_MAC_ADDRESS,

        SET_SERVER_IP
    }
}
