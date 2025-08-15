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
    fa_sdn.h

  Summary:
    This header file provides prototypes and definitions for the Future Automation
    RS485 interface.

  Description:
    This header file provides function prototypes and data type definitions for
    the Future Automation SDN interface.
 *******************************************************************************/

#ifndef FA_SDN_H
#define FA_SDN_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "fa_usart.h"
#include "protocol_common.h"




// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

//flow control timers
#define SDN_INACTIVE_TIMEOUT_mS           400
#define SDN_BUS_FREE_TIMEOUT_mS           3
#define SDN_MIN_RESPONSE_TIME_mS          5
#define SDN_MAX_RESPONSE_TIME_mS          255

#define SDN_MSG_NO_PAYLOAD 11

#define SDN_ERROR_DATA_OUT_OF_RANGE 0x1
#define SDN_ERROR_UNKNOWN_MSG 0x10
#define SDN_ERROR_LENGTH 0x11
#define SDN_ERROR_CHECKSUM 0x12
#define SDN_ERROR_BUSY 0xFF

// *****************************************************************************
// *****************************************************************************
// Section: Function declarations
// *****************************************************************************
// *****************************************************************************
#pragma pack(push, 1)

typedef union {

    struct {
        uint8_t MessageType;
        uint8_t Ack : 1;
        uint8_t Length : 7;
        uint8_t SourceNodeType : 4;
        uint8_t DestNodeType : 4;
        t_USART_ID Source;
        t_USART_ID Dest;
        uint8_t Data[FRAMEWORK_MAX_PACKET_SIZE - SDN_MSG_NO_PAYLOAD];
        uint16_t Checksum;
    };

    uint8_t raw[FRAMEWORK_MAX_PACKET_SIZE];
} t_SDN_message;
#pragma pack(pop)


// *****************************************************************************
// *****************************************************************************
// Functions
// *****************************************************************************
// *****************************************************************************
void FA_SDN_Initialize(void);
void FA_SDN_Initialize_232(uint32_t USART_232_Port, USART_SERIAL_SETUP* USART_Config, bool Sys_Has_Echo);
void FA_SDN_Initialize_485(uint32_t USART_485_Port, USART_SERIAL_SETUP* USART_Config);
void FA_SDN_Tasks(void);
bool FA_SDN_Check_For_DeviceID_Or_Broadcast(t_InterfaceType e_InterfaceType);
bool FA_SDN_Check_For_DeviceID(t_InterfaceType e_InterfaceType);
bool FA_SDN_Check_For_BroadcastID(t_InterfaceType e_InterfaceType);
uint8_t FA_SDN_Get_BroadcastID(uint8_t BroadcastByte);
uint8_t FA_SDN_GetRxData(t_InterfaceType e_InterfaceType, uint8_t u16_DataByte);
void SDN_Start_Timers(t_InterfaceType e_InterfaceType);
bool FA_SDN_Check_ForResponseBit(t_InterfaceType e_InterfaceType);
void Start_SDN_Timer(t_FlowControlTimers SelectStartTimer);
uint8_t FA_SDN_Get_Device_ID(uint8_t uID_Byte);
void FA_APP_InactiveTimeOut(void);
uint16_t FA_SDN_GET_FA_CMD(void);
void FA_SDN_Initialize_TCP(bool b_Has_Echo);
void FA_SDN_Send_Message(t_InterfaceType e_InterfaceType, uint8_t MessageType, bool Ack, uint8_t Length, t_USART_ID* source, t_USART_ID* target, uint8_t* Data);
void FA_SDN_Send_Ack(t_InterfaceType e_InterfaceType);
void FA_SDN_Send_Nack(t_InterfaceType e_InterfaceType, uint8_t u8_Error_Code);

#endif

//DOM-IGNORE-BEGIN
#ifdef __cplusplus

#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

