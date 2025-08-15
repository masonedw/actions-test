#include "fa_mech_app.h"
#include "framework.h"
#include "fa_timer.h"
#include "definitions.h"

#define PWM_TIMESLICE 10000.0

typedef enum {
    pwm_active, pwm_inactive, pwm_stopping
} t_pwm_status;

_Bool duty_at_target(t_channel_id dc_channel);

typedef struct {
    t_pwm_status eStatus;
    float f_launch_duty;
    float f_duty_adjustment;
    float f_current_duty;
    float f_target_duty;
    uint32_t u32_ramp_timer;
} t_pwm_channel;

t_pwm_channel pwm_chan[FRAMEWORK_NO_OF_DC_CHANNELS];

uint32_t u32_period;

/******************************************************************************
  internal Function:
    FA_PWM_Init

  Remarks:


 */

void FA_PWM_init_service(void) {
    u32_period = FRAMEWORK_init_pwm();
}

void FA_PWM_period(void) {
    uint8_t u8_loop;
    for (u8_loop = chan_dc1; u8_loop < FRAMEWORK_NO_OF_DC_CHANNELS; u8_loop++) {
        if (pwm_chan[u8_loop].eStatus != pwm_inactive) {
            FRAMEWORK_set_pwm_duty(u8_loop, u32_period * pwm_chan[u8_loop].f_current_duty);
            if (pwm_chan[u8_loop].eStatus == pwm_stopping) {
                pwm_chan[u8_loop].eStatus = pwm_inactive;
            }
        }
    }
}

void FA_PWM_init_channel(t_motor_id eMotorId, float f_launch) {
    t_channel_id dc_channel;

    dc_channel = FA_MECH_MOTOR_get_channel_id(eMotorId);

    /* Register callback function for period event */
    if (dc_channel < FRAMEWORK_NO_OF_DC_CHANNELS) {
        pwm_chan[dc_channel].eStatus = pwm_inactive;
        pwm_chan[dc_channel].f_launch_duty = f_launch;
        pwm_chan[dc_channel].f_duty_adjustment = 0.0;
        pwm_chan[dc_channel].f_current_duty = 0.0;
        pwm_chan[dc_channel].f_target_duty = 0.0;
        pwm_chan[dc_channel].u32_ramp_timer = 0;
    }
}

/******************************************************************************
  internal Function:
    FA_PWM_ramp_over_time

  Remarks:

 */
void FA_PWM_ramp_over_time(t_motor_id eMotorId, float fTargetDuty, uint32_t u32_ramp_us) {
    t_channel_id dc_channel;

    dc_channel = FA_MECH_MOTOR_get_channel_id(eMotorId);

    if (dc_channel < FRAMEWORK_NO_OF_DC_CHANNELS) {
        // if PWM channel is disabled, start at the launch duty for the subject motor
        if (pwm_chan[dc_channel].eStatus != pwm_active) {
            pwm_chan[dc_channel].f_current_duty = pwm_chan[dc_channel].f_launch_duty;
        }

        // Set this channel target duty and ramp profile
        pwm_chan[dc_channel].f_target_duty = fTargetDuty;
        pwm_chan[dc_channel].f_duty_adjustment = (fTargetDuty - pwm_chan[dc_channel].f_current_duty) / (((float) u32_ramp_us) / PWM_TIMESLICE);

        // start this channel ramp timer if not already running
        if (pwm_chan[dc_channel].eStatus != pwm_active) {
            FA_TIMER_Start(PWM_TIMESLICE, &pwm_chan[dc_channel].u32_ramp_timer, Time_uS);
        }
        pwm_chan[dc_channel].eStatus = pwm_active;
    }
}

/******************************************************************************
  internal Function:
    FA_PWM_Stop

  Remarks:

 */
void FA_PWM_stop(t_motor_id eMotorId) {
    t_channel_id dc_channel;

    dc_channel = FA_MECH_MOTOR_get_channel_id(eMotorId);

    if (dc_channel < FRAMEWORK_NO_OF_DC_CHANNELS) {
        FA_TIMER_Stop(&pwm_chan[dc_channel].u32_ramp_timer);
        pwm_chan[dc_channel].f_current_duty = 0.0;
        pwm_chan[dc_channel].eStatus = pwm_stopping;
    }
}

_Bool FA_PWM_isOn(t_motor_id eMotorId) {
    _Bool bRetVal = false;

    t_channel_id dc_channel;

    dc_channel = FA_MECH_MOTOR_get_channel_id(eMotorId);

    if (dc_channel < FRAMEWORK_NO_OF_DC_CHANNELS) {
        bRetVal = (pwm_chan[dc_channel].eStatus == pwm_active);
    }

    return bRetVal;
}

void FA_PWM_Handler(t_motor_id eMotorId) {
    t_channel_id dc_channel;

    dc_channel = FA_MECH_MOTOR_get_channel_id(eMotorId);

    if (pwm_chan[dc_channel].eStatus == pwm_active) {
        // if the ramp timer is ready to increment the current duty for this channel
        if (FA_TIMER_Has_Finished(&pwm_chan[dc_channel].u32_ramp_timer)) {
            pwm_chan[dc_channel].f_current_duty += pwm_chan[dc_channel].f_duty_adjustment;

            // if we've reached or surpassed the target duty,cap at the target and level the ramp
            if (duty_at_target(dc_channel)) {
                pwm_chan[dc_channel].f_current_duty = pwm_chan[dc_channel].f_target_duty;
                pwm_chan[dc_channel].f_duty_adjustment = 0.0;
            } else {
                FA_TIMER_Start(PWM_TIMESLICE, &pwm_chan[dc_channel].u32_ramp_timer, Time_uS);
            }
        }
    }
}

/******************************************************************************
  internal Function:
    FA_PWM_Tasks

  Remarks:

 */

_Bool duty_at_target(t_channel_id dc_channel) {
    _Bool bRetVal = true;

    if (pwm_chan[dc_channel].eStatus == pwm_active) {
        if (pwm_chan[dc_channel].f_duty_adjustment < 0) {
            bRetVal = (pwm_chan[dc_channel].f_current_duty <= pwm_chan[dc_channel].f_target_duty);
        } else if (pwm_chan[dc_channel].f_duty_adjustment > 0) {
            bRetVal = (pwm_chan[dc_channel].f_current_duty >= pwm_chan[dc_channel].f_target_duty);
        }

    }
    return bRetVal;
}


