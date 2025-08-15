/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    fa_pwm.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _PWM_TASK_H    /* Guard against multiple inclusion */
#define _PWM_TASK_H

#include <stdint.h>
#include "fa_mech_motor.h"


void FA_PWM_init_service(void);
void FA_PWM_init_channel(t_motor_id eMotorId, float f_launch);
void FA_PWM_Handler(t_motor_id eMotorId);
void FA_PWM_ramp_over_time(t_motor_id eMotorId, float fTargetDuty, uint32_t u32_ramp_us);
void FA_PWM_stop(t_motor_id eMotorId);
_Bool FA_PWM_isOn(t_motor_id eMotorId);


#define PWM_RAMP_1_SEC  1000000
#define PWM_RAMP_2_SEC  2000000
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */


#endif