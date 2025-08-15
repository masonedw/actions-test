/*******************************************************************************
  System Initialization File

  File Name:
    initialization.c

  Summary:
    This file contains source code necessary to initialize the system.

  Description:
    This file contains source code necessary to initialize the system.  It
    implements the "SYS_Initialize" function, defines the configuration bits,
    and allocates any necessary global system resources,
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
 * Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "configuration.h"
#include "definitions.h"
#include "device.h"

//******************************************************************
// ALTERATION TO HARMONY GENERATED CODE - DO NOT REPLACE
//------------------------------------------------------------------
#include "framework.h"
//******************************************************************
// END OF ALTERATION
//******************************************************************

// ****************************************************************************
// ****************************************************************************
// Section: Configuration Bits
// ****************************************************************************
// ****************************************************************************
#pragma config BOD33_DIS = SET
#pragma config BOD33USERLEVEL = 0x1cU
#pragma config BOD33_ACTION = RESET
#pragma config BOD33_HYST = 0x2U
#pragma config NVMCTRL_BOOTPROT = 0

//******************************************************************
// ALTERATION TO HARMONY GENERATED CODE - DO NOT REPLACE
//      SEEBLK & PSZ are relocated below to offer a range of
//      SmartEEPROM memory spaces
//------------------------------------------------------------------

#pragma config RAMECC_ECCDIS = SET
#pragma config WDT_ENABLE = CLEAR
#pragma config WDT_ALWAYSON = CLEAR
#pragma config WDT_PER = CYC8192
#pragma config WDT_WINDOW = CYC8192
#pragma config WDT_EWOFFSET = CYC8192
#pragma config WDT_WEN = CLEAR
#pragma config NVMCTRL_REGION_LOCKS = 0xffffffffU


//******************************************************************
// ALTERATION TO HARMONY GENERATED CODE - DO NOT REPLACE
//     SMART EEPROM memory spaces
//------------------------------------------------------------------


// No SmartEEPROM
#ifndef FRAMEWORK_SMART_EEPROM_SIZE
#pragma config NVMCTRL_SEESBLK = 0x0
#pragma config NVMCTRL_SEEPSZ = 0x0

// 1024 bytes
#elif   FRAMEWORK_SMART_EEPROM_SIZE <= EEPROM_1K
#pragma config NVMCTRL_SEESBLK = 0x1
#pragma config NVMCTRL_SEEPSZ = 0x1

// 2048 bytes
// ----------
#elif FRAMEWORK_SMART_EEPROM_SIZE <= EEPROM_2K
#pragma config NVMCTRL_SEESBLK = 0x1
#pragma config NVMCTRL_SEEPSZ = 0x2

// 4096 bytes
// ----------
#elif FRAMEWORK_SMART_EEPROM_SIZE <= EEPROM_4K
#pragma config NVMCTRL_SEESBLK = 0x1
#pragma config NVMCTRL_SEEPSZ = 0x3

// 8192 bytes
// ----------
#elif FRAMEWORK_SMART_EEPROM_SIZE <= EEPROM_8K
#pragma config NVMCTRL_SEESBLK = 0x2
#pragma config NVMCTRL_SEEPSZ = 0x4

// 16384 bytes
// -----------
#elif FRAMEWORK_SMART_EEPROM_SIZE <= EEPROM_16K
#pragma config NVMCTRL_SEESBLK = 0x3
#pragma config NVMCTRL_SEEPSZ = 0x5

// 32768 bytes
// -----------
#elif FRAMEWORK_SMART_EEPROM_SIZE <= EEPROM_32K
#pragma config NVMCTRL_SEESBLK = 0x5
#pragma config NVMCTRL_SEEPSZ = 0x6

// 65536 bytes
// -----------
#elif FRAMEWORK_SMART_EEPROM_SIZE <= EEPROM_2K
#pragma config NVMCTRL_SEESBLK = 0xA
#pragma config NVMCTRL_SEEPSZ = 0x7
#endif


//******************************************************************
// END OF ALTERATION
//******************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Driver Initialization Data
// *****************************************************************************
// *****************************************************************************
/* Following MISRA-C rules are deviated in the below code block */
/* MISRA C-2012 Rule 11.1 */
/* MISRA C-2012 Rule 11.3 */
/* MISRA C-2012 Rule 11.8 */
/* Forward declaration of PHY initialization data */
const DRV_ETHPHY_INIT tcpipPhyInitData_KSZ8091;

/* Forward declaration of MIIM 0 initialization data */
static const DRV_MIIM_INIT drvMiimInitData_0;

// <editor-fold defaultstate="collapsed" desc="DRV_USART Instance 2 Initialization Data">

static DRV_USART_CLIENT_OBJ drvUSART2ClientObjPool[DRV_USART_CLIENTS_NUMBER_IDX2];

/* USART transmit/receive transfer objects pool */
static DRV_USART_BUFFER_OBJ drvUSART2BufferObjPool[DRV_USART_QUEUE_SIZE_IDX2];

static const DRV_USART_PLIB_INTERFACE drvUsart2PlibAPI = {
    .readCallbackRegister = (DRV_USART_PLIB_READ_CALLBACK_REG) SERCOM1_USART_ReadCallbackRegister,
    .read_t = (DRV_USART_PLIB_READ) SERCOM1_USART_Read,
    .readIsBusy = (DRV_USART_PLIB_READ_IS_BUSY) SERCOM1_USART_ReadIsBusy,
    .readCountGet = (DRV_USART_PLIB_READ_COUNT_GET) SERCOM1_USART_ReadCountGet,
    .readAbort = (DRV_USART_PLIB_READ_ABORT) SERCOM1_USART_ReadAbort,
    .writeCallbackRegister = (DRV_USART_PLIB_WRITE_CALLBACK_REG) SERCOM1_USART_WriteCallbackRegister,
    .write_t = (DRV_USART_PLIB_WRITE) SERCOM1_USART_Write,
    .writeIsBusy = (DRV_USART_PLIB_WRITE_IS_BUSY) SERCOM1_USART_WriteIsBusy,
    .writeCountGet = (DRV_USART_PLIB_WRITE_COUNT_GET) SERCOM1_USART_WriteCountGet,
    .errorGet = (DRV_USART_PLIB_ERROR_GET) SERCOM1_USART_ErrorGet,
    .serialSetup = (DRV_USART_PLIB_SERIAL_SETUP) SERCOM1_USART_SerialSetup
};

static const uint32_t drvUsart2remapDataWidth[] = {0x5, 0x6, 0x7, 0x0, 0x1};
static const uint32_t drvUsart2remapParity[] = {0x2, 0x0, 0x80000, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU};
static const uint32_t drvUsart2remapStopBits[] = {0x0, 0xFFFFFFFFU, 0x40};
static const uint32_t drvUsart2remapError[] = {0x4, 0x0, 0x2};

static const DRV_USART_INTERRUPT_SOURCES drvUSART2InterruptSources = {
    /* Peripheral has more than one interrupt vector */
    .isSingleIntSrc = false,

    /* Peripheral interrupt lines */
    .intSources.multi.usartTxCompleteInt = (int32_t) SERCOM1_1_IRQn,
    .intSources.multi.usartTxReadyInt = (int32_t) SERCOM1_0_IRQn,
    .intSources.multi.usartRxCompleteInt = (int32_t) SERCOM1_2_IRQn,
    .intSources.multi.usartErrorInt = (int32_t) SERCOM1_OTHER_IRQn,
};

static const DRV_USART_INIT drvUsart2InitData = {
    .usartPlib = &drvUsart2PlibAPI,

    /* USART Number of clients */
    .numClients = DRV_USART_CLIENTS_NUMBER_IDX2,

    /* USART Client Objects Pool */
    .clientObjPool = (uintptr_t) & drvUSART2ClientObjPool[0],

    /* Combined size of transmit and receive buffer objects */
    .bufferObjPoolSize = DRV_USART_QUEUE_SIZE_IDX2,

    /* USART transmit and receive buffer buffer objects pool */
    .bufferObjPool = (uintptr_t) & drvUSART2BufferObjPool[0],

    .interruptSources = &drvUSART2InterruptSources,

    .remapDataWidth = drvUsart2remapDataWidth,

    .remapParity = drvUsart2remapParity,

    .remapStopBits = drvUsart2remapStopBits,

    .remapError = drvUsart2remapError,

    .dataWidth = DRV_USART_DATA_8_BIT,
};

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="DRV_USART Instance 1 Initialization Data">

static DRV_USART_CLIENT_OBJ drvUSART1ClientObjPool[DRV_USART_CLIENTS_NUMBER_IDX1];

/* USART transmit/receive transfer objects pool */
static DRV_USART_BUFFER_OBJ drvUSART1BufferObjPool[DRV_USART_QUEUE_SIZE_IDX1];

static const DRV_USART_PLIB_INTERFACE drvUsart1PlibAPI = {
    .readCallbackRegister = (DRV_USART_PLIB_READ_CALLBACK_REG) SERCOM0_USART_ReadCallbackRegister,
    .read_t = (DRV_USART_PLIB_READ) SERCOM0_USART_Read,
    .readIsBusy = (DRV_USART_PLIB_READ_IS_BUSY) SERCOM0_USART_ReadIsBusy,
    .readCountGet = (DRV_USART_PLIB_READ_COUNT_GET) SERCOM0_USART_ReadCountGet,
    .readAbort = (DRV_USART_PLIB_READ_ABORT) SERCOM0_USART_ReadAbort,
    .writeCallbackRegister = (DRV_USART_PLIB_WRITE_CALLBACK_REG) SERCOM0_USART_WriteCallbackRegister,
    .write_t = (DRV_USART_PLIB_WRITE) SERCOM0_USART_Write,
    .writeIsBusy = (DRV_USART_PLIB_WRITE_IS_BUSY) SERCOM0_USART_WriteIsBusy,
    .writeCountGet = (DRV_USART_PLIB_WRITE_COUNT_GET) SERCOM0_USART_WriteCountGet,
    .errorGet = (DRV_USART_PLIB_ERROR_GET) SERCOM0_USART_ErrorGet,
    .serialSetup = (DRV_USART_PLIB_SERIAL_SETUP) SERCOM0_USART_SerialSetup
};

static const uint32_t drvUsart1remapDataWidth[] = {0x5, 0x6, 0x7, 0x0, 0x1};
static const uint32_t drvUsart1remapParity[] = {0x2, 0x0, 0x80000, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU};
static const uint32_t drvUsart1remapStopBits[] = {0x0, 0xFFFFFFFFU, 0x40};
static const uint32_t drvUsart1remapError[] = {0x4, 0x0, 0x2};

static const DRV_USART_INTERRUPT_SOURCES drvUSART1InterruptSources = {
    /* Peripheral has more than one interrupt vector */
    .isSingleIntSrc = false,

    /* Peripheral interrupt lines */
    .intSources.multi.usartTxCompleteInt = (int32_t) SERCOM0_1_IRQn,
    .intSources.multi.usartTxReadyInt = (int32_t) SERCOM0_0_IRQn,
    .intSources.multi.usartRxCompleteInt = (int32_t) SERCOM0_2_IRQn,
    .intSources.multi.usartErrorInt = (int32_t) SERCOM0_OTHER_IRQn,
};

static const DRV_USART_INIT drvUsart1InitData = {
    .usartPlib = &drvUsart1PlibAPI,

    /* USART Number of clients */
    .numClients = DRV_USART_CLIENTS_NUMBER_IDX1,

    /* USART Client Objects Pool */
    .clientObjPool = (uintptr_t) & drvUSART1ClientObjPool[0],

    /* Combined size of transmit and receive buffer objects */
    .bufferObjPoolSize = DRV_USART_QUEUE_SIZE_IDX1,

    /* USART transmit and receive buffer buffer objects pool */
    .bufferObjPool = (uintptr_t) & drvUSART1BufferObjPool[0],

    .interruptSources = &drvUSART1InterruptSources,

    .remapDataWidth = drvUsart1remapDataWidth,

    .remapParity = drvUsart1remapParity,

    .remapStopBits = drvUsart1remapStopBits,

    .remapError = drvUsart1remapError,

    .dataWidth = DRV_USART_DATA_8_BIT,
};

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="DRV_USART Instance 0 Initialization Data">

static DRV_USART_CLIENT_OBJ drvUSART0ClientObjPool[DRV_USART_CLIENTS_NUMBER_IDX0];

/* USART transmit/receive transfer objects pool */
static DRV_USART_BUFFER_OBJ drvUSART0BufferObjPool[DRV_USART_QUEUE_SIZE_IDX0];

static const DRV_USART_PLIB_INTERFACE drvUsart0PlibAPI = {
    .readCallbackRegister = (DRV_USART_PLIB_READ_CALLBACK_REG) SERCOM5_USART_ReadCallbackRegister,
    .read_t = (DRV_USART_PLIB_READ) SERCOM5_USART_Read,
    .readIsBusy = (DRV_USART_PLIB_READ_IS_BUSY) SERCOM5_USART_ReadIsBusy,
    .readCountGet = (DRV_USART_PLIB_READ_COUNT_GET) SERCOM5_USART_ReadCountGet,
    .readAbort = (DRV_USART_PLIB_READ_ABORT) SERCOM5_USART_ReadAbort,
    .writeCallbackRegister = (DRV_USART_PLIB_WRITE_CALLBACK_REG) SERCOM5_USART_WriteCallbackRegister,
    .write_t = (DRV_USART_PLIB_WRITE) SERCOM5_USART_Write,
    .writeIsBusy = (DRV_USART_PLIB_WRITE_IS_BUSY) SERCOM5_USART_WriteIsBusy,
    .writeCountGet = (DRV_USART_PLIB_WRITE_COUNT_GET) SERCOM5_USART_WriteCountGet,
    .errorGet = (DRV_USART_PLIB_ERROR_GET) SERCOM5_USART_ErrorGet,
    .serialSetup = (DRV_USART_PLIB_SERIAL_SETUP) SERCOM5_USART_SerialSetup
};

static const uint32_t drvUsart0remapDataWidth[] = {0x5, 0x6, 0x7, 0x0, 0x1};
static const uint32_t drvUsart0remapParity[] = {0x2, 0x0, 0x80000, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU};
static const uint32_t drvUsart0remapStopBits[] = {0x0, 0xFFFFFFFFU, 0x40};
static const uint32_t drvUsart0remapError[] = {0x4, 0x0, 0x2};

static const DRV_USART_INTERRUPT_SOURCES drvUSART0InterruptSources = {
    /* Peripheral has more than one interrupt vector */
    .isSingleIntSrc = false,

    /* Peripheral interrupt lines */
    .intSources.multi.usartTxCompleteInt = (int32_t) SERCOM5_1_IRQn,
    .intSources.multi.usartTxReadyInt = (int32_t) SERCOM5_0_IRQn,
    .intSources.multi.usartRxCompleteInt = (int32_t) SERCOM5_2_IRQn,
    .intSources.multi.usartErrorInt = (int32_t) SERCOM5_OTHER_IRQn,
};

static const DRV_USART_INIT drvUsart0InitData = {
    .usartPlib = &drvUsart0PlibAPI,

    /* USART Number of clients */
    .numClients = DRV_USART_CLIENTS_NUMBER_IDX0,

    /* USART Client Objects Pool */
    .clientObjPool = (uintptr_t) & drvUSART0ClientObjPool[0],

    /* Combined size of transmit and receive buffer objects */
    .bufferObjPoolSize = DRV_USART_QUEUE_SIZE_IDX0,

    /* USART transmit and receive buffer buffer objects pool */
    .bufferObjPool = (uintptr_t) & drvUSART0BufferObjPool[0],

    .interruptSources = &drvUSART0InterruptSources,

    .remapDataWidth = drvUsart0remapDataWidth,

    .remapParity = drvUsart0remapParity,

    .remapStopBits = drvUsart0remapStopBits,

    .remapError = drvUsart0remapError,

    .dataWidth = DRV_USART_DATA_8_BIT,
};

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="DRV_AT24 Initialization Data">

/* I2C PLIB Interface Initialization for AT24 Driver */
static const DRV_AT24_PLIB_INTERFACE drvAT24PlibAPI = {

    /* I2C PLIB WriteRead function */
    .writeRead = (DRV_AT24_PLIB_WRITE_READ) SERCOM7_I2C_WriteRead,

    /* I2C PLIB Write function */
    .write_t = (DRV_AT24_PLIB_WRITE) SERCOM7_I2C_Write,

    /* I2C PLIB Read function */
    .read_t = (DRV_AT24_PLIB_READ) SERCOM7_I2C_Read,

    /* I2C PLIB Transfer Status function */
    .isBusy = (DRV_AT24_PLIB_IS_BUSY) SERCOM7_I2C_IsBusy,

    /* I2C PLIB Error Status function */
    .errorGet = (DRV_AT24_PLIB_ERROR_GET) SERCOM7_I2C_ErrorGet,

    /* I2C PLIB Callback Register */
    .callbackRegister = (DRV_AT24_PLIB_CALLBACK_REGISTER) SERCOM7_I2C_CallbackRegister,
};

/* AT24 Driver Initialization Data */
static const DRV_AT24_INIT drvAT24InitData = {
    /* I2C PLIB API  interface*/
    .i2cPlib = &drvAT24PlibAPI,

    /* 7-bit I2C Slave address */
    .slaveAddress = 0x5e,

    /* EEPROM Page Size in bytes */
    .pageSize = DRV_AT24_EEPROM_PAGE_SIZE,

    /* Total size of the EEPROM in bytes */
    .flashSize = DRV_AT24_EEPROM_FLASH_SIZE,

    /* AT24 Number of clients */
    .numClients = DRV_AT24_CLIENTS_NUMBER_IDX,

    .blockStartAddress = 0x0,
};
// </editor-fold>
/* Forward declaration of GMAC initialization data */
const TCPIP_MODULE_MAC_PIC32C_CONFIG tcpipGMACInitData;





// *****************************************************************************
// *****************************************************************************
// Section: System Data
// *****************************************************************************
// *****************************************************************************
/* Structure to hold the object handles for the modules in the system. */
SYSTEM_OBJECTS sysObj;

// *****************************************************************************
// *****************************************************************************
// Section: Library/Stack Initialization Data
// *****************************************************************************
// *****************************************************************************
/*** KSZ8091 PHY Driver Time-Out Initialization Data ***/
DRV_ETHPHY_TMO drvksz8091Tmo = {
    .resetTmo = DRV_ETHPHY_KSZ8091_RESET_CLR_TMO,
    .aNegDoneTmo = DRV_ETHPHY_KSZ8091_NEG_DONE_TMO,
    .aNegInitTmo = DRV_ETHPHY_KSZ8091_NEG_INIT_TMO,
};

/*** ETH PHY Initialization Data ***/
const DRV_ETHPHY_INIT tcpipPhyInitData_KSZ8091 = {
    .ethphyId = DRV_KSZ8091_PHY_PERIPHERAL_ID,
    .phyAddress = DRV_KSZ8091_PHY_ADDRESS,
    .phyFlags = DRV_KSZ8091_PHY_CONFIG_FLAGS,
    .pPhyObject = &DRV_ETHPHY_OBJECT_KSZ8091,
    .resetFunction = 0,
    .ethphyTmo = &drvksz8091Tmo,
    .pMiimObject = &DRV_MIIM_OBJECT_BASE_Default,
    .pMiimInit = &drvMiimInitData_0,
    .miimIndex = 0,
};


/*** MIIM Driver Instance 0 Configuration ***/
static const DRV_MIIM_INIT drvMiimInitData_0 = {
    .miimId = DRV_MIIM_ETH_MODULE_ID_0,
};

/* Net Presentation Layer Data Definitions */
#include "net_pres/pres/net_pres_enc_glue.h"

static const NET_PRES_TransportObject netPresTransObject0SS = {
    .fpOpen = (NET_PRES_TransOpen) TCPIP_TCP_ServerOpen,
    .fpLocalBind = (NET_PRES_TransBind) TCPIP_TCP_Bind,
    .fpRemoteBind = (NET_PRES_TransBind) TCPIP_TCP_RemoteBind,
    .fpOptionGet = (NET_PRES_TransOption) TCPIP_TCP_OptionsGet,
    .fpOptionSet = (NET_PRES_TransOption) TCPIP_TCP_OptionsSet,
    .fpIsConnected = (NET_PRES_TransBool) TCPIP_TCP_IsConnected,
    .fpWasReset = (NET_PRES_TransBool) TCPIP_TCP_WasReset,
    .fpWasDisconnected = (NET_PRES_TransBool) TCPIP_TCP_WasDisconnected,
    .fpDisconnect = (NET_PRES_TransBool) TCPIP_TCP_Disconnect,
    .fpConnect = (NET_PRES_TransBool) TCPIP_TCP_Connect,
    .fpClose = (NET_PRES_TransClose) TCPIP_TCP_Close,
    .fpSocketInfoGet = (NET_PRES_TransSocketInfoGet) TCPIP_TCP_SocketInfoGet,
    .fpFlush = (NET_PRES_TransBool) TCPIP_TCP_Flush,
    .fpPeek = (NET_PRES_TransPeek) TCPIP_TCP_ArrayPeek,
    .fpDiscard = (NET_PRES_TransDiscard) TCPIP_TCP_Discard,
    .fpHandlerRegister = (NET_PRES_TransHandlerRegister) TCPIP_TCP_SignalHandlerRegister,
    .fpHandlerDeregister = (NET_PRES_TransSignalHandlerDeregister) TCPIP_TCP_SignalHandlerDeregister,
    .fpRead = (NET_PRES_TransRead) TCPIP_TCP_ArrayGet,
    .fpWrite = (NET_PRES_TransWrite) TCPIP_TCP_ArrayPut,
    .fpReadyToRead = (NET_PRES_TransReady) TCPIP_TCP_GetIsReady,
    .fpReadyToWrite = (NET_PRES_TransReady) TCPIP_TCP_PutIsReady,
    .fpIsPortDefaultSecure = (NET_PRES_TransIsPortDefaultSecured) TCPIP_Helper_TCPSecurePortGet,
};
static const NET_PRES_TransportObject netPresTransObject0SC = {
    .fpOpen = (NET_PRES_TransOpen) TCPIP_TCP_ClientOpen,
    .fpLocalBind = (NET_PRES_TransBind) TCPIP_TCP_Bind,
    .fpRemoteBind = (NET_PRES_TransBind) TCPIP_TCP_RemoteBind,
    .fpOptionGet = (NET_PRES_TransOption) TCPIP_TCP_OptionsGet,
    .fpOptionSet = (NET_PRES_TransOption) TCPIP_TCP_OptionsSet,
    .fpIsConnected = (NET_PRES_TransBool) TCPIP_TCP_IsConnected,
    .fpWasReset = (NET_PRES_TransBool) TCPIP_TCP_WasReset,
    .fpWasDisconnected = (NET_PRES_TransBool) TCPIP_TCP_WasDisconnected,
    .fpDisconnect = (NET_PRES_TransBool) TCPIP_TCP_Disconnect,
    .fpConnect = (NET_PRES_TransBool) TCPIP_TCP_Connect,
    .fpClose = (NET_PRES_TransClose) TCPIP_TCP_Close,
    .fpSocketInfoGet = (NET_PRES_TransSocketInfoGet) TCPIP_TCP_SocketInfoGet,
    .fpFlush = (NET_PRES_TransBool) TCPIP_TCP_Flush,
    .fpPeek = (NET_PRES_TransPeek) TCPIP_TCP_ArrayPeek,
    .fpDiscard = (NET_PRES_TransDiscard) TCPIP_TCP_Discard,
    .fpHandlerRegister = (NET_PRES_TransHandlerRegister) TCPIP_TCP_SignalHandlerRegister,
    .fpHandlerDeregister = (NET_PRES_TransSignalHandlerDeregister) TCPIP_TCP_SignalHandlerDeregister,
    .fpRead = (NET_PRES_TransRead) TCPIP_TCP_ArrayGet,
    .fpWrite = (NET_PRES_TransWrite) TCPIP_TCP_ArrayPut,
    .fpReadyToRead = (NET_PRES_TransReady) TCPIP_TCP_GetIsReady,
    .fpReadyToWrite = (NET_PRES_TransReady) TCPIP_TCP_PutIsReady,
    .fpIsPortDefaultSecure = (NET_PRES_TransIsPortDefaultSecured) TCPIP_Helper_TCPSecurePortGet,
};
static const NET_PRES_TransportObject netPresTransObject0DS = {
    .fpOpen = (NET_PRES_TransOpen) TCPIP_UDP_ServerOpen,
    .fpLocalBind = (NET_PRES_TransBind) TCPIP_UDP_Bind,
    .fpRemoteBind = (NET_PRES_TransBind) TCPIP_UDP_RemoteBind,
    .fpOptionGet = (NET_PRES_TransOption) TCPIP_UDP_OptionsGet,
    .fpOptionSet = (NET_PRES_TransOption) TCPIP_UDP_OptionsSet,
    .fpIsConnected = (NET_PRES_TransBool) TCPIP_UDP_IsConnected,
    .fpWasReset = NULL,
    .fpWasDisconnected = NULL,
    .fpDisconnect = (NET_PRES_TransBool) TCPIP_UDP_Disconnect,
    .fpConnect = NULL,
    .fpClose = (NET_PRES_TransClose) TCPIP_UDP_Close,
    .fpSocketInfoGet = (NET_PRES_TransSocketInfoGet) TCPIP_UDP_SocketInfoGet,
    .fpFlush = (NET_PRES_TransBool) TCPIP_UDP_Flush,
    .fpPeek = NULL,
    .fpDiscard = (NET_PRES_TransDiscard) TCPIP_UDP_Discard,
    .fpHandlerRegister = (NET_PRES_TransHandlerRegister) TCPIP_UDP_SignalHandlerRegister,
    .fpHandlerDeregister = (NET_PRES_TransSignalHandlerDeregister) TCPIP_UDP_SignalHandlerDeregister,
    .fpRead = (NET_PRES_TransRead) TCPIP_UDP_ArrayGet,
    .fpWrite = (NET_PRES_TransWrite) TCPIP_UDP_ArrayPut,
    .fpReadyToRead = (NET_PRES_TransReady) TCPIP_UDP_GetIsReady,
    .fpReadyToWrite = (NET_PRES_TransReady) TCPIP_UDP_PutIsReady,
    .fpIsPortDefaultSecure = (NET_PRES_TransIsPortDefaultSecured) TCPIP_Helper_UDPSecurePortGet,
};
static const NET_PRES_TransportObject netPresTransObject0DC = {
    .fpOpen = (NET_PRES_TransOpen) TCPIP_UDP_ClientOpen,
    .fpLocalBind = (NET_PRES_TransBind) TCPIP_UDP_Bind,
    .fpRemoteBind = (NET_PRES_TransBind) TCPIP_UDP_RemoteBind,
    .fpOptionGet = (NET_PRES_TransOption) TCPIP_UDP_OptionsGet,
    .fpOptionSet = (NET_PRES_TransOption) TCPIP_UDP_OptionsSet,
    .fpIsConnected = (NET_PRES_TransBool) TCPIP_UDP_IsConnected,
    .fpWasReset = NULL,
    .fpWasDisconnected = NULL,
    .fpDisconnect = (NET_PRES_TransBool) TCPIP_UDP_Disconnect,
    .fpConnect = NULL,
    .fpClose = (NET_PRES_TransClose) TCPIP_UDP_Close,
    .fpSocketInfoGet = (NET_PRES_TransSocketInfoGet) TCPIP_UDP_SocketInfoGet,
    .fpFlush = (NET_PRES_TransBool) TCPIP_UDP_Flush,
    .fpPeek = NULL,
    .fpDiscard = (NET_PRES_TransDiscard) TCPIP_UDP_Discard,
    .fpHandlerRegister = (NET_PRES_TransHandlerRegister) TCPIP_UDP_SignalHandlerRegister,
    .fpHandlerDeregister = (NET_PRES_TransSignalHandlerDeregister) TCPIP_UDP_SignalHandlerDeregister,
    .fpRead = (NET_PRES_TransRead) TCPIP_UDP_ArrayGet,
    .fpWrite = (NET_PRES_TransWrite) TCPIP_UDP_ArrayPut,
    .fpReadyToRead = (NET_PRES_TransReady) TCPIP_UDP_GetIsReady,
    .fpReadyToWrite = (NET_PRES_TransReady) TCPIP_UDP_PutIsReady,
    .fpIsPortDefaultSecure = (NET_PRES_TransIsPortDefaultSecured) TCPIP_Helper_UDPSecurePortGet,
};

static const NET_PRES_INST_DATA netPresCfgs[] = {

    {
        .pTransObject_ss = &netPresTransObject0SS,
        .pTransObject_sc = &netPresTransObject0SC,
        .pTransObject_ds = &netPresTransObject0DS,
        .pTransObject_dc = &netPresTransObject0DC,
        .pProvObject_ss = NULL,
        .pProvObject_sc = NULL,
        .pProvObject_ds = NULL,
        .pProvObject_dc = NULL,
    },

};

static const NET_PRES_INIT_DATA netPresInitData = {
    .numLayers = sizeof (netPresCfgs) / sizeof (NET_PRES_INST_DATA),
    .pInitData = netPresCfgs
};




// <editor-fold defaultstate="collapsed" desc="TCP/IP Stack Initialization Data">
// *****************************************************************************
// *****************************************************************************
// Section: TCPIP Data
// *****************************************************************************
// *****************************************************************************
/*** ARP Service Initialization Data ***/
const TCPIP_ARP_MODULE_CONFIG tcpipARPInitData = {
    .cacheEntries = TCPIP_ARP_CACHE_ENTRIES,
    .deleteOld = TCPIP_ARP_CACHE_DELETE_OLD,
    .entrySolvedTmo = TCPIP_ARP_CACHE_SOLVED_ENTRY_TMO,
    .entryPendingTmo = TCPIP_ARP_CACHE_PENDING_ENTRY_TMO,
    .entryRetryTmo = TCPIP_ARP_CACHE_PENDING_RETRY_TMO,
    .permQuota = TCPIP_ARP_CACHE_PERMANENT_QUOTA,
    .purgeThres = TCPIP_ARP_CACHE_PURGE_THRESHOLD,
    .purgeQuanta = TCPIP_ARP_CACHE_PURGE_QUANTA,
    .retries = TCPIP_ARP_CACHE_ENTRY_RETRIES,
    .gratProbeCount = TCPIP_ARP_GRATUITOUS_PROBE_COUNT,
};

/*** Announce Discovery Initialization Data ***/
const TCPIP_ANNOUNCE_MODULE_CONFIG tcpipAnnounceInitData = {
    0
};

/*** UDP Sockets Initialization Data ***/
const TCPIP_UDP_MODULE_CONFIG tcpipUDPInitData = {
    .nSockets = TCPIP_UDP_MAX_SOCKETS,
    .sktTxBuffSize = TCPIP_UDP_SOCKET_DEFAULT_TX_SIZE,
};

/*** TCP Sockets Initialization Data ***/
const TCPIP_TCP_MODULE_CONFIG tcpipTCPInitData = {
    .nSockets = TCPIP_TCP_MAX_SOCKETS,
    .sktTxBuffSize = TCPIP_TCP_SOCKET_DEFAULT_TX_SIZE,
    .sktRxBuffSize = TCPIP_TCP_SOCKET_DEFAULT_RX_SIZE,
};






/*** DHCP client Initialization Data ***/
const TCPIP_DHCP_MODULE_CONFIG tcpipDHCPInitData = {
    .dhcpEnable = false,
    .dhcpTmo = TCPIP_DHCP_TIMEOUT,
    .dhcpCliPort = TCPIP_DHCP_CLIENT_CONNECT_PORT,
    .dhcpSrvPort = TCPIP_DHCP_SERVER_LISTEN_PORT,

};

/*** Berkeley API Initialization Data ***/
const BERKELEY_MODULE_CONFIG tcpipBerkeleyInitData = {
    .maxSockets = MAX_BSD_SOCKETS,
};

/*** ICMP Server Initialization Data ***/
const TCPIP_ICMP_MODULE_CONFIG tcpipICMPInitData = {
    0
};












/*** DNS Client Initialization Data ***/
const TCPIP_DNS_CLIENT_MODULE_CONFIG tcpipDNSClientInitData = {
    .deleteOldLease = TCPIP_DNS_CLIENT_DELETE_OLD_ENTRIES,
    .cacheEntries = TCPIP_DNS_CLIENT_CACHE_ENTRIES,
    .entrySolvedTmo = TCPIP_DNS_CLIENT_CACHE_ENTRY_TMO,
    .nIPv4Entries = TCPIP_DNS_CLIENT_CACHE_PER_IPV4_ADDRESS,
    .ipAddressType = TCPIP_DNS_CLIENT_ADDRESS_TYPE,
    .nIPv6Entries = TCPIP_DNS_CLIENT_CACHE_PER_IPV6_ADDRESS,
};



/*** IPv4 Initialization Data ***/


const TCPIP_IPV4_MODULE_CONFIG tcpipIPv4InitData = {
    .arpEntries = TCPIP_IPV4_ARP_SLOTS,
};






TCPIP_STACK_HEAP_INTERNAL_CONFIG tcpipHeapConfig = {
    .heapType = TCPIP_STACK_HEAP_TYPE_INTERNAL_HEAP,
    .heapFlags = TCPIP_STACK_HEAP_USE_FLAGS,
    .heapUsage = TCPIP_STACK_HEAP_USAGE_CONFIG,
    .malloc_fnc = TCPIP_STACK_MALLOC_FUNC,
    .free_fnc = TCPIP_STACK_FREE_FUNC,
    .heapSize = TCPIP_STACK_DRAM_SIZE,
};


const TCPIP_NETWORK_CONFIG __attribute__((unused)) TCPIP_HOSTS_CONFIGURATION[] = {
    /*** Network Configuration Index 0 ***/
    {
        .interface = TCPIP_NETWORK_DEFAULT_INTERFACE_NAME_IDX0,
        .hostName = TCPIP_NETWORK_DEFAULT_HOST_NAME_IDX0,

        //******************************************************************
        // ALTERATION TO HARMONY GENERATED CODE - DO NOT REPLACE
        //     the const * char macAddr has been removed and is replaced by
        //     a configurable MAC address supplied via the Future EEPROM
        //------------------------------------------------------------------

        .ipAddr = TCPIP_NETWORK_DEFAULT_IP_ADDRESS_IDX0,
        .ipMask = TCPIP_NETWORK_DEFAULT_IP_MASK_IDX0,
        .gateway = TCPIP_NETWORK_DEFAULT_GATEWAY_IDX0,
        .priDNS = TCPIP_NETWORK_DEFAULT_DNS_IDX0,
        .secondDNS = TCPIP_NETWORK_DEFAULT_SECOND_DNS_IDX0,
        .powerMode = TCPIP_NETWORK_DEFAULT_POWER_MODE_IDX0,
        .startFlags = TCPIP_NETWORK_DEFAULT_INTERFACE_FLAGS_IDX0,
        .pMacObject = &TCPIP_NETWORK_DEFAULT_MAC_DRIVER_IDX0,
    },
};

const size_t TCPIP_HOSTS_CONFIGURATION_SIZE = sizeof (TCPIP_HOSTS_CONFIGURATION) / sizeof (*TCPIP_HOSTS_CONFIGURATION);

const TCPIP_STACK_MODULE_CONFIG TCPIP_STACK_MODULE_CONFIG_TBL [] = {
    {TCPIP_MODULE_IPV4, &tcpipIPv4InitData},

    {TCPIP_MODULE_ICMP, 0}, // TCPIP_MODULE_ICMP

    {TCPIP_MODULE_ARP, &tcpipARPInitData}, // TCPIP_MODULE_ARP
    {TCPIP_MODULE_UDP, &tcpipUDPInitData}, // TCPIP_MODULE_UDP
    {TCPIP_MODULE_TCP, &tcpipTCPInitData}, // TCPIP_MODULE_TCP
    {TCPIP_MODULE_DHCP_CLIENT, &tcpipDHCPInitData}, // TCPIP_MODULE_DHCP_CLIENT
    {TCPIP_MODULE_ANNOUNCE, &tcpipAnnounceInitData}, // TCPIP_MODULE_ANNOUNCE
    {TCPIP_MODULE_DNS_CLIENT, &tcpipDNSClientInitData}, // TCPIP_MODULE_DNS_CLIENT

    {TCPIP_MODULE_BERKELEY, &tcpipBerkeleyInitData}, // TCPIP_MODULE_BERKELEY
    {TCPIP_MODULE_COMMAND, 0}, // TCPIP_MODULE_COMMAND,
    { TCPIP_MODULE_MANAGER, &tcpipHeapConfig}, // TCPIP_MODULE_MANAGER

    // MAC modules
    {TCPIP_MODULE_MAC_PIC32C, &tcpipGMACInitData}, // TCPIP_MODULE_MAC_PIC32C

};

const size_t TCPIP_STACK_MODULE_CONFIG_TBL_SIZE = sizeof (TCPIP_STACK_MODULE_CONFIG_TBL) / sizeof (*TCPIP_STACK_MODULE_CONFIG_TBL);

/*********************************************************************
 * Function:        SYS_MODULE_OBJ TCPIP_STACK_Init()
 *
 * PreCondition:    None
 *
 * Input:
 *
 * Output:          valid system module object if Stack and its componets are initialized
 *                  SYS_MODULE_OBJ_INVALID otherwise
 *
 * Overview:        The function starts the initialization of the stack.
 *                  If an error occurs, the SYS_ERROR() is called
 *                  and the function de-initialize itself and will return false.
 *
 * Side Effects:    None
 *
 * Note:            This function must be called before any of the
 *                  stack or its component routines are used.
 *
 ********************************************************************/


SYS_MODULE_OBJ TCPIP_STACK_Init(void) {
    TCPIP_STACK_INIT tcpipInit;

    tcpipInit.pNetConf = TCPIP_HOSTS_CONFIGURATION;
    tcpipInit.nNets = TCPIP_HOSTS_CONFIGURATION_SIZE;
    tcpipInit.pModConfig = TCPIP_STACK_MODULE_CONFIG_TBL;
    tcpipInit.nModules = TCPIP_STACK_MODULE_CONFIG_TBL_SIZE;
    tcpipInit.initCback = 0;

    return TCPIP_STACK_Initialize(0, &tcpipInit.moduleInit);
}
// </editor-fold>


uint8_t txPrioNumToQueIndxGmac [DRV_GMAC_NUMBER_OF_QUEUES];
uint8_t rxPrioNumToQueIndxGmac [DRV_GMAC_NUMBER_OF_QUEUES];

/*** GMAC Initialization Data ***/
TCPIP_MODULE_GMAC_QUEUE_CONFIG gmac_queue_config[DRV_GMAC_NUMBER_OF_QUEUES] = {
    { /** QUEUE 0 Initialization**/
        .queueTxEnable = true,
        .queueRxEnable = true,
        .nRxDescCnt = 8,
        .nTxDescCnt = 8,
        .rxBufferSize = 1536,
        .txMaxPktSize = 1536,
        .nRxDedicatedBuffers = 8,
        .nRxAddlBuffCount = 2,
        .nRxBuffCntThres = 1,
        .nRxBuffAllocCnt = 2,
        .queueIntSrc = GMAC_IRQn,
    },
};

const TCPIP_MODULE_MAC_PIC32C_CONFIG tcpipGMACInitData = {
    .gmac_queue_config = gmac_queue_config,
    .macQueNum = DRV_GMAC_NUMBER_OF_QUEUES,
    .txPrioNumToQueIndx = txPrioNumToQueIndxGmac,
    .rxPrioNumToQueIndx = rxPrioNumToQueIndxGmac,
    .ethFlags = TCPIP_GMAC_ETH_OPEN_FLAGS,
    .linkInitDelay = DRV_KSZ8091_PHY_LINK_INIT_DELAY,
    .ethModuleId = TCPIP_GMAC_MODULE_ID,
    .pPhyBase = &DRV_ETHPHY_OBJECT_BASE_Default,
    .pPhyInit = &tcpipPhyInitData_KSZ8091,
    .checksumOffloadRx = DRV_GMAC_RX_CHKSM_OFFLOAD,
    .checksumOffloadTx = DRV_GMAC_TX_CHKSM_OFFLOAD,
    .macTxPrioNum = TCPIP_GMAC_TX_PRIO_COUNT,
    .macRxPrioNum = TCPIP_GMAC_RX_PRIO_COUNT,
    .macRxFilt = TCPIP_GMAC_RX_FILTERS,
};




// *****************************************************************************
// *****************************************************************************
// Section: System Initialization
// *****************************************************************************
// *****************************************************************************
// <editor-fold defaultstate="collapsed" desc="SYS_TIME Initialization Data">

static const SYS_TIME_PLIB_INTERFACE sysTimePlibAPI = {
    .timerCallbackSet = (SYS_TIME_PLIB_CALLBACK_REGISTER) TC0_TimerCallbackRegister,
    .timerStart = (SYS_TIME_PLIB_START) TC0_TimerStart,
    .timerStop = (SYS_TIME_PLIB_STOP) TC0_TimerStop,
    .timerFrequencyGet = (SYS_TIME_PLIB_FREQUENCY_GET) TC0_TimerFrequencyGet,
    .timerPeriodSet = (SYS_TIME_PLIB_PERIOD_SET) TC0_Timer32bitPeriodSet,
    .timerCompareSet = (SYS_TIME_PLIB_COMPARE_SET) TC0_Timer32bitCompareSet,
    .timerCounterGet = (SYS_TIME_PLIB_COUNTER_GET) TC0_Timer32bitCounterGet,
};

static const SYS_TIME_INIT sysTimeInitData = {
    .timePlib = &sysTimePlibAPI,
    .hwTimerIntNum = TC0_IRQn,
};

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="SYS_CONSOLE Instance 0 Initialization Data">


static const SYS_CONSOLE_UART_PLIB_INTERFACE sysConsole0UARTPlibAPI = {
    .read_t = (SYS_CONSOLE_UART_PLIB_READ) SERCOM2_USART_Read,
    .readCountGet = (SYS_CONSOLE_UART_PLIB_READ_COUNT_GET) SERCOM2_USART_ReadCountGet,
    .readFreeBufferCountGet = (SYS_CONSOLE_UART_PLIB_READ_FREE_BUFFFER_COUNT_GET) SERCOM2_USART_ReadFreeBufferCountGet,
    .write_t = (SYS_CONSOLE_UART_PLIB_WRITE) SERCOM2_USART_Write,
    .writeCountGet = (SYS_CONSOLE_UART_PLIB_WRITE_COUNT_GET) SERCOM2_USART_WriteCountGet,
    .writeFreeBufferCountGet = (SYS_CONSOLE_UART_PLIB_WRITE_FREE_BUFFER_COUNT_GET) SERCOM2_USART_WriteFreeBufferCountGet,
};

static const SYS_CONSOLE_UART_INIT_DATA sysConsole0UARTInitData = {
    .uartPLIB = &sysConsole0UARTPlibAPI,
};

static const SYS_CONSOLE_INIT sysConsole0Init = {
    .deviceInitData = (const void*) &sysConsole0UARTInitData,
    .consDevDesc = &sysConsoleUARTDevDesc,
    .deviceIndex = 0,
};



// </editor-fold>



const SYS_CMD_INIT sysCmdInit = {
    .moduleInit =
    {0},
    .consoleCmdIOParam = SYS_CMD_SINGLE_CHARACTER_READ_CONSOLE_IO_PARAM,
    .consoleIndex = 0,
};


static const SYS_DEBUG_INIT debugInit = {
    .moduleInit =
    {0},
    .errorLevel = SYS_DEBUG_GLOBAL_ERROR_LEVEL,
    .consoleIndex = 0,
};





// *****************************************************************************
// *****************************************************************************
// Section: Local initialization functions
// *****************************************************************************
// *****************************************************************************

/* MISRAC 2012 deviation block end */

/*******************************************************************************
  Function:
    void SYS_Initialize ( void *data )

  Summary:
    Initializes the board, services, drivers, application and other modules.

  Remarks:
 */

void SYS_Initialize(void* data) {

    /* MISRAC 2012 deviation block start */
    /* MISRA C-2012 Rule 2.2 deviated in this file.  Deviation record ID -  H3_MISRAC_2012_R_2_2_DR_1 */

    NVMCTRL_Initialize();


    PORT_Initialize();

    CLOCK_Initialize();




    BSP_Initialize();
    TCC0_PWMInitialize();

    SUPC_Initialize();

    TC0_TimerInitialize();

    TC2_TimerInitialize();

    TC7_TimerInitialize();

    SERCOM2_USART_Initialize();

    SERCOM1_USART_Initialize();

    EVSYS_Initialize();

    SERCOM0_USART_Initialize();

    SERCOM7_I2C_Initialize();

    SERCOM5_USART_Initialize();

    EIC_Initialize();



    /* MISRAC 2012 deviation block start */
    /* Following MISRA-C rules deviated in this block  */
    /* MISRA C-2012 Rule 11.3 - Deviation record ID - H3_MISRAC_2012_R_11_3_DR_1 */
    /* MISRA C-2012 Rule 11.8 - Deviation record ID - H3_MISRAC_2012_R_11_8_DR_1 */


    /* Initialize the MIIM Driver Instance 0*/
    sysObj.drvMiim_0 = DRV_MIIM_OBJECT_BASE_Default.DRV_MIIM_Initialize(DRV_MIIM_DRIVER_INDEX_0, (const SYS_MODULE_INIT *) &drvMiimInitData_0);

    sysObj.drvUsart2 = DRV_USART_Initialize(DRV_USART_INDEX_2, (SYS_MODULE_INIT *) & drvUsart2InitData);

    sysObj.drvUsart1 = DRV_USART_Initialize(DRV_USART_INDEX_1, (SYS_MODULE_INIT *) & drvUsart1InitData);

    sysObj.drvUsart0 = DRV_USART_Initialize(DRV_USART_INDEX_0, (SYS_MODULE_INIT *) & drvUsart0InitData);

    sysObj.drvAT24 = DRV_AT24_Initialize(DRV_AT24_INDEX, (SYS_MODULE_INIT *) & drvAT24InitData);


    /* MISRA C-2012 Rule 11.3, 11.8 deviated below. Deviation record ID -
    H3_MISRAC_2012_R_11_3_DR_1 & H3_MISRAC_2012_R_11_8_DR_1*/

    sysObj.sysTime = SYS_TIME_Initialize(SYS_TIME_INDEX_0, (SYS_MODULE_INIT *) & sysTimeInitData);

    /* MISRAC 2012 deviation block end */
    /* MISRA C-2012 Rule 11.3, 11.8 deviated below. Deviation record ID -
     H3_MISRAC_2012_R_11_3_DR_1 & H3_MISRAC_2012_R_11_8_DR_1*/
    sysObj.sysConsole0 = SYS_CONSOLE_Initialize(SYS_CONSOLE_INDEX_0, (SYS_MODULE_INIT *) & sysConsole0Init);
    /* MISRAC 2012 deviation block end */
    SYS_CMD_Initialize((SYS_MODULE_INIT*) & sysCmdInit);

    /* MISRA C-2012 Rule 11.3, 11.8 deviated below. Deviation record ID -
     H3_MISRAC_2012_R_11_3_DR_1 & H3_MISRAC_2012_R_11_8_DR_1*/

    sysObj.sysDebug = SYS_DEBUG_Initialize(SYS_DEBUG_INDEX_0, (SYS_MODULE_INIT*) & debugInit);

    /* MISRAC 2012 deviation block end */


    /* Network Presentation Layer Initialization */
    sysObj.netPres = NET_PRES_Initialize(0, (SYS_MODULE_INIT*) & netPresInitData);
    /* TCPIP Stack Initialization */
    sysObj.tcpip = TCPIP_STACK_Init();
    SYS_ASSERT(sysObj.tcpip != SYS_MODULE_OBJ_INVALID, "TCPIP_STACK_Init Failed");


    CRYPT_WCCB_Initialize();

    NVIC_Initialize();


    /* MISRAC 2012 deviation block end */
}

//******************************************************************
// ALTERATION TO HARMONY GENERATED CODE - DO NOT REPLACE
//     SMART EEPROM memory spaces
//------------------------------------------------------------------

//void SYS_InitialiseTCP() {
//    /* Network Presentation Layer Initialization */
//    sysObj.netPres = NET_PRES_Initialize(0, (SYS_MODULE_INIT*) & netPresInitData);
//    /* TCPIP Stack Initialization */
//    sysObj.tcpip = TCPIP_STACK_Init();
//    SYS_ASSERT(sysObj.tcpip != SYS_MODULE_OBJ_INVALID, "TCPIP_STACK_Init Failed");
//}
//******************************************************************
// END OF ALTERATION
//******************************************************************


/*******************************************************************************
 End of File
 */
