/*******************************************************************************
  Future Automation Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    fa_protocol_common.c

  Summary:
    This file contains the source code for the Future Automation common protocol functionality.

  Description:
    This file provides all the necessary functions to work with the common protocol data.

 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "protocol_common.h"
#include "fa_ir.h"
#include "fa_eeprom_map.h"
#include "fa_eeprom.h"
#include "fa_tcpip.h"
#include "fa_at24.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Definitions
// *****************************************************************************
// *****************************************************************************

static uint8_t u8_DeviceLabel[LABEL_LENGTH];
static uint8_t u8_DeviceID[DEVICE_ID_SIZE] = {0xAA, 0xAA, 0xAA}; // 0xAAAAAA is default device ID.
static uint8_t u8_BroadcastID[3] = {0xFF, 0xFF, 0xFF};

static bool SystemHasSetID = false;
static bool bSaveDevice_ID = false;

//new
bool bSaveSystem_Label = false;
static bool bSaveDevice_IPv4 = false;

static bool tempDCHP;
static IPV4_ADDR tempStaticIP;
static IPV4_ADDR tempStaticMask;
static t_fa_port tempStaticPort;

// *****************************************************************************
// *****************************************************************************
// Section: Forward references
// *****************************************************************************
// *****************************************************************************

void FA_PROTOCOL_COMMON_Set_System_ID(void);
bool FA_PROTOCOL_COMMON_Check_For_System_ID(void);
void FA_PROTOCOL_COMMON_Save_Device_ID(void);
void FA_PROTOCOL_COMMON_Save_IPv4(void);
void FA_PROTOCOL_COMMON_Save_Label(void);


// *****************************************************************************
// *****************************************************************************
// Section: Protocol Common External Functions
// *****************************************************************************
// *****************************************************************************

void FA_PROTOCOL_COMMON_Initialize(void) {
    for (uint8_t i = 0; i < 16; i++) {
        //Memory_pointer = Memory_pointer + i;
        u8_DeviceLabel[i] = FA_EEPROM_read_8(EEPROM_DEVICE_LABEL + i);
    }

    u8_DeviceID[0] = FA_EEPROM_read_8(EEPROM_DEVICE_ID);
    u8_DeviceID[1] = FA_EEPROM_read_8(EEPROM_DEVICE_ID + 1);
    u8_DeviceID[2] = FA_EEPROM_read_8(EEPROM_DEVICE_ID + 2);

    //bool flag to check if device ID has been grabbed from MAC Address
    SystemHasSetID = FA_EEPROM_read_8(EEPROM_SYSTEM_ID_SET);

    tempDCHP = (bool)FA_EEPROM_read_8(EEPROM_DHCP);
    tempStaticPort.port = FA_EEPROM_read_16(EEPROM_PORT_ADDR);
    tempStaticIP.Val = FA_EEPROM_read_32(EEPROM_STATIC_IP);
    tempStaticIP.Val = FA_EEPROM_read_32(EEPROM_STATIC_MASK);
}

void FA_PROTOCOL_COMMON_Reset_EEPROM(void) {
    FA_EEPROM_write_8(EEPROM_SYSTEM_ID_SET, 0); /* reset ID */
    FA_EEPROM_fill_block(EEPROM_DEVICE_ID, 0, DEVICE_ID_LENGTH);
    FA_EEPROM_fill_block(EEPROM_DEVICE_LABEL, 0, LABEL_LENGTH);
    FA_EEPROM_write_8(EEPROM_DHCP, 1);
    FA_EEPROM_write_32(EEPROM_STATIC_IP, 0);
    FA_EEPROM_write_32(EEPROM_STATIC_MASK, 0);
    FA_EEPROM_write_16(EEPROM_PORT_ADDR, DEFAULT_SERVER_PORT);
    FA_EEPROM_fill_block(EEPROM_CLIENT_CONNECTIONS, 0, sizeof (CLIENT_CONN_DATA) * NUM_CLIENTS);
}

void FA_PROTOCOL_COMMON_InactiveTimeOut(void) {
    FA_PROTOCOL_COMMON_Save_Label();
    FA_PROTOCOL_COMMON_Save_IPv4();
    FA_PROTOCOL_COMMON_Save_Device_ID();
}

void FA_PROTOCOL_COMMON_Tasks(void) {
    if (FA_PROTOCOL_COMMON_Check_For_System_ID()) {
        FA_PROTOCOL_COMMON_Set_System_ID();
    }
}

void FA_PROTOCOL_COMMON_Get_Label(uint8_t* u8Ptr_Label) {
    memcpy(u8Ptr_Label, u8_DeviceLabel, LABEL_LENGTH);
}

void FA_PROTOCOL_COMMON_Set_Label(uint8_t* u8Ptr_Label) {
    memcpy(u8_DeviceLabel, u8Ptr_Label, LABEL_LENGTH);
    bSaveSystem_Label = true;
}

void FA_PROTOCOL_COMMON_Get_Device_ID(uint8_t* u8Ptr_DeviceID) {
    memcpy(u8Ptr_DeviceID, u8_DeviceID, DEVICE_ID_LENGTH);
}

bool FA_PROTOCOL_COMMON_Set_Device_ID(uint8_t* u8Ptr_DeviceID) {
    bool bRetVal = false;

    bool all_ff = (u8Ptr_DeviceID[0] == 0xFF) &&
            (u8Ptr_DeviceID[1] == 0xFF) &&
            (u8Ptr_DeviceID[2] == 0xFF);

    bool all_zero = (u8Ptr_DeviceID[0] == 0x00) &&
            (u8Ptr_DeviceID[1] == 0x00) &&
            (u8Ptr_DeviceID[2] == 0x00);

    if (!all_ff && !all_zero) {
        u8_DeviceID[0] = u8Ptr_DeviceID[0];
        u8_DeviceID[1] = u8Ptr_DeviceID[1];
        u8_DeviceID[2] = u8Ptr_DeviceID[2];
        bSaveDevice_ID = true;
        bRetVal = true;
    }

    return bRetVal;
}

void FA_PROTOCOL_COMMON_Get_IPv4_Address(bool* bPtr_DHCP_Enabled, IPV4_ADDR* tPtr_NewIP, SUBNET_MASK* tPtr_Subnet, uint16_t* u16Ptr_Port) {
    tempStaticIP = FA_TCPIP_Get_IP();
    tempStaticMask = FA_TCPIP_Get_Mask();

    *bPtr_DHCP_Enabled = tempDCHP;
    *tPtr_NewIP = tempStaticIP;
    *tPtr_Subnet = tempStaticMask;
    *u16Ptr_Port = tempStaticPort.port;
}

void FA_PROTOCOL_COMMON_Set_IPv4_Address(bool b_DHCP_Enabled, IPV4_ADDR t_NewIP, SUBNET_MASK t_Subnet, uint16_t u16_Port) {
    tempDCHP = b_DHCP_Enabled;

    if (!b_DHCP_Enabled) {
        tempStaticIP = t_NewIP;
        tempStaticMask = t_Subnet;
    }

    if (u16_Port != 0) {
        tempStaticPort.port = u16_Port;
    }

    bSaveDevice_IPv4 = true;
}

void FA_PROTOCOL_COMMON_Get_MAC(uint8_t* u8Ptr_MAC) {
    TCPIP_MAC_ADDR mTempMAC;

    mTempMAC = FA_TCPIP_Get_MAC_Address();

    u8Ptr_MAC[0] = mTempMAC.v[0];
    u8Ptr_MAC[1] = mTempMAC.v[1];
    u8Ptr_MAC[2] = mTempMAC.v[2];
    u8Ptr_MAC[3] = mTempMAC.v[3];
    u8Ptr_MAC[4] = mTempMAC.v[4];
    u8Ptr_MAC[5] = mTempMAC.v[5];
}

void FA_PROTOCOL_COMMON_Get_Broadcast_ID(uint8_t* u8Ptr_Broadcast_ID) {
    memcpy(u8Ptr_Broadcast_ID, u8_BroadcastID, DEVICE_ID_LENGTH);
}

bool FA_PROTOCOL_COMMON_Check_For_Device_ID(t_USART_ID Target_ID) {

    bool bRetVal = false;

    if (((u8_DeviceID[0] & 0x00FF) == Target_ID.u8_USART_ID_1) &&
        ((u8_DeviceID[1] & 0x00FF) == Target_ID.u8_USART_ID_2) &&
        ((u8_DeviceID[2] & 0x00FF) == Target_ID.u8_USART_ID_3)) {
        bRetVal = true;
    }

    return bRetVal;
}

bool FA_PROTOCOL_COMMON_Check_For_Broadcast_ID(t_USART_ID Target_ID) {

    bool bRetVal = false;

    if (((u8_BroadcastID[0] & 0x00FF) == Target_ID.u8_USART_ID_3) &&
        ((u8_BroadcastID[1] & 0x00FF) == Target_ID.u8_USART_ID_2) &&
        ((u8_BroadcastID[2] & 0x00FF) == Target_ID.u8_USART_ID_1)) {
        bRetVal = true;
    }

    return bRetVal;
}

// *****************************************************************************
// *****************************************************************************
// Section: Protocol Common Internal Functions
// *****************************************************************************
// *****************************************************************************

/**
 * FA_PROTOCOL_COMMON_Set_System_ID sets the "System ID", which is the same as the Device ID
 * but the ID is derived from the MAC address. This function is invoked by FA_PROTOCOL_COMMON_Tasks
 * to write the first instance of the Device ID, and once set is never called again.
 */
void FA_PROTOCOL_COMMON_Set_System_ID(void) {

    TCPIP_MAC_ADDR mTempMAC;

    mTempMAC = FA_TCPIP_Get_MAC_Address();

    if (GET_MAC_OUI(0) == mTempMAC.v[0] && GET_MAC_OUI(1) == mTempMAC.v[1] && GET_MAC_OUI(2) == mTempMAC.v[2]) {
        u8_DeviceID[0] = mTempMAC.v[3];
        u8_DeviceID[1] = mTempMAC.v[4];
        u8_DeviceID[2] = mTempMAC.v[5];
        //lock off from doing this again
        SystemHasSetID = true;
        FA_EEPROM_write_8(EEPROM_SYSTEM_ID_SET, 1); /* KeypadID Enabled */
        //save new id in memory
        bSaveDevice_ID = true;
        FA_PROTOCOL_COMMON_Save_Device_ID();
    }
}

/**
 * FA_PROTOCOL_COMMON_Check_For_System_ID will return true if the "System ID" has not been set
 * already or is equal to all 0s or all Fs.
 * @return
 */
bool FA_PROTOCOL_COMMON_Check_For_System_ID(void) {
    bool bRetVal = false;

    if (SystemHasSetID == false) {
        bRetVal = true;
    } else if ((u8_DeviceID[0] == 0xFF) && (u8_DeviceID[1] == 0xFF) && (u8_DeviceID[2] == 0xFF)) {
        bRetVal = true;
    } else if ((u8_DeviceID[0] == 0x00) && (u8_DeviceID[1] == 0x00) && (u8_DeviceID[2] == 0x00)) {
        bRetVal = true;
    }
    return bRetVal;
}

/**
 * FA_PROTOCOL_COMMON_Save_Device_ID writes the device ID to EEPROM if the flag
 * has been set.
 */
void FA_PROTOCOL_COMMON_Save_Device_ID(void) {
    if (bSaveDevice_ID == true) {
        bSaveDevice_ID = false;
        FA_EEPROM_write_8(EEPROM_DEVICE_ID + 0, u8_DeviceID[0]);
        FA_EEPROM_write_8(EEPROM_DEVICE_ID + 1, u8_DeviceID[1]);
        FA_EEPROM_write_8(EEPROM_DEVICE_ID + 2, u8_DeviceID[2]);
    }
}

/**
 * FA_PROTOCOL_Common_Save_IPv4 writes the IPv4 settings to EEPROM if the flag
 * has been set.
 */
void FA_PROTOCOL_COMMON_Save_IPv4(void) {
    if (bSaveDevice_IPv4) {
        bSaveDevice_IPv4 = false;

        FA_EEPROM_write_8(EEPROM_DHCP, tempDCHP);
        FA_EEPROM_write_block(EEPROM_STATIC_IP, (uint8_t *) & tempStaticIP, 4);
        FA_EEPROM_write_block(EEPROM_STATIC_MASK, (uint8_t *) & tempStaticMask, 4);
        FA_EEPROM_write_16(EEPROM_PORT_ADDR, tempStaticPort.port);

        FA_TCPIP_Reload_Configuration();
    }
}

/**
 * FA_PROTOCOL_COMMON_Save_Label writes the device label to EEPROM if the flag
 * has been set.
 */
void FA_PROTOCOL_COMMON_Save_Label(void) {
    if (bSaveSystem_Label) {
        bSaveSystem_Label = false;
        FA_EEPROM_write_block(EEPROM_DEVICE_LABEL, u8_DeviceLabel, LABEL_LENGTH);
    }
}
