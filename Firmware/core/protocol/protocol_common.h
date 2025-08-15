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
    fa_protocol_common.h

  Summary:
    This header file provides prototypes and definitions for the Future Automation
    common protocol interface.

  Description:
    This header file is the public interface for the common protocol functionality.
    This file should not be interpreted as "FA_PROTOCOL"_COMMON, it is not a common file
    for FA PROTOCOL. It should be interpreted as FA_"PROTOCOL_COMMON", it is the common
    functionality between any protocol such as FA Protocol or SDN Protocol. Therefore,
    the contents of this file should not contain anything exclusive to any given protocol.
 *******************************************************************************/

#ifndef FA_PROTOCOL_COMMON_H
#define FA_PROTOCOL_COMMON_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "fa_packet.h"
#include "tcpip/tcpip.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define MSG_NO_PAYLOAD       12
#define LABEL_LENGTH 16
#define CMD_LENGTH 13
#define DEVICE_ID_LENGTH 3
#define MAC_LENGTH 6
#define IP_LENGTH 11
#define TOP_BIT_BM 0x80
#define REMAINING_BITS_BM 0x7F
#define BOTTOM_BYTE_BM 0x00FF
#define DEVICE_ID_SIZE 3
#define PROTOCOL_DATA_BUFFER_SIZE 50
// 485, 232, 3 TCP clients, 6 TCP Server connections
#define NUMBER_OF_INTERFACES 11
#define Motor_Direction_length  1
#define Motor_Limits_length  4
#define Motor_Speed_length 4

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

// Subnet mask is structured the same as an IP address
typedef IPV4_ADDR SUBNET_MASK;

/**
 * Interface Types
 *
 * Enum to represent a specific interface, such as 232, 485, etc.
 */
typedef enum {
    Interface_485,
    Interface_232,
    Interface_TCPIP_Server_Client_1,
    Interface_TCPIP_Server_Client_2,
    Interface_TCPIP_Server_Client_3,
    Interface_TCPIP_Server_Client_4,
    Interface_TCPIP_Server_Client_5,
    Interface_TCPIP_Server_Client_6,
    Interface_TCPIP_Client_1,
    Interface_TCPIP_Client_2,
    Interface_TCPIP_Client_3,
} t_InterfaceType;

/**
 * Flow Control Timers
 *
 * Enum to represent the different timers.
 */
enum emumFlowControlTimers {
    Timer_485_BusFree, Timer_485_BusFreeRand, Timer_Inactive
};
typedef enum emumFlowControlTimers t_FlowControlTimers;

/**
 * USART ID
 *
 * Struct to represent an address to send information to.
 * I.E. the device ID of another device to send a protcol message to.
 */
typedef struct {
    uint8_t u8_USART_ID_1;
    uint8_t u8_USART_ID_2;
    uint8_t u8_USART_ID_3;
} t_USART_ID;

// *****************************************************************************
// *****************************************************************************
// Section: Function declarations
// *****************************************************************************
// *****************************************************************************
/**
 * FA_PROTOCOL_COMMON_Initialize is the common protocol initialisation function. This function must be called
 * before using any protocol code.
 *
 * The function will read the common protocol data (Device label, device ID, etc.)
 * from EEPROM and store them ready for a protocol to be used.
 */
void FA_PROTOCOL_COMMON_Initialize(void);

/**
 * FA_PROTOCOL_COMMON_Reset_EEPROM is used to reset the common protocol EEPROM data.
 */
void FA_PROTOCOL_COMMON_Reset_EEPROM(void);

/**
 * FA_PROTOCOL_Common_InactiveTimeOut is the function that should be called when the inactive timer expires.
 * This will then process the tasks that are only to be done once device communication has finished after some time.
 * Namely, writing new data to EEPROM.
 */
void FA_PROTOCOL_COMMON_InactiveTimeOut(void);

/**
 * FA_PROTOCOL_COMMON_Tasks performs the common protocol tasks. This function must be called
 * once every iteration of the main loop.
 */
void FA_PROTOCOL_COMMON_Tasks(void);

/**
 * FA_PROTOCOL_COMMON_Get_Label is a getter function for the label of the device.
 * The function assumes the size of the buffer passed as an argument is "LABEL_LENGTH"
 * long.
 * @param u8Ptr_Label A pointer to the buffer to copy the label into.
 */
void FA_PROTOCOL_COMMON_Get_Label(uint8_t* u8Ptr_Label);

/**
 * FA_PROTOCOL_COMMON_Set_Label is a setter for the device label. The function assumes the label
 * is "LABEL_LENGTH" long. The function also marks the label as ready to be saved
 * to EEPROM once communication on the device has halted for the length of the
 * inactive timeout.
 *
 * @param u8Ptr_Label A pointer to the label which will become the devices new label.
 */
void FA_PROTOCOL_COMMON_Set_Label(uint8_t* u8Ptr_Label);

/**
 * FA_PROTOCOL_COMMON_Get_Device_ID is a getter function for the Device ID of the device.
 * The function assumes the size of the buffer passed as an argument is "DEVICE_ID_LENGTH"
 *
 * @param u8_Device_ID A pointer to the buffer to copy the device ID into.
 */
void FA_PROTOCOL_COMMON_Get_Device_ID(uint8_t* u8Ptr_DeviceID);

/**
 * FA_PROTOCOL_COMMON_Set_Device_ID is a setter for the Device ID. The function assumes the Device ID
 * is "DEVICE_ID_SIZE" long. The function also marks the Device ID as ready to be saved
 * to EEPROM once communication on the device has halted for the length of the
 * inactive timeout.
 *
 * @param u8Ptr_Device_ID Buffer that is DEVICE_ID_LENGTH long containing the device ID.
 * @return True if the device ID was set correctly.
 */
bool FA_PROTOCOL_COMMON_Set_Device_ID(uint8_t* u8Ptr_DeviceID);

/**
 * FA_PROTOCOL_COMMON_Get_IPv4_Address will get the current network data for the control board and
 * write it to the supplied buffers.
 *
 * @param bPtr_DHCP_Enabled A pointer to a bool to write the DHCP_Enabled flag to.
 * @param tPtr_NewIP A pointer to the IPV4 struct to write the data to.
 * @param tPtr_Subnet A pointer to the SUBNET_MASK struct to write the data to.
 * @param u16Ptr_Port A pointer to the port to write the data to.
 */
void FA_PROTOCOL_COMMON_Get_IPv4_Address(bool* bPtr_DHCP_Enabled, IPV4_ADDR* tPtr_NewIP, SUBNET_MASK* tPtr_Subnet, uint16_t* u16Ptr_Port);

/**
 * FA_PROTOCOL_COMMON_Set_IPv4_Address will set the new network data for the control board
 * as supplied by the arguments. The function also marks the network data as ready to be saved
 * to EEPROM once communication on the device has halted for the length of the
 * inactive timeout.
 *
 * @param b_DHCP_Enabled A boolean representing if DHCP is enabled.
 * @param t_NewIP The new IP address The new IP address to set the board to.
 * @param t_Subnet The new subnet mask. The new Subnet Mask to set the board to.
 * @param u16_port The new TCP server port. The new sever port to listen on.
 */
void FA_PROTOCOL_COMMON_Set_IPv4_Address(bool b_DHCP_Enabled, IPV4_ADDR t_NewIP, SUBNET_MASK t_Subnet, uint16_t u16_Port);

/**
 * FA_PROTOCOL_COMMON_Get_MAC will write the MAC address to the buffer pointed to by the argument.
 *
 * @param u8Ptr_MAC A pointer to a buffer to write the MAC address to.
 */
void FA_PROTOCOL_COMMON_Get_MAC(uint8_t* u8Ptr_MAC);

/**
 * FA_PROTOCOL_COMMON_Get_Broadcast_ID is a getter function for the broadcast ID.
 * The function assumes the size of the buffer passed as an argument is "DEVICE_ID_LENGTH".
 *
 * @param u8_Broadcast_ID A pointer to the buffer to copy the broadcast ID into.
 */
void FA_PROTOCOL_COMMON_Get_Broadcast_ID(uint8_t* u8Ptr_Broadcast_ID);

/**
 * FA_PROTOCOL_COMMON_Check_for_Device_ID will check to see if the devices ID is
 * equal to the ID passed as the argument.
 * @param Target_ID The ID to check is equal to the device ID.
 * @return True if the supplied ID is equal to the devices ID. False if not.
 */
bool FA_PROTOCOL_COMMON_Check_For_Device_ID(t_USART_ID Target_ID);

/**
 * FA_PROTOCOL_COMMON_Check_for_Broadcast_ID will check to see if the supplied
 * ID is equal to the broadcast ID.
 * @param Target_ID The ID to check is equal to the broadcast ID.
 * @return True if the supplied ID is equal to the broadcast ID. False if not.
 */
bool FA_PROTOCOL_COMMON_Check_For_Broadcast_ID(t_USART_ID Target_ID);

#endif

//DOM-IGNORE-BEGIN
#ifdef __cplusplus

#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

