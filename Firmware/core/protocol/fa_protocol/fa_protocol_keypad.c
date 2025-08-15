/*******************************************************************************
  Future Automation Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    fa_protocol_common.c

  Summary:
    This file contains the source code for the Future Automation RS485 interface.

  Description:
    This file contains the source code for the Future Automation RS485 interface.
    This provides all transport layer and command-set functionality

 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "protocol_common.h"
#include "fa_protocol.h"
#include "fa_protocol_keypad.h"
#include "fa_protocol_cmd_list.h"
#include "fa_eeprom_map.h"
#include "fa_eeprom.h"


// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define MOTOR_MOVE_CMD_DATA_LENGTH 2

// *****************************************************************************
// *****************************************************************************
// Config states
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Global Definitions
// *****************************************************************************
// *****************************************************************************

#ifdef FRAMEWORK_PCB_0117
#elif  FRAMEWORK_PCB_0120
#define FA_MAX_BUTTONS 12
#elif  FRAMEWORK_PCB_0123
#define FA_MAX_BUTTONS 100
#else
#define FA_MAX_BUTTONS 8
#endif

static t_FA_PROTOCOL_KEYPAD_MessageStructure KeyPadCmds[FA_MAX_BUTTONS];
static bool KeypadMemoryChanged[FA_MAX_BUTTONS] = {0};
static bool bSaveKeypadCMD_Master = false;

// Flag which controls when to not respond to a certain message.
// The flag can be set, and it will be cleared after the inactive timeout
// timer has finished.
static bool bSignOnOnce = false;
// *****************************************************************************
// *****************************************************************************
// Section: Forward references
// *****************************************************************************
// *****************************************************************************




// *****************************************************************************
// *****************************************************************************
// Section: fa protocol Functions
// *****************************************************************************
// *****************************************************************************

/******************************************************************************
  External Function:
    FA_SDN_KEYPAD_reset_eeprom

  Remarks: reset the keypad specific eeprom

 */
void FA_PROTOCOL_KEYPAD_reset_eeprom(void) {
    t_FA_PROTOCOL_KEYPAD_MessageStructure keypadStopCommand;

    keypadStopCommand.u8_OnPress_Command = CMD_Stop;
    keypadStopCommand.u8_OnPress_Target_Value_L = 0xFF;
    keypadStopCommand.u8_OnPress_Target_Value_H = 0xFF;

    keypadStopCommand.u8_OnHold_Command = CMD_Stop;
    keypadStopCommand.u8_OnHold_Target_Value_L = 0xFF;
    keypadStopCommand.u8_OnHold_Target_Value_H = 0xFF;

    keypadStopCommand.u8_OnRelease_Command = CMD_Stop;
    keypadStopCommand.u8_OnRelease_Target_Value_L = 0xFF;
    keypadStopCommand.u8_OnRelease_Target_Value_H = 0xFF;

    keypadStopCommand.u8_Target_ID_1 = 0xFF;
    keypadStopCommand.u8_Target_ID_2 = 0xFF;
    keypadStopCommand.u8_Target_ID_3 = 0xFF;

    for (int i = EEPROM_KEYPAD_COMMANDS; i < EEPROM_KEYPAD_COMMANDS + SIZEOF_KEYPAD_COMMANDS; i += sizeof (t_FA_PROTOCOL_KEYPAD_MessageStructure)) {
        FA_EEPROM_write_block(i, (uint8_t*) & keypadStopCommand, sizeof (keypadStopCommand));
    }
    FA_EEPROM_fill_block(EEPROM_KEYPAD_ToggleState, 0x00, KEYPAD_TOGGLESTATE_SIZE);
}

void FA_PROTOCOL_KEYPAD_INITIALIZE(void) {
    FA_EEPROM_read_block(EEPROM_KEYPAD_COMMANDS, (uint8_t*)KeyPadCmds, SIZEOF_KEYPAD_COMMANDS);
}

/******************************************************************************
external Function:

  Remarks:

 */
void FA_PROTOCOL_Handle_Keypad_Message(t_InterfaceType e_InterfaceType, t_fa_protocol_message * u8_CurrentRx) {
    switch (u8_CurrentRx->MessageType) {
            //*********************************
            //Flow / Free Commands
            //*********************************
        case Take_Control_of_Bus:
            break;
        case Validate_Device:
            if (FA_PROTOCOL_Check_For_DeviceID(e_InterfaceType) && FA_PROTOCOL_Check_ForResponseBit(e_InterfaceType)) {
                FA_PROTOCOL_Send_Ack(e_InterfaceType);
                bSignOnOnce = true;
            }
            break;

            //*********************************
            //Set Motor Info
            //*********************************
            //        case Set_Motor_ID:
            //            break;
            //        case Set_Motor_Label:
            //            break;
            //        case Set_Motor_Direction:
            //            break;
            //        case Set_Motor_Limits:
            //            break;
            //        case Set_Motor_Speed:
            //            break;
            //        case Set_Motor_Group_ID:
            //            break;
            //        case Set_Motor_Internal_Position:
            //            break;
            //        case Set_Current_Position:

            //            break;
            //*********************************
            //Set Keypad Info
            //*********************************
        case Set_Keypad_ID:
            if (FA_PROTOCOL_Check_For_DeviceID(e_InterfaceType)) {
                if ((u8_CurrentRx->Length - MSG_NO_PAYLOAD) == DEVICE_ID_LENGTH) {
                    if (FA_PROTOCOL_COMMON_Set_Device_ID(u8_CurrentRx->Data)) {
                        if (FA_PROTOCOL_Check_ForResponseBit(e_InterfaceType)) {
                            FA_PROTOCOL_Send_Ack(e_InterfaceType);
                        }
                    } else if (FA_PROTOCOL_Check_ForResponseBit(e_InterfaceType)) {
                        FA_PROTOCOL_Send_Nack(e_InterfaceType);
                    }
                }
            }
            break;
        case Set_Keypad_Label:
            if (FA_PROTOCOL_Check_For_DeviceID(e_InterfaceType)) {
                // Check that the Set Keypad Label message contains 16 bytes in the data section of the message.
                if ((u8_CurrentRx->Length - MSG_NO_PAYLOAD) == LABEL_LENGTH) {
                    FA_PROTOCOL_COMMON_Set_Label(u8_CurrentRx->Data);
                    if (FA_PROTOCOL_Check_ForResponseBit(e_InterfaceType)) {
                        FA_PROTOCOL_Send_Ack(e_InterfaceType);
                    }
                } else if (FA_PROTOCOL_Check_ForResponseBit(e_InterfaceType)) {
                    FA_PROTOCOL_Send_Nack(e_InterfaceType);
                }
            }
            break;
        case Set_Keypad_CMD:
            if (FA_PROTOCOL_Check_For_DeviceID(e_InterfaceType)) {
                uint8_t cmdIndex = 0;

                if ((u8_CurrentRx->Length - MSG_NO_PAYLOAD) == CMD_LENGTH) {

                    cmdIndex = u8_CurrentRx->Data[0] - 1;
                    if (cmdIndex < FA_MAX_BUTTONS) {
                        KeyPadCmds[cmdIndex].u8_OnPress_Command = u8_CurrentRx->Data[1];
                        KeyPadCmds[cmdIndex].u8_OnPress_Target_Value_L = u8_CurrentRx->Data[2];
                        KeyPadCmds[cmdIndex].u8_OnPress_Target_Value_H = u8_CurrentRx->Data[3];

                        KeyPadCmds[cmdIndex].u8_OnHold_Command = u8_CurrentRx->Data[4];
                        KeyPadCmds[cmdIndex].u8_OnHold_Target_Value_L = u8_CurrentRx->Data[5];
                        KeyPadCmds[cmdIndex].u8_OnHold_Target_Value_H = u8_CurrentRx->Data[6];

                        KeyPadCmds[cmdIndex].u8_OnRelease_Command = u8_CurrentRx->Data[7];
                        KeyPadCmds[cmdIndex].u8_OnRelease_Target_Value_L = u8_CurrentRx->Data[8];
                        KeyPadCmds[cmdIndex].u8_OnRelease_Target_Value_H = u8_CurrentRx->Data[9];

                        KeyPadCmds[cmdIndex].u8_Target_ID_1 = u8_CurrentRx->Data[10];
                        KeyPadCmds[cmdIndex].u8_Target_ID_2 = u8_CurrentRx->Data[11];
                        KeyPadCmds[cmdIndex].u8_Target_ID_3 = u8_CurrentRx->Data[12];
                        KeypadMemoryChanged[cmdIndex] = true;

                        bSaveKeypadCMD_Master = true;

                        if (FA_PROTOCOL_Check_ForResponseBit(e_InterfaceType)) {
                            FA_PROTOCOL_Send_Ack(e_InterfaceType);
                        }
                    } else if (FA_PROTOCOL_Check_ForResponseBit(e_InterfaceType)) {
                        FA_PROTOCOL_Send_Nack(e_InterfaceType);
                    }
                }
            }
            break;
            //*********************************
            //Set General Info
            //*********************************
            //        case Set_MAC_Adrs:
            //            break;
        case Set_IP_Adrs:
            if (FA_PROTOCOL_Check_For_DeviceID(e_InterfaceType)) {
                if ((u8_CurrentRx->Length - MSG_NO_PAYLOAD) == IP_LENGTH) {
                    bool b_DHCP_Enabled;
                    IPV4_ADDR t_NewIP;
                    SUBNET_MASK t_Subnet;
                    uint16_t u16_Port;

                    b_DHCP_Enabled = u8_CurrentRx->Data[0];
                    memcpy(&u16_Port, &u8_CurrentRx->Data[1], sizeof (u16_Port));
                    memcpy(&t_NewIP, &u8_CurrentRx->Data[3], sizeof (t_NewIP));
                    memcpy(&t_Subnet, &u8_CurrentRx->Data[7], sizeof (t_Subnet));

                    FA_PROTOCOL_COMMON_Set_IPv4_Address(b_DHCP_Enabled, t_NewIP, t_Subnet, u16_Port);

                    if (FA_PROTOCOL_Check_ForResponseBit(e_InterfaceType)) {
                        FA_PROTOCOL_Send_Ack(e_InterfaceType);
                    }
                } else if (FA_PROTOCOL_Check_ForResponseBit(e_InterfaceType)) {
                    FA_PROTOCOL_Send_Nack(e_InterfaceType);
                }
            }
            break;
            //        case Set_Custom:
            //            break;
            //*********************************
            //Get Motor Info
            //*********************************
            //        case Get_Motor_ID:
            //            break;
            //        case Get_Motor_Label:
            //            break;
            //        case Get_Motor_Direction:
            //            break;
            //        case Get_Motor_Limits:
            //            break;
            //        case Get_Motor_Speed:
            //            break;
            //        case Get_Motor_Group_ID:
            //            break;
            //        case Get_Motor_Internal_Position:
            //            break;
            //        case Get_Motor_Current_Position:
            //            break;
            //*********************************
            //Get Keypad Info
            //*********************************
        case Get_Keypad_ID:
            if (FA_PROTOCOL_Check_For_DeviceID_Or_Broadcast(e_InterfaceType) && FA_PROTOCOL_Check_ForResponseBit(e_InterfaceType) && !bSignOnOnce) {
                FA_PROTOCOL_Send_Message(e_InterfaceType, Post_Keypad_ID, false, MSG_NO_PAYLOAD, false, NULL, NULL);
                bSignOnOnce = true;
            }
            break;
        case Get_Keypad_Label:
            if (FA_PROTOCOL_Check_For_DeviceID(e_InterfaceType) && FA_PROTOCOL_Check_ForResponseBit(e_InterfaceType)) {
                uint8_t pLabel[LABEL_LENGTH];

                FA_PROTOCOL_COMMON_Get_Label(pLabel);
                FA_PROTOCOL_Send_Message(e_InterfaceType, Post_Keypad_Label, false, MSG_NO_PAYLOAD + LABEL_LENGTH, false, NULL, pLabel);
            }
            break;
        case Get_Keypad_CMD:
            if (FA_PROTOCOL_Check_For_DeviceID(e_InterfaceType) && FA_PROTOCOL_Check_ForResponseBit(e_InterfaceType)) {
                uint8_t cmdIndex = 0;
                uint8_t dataBuffer[CMD_LENGTH];

                dataBuffer[0] = u8_CurrentRx->Data[0];
                cmdIndex = u8_CurrentRx->Data[0] - 1;

                if (cmdIndex < FA_MAX_BUTTONS) {
                    dataBuffer[1] = KeyPadCmds[cmdIndex].u8_OnPress_Command;
                    dataBuffer[2] = KeyPadCmds[cmdIndex].u8_OnPress_Target_Value_L;
                    dataBuffer[3] = KeyPadCmds[cmdIndex].u8_OnPress_Target_Value_H;

                    dataBuffer[4] = KeyPadCmds[cmdIndex].u8_OnHold_Command;
                    dataBuffer[5] = KeyPadCmds[cmdIndex].u8_OnHold_Target_Value_L;
                    dataBuffer[6] = KeyPadCmds[cmdIndex].u8_OnHold_Target_Value_H;

                    dataBuffer[7] = KeyPadCmds[cmdIndex].u8_OnRelease_Command;
                    dataBuffer[8] = KeyPadCmds[cmdIndex].u8_OnRelease_Target_Value_L;
                    dataBuffer[9] = KeyPadCmds[cmdIndex].u8_OnRelease_Target_Value_H;

                    dataBuffer[10] = KeyPadCmds[cmdIndex].u8_Target_ID_1;
                    dataBuffer[11] = KeyPadCmds[cmdIndex].u8_Target_ID_2;
                    dataBuffer[12] = KeyPadCmds[cmdIndex].u8_Target_ID_3;

                    FA_PROTOCOL_Send_Message(e_InterfaceType, Post_Keypad_CMD, false, MSG_NO_PAYLOAD + CMD_LENGTH, false, NULL, dataBuffer);
                } else {
                    FA_PROTOCOL_Send_Nack(e_InterfaceType);
                }

            }
            break;
            //*********************************
            //Get General Info
            //*********************************
            //        case Get_Software_Version:
            //            if (FA_PROTOCOL_Check_for_DeviceID(e_InterfaceType)){
            //               if(FA_PROTOCOL_Check_ForResponseBit(e_InterfaceType)){
            //                   todo getsoft(&tNextTxPacket);
            //                   tNextTxPacket.data[0] = Post_Software_Version;
            //                   FA_PROTOCOL_Build_TargetedResponse(e_InterfaceType, &tNextTxPacket);
            //                }
            //            }
            //            break;
        case Get_MAC_Adrs:
            if (FA_PROTOCOL_Check_For_DeviceID(e_InterfaceType)) {
                if (FA_PROTOCOL_Check_ForResponseBit(e_InterfaceType)) {
                    uint8_t mac[MAC_LENGTH];
                    FA_PROTOCOL_COMMON_Get_MAC(mac);

                    FA_PROTOCOL_Send_Message(e_InterfaceType, Post_MAC_Adrs, false, MSG_NO_PAYLOAD + MAC_LENGTH, false, NULL, mac);
                }
            }
            break;
        case Get_IP_Adrs:
            if (FA_PROTOCOL_Check_For_DeviceID(e_InterfaceType)) {
                if (FA_PROTOCOL_Check_ForResponseBit(e_InterfaceType)) {
                    uint8_t u8_IP[IP_LENGTH];
                    bool b_DHCP_Enabled;
                    IPV4_ADDR t_NewIP;
                    SUBNET_MASK t_Subnet;
                    uint16_t u16_Port;

                    FA_PROTOCOL_COMMON_Get_IPv4_Address(&b_DHCP_Enabled, &t_NewIP, &t_Subnet, &u16_Port);

                    u8_IP[0] = b_DHCP_Enabled;
                    memcpy(u8_IP + 1, &u16_Port, sizeof (u16_Port));
                    memcpy(u8_IP + 3, &t_NewIP, sizeof (t_NewIP));
                    memcpy(u8_IP + 7, &t_Subnet, sizeof (t_Subnet));

                    FA_PROTOCOL_Send_Message(e_InterfaceType, Post_IP_Adrs, false, MSG_NO_PAYLOAD + IP_LENGTH, false, NULL, u8_IP);
                }
            }
            break;
            //        case Get_Custom:
            //            break;
            //*********************************
            //Control
            //*********************************
            //        case CMD_Stop:
            //            break;
            //        case CMD_Move:
            //            break;
            //        case CMD_Go_To_Internal_Pos:
            //            break;
            //        case CMD_Next_Internal_Pos:
            //            break;
            //        case CMD_Jog_mm:
            //            break;
            //        case CMD_Jog_Pulse:
            //            break;
            //        case CMD_Go_To_Percent:
            //            break;
            //        case CMD_Visual_Feedback:
            //            break;
            //        case CMD_Move_Toggle:
            //            break;
            //        case CMD_Custom_Control:
            //            break;
            //*********************************
            //Ack
            //*********************************
            //        case Ack:
            //            break;
            //*********************************
            //Nack
            //*********************************
            //        case Nack:
            //            break;
            //*********************************
            //Post Motor Commands
            //*********************************
            //        case Post_Motor_ID:
            //            break;
            //        case Post_Motor_Label:
            //            break;
            //        case Post_Motor_Direction:
            //            break;
            //        case Post_Motor_Limits:
            //            break;
            //        case Post_Motor_Speeds:
            //            break;
            //        case Post_Motor_Group_ID:
            //            break;
            //        case Post_Motor_Internal_Positions:
            //            break;
            //        case Post_Motor_Current_Position:
            //            break;
            //*********************************
            //Post Keypad Commands
            //*********************************
            //        case Post_Keypad_ID:
            //            break;
            //        case Post_Keypad_Label:
            //            break;
            //        case Post_Keypad_CMD:
            //            break;
            //*********************************
            //Post General Commands
            //*********************************
            //        case Post_Software_Version:
            //            break;
            //        case Post_MAC_Adrs:
            //            break;
            //        case Post_IP_Adrs:
            //            break;
            //        case Post_Custom:
            //            break;
        default:
            if (FA_PROTOCOL_Check_For_DeviceID(e_InterfaceType) && FA_PROTOCOL_Check_ForResponseBit(e_InterfaceType)) {
                FA_PROTOCOL_Send_Nack(e_InterfaceType);
            }
            break;
    }
}

void FA_PROTOCOL_Keypad_Handle_CMD(uint8_t CurrentCMD, t_IR_Button_State Press_Type) {
    bool TempToggle;

    if ((CurrentCMD >= 0)&&(CurrentCMD < FA_MAX_BUTTONS)) {
        //If Toggle
        if ((Press_Type == On_Press) && (KeyPadCmds[CurrentCMD].u8_OnPress_Command == CMD_Move_Toggle)) {
            TempToggle = FA_EEPROM_read_8(EEPROM_KEYPAD_ToggleState + CurrentCMD);
            if (TempToggle == true) {
                Press_Type = On_Hold;
                FA_EEPROM_write_8(EEPROM_KEYPAD_ToggleState + CurrentCMD, false);
            } else {
                Press_Type = On_Release;
                FA_EEPROM_write_8(EEPROM_KEYPAD_ToggleState + CurrentCMD, true);
            }
        }

        t_USART_ID target;
        uint8_t dataBuffer[2] = {0};

        target.u8_USART_ID_1 = KeyPadCmds[CurrentCMD].u8_Target_ID_1;
        target.u8_USART_ID_2 = KeyPadCmds[CurrentCMD].u8_Target_ID_2;
        target.u8_USART_ID_3 = KeyPadCmds[CurrentCMD].u8_Target_ID_3;

        switch (Press_Type) {
            case On_Press:
                dataBuffer[0] = KeyPadCmds[CurrentCMD].u8_OnPress_Target_Value_L;
                dataBuffer[1] = KeyPadCmds[CurrentCMD].u8_OnPress_Target_Value_H;
                FA_PROTOCOL_Send_Message(Interface_485, KeyPadCmds[CurrentCMD].u8_OnPress_Command, false, MSG_NO_PAYLOAD + MOTOR_MOVE_CMD_DATA_LENGTH, false, &target, dataBuffer);
                break;
            case On_Hold:
                dataBuffer[0] = KeyPadCmds[CurrentCMD].u8_OnHold_Target_Value_L;
                dataBuffer[1] = KeyPadCmds[CurrentCMD].u8_OnHold_Target_Value_H;
                FA_PROTOCOL_Send_Message(Interface_485, KeyPadCmds[CurrentCMD].u8_OnHold_Command, false, MSG_NO_PAYLOAD + MOTOR_MOVE_CMD_DATA_LENGTH, false, &target, dataBuffer);
                break;
            case On_Release:
                dataBuffer[0] = KeyPadCmds[CurrentCMD].u8_OnRelease_Target_Value_L;
                dataBuffer[1] = KeyPadCmds[CurrentCMD].u8_OnRelease_Target_Value_H;
                FA_PROTOCOL_Send_Message(Interface_485, KeyPadCmds[CurrentCMD].u8_OnRelease_Command, false, MSG_NO_PAYLOAD + MOTOR_MOVE_CMD_DATA_LENGTH, false, &target, dataBuffer);
                break;
            default:
                break;
        }
    }
}

/******************************************************************************
external Function:
FA_SDN_Keypad_Save_CMDs
Remarks: the keypad can have upto 100 cmds , this is called after inactive timeout

 */
void FA_PROTOCOL_Keypad_Save_CMDs(void) {
    uint32_t u32ptr_memory_pointer;

    if (bSaveKeypadCMD_Master) {
        bSaveKeypadCMD_Master = false;

        for (uint8_t u8_Button = 0; u8_Button < FA_MAX_BUTTONS; u8_Button++) {
            if (KeypadMemoryChanged[u8_Button] == true) {
                KeypadMemoryChanged[u8_Button] = false;
                u32ptr_memory_pointer = EEPROM_KEYPAD_COMMANDS + (u8_Button * sizeof (t_FA_PROTOCOL_KEYPAD_MessageStructure));
                FA_EEPROM_write_block(u32ptr_memory_pointer, (uint8_t*) & KeyPadCmds[u8_Button], sizeof (t_FA_PROTOCOL_KEYPAD_MessageStructure));
            }
        }
    }
}

/******************************************************************************
external Function:
FA_SDN_Keypad_Save_CMDs
Remarks: temp values need to persist until inactive timeout,
 * this is called to remove temp values after timeout

 */
void FA_PROTOCOL_Keypad_InactiveTimeOut(void) {
    bSignOnOnce = false;
    FA_PROTOCOL_Keypad_Save_CMDs();
}
