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
    fa_protocol.h

  Summary:
    This header file provides prototypes and definitions for the Future Automation
    RS485 interface.

  Description:
    This header file provides function prototypes and data type definitions for
    the Future Automation SDN interface.
 *******************************************************************************/

#ifndef FA_POTOCOL_H
#define FA_POTOCOL_H

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
#include "fa_packet.h"



// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

#define FA_PROTOCOL_INACTIVE_TIMEOUT_mS           150
#define FA_PROTOCOL_BUS_FREE_TIMEOUT_uS           7500
#define FA_PROTOCOL_MIN_RESPONSE_TIME_uS          (FA_PROTOCOL_BUS_FREE_TIMEOUT_uS)
#define FA_PROTOCOL_MAX_RESPONSE_TIME_uS          75000

#pragma pack(push, 1)

typedef union {

    struct {
        uint8_t MessageType;
        uint8_t ResponseBit : 1;
        uint8_t Length : 7;
        uint16_t MasterSlaveBit : 1;
        uint16_t NodeType : 15;
        t_USART_ID SenderID;
        t_USART_ID TargetID;
        uint8_t Data[FRAMEWORK_MAX_PACKET_SIZE - MSG_NO_PAYLOAD];
        uint16_t Checksum;
    };

    uint8_t raw[FRAMEWORK_MAX_PACKET_SIZE];
} t_fa_protocol_message;
#pragma pack(pop)



// *****************************************************************************
// *****************************************************************************
// Section: Function declarations
// *****************************************************************************
// *****************************************************************************



// *****************************************************************************
// *****************************************************************************
// Functions
// *****************************************************************************
// *****************************************************************************
void FA_PROTOCOL_Initialize(void);
void FA_PROTOCOL_Initialize_232(uint32_t USART_232_Port, USART_SERIAL_CONFIG* USART_Config, bool b_Has_Echo);
void FA_PROTOCOL_Initialize_485(uint32_t USART_485_Port, USART_SERIAL_CONFIG* USART_Config);
void FA_PROTOCOL_Initialize_TCPIP(bool b_Has_Echo);
void FA_PROTOCOL_Tasks(void);
void FA_PROTOCOL_Start_Timers(t_InterfaceType e_InterfaceType);

bool FA_PROTOCOL_Check_For_DeviceID_Or_Broadcast(t_InterfaceType e_InterfaceType);
bool FA_PROTOCOL_CheckProgrammerID(t_InterfaceType e_InterfaceType);
bool FA_PROTOCOL_Check_For_BroadcastID(t_InterfaceType e_InterfaceType);
bool FA_PROTOCOL_Check_ForResponseBit(t_InterfaceType e_InterfaceType);
bool FA_PROTOCOL_Check_For_DeviceID(t_InterfaceType e_InterfaceType);
t_USART_ID FA_PROTOCOL_Get_SenderID(t_InterfaceType e_InterfaceType);
bool FA_PROTOCOL_Set_DeviceID(t_InterfaceType e_InterfaceType);
void FA_PROTOCOL_Send_Message(t_InterfaceType e_InterfaceType, uint8_t MessageType, bool ResponseBit, uint8_t Length, bool MasterSlaveBit, t_USART_ID* target, uint8_t* Data);
void FA_PROTOCOL_Send_Message_Struct(t_InterfaceType e_InterfaceType, t_fa_protocol_message* t_DataStruct);
void FA_PROTOCOL_Send_Ack(t_InterfaceType e_InterfaceType);
void FA_PROTOCOL_Send_Nack(t_InterfaceType e_InterfaceType);
uint16_t FA_PROTOCOL_Get_FA_CMD(void);

#endif

//DOM-IGNORE-BEGIN
#ifdef __cplusplus

#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

