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
    fa_app.h

  Summary:
    This header file provides prototypes and definitions for the Future Automation
    application.

  Description:
    This header file provides function prototypes and data type definitions for
    the Future Automation application.  This provides a top-level coding root
    for all product specific functionality
 *******************************************************************************/

#ifndef FA_EEPROM_MAP_H
#define FA_EEPROM_MAP_H


#define FRAMEWORK_SMART_EEPROM_SIZE    EEPROM_2K

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
// None




// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************

// Admin
// Reserved 0000H - 000FH
// TOUCH and MAC ADDRESSS

#define EEPROM_SYSTEM_ID_SET                    0x0028		/* 1 byte*/
#define EEPROM_DEVICE_ID                        0x0040		/* 3 bytes  */
#define EEPROM_DEVICE_LABEL                     0x0043		/* 16 bytes  */
#define EEPROM_MOTOR_DIRECTION                  0x0053		/* 1 bytes  */
#define EEPROM_MOTOR_GROUP                      0x0054      /* 48 bytes */
#define EEPROM_InternalPosition_Count           0x0084		/* 32 bytes  */
#define EEPROM_InternalPosition_Percent         0x00A4		/* 16 bytes  */
#define EEPROM_Motor_Slow_Speed                 0x00B4		/* 1 bytes  */
#define EEPROM_Motor_Fast_Up_Speed              0x00B5		/* 1 bytes  */
#define EEPROM_Motor_Fast_Down_Speed            0x00B6		/* 1 bytes  */
#define EEPROM_Up_Limit                         0x00B7		/* 2 bytes  */
#define EEPROM_Down_Limit                       0x00B9		/* 2 bytes  */


// Counter 1
#define EEPROM_POT_OFFSET_1       0x00C0      /* 2 bytes */
#define EEPROM_OUT_LIMIT_1        0x00C2      /* 2 bytes */
#define EEPROM_COUNTER_1          0x00C4      /* 2 bytes */

// Counter 2
#define EEPROM_POT_OFFSET_2       0x00C6      /* 2 bytes */
#define EEPROM_OUT_LIMIT_2        0x00C8      /* 2 bytes */
#define EEPROM_COUNTER_2          0x00CA      /* 2 bytes */

// Counter 3
#define EEPROM_POT_OFFSET_3       0x00CC      /* 2 bytes */
#define EEPROM_OUT_LIMIT_3        0x00CE      /* 2 bytes */
#define EEPROM_COUNTER_3          0x00D0      /* 2 bytes */

// Counter 4
#define EEPROM_POT_OFFSET_4       0x00D2      /* 2 bytes */
#define EEPROM_OUT_LIMIT_4        0x00D4      /* 2 bytes */
#define EEPROM_COUNTER_4          0x00D6      /* 2 bytes */

// Counter 5
#define EEPROM_POT_OFFSET_5       0x00D8      /* 2 bytes */
#define EEPROM_OUT_LIMIT_5        0x00DA      /* 2 bytes */
#define EEPROM_COUNTER_5          0x00DC      /* 2 bytes */

/* Next available location	      0x00DE			        */




// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************
// None



// *****************************************************************************
// *****************************************************************************
// Section: Function declarations
// *****************************************************************************
// *****************************************************************************

//******************************************************************************
// None



#endif /* FA_EEPROM_MAP_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

