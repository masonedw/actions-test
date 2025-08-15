/*******************************************************************************
  Future Automation Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    fa_sdn_keypad.c

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
#include "fa_sdn_keypad.h"
#include "fa_eeprom_map.h"
#include "fa_sdn_cmd_list.h"
#include "fa_tcpip.h"
#include "protocol_common.h"


// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define SDN_POST_KEYPAD_FIRMWARE_LENGTH 0xD
#define SDN_FLOW_KEYPAD_CMDS1_LENGTH 0xC
#define SDN_FLOW_KEYPAD_CMDS3_LENGTH 0xB
#define SDN_POST_KEYPAD_CMDS_LENGTH 0x21
#define SDN_POST_KEYPAD_GROUP_LENGTH 0x23
#define SDN_POST10_KEYPAD_GROUP_LENGTH 0x2A
#define SDN_POST_IPV4_ADDRESS_LENGTH 0x16
#define SDN_POST_LABEL_LENGTH 0x1B
#define SDN_MOTOR_MOVEMENT_LENGTH 0xF

#define SDN_TARGET_ALL_MOTORS 0x1
#define SDN_TARGET_SPECIFIC_MOTOR 0x2
#define SDN_TARGET_SPECIFIC_GROUP 0x80



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
#define FA_MAX_BUTTONS 100
#elif  FRAMEWORK_PCB_0120
#define FA_MAX_BUTTONS 12
#elif  FRAMEWORK_PCB_0123
#define FA_MAX_BUTTONS 100
#else
#define FA_MAX_BUTTONS 8
#endif

static t_SDN_KEYPAD_MessageStructure KeyPadCmds[FA_MAX_BUTTONS];
static bool KeyPadMemoryChanged[FA_MAX_BUTTONS] = {0};
static bool bSignOnOnce = false;
static bool bSaveKeypadCMD_Master = false;
// *****************************************************************************
// *****************************************************************************
// Section: Forward references
// *****************************************************************************
// *****************************************************************************


void Get_CMD_Conversion(uint8_t Current_CMD, uint8_t* u8_Message_Type, uint8_t* u8_Data);


// *****************************************************************************
// *****************************************************************************
// Section: fa sdn Functions
// *****************************************************************************
// *****************************************************************************

/******************************************************************************
  External Function:
    FA_SDN_KEYPAD_reset_eeprom

  Remarks: reset the keypad specific eeprom

 */
void FA_SDN_KEYPAD_reset_eeprom(void) {
    FA_EEPROM_fill_block(EEPROM_KEYPAD_COMMANDS, 0xFF, 1500);
    FA_EEPROM_fill_block(EEPROM_KEYPAD_ToggleState, 0x00, 100);
}

/******************************************************************************
  External Function:
    FA_SDN_KEYPAD_INITIALIZE

  Remarks: initialise the keypad buttons

 */
void FA_SDN_KEYPAD_INITIALIZE(void) {
    FA_EEPROM_read_block(EEPROM_KEYPAD_COMMANDS, (uint8_t*) KeyPadCmds, sizeof (t_SDN_KEYPAD_MessageStructure) * FA_MAX_BUTTONS);
}

/******************************************************************************
external Function:
FA_SDN_Handle_Keypad_Message
  Remarks: all keypad specific cmds

 */
void FA_SDN_Handle_Keypad_Message(t_InterfaceType e_InterfaceType, t_SDN_message* u16_CurrentRx) {
    uint8_t u8_For_Loop_Counter = 0;

    switch (u16_CurrentRx->MessageType) {
        case Get_Keypad_Firmawre:
            if (FA_SDN_Check_ForResponseBit(e_InterfaceType)) {
                if (bSignOnOnce == false) {
                    uint8_t u8_firmware[2] = {0x01, 0x01};
                    FA_SDN_Send_Message(e_InterfaceType, Post_Keypad_Firmawre, false, SDN_POST_KEYPAD_FIRMWARE_LENGTH, NULL, NULL, u8_firmware);
                }
            }
            break;
        case Flow_Programming_Keypad:
            if (FA_SDN_Check_ForResponseBit(e_InterfaceType)) {
                FA_SDN_Send_Ack(e_InterfaceType);
            }
            break;
        case Set_Keypad_ID:
            if (FA_SDN_Check_For_DeviceID(e_InterfaceType)) {
                if (FA_PROTOCOL_COMMON_Set_Device_ID((uint8_t*) u16_CurrentRx->Data)) {
                    if (FA_SDN_Check_ForResponseBit(e_InterfaceType)) {
                        FA_SDN_Send_Ack(e_InterfaceType);
                    }
                } else if (FA_SDN_Check_ForResponseBit(e_InterfaceType)) {
                    FA_SDN_Send_Nack(e_InterfaceType, SDN_ERROR_DATA_OUT_OF_RANGE);
                }
            }
            break;
        case Set_Keypad_CMDs:
            if (FA_SDN_Check_For_DeviceID(e_InterfaceType)) {
                uint8_t u8_cmdIndex;

                u8_cmdIndex = u16_CurrentRx->Data[0] - 1;
                if (u8_cmdIndex < FA_MAX_BUTTONS) {
                    KeyPadCmds[u8_cmdIndex].u8_OnPress_Command = u16_CurrentRx->Data[1];
                    KeyPadCmds[u8_cmdIndex].u8_OnPress_Target_Value_H = u16_CurrentRx->Data[2];
                    KeyPadCmds[u8_cmdIndex].u8_OnPress_Target_Value_L = u16_CurrentRx->Data[3];
                    KeyPadCmds[u8_cmdIndex].u8_OnPress_Targeting = u16_CurrentRx->Data[4];
                    KeyPadCmds[u8_cmdIndex].u8_OnHold_Command = u16_CurrentRx->Data[8];
                    KeyPadCmds[u8_cmdIndex].u8_OnHold_Target_Value_H = u16_CurrentRx->Data[9];
                    KeyPadCmds[u8_cmdIndex].u8_OnHold_Target_Value_L = u16_CurrentRx->Data[10];
                    KeyPadCmds[u8_cmdIndex].u8_OnHold_Targeting = u16_CurrentRx->Data[11];
                    KeyPadCmds[u8_cmdIndex].u8_OnRelease_Command = u16_CurrentRx->Data[15];
                    KeyPadCmds[u8_cmdIndex].u8_OnRelease_Target_Value_H = u16_CurrentRx->Data[16];
                    KeyPadCmds[u8_cmdIndex].u8_OnRelease_Target_Value_L = u16_CurrentRx->Data[17];
                    KeyPadCmds[u8_cmdIndex].u8_OnRelease_Targeting = u16_CurrentRx->Data[18];
                    if (KeyPadCmds[u8_cmdIndex].u8_OnPress_Targeting == 0x02 || KeyPadCmds[u8_cmdIndex].u8_OnPress_Targeting == 0x01) {
                        KeyPadCmds[u8_cmdIndex].u8_Target_ID_3 = u16_CurrentRx->Data[5];
                        KeyPadCmds[u8_cmdIndex].u8_Target_ID_2 = u16_CurrentRx->Data[6];
                        KeyPadCmds[u8_cmdIndex].u8_Target_ID_1 = u16_CurrentRx->Data[7];
                    } else if (KeyPadCmds[u8_cmdIndex].u8_OnHold_Targeting == 0x02 || KeyPadCmds[u8_cmdIndex].u8_OnHold_Targeting == 0x01) {
                        KeyPadCmds[u8_cmdIndex].u8_Target_ID_3 = u16_CurrentRx->Data[12];
                        KeyPadCmds[u8_cmdIndex].u8_Target_ID_2 = u16_CurrentRx->Data[13];
                        KeyPadCmds[u8_cmdIndex].u8_Target_ID_1 = u16_CurrentRx->Data[14];
                    } else if (KeyPadCmds[u8_cmdIndex].u8_OnRelease_Targeting == 0x02 || KeyPadCmds[u8_cmdIndex].u8_OnRelease_Targeting == 0x01) {
                        KeyPadCmds[u8_cmdIndex].u8_Target_ID_3 = u16_CurrentRx->Data[19];
                        KeyPadCmds[u8_cmdIndex].u8_Target_ID_2 = u16_CurrentRx->Data[20];
                        KeyPadCmds[u8_cmdIndex].u8_Target_ID_1 = u16_CurrentRx->Data[21];
                    }
                    KeyPadMemoryChanged[u8_cmdIndex] = true;
                    bSaveKeypadCMD_Master = true;
                }
                if (FA_SDN_Check_ForResponseBit(e_InterfaceType)) {
                    FA_SDN_Send_Ack(e_InterfaceType);
                }
                break;
            }
        case Set_Keypad_Group:
            for (u8_For_Loop_Counter = 0; u8_For_Loop_Counter < 8; u8_For_Loop_Counter++) {
                if (KeyPadCmds[u8_For_Loop_Counter].u8_OnPress_Targeting == 0x80 || KeyPadCmds[u8_For_Loop_Counter].u8_OnHold_Targeting == 0x80
                        || KeyPadCmds[u8_For_Loop_Counter].u8_OnRelease_Targeting == 0x80) {
                    KeyPadCmds[u8_For_Loop_Counter].u8_Target_ID_3 = u16_CurrentRx->Data[0 + (u8_For_Loop_Counter * 3)];
                    KeyPadCmds[u8_For_Loop_Counter].u8_Target_ID_2 = u16_CurrentRx->Data[1 + (u8_For_Loop_Counter * 3)];
                    KeyPadCmds[u8_For_Loop_Counter].u8_Target_ID_1 = u16_CurrentRx->Data[2 + (u8_For_Loop_Counter * 3)];
                    KeyPadMemoryChanged[u8_For_Loop_Counter] = true;
                    bSaveKeypadCMD_Master = true;
                }
            }
            if (FA_SDN_Check_ForResponseBit(e_InterfaceType)) {
                FA_SDN_Send_Ack(e_InterfaceType);
            }
            break;
        case Set_10Keypad_Group:
            if (FA_SDN_Check_For_DeviceID(e_InterfaceType)) {
                uint8_t u8_cmdIndex;

                u8_cmdIndex = u16_CurrentRx->Data[0] - 1;
                if (u8_cmdIndex < FA_MAX_BUTTONS) {
                    for (u8_For_Loop_Counter = 0; u8_For_Loop_Counter < 10; u8_For_Loop_Counter++) {
                        if (KeyPadCmds[u8_cmdIndex + u8_For_Loop_Counter].u8_OnPress_Targeting == 0x80 || KeyPadCmds[u8_cmdIndex + u8_For_Loop_Counter].u8_OnHold_Targeting == 0x80
                                || KeyPadCmds[u8_cmdIndex + u8_For_Loop_Counter].u8_OnRelease_Targeting == 0x80) {
                            KeyPadCmds[u8_cmdIndex + u8_For_Loop_Counter].u8_Target_ID_3 = u16_CurrentRx->Data[1 + (u8_For_Loop_Counter * 3)];
                            KeyPadCmds[u8_cmdIndex + u8_For_Loop_Counter].u8_Target_ID_2 = u16_CurrentRx->Data[2 + (u8_For_Loop_Counter * 3)];
                            KeyPadCmds[u8_cmdIndex + u8_For_Loop_Counter].u8_Target_ID_1 = u16_CurrentRx->Data[3 + (u8_For_Loop_Counter * 3)];
                            KeyPadMemoryChanged[u8_cmdIndex + u8_For_Loop_Counter] = true;
                            bSaveKeypadCMD_Master = true;
                        }
                    }
                }
                if (FA_SDN_Check_ForResponseBit(e_InterfaceType)) {
                    FA_SDN_Send_Ack(e_InterfaceType);
                }
            }
            break;
        case Flow_Get_Keypad_CMDs:
            if (FA_SDN_Check_ForResponseBit(e_InterfaceType)) {
                uint8_t u8_data[1] = {0x01};
                FA_SDN_Send_Message(e_InterfaceType, ACK_Keypad_CMDs, false, SDN_FLOW_KEYPAD_CMDS1_LENGTH, NULL, NULL, u8_data);
            }
            break;
        case Flow_Get_Keypad_CMDs_2:
            if (FA_SDN_Check_ForResponseBit(e_InterfaceType)) {
                FA_SDN_Send_Ack(e_InterfaceType);
            }
            break;
        case Flow_Get_Keypad_CMDs_3:
            if (FA_SDN_Check_ForResponseBit(e_InterfaceType)) {
                FA_SDN_Send_Message(e_InterfaceType, ACK_Keypad_CMDs_2, false, SDN_MSG_NO_PAYLOAD, NULL, NULL, NULL);
            }
            break;
        case Get_Keypad_CMDs:
            if (FA_SDN_Check_ForResponseBit(e_InterfaceType)) {
                uint8_t cmdIndex;
                uint8_t u8_data[22];

                cmdIndex = (FA_SDN_GetRxData(e_InterfaceType, 9)) - 1;
                if (cmdIndex < FA_MAX_BUTTONS) {
                    u8_data[0] = cmdIndex + 1;
                    u8_data[1] = KeyPadCmds[cmdIndex].u8_OnPress_Command;
                    u8_data[2] = KeyPadCmds[cmdIndex].u8_OnPress_Target_Value_H;
                    u8_data[3] = KeyPadCmds[cmdIndex].u8_OnPress_Target_Value_L;
                    u8_data[4] = KeyPadCmds[cmdIndex].u8_OnPress_Targeting;
                    u8_data[5] = 0xFF;
                    u8_data[6] = 0xFF;
                    u8_data[7] = 0xFF;
                    u8_data[8] = KeyPadCmds[cmdIndex].u8_OnHold_Command;
                    u8_data[9] = KeyPadCmds[cmdIndex].u8_OnHold_Target_Value_H;
                    u8_data[10] = KeyPadCmds[cmdIndex].u8_OnHold_Target_Value_L;
                    u8_data[11] = KeyPadCmds[cmdIndex].u8_OnHold_Targeting;
                    u8_data[12] = 0xFF;
                    u8_data[13] = 0xFF;
                    u8_data[14] = 0xFF;
                    u8_data[15] = KeyPadCmds[cmdIndex].u8_OnRelease_Command;
                    u8_data[16] = KeyPadCmds[cmdIndex].u8_OnRelease_Target_Value_H;
                    u8_data[17] = KeyPadCmds[cmdIndex].u8_OnRelease_Target_Value_L;
                    u8_data[18] = KeyPadCmds[cmdIndex].u8_OnRelease_Targeting;
                    u8_data[19] = 0xFF;
                    u8_data[20] = 0xFF;
                    u8_data[21] = 0xFF;
                    if (KeyPadCmds[cmdIndex].u8_OnPress_Targeting == 0x02 || KeyPadCmds[cmdIndex].u8_OnPress_Targeting == 0x01) {
                        u8_data[5] = KeyPadCmds[cmdIndex].u8_Target_ID_3;
                        u8_data[6] = KeyPadCmds[cmdIndex].u8_Target_ID_2;
                        u8_data[7] = KeyPadCmds[cmdIndex].u8_Target_ID_1;
                    } else if (KeyPadCmds[cmdIndex].u8_OnHold_Targeting == 0x02 || KeyPadCmds[cmdIndex].u8_OnHold_Targeting == 0x01) {
                        u8_data[12] = KeyPadCmds[cmdIndex].u8_Target_ID_3;
                        u8_data[13] = KeyPadCmds[cmdIndex].u8_Target_ID_2;
                        u8_data[14] = KeyPadCmds[cmdIndex].u8_Target_ID_1;
                    } else if (KeyPadCmds[cmdIndex].u8_OnRelease_Targeting == 0x02 || KeyPadCmds[cmdIndex].u8_OnRelease_Targeting == 0x01) {
                        u8_data[19] = KeyPadCmds[cmdIndex].u8_Target_ID_3;
                        u8_data[20] = KeyPadCmds[cmdIndex].u8_Target_ID_2;
                        u8_data[21] = KeyPadCmds[cmdIndex].u8_Target_ID_1;
                    }
                    FA_SDN_Send_Message(e_InterfaceType, Post_Keypad_CMDs, false, SDN_POST_KEYPAD_CMDS_LENGTH, NULL, NULL, u8_data);
                }
            }
            break;
        case Get_Keypad_Group:
            if (FA_SDN_Check_ForResponseBit(e_InterfaceType)) {
                uint8_t u8_data[24];

                for (u8_For_Loop_Counter = 0; u8_For_Loop_Counter < 8; u8_For_Loop_Counter++) {
                    if (KeyPadCmds[u8_For_Loop_Counter].u8_OnPress_Targeting == 0x80 || KeyPadCmds[u8_For_Loop_Counter].u8_OnHold_Targeting == 0x80
                            || KeyPadCmds[u8_For_Loop_Counter].u8_OnRelease_Targeting == 0x80) {
                        u8_data[0 + (u8_For_Loop_Counter * 3)] = KeyPadCmds[u8_For_Loop_Counter].u8_Target_ID_3;
                        u8_data[1 + (u8_For_Loop_Counter * 3)] = KeyPadCmds[u8_For_Loop_Counter].u8_Target_ID_2;
                        u8_data[2 + (u8_For_Loop_Counter * 3)] = KeyPadCmds[u8_For_Loop_Counter].u8_Target_ID_1;
                    } else {
                        u8_data[0 + (u8_For_Loop_Counter * 3)] = 0xFF;
                        u8_data[1 + (u8_For_Loop_Counter * 3)] = 0xFF;
                        u8_data[2 + (u8_For_Loop_Counter * 3)] = 0xFF;
                    }
                }
                FA_SDN_Send_Message(e_InterfaceType, Post_Keypad_Group, false, SDN_POST_KEYPAD_GROUP_LENGTH, NULL, NULL, u8_data);
            }
            break;
        case Get_10Keypad_Group:
            if (FA_SDN_Check_ForResponseBit(e_InterfaceType)) {
                uint8_t cmdIndex;
                uint8_t u8_data[31];

                cmdIndex = u16_CurrentRx->Data[0] - 1;
                if (cmdIndex < FA_MAX_BUTTONS) {
                    u8_data[0] = cmdIndex + 1;

                    for (u8_For_Loop_Counter = 0; u8_For_Loop_Counter < 10; u8_For_Loop_Counter++) {
                        if (KeyPadCmds[u8_For_Loop_Counter + (cmdIndex * 10)].u8_OnPress_Targeting == 0x80 || KeyPadCmds[u8_For_Loop_Counter + (cmdIndex * 10)].u8_OnHold_Targeting == 0x80
                                || KeyPadCmds[u8_For_Loop_Counter + (cmdIndex * 10)].u8_OnRelease_Targeting == 0x80) {
                            u8_data[1 + (u8_For_Loop_Counter * 3)] = KeyPadCmds[u8_For_Loop_Counter + (cmdIndex * 10)].u8_Target_ID_3;
                            u8_data[2 + (u8_For_Loop_Counter * 3)] = KeyPadCmds[u8_For_Loop_Counter + (cmdIndex * 10)].u8_Target_ID_2;
                            u8_data[3 + (u8_For_Loop_Counter * 3)] = KeyPadCmds[u8_For_Loop_Counter + (cmdIndex * 10)].u8_Target_ID_1;
                        } else {
                            u8_data[1 + (u8_For_Loop_Counter * 3)] = 0xFF;
                            u8_data[2 + (u8_For_Loop_Counter * 3)] = 0xFF;
                            u8_data[3 + (u8_For_Loop_Counter * 3)] = 0xFF;
                        }
                    }
                    FA_SDN_Send_Message(e_InterfaceType, Post_10Keypad_Group, false, SDN_POST10_KEYPAD_GROUP_LENGTH, NULL, NULL, u8_data);
                }
            }
            break;
        case Flow_IPv4_Address:
            if (FA_SDN_Check_ForResponseBit(e_InterfaceType)) {
                FA_SDN_Send_Ack(e_InterfaceType);
            }
            break;
        case Get_IPv4_Address:
            if (FA_SDN_Check_ForResponseBit(e_InterfaceType)) {
                if (FA_SDN_Check_For_DeviceID(e_InterfaceType)) {
                    uint8_t u8_data[11];
                    IPV4_ADDR t_IP;
                    SUBNET_MASK t_Mask;
                    t_fa_port t_Port;
                    bool b_DCHP;

                    FA_PROTOCOL_COMMON_Get_IPv4_Address(&b_DCHP, &t_IP, &t_Mask, &t_Port.port);

                    memcpy(u8_data, &t_IP, sizeof (t_IP));
                    memcpy(u8_data + 4, &t_Mask, sizeof (t_Mask));
                    u8_data[8] = b_DCHP;
                    memcpy(u8_data + 9, &t_Port, sizeof (t_Port));

                    FA_SDN_Send_Message(e_InterfaceType, Post_IPv4_Address, false, SDN_POST_IPV4_ADDRESS_LENGTH, NULL, NULL, u8_data);
                }
            }
            break;
            //tcpip
        case Set_IPv4_Address:
            if (FA_SDN_Check_For_DeviceID(e_InterfaceType)) {
                IPV4_ADDR t_IP;
                SUBNET_MASK t_Mask;
                t_fa_port t_Port;
                bool b_DCHP;

                if ((u16_CurrentRx->Data[9] != 0) && (u16_CurrentRx->Data[10] != 0)) {
                    t_Port.server_port_lsb = u16_CurrentRx->Data[9];
                    t_Port.server_port_msb = u16_CurrentRx->Data[10];
                }
                if (u16_CurrentRx->Data[8] == 0) {
                    // Extract IP address and mask
                    t_IP.v[0] = u16_CurrentRx->Data[0];
                    t_IP.v[1] = u16_CurrentRx->Data[1];
                    t_IP.v[2] = u16_CurrentRx->Data[2];
                    t_IP.v[3] = u16_CurrentRx->Data[3];
                    t_Mask.v[0] = u16_CurrentRx->Data[4];
                    t_Mask.v[1] = u16_CurrentRx->Data[5];
                    t_Mask.v[2] = u16_CurrentRx->Data[6];
                    t_Mask.v[3] = u16_CurrentRx->Data[7];

                    b_DCHP = false;
                } else {
                    b_DCHP = true;
                }

                FA_PROTOCOL_COMMON_Set_IPv4_Address(b_DCHP, t_IP, t_Mask, t_Port.port);

                if (FA_SDN_Check_ForResponseBit(e_InterfaceType)) {
                    FA_SDN_Send_Ack(e_InterfaceType);
                }
            }
            break;
        case Set_SDN_Label:
            if (FA_SDN_Check_For_DeviceID(e_InterfaceType)) {
                FA_PROTOCOL_COMMON_Set_Label(u16_CurrentRx->Data);
                if (FA_SDN_Check_ForResponseBit(e_InterfaceType)) {
                    FA_SDN_Send_Ack(e_InterfaceType);
                }
            }
            break;
        case Get_SDN_Label:
            if (FA_SDN_Check_ForResponseBit(e_InterfaceType)) {
                if (FA_SDN_Check_For_DeviceID(e_InterfaceType)) {
                    uint8_t data[LABEL_LENGTH];

                    FA_PROTOCOL_COMMON_Get_Label(data);
                    FA_SDN_Send_Message(e_InterfaceType, Post_SDN_Label, false, SDN_POST_LABEL_LENGTH, NULL, NULL, data);

                    bSignOnOnce = true;
                }
            }
            break;
        case Set_Server_IPs: if (FA_SDN_Check_For_DeviceID(e_InterfaceType)) {
                IPV4_ADDR addr;
                uint16_t port = 0;
                uint8_t listIndex = 0xFF; //0xFF used as invalid index.

                addr.v[0] = u16_CurrentRx->Data[4];
                addr.v[1] = u16_CurrentRx->Data[3];
                addr.v[2] = u16_CurrentRx->Data[2];
                addr.v[3] = u16_CurrentRx->Data[1];

                port = u16_CurrentRx->Data[9];
                port = ((u16_CurrentRx->Data[10] << 8) | port);

                listIndex = u16_CurrentRx->Data[0];
                if (listIndex > (NUM_CLIENTS - 1)) {
                    SYS_CONSOLE_PRINT("TCP Client: Cannot assign client connection data from table index %d. Only %d clients are supported.\r\n", listIndex, NUM_CLIENTS);
                } else {
                    FA_TCPIP_Set_Client_IP_Address(addr, port, listIndex);
                }

                FA_SDN_Send_Ack(e_InterfaceType);
            }
            break;
        case SDN_Factory_Reset:
            if (FA_SDN_Check_For_DeviceID(e_InterfaceType)) {
                SYS_RESET_SoftwareReset();
            }
            break;
        default:
            if (FA_SDN_Check_For_DeviceID(e_InterfaceType)) {
                if (FA_SDN_Check_ForResponseBit(e_InterfaceType)) {
                    FA_SDN_Send_Nack(e_InterfaceType, SDN_ERROR_UNKNOWN_MSG);
                }
            }
            break;
    }
}

/******************************************************************************
external Function:
FA_SDN_Keypad_Handle_CMD
  Remarks: the call to generate a keypad command

 */
void FA_SDN_Keypad_Handle_CMD(uint16_t CurrentCMD, t_IR_Button_State Press_Type) {
    uint8_t u8_data[50];
    uint8_t u8_message_type;
    t_USART_ID source;
    t_USART_ID dest;
    bool TempToggle;

    if ((CurrentCMD >= 0)&&(CurrentCMD < FA_MAX_BUTTONS)) {
        //If Toggle
        if ((Press_Type == On_Press) && (KeyPadCmds[CurrentCMD].u8_OnPress_Command == Move_Toggle)) {
            TempToggle = FA_EEPROM_read_8(EEPROM_KEYPAD_ToggleState + CurrentCMD);
            if (TempToggle == true) {
                Press_Type = On_Hold;
                FA_EEPROM_write_8(EEPROM_KEYPAD_ToggleState + CurrentCMD, false);
            } else {
                Press_Type = On_Release;
                FA_EEPROM_write_8(EEPROM_KEYPAD_ToggleState + CurrentCMD, true);
            }
        }

        switch (Press_Type) {
            case On_Press:
                Get_CMD_Conversion(KeyPadCmds[CurrentCMD].u8_OnPress_Command, &u8_message_type, u8_data);
                FA_SDN_Keypad_Get_Custom_Targeting(KeyPadCmds[CurrentCMD].u8_OnPress_Targeting, CurrentCMD, &source, &dest);
                u8_data[1] = KeyPadCmds[CurrentCMD].u8_OnPress_Target_Value_H;
                u8_data[2] = KeyPadCmds[CurrentCMD].u8_OnPress_Target_Value_L;
                // 4th byte always set to 0.
                u8_data[3] = 0;
                FA_SDN_Send_Message(Interface_485, u8_message_type, false, SDN_MOTOR_MOVEMENT_LENGTH, &source, &dest, u8_data);
                break;
            case On_Hold:
                Get_CMD_Conversion(KeyPadCmds[CurrentCMD].u8_OnHold_Command, &u8_message_type, u8_data);
                FA_SDN_Keypad_Get_Custom_Targeting(KeyPadCmds[CurrentCMD].u8_OnHold_Targeting, CurrentCMD, &source, &dest);
                u8_data[1] = KeyPadCmds[CurrentCMD].u8_OnHold_Target_Value_H;
                u8_data[2] = KeyPadCmds[CurrentCMD].u8_OnHold_Target_Value_L;
                // 4th byte always set to 0.
                u8_data[3] = 0;
                FA_SDN_Send_Message(Interface_485, u8_message_type, false, SDN_MOTOR_MOVEMENT_LENGTH, &source, &dest, u8_data);
                break;
            case On_Release:
                Get_CMD_Conversion(KeyPadCmds[CurrentCMD].u8_OnRelease_Command, &u8_message_type, u8_data);
                FA_SDN_Keypad_Get_Custom_Targeting(KeyPadCmds[CurrentCMD].u8_OnRelease_Targeting, CurrentCMD, &source, &dest);
                u8_data[1] = KeyPadCmds[CurrentCMD].u8_OnRelease_Target_Value_H;
                u8_data[2] = KeyPadCmds[CurrentCMD].u8_OnRelease_Target_Value_L;
                // 4th byte always set to 0.
                u8_data[3] = 0;
                FA_SDN_Send_Message(Interface_485, u8_message_type, false, SDN_MOTOR_MOVEMENT_LENGTH, &source, &dest, u8_data);
                break;
            default:
                break;
        }
    }
}

/******************************************************************************
external Function:
FA_SDN_Keypad_Get_Custom_Targeting
  Remarks: keypad message structure changes depending on what the target is,
 * this changes the target structure

 */
void FA_SDN_Keypad_Get_Custom_Targeting(uint8_t u8_MessageTarget, uint8_t CurrentCMD, t_USART_ID* source, t_USART_ID* dest) {
    uint8_t u8_DeviceID[DEVICE_ID_LENGTH];
    uint8_t u8_BroadcastID[DEVICE_ID_LENGTH];

    FA_PROTOCOL_COMMON_Get_Device_ID(u8_DeviceID);
    FA_PROTOCOL_COMMON_Get_Broadcast_ID(u8_BroadcastID);

    switch (u8_MessageTarget) {
            // Default to all motors. This should only happen when the
            // keypad hasn't been programmed and should target all motors for the stop command.
        default:
        case SDN_TARGET_ALL_MOTORS:
            source->u8_USART_ID_1 = u8_DeviceID[2];
            source->u8_USART_ID_2 = u8_DeviceID[1];
            source->u8_USART_ID_3 = u8_DeviceID[0];
            dest->u8_USART_ID_1 = u8_BroadcastID[2];
            dest->u8_USART_ID_2 = u8_BroadcastID[1];
            dest->u8_USART_ID_3 = u8_BroadcastID[0];
            break;
        case SDN_TARGET_SPECIFIC_MOTOR:
            source->u8_USART_ID_1 = u8_DeviceID[2];
            source->u8_USART_ID_2 = u8_DeviceID[1];
            source->u8_USART_ID_3 = u8_DeviceID[0];
            dest->u8_USART_ID_1 = KeyPadCmds[CurrentCMD].u8_Target_ID_3;
            dest->u8_USART_ID_2 = KeyPadCmds[CurrentCMD].u8_Target_ID_2;
            dest->u8_USART_ID_3 = KeyPadCmds[CurrentCMD].u8_Target_ID_1;
            break;
        case SDN_TARGET_SPECIFIC_GROUP:
            source->u8_USART_ID_1 = KeyPadCmds[CurrentCMD].u8_Target_ID_3;
            source->u8_USART_ID_2 = KeyPadCmds[CurrentCMD].u8_Target_ID_2;
            source->u8_USART_ID_3 = KeyPadCmds[CurrentCMD].u8_Target_ID_1;
            dest->u8_USART_ID_1 = 0x00;
            dest->u8_USART_ID_2 = 0x00;
            dest->u8_USART_ID_3 = 0x00;
            break;
    }
}

/******************************************************************************
external Function:
FA_SDN_Keypad_Send_Stop
  Remarks: Stop is a very specific message, it needs to be sent as fast as possible without violating the protocol
 * stop can be called from a number of different places, it will build the message, start the bus free timeout and send the message asap

 */
void FA_SDN_Keypad_Send_Stop(void) {
    uint8_t u8_data[50];
    uint8_t u8_message_type;
    t_USART_ID source;
    t_USART_ID dest;

    Get_CMD_Conversion(Set_Move_Stop, &u8_message_type, u8_data);
    FA_SDN_Keypad_Get_Custom_Targeting(0x01, 0, &source, &dest);

    FA_SDN_Send_Message(Interface_485, u8_message_type, false, SDN_MOTOR_MOVEMENT_LENGTH, &source, &dest, u8_data);

    SDN_Start_Timers(Interface_485);
}

/******************************************************************************
external Function:
FA_SDN_Keypad_Save_CMDs
 Remarks: the keypad can have upto 100 cmds , this is called after inactive timeout

 */
void FA_SDN_Keypad_Save_CMDs(void) {
    uint32_t u32ptr_memory_pointer;

    if (bSaveKeypadCMD_Master) {
        bSaveKeypadCMD_Master = false;
        for (uint8_t u8_Button = 0; u8_Button < FA_MAX_BUTTONS; u8_Button++) {
            if (KeyPadMemoryChanged[u8_Button] == true) {
                KeyPadMemoryChanged[u8_Button] = false;
                u32ptr_memory_pointer = EEPROM_KEYPAD_COMMANDS + (u8_Button * sizeof (t_SDN_KEYPAD_MessageStructure));
                FA_EEPROM_write_block(u32ptr_memory_pointer, (uint8_t*) & KeyPadCmds[u8_Button], (sizeof (t_SDN_KEYPAD_MessageStructure)));
            }
        }
    }
}

/******************************************************************************
internal Function:
FA_SDN_Keypad_Save_CMDs
 Remarks: each keypad cmd when programmed has a specific number,
 * when used it needs processing to a movement cmd for the target to understand

 */
void Get_CMD_Conversion(uint8_t Current_CMD, uint8_t* u8_Message_Type, uint8_t* u8_Data) {
    switch (Current_CMD) {
        case Set_Move_Up:
            *u8_Message_Type = Move_Three;
            u8_Data[0] = 0x01;
            break;
        case Set_Move_Down:
            *u8_Message_Type = Move_Three;
            u8_Data[0] = 0x00;
            break;
        case Set_Move_Go_To_IP:
            *u8_Message_Type = Move_Three;
            u8_Data[0] = 0x02;
            break;
        case Set_Move_Next_IP_Up:
            *u8_Message_Type = Move_Four;
            u8_Data[0] = 0x01;
            break;
        case Set_Move_Next_IP_Down:
            *u8_Message_Type = Move_Four;
            u8_Data[0] = 0x00;
            break;
        case Set_Move_Go_To_Pulse:
            *u8_Message_Type = Move_Three;
            u8_Data[0] = 0x03;
            break;
        case Set_Move_Jog_Up_mS:
            *u8_Message_Type = Move_Four;
            u8_Data[0] = 0x05;
            break;
        case Set_Move_Jog_Down_mS:
            *u8_Message_Type = Move_Four;
            u8_Data[0] = 0x04;
            break;
        case Set_Move_Jog_Up_Pulse:
            *u8_Message_Type = Move_Four;
            u8_Data[0] = 0x03;
            break;
        case Set_Move_Jog_Down_Puse:
            *u8_Message_Type = Move_Four;
            u8_Data[0] = 0x02;
            break;
        case Set_Move_Go_To_Percent:
            *u8_Message_Type = Move_Three;
            u8_Data[0] = 0x04;
            break;
            //Set IP
        case 0x25:
            *u8_Message_Type = Set_Motor_Internal_Position;
            u8_Data[0] = 0x01;
            break;
        case Set_Move_Stop:
        default:
            *u8_Message_Type = Move_Stop;
            u8_Data[0] = 0x00;

            break;
    }
}

/******************************************************************************
external Function:
FA_SDN_Keypad_Save_CMDs
 Remarks: temp values need to persist until inactive timeout,
 * this is called to remove temp values after timeout

 */
void FA_SDN_Keypad_InactiveTimeOut(void) {
    bSignOnOnce = false;
    FA_SDN_Keypad_Save_CMDs();
    FA_PROTOCOL_COMMON_InactiveTimeOut();
    FA_TCPIP_Write_Client_Connections();
}

