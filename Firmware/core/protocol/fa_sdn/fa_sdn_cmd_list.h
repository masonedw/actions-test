/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

/*******************************************************************************
  Future Automation Application Header File

  Company:
    Future Automation.

  File Name:
    fa_sdn_cmd_list.h

  Summary:
    This header file provides prototypes and definitions for the Future Automation
    RS485 interface.

  Description:
    This header file provides function prototypes and data type definitions for
    the Future Automation SDN interface.
 *******************************************************************************/

#ifndef FA_SDN_CMD_LIST_H
#define FA_SDN_CMD_LIST_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

//Internal Move CMDs
#define Move_Pulse_Down                     0x01
#define Move_Stop                           0x02
#define Move_Three                          0x03
#define Move_Four                           0x04
#define Move_Visual_Feedback                0x05

//Motor Specific CMDs
#define Get_Motor_Pos               0x0C
#define Post_Motor_Pos              0x0D

//*********************************
//Flow / Free Commands
//*********************************
//#define 0x00
#define Set_Move_Up                         0x01
#define Set_Move_Down                       0x02
#define Set_Move_Stop                       0x03
#define Set_Move_Go_To_IP                   0x04
#define Set_Move_Next_IP_Up                 0x05
#define Set_Move_Next_IP_Down               0x06
//#define 0x07
#define Set_Move_Go_To_Pulse                0x08
//#define 0x09
#define Set_Move_Jog_Up_mS                  0x0A
#define Set_Move_Jog_Down_mS                0x0B
#define Set_Move_Jog_Up_Pulse               0x0C
#define Set_Move_Jog_Down_Puse              0x0D
//#define 0x0E
//#define 0x0F
//*********************************
//Set Motor Info
//*********************************
#define Set_Move_Go_To_Percent                0x10
#define Set_Motor_Limits                      0x11
#define Set_Motor_Orientation                 0x12
#define Set_Motor_Speeds                      0x13
//#define 0x14
#define Set_Motor_Internal_Position           0x15
#define Set_Server_IPs                       0x16
//#define 0x17
//#define 0x18
//#define 0x19
//#define 0x1A
//#define 0x1B
//#define 0x1C
//#define 0x1D
//#define 0x1E
#define SDN_Factory_Reset             0x1F
//*********************************
//Set Keypad Info
//*********************************
//define 0x20
#define Get_Motor_Limits                0x21
#define Get_Motor_Orientation           0x22
#define Get_Motor_Speeds                0x23
#define Move_Toggle                     0x24
#define Get_Motor_Internal_Position     0x25
//#define 0x26
//#define 0x27
//#define 0x28
//#define 0x29
//#define 0x2A
//#define 0x2B
//#define 0x2C
//#define 0x2D
//#define 0x2E
//#define 0x2F
//*********************************
//Set General Info
//*********************************
//#define 0x30
#define Post_Motor_Limits               0x31
#define Post_Motor_Orientation          0x32
#define Post_Motor_Speeds               0x33
//#define 0x34
#define Post_Motor_Internal_Position     0x35
//#define 0x36
//#define 0x37
//#define 0x38
//#define 0x39
//#define 0x3A
//#define 0x3B
//#define 0x3C
//#define 0x3D
//#define 0x3E
//#define 0x3F
//*********************************
//Available 
//*********************************
#define Flow_Search_For_Motors_3        0x40
#define Get_Motor_Group_ID              0x41
//#define 0x42
//#define 0x43
//#define 0x44
#define Get_SDN_Label                   0x45
//#define 0x46
//#define 0x47
//#define 0x48
//#define 0x49
//#define 0x4A
//#define 0x4B
//#define 0x4C
//#define 0x4D
//#define 0x4E
//#define 0x4F
//*********************************
//Get Motor Info
//*********************************
#define Flow_Search_For_Motors_2        0x50
#define Set_Motor_Group_IDs             0x51
//#define 0x52
//#define 0x53
#define Set_Motor_ID                    0x54
#define Set_SDN_Label                   0x55
#define Flow_Search_For_Motors          0x56
//#define 0x57
//#define 0x58
//#define 0x59
//#define 0x5A
//#define 0x5B
//#define 0x5C
//#define 0x5D
//#define 0x5E
//#define 0x5F
//*********************************
//Get Keypad Info
//*********************************
#define Post_Motor_Part3                0x60
#define Post_Motor_Group_ID             0x61
//#define 0x62
//#define 0x63
//#define 0x64
#define Post_SDN_Label                  0x65
//#define 0x66
//#define 0x67
//#define 0x68
//#define 0x69
//#define 0x6A
//#define 0x6B
//#define 0x6C
//#define 0x6D
//#define 0x6E
#define SDN_NACK                        0x6F
//*********************************
//Get General Info
//*********************************
#define Flow_Get_Keypad_CMDs_2          0x70
//#define 0x71
//#define 0x72
//#define 0x73
#define Get_Firmware_Version            0x74
#define Post_Firmware_Version           0x75
//#define 0x76
//#define 0x77
//#define 0x78
//#define 0x79
//#define 0x7A
//#define 0x7B
//#define 0x7C
//#define 0x7D
//#define 0x7E
#define SDN_ACK                         0x7F
//*********************************

//Available 
//*********************************
#define Flow_Get_Keypad_CMDs_3          0x80
#define Flow_Get_Keypad_CMDs            0x81
#define Get_Keypad_CMDs                 0x82
#define Get_Keypad_Group                0x83
#define Get_10Keypad_Group              0x84
#define Set_10Keypad_Group              0x85
//#define 0x86
//#define 0x87
//#define 0x88
//#define 0x89
//#define 0x8A
//#define 0x8B
//#define 0x8C
//#define 0x8D
//#define 0x8E
#define Get_Keypad_Firmawre             0x8F
//*********************************
//Control
//*********************************
#define Set_Keypad_ID                   0x90
//#define 0x91
#define Set_Keypad_CMDs                 0x92
#define Set_Keypad_Group                0x93
#define Flow_Programming_Keypad         0x94
//#define 0x95
//#define 0x96
//#define 0x97
//#define 0x98
//#define 0x99
//#define 0x9A
//#define 0x9B
//#define 0x9C
//#define 0x9D
//#define 0x9E
#define Post_Keypad_Firmawre            0x9F
//*********************************
//Ack
//*********************************
#define ACK_Keypad_CMDs_2               0xA0
#define ACK_Keypad_CMDs                 0xA1
#define Post_Keypad_CMDs                0xA2
#define Post_Keypad_Group               0xA3
#define Post_10Keypad_Group             0xA4
//#define 0xA5
//#define 0xA6
//#define 0xA7

//#define 0xA8
//#define 0xA9
//#define 0xAA
//#define 0xAB
//#define 0xAC
//#define 0xAD
//#define 0xAE
//#define 0xAF
//*********************************
//Nack
//*********************************
//#define 0xB0
//#define 0xB1
//#define 0xB2
//#define 0xB3
//#define 0xB4
//#define 0xB5
//#define 0xB6
//#define 0xB7
//#define 0xB8
//#define 0xB9
//#define 0xBA
//#define 0xBB
//#define 0xBC
//#define 0xBD
//#define 0xBE
//#define 0xBF
//*********************************
//Post Motor Commands
//*********************************
//#define 0xC0
//#define 0xC1
//#define 0xC2
//#define 0xC3
//#define 0xC4
//#define 0xC5
//#define 0xC6
//#define 0xC7
//#define 0xC8
//#define 0xC9
//#define 0xCA
//#define 0xCB
//#define 0xCC
//#define 0xCD
//#define 0xCE
//#define 0xCF
//*********************************
//Post Keypad Commands
//*********************************
//#define 0xD0
//#define 0xD1
//#define 0xD2
//#define 0xD3
//#define 0xD4
//#define 0xD5
//#define 0xD6
//#define 0xD7
//#define 0xD8
//#define 0xD9
//#define 0xDA
//#define 0xDB
//#define 0xDC
//#define 0xDD
//#define 0xDE
//#define 0xDF
//*********************************
//Post General Commands
//*********************************
#define Flow_IPv4_Address               0xE0
#define Get_IPv4_Address                0xE1
#define Set_IPv4_Address                0xE2
#define Post_IPv4_Address               0xE3
//#define 0xE4
//#define 0xE5
//#define 0xE6
#define Flow_Set_MAC_Address            0xE7
#define Get_MAC_Address                 0xE8
#define Set_MAC_Address                 0xE9
#define Post_MAC_Address                0xEA
//#define 0xEB
//#define 0xEC
//#define 0xED
//#define 0xEE
//#define 0xEF
//*********************************
//Available 
//*********************************
//#define 0xF0
//#define 0xF1
//#define 0xF2
//#define 0xF3
//#define 0xF4
//#define 0xF5
//#define 0xF6
//#define 0xF7
//#define 0xF8
//#define 0xF9
//#define 0xFA
#define Post_New_Motor_ID               0xFB
//#define 0xFC
//#define 0xFD
//#define 0xFE
//#define 0xFF
// *****************************************************************************
// *****************************************************************************
// Section: Protocol Commands 
// *****************************************************************************
// *****************************************************************************


#endif

//DOM-IGNORE-BEGIN
#ifdef __cplusplus

#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

