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
  Future Automation Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    fa_rs485.c

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
#include "fa_at24.h"
#include "fa_eeprom.h"


// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define AT24_EEPROM_MEM_ADDR             0x00
#define AT24_EUI_ADDR                    0x9A


// *****************************************************************************
// *****************************************************************************
// Section: Global Data Types
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
FA_AT24_DATA at24Data;
uint8_t mac_oui[3] = {0xFC, 0xC2, 0x3D};


// *****************************************************************************
// *****************************************************************************
// Section: Forward references
// *****************************************************************************
// *****************************************************************************



// *****************************************************************************
// *****************************************************************************
// Section: Initialisation
// *****************************************************************************
// *****************************************************************************

static void eeprom_EventHandler(DRV_AT24_TRANSFER_STATUS event, uintptr_t context) {
    switch (event) {
        case DRV_AT24_TRANSFER_STATUS_COMPLETED:
            at24Data.isTransferDone = true;
            break;
        case DRV_AT24_TRANSFER_STATUS_ERROR:
        default:
            at24Data.isTransferDone = false;
            at24Data.state = FA_AT24_STATE_ERROR;
            break;
    }
}

void FA_AT24_Init(void) {
    /* Place the App state machine in its initial state. */
    at24Data.state = FA_AT24_STATE_INIT;

    at24Data.isTransferDone = false;

}

void FA_AT24_Tasks(void) {
    //uint32_t i;
    /* Check the application's current state. */
    switch (at24Data.state) {
            /* Application's initial state. */
        case FA_AT24_STATE_INIT:

            at24Data.drvHandle = DRV_AT24_Open(DRV_AT24_INDEX, DRV_IO_INTENT_READWRITE);

            if (at24Data.drvHandle != DRV_HANDLE_INVALID) {
                DRV_AT24_EventHandlerSet(at24Data.drvHandle, eeprom_EventHandler, 0);

                at24Data.state = FA_AT24_STATE_READ;
            } else {
                at24Data.state = FA_AT24_STATE_ERROR;
            }
            break;

            //case FA_AT24_STATE_WRITE:

            //    /* Fill up the write buffer */
            //    for (i = 0; i < BUFFER_SIZE; i++)
            //    {
            //        at24Data.writeBuffer[i] = i;
            //    }

            /* Set the next state first as callback may be fired before the state
             * is changed; potentially over-writing error state set from the callback */
            //    at24Data.state = FA_AT24_STATE_WAIT_WRITE_COMPLETE;

            //    if (DRV_AT24_Write(at24Data.drvHandle,
            //            at24Data.writeBuffer,
            //            BUFFER_SIZE,
            //            AT24_EEPROM_MEM_ADDR) == false)
            //    {
            //        at24Data.state = FA_AT24_STATE_ERROR;
            //    }
            //    break;

            //case FA_AT24_STATE_WAIT_WRITE_COMPLETE:

            //    if (at24Data.isTransferDone == true)
            //    {
            //        at24Data.isTransferDone = false;
            //        at24Data.state = FA_AT24_STATE_READ;
            //    }
            //    break;

        case FA_AT24_STATE_READ:

            at24Data.state = FA_AT24_STATE_WAIT_READ_COMPLETE;

            if (DRV_AT24_Read(at24Data.drvHandle,
                    at24Data.readBuffer,
                    6,
                    AT24_EUI_ADDR) == false) {
                at24Data.state = FA_AT24_STATE_ERROR;
            }
            break;

        case FA_AT24_STATE_WAIT_READ_COMPLETE:

            if (at24Data.isTransferDone == true) {
                at24Data.isTransferDone = false;
                at24Data.state = FA_AT24_STATE_VERIFY;
            }
            break;

        case FA_AT24_STATE_VERIFY:
            if (memcmp(mac_oui, at24Data.readBuffer, 3) == 0) {
                uint32_t seed;

                at24Data.state = FA_AT24_STATE_SUCCESS;
                FA_EEPROM_write_mac(at24Data.readBuffer);

                // Once the mac has been retrieved, use it to seed rand.
                // Interpreting the result of memcpy will look wrong becasue
                // of the endianess, but it doesnt matter as long as the mac is unique.
                memcpy(&seed, at24Data.readBuffer + 2, sizeof (seed));
                srand(seed);

            } else {
                at24Data.state = FA_AT24_STATE_ERROR;
            }
            break;

        case FA_AT24_STATE_SUCCESS:
            /* Turn on the LED to indicate success */
            at24Data.state = FA_AT24_STATE_IDLE;
            break;

        case FA_AT24_STATE_ERROR:
            at24Data.state = FA_AT24_STATE_IDLE;
            break;

        case FA_AT24_STATE_IDLE:
        default:
            break;
    }
}

_Bool FA_AT24_get_mac(uint8_t * dest) {
    _Bool bRetVal = false;

    uint8_t ee_mac_address[6];

    FA_EEPROM_read_mac(ee_mac_address);

    if (memcmp(mac_oui, ee_mac_address, 3) == 0) {
        memcpy(dest, ee_mac_address, 6);
        bRetVal = true;
    }
    return bRetVal;
}

uint8_t GET_MAC_OUI(uint8_t Target) {
    return mac_oui[Target];
}

/*******************************************************************************
 End of File
 */