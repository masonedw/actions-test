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
#include "protocol_common.h"
#include "fa_protocol_keypad.h"



// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
//#define SIZEOF_KEYPAD_COMMANDS (sizeof(t_KeypadMessageStructure) * FA_MAX_BUTTONS)
#define KEYPAD_TOGGLESTATE_SIZE 100
#define MOTOR_GROUP_SIZE 48
#define MOTOR_DIRECTION_SIZE 1
#define GROUPALL_ID_SIZE 3


#define EEPROM_SYSTEM_ID_SET        0x0055		/* 1 byte*/
//#define EEPROM_KEYPAD_COMMANDS      0x0112
#define EEPROM_DEVICE_ID            EEPROM_SYSTEM_ID_SET + MOTOR_DIRECTION_SIZE
//#define EEPROM_KEYPAD_ToggleState   EEPROM_DEVICE_ID + DEVICE_ID_SIZE
#define EEPROM_DEVICE_LABEL         EEPROM_DEVICE_ID + GROUPALL_ID_SIZE
#define EEPROM_MOTOR_DIRECTION      EEPROM_DEVICE_LABEL + LABEL_LENGTH
#define EEPROM_MOTOR_GROUP          EEPROM_MOTOR_DIRECTION + MOTOR_DIRECTION_SIZE
#define EEPROM_GroupALL_ID          EEPROM_MOTOR_GROUP + MOTOR_GROUP_SIZE
#define EEPROM_MotorAll_ID          EEPROM_GroupALL_ID + GROUPALL_ID_SIZE
/* Next available location          0x069C			        */




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

