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
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    fa_eeprom.c

  Summary:
    This file contains the source code for utlizing Smart EEPROM.


 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "framework.h"

//#define EEPROM_TOUCH    0x0000
//#define EEPROM_MAC      0x0001


extern void FA_APP_reset_eeprom(void);

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************




// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************





// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************





// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************





// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

void FA_EEPROM_init(void) {
    if (FRAMEWORK_read_eeprom_8(EEPROM_TOUCH) != EEPROM_INITIALISED_SIGNATURE) {
        FA_APP_reset_eeprom();
        FRAMEWORK_write_eeprom_8(EEPROM_TOUCH, EEPROM_INITIALISED_SIGNATURE);
    }
}

_Bool FA_EEPROM_write_8(uint16_t u16_addr, uint8_t u8_data) {
    _Bool bRetVal = false;

    if (u16_addr <= FRAMEWORK_SMART_EEPROM_SIZE) {
        FRAMEWORK_write_eeprom_8(u16_addr, u8_data);
        bRetVal = true;
    }
    return bRetVal;
}

_Bool FA_EEPROM_write_16(uint16_t u16_addr, uint16_t u16_data) {
    _Bool bRetVal = false;

    if (u16_addr + 1 <= FRAMEWORK_SMART_EEPROM_SIZE) {
        FRAMEWORK_write_eeprom_16(u16_addr, u16_data);
        bRetVal = true;
    }
    return bRetVal;
}

_Bool FA_EEPROM_write_32(uint16_t u16_addr, uint32_t u32_data) {
    _Bool bRetVal = false;

    if (u16_addr + 3 <= FRAMEWORK_SMART_EEPROM_SIZE) {
        FRAMEWORK_write_eeprom_32(u16_addr, u32_data);
        bRetVal = true;
    }
    return bRetVal;
}

_Bool FA_EEPROM_write_block(uint16_t u16_addr, uint8_t * u8_data, uint16_t u16_length) {
    _Bool bRetVal = false;

    if (u16_addr + u16_length < FRAMEWORK_SMART_EEPROM_SIZE) {
        FRAMEWORK_write_eeprom_block(u16_addr, u8_data, u16_length);
        bRetVal = true;
    }
    return bRetVal;
}

_Bool FA_EEPROM_fill_block(uint16_t u16_addr, uint8_t u8_data, uint16_t u16_length) {
    _Bool bRetVal = false;

    if (u16_addr + u16_length < FRAMEWORK_SMART_EEPROM_SIZE) {
        FRAMEWORK_fill_eeprom_block(u16_addr, u8_data, u16_length);
        bRetVal = true;
    }
    return bRetVal;
}

uint8_t FA_EEPROM_read_8(uint16_t u16_addr) {
    uint8_t u8_retVal = 0xFF;

    if (u16_addr <= FRAMEWORK_SMART_EEPROM_SIZE) {
        u8_retVal = FRAMEWORK_read_eeprom_8(u16_addr);
    }
    return u8_retVal;
}

uint16_t FA_EEPROM_read_16(uint16_t u16_addr) {
    uint16_t u16_retVal = 0xFFFF;

    if (u16_addr + 1 <= FRAMEWORK_SMART_EEPROM_SIZE) {
        u16_retVal = FRAMEWORK_read_eeprom_16(u16_addr);
    }
    return u16_retVal;
}

uint32_t FA_EEPROM_read_32(uint16_t u16_addr) {
    uint32_t u32_retVal = 0xFFFFFFFF;

    if (u16_addr + 3 <= FRAMEWORK_SMART_EEPROM_SIZE) {
        u32_retVal = FRAMEWORK_read_eeprom_32(u16_addr);
    }
    return u32_retVal;
}

_Bool FA_EEPROM_read_block(uint16_t u16_addr, uint8_t * u8_data, uint16_t u16_length) {
    _Bool bRetVal = false;

    if (u16_addr + u16_length < FRAMEWORK_SMART_EEPROM_SIZE) {
        FRAMEWORK_read_eeprom_block(u16_addr, u8_data, u16_length);
        bRetVal = true;
    }
    return bRetVal;
}

void FA_EEPROM_read_mac(uint8_t * u8_data) {
    FA_EEPROM_read_block(EEPROM_MAC, u8_data, 6);
}

void FA_EEPROM_write_mac(uint8_t * u8_data) {
    FA_EEPROM_write_block(EEPROM_MAC, u8_data, 6);
}

void FA_EEPROM_factory_reset(void) {
    FRAMEWORK_write_eeprom_8(EEPROM_TOUCH, 0xFF);
    do {
    } while (1);
}





/*******************************************************************************
 End of File
 */
