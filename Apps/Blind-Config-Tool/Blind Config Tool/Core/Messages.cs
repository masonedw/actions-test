using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Blind_Config_Tool.Core
{
    public enum Messages
    {
        NO_MESSAGE,
        SEARCH_FOR_MOTORS,
        SEARCH_FOR_MOTORS_GROUPS_AND_IPS,
        UPDATE_MOTOR_PAGE,
        UPDATE_GROUPS_AND_IPS,
        GET_BUTTON_DATA_FROM_KEYPAD,
        PROGRAM_KEYPAD_COMMANDS,
        SEARCH_FOR_KEYPADS,
        MOVE_MOTOR_UP,
        MOVE_MOTOR_DOWN,
        MOVE_TO_UP_LIMIT,
        MOVE_TO_DOWN_LIMIT,
        MOVE_TO_SELECTED_IP,
        STOP_MOTOR_MOVEMENT,
        START_IDENTIFICATION_MOVEMENT,
        FACTORY_RESET_MOTOR,
        MOVE_TO_POSITION,
        SET_UP_LIMIT,
        SET_DOWN_LIMIT,
        GET_MOTOR_DATA,
        SAVE_NEW_MOTOR_SETTINGS,
        SAVE_GROUPS_AND_IPS,
        SAVE_NEW_KEYPAD_ADDRESS,
        SET_MAC_ADDRESS,
        SET_IP_ADDRESS,
        GET_IP_ADDRESS,
        SET_MOTOR_ADDRESS,
        SET_KEYPAD_NAME,
        SET_MOTOR_NAME,
        SET_SPEEDS,
        SET_MOVEMENT_DIRECTION,
        SEARCH_FOR_ALL_DEVICES,
        SET_IP_TO_CURRENT_POSITION,
        SET_SERVER_IPS
    }
}
