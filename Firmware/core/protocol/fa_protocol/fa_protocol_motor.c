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
#include "fa_protocol_motor.h"


// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************


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

// Flag which controls when to not respond to a certain message.
// The flag can be set, and it will be cleared after the inactive timeout
// timer has finished.
static bool bSignOnOnce = false;

static t_MotorStructure Motor_Settings;

static t_USART_ID Motor_Group_ID[16];
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

void FA_PROTOCOL_MOTOR_INITIALIZE(void) {

}

/******************************************************************************
  External Function:
    FA_SDN_MOTOR_reset_eeprom

  Remarks: reset the MOTOR specific eeprom

 */
void FA_PROTOCOL_Motor_reset_eeprom(void) {

}

/******************************************************************************
  External Function:
    FA_PROTOCOL_MOTOR_INITIALIZE

  Remarks: initialize MOTOR specific protocol

 */
void FA_PROTOCOL_Motor_INITIALIZE(void) {

}

void FA_PROTOCOL_Handle_Motor_Message(t_InterfaceType e_InterfaceType, t_fa_protocol_message * u8_CurrentRx) {
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
        case Set_Motor_ID:
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
        case Set_Motor_Label:
            if (FA_PROTOCOL_Check_For_DeviceID(e_InterfaceType)) {
                // Check that the Set Motor Label message contains 16 bytes in the data section of the message.
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
        case Set_Motor_Direction:
            if (FA_PROTOCOL_Check_For_DeviceID(e_InterfaceType)) {
                //set the movement direction reversed = 0 / false
                if ((u8_CurrentRx->Length - MSG_NO_PAYLOAD) == Motor_Direction_length) {
                    Motor_Settings.b_Direction = u8_CurrentRx->Data[0];
                    if (FA_PROTOCOL_Check_ForResponseBit(e_InterfaceType)) {
                        FA_PROTOCOL_Send_Ack(e_InterfaceType);
                    }
                } else if (FA_PROTOCOL_Check_ForResponseBit(e_InterfaceType)) {
                    FA_PROTOCOL_Send_Nack(e_InterfaceType);
                }
            }
            break;
        case Set_Motor_Limits:
            if (FA_PROTOCOL_Check_For_DeviceID(e_InterfaceType)) {
                //set motor Limits
                if ((u8_CurrentRx->Length - MSG_NO_PAYLOAD) == Motor_Limits_length) {
                    Motor_Settings.Up_Limit = ((u8_CurrentRx->Data[0] << 8)&0xFF00) + (u8_CurrentRx->Data[1]&0x00FF);
                    Motor_Settings.Down_Limit = ((u8_CurrentRx->Data[2] << 8)&0xFF00) + (u8_CurrentRx->Data[3]&0x00FF);
                    if (FA_PROTOCOL_Check_ForResponseBit(e_InterfaceType)) {
                        FA_PROTOCOL_Send_Ack(e_InterfaceType);
                    }
                } else if (FA_PROTOCOL_Check_ForResponseBit(e_InterfaceType)) {
                    FA_PROTOCOL_Send_Nack(e_InterfaceType);
                }
            }
            break;
        case Set_Motor_Speed:
            if (FA_PROTOCOL_Check_For_DeviceID(e_InterfaceType)) {
                //set motor Limits
                if ((u8_CurrentRx->Length - MSG_NO_PAYLOAD) == Motor_Speed_length) {
                    Motor_Settings.u8_Up_Speed = ((u8_CurrentRx->Data[0] << 8)&0xFF00) + (u8_CurrentRx->Data[1]&0x00FF);
                    Motor_Settings.u8_Down_Speed = ((u8_CurrentRx->Data[2] << 8)&0xFF00) + (u8_CurrentRx->Data[3]&0x00FF);
                    if (FA_PROTOCOL_Check_ForResponseBit(e_InterfaceType)) {
                        FA_PROTOCOL_Send_Ack(e_InterfaceType);
                    }
                } else if (FA_PROTOCOL_Check_ForResponseBit(e_InterfaceType)) {
                    FA_PROTOCOL_Send_Nack(e_InterfaceType);
                }
            }
            break;
        case Set_Motor_Group_ID:
            if (FA_PROTOCOL_Check_For_DeviceID(e_InterfaceType)) {
                //set motor Limits
                if ((u8_CurrentRx->Length - MSG_NO_PAYLOAD) == (DEVICE_ID_LENGTH + 1)) {

                    Motor_Group_ID[u8_CurrentRx->Data[0]].u8_USART_ID_1 = u8_CurrentRx->Data[1];
                    Motor_Group_ID[u8_CurrentRx->Data[0]].u8_USART_ID_2 = u8_CurrentRx->Data[2];
                    Motor_Group_ID[u8_CurrentRx->Data[0]].u8_USART_ID_3 = u8_CurrentRx->Data[3];

                    if (FA_PROTOCOL_Check_ForResponseBit(e_InterfaceType)) {
                        FA_PROTOCOL_Send_Ack(e_InterfaceType);
                    }
                } else if (FA_PROTOCOL_Check_ForResponseBit(e_InterfaceType)) {
                    FA_PROTOCOL_Send_Nack(e_InterfaceType);
                }
            }
            break;
        case Set_Motor_Internal_Position:
            break;
        case Set_Current_Position:
            break;
            //*********************************
            //Set Keypad Info
            //*********************************
            // case Set_Keypad_ID:
            //     break;
            // case Set_Keypad_Label:
            //     break;
            // case Set_Keypad_CMD:
            //     break;
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
        case Get_Motor_ID:
            if (FA_PROTOCOL_Check_ForResponseBit(e_InterfaceType) && !bSignOnOnce) {
                FA_PROTOCOL_Send_Message(e_InterfaceType, Post_Motor_ID, false, MSG_NO_PAYLOAD, false, NULL, NULL);
                //                bSignOnOnce = true;
            }
            break;
        case Get_Motor_Label:
            if (FA_PROTOCOL_Check_For_DeviceID(e_InterfaceType) && FA_PROTOCOL_Check_ForResponseBit(e_InterfaceType)) {
                uint8_t pLabel[LABEL_LENGTH];

                FA_PROTOCOL_COMMON_Get_Label(pLabel);
                FA_PROTOCOL_Send_Message(e_InterfaceType, Post_Motor_Label, false, MSG_NO_PAYLOAD + LABEL_LENGTH, false, NULL, pLabel);
            }
            break;
        case Get_Motor_Direction:
            break;
        case Get_Motor_Limits:
            break;
        case Get_Motor_Speed:
            break;
        case Get_Motor_Group_ID:
            break;
        case Get_Motor_Internal_Position:
            break;
        case Get_Motor_Current_Position:
            break;
            //*********************************
            //Get Keypad Info
            //*********************************
            // case Get_Keypad_ID:
            //     break;
            // case Get_Keypad_Label:
            //     break;
            // case Get_Keypad_CMD:
            //     break;
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
        case CMD_Stop:
            break;
        case CMD_Move:
            break;
        case CMD_Go_To_Internal_Pos:
            break;
        case CMD_Next_Internal_Pos:
            break;
        case CMD_Jog_Up_mm:
            break;
        case CMD_Jog_Down_mm:
            break;
        case CMD_Jog_Up_Pulse:
            break;
        case CMD_Jog_Down_Pulse:
            break;
        case CMD_Go_To_Percent:
            break;
        case CMD_Visual_Feedback:
            break;
        case CMD_Move_Toggle:
            break;
        case CMD_Custom_Control:
            break;
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

/******************************************************************************
external Function:
FA_PROTOCOL_Motor_InactiveTimeOut
Remarks: temp values need to persist until inactive timeout,
 * this is called to remove temp values after timeout

 */
void FA_PROTOCOL_Motor_InactiveTimeOut(void) {
    bSignOnOnce = false;
}