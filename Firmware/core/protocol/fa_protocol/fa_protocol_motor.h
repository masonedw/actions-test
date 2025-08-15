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
    fa_sdn_motor.h

  Summary:
    This header file provides prototypes and definitions for the Future Automation
    RS485 interface.

  Description:
    This header file provides function prototypes and data type definitions for
    the Future Automation SDN interface.
 *******************************************************************************/

#ifndef FA_PROTOCOL_MOTOR_H
#define FA_PROTOCOL_MOTOR_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "protocol_common.h"
#include "fa_protocol_cmd_list.h"
#include "fa_protocol.h"

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************
#pragma pack(push, 1)

typedef struct {
    bool b_Direction;

    uint8_t u8_Up_Speed;
    uint8_t u8_Down_Speed;

    uint16_t Up_Limit;
    uint16_t Down_Limit;

    bool MemoryChanged;

} t_MotorStructure;
#pragma pack(pop)

// *****************************************************************************
// *****************************************************************************
// Section: Function declarations
// *****************************************************************************
// *****************************************************************************

void FA_PROTOCOL_Motor_reset_eeprom(void);
void FA_PROTOCOL_Motor_INITIALIZE(void);
void FA_PROTOCOL_Handle_Motor_Message(t_InterfaceType e_InterfaceType, t_fa_protocol_message * u8_CurrentRx);
void FA_PROTOCOL_Motor_InactiveTimeOut(void);

// *****************************************************************************
// *****************************************************************************
// Functions
// *****************************************************************************
// *****************************************************************************


#endif

//DOM-IGNORE-BEGIN
#ifdef __cplusplus

#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

