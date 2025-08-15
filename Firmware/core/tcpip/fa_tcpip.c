/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    fa_tcpip.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "logger/logger.h"
#include "fa_tcpip.h"
#include "protocol_common.h"
#include "../tcpip_helpers.h"

#ifdef __ICCARM__
__attribute__((section(".bss.errno"))) int errno = 0; // initialization required to provide definition
#include "toolchain_specifics.h"                                // extended E codes not provided in IAR errno.h
#else
#include <errno.h>
#if (__XC32_VERSION < 4000) || (__XC32_VERSION == 243739000)
// xc32 versions >= v4.0 no longer have sys/errno.h
#include <sys/errno.h>
#endif
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

extern SYS_MODULE_OBJ TCPIP_STACK_Init(void);

static IPV4_ADDR CurrentIPAddress;
static IPV4_ADDR CurrentMask;
static bool DHCP_Enabled;
static uint16_t SERVER_PORT = DEFAULT_SERVER_PORT;
CLIENT_CONN_DATA connectionData[NUM_CLIENTS];
static TCPIP_NET_HANDLE StaticnetH;
TCP_OPTION_KEEP_ALIVE_DATA keepAliveOptions;
bool writeClientConnections = false;
APP_DATA appData = {0};

// *****************************************************************************
// *****************************************************************************
// Section: Local Function Definitions
// *****************************************************************************
// *****************************************************************************

bool ServerTasks(uint8_t(*u8ptr_RxPacket)[PROTOCOL_DATA_BUFFER_SIZE], uint8_t* u8ptr_RxPacketLength,
                 uint8_t(*u8ptr_TxPacket)[PROTOCOL_DATA_BUFFER_SIZE], uint8_t* u8ptr_TxPacketLength,
                 uint8_t* u8ptr_tcp_server_interface_selector);

bool ClientTasks(uint8_t(*u8ptr_RxPacket)[PROTOCOL_DATA_BUFFER_SIZE], uint8_t* u8ptr_RxPacketLength,
                 uint8_t(*u8ptr_TxPacket)[PROTOCOL_DATA_BUFFER_SIZE], uint8_t* u8ptr_TxPacketLength,
                 uint8_t* u8ptr_tcp_client_interface_selector);

void Load_Client_Connections();

void Load_Configuration(void);

TCP_MESSAGE_INFO InterfaceToMessageInfo(t_InterfaceType e_InterfaceType);

void TCP_Announce_Append_FA_Identifier(TCPIP_NET_HANDLE hNet, UDP_SOCKET s);

// *****************************************************************************
// *****************************************************************************
// Section: External Functions
// *****************************************************************************
// *****************************************************************************

void FA_TCPIP_Init(void) {
    // Check that the we arent using more sockets than the MAX_BSD_SOCKETS.
    // We need to minus 1 since the servers socket for listening for new connections takes up one.
    static_assert((MAX_BSD_SOCKETS - (NUM_SERVER_CLIENTS + NUM_CLIENTS)) - 1 > -1, "Not enough sockets to satisfy server and client.");

    /* Place the App state machine in its initial state. */
    appData.state = APP_WAITING_FOR_INITIALIZATION;
    appData.serverData.state = APP_BSD_CREATE_SOCKET;
    appData.serverSocket = INVALID_SOCKET;

    for (int i = 0; i < NUM_CLIENTS; i++) {
        appData.clientData[i].socket = INVALID_SOCKET;
    }

    keepAliveOptions.keepAliveEnable = true;
    keepAliveOptions.keepAliveTmo = 60000;
    keepAliveOptions.keepAliveUnackLim = 3;

    Load_Configuration();
    TCPIP_ANNOUNCE_CallbackRegister(TCP_Announce_Append_FA_Identifier);
}

void FA_TCPIP_Reinitialise(void) {
    for (uint8_t i = 0; i < NUM_SERVER_CLIENTS; i++) {
        if (appData.serverData.ClientSock[i] != INVALID_SOCKET) {
            closesocket(appData.serverData.ClientSock[i]);
            appData.serverData.ClientSock[i] = INVALID_SOCKET;
        }
    }
    // Close the server socket too
    if (appData.serverData.bsdServerSocket != INVALID_SOCKET) {
        closesocket(appData.serverData.bsdServerSocket);
        appData.serverData.bsdServerSocket = INVALID_SOCKET;
    }

    for (int i = 0; i < NUM_CLIENTS; i++) {
        closesocket(appData.clientData[i].socket);
        appData.clientData[i].state = APP_TCPIP_SETUP_SOCKET;
    }

    TCPIP_STACK_NetDown(StaticnetH);
    TCPIP_STACK_Deinitialize(sysObj.tcpip);

    sysObj.tcpip = TCPIP_STACK_Init();
    FA_TCPIP_Init();
}

void FA_TCPIP_Reload_Configuration(void) {
    Load_Configuration();
    FA_TCPIP_Reinitialise();
}

bool FA_TCPIP_Tasks(uint8_t(*u8ptr_RxPacket)[PROTOCOL_DATA_BUFFER_SIZE], uint8_t* u8ptr_RxPacketLength,
                    uint8_t(*u8ptr_TxPacket)[PROTOCOL_DATA_BUFFER_SIZE], uint8_t* u8ptr_TxPacketLength,
                    uint8_t* u8ptr_tcp_server_interface_selector, uint8_t* u8ptr_tcp_client_interface_selector) {

    SYS_STATUS tcpipStat;
    const char *netName, *netBiosName;
    TCPIP_NET_HANDLE netH;
    int i, nNets;
    bool bRet = false;

    switch (appData.state) {
        case APP_WAITING_FOR_INITIALIZATION:
            tcpipStat = TCPIP_STACK_Status(sysObj.tcpip);
            if (tcpipStat < 0) { // some error occurred
                LOG_WARNING(" APP: TCP/IP stack initialization failed!\r\n");
                appData.state = APP_TCPIP_ERROR;
            } else if (tcpipStat == SYS_STATUS_READY) {
                // now that the stack is ready we can check the
                // available interfaces
                nNets = TCPIP_STACK_NumberOfNetworksGet();
                for (i = 0; i < nNets; i++) {

                    netH = TCPIP_STACK_IndexToNet(i);
                    netName = TCPIP_STACK_NetNameGet(netH);
                    netBiosName = TCPIP_STACK_NetBIOSName(netH);
#if defined(TCPIP_STACK_USE_NBNS)
                    LOG_VERBOSE("    Interface %s on host %s - NBNS enabled\r\n", netName, netBiosName);
#else
                    LOG_VERBOSE("    Interface %s on host %s - NBNS disabled\r\n", netName, netBiosName);
#endif  // defined(TCPIP_STACK_USE_NBNS)
                    (void)netName; // avoid compiler warning
                    (void)netBiosName; // if SYS_CONSOLE_PRINT is null macro

                }
                appData.state = APP_TCPIP_WAIT_FOR_IP;
            }
            break;


        case APP_TCPIP_WAIT_FOR_IP:

            // if the IP address of an interface has changed
            // display the new value on the system console
            nNets = TCPIP_STACK_NumberOfNetworksGet();

            for (i = 0; i < nNets; i++) {
                netH = TCPIP_STACK_IndexToNet(i);
                if (!TCPIP_STACK_NetIsReady(netH)) {
                    return false; // interface not ready yet!
                }

                StaticnetH = netH;
                if (DHCP_Enabled) {
                    TCPIP_DHCP_Enable(netH);

                    // Convert IP and Mask to host byte order
                    CurrentIPAddress.Val = TCPIP_Helper_ntohl(TCPIP_STACK_NetAddress(netH));
                    CurrentMask.Val = TCPIP_Helper_ntohl(TCPIP_STACK_NetMask(netH));
                } else {
                    IPV4_ADDR ip;
                    IPV4_ADDR mask;

                    TCPIP_DHCP_Disable(netH);

                    // Convert IP and Mask back to network byte order
                    ip.Val = TCPIP_Helper_htonl(CurrentIPAddress.Val);
                    mask.Val = TCPIP_Helper_htonl(CurrentMask.Val);

                    TCPIP_STACK_NetAddressSet(netH, &ip, &mask, true);
                }

                LOG_INFO("%s IP Address: %d.%d.%d.%d \r\n", TCPIP_STACK_NetNameGet(netH), CurrentIPAddress.v[3], CurrentIPAddress.v[2], CurrentIPAddress.v[1], CurrentIPAddress.v[0]);

                appData.state = APP_RUN_TASKS;
                for (int i = 0; i < NUM_CLIENTS; i++) {
                    appData.clientData[i].state = APP_TCPIP_SETUP_SOCKET;
                }

                appData.serverData.state = APP_BSD_CREATE_SOCKET;
            }
            break;
        case APP_RUN_TASKS:
            bRet = ServerTasks(u8ptr_RxPacket, u8ptr_RxPacketLength,
                               u8ptr_TxPacket, u8ptr_TxPacketLength,
                               u8ptr_tcp_server_interface_selector);
            bRet |= ClientTasks(u8ptr_RxPacket, u8ptr_RxPacketLength,
                                u8ptr_TxPacket, u8ptr_TxPacketLength,
                                u8ptr_tcp_client_interface_selector);
            return bRet;
        case APP_TCPIP_ERROR:
            break;

        default:
            return false;

    }
    return bRet;
}

IPV4_ADDR FA_TCPIP_Get_IP(void) {
    return CurrentIPAddress;
}

IPV4_ADDR FA_TCPIP_Get_Mask(void) {
    return CurrentMask;
}

TCPIP_MAC_ADDR FA_TCPIP_Get_MAC_Address(void) {
    uint8_t default_mac[MAC_LENGTH] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC}; // MAC address of FA software department's SAME54 Xplained development board "B")
    TCPIP_MAC_ADDR ee_mac;

    if (FRAMEWORK_get_mac((uint8_t *) & ee_mac) == false) {
        memcpy((uint8_t *) & ee_mac, default_mac, MAC_LENGTH);
    }
    return ee_mac;
}

void FA_TCPIP_Set_Client_IP_Address(IPV4_ADDR ip, uint16_t port, uint8_t listIndex) {
    if (appData.clientData[listIndex].socket != INVALID_SOCKET) {
        closesocket(appData.clientData[listIndex].socket);
        LOG_VERBOSE("TCP Client: Socket %d was closed because a new client connection has been requested.\r\n", appData.clientData[listIndex].socket);
    }

    appData.clientData[listIndex].state = APP_TCPIP_SETUP_SOCKET;
    appData.clientData[listIndex].addr.sin_addr.S_un.S_addr = ip.Val;
    appData.clientData[listIndex].addr.sin_port = port;

    connectionData[listIndex].ip_address.Val = ip.Val;
    connectionData[listIndex].port = port;

    // Mark data for writing to EEPROM
    writeClientConnections = true;
}

bool FA_TCPIP_Write_Client_Connections() {
    if (writeClientConnections) {
        writeClientConnections = false;
        return FA_EEPROM_write_block(EEPROM_CLIENT_CONNECTIONS, (uint8_t *)connectionData, sizeof (CLIENT_CONN_DATA) * NUM_CLIENTS);
    }

    return false;
}

// *****************************************************************************
// *****************************************************************************
// Section: Internal Functions
// *****************************************************************************
// *****************************************************************************

/**
 * ServerTasks performs the TCP server operations.
 * The starts listening on the specified port and will accept connections and load
 * the received data into the supplied buffers that are passed in as parameters.
 *
 * @param u8ptr_RxPacket A pointer to the whole RX packet buffer.
 * @param u8ptr_RxPacketLength A pointer to the RX packet length buffer.
 * @param u8ptr_TxPacket A pointer to the whole TX packet buffer.
 * @param u8ptr_TxPacketLength A pointer to the TX packet length buffer.
 * @param u8ptr_tcp_server_interface_selector A pointer to the TCP server interface selector.
 * @return
 */
bool ServerTasks(uint8_t(*u8ptr_RxPacket)[PROTOCOL_DATA_BUFFER_SIZE], uint8_t* u8ptr_RxPacketLength,
                 uint8_t(*u8ptr_TxPacket)[PROTOCOL_DATA_BUFFER_SIZE], uint8_t* u8ptr_TxPacketLength,
                 uint8_t* u8ptr_tcp_server_interface_selector) {
    bool bRetVal = false;

    switch (appData.serverData.state) {
        case APP_BSD_CREATE_SOCKET:
        {
            // Initialize all client socket handles so that we don't process
            // them in the BSD_OPERATION state
            for (int clientPointer = 0; clientPointer < NUM_SERVER_CLIENTS; clientPointer++)
                appData.serverData.ClientSock[clientPointer] = INVALID_SOCKET;

            // Create a socket for this server to listen and accept connections on
            SOCKET tcpSkt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (tcpSkt == INVALID_SOCKET)
                return false;

            LOG_VERBOSE("TCP Server: Server socket created: %d\r\n", tcpSkt);
            appData.serverData.bsdServerSocket = (SOCKET)tcpSkt;
            appData.serverData.state = APP_BSD_BIND;
        }
            break;

        case APP_BSD_BIND:
        {
            // Bind socket to a local port
            struct sockaddr_in addr;
            int addrlen = sizeof (struct sockaddr_in);
            addr.sin_port = SERVER_PORT;
            if (DHCP_Enabled) {
                addr.sin_addr.S_un.S_addr = IP_ADDR_ANY; // Bind to any available address
            } else {
                // Manually construct the 32-bit IP address
                addr.sin_addr.S_un.S_addr = TCPIP_Helper_htonl(CurrentIPAddress.Val);
            }

            if (bind(appData.serverData.bsdServerSocket, (struct sockaddr*)&addr, addrlen) == SOCKET_ERROR) {
                LOG_VERBOSE("TCP Server: Bind failed!\r\n");
                return false;
            }

            appData.serverData.state = APP_BSD_LISTEN;
            LOG_VERBOSE("TCP Server: Server listening on port %d...\r\n", SERVER_PORT);
        }
            break;

        case APP_BSD_LISTEN:
        {
            if (listen(appData.serverData.bsdServerSocket, NUM_SERVER_CLIENTS) == 0) {
                appData.serverData.state = APP_BSD_OPERATION;
                LOG_VERBOSE("TCP Server: Waiting for Client Connection on port: %d\r\n", SERVER_PORT);
            }
        }
            break;
        case APP_BSD_OPERATION:
        {
            struct sockaddr_in addRemote;
            int addrlen = sizeof (struct sockaddr_in);

            // Convert interface selector to 0 indexed socket value.
            SOCKET skt = *u8ptr_tcp_server_interface_selector - Interface_TCPIP_Server_Client_1;

            // Accept any pending connection requests, assuming we have a place to store the socket descriptor
            if (appData.serverData.ClientSock[skt] == INVALID_SOCKET) {
                appData.serverData.ClientSock[skt] = accept(appData.serverData.bsdServerSocket, (struct sockaddr*)&addRemote, &addrlen);

                // If a client connection is accepted then configure keep alive.
                if (appData.serverData.ClientSock[skt] != SOCKET_ERROR) {
                    TCPIP_TCP_OptionsSet(TCPIP_BSD_Socket(appData.serverData.ClientSock[skt]), TCP_OPTION_KEEP_ALIVE, &keepAliveOptions);
                    LOG_VERBOSE("TCP Server: Accepted Connection on socket %d\r\n", appData.serverData.ClientSock[skt]);
                }
            }

            // Process socket. If socket is invalid then look for the next valid socket.
            for (int i = 0; i < NUM_SERVER_CLIENTS; i++) {
                if (appData.serverData.ClientSock[(skt + i) % NUM_SERVER_CLIENTS] != INVALID_SOCKET) {
                    // Once we've found a socket that can be processed, add the number of iterations
                    // it took to the interface selector to get the interface selector of the valid socket.
                    *u8ptr_tcp_server_interface_selector = ((*u8ptr_tcp_server_interface_selector - Interface_TCPIP_Server_Client_1 + i) % NUM_SERVER_CLIENTS) + Interface_TCPIP_Server_Client_1;

                    bRetVal = FA_TCPIP_Read_Message(*u8ptr_tcp_server_interface_selector, u8ptr_RxPacket[*u8ptr_tcp_server_interface_selector], &u8ptr_RxPacketLength[*u8ptr_tcp_server_interface_selector]);
                    FA_TCPIP_Send_Message(*u8ptr_tcp_server_interface_selector, u8ptr_TxPacket[*u8ptr_tcp_server_interface_selector], &u8ptr_TxPacketLength[*u8ptr_tcp_server_interface_selector]);
                    break;
                }
            }
        }
            break;
        case APP_BSD_CLOSE:
            // Close all client sockets before changing IP
            FA_TCPIP_Reinitialise();
            break;
        default:
            break;
    }
    return bRetVal;
}

/**
 * Performs TCP client tasks.
 * Attempts to connect to the connection data stored in EEPROM.
 * If no connection data is in EEPROM (IP and Port 0 is used to indicate no data),
 * connection data can be set via the Configurator Tool, which will be written
 * to EEPROM so the board can connect on future start ups.
 *
 * Currently, the client will parrot messages received from the server back to the
 * server.
 *
 * @param u8ptr_RxPacket A pointer to the whole RX packet buffer.
 * @param u8ptr_RxPacketLength A pointer to the RX packet length buffer.
 * @param u8ptr_TxPacket A pointer to the whole TX packet buffer.
 * @param u8ptr_TxPacketLength A pointer to the TX packet length buffer.
 * @param u8ptr_tcp_client_interface_selector A pointer to the TCP client interface selector.
 * @return
 */
bool ClientTasks(uint8_t(*u8ptr_RxPacket)[PROTOCOL_DATA_BUFFER_SIZE], uint8_t* u8ptr_RxPacketLength,
                 uint8_t(*u8ptr_TxPacket)[PROTOCOL_DATA_BUFFER_SIZE], uint8_t* u8ptr_TxPacketLength,
                 uint8_t* u8ptr_tcp_client_interface_selector) {
    bool bRetVal = false;

    for (int i = 0; i < NUM_CLIENTS; i++) {
        switch (appData.clientData[i].state) {
            case APP_TCPIP_SETUP_SOCKET:
            {
                int skt;

                // If the IP and port are 0, do nothing.
                if (appData.clientData[i].addr.sin_addr.S_un.S_addr == 0 &&
                    appData.clientData[i].addr.sin_port == 0) {
                    return false;
                }

                // Create the socket and store in the app data
                if ((skt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == SOCKET_ERROR) {
                    return false;
                } else {
                    appData.clientData[i].socket = (SOCKET)skt;
                }

                LOG_VERBOSE("TCP Client: Socket created %d.\r\n", skt);
                appData.clientData[i].state = APP_TCPIP_CONNECT;
                break;
            }

            case APP_TCPIP_CONNECT:
            {
                if (connect(appData.clientData[i].socket, (struct sockaddr*)&appData.clientData[i].addr, sizeof (struct sockaddr)) < 0) {
                    return false;
                }
                LOG_VERBOSE("TCP Client: Connected on socket %d!\r\n", appData.clientData[i].socket);
                TCPIP_TCP_OptionsSet(TCPIP_BSD_Socket(appData.clientData[i].socket), TCP_OPTION_KEEP_ALIVE, &keepAliveOptions);

                appData.clientData[i].state = APP_BSD_OPERATION;
                break;
            }

            case APP_BSD_OPERATION:
            {
                // Convert interface selector to 0 indexed value.
                SOCKET skt = *u8ptr_tcp_client_interface_selector - Interface_TCPIP_Client_1;

                for (int i = 0; i < NUM_CLIENTS; i++) {
                    if (appData.clientData[(skt + i) % NUM_CLIENTS].socket != INVALID_SOCKET) {
                        // Once we've found a socket that can be processed, add the number of iterations
                        // it took to the interface selector to get the interface selector of the valid socket.
                        *u8ptr_tcp_client_interface_selector = ((*u8ptr_tcp_client_interface_selector - Interface_TCPIP_Client_1 + i) % NUM_CLIENTS) + Interface_TCPIP_Client_1;

                        // If the interface selector changes, the client_rx/tx_message also needs to be incremented too
                        bRetVal = FA_TCPIP_Read_Message(*u8ptr_tcp_client_interface_selector, u8ptr_RxPacket[*u8ptr_tcp_client_interface_selector], &u8ptr_RxPacketLength[*u8ptr_tcp_client_interface_selector]);
                        FA_TCPIP_Send_Message(*u8ptr_tcp_client_interface_selector, u8ptr_TxPacket[*u8ptr_tcp_client_interface_selector], &u8ptr_TxPacketLength[*u8ptr_tcp_client_interface_selector]);
                    }
                    break;
                }

                default:
                {
                    bRetVal = false;
                    break;
                }
            }
        }
    }
    return bRetVal;
}

void FA_TCPIP_Send_Message(t_InterfaceType e_InterfaceType, uint8_t* u8_Message, uint8_t* u8_Message_Length) {
    if (*u8_Message_Length > 0) {
        TCP_MESSAGE_INFO message_info = InterfaceToMessageInfo(e_InterfaceType);

        if (message_info.mode == TCP_MODE_Server) {
            if (send(appData.serverData.ClientSock[message_info.socket], (const char*)u8_Message, *u8_Message_Length, 0) == -1) {
                LOG_VERBOSE("TCP Server Response: Message failed to send...\r\n");
                if (errno == ECONNRESET) {
                    closesocket(appData.serverData.ClientSock[message_info.socket]);
                    appData.serverData.ClientSock[message_info.socket] = INVALID_SOCKET;
                    LOG_VERBOSE("\r\nTCP Server: Connection Closed during server response step.\r\n");
                }
            } else {
                LOG_VERBOSE("TCP Server Response: Message sent!\r\n");
                memset(u8_Message, 0, PROTOCOL_DATA_BUFFER_SIZE);
                *u8_Message_Length = 0;
            }
        } else if (message_info.mode == TCP_MODE_Client) {
            if (send(appData.clientData[message_info.socket].socket, (const char*)u8_Message, *u8_Message_Length, 0) == -1) {
                LOG_VERBOSE("TCP Client: Message failed to send...\r\n");
                if (errno == ECONNRESET) {
                    closesocket(appData.clientData[message_info.socket].socket);
                    appData.clientData[message_info.socket].socket = INVALID_SOCKET;
                    LOG_VERBOSE("TCP Client: Connection Closed\r\n");
                    appData.clientData[message_info.socket].state = APP_TCPIP_SETUP_SOCKET;
                }
            } else {
                LOG_VERBOSE("TCP Client: Message sent!\r\n");
                memset(u8_Message, 0, PROTOCOL_DATA_BUFFER_SIZE);
                *u8_Message_Length = 0;
            }
        }
    }
}

bool FA_TCPIP_Read_Message(t_InterfaceType e_InterfaceType, uint8_t* u8_Message, uint8_t* u8_Message_Length) {
    bool b_RetVal = false;
    TCP_MESSAGE_INFO message_info = InterfaceToMessageInfo(e_InterfaceType);

    if (message_info.mode == TCP_MODE_Server) {
        *u8_Message_Length = recv(appData.serverData.ClientSock[message_info.socket], (char*)u8_Message, FRAMEWORK_MAX_PACKET_SIZE, 0);

        if (*u8_Message_Length > 0 && *u8_Message_Length != 0xFF) {
            b_RetVal = true;
            LOG_VERBOSE("TCP Server: Message Received!\r\n");
        } else if ((*u8_Message_Length == 0) || (errno != EWOULDBLOCK && errno != ENOTCONN)) {
            LOG_VERBOSE("TCP Server: Client Closed Socket %d\r\n", appData.serverData.ClientSock[message_info.socket]);
            closesocket(appData.serverData.ClientSock[message_info.socket]);
            appData.serverData.ClientSock[message_info.socket] = INVALID_SOCKET;
        }

    } else if (message_info.mode == TCP_MODE_Client) {
        *u8_Message_Length = recv(appData.clientData[message_info.socket].socket, (char*)u8_Message, FRAMEWORK_MAX_PACKET_SIZE, 0);

        if (*u8_Message_Length > 0 && *u8_Message_Length != 0xFF) {
            b_RetVal = true;
            LOG_VERBOSE("TCP Client: Message Received!\r\n");
        } else if ((*u8_Message_Length == 0) || (errno != EWOULDBLOCK && errno != ENOTCONN)) {
            closesocket(appData.clientData[message_info.socket].socket);
            appData.clientData[message_info.socket].socket = INVALID_SOCKET;
            LOG_VERBOSE("TCP Client: Connection Closed\r\n");
            appData.clientData[message_info.socket].state = APP_TCPIP_SETUP_SOCKET;
        }
    }

    return b_RetVal;
}

TCP_MESSAGE_INFO InterfaceToMessageInfo(t_InterfaceType e_InterfaceType) {
    TCP_MESSAGE_INFO message_info = {0};

    if (e_InterfaceType >= Interface_TCPIP_Server_Client_1 && e_InterfaceType <= Interface_TCPIP_Server_Client_6) {
        message_info.socket = e_InterfaceType - Interface_TCPIP_Server_Client_1;
        message_info.mode = TCP_MODE_Server;
    } else if (e_InterfaceType >= Interface_TCPIP_Client_1 && e_InterfaceType <= Interface_TCPIP_Client_3) {
        message_info.socket = e_InterfaceType - Interface_TCPIP_Client_1;
        message_info.mode = TCP_MODE_Client;
    } else {
        LOG_ERROR("Invalid interface passed to InterfaceToMessageInfo");
    }

    return message_info;
}

/**
 * Load_Configuration loads the TCP configuration from EEPROM into the
 * configuration variables.
 */
void Load_Configuration(void) {
    DHCP_Enabled = FA_EEPROM_read_8(EEPROM_DHCP);
    FA_EEPROM_read_block(EEPROM_STATIC_IP, (uint8_t *) & CurrentIPAddress, 4);
    FA_EEPROM_read_block(EEPROM_STATIC_MASK, (uint8_t *) & CurrentMask, 4);
    Load_Client_Connections();
    SERVER_PORT = FA_EEPROM_read_16(EEPROM_PORT_ADDR);
}

/**
 * Loads the client connection data from EEPROM into the clients. The connection
 * data is initialised as 0s on first start up.
 */
void Load_Client_Connections() {
    char string[16];
    CLIENT_CONN_DATA connectionData[NUM_CLIENTS];
    FA_EEPROM_read_block(EEPROM_CLIENT_CONNECTIONS, (uint8_t *)connectionData, sizeof (CLIENT_CONN_DATA) * NUM_CLIENTS);

    for (int i = 0; i < NUM_CLIENTS; i++) {
        appData.clientData[i].addr.sin_addr.S_un.S_addr = connectionData[i].ip_address.Val;
        appData.clientData[i].addr.sin_port = connectionData[i].port;
        TCPIP_Helper_IPAddressToString(&connectionData[i].ip_address, string, sizeof (string));
        LOG_VERBOSE("TCP Client: Loading client data - IP=%s, PORT=%d\r\n", string, connectionData[i].port);
    }
}

/**
 * TCP_Announce_Append_FA_Identifier will append a FA identifier to the TCP announce message so that we
 * can differentiate between other TCP Announce microchip products and FA products.
 *
 * @param hNet Network interface handle passed into callback function.
 * @param s UDP socket passed in to callback function.
 * @return void
 */
void TCP_Announce_Append_FA_Identifier(TCPIP_NET_HANDLE hNet, UDP_SOCKET s) {
    if (TCPIP_UDP_PutIsReady(s)) {
        TCPIP_UDP_StringPut(s, (const uint8_t*)"FA_DEVICE");
    }
}

/*******************************************************************************
 End of File
 */