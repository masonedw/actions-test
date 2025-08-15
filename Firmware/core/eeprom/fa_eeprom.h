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
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
 *******************************************************************************/

#ifndef _FA_EEPROM_H
#define _FA_EEPROM_H

#include <stdint.h>


#define EEPROM_1K   0x03FF
#define EEPROM_2K   0x07FF
#define EEPROM_4K   0x0FFF
#define EEPROM_8K   0x1FFF
#define EEPROM_16K  0x3FFF
#define EEPROM_32K  0x7FFF
#define EEPROM_64K  0xFFFF

#define EEPROM_INITIALISED_SIGNATURE 0xAA

// EEPROM - reserved area

#define EEPROM_TOUCH            0x0000      /* 1 byte   */

// TCPIP
#define EEPROM_MAC              0x0001      /* 6 byte   */
#define EEPROM_DHCP			    0x0007		/* 1 byte   */
#define EEPROM_STATIC_IP		0x0008		/* 4 bytes  */
#define EEPROM_STATIC_MASK		0x000C		/* 4 bytes  */
#define EEPROM_PORT_ADDR        0x0010		/* 2 bytes  */
#define EEPROM_CLIENT_CONNECTIONS 0x0012    /* 6 bytes */

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************





// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

void FA_EEPROM_init(void);

_Bool FA_EEPROM_write_8(uint16_t u16_addr, uint8_t u8_data);
_Bool FA_EEPROM_write_16(uint16_t u16_addr, uint16_t u16_data);
_Bool FA_EEPROM_write_32(uint16_t u16_addr, uint32_t u32_data);
_Bool FA_EEPROM_write_block(uint16_t u16_addr, uint8_t * u8_data, uint16_t u16_length);
_Bool FA_EEPROM_fill_block(uint16_t u16_addr, uint8_t u8_data, uint16_t u16_length);

uint8_t FA_EEPROM_read_8(uint16_t u16_addr);
uint16_t FA_EEPROM_read_16(uint16_t u16_addr);
uint32_t FA_EEPROM_read_32(uint16_t u16_addr);
_Bool FA_EEPROM_read_block(uint16_t u16_addr, uint8_t * u8_data, uint16_t u16_length);

void FA_EEPROM_read_mac(uint8_t * u8_data);
void FA_EEPROM_write_mac(uint8_t * u8_data);

void FA_EEPROM_factory_reset(void);


#endif /* _FA_EEPROM_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

