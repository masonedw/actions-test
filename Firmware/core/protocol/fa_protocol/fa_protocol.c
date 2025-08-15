/*******************************************************************************
  Future Automation Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    fa_protocol.c

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
#include "fa_protocol.h"
#include "fa_timer.h"
#include "fa_app.h"
#include "fa_at24.h"
#include "fa_tcpip.h"
#include "fa_crc.h"
#include "protocol_common.h"
#include "fa_protocol_cmd_list.h"
#include "fn_codes.h"
// #include "fa_usart.h"
// #include "fa_tcpip.h"
// #include "framework.h"
//  *****************************************************************************
//  *****************************************************************************
//  Section: Macros
//  *****************************************************************************
//  *****************************************************************************
#define RESPONSE_BIT_BM 0x80

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

static uint32_t FA_Protocol_232_Port = 0;
static bool FA_Protocol_Has_232 = false;
static uint32_t FA_Protocol_485_Port = 0;
static bool FA_Protocol_Has_485 = false;
static bool FA_Protocol_Has_TCPIP = false;
static bool FA_Protocol_Has_TCPIP_Forwarding = false;
static bool FA_Protocol_Is_In_TCPIP_Echo_Mode = false;
static bool FA_Protocol_Has_232_Forwarding = false;
static bool FA_Protocol_Is_In_232_Echo_Mode = false;
static t_InterfaceType FA_Protocol_TCPIP_Forwarding_Interface;

// Data Storage
// Receive array for 485 and 232
static uint8_t u8_FAP_RxPacket[NUMBER_OF_INTERFACES][PROTOCOL_DATA_BUFFER_SIZE];
// Receive Length for 485 and 232
static uint8_t u8_FAP_RxPacketLength[NUMBER_OF_INTERFACES] = {0};
// Transmit array for 485 and 232
static uint8_t u8_FAP_TxPacket[NUMBER_OF_INTERFACES][PROTOCOL_DATA_BUFFER_SIZE];
// Transmit Length for 485 and 232
static uint8_t u8_FAP_TxPacketLength[NUMBER_OF_INTERFACES] = {0};

static bool b_FAP_SendMessageFlag[NUMBER_OF_INTERFACES] = {0};
// timers
static uint32_t u32_FAP_485BusFree_Timer = 0;
static uint32_t u32_FAP_Inactive_Timer = 0;
// static uint32_t u32_FAP_TCPIPInactive_Timer = 0;

uint16_t u16_fn_FA_CMD = FN_NULL;

// *****************************************************************************
// *****************************************************************************
// Section: Forward references
// *****************************************************************************
// *****************************************************************************

void FA_PROTOCOL_read485(void);
void FA_PROTOCOL_read232(void);
void FA_PROTOCOL_readtcpip(uint8_t tcp_server_interface_selector, uint8_t tcp_client_interface_selector);
void FA_PROTOCOL_GET_DATA(t_InterfaceType e_InterfaceType);
bool FA_PROTOCOL_CheckSumandLength(t_InterfaceType e_InterfaceType);
void Process_FA_Timers(void);
uint32_t Random_Protocol_Response_Time(void);
// void Handle_FA_Timers(t_TimerEvent eTimerEvent, t_InterfaceType e_InterfaceType);
void Start_FA_Timer(t_FlowControlTimers SelectStartTimer);
void FA_PROTOCOL_AddCheckSum(t_InterfaceType e_InterfaceType);
void FA_PROTOCOL_Pack(t_fa_protocol_message* t_DataStruct, uint8_t *u8_RawDataBuffer);
void FA_PROTOCOL_Pack_Common_Data(t_InterfaceType e_InterfaceType, t_fa_protocol_message* t_DataStruct);
void FA_PROTOCOL_Unpack(uint8_t* buf, t_fa_protocol_message* t_DataStruct);
void FA_PROTOCOL_Handle_FA_Command(t_InterfaceType e_InterfaceType);
bool EchoMessage(t_InterfaceType e_SrcInterfaceType, t_InterfaceType e_DestInterfaceType);
// *****************************************************************************
// *****************************************************************************
// Section: fa protocol Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void FA_PROTOCOL_Initialise ( void )

  Remarks:
    See prototype in fa_sdn.h.
 * Call this when Initialising the system, before using any other part
 */
void FA_PROTOCOL_Initialize_232(uint32_t USART_232_Port, USART_SERIAL_CONFIG* USART_Config, bool b_Has_Echo) {
    FA_Protocol_232_Port = USART_232_Port;
    FA_Protocol_Has_232 = true;
    FA_Protocol_Has_232_Forwarding = b_Has_Echo;
    FA_USART_Initialize(FA_Protocol_232_Port, USART_Config);

}

void FA_PROTOCOL_Initialize_485(uint32_t USART_485_Port, USART_SERIAL_CONFIG* USART_Config) {
    FA_Protocol_485_Port = USART_485_Port;
    FA_Protocol_Has_485 = true;
    FA_USART_Initialize(FA_Protocol_485_Port, USART_Config);
}

void FA_PROTOCOL_Initialize_TCPIP(bool b_Has_Echo) {
    // Initialise TCP stack.
    //SYS_InitialiseTCP();
    FA_Protocol_Has_TCPIP_Forwarding = b_Has_Echo;
    FA_Protocol_Has_TCPIP = true;
    FA_TCPIP_Init();
}

/*******************************************************************************
  Function:
    void FA_SDN_Initialise( void )

  Remarks:
 * IF the system uses SDN then
 * Call this when Initialising the system, before using any other part
 */
void FA_PROTOCOL_Initialize(void) {
}

/******************************************************************************
  Function:
    void FA_PROTOCOL_Tasks ( void )

  Remarks:
 * call each time through the main loop after Initialisation
 * handles the flow of the sdn task
 */

void FA_PROTOCOL_Tasks(void) {
    // Initialise as 1 minus the interface so that the first increment sets it to the correct value.
    static uint8_t tcp_server_interface_selector = Interface_TCPIP_Server_Client_1 - 1;
    static uint8_t tcp_client_interface_selector = Interface_TCPIP_Client_1 - 1;

    if (FA_Protocol_Has_TCPIP) {
        // These will ensure that the interface selector values loop through only their interfaces.
        // E.g. tcp_client_interface_selector will only be 8,9,10, which are the client interface numbers.
        tcp_server_interface_selector = ((tcp_server_interface_selector - Interface_TCPIP_Server_Client_1 + 1) % NUM_SERVER_CLIENTS) + Interface_TCPIP_Server_Client_1;
        tcp_client_interface_selector = ((tcp_client_interface_selector - Interface_TCPIP_Client_1 + 1) % NUM_CLIENTS) + Interface_TCPIP_Client_1;
    }
    if (FA_Protocol_Has_485) {
        FA_USART_Tasks(FA_Protocol_485_Port);
        if (FA_USART_ByteTransmittedOrReceived(FA_Protocol_485_Port)) {
            FA_PROTOCOL_Start_Timers(Interface_485);
        }
    }

    if (FA_Protocol_Has_232) {
        FA_USART_Tasks(FA_Protocol_232_Port);
        if (FA_USART_ByteTransmittedOrReceived(FA_Protocol_232_Port)) {
            FA_PROTOCOL_Start_Timers(Interface_232);
        }
    }

    if (FA_Protocol_Has_485) {
        FA_PROTOCOL_read485();
    }

    if (FA_Protocol_Has_232) {
        FA_PROTOCOL_read232();
    }

    if (FA_Protocol_Has_TCPIP) {
        FA_TCPIP_Tasks(u8_FAP_RxPacket, u8_FAP_RxPacketLength,
                       u8_FAP_TxPacket, u8_FAP_TxPacketLength,
                       &tcp_server_interface_selector, &tcp_client_interface_selector);
        FA_PROTOCOL_readtcpip(tcp_server_interface_selector, tcp_client_interface_selector);
    }


    Process_FA_Timers();
}

/**
 * Attempts to read a RS485 message and process it if a message is avaliable.
 * If the message is not a FA message, it will bypass FA protocol and be handled
 * as an FA_x command.
 */
void FA_PROTOCOL_read485(void) {
    uint32_t elapsedTime = 0;

    // get message length and fill the message holding array
    u8_FAP_RxPacketLength[Interface_485] = FA_USART_read(FA_Protocol_485_Port, u8_FAP_RxPacket[Interface_485]);
    if (u8_FAP_RxPacketLength[Interface_485] > 0) {
        if (FA_Protocol_Is_In_TCPIP_Echo_Mode) {
            EchoMessage(Interface_485, FA_Protocol_TCPIP_Forwarding_Interface);
            FA_TCPIP_Send_Message(FA_Protocol_TCPIP_Forwarding_Interface, u8_FAP_RxPacket[FA_Protocol_TCPIP_Forwarding_Interface],
                                  &u8_FAP_RxPacketLength[FA_Protocol_TCPIP_Forwarding_Interface]);
        } else if (FA_Protocol_Is_In_232_Echo_Mode) {
            EchoMessage(Interface_485, Interface_232);
            FA_USART_send(FA_Protocol_232_Port, u8_FAP_TxPacket[Interface_232], u8_FAP_TxPacketLength[Interface_232]);
        } else if (FA_PROTOCOL_CheckSumandLength(Interface_485) == true) {
            FA_PROTOCOL_GET_DATA(Interface_485);
            if (FA_PROTOCOL_Check_For_DeviceID(Interface_485)) {
                if (FA_TIMER_Running(u32_FAP_485BusFree_Timer)) {
                    // If the message was targeting us and the timer is running,
                    // we need to reset the timer to expire at the normal bus free time.
                    elapsedTime = FA_TIMER_get_Value(u32_FAP_485BusFree_Timer, Time_uS);
                    if (elapsedTime >= FA_PROTOCOL_BUS_FREE_TIMEOUT_uS) {
                        elapsedTime = 0;
                    }
                    FA_TIMER_Start(FA_PROTOCOL_BUS_FREE_TIMEOUT_uS - elapsedTime, &u32_FAP_485BusFree_Timer, Time_uS);
                } else {
                    // Code should not come here during normal execution. If it does, we've received the whole FA packet
                    // after the bus free timeout, which shouldn't be allowed, so restart the timers.
                    FA_TIMER_Start(FA_PROTOCOL_BUS_FREE_TIMEOUT_uS, &u32_FAP_485BusFree_Timer, Time_uS);
                    FA_TIMER_Start(FA_PROTOCOL_INACTIVE_TIMEOUT_mS, &u32_FAP_Inactive_Timer, Time_mS);
                }
            } else if (!FA_TIMER_Running(u32_FAP_485BusFree_Timer)) {
                FA_PROTOCOL_Start_Timers(Interface_485);
            }
        } else {
            FA_PROTOCOL_Handle_FA_Command(Interface_485);
            if (!FA_TIMER_Running(u32_FAP_485BusFree_Timer)) {
                // Again, code should not come here during normal execution.
                FA_PROTOCOL_Start_Timers(Interface_485);
            }
        }
    }
}

/**
 * Attempts to read a RS232 message and process it if a message is avaliable.
 * If the message is not a FA message, it will bypass FA protocol and be handled
 * as an FA_x command.
 */
void FA_PROTOCOL_read232(void) {
    // get message length and fill the message holding array
    u8_FAP_RxPacketLength[Interface_232] = FA_USART_read(FA_Protocol_232_Port, u8_FAP_RxPacket[Interface_232]);
    if (u8_FAP_RxPacketLength[Interface_232] > 0) {
        if (FA_PROTOCOL_CheckSumandLength(Interface_232) == true) {
            FA_PROTOCOL_GET_DATA(Interface_232);
            if (FA_Protocol_Has_232_Forwarding) {
                FA_Protocol_Is_In_232_Echo_Mode = true;
                EchoMessage(Interface_232, Interface_485);
                FA_USART_send(FA_Protocol_485_Port, u8_FAP_TxPacket[Interface_485], u8_FAP_TxPacketLength[Interface_485]);
            }
        } else {
            FA_PROTOCOL_Handle_FA_Command(Interface_232);
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
void FA_PROTOCOL_readtcpip(uint8_t tcp_server_interface_selector, uint8_t tcp_client_interface_selector) {
    if (u8_FAP_RxPacketLength[tcp_server_interface_selector] > 0) {
        if (FA_PROTOCOL_CheckSumandLength(tcp_server_interface_selector) == true) {
            FA_PROTOCOL_GET_DATA(tcp_server_interface_selector);
            if (FA_Protocol_Has_TCPIP_Forwarding) {
                FA_Protocol_Is_In_TCPIP_Echo_Mode = true;
                FA_Protocol_TCPIP_Forwarding_Interface = tcp_server_interface_selector;
                EchoMessage(tcp_server_interface_selector, Interface_485);
                FA_USART_send(FA_Protocol_485_Port, u8_FAP_TxPacket[Interface_485], u8_FAP_TxPacketLength[Interface_485]);
            }
        } else {
            FA_PROTOCOL_Handle_FA_Command(tcp_server_interface_selector);
        }
    }

    if (u8_FAP_RxPacketLength[tcp_client_interface_selector] > 0) {
        if (FA_PROTOCOL_CheckSumandLength(tcp_client_interface_selector) == true) {
            FA_PROTOCOL_GET_DATA(tcp_client_interface_selector);
            if (FA_Protocol_Has_TCPIP_Forwarding) {
                FA_Protocol_Is_In_TCPIP_Echo_Mode = true;
                EchoMessage(tcp_client_interface_selector, Interface_485);
                FA_USART_send(FA_Protocol_485_Port, u8_FAP_TxPacket[Interface_485], u8_FAP_TxPacketLength[Interface_485]);
            }
        } else {
            FA_PROTOCOL_Handle_FA_Command(tcp_client_interface_selector);
        }
    }
}

/******************************************************************************
Function:
void FA_PROTOCOL_GET_DATA(t_InterfaceType e_InterfaceType){

Remarks:
Gets the received data and inserts it into a receive data structure,
then calls the function to process the received data.

 */
void FA_PROTOCOL_GET_DATA(t_InterfaceType e_InterfaceType) {
    t_fa_protocol_message t_RxData;

    FA_PROTOCOL_Unpack(u8_FAP_RxPacket[e_InterfaceType], &t_RxData);
    FA_APP_HANDLE_FA_PROTOCOL_MESSAGE(e_InterfaceType, &t_RxData);
}

/******************************************************************************
internal Function:
CalcSumandLength ( uint16_t MessageLength )

Remarks:
calculates the length and Checksum for outgoing messages

 */
bool FA_PROTOCOL_CheckSumandLength(t_InterfaceType e_InterfaceType) {
    bool bRetVal = false;
    bool bMesLength = false;
    uint16_t u16_MesLength = 0;
    bool bChecksumCheck = false;
    uint16_t u16_CalculatedCrc = 0xFFFF;


    u16_MesLength = u8_FAP_RxPacket[e_InterfaceType][1] & REMAINING_BITS_BM;

    if (u16_MesLength == u8_FAP_RxPacketLength[e_InterfaceType]) {
        bMesLength = true;
    }

    u16_CalculatedCrc = crc16_ccitt(u8_FAP_RxPacket[e_InterfaceType], u16_MesLength);

    // If we CRC the whole message (including the CRC part of the message),
    // the CRC calculation will equal 0 if there were no errors detected.
    if (u16_CalculatedCrc == 0) {
        bChecksumCheck = true;
    }

    if ((bMesLength == true) && (bChecksumCheck == true)) {

        bRetVal = true;
    }
    return bRetVal;
}

/******************************************************************************
internal Function:
handles timers

Remarks:
checks if each timer has overflowed and changes the state accordingly
 */
void Process_FA_Timers(void) {

    if (FA_TIMER_Has_Finished(&u32_FAP_485BusFree_Timer)) {
        if (b_FAP_SendMessageFlag[Interface_485] == true) {
            FA_USART_send(FA_Protocol_485_Port, u8_FAP_TxPacket[Interface_485], u8_FAP_TxPacketLength[Interface_485]);
            b_FAP_SendMessageFlag[Interface_485] = false;
        }
    } else if (b_FAP_SendMessageFlag[Interface_485] == true && !FA_TIMER_Running(u32_FAP_485BusFree_Timer)) {
        // If the send flag was set to true and the bus free timer isnt running then just send the message straight away
        // because the timer should have expired. This is needed for the messages that originate from IR to work since
        // IR has no incoming message that would have caused the bus free timer to have expired to indicate that we need to send a message.
        FA_USART_send(FA_Protocol_485_Port, u8_FAP_TxPacket[Interface_485], u8_FAP_TxPacketLength[Interface_485]);
        b_FAP_SendMessageFlag[Interface_485] = false;
    }

    if (b_FAP_SendMessageFlag[Interface_232] == true) {
        FA_USART_send(FA_Protocol_232_Port, u8_FAP_TxPacket[Interface_232], u8_FAP_TxPacketLength[Interface_232]);
        b_FAP_SendMessageFlag[Interface_232] = false;
    }

    // save vals on system timeout
    if (FA_TIMER_Has_Finished(&u32_FAP_Inactive_Timer)) {
        FA_APP_FAP_InactiveTimeOut();
        FA_Protocol_Is_In_TCPIP_Echo_Mode = false;
        FA_Protocol_Is_In_232_Echo_Mode = false;
    }
}

/**
 * Generates a random number within the Protocols Min and Max response time, used for staggering
 * responses to broadcast messages to avoid bus contention.
 * @return Random number between PROTOCOL_MIN_RESPONSE_TIME_uS and PROTOCOL_MAX_RESPONSE_TIME_uS, in microseconds.
 */
uint32_t Random_Protocol_Response_Time(void) {
    // Rand is seeded via the mac address.
    return ((rand() * 1000) % (FA_PROTOCOL_MAX_RESPONSE_TIME_uS - FA_PROTOCOL_MIN_RESPONSE_TIME_uS + 1)) +FA_PROTOCOL_MIN_RESPONSE_TIME_uS;
}

/******************************************************************************
internal Functions:
start timers

Remarks:
checks if timer is running, if so it restarts the timer
if not then it starts a new timer and saves the timer number
 */

void Start_FA_Timer(t_FlowControlTimers SelectStartTimer) {
    switch (SelectStartTimer) {
        case Timer_485_BusFree:
            FA_TIMER_Start(FA_PROTOCOL_BUS_FREE_TIMEOUT_uS, &u32_FAP_485BusFree_Timer, Time_uS);
            break;
        case Timer_485_BusFreeRand:
            FA_TIMER_Start(Random_Protocol_Response_Time(), &u32_FAP_485BusFree_Timer, Time_uS);
            break;
        case Timer_Inactive:
            FA_TIMER_Start(FA_PROTOCOL_INACTIVE_TIMEOUT_mS, &u32_FAP_Inactive_Timer, Time_mS);
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
void FA_PROTOCOL_Start_Timers(t_InterfaceType e_InterfaceType) {
    if (e_InterfaceType == Interface_485) {
        Start_FA_Timer(Timer_485_BusFreeRand);
        Start_FA_Timer(Timer_Inactive);
    } else {
        Start_FA_Timer(Timer_Inactive);
    }
}

/******************************************************************************
External Function:
bool FA_PROTOCOL_CheckForAddress(t_InterfaceType e_InterfaceType);

Remarks:
Returns true if the received message is targeted to this device.
It checks for the device ID or if the message was a broadcast message.

 */
bool FA_PROTOCOL_Check_For_DeviceID_Or_Broadcast(t_InterfaceType e_InterfaceType) {
    bool bRetVal = false;
    // targeting for individual motors
    if (FA_PROTOCOL_Check_For_DeviceID(e_InterfaceType)) {
        bRetVal = true;
    }// targeting for broadcast message
    else if (FA_PROTOCOL_Check_For_BroadcastID(e_InterfaceType)) {

        bRetVal = true;
    }

    return bRetVal;
}

/******************************************************************************
External Function:
FA_SDN_Check_for_DeviceID

Remarks: check for our ID

 */
bool FA_PROTOCOL_Check_For_DeviceID(t_InterfaceType e_InterfaceType) {
    t_USART_ID Target_ID;

    // to do store target address here
    Target_ID.u8_USART_ID_1 = u8_FAP_RxPacket[e_InterfaceType][7];
    Target_ID.u8_USART_ID_2 = u8_FAP_RxPacket[e_InterfaceType][8];
    Target_ID.u8_USART_ID_3 = u8_FAP_RxPacket[e_InterfaceType][9];

    return FA_PROTOCOL_COMMON_Check_For_Device_ID(Target_ID);
}

/******************************************************************************
External Function:
FA_SDN_Check_for_BroadcastID

Remarks: check for broadcast ID

 */
bool FA_PROTOCOL_Check_For_BroadcastID(t_InterfaceType e_InterfaceType) {
    t_USART_ID Target_ID;

    Target_ID.u8_USART_ID_1 = u8_FAP_RxPacket[e_InterfaceType][7];
    Target_ID.u8_USART_ID_2 = u8_FAP_RxPacket[e_InterfaceType][8];
    Target_ID.u8_USART_ID_3 = u8_FAP_RxPacket[e_InterfaceType][9];

    return FA_PROTOCOL_COMMON_Check_For_Broadcast_ID(Target_ID);
}

/******************************************************************************
External Function:
t_USART_ID FA_PROTOCOL_Get_SenderID(t_InterfaceType e_InterfaceType)

Remarks:
Returns the ID of the device whos message was just received.
 */
t_USART_ID FA_PROTOCOL_Get_SenderID(t_InterfaceType e_InterfaceType) {
    t_USART_ID Sender_ID;

    // to do store target address here
    Sender_ID.u8_USART_ID_1 = u8_FAP_RxPacket[e_InterfaceType][4];
    Sender_ID.u8_USART_ID_2 = u8_FAP_RxPacket[e_InterfaceType][5];
    Sender_ID.u8_USART_ID_3 = u8_FAP_RxPacket[e_InterfaceType][6];

    return Sender_ID;
}

/******************************************************************************
Internal Function:
FA_PROTOCOL_AddCheckSum(t_InterfaceType e_InterfaceType)

Remarks:
calculates the length and Checksum for outgoing messages

 */
void FA_PROTOCOL_AddCheckSum(t_InterfaceType e_InterfaceType) {
    uint16_t u16_MesLength = 0;
    uint16_t crc = 0;

    u16_MesLength = u8_FAP_TxPacket[e_InterfaceType][1] & TOP_BIT_BM;
    crc = crc16_ccitt(u8_FAP_TxPacket[e_InterfaceType], u16_MesLength - 2);

    u8_FAP_TxPacket[e_InterfaceType][u16_MesLength - 2] = (crc >> 8);
    u8_FAP_TxPacket[e_InterfaceType][u16_MesLength - 1] = (crc & 0xFF);
}

/******************************************************************************
Internal Function:
bool FA_PROTOCOL_Check_ForResponseBit(t_InterfaceType e_InterfaceType)

Remarks:
Returns true if the response bit was set to 1 in the received message.

 */
bool FA_PROTOCOL_Check_ForResponseBit(t_InterfaceType e_InterfaceType) {
    bool bRetval = false;
    if (((u8_FAP_RxPacket[e_InterfaceType][1]) & RESPONSE_BIT_BM) == RESPONSE_BIT_BM) {
        bRetval = true;
    }

    return bRetval;
}

void FA_PROTOCOL_Handle_FA_Command(t_InterfaceType e_InterfaceType) {
    uint8_t ack[7] = {'f', 'a', '_', 'a', 'c', 'k', '\r'};
    uint8_t nak[7] = {'f', 'a', '_', 'n', 'a', 'k', '\r'};
    t_fa_packet_data command_packet;
    uint16_t CurrentCMD = GLB_NULL_INT;


    if (FA_PACKET_convert_fa_rs232(u8_FAP_RxPacket[e_InterfaceType], u8_FAP_RxPacketLength[e_InterfaceType], &command_packet)) {
        u16_fn_FA_CMD = FA_PACKET_extract_fn(&command_packet);
        b_FAP_SendMessageFlag[e_InterfaceType] = true;
        for (uint8_t i = 0; i < 7; i++) {
            u8_FAP_TxPacket[e_InterfaceType][i] = (u16_fn_FA_CMD == FN_NULL) ? nak[i] : ack[i];
        }
        u8_FAP_TxPacketLength[e_InterfaceType] = 7;
    } else if (FA_PACKET_convert_sdn_button(u8_FAP_RxPacket[e_InterfaceType], u8_FAP_RxPacketLength[e_InterfaceType], &command_packet)) {
        CurrentCMD = FA_PACKET_extract_sdn_button(&command_packet);
        if (CurrentCMD != GLB_NULL_INT) {
            FA_APP_HandleButtonCMD(CurrentCMD);
        }
    }
}

void FA_PROTOCOL_Send_Message_Struct(t_InterfaceType e_InterfaceType, t_fa_protocol_message* t_DataStruct) {
    FA_PROTOCOL_Pack(t_DataStruct, u8_FAP_TxPacket[e_InterfaceType]);
    b_FAP_SendMessageFlag[e_InterfaceType] = true;
    u8_FAP_TxPacketLength[e_InterfaceType] = t_DataStruct->Length;
}

/**
 * @brief A single function call that can take in the different parts of an FA Protocol message
 * and pack it directly into the transmit buffer and mark the message ready to be transmitted.
 * Best used for transmitting simple messages in single function call.
 *
 * @param e_InterfaceType
 * @param messageType
 * @param responseBit
 * @param length
 * @param masterSlaveBit
 * @param data
 */
void FA_PROTOCOL_Send_Message(t_InterfaceType e_InterfaceType, uint8_t MessageType, bool ResponseBit, uint8_t Length, bool MasterSlaveBit, t_USART_ID* target, uint8_t* Data) {
    uint16_t u16_PayloadSize = 0;
    uint8_t deviceID[DEVICE_ID_LENGTH];
    t_USART_ID replyTarget;
    uint16_t checksum;
    uint16_t nodeType;


    FA_PROTOCOL_COMMON_Get_Device_ID(deviceID);
    nodeType = FA_APP_GetNodeType();
    replyTarget = FA_PROTOCOL_Get_SenderID(e_InterfaceType);

    u8_FAP_TxPacket[e_InterfaceType][0] = MessageType;
    u8_FAP_TxPacket[e_InterfaceType][1] = ResponseBit << 7;
    u8_FAP_TxPacket[e_InterfaceType][1] = Length & REMAINING_BITS_BM;
    u8_FAP_TxPacket[e_InterfaceType][2] = MasterSlaveBit << 7;
    u8_FAP_TxPacket[e_InterfaceType][2] = (nodeType >> 8) & REMAINING_BITS_BM;
    u8_FAP_TxPacket[e_InterfaceType][3] = (nodeType) & BOTTOM_BYTE_BM;
    u8_FAP_TxPacket[e_InterfaceType][4] = deviceID[0];
    u8_FAP_TxPacket[e_InterfaceType][5] = deviceID[1];
    u8_FAP_TxPacket[e_InterfaceType][6] = deviceID[2];

    if (target == NULL) {
        u8_FAP_TxPacket[e_InterfaceType][7] = replyTarget.u8_USART_ID_1;
        u8_FAP_TxPacket[e_InterfaceType][8] = replyTarget.u8_USART_ID_2;
        u8_FAP_TxPacket[e_InterfaceType][9] = replyTarget.u8_USART_ID_3;
    } else {
        u8_FAP_TxPacket[e_InterfaceType][7] = target->u8_USART_ID_1;
        u8_FAP_TxPacket[e_InterfaceType][8] = target->u8_USART_ID_2;
        u8_FAP_TxPacket[e_InterfaceType][9] = target->u8_USART_ID_3;
    }


    u16_PayloadSize = Length - MSG_NO_PAYLOAD;
    if (u16_PayloadSize > 0) {
        memcpy(&u8_FAP_TxPacket[e_InterfaceType][10], Data, u16_PayloadSize);
    }

    checksum = crc16_ccitt(u8_FAP_TxPacket[e_InterfaceType], Length - 2);

    u8_FAP_TxPacket[e_InterfaceType][10 + u16_PayloadSize] = (checksum >> 8) & BOTTOM_BYTE_BM;
    u8_FAP_TxPacket[e_InterfaceType][11 + u16_PayloadSize] = checksum & BOTTOM_BYTE_BM;

    b_FAP_SendMessageFlag[e_InterfaceType] = true;
    u8_FAP_TxPacketLength[e_InterfaceType] = Length;
}

void FA_PROTOCOL_Send_Ack(t_InterfaceType e_InterfaceType) {
    FA_PROTOCOL_Send_Message(e_InterfaceType, Ack, false, MSG_NO_PAYLOAD, false, NULL, NULL);
}

/**
 * Sends a FA Protocol NACK message over the given interface.
 * @param e_InterfaceType The interface to send the message over.
 */
void FA_PROTOCOL_Send_Nack(t_InterfaceType e_InterfaceType) {
    FA_PROTOCOL_Send_Message(e_InterfaceType, Nack, false, MSG_NO_PAYLOAD, false, NULL, NULL);
}

uint16_t FA_PROTOCOL_Get_FA_CMD(void) {
    uint16_t u16_RetVal = u16_fn_FA_CMD;
    u16_fn_FA_CMD = FN_NULL;
    return u16_RetVal;
}

/**
 * Helper function that packs the data that is common across all messages into the t_fa_protocol_message struct.
 * @param e_InterfaceType
 * @param t_DataStruct
 */
void FA_PROTOCOL_Pack_Common_Data(t_InterfaceType e_InterfaceType, t_fa_protocol_message* t_DataStruct) {
    t_USART_ID target;
    uint8_t deviceID[DEVICE_ID_LENGTH];

    FA_PROTOCOL_COMMON_Get_Device_ID(deviceID);
    target = FA_PROTOCOL_Get_SenderID(e_InterfaceType);

    t_DataStruct->NodeType = FA_APP_GetNodeType();
    t_DataStruct->SenderID.u8_USART_ID_1 = deviceID[0];
    t_DataStruct->SenderID.u8_USART_ID_2 = deviceID[1];
    t_DataStruct->SenderID.u8_USART_ID_3 = deviceID[2];

    t_DataStruct->TargetID.u8_USART_ID_1 = target.u8_USART_ID_1;
    t_DataStruct->TargetID.u8_USART_ID_2 = target.u8_USART_ID_2;
    t_DataStruct->TargetID.u8_USART_ID_3 = target.u8_USART_ID_3;
}

/**
 * Helper function that takes a t_fa_protocol_message struct and packs it into a FA Protocol message.
 *
 * @param t_DataStruct A pointer to the struct to take the data from
 * @param u8_RawDataBuffer A pointer to the buffer the message was received in

 */
void FA_PROTOCOL_Pack(t_fa_protocol_message* t_DataStruct, uint8_t *u8_RawDataBuffer) {
    uint16_t u16_PayloadSize = 0;

    // Take the struct, insert it into the buffer, and make sure the checksum is correctly placed
    // at the end of the actual data, and not just at the end of the structs data field,
    // since that will possibly contain lots of empty data.
    u8_RawDataBuffer[0] = t_DataStruct->MessageType;
    u8_RawDataBuffer[1] = t_DataStruct->ResponseBit << 7;
    u8_RawDataBuffer[1] = t_DataStruct->Length & REMAINING_BITS_BM;
    u8_RawDataBuffer[2] = t_DataStruct->MasterSlaveBit << 7;
    u8_RawDataBuffer[2] = (t_DataStruct->NodeType >> 8) & REMAINING_BITS_BM;
    u8_RawDataBuffer[3] = (t_DataStruct->NodeType) & BOTTOM_BYTE_BM;
    u8_RawDataBuffer[4] = t_DataStruct->SenderID.u8_USART_ID_1;
    u8_RawDataBuffer[5] = t_DataStruct->SenderID.u8_USART_ID_2;
    u8_RawDataBuffer[6] = t_DataStruct->SenderID.u8_USART_ID_3;
    u8_RawDataBuffer[7] = t_DataStruct->TargetID.u8_USART_ID_1;
    u8_RawDataBuffer[8] = t_DataStruct->TargetID.u8_USART_ID_2;
    u8_RawDataBuffer[9] = t_DataStruct->TargetID.u8_USART_ID_3;

    u16_PayloadSize = t_DataStruct->Length - MSG_NO_PAYLOAD;
    if (u16_PayloadSize > 0) {
        memcpy(&u8_RawDataBuffer[10], &t_DataStruct->Data, u16_PayloadSize);
    }

    u8_RawDataBuffer[10 + u16_PayloadSize] = (t_DataStruct->Checksum >> 8) & BOTTOM_BYTE_BM;
    u8_RawDataBuffer[11 + u16_PayloadSize] = t_DataStruct->Checksum & BOTTOM_BYTE_BM;
}

/**
 * Helper function that takes a receive buffer of a FA Protocol message and unpacks it into a t_fa_protocol_message struct.
 * The message in the receive buffer must be following the FA Protocol message structure.
 *
 * @param u8_RawDataBuffer A pointer to the RX buffer the FA protocol message is in.
 * @param t_DataStruct A pointer to the struct to write the unpacked data to.
 */
void FA_PROTOCOL_Unpack(uint8_t* u8_RawDataBuffer, t_fa_protocol_message* t_DataStruct) {
    uint16_t u16_PayloadSize = 0;

    t_DataStruct->MessageType = u8_RawDataBuffer[0];
    t_DataStruct->ResponseBit = (bool)(u8_RawDataBuffer[1] & TOP_BIT_BM);
    t_DataStruct->Length = u8_RawDataBuffer[1] & REMAINING_BITS_BM;
    t_DataStruct->MasterSlaveBit = (bool)(u8_RawDataBuffer[2] & TOP_BIT_BM);
    t_DataStruct->NodeType = (uint16_t)((u8_RawDataBuffer[2] & REMAINING_BITS_BM) << 7) | (uint16_t)u8_RawDataBuffer[3];
    t_DataStruct->SenderID.u8_USART_ID_1 = u8_RawDataBuffer[4];
    t_DataStruct->SenderID.u8_USART_ID_2 = u8_RawDataBuffer[5];
    t_DataStruct->SenderID.u8_USART_ID_3 = u8_RawDataBuffer[6];
    t_DataStruct->TargetID.u8_USART_ID_1 = u8_RawDataBuffer[7];
    t_DataStruct->TargetID.u8_USART_ID_2 = u8_RawDataBuffer[8];
    t_DataStruct->TargetID.u8_USART_ID_3 = u8_RawDataBuffer[9];


    u16_PayloadSize = t_DataStruct->Length - MSG_NO_PAYLOAD;
    if (u16_PayloadSize > 0) {
        memcpy(&t_DataStruct->Data, &u8_RawDataBuffer[10], u16_PayloadSize);
    }
}

/**
 * EchoMessage is used for the echo functionality whereby a message from one interface (E.G TCPIP)
 * can be "Echoed" onto another interface (E.G RS485).
 *
 * This can allow users to do things like communicate via RS485 from TCP, or
 * communicate via RS485 from RS232.
 *
 * @param e_SrcInterfaceType The source interface that the message to be echoed was received on.
 * @param e_DestInterfaceType The destination interface that the message to be echoed on will be transmitted on.
 * @return True if the
 */
bool EchoMessage(t_InterfaceType e_SrcInterfaceType, t_InterfaceType e_DestInterfaceType) {
    bool b_RetVal = false;

    if (u8_FAP_RxPacketLength[e_SrcInterfaceType] <= PROTOCOL_DATA_BUFFER_SIZE) {
        memcpy(u8_FAP_TxPacket[e_DestInterfaceType], u8_FAP_RxPacket[e_SrcInterfaceType], u8_FAP_RxPacketLength[e_SrcInterfaceType]);
        u8_FAP_TxPacketLength[e_DestInterfaceType] = u8_FAP_RxPacketLength[e_SrcInterfaceType];
        b_RetVal = true;
    }
    return b_RetVal;
}