/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    fa_tcpip.h

  Summary:
    This header file provides prototypes and definitions for the FA TCPIP interface.

  Description:
    This header file provides function prototypes and data type definitions for
    the FA TCPIP interface. Using this interface you can implement TCPIP into any
    project and get communication over a traditional network.
 *******************************************************************************/

#ifndef _FA_TCPIP_H
#define _FA_TCPIP_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "tcpip/tcpip.h"
#include "configuration.h"
#include "framework.h"
#include "protocol_common.h"


// *****************************************************************************
// *****************************************************************************
// Section: External Data
// *****************************************************************************
// *****************************************************************************

extern const uint8_t MPFS_IMAGE_DATA[];

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************

#define NUM_SERVER_CLIENTS 6
#define NUM_CLIENTS 3
#define DEFAULT_SERVER_PORT 9670

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

/**
 * APP_STATES
 *
 * This enumeration defines the valid application states.
 * These states determine the behavior of the application at various times.
 */
typedef enum {
    // Main state machine states
    APP_WAITING_FOR_INITIALIZATION,
    APP_TCPIP_WAIT_FOR_IP,
    APP_RUN_TASKS,

    // TCP server state machine states
    APP_BSD_INIT,
    APP_BSD_CREATE_SOCKET,
    APP_BSD_BIND,
    APP_BSD_LISTEN,
    APP_BSD_OPERATION,

    // TCP client state machine states
    APP_TCPIP_SETUP_SOCKET,
    APP_TCPIP_CONNECT,
    APP_RECEIVE_MESSAGE,
    APP_SEND_MESSAGE,

    // Error and close states
    APP_TCPIP_ERROR,
    APP_BSD_CLOSE
} APP_STATES;

/**
 * SERVER_DATA
 *
 * This struct defines the data needed for the TCP Server to operate.
 */
typedef struct {
    APP_STATES state;
    SOCKET bsdServerSocket;
    SOCKET ClientSock[NUM_SERVER_CLIENTS];
} SERVER_DATA;

/**
 * CLIENT_DATA
 *
 * This struct defines the data needed for the TCP Client to operate.
 */
typedef struct {
    APP_STATES state;
    SOCKET socket;
    char* host;
    char* path;
    uint16_t port;
    struct sockaddr_in addr;
} CLIENT_DATA;

/**
 * APP_DATA
 *
 * This struct defines the data needed for the whole TCP stack to operate.
 */
typedef struct {
    APP_STATES state;
    SERVER_DATA serverData;
    CLIENT_DATA clientData[NUM_CLIENTS];
    TCP_SOCKET serverSocket;
    char* host;
    char* path;
    uint16_t port;
} APP_DATA;

/**
 * CLINET_CONN_DATA
 *
 * This struct defines the data needed for the TCP client to make a connection to
 * a TCP server.
 */
typedef struct __attribute__((packed)) {
    IPV4_ADDR ip_address;
    uint16_t port;
}
CLIENT_CONN_DATA;

/**
 * t_fa_port
 *
 * A union representing a port number. The port can be read via the whole 16 bits,
 * or via the LSB and MSB or the 16 bit value.
 */
typedef union {
    uint16_t port;

    struct {
        uint8_t server_port_lsb;
        uint8_t server_port_msb;
    };
} t_fa_port;

/**
 * TCP_MODE
 *
 * Enum representing the two modes of TCP, client and server.
 */
typedef enum {
    TCP_MODE_Server,
    TCP_MODE_Client
} TCP_MODE;

/**
 * TCP_MESSAGE_INFO
 *
 * Struct containing a TCP socket and the TCP mode, used for sending and receiving TCP messages.
 */
typedef struct {
    SOCKET socket;
    TCP_MODE mode;
}
TCP_MESSAGE_INFO;

// *****************************************************************************
// *****************************************************************************
// Section: External Functions
// *****************************************************************************
// *****************************************************************************

/**
 * FA_TCPIP_Init is used to initialise the FA TCPIP stack to a ready to use state.
 */
void FA_TCPIP_Init(void);

/**
 * FA_TCPIP_Reinitialise will shutdown the TCP stack and initialise it again, effectively
 * restarting the TCP stack.
 */
void FA_TCPIP_Reinitialise(void);

/**
 * FA_TCPIP_Reload_TCP_Configuration will read the latest IP and TCP settings
 * from EEPROM and restart the TCP stack so the new configuration is used.
 */
void FA_TCPIP_Reload_Configuration(void);


/**
 * FA_TCPIP_Tasks is the function that performs all the tasks to make the TCP stack
 * operational.
 *
 * @param u8ptr_RxPacket A pointer to the whole RX packet buffer.
 * @param u8ptr_RxPacketLength A pointer to the RX packet length buffer.
 * @param u8ptr_TxPacket A pointer to the whole TX packet buffer.
 * @param u8ptr_TxPacketLength A pointer to the TX packet length buffer.
 * @param u8ptr_tcp_server_interface_selector A pointer to the TCP server interface selector.
 * @param u8ptr_tcp_client_interface_selector A pointer to the TCP client interface selector.
 * @return
 */
bool FA_TCPIP_Tasks(uint8_t(*u8ptr_RxPacket)[PROTOCOL_DATA_BUFFER_SIZE], uint8_t* u8ptr_RxPacketLength,
        uint8_t(*u8ptr_TxPacket)[PROTOCOL_DATA_BUFFER_SIZE], uint8_t* u8ptr_TxPacketLength,
        uint8_t* u8ptr_tcp_server_interface_selector, uint8_t* u8ptr_tcp_client_interface_selector);

/**
 * FA_TCPIP_Get_IP returns the currently used IP address.
 * @return Currently used IP address.
 */
IPV4_ADDR FA_TCPIP_Get_IP(void);
/**
 * FA_TCPIP_Get_Mask returns the currently used Subnet Mask.
 * @return Currently used Subnet Mask.
 */
IPV4_ADDR FA_TCPIP_Get_Mask(void);

/**
 * FA_TCPIP_Get_MAC_Address Returns the MAC address of the device.
 * @return TCPIP_MAC_ADDR representing the MAC address
 */
TCPIP_MAC_ADDR FA_TCPIP_Get_MAC_Address(void);

/**
 * FA_TCPIP_Set_Client_IP_Address will set the client connection data for the TCP
 * client to connect to, and will save that information to EEPROM.
 *
 * @param ip The IP address to connect to.
 * @param port The port to connect to.
 * @param listIndex The index into the client connection data array to write the data to.
 */
void FA_TCPIP_Set_Client_IP_Address(IPV4_ADDR ip, uint16_t port, uint8_t listIndex);

/**
 * FA_TCPIP_Write_Client_Connections writes the client connection data to EEPROM
 *
 * @return True if the client connection data was successfully written to EEPROM,
 *         false if not.
 */
bool FA_TCPIP_Write_Client_Connections();

/**
 * FA_TCPIP_Send_Message will instantly send a message over TCP to socket who belongs to the provided interface.
 *
 * @param e_InterfaceType The TCP interface selector to send the message over.
 * @param u8_Message A pointer to the buffer containing the data to send.
 * @param u8_Message_Length The length of data to send in the buffer.
 */
void FA_TCPIP_Send_Message(t_InterfaceType e_InterfaceType, uint8_t* u8_Message, uint8_t* u8_Message_Length);

/**
 * FA_TCPIP_Read_Message reads a message on the given interface.
 *
 * @param t_InterfaceType The TCP interface selector to read the message from.
 * @param u8_Message The receive buffer where the message will be read to.
 * @param u8_Message_Length The receive length where the length will be stored in.
 * @return True if a message was received, false if not.
 */
bool FA_TCPIP_Read_Message(t_InterfaceType e_InterfaceType, uint8_t* u8_Message, uint8_t* u8_Message_Length);

#endif /* _FA_TCPIP_H */
/*******************************************************************************
 End of File
 */