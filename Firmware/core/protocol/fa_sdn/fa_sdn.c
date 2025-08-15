/*******************************************************************************
  Future Automation Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    fa_sdn.c

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
#include "fa_sdn.h"
#include "fa_sdn_keypad.h"
#include "fa_timer.h"
#include "fa_app.h"
#include "fa_eeprom_map.h"
#include "fa_eeprom.h"
#include "fa_tcpip.h"
#include "fa_sdn_cmd_list.h"
#include "fa_ir.h"
#include "fa_at24.h"
#include "protocol_common.h"
#include "fa_tcpip.h"
#include "fn_codes.h"
#include "logger/logger.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define SDN_MSG_LENGTH_BM 0x1F
// The EXT part of Ack/Length is used if the message length exceedes 31.
// This macro only covers the low part of EXT.
#define SDN_MSG_LENGTH_EXT_L_BM 0x3F
#define SDN_MSG_SOURCE_NODE_TYPE_BM 0xF0
#define SDN_MSG_DEST_NODE_TYPE_BM 0x0F

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

static uint32_t FA_SDN_232_Port = 0;
static bool FA_SDN_Has_232 = false;
static bool FA_SDN_Has_232_Forwarding = false;
static bool FA_SDN_Has_TCPIP_Forwarding = false;
static uint32_t FA_SDN_485_Port = 0;
static bool FA_SDN_Has_485 = false;
static bool FA_SDN_Has_TCPIP = false;

//Data Storage
//Receive array for 485 and 232
static uint8_t u8_SDN_RxPacket[NUMBER_OF_INTERFACES][PROTOCOL_DATA_BUFFER_SIZE];
//Receive Length for 485 and 232
static uint8_t u8_SDN_RxPacketLength[NUMBER_OF_INTERFACES] = {0};
//Transmit array for 485 and 232
static uint8_t u8_SDN_TxPacket[NUMBER_OF_INTERFACES][PROTOCOL_DATA_BUFFER_SIZE];
//Transmit Length for 485 and 232
static uint8_t u8_SDN_TxPacketLength[NUMBER_OF_INTERFACES] = {0};
static bool b_SDN_SendMessageFlag[NUMBER_OF_INTERFACES] = {0};

//timers
static uint32_t u32_SDN_485BusFree_Timer = 0;
static uint32_t u32_SDN_Inactive_Timer = 0;

static bool FA_SDN_IS_IN_232_ECHO_MODE = false;
static bool FA_SDN_IS_IN_TCPIP_ECHO_MODE = false;
static t_InterfaceType FA_SDN_TCPIP_Forwarding_Interface;

uint16_t u16_fn_FA_CMD = FN_NULL;


// *****************************************************************************
// *****************************************************************************
// Section: Forward references
// *****************************************************************************
// *****************************************************************************

void FA_SDN_read485(void);
void FA_SDN_read232(void);
bool FA_SDN_CheckSumandLength(t_InterfaceType e_InterfaceType);
void Process_SDN_Timers(void);
uint32_t Random_Protocol_Response_Time(void);
void FA_SDN_AddCheckSum(t_InterfaceType e_InterfaceType);
void FA_SDN_Save_IPv4(void);
void FA_SDN_HANDLE_FA_CMD(t_InterfaceType e_InterfaceType);
void FA_SDN_readtcpip(uint8_t tcp_server_interface_selector, uint8_t tcp_client_interface_selector);
void FA_SDN_GET_DATA(t_InterfaceType e_InterfaceType);
t_USART_ID FA_SDN_Get_SenderID(t_InterfaceType e_InterfaceType);
void SDN_Unpack(uint8_t* u8_RawDataBuffer, t_SDN_message* t_DataStruct, uint8_t u8_message_length);
bool EchoMessage(t_InterfaceType e_SrcInterfaceType, t_InterfaceType e_DestInterfaceType);
// *****************************************************************************
// *****************************************************************************
// Section: fa sdn Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void FA_SDN_Initialise232 ( void )

  Remarks:
 * IF the system uses SDN RS232 then
 * Call this when Initialising the system, before using any other part
 */
void FA_SDN_Initialize_232(uint32_t USART_232_Port, USART_SERIAL_SETUP* USART_Config, bool Sys_Has_Echo) {
    //Sets the USART Port so SDN can be used on multiple channels
    FA_SDN_232_Port = USART_232_Port;
    //allows the SDN_Tasks to check the 485 message
    FA_SDN_Has_232 = true;
    //initialise the 232 port
    FA_USART_Initialize(FA_SDN_232_Port, USART_Config);
    //fa_sdn can be used to pass messages from the 232 bus to the 485 bus and back again
    //this allows the system to echo 232 messages on 485 and return the responses
    FA_SDN_Has_232_Forwarding = Sys_Has_Echo;
}

/*******************************************************************************
  Function:
    void FA_SDN_Initialise485 ( void )

  Remarks:
 * IF the system uses SDN RS485 then
 * Call this when Initialising the system, before using any other part
 */
void FA_SDN_Initialize_485(uint32_t USART_485_Port, USART_SERIAL_SETUP* USART_Config) {
    //Sets the USART Port so SDN can be used on multiple channels
    FA_SDN_485_Port = USART_485_Port;
    //allows the SDN_Tasks to check the 485 message
    FA_SDN_Has_485 = true;
    //initialise the 485 port
    FA_USART_Initialize(FA_SDN_485_Port, USART_Config);
}

void FA_SDN_Initialize_TCP(bool b_Has_Echo) {
    FA_SDN_Has_TCPIP = true;
    FA_SDN_Has_TCPIP_Forwarding = b_Has_Echo;
    FA_TCPIP_Init();
}

/*******************************************************************************
  Function:
    void FA_SDN_Initialise( void )

  Remarks:
 * IF the system uses SDN then
 * Call this when Initialising the system, before using any other part
 */
void FA_SDN_Initialize(void) {


}

/******************************************************************************
  Function:
    void FA_SDN_Tasks ( void )

  Remarks:
 * call each time through the main loop after Initialisation
 * handles the flow of the sdn task
 */

void FA_SDN_Tasks(void) {
    // Initialise as 1 minus the interface so that the first increment sets it to the correct value.
    static uint8_t tcp_server_interface_selector = Interface_TCPIP_Server_Client_1 - 1;
    static uint8_t tcp_client_interface_selector = Interface_TCPIP_Client_1 - 1;

    if (FA_SDN_Has_TCPIP) {
        // These will ensure that the interface selector values loop through only their interfaces.
        // E.g. tcp_client_interface_selector will only be 8,9,10, which are the client interface numbers.
        tcp_server_interface_selector = ((tcp_server_interface_selector - Interface_TCPIP_Server_Client_1 + 1) % NUM_SERVER_CLIENTS) + Interface_TCPIP_Server_Client_1;
        tcp_client_interface_selector = ((tcp_client_interface_selector - Interface_TCPIP_Client_1 + 1) % NUM_CLIENTS) + Interface_TCPIP_Client_1;
    }

    if (FA_SDN_Has_485) {
        FA_USART_Tasks(FA_SDN_485_Port);
        if (FA_USART_ByteTransmittedOrReceived(FA_SDN_485_Port)) {
            SDN_Start_Timers(Interface_485);
        }
    }
    if (FA_SDN_Has_232) {
        FA_USART_Tasks(FA_SDN_232_Port);
        if (FA_USART_ByteTransmittedOrReceived(FA_SDN_232_Port)) {
            SDN_Start_Timers(Interface_485);
        }
    }
    if (FA_SDN_Has_485) {
        FA_SDN_read485();
    }
    if (FA_SDN_Has_232) {
        FA_SDN_read232();
    }

    if (FA_SDN_Has_TCPIP) {
        FA_TCPIP_Tasks(u8_SDN_RxPacket, u8_SDN_RxPacketLength,
                       u8_SDN_TxPacket, u8_SDN_TxPacketLength,
                       &tcp_server_interface_selector, &tcp_client_interface_selector);
        FA_SDN_readtcpip(tcp_server_interface_selector, tcp_client_interface_selector);
    }

    Process_SDN_Timers();
}

/******************************************************************************
  Function:
    void FA_SDN_read ( void )

  Remarks:
   controls the read command and allows tasks to be more readable

 */
void FA_SDN_read485(void) {
    uint32_t u32_elapsedTime = 0;

    //get message length and fill the message holding array
    u8_SDN_RxPacketLength[Interface_485] = (uint8_t)(FA_USART_read(FA_SDN_485_Port, u8_SDN_RxPacket[Interface_485]));
    if (u8_SDN_RxPacketLength[Interface_485] > 0) {
        //if echo is activated send over 232 regardless
        if (FA_SDN_IS_IN_232_ECHO_MODE) {
            EchoMessage(Interface_485, Interface_232);
            FA_USART_send(FA_SDN_232_Port, u8_SDN_TxPacket[Interface_232], (((uint16_t)u8_SDN_TxPacketLength[Interface_232])&0x00FF));
        } else if (FA_SDN_IS_IN_TCPIP_ECHO_MODE) {
            EchoMessage(Interface_485, FA_SDN_TCPIP_Forwarding_Interface);
            FA_TCPIP_Send_Message(FA_SDN_TCPIP_Forwarding_Interface, u8_SDN_RxPacket[FA_SDN_TCPIP_Forwarding_Interface],
                                  &u8_SDN_RxPacketLength[FA_SDN_TCPIP_Forwarding_Interface]);
        } else {
            if (FA_SDN_CheckSumandLength(Interface_485) == true) {
                FA_SDN_GET_DATA(Interface_485);
                if (FA_SDN_Check_For_DeviceID(Interface_485)) {
                    if (FA_TIMER_Running(u32_SDN_485BusFree_Timer)) {
                        // If the message was targeting us and the timer is running,
                        // we need to reset the timer to expire at the normal bus free time.
                        u32_elapsedTime = FA_TIMER_get_Value(u32_SDN_485BusFree_Timer, Time_uS);
                        if (u32_elapsedTime >= SDN_BUS_FREE_TIMEOUT_mS) {
                            u32_elapsedTime = 0;
                        }
                        FA_TIMER_Start(SDN_BUS_FREE_TIMEOUT_mS - u32_elapsedTime, &u32_SDN_485BusFree_Timer, Time_mS);
                    } else {
                        // Code should not come here during normal execution. If it does, we've received the whole packet
                        // after the bus free timeout, which shouldn't be allowed, so restart the timers.
                        FA_TIMER_Start(SDN_BUS_FREE_TIMEOUT_mS, &u32_SDN_485BusFree_Timer, Time_mS);
                        FA_TIMER_Start(SDN_INACTIVE_TIMEOUT_mS, &u32_SDN_Inactive_Timer, Time_mS);
                    }
                } else if (!FA_TIMER_Running(u32_SDN_485BusFree_Timer)) {
                    SDN_Start_Timers(Interface_485);
                }
            } else {
                FA_SDN_HANDLE_FA_CMD(Interface_485);
                if (!FA_TIMER_Running(u32_SDN_485BusFree_Timer)) {
                    // Again, code should not come here during normal execution.
                    SDN_Start_Timers(Interface_485);
                }
            }
        }
    }
}

/******************************************************************************
  Function:
    void FA_SDN_read232 ( void )

  Remarks:
   controls the read command and allows tasks to be more readable

 */
void FA_SDN_read232(void) {
    //get message length and fill the message holding array
    u8_SDN_RxPacketLength[Interface_232] = (uint8_t)(FA_USART_read(FA_SDN_232_Port, u8_SDN_RxPacket[Interface_232]));
    if (u8_SDN_RxPacketLength[Interface_232] > 0) {

        if (FA_SDN_CheckSumandLength(Interface_232) == true) {
            FA_SDN_GET_DATA(Interface_232);
            if (FA_SDN_Has_232_Forwarding) {
                FA_SDN_IS_IN_232_ECHO_MODE = true;
                EchoMessage(Interface_232, Interface_485);
                FA_USART_send(FA_SDN_485_Port, u8_SDN_TxPacket[Interface_485], (((uint16_t)u8_SDN_TxPacketLength[Interface_485])&0x00FF));
            }
        } else {
            FA_SDN_HANDLE_FA_CMD(Interface_232);
        }
    }
}

/**
 * Attempts to read a client and server TCPIP message and process it if a message is avaliable.
 * If the message is not a FA message, it will bypass FA protocol and be handled
 * as an FA_x command.
 *
 * @param tcp_server_interface_selector The TCP server interface number to process.
 * @param tcp_client_interface_selector The TCP client interface number to process.
 */
void FA_SDN_readtcpip(uint8_t tcp_server_interface_selector, uint8_t tcp_client_interface_selector) {
    if (u8_SDN_RxPacketLength[tcp_server_interface_selector] > 0) {
        if (FA_SDN_CheckSumandLength(tcp_server_interface_selector) == true) {
            FA_SDN_GET_DATA(tcp_server_interface_selector);
            if (FA_SDN_Has_TCPIP_Forwarding) {
                FA_SDN_IS_IN_TCPIP_ECHO_MODE = true;
                FA_SDN_TCPIP_Forwarding_Interface = tcp_server_interface_selector;
                EchoMessage(tcp_server_interface_selector, Interface_485);
                FA_USART_send(FA_SDN_485_Port, u8_SDN_TxPacket[Interface_485], u8_SDN_TxPacketLength[Interface_485]);
            }
        } else {
            FA_SDN_HANDLE_FA_CMD(tcp_server_interface_selector);
        }
    }

    if (u8_SDN_RxPacketLength[tcp_client_interface_selector] > 0) {
        if (FA_SDN_CheckSumandLength(tcp_client_interface_selector) == true) {
            FA_SDN_GET_DATA(tcp_client_interface_selector);
            if (FA_SDN_Has_TCPIP_Forwarding) {
                FA_SDN_IS_IN_TCPIP_ECHO_MODE = true;
                FA_SDN_TCPIP_Forwarding_Interface = tcp_client_interface_selector;
                EchoMessage(tcp_client_interface_selector, Interface_485);
                FA_USART_send(FA_SDN_485_Port, u8_SDN_TxPacket[Interface_485], u8_SDN_TxPacketLength[Interface_485]);
            }
        } else {
            FA_SDN_HANDLE_FA_CMD(tcp_client_interface_selector);
        }
    }
}

/******************************************************************************
  internal Function:
    CalcSumandLength ( uint16_t MessageLength )

  Remarks:
 calculates the length and Checksum for outgoing messages

 */
bool FA_SDN_CheckSumandLength(t_InterfaceType e_InterfaceType) {
    bool bRetVal = false;
    bool bMesLength = false;
    bool bChecksumCheck = false;

    uint8_t u8_inc = 0;
    uint16_t u16_ChecksumTotal = 0;


    if (u8_SDN_RxPacketLength[e_InterfaceType] <= 31) {
        if ((FA_SDN_GetRxData(e_InterfaceType, 1) & 0x1F) == u8_SDN_RxPacketLength[e_InterfaceType]) {
            bMesLength = true;
        }
    } else if ((u8_SDN_RxPacketLength[e_InterfaceType] <= 63)&&(u8_SDN_RxPacketLength[e_InterfaceType] > 31)) {
        if ((FA_SDN_GetRxData(e_InterfaceType, 1) & 0x3F) == u8_SDN_RxPacketLength[e_InterfaceType]) {
            bMesLength = true;
        }
    }

    //calculate checksum
    for (u8_inc = 0; u8_inc < (u8_SDN_RxPacketLength[e_InterfaceType] - 2); u8_inc++) {
        u16_ChecksumTotal = u16_ChecksumTotal + (((uint16_t)(u8_SDN_RxPacket[e_InterfaceType][u8_inc]))&0x00FF);
    }

    if (((uint8_t)(((u16_ChecksumTotal >> 8) & 0x00FF)) == u8_SDN_RxPacket[e_InterfaceType][u8_inc]) &&
        (((uint8_t)(u16_ChecksumTotal & 0x00FF)) == u8_SDN_RxPacket[e_InterfaceType][u8_inc + 1])) {
        bChecksumCheck = true;
    }

    if ((bMesLength == true) && (bChecksumCheck == true)) {
        bRetVal = true;
    }//send nack if being targeted and fault condition
    else if (FA_SDN_Check_For_DeviceID(e_InterfaceType)) {
        if (bMesLength == false) {
            FA_SDN_Send_Nack(e_InterfaceType, SDN_ERROR_LENGTH);
        } else if (bChecksumCheck == false) {
            FA_SDN_Send_Nack(e_InterfaceType, SDN_ERROR_CHECKSUM);
        }
    }

    return bRetVal;
}

/******************************************************************************
  internal Function:
    FA_SDN_EchoBuild ( uint16_t MessageLength )

  Remarks:
 builds the echo message to be sent over the other interface

 */
bool EchoMessage(t_InterfaceType e_SrcInterfaceType, t_InterfaceType e_DestInterfaceType) {
    bool b_RetVal = false;

    if (u8_SDN_RxPacketLength[e_SrcInterfaceType] <= PROTOCOL_DATA_BUFFER_SIZE) {
        memcpy(u8_SDN_TxPacket[e_DestInterfaceType], u8_SDN_RxPacket[e_SrcInterfaceType], u8_SDN_RxPacketLength[e_SrcInterfaceType]);
        u8_SDN_TxPacketLength[e_DestInterfaceType] = u8_SDN_RxPacketLength[e_SrcInterfaceType];
        b_RetVal = true;
    }
    return b_RetVal;
}

/******************************************************************************
  internal Function:
    handles timers

  Remarks:
 checks if each timer has overflowed and changes the state accordingly
 */
void Process_SDN_Timers(void) {
    //send 485 when bus is free
    if (FA_TIMER_Has_Finished(&u32_SDN_485BusFree_Timer)) {
        if (b_SDN_SendMessageFlag[Interface_485] == true) {
            FA_USART_send(FA_SDN_485_Port, u8_SDN_TxPacket[Interface_485], u8_SDN_TxPacketLength[Interface_485]);
            b_SDN_SendMessageFlag[Interface_485] = false;
        }
    } else if (b_SDN_SendMessageFlag[Interface_485] == true && !FA_TIMER_Running(u32_SDN_485BusFree_Timer)) {
        // If the send flag was set to true and the bus free timer isnt running then just send the message straight away
        // because the timer should have expired. This si needed for the messages that originate from IR to work since
        // IR has no incoming message that would have caused the bus free timer to have expired to indicate that we need to send a message.
        FA_USART_send(FA_SDN_485_Port, u8_SDN_TxPacket[Interface_485], u8_SDN_TxPacketLength[Interface_485]);
        b_SDN_SendMessageFlag[Interface_485] = false;
    }
    //send 232 asap
    if (b_SDN_SendMessageFlag[Interface_232] == true) {
        FA_USART_send(FA_SDN_232_Port, u8_SDN_TxPacket[Interface_232], u8_SDN_TxPacketLength[Interface_232]);
        b_SDN_SendMessageFlag[Interface_232] = false;
    }
    //save vals on system timeout
    if (FA_TIMER_Has_Finished(&u32_SDN_Inactive_Timer)) {
        FA_APP_InactiveTimeOut();
        FA_SDN_IS_IN_232_ECHO_MODE = false;
        FA_SDN_IS_IN_TCPIP_ECHO_MODE = false;
    }
}

/**
 * Generates a random number within the Protocols Min and Max response time, used for staggering
 * responses to broadcast messages to avoid bus contention.
 * @return Random number between min and max, in microseconds.
 */
uint32_t Random_Protocol_Response_Time(void) {
    // Rand is seeded via the mac address.
    return ((rand()) % (SDN_MIN_RESPONSE_TIME_mS - SDN_MIN_RESPONSE_TIME_mS + 1)) +SDN_MIN_RESPONSE_TIME_mS;
}

/******************************************************************************
  internal Functions:
    start timers

  Remarks:
 * All SDN protocol timers are in fa_sdn regardless if working as a motor or keypad
 *
 checks if timer is running, if so it restarts the timer
 if not then it starts a new timer and saves the timer number
 */

void Start_SDN_Timer(t_FlowControlTimers SelectStartTimer) {
    switch (SelectStartTimer) {
        case Timer_Inactive: FA_TIMER_Start(SDN_INACTIVE_TIMEOUT_mS, &u32_SDN_Inactive_Timer, Time_mS);
            break;
        case Timer_485_BusFree: FA_TIMER_Start(SDN_BUS_FREE_TIMEOUT_mS, &u32_SDN_485BusFree_Timer, Time_mS);
            break;
        case Timer_485_BusFreeRand: FA_TIMER_Start(Random_Protocol_Response_Time(), &u32_SDN_485BusFree_Timer, Time_mS);
            break;
        default:
            // Do nothing
            break;
    }
}

/******************************************************************************
  external Function:
    start timers

  Remarks:
 * this function makes it easier to start timers for the protocol when they are required
 * do a random response time if the message is a broadcast message
 */
void SDN_Start_Timers(t_InterfaceType e_InterfaceType) {
    if (e_InterfaceType == Interface_485) {
        Start_SDN_Timer(Timer_485_BusFreeRand);
        Start_SDN_Timer(Timer_Inactive);
    } else {
        Start_SDN_Timer(Timer_Inactive);
    }

}

/******************************************************************************
  External Function:
    CheckForAddress

  Remarks: see if the target is us or a broadcast message

 */
bool FA_SDN_Check_For_DeviceID_Or_Broadcast(t_InterfaceType e_InterfaceType) {
    bool bRetVal = false;

    //targeting for individual motors
    if (FA_SDN_Check_For_DeviceID(e_InterfaceType)) {
        bRetVal = true;
    }//targeting for broadcast message
    else if (FA_SDN_Check_For_BroadcastID(e_InterfaceType)) {
        bRetVal = true;
    }

    return bRetVal;
}

/******************************************************************************
  External Function:
    FA_SDN_Check_for_DeviceID

  Remarks: check for our ID

 */
bool FA_SDN_Check_For_DeviceID(t_InterfaceType e_InterfaceType) {
    t_USART_ID Target_ID;

    //to do store target address here
    Target_ID.u8_USART_ID_1 = FA_SDN_GetRxData(e_InterfaceType, 8);
    Target_ID.u8_USART_ID_2 = FA_SDN_GetRxData(e_InterfaceType, 7);
    Target_ID.u8_USART_ID_3 = FA_SDN_GetRxData(e_InterfaceType, 6);

    return FA_PROTOCOL_COMMON_Check_For_Device_ID(Target_ID);
}

/******************************************************************************
  External Function:
    FA_SDN_Check_for_BroadcastID

  Remarks: check for broadcast ID

 */
bool FA_SDN_Check_For_BroadcastID(t_InterfaceType e_InterfaceType) {
    t_USART_ID Targit_ID;

    Targit_ID.u8_USART_ID_1 = FA_SDN_GetRxData(e_InterfaceType, 8);
    Targit_ID.u8_USART_ID_2 = FA_SDN_GetRxData(e_InterfaceType, 7);
    Targit_ID.u8_USART_ID_3 = FA_SDN_GetRxData(e_InterfaceType, 6);

    return FA_PROTOCOL_COMMON_Check_For_Broadcast_ID(Targit_ID);
}

/******************************************************************************
  External Function:
    FA_SDN_Build_Message

  Remarks: this builds a message with the current board as the sender

 */
void FA_SDN_SEND_FA_232(uint8_t * u8_MessageToSend, uint8_t FlashLength) {
    b_SDN_SendMessageFlag[Interface_232] = true;

    for (uint8_t i = 0; i < FlashLength; i++) {
        //u8_SDN_TxPacket[Interface_232][i] = (~u8_MessageToSend[i] & 0x00FF);
        u8_SDN_TxPacket[Interface_232][i] = u8_MessageToSend[i];
    }
    u8_SDN_TxPacketLength[Interface_232] = FlashLength;
}

/******************************************************************************
  internal Function:
    ADD Checksum ( uint16_t MessageLength )

  Remarks:
 calculates the length and Checksum for outgoing messages

 */
void FA_SDN_AddCheckSum(t_InterfaceType e_InterfaceType) {
    uint16_t u16_ChecksumTotal = 0;

    //calculate checksum
    for (uint8_t u8_inc = 0; u8_inc < (u8_SDN_TxPacketLength[e_InterfaceType] - 2); u8_inc++) {
        u16_ChecksumTotal = u16_ChecksumTotal + u8_SDN_TxPacket[e_InterfaceType][u8_inc];
    }

    u8_SDN_TxPacket[e_InterfaceType][u8_SDN_TxPacketLength[e_InterfaceType] - 2] = (uint8_t)((u16_ChecksumTotal >> 8) & 0x00FF);
    u8_SDN_TxPacket[e_InterfaceType][u8_SDN_TxPacketLength[e_InterfaceType] - 1] = (uint8_t)(u16_ChecksumTotal & 0x00FF);
}

uint8_t FA_SDN_GetRxData(t_InterfaceType e_InterfaceType, uint8_t u8_DataByte) {
    return (~(u8_SDN_RxPacket[e_InterfaceType][u8_DataByte]) & 0x00FF);
}

bool FA_SDN_Check_ForResponseBit(t_InterfaceType e_InterfaceType) {
    bool bRetval = false;
    // if (((FA_SDN_GetRxData(e_InterfaceType, 1)&0x80)==1)){
    bRetval = true;
    //}
    return bRetval;
}

void FA_SDN_HANDLE_FA_CMD(t_InterfaceType e_InterfaceType) {
    uint8_t ack[7] = {'f', 'a', '_', 'a', 'c', 'k', '\r'};
    uint8_t nak[7] = {'f', 'a', '_', 'n', 'a', 'k', '\r'};
    t_fa_packet_data command_packet;
    uint16_t CurrentCMD = GLB_NULL_INT;

    if (FA_PACKET_convert_fa_rs232(u8_SDN_RxPacket[e_InterfaceType], u8_SDN_RxPacketLength[e_InterfaceType], &command_packet)) {
        u16_fn_FA_CMD = FA_PACKET_extract_fn(&command_packet);
        b_SDN_SendMessageFlag[e_InterfaceType] = true;
        for (uint8_t i = 0; i < 7; i++) {
            u8_SDN_TxPacket[e_InterfaceType][i] = (u16_fn_FA_CMD == FN_NULL) ? nak[i] : ack[i];
        }
        u8_SDN_TxPacketLength[e_InterfaceType] = 7;
    } else if (FA_PACKET_convert_sdn_button(u8_SDN_RxPacket[e_InterfaceType], u8_SDN_RxPacketLength[e_InterfaceType], &command_packet)) {
        CurrentCMD = FA_PACKET_extract_sdn_button(&command_packet);
        if (CurrentCMD != GLB_NULL_INT) {
            FA_APP_HandleButtonCMD(CurrentCMD);
        }
    }
}

uint16_t FA_SDN_GET_FA_CMD(void) {
    uint16_t u16_RetVal = u16_fn_FA_CMD;
    u16_fn_FA_CMD = FN_NULL;
    return u16_RetVal;
}

void FA_SDN_GET_DATA(t_InterfaceType e_InterfaceType) {
    t_SDN_message t_RxData = {0};

    SDN_Unpack(u8_SDN_RxPacket[e_InterfaceType], &t_RxData, u8_SDN_RxPacketLength[e_InterfaceType]);
    FA_APP_HANDLE_SDN_MESSAGE(e_InterfaceType, &t_RxData);
}

void SDN_Unpack(uint8_t* u8_RawDataBuffer, t_SDN_message* t_DataStruct, uint8_t u8_message_length) {
    uint16_t u16_PayloadSize = 0;

    t_DataStruct->MessageType = ~u8_RawDataBuffer[0];
    t_DataStruct->Ack = (bool)(~u8_RawDataBuffer[1] & TOP_BIT_BM);

    // The reserved EXT part of byte 2 is used if the length exceeds 31.
    // Don't take the EXT part if the size is less than 31 just incase it
    // contains garbage data, otherwise, also use the EXT part.
    if (u8_message_length <= SDN_MSG_LENGTH_BM) {
        t_DataStruct->Length = (~u8_RawDataBuffer[1]) & SDN_MSG_LENGTH_BM;
    } else if ((u8_message_length <= SDN_MSG_LENGTH_EXT_L_BM) && (u8_message_length > SDN_MSG_LENGTH_BM)) {
        t_DataStruct->Length = (~u8_RawDataBuffer[1]) & SDN_MSG_LENGTH_EXT_L_BM;
    }

    t_DataStruct->SourceNodeType = (uint8_t)(~u8_RawDataBuffer[2] & SDN_MSG_SOURCE_NODE_TYPE_BM) >> 4;
    t_DataStruct->DestNodeType = (uint8_t)(~u8_RawDataBuffer[2] & SDN_MSG_DEST_NODE_TYPE_BM);
    t_DataStruct->Source.u8_USART_ID_1 = ~u8_RawDataBuffer[3];
    t_DataStruct->Source.u8_USART_ID_2 = ~u8_RawDataBuffer[4];
    t_DataStruct->Source.u8_USART_ID_3 = ~u8_RawDataBuffer[5];
    t_DataStruct->Dest.u8_USART_ID_1 = ~u8_RawDataBuffer[6];
    t_DataStruct->Dest.u8_USART_ID_2 = ~u8_RawDataBuffer[7];
    t_DataStruct->Dest.u8_USART_ID_3 = ~u8_RawDataBuffer[8];

    u16_PayloadSize = t_DataStruct->Length - SDN_MSG_NO_PAYLOAD;
    if (u16_PayloadSize > 0) {
        for (int i = 0; i < u16_PayloadSize; i++) {
            t_DataStruct->Data[i] = ~u8_RawDataBuffer[9 + i];
        }
    }
}

void FA_SDN_Send_Message(t_InterfaceType e_InterfaceType, uint8_t MessageType, bool Ack, uint8_t Length, t_USART_ID* source, t_USART_ID* target, uint8_t* Data) {
    uint16_t u16_PayloadSize = 0;
    uint8_t deviceID[DEVICE_ID_LENGTH];
    t_USART_ID replyTarget;
    uint16_t nodeType;


    FA_PROTOCOL_COMMON_Get_Device_ID(deviceID);
    replyTarget = FA_SDN_Get_SenderID(e_InterfaceType);

    u8_SDN_TxPacket[e_InterfaceType][0] = ~MessageType;
    u8_SDN_TxPacket[e_InterfaceType][1] = ~(Ack << 7);
    if (Length <= SDN_MSG_LENGTH_BM) {
        u8_SDN_TxPacket[e_InterfaceType][1] = ~(Length & SDN_MSG_LENGTH_BM);
    } else if ((Length <= SDN_MSG_LENGTH_EXT_L_BM) && (Length > SDN_MSG_LENGTH_BM)) {
        u8_SDN_TxPacket[e_InterfaceType][1] = ~(Length & SDN_MSG_LENGTH_EXT_L_BM);
    }

    // If we are sending a motor move command we have to change our node type to 0
    // since we are the master commanding a move.
    if (MessageType == Move_Pulse_Down || MessageType == Move_Stop ||
        MessageType == Move_Three || MessageType == Move_Four ||
        MessageType == Move_Visual_Feedback || MessageType == Set_Motor_Internal_Position) {
        nodeType = 0;

    } else {
        // Otherwise use our actual node type.
        nodeType = FA_APP_GetNodeType();
    }

    u8_SDN_TxPacket[e_InterfaceType][2] = ~nodeType;

    if (source == NULL) {
        u8_SDN_TxPacket[e_InterfaceType][3] = ~deviceID[2];
        u8_SDN_TxPacket[e_InterfaceType][4] = ~deviceID[1];
        u8_SDN_TxPacket[e_InterfaceType][5] = ~deviceID[0];
    } else {
        u8_SDN_TxPacket[e_InterfaceType][3] = ~source->u8_USART_ID_1;
        u8_SDN_TxPacket[e_InterfaceType][4] = ~source->u8_USART_ID_2;
        u8_SDN_TxPacket[e_InterfaceType][5] = ~source->u8_USART_ID_3;
    }

    if (target == NULL) {
        u8_SDN_TxPacket[e_InterfaceType][6] = ~replyTarget.u8_USART_ID_1;
        u8_SDN_TxPacket[e_InterfaceType][7] = ~replyTarget.u8_USART_ID_2;
        u8_SDN_TxPacket[e_InterfaceType][8] = ~replyTarget.u8_USART_ID_3;
    } else {
        u8_SDN_TxPacket[e_InterfaceType][6] = ~target->u8_USART_ID_1;
        u8_SDN_TxPacket[e_InterfaceType][7] = ~target->u8_USART_ID_2;
        u8_SDN_TxPacket[e_InterfaceType][8] = ~target->u8_USART_ID_3;
    }

    u16_PayloadSize = Length - SDN_MSG_NO_PAYLOAD;
    if (u16_PayloadSize > 0) {
        for (int i = 0; i < u16_PayloadSize; i++) {
            u8_SDN_TxPacket[e_InterfaceType][9 + i] = ~Data[i];
        }
    }

    u8_SDN_TxPacketLength[e_InterfaceType] = Length;
    FA_SDN_AddCheckSum(e_InterfaceType);
    b_SDN_SendMessageFlag[e_InterfaceType] = true;
    LOG_VERBOSE("FA_SDN_Send_Message - Message sent!\r\n");
}

void FA_SDN_Send_Ack(t_InterfaceType e_InterfaceType) {

    FA_SDN_Send_Message(e_InterfaceType, SDN_ACK, false, SDN_MSG_NO_PAYLOAD, NULL, NULL, NULL);
}

void FA_SDN_Send_Nack(t_InterfaceType e_InterfaceType, uint8_t u8_Error_Code) {

    uint8_t u8_data[1] = {u8_Error_Code};
    FA_SDN_Send_Message(e_InterfaceType, SDN_NACK, false, SDN_MSG_NO_PAYLOAD + 1, NULL, NULL, u8_data);
}

t_USART_ID FA_SDN_Get_SenderID(t_InterfaceType e_InterfaceType) {
    t_USART_ID Sender_ID;

    // to do store target address here
    Sender_ID.u8_USART_ID_1 = ~u8_SDN_RxPacket[e_InterfaceType][3];
    Sender_ID.u8_USART_ID_2 = ~u8_SDN_RxPacket[e_InterfaceType][4];
    Sender_ID.u8_USART_ID_3 = ~u8_SDN_RxPacket[e_InterfaceType][5];

    return Sender_ID;
}
