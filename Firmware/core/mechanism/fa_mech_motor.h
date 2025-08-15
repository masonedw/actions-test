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

#ifndef FA_MECH_MOTOR_H
#define FA_MECH_MOTOR_H

#include <stdint.h>
#include "framework.h"
#include "fa_mech_app.h"
#include "fa_mech_script.h"
#include "fa_adc.h"

typedef struct {
    t_channel_id e_channel_id;
    uint8_t u8_counter_id;
    t_adc_id e_pot_id;
    t_chan_ccx e_hall_a_cc;
    t_chan_ccx e_hall_b_cc;
} t_motor_data;





// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
void FA_MECH_MOTOR_init(t_motor_id eRequestedMotor, t_channel_id channel_id);
t_channel_id FA_MECH_MOTOR_get_channel_id(t_motor_id eMotorId);
uint8_t FA_MECH_MOTOR_get_counter_id(t_motor_id eMotorId);
t_chan_ccx FA_MECH_MOTOR_get_hall_a(t_motor_id eMotorId);
t_chan_ccx FA_MECH_MOTOR_get_hall_b(t_motor_id eMotorId);
t_adc_id FA_MECH_MOTOR_get_pot_id(t_motor_id eMotorId);
void FA_MECH_MOTOR_set_counter_id(t_motor_id eMotorId, uint8_t u8_data);
void FA_MECH_MOTOR_set_hall_a(t_motor_id eMotorId, t_chan_ccx e_data);
void FA_MECH_MOTOR_set_hall_b(t_motor_id eMotorId, t_chan_ccx e_data);
void FA_MECH_MOTOR_set_pot_id(t_motor_id eMotorId, t_adc_id e_data);
extern void FA_MECH_MOTOR_Tasks(void);
extern void FA_MECH_MOTOR_start(t_motorAction eMotorAction);
extern void FA_MECH_MOTOR_stop(void);

#endif /* FA_MECH_MOTOR_DRIVER_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

