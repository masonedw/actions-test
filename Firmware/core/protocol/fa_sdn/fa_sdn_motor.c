/*******************************************************************************
  Future Automation Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    fa_sdn_motor.c

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
#include "fa_sdn_motor.h"
#include "fa_sdn_cmd_list.h"
#include "fa_global.h"
#include "system/reset/sys_reset.h"

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


static bool bSignOnOnce = false;
static uint8_t Motor_Group_IDs[3][16];
static uint16_t u16_Mech_Function;

//Save Flags 
bool bSave_InternalPos = true;
bool bSave_InternalPosPointer[16] = {false}; // The first element is false, and all others are also initialized to 0
bool bSave_GroupID = true;
bool bSave_GroupIDPointer[16] = {false}; // The first element is false, and all others are also initialized to 0.
bool bSaveLimits = false;
bool bSaveSpeeds = false;
bool bSaveOrientation = false;
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
    FA_MOTOR_reset_eeprom

  Remarks: clear the motor specific eeprom
 
 */
void FA_SDN_MOTOR_reset_eeprom(void) {
    FA_SEEPROM_fill_block(EEPROM_MOTOR_GROUP, 0, 48);
}

/******************************************************************************
  External Function:
    FA_SDN_MOTOR_INITIALIZE

  Remarks: initialise the motor specific ram
 
 */
void FA_SDN_MOTOR_INITIALIZE(void){
    for (uint8_t count = 0; count < 16; count++){
        Motor_Group_IDs[0][count] = FA_SEEPROM_read_8(EEPROM_MOTOR_GROUP+(count*3));
        Motor_Group_IDs[1][count] = FA_SEEPROM_read_8(EEPROM_MOTOR_GROUP+((count*3)+2));
        Motor_Group_IDs[2][count] = FA_SEEPROM_read_8(EEPROM_MOTOR_GROUP+((count*3)+4));   
    }
    
    u16_Mech_Function = FN_NULL;
}

/******************************************************************************
external Function:
FA_SDN_Handle_Motor_Message
 * 
  Remarks: all motor specific cmds
 
 */
void FA_SDN_Handle_Motor_Message(t_InterfaceType e_InterfaceType, uint8_t u8_CurrentRx){
   uint8_t u_NextTxData[50];
   uint8_t u_Target;
   uint8_t u_TempDeviceID[3];
   
    switch (u8_CurrentRx){
        case Move_Stop:             //stop
                                    if (FA_SDN_Check_ForResponseBit(e_InterfaceType)){
                                        FA_SDN_Build_Ack(e_InterfaceType);
                                    }
                                    u16_Mech_Function = FN_USER_STOP;
                break;
        case Move_Pulse_Down:       //pulse down
                                    if (FA_SDN_Check_ForResponseBit(e_InterfaceType)){
                                        FA_SDN_Build_Ack(e_InterfaceType);
                                    }
                break;
        case Move_Three:            switch (FA_SDN_GetRxData(e_InterfaceType, 9)){
                                        case 0x00://Down
                                            u16_Mech_Function = FN_IN;
                                            break;
                                        case 0x01://Up
                                            u16_Mech_Function = FN_OUT;
                                            break;
                                        case 0x02://Go to IP
                                            u16_Mech_Function = FN_PRESET;
                                            break;
                                        case 0x03://Go to Pulse
                                            break;
                                        case 0x04://Go to Percent
                                            break;
                                    }
                break;
        case Move_Four:             switch (FA_SDN_GetRxData(e_InterfaceType, 9)){
                                        case 0x00://Next IP Down
                                            break;
                                        case 0x01://Next IP UP
                                            break;
                                        case 0x02://Jog Down Pulse
                                            break;
                                        case 0x03://Jog Up Pulse
                                            break;
                                        case 0x04://Jog Down mS
                                            break;
                                        case 0x05://Jog Up mS
                                            break;
                                    }
                break;
        case Set_Motor_Limits:          if (FA_SDN_Check_for_DeviceID(e_InterfaceType)){
                                            //get number of pulses from FA_Mech
                                            if (FA_SDN_GetRxData(e_InterfaceType, 10) == true){
                                                FA_App_Set_Up_Limit();
                                            }
                                            else if (FA_SDN_GetRxData(e_InterfaceType, 10) == false){
                                                FA_App_Set_Down_Limit();
                                            }
                                            bSaveLimits = true;
                                           if (FA_SDN_Check_ForResponseBit(e_InterfaceType)){
                                                FA_SDN_Build_Ack(e_InterfaceType);
                                           }
                                        }
                break;
        case Get_Motor_Limits:          if (FA_SDN_Check_ForResponseBit(e_InterfaceType)){
                                            if (FA_SDN_Check_for_DeviceID(e_InterfaceType)){
                                                u_NextTxData[0] = Post_Motor_Limits;  
                                                u_NextTxData[1] = 0x0F;
                                                u_NextTxData[6] = FA_SDN_GetRxData(e_InterfaceType, 3);
                                                u_NextTxData[7] = FA_SDN_GetRxData(e_InterfaceType, 4);
                                                u_NextTxData[8] = FA_SDN_GetRxData(e_InterfaceType, 5);
                                                u_NextTxData[9] = FA_App_Get_Down_Limit() & 0x00FF;
                                                u_NextTxData[10] = ((FA_App_Get_Down_Limit() >> 8)&0x00FF);
                                                u_NextTxData[11] = FA_App_Get_Up_Limit() & 0x00FF;
                                                u_NextTxData[12] = ((FA_App_Get_Up_Limit() >> 8)&0x00FF);
                                                FA_SDN_Build_Message(e_InterfaceType, u_NextTxData);
                                            }
                                        }
                break;
        case Set_Motor_Orientation:     if (FA_SDN_Check_for_DeviceID(e_InterfaceType)){
                                            if (FA_App_SystemHas_Direction()){
                                                FA_App_Set_Direction(FA_SDN_GetRxData(e_InterfaceType, 9));
                                                bSaveOrientation = true;
                                                if (FA_SDN_Check_ForResponseBit(e_InterfaceType)){
                                                    FA_SDN_Build_Ack(e_InterfaceType);
                                                }
                                            }
                                            else {
                                                FA_SDN_Build_Nack(e_InterfaceType, 10);
                                            }    
                                        }
                break;
        case Get_Motor_Orientation:     if (FA_SDN_Check_ForResponseBit(e_InterfaceType)){
                                            if (FA_SDN_Check_for_DeviceID(e_InterfaceType)){
                                                if (FA_App_SystemHas_Direction()){
                                                    u_NextTxData[0] = Post_Motor_Orientation;  
                                                    u_NextTxData[1] = 0x0C;
                                                    u_NextTxData[6] = FA_SDN_GetRxData(e_InterfaceType, 3);
                                                    u_NextTxData[7] = FA_SDN_GetRxData(e_InterfaceType, 4);
                                                    u_NextTxData[8] = FA_SDN_GetRxData(e_InterfaceType, 5);
                                                    u_NextTxData[9] = !FA_App_Get_Direction();
                                                    FA_SDN_Build_Message(e_InterfaceType, u_NextTxData);
                                                }
                                                else {
                                                    FA_SDN_Build_Nack(e_InterfaceType, 10);
                                                }
                                             }
                                        }
                break;
        case Set_Motor_Speeds:          if (FA_SDN_Check_for_DeviceID(e_InterfaceType)){
                                            FA_App_Set_SlowSpeed(FA_SDN_GetRxData(e_InterfaceType, 13));
                                            FA_App_Set_FastDownSpeed(FA_SDN_GetRxData(e_InterfaceType, 12));
                                            FA_App_Set_FastUpSpeed(FA_SDN_GetRxData(e_InterfaceType, 11));
                                            bSaveSpeeds = true; 
                                            if (FA_SDN_Check_ForResponseBit(e_InterfaceType)){
                                                FA_SDN_Build_Ack(e_InterfaceType);
                                            }
                                        }
                break;
        case Get_Motor_Speeds:          if (FA_SDN_Check_ForResponseBit(e_InterfaceType)){
                                            if (FA_SDN_Check_for_DeviceID(e_InterfaceType)){
                                                if (FA_App_SystemHas_Speeds()){
                                                    u_NextTxData[0] = Post_Motor_Speeds;  
                                                    u_NextTxData[1] = 0x0E;
                                                    u_NextTxData[6] = FA_SDN_GetRxData(e_InterfaceType, 3);
                                                    u_NextTxData[7] = FA_SDN_GetRxData(e_InterfaceType, 4);
                                                    u_NextTxData[8] = FA_SDN_GetRxData(e_InterfaceType, 5);
                                                    u_NextTxData[9] = FA_App_Get_FastUpSpeed();
                                                    u_NextTxData[10] = FA_App_Get_FastDownSpeed();
                                                    u_NextTxData[11] = FA_App_Get_SlowSpeed();
                                                    FA_SDN_Build_Message(e_InterfaceType, u_NextTxData);
                                                }
                                                else {
                                                    FA_SDN_Build_Nack(e_InterfaceType, 10);
                                                }
                                             }
                                        }
                break;
        case Set_Motor_Internal_Position:   if (FA_SDN_Check_for_DeviceID(e_InterfaceType)){
                                                //bit 10 is the target, starting at 1
                                                u_Target = (FA_SDN_GetRxData(e_InterfaceType, 10)-1);
                                                //bit 9 tells us if % of count is being set 
                                                if (FA_SDN_GetRxData(e_InterfaceType, 9) == 3){
                                                    //Motor_InternalPos_Percent[u_Target] = (FA_SDN_GetRxData(e_InterfaceType, 11)&0x00FF);
                                                    FA_App_Set_InternalPos(u_Target, (uint16_t)((float)FA_App_Get_Up_Limit() * ((float)(FA_SDN_GetRxData(e_InterfaceType, 11)&0x00FF)/100.0)));
                                                }
                                                else if (FA_SDN_GetRxData(e_InterfaceType, 9) == 2){
                                                    FA_App_Set_InternalPos(u_Target, (uint16_t)(FA_SDN_GetRxData(e_InterfaceType, 12) + ((FA_SDN_GetRxData(e_InterfaceType, 11)<< 8)&0xFF00)));
                                                }
                                                //FA_Motor_Save_InternalPos(u_Target);
                                                bSave_InternalPos = true;
                                                bSave_InternalPosPointer[u_Target] = true;
                                                if (FA_SDN_Check_ForResponseBit(e_InterfaceType)){
                                                    FA_SDN_Build_Ack(e_InterfaceType);
                                                }
                                            }
                break;
        case Get_Motor_Internal_Position:   if (FA_SDN_Check_ForResponseBit(e_InterfaceType)){
                                                if (FA_SDN_Check_for_DeviceID(e_InterfaceType)){
                                                    if (FA_App_SystemHas_IPs()){
                                                        u_Target = (FA_SDN_GetRxData(e_InterfaceType, 9)-1);
                                                        u_NextTxData[0] = Post_Motor_Internal_Position;  
                                                        u_NextTxData[1] = 0x0F;
                                                        u_NextTxData[6] = FA_SDN_GetRxData(e_InterfaceType, 3);
                                                        u_NextTxData[7] = FA_SDN_GetRxData(e_InterfaceType, 4);
                                                        u_NextTxData[8] = FA_SDN_GetRxData(e_InterfaceType, 5);
                                                        u_NextTxData[9] = u_Target + 1;
                                                        u_NextTxData[10] = (uint8_t)(FA_App_Get_InternalPos(u_Target) & 0x00FF);
                                                        u_NextTxData[11] = (uint8_t)((FA_App_Get_InternalPos(u_Target)  >> 8)&0x00FF); 
                                                        u_NextTxData[12] = (uint8_t)(((float)FA_App_Get_InternalPos(u_Target) /(float)FA_App_Get_Up_Limit())*100);//Motor_InternalPos_Percent[u_Target] & 0x00FF;
                                                        FA_SDN_Build_Message(e_InterfaceType, u_NextTxData);
                                                    }
                                                    else {
                                                        FA_SDN_Build_Nack(e_InterfaceType, 10);
                                                    }
                                                }
                                            }
                break;
        case Set_Motor_Group_IDs:           if (FA_SDN_Check_for_DeviceID(e_InterfaceType)){
                                                u_Target = FA_SDN_GetRxData(e_InterfaceType, 9);
                                                Motor_Group_IDs[0][u_Target] = FA_SDN_GetRxData(e_InterfaceType, 12);
                                                Motor_Group_IDs[1][u_Target] = FA_SDN_GetRxData(e_InterfaceType, 11);
                                                Motor_Group_IDs[2][u_Target] = FA_SDN_GetRxData(e_InterfaceType, 10);
                                                bSave_GroupID = true;
                                                bSave_GroupIDPointer[u_Target] = true;
                                                if (FA_SDN_Check_ForResponseBit(e_InterfaceType)){
                                                    FA_SDN_Build_Ack(e_InterfaceType);
                                                }
                                            }
                break; 
        case Get_Motor_Group_ID:            if (FA_SDN_Check_ForResponseBit(e_InterfaceType)){
                                                if (FA_SDN_Check_for_DeviceID(e_InterfaceType)){
                                                    u_Target = FA_SDN_GetRxData(e_InterfaceType, 9);
                                                    u_NextTxData[0] = Post_Motor_Group_ID;  
                                                    u_NextTxData[1] = 0x0F;
                                                    u_NextTxData[6] = FA_SDN_GetRxData(e_InterfaceType, 3);
                                                    u_NextTxData[7] = FA_SDN_GetRxData(e_InterfaceType, 4);
                                                    u_NextTxData[8] = FA_SDN_GetRxData(e_InterfaceType, 5);
                                                    u_NextTxData[9] = u_Target;
                                                    u_NextTxData[10] = Motor_Group_IDs[2][u_Target]; 
                                                    u_NextTxData[11] = Motor_Group_IDs[1][u_Target];
                                                    u_NextTxData[12] = Motor_Group_IDs[0][u_Target];
                                                    FA_SDN_Build_Message(e_InterfaceType, u_NextTxData);
                                                }
                                            }
                break;
        case Flow_Search_For_Motors:    
                break;
        case Flow_Search_For_Motors_2: 
                                        if (FA_SDN_Check_ForResponseBit(e_InterfaceType)){
                                            if (FA_SDN_Check_for_DeviceID(e_InterfaceType)){
                                                FA_SDN_Build_Nack(e_InterfaceType, 0x10);
                                            }
                                        }
                break;
        case Flow_Search_For_Motors_3:  if (bSignOnOnce == false){
                                        //if (FA_SDN_Check_ForResponseBit(e_InterfaceType)){
                                           // if (bSignOnOnce == false){
                                                u_NextTxData[0] = Post_Motor_Part3;  
                                                u_NextTxData[1] = 0x0B;
                                                u_NextTxData[6] = FA_SDN_GetRxData(e_InterfaceType, 3);
                                                u_NextTxData[7] = FA_SDN_GetRxData(e_InterfaceType, 4);
                                                u_NextTxData[8] = FA_SDN_GetRxData(e_InterfaceType, 5);
                                                FA_SDN_Build_Message(e_InterfaceType, u_NextTxData);
                                            }
                                        //}
                break;
        case Set_Motor_ID:              if (FA_SDN_Check_for_DeviceID(e_InterfaceType)){
                                            //load message with old ID 
                                            u_NextTxData[9] = FA_PROTOCOL_COMMON_Get_Device_ID(2);
                                            u_NextTxData[10] = FA_PROTOCOL_COMMON_Get_Device_ID(1);
                                            u_NextTxData[11] = FA_PROTOCOL_COMMON_Get_Device_ID(0);
                                            
                                            //check for all zeros
                                            u_TempDeviceID[0] = FA_SDN_GetRxData(e_InterfaceType, 11);
                                            u_TempDeviceID[1] = FA_SDN_GetRxData(e_InterfaceType, 10);
                                            u_TempDeviceID[2] = FA_SDN_GetRxData(e_InterfaceType, 9);
                                            //save new ID
                                            FA_PROTOCOL_COMMON_Set_Device_ID(u_TempDeviceID);
                                            u_NextTxData[0] = Post_New_Motor_ID;  
                                            u_NextTxData[1] = 0x0E;
                                            u_NextTxData[6] = FA_SDN_GetRxData(e_InterfaceType, 3);
                                            u_NextTxData[7] = FA_SDN_GetRxData(e_InterfaceType, 4);
                                            u_NextTxData[8] = FA_SDN_GetRxData(e_InterfaceType, 5);
                                            if (FA_SDN_Check_ForResponseBit(e_InterfaceType)){
                                                FA_SDN_Build_Message(e_InterfaceType, u_NextTxData);
                                            }
                                        }
                break;
        case Get_Firmware_Version:      if (FA_SDN_Check_ForResponseBit(e_InterfaceType)){
                                            if (FA_SDN_Check_for_DeviceID(e_InterfaceType)){
                                                u_NextTxData[0] = Post_Firmware_Version;  
                                                u_NextTxData[1] = 0x11;
                                                u_NextTxData[6] = FA_SDN_GetRxData(e_InterfaceType, 3);
                                                u_NextTxData[7] = FA_SDN_GetRxData(e_InterfaceType, 4);
                                                u_NextTxData[8] = FA_SDN_GetRxData(e_InterfaceType, 5);
                                                u_NextTxData[9] = 0x62; 
                                                u_NextTxData[10] = 0x8A;
                                                u_NextTxData[11] = 0xA9;
                                                u_NextTxData[12] = 0x46;
                                                u_NextTxData[13] = 0x96;
                                                u_NextTxData[14] = 0x01;
                                                FA_SDN_Build_Message(e_InterfaceType, u_NextTxData);
                                            }
                                        }
                break;
        case Get_Motor_Pos:             if (FA_SDN_Check_ForResponseBit(e_InterfaceType)){
                                            if (FA_SDN_Check_for_DeviceID(e_InterfaceType)){
                                                u_NextTxData[0] = Post_Motor_Pos;  
                                                u_NextTxData[1] = 0x10;
                                                u_NextTxData[6] = FA_SDN_GetRxData(e_InterfaceType, 3);
                                                u_NextTxData[7] = FA_SDN_GetRxData(e_InterfaceType, 4);
                                                u_NextTxData[8] = FA_SDN_GetRxData(e_InterfaceType, 5);
                                                u_NextTxData[9] = 0xF4; 
                                                u_NextTxData[10] = 0x01;
                                                u_NextTxData[11] = 0x32;
                                                u_NextTxData[12] = 0xFF;
                                                u_NextTxData[13] = 0xFF;
                                                FA_SDN_Build_Message(e_InterfaceType, u_NextTxData);
                                            }
                                        }
                break;
/*         case Flow_Set_MAC_Address:      FA_SDN_Build_Ack(e_InterfaceType);
                break;
        //set mac, mac rx
        case Set_MAC_Address:           if (FA_SDN_Check_for_DeviceID(e_InterfaceType)){
                                           FA_SDN_SET_MAC_ADDRESS(e_InterfaceType);
                                           u_NextTxData[0] = SDN_ACK;  
                                           u_NextTxData[1] = 0x0B;
                                           u_NextTxData[6] = FA_SDN_GetRxData(e_InterfaceType, 3);
                                           u_NextTxData[7] = FA_SDN_GetRxData(e_InterfaceType, 4);
                                           u_NextTxData[8] = FA_SDN_GetRxData(e_InterfaceType, 5);
                                           FA_SDN_Build_Message(e_InterfaceType, u_NextTxData);
                                        }
                break;
        //set mac, mac rx
        case Get_MAC_Address:           FA_SDN_GET_MAC_ADDRESS(e_InterfaceType);
                break;*/
        case Flow_IPv4_Address:         if (FA_SDN_Check_ForResponseBit(e_InterfaceType)){
                                            FA_SDN_Build_Ack(e_InterfaceType);
                                        }
                break;
        //get IPv4, address
        case Get_IPv4_Address:          if (FA_SDN_Check_ForResponseBit(e_InterfaceType)){
                                            FA_SDN_GET_IPv4_ADDRESS(e_InterfaceType);
                                        }
                break;
        //tcpip
        case Set_IPv4_Address:          if (FA_SDN_Check_for_DeviceID(e_InterfaceType)){
                                            FA_SDN_SET_IPv4_ADDRESS(e_InterfaceType);
                                            if (FA_SDN_Check_ForResponseBit(e_InterfaceType)){
                                                FA_SDN_Build_Ack(e_InterfaceType);
                                            }
                                        }
                break;  
        case Set_SDN_Label:             if (FA_SDN_Check_for_DeviceID(e_InterfaceType)){
                                            FA_SDN_SET_LABEL(e_InterfaceType);
                                            if (FA_SDN_Check_ForResponseBit(e_InterfaceType)){
                                                FA_SDN_Build_Ack(e_InterfaceType);
                                            }
                                        }
                break;
        case Get_SDN_Label:             if (FA_SDN_Check_for_DeviceID(e_InterfaceType)){
                                            u_NextTxData[0] = Post_SDN_Label;  
                                            u_NextTxData[1] = 0x1B;
                                            u_NextTxData[6] = FA_SDN_GetRxData(e_InterfaceType, 3);
                                            u_NextTxData[7] = FA_SDN_GetRxData(e_InterfaceType, 4);
                                            u_NextTxData[8] = FA_SDN_GetRxData(e_InterfaceType, 5);
                                            for (uint8_t u8_For_Loop_Counter = 0; u8_For_Loop_Counter < 16; u8_For_Loop_Counter++){
                                                u_NextTxData[u8_For_Loop_Counter+9] = FA_PROTOCOL_COMMON_GET_LABEL(u8_For_Loop_Counter);
                                            }
                                            if (FA_SDN_Check_ForResponseBit(e_InterfaceType)){
                                                FA_SDN_Build_Message(e_InterfaceType, u_NextTxData);
                                            }
                                            bSignOnOnce = true;
                                        }
                break;    
            case SDN_Factory_Reset:     if (FA_SDN_Check_for_DeviceID(e_InterfaceType)){
                                            SYS_RESET_SoftwareReset();
                                        }
                break; 
        default:    if (FA_SDN_Check_for_DeviceID(e_InterfaceType)){
                        if (FA_SDN_Check_ForResponseBit(e_InterfaceType)){
                            FA_SDN_Build_Nack(e_InterfaceType, 0x10);
                        }
                    }
                break;
            
    }
}     


uint16_t FA_SDN_Motor_Retrieve_Mech_Function(void) {
    uint16_t u16_retVal;
    
    u16_retVal = u16_Mech_Function;
    u16_Mech_Function = FN_NULL;
    return u16_retVal;
}


 /******************************************************************************
external Function:
FA_SDN_Motor_Clear_Temp
  Remarks: temp values need to persist until inactive timeout, 
  * this is called to remove temp values after timeout
 
 */
void FA_SDN_Motor_InactiveTimeOut(void){
    bSignOnOnce = false;
    //save Preset Positions to EEPROM
    if (bSave_InternalPos){
        bSave_InternalPos = false;
        for (uint8_t InternalPosPointer; InternalPosPointer < 16; InternalPosPointer ++){
            if (bSave_InternalPosPointer[InternalPosPointer]){
                bSave_InternalPosPointer[InternalPosPointer] = false; 
                FA_App_Save_InternalPos(InternalPosPointer);
            }
        } 
    }
    //save bSave_GroupID to EEPROM
    if (bSave_GroupID){
        bSave_GroupID = false;
        for (uint8_t GroupIDPointer; GroupIDPointer < 16; GroupIDPointer ++){
            if (bSave_GroupIDPointer[GroupIDPointer]){
                bSave_GroupIDPointer[GroupIDPointer] = false; 
                FA_Motor_Save_GroupID(GroupIDPointer);
            }
        } 
    }
    if (bSaveLimits){
        bSaveLimits = false;
        FA_App_Save_MotorLimits();
    }
    if (bSaveSpeeds){
        bSaveSpeeds = false;
        FA_App_Save_MotorSpeeds();
    }
    if (bSaveOrientation){
        FA_App_Save_Direction();
    }
}

/******************************************************************************
external Function:
FA_SDN_Motor_CheckForGroupID
  Remarks: motors can be targeted as part of a group
 * the group structure is always 
 *      sender ID = Group ID
 *      Target ID = 0x00
 * 
 * We can use this to first check for zeros in the target then check for group ID
 
 */
bool FA_SDN_Motor_CheckForGroupID(t_InterfaceType e_InterfaceType){
    bool bRetval = false;
    t_USART_ID Targit_ID;
    
    if ((FA_SDN_GetRxData(e_InterfaceType, 6) == 0x00) &&
    (FA_SDN_GetRxData(e_InterfaceType, 7) == 0x00) &&
    (FA_SDN_GetRxData(e_InterfaceType, 8) == 0x00)){
    
        //to do store target address here 
        Targit_ID.u8_USART_ID_1 = FA_SDN_GetRxData(e_InterfaceType, 5);
        Targit_ID.u8_USART_ID_2 = FA_SDN_GetRxData(e_InterfaceType, 4);
        Targit_ID.u8_USART_ID_3 = FA_SDN_GetRxData(e_InterfaceType, 3);



        for (uint8_t ForLoop = 0; ForLoop < 16; ForLoop++){
            //targeting for individual motors 
            if((Motor_Group_IDs[0][ForLoop] == Targit_ID.u8_USART_ID_1) &&
               (Motor_Group_IDs[1][ForLoop] == Targit_ID.u8_USART_ID_2) &&
                (Motor_Group_IDs[2][ForLoop] == Targit_ID.u8_USART_ID_3)){

                    bRetval = true;

            }
        }
    }
    return bRetval;
}

 /******************************************************************************
external Function:
FA_Motor_Save_GroupID:

  Remarks: call to save the group id into eeprom
 
 */
void FA_Motor_Save_GroupID (uint8_t GroupMultiplyer){
    FA_SEEPROM_write_8(EEPROM_MOTOR_GROUP+(GroupMultiplyer*3), Motor_Group_IDs[0][GroupMultiplyer]);
    FA_SEEPROM_write_8(EEPROM_MOTOR_GROUP+((GroupMultiplyer*3)+1), Motor_Group_IDs[1][GroupMultiplyer]);
    FA_SEEPROM_write_8(EEPROM_MOTOR_GROUP+((GroupMultiplyer*3)+2), Motor_Group_IDs[2][GroupMultiplyer]);
}