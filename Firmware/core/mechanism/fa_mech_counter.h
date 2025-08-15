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
    fa_rs485.h

  Summary:
    This header file provides prototypes and definitions for the Future Automation
    RS485 interface.

  Description:
    This header file provides function prototypes and data type definitions for
    the Future Automation RS485 interface.  This provides all transport layer
    and command-set functionality
 *******************************************************************************/

#ifndef FA_MECH_POT_COUNTER_H
#define FA_MECH_POT_COUNTER_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "fa_mech_app.h"

#define NULL_COUNTER_ID 0xFF

typedef enum {
    hall_counter, pot_counter
} t_counter_type;


void FA_MECH_COUNTER_init(t_motor_id eMotorId, int16_t i16_deadband_val);
void FA_MECH_COUNTER_reference(t_motor_id eMotorId);
void FA_MECH_COUNTER_set_out_limit(t_motor_id eMotorId);
void FA_MECH_COUNTER_set_preset(t_motor_id eMotorId, uint16_t u16_ee_address);
int16_t FA_MECH_COUNTER_get_out_limit(t_motor_id eMotorId);
int16_t FA_MECH_COUNTER_getCounter(t_motor_id eMotorId);
uint16_t FA_MECH_COUNTER_target_count(t_motor_id eMotorId, int16_t i16_target_pos);
uint16_t FA_MECH_COUNTER_target_preset(t_motor_id eMotorId, uint16_t u16_ee_address);
void FA_MECH_COUNTER_target_in_switch(t_motor_id eMotorId);
void FA_MECH_COUNTER_target_out_switch(t_motor_id eMotorId);
int16_t FA_MECH_COUNTER_handle_counter(t_motor_id eMotorId);
_Bool FA_MECH_COUNTER_isReferenced(t_motor_id eMotorId);



#endif /* FA_MECH_POT_COUNTER_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

