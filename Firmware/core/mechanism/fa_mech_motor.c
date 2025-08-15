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
#include "hw.h"
#include "fn_codes.h"
#include "framework.h"
#include "fa_mech_app.h"
#include "fa_timer.h"



// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define MOTOR_TRANSITION_MS  250

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Types
// *****************************************************************************
// *****************************************************************************

typedef enum {
    event_Timeout, event_Start, event_Stop
} t_MotorDriver_event;

typedef enum {
    state_idle, state_enable_step_a, state_enable_step_b
} t_MotorDriver_state;



// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
t_motor_id eActiveMotor = motor_null;
uint8_t u8_active_fn = FN_NULL;
t_motor_id ePendingMotor = motor_null;
uint8_t u8_pendingFn = FN_NULL;
t_MotorDriver_state eMotorDriver_state = state_idle;
static uint32_t u32_motor_transition_timer = 0;


t_motor_data motor_data[HW_NUMBER_OF_MOTORS];


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

void handle_idle_state(t_MotorDriver_event eEvent) {
    switch (eEvent) {
        case event_Start:
            FA_DEVICE_deactivate_motor(eActiveMotor);
            eActiveMotor = motor_null;
            if (ePendingMotor != motor_null) {
                eMotorDriver_state = state_enable_step_a;
                FA_TIMER_Start(MOTOR_TRANSITION_MS, &u32_motor_transition_timer, Time_mS);
            }
            break;

        case event_Stop:
            FA_DEVICE_deactivate_motor(eActiveMotor);
            FA_TIMER_Start(MOTOR_TRANSITION_MS, &u32_motor_transition_timer, Time_mS);
            break;

        default:
            // Do nothing
            break;
    }
}

void handle_enable_step_a_state(t_MotorDriver_event eEvent) {
    switch (eEvent) {
        case event_Timeout:
            eActiveMotor = ePendingMotor;
            u8_active_fn = u8_pendingFn;
            ePendingMotor = motor_null;

            // Only set direction at this stage if the motor is AC -
            // For DC, direction must be set immediately after PWM activation in enable_step_b to preserve integrity of PWM output and ramp timer
            if (motor_data[eActiveMotor].e_channel_id < FRAMEWORK_NO_OF_DC_CHANNELS) {
                FA_DEVICE_set_motor_direction(eActiveMotor, u8_active_fn);
            }
            eMotorDriver_state = state_enable_step_b;
            break;

        case event_Stop:
            FA_DEVICE_deactivate_motor(eActiveMotor);
            eMotorDriver_state = state_idle;
            break;

        default:
            eMotorDriver_state = state_idle;
            break;
    }
    FA_TIMER_Start(MOTOR_TRANSITION_MS, &u32_motor_transition_timer, Time_mS);
}

void handle_enable_step_b_state(t_MotorDriver_event eEvent) {
    switch (eEvent) {
        case event_Stop:
            FA_DEVICE_deactivate_motor(eActiveMotor);
            break;
        case event_Timeout:
            FA_DEVICE_activate_motor(eActiveMotor);
            // If this is a DC motor, set direction immediately after PWM activation in enable_step_b to preserve integrity of PWM output and ramp timer
            if (motor_data[eActiveMotor].e_channel_id < FRAMEWORK_NO_OF_DC_CHANNELS) {
                FA_DEVICE_set_motor_direction(eActiveMotor, u8_active_fn);
            }
            break;
        default:
            // do nothing
            break;
    }
    FA_TIMER_Start(MOTOR_TRANSITION_MS, &u32_motor_transition_timer, Time_mS);
    eMotorDriver_state = state_idle;
}

void motor_driver_state_machine(t_MotorDriver_event eEvent) {
    switch (eMotorDriver_state) {
        case state_idle:
            handle_idle_state(eEvent);
            break;
        case state_enable_step_a:
            handle_enable_step_a_state(eEvent);
            break;
        case state_enable_step_b:
            handle_enable_step_b_state(eEvent);
            break;
    }
}

void FA_MECH_MOTOR_init(t_motor_id eRequestedMotor, t_channel_id channel_id) {
    if (eRequestedMotor < HW_NUMBER_OF_MOTORS) {
        motor_data[eRequestedMotor].e_channel_id = channel_id; /* associated DC or AC channel */
        motor_data[eRequestedMotor].e_pot_id = adc_null;
        motor_data[eRequestedMotor].u8_counter_id = HW_NUMBER_OF_COUNTERS;
        motor_data[eRequestedMotor].e_hall_a_cc = null_cc;
        motor_data[eRequestedMotor].e_hall_b_cc = null_cc;
    }
}

t_channel_id FA_MECH_MOTOR_get_channel_id(t_motor_id eMotorId) {
    t_channel_id eRetVal = chan_null;

    if (eMotorId < HW_NUMBER_OF_MOTORS) {
        eRetVal = motor_data[eMotorId].e_channel_id;
    }
    return eRetVal;
}

uint8_t FA_MECH_MOTOR_get_counter_id(t_motor_id eMotorId) {
    uint8_t u8_ret_val = 0xFF;

    if (eMotorId < HW_NUMBER_OF_MOTORS) {
        u8_ret_val = motor_data[eMotorId].u8_counter_id;
    }
    return u8_ret_val;
}

t_chan_ccx FA_MECH_MOTOR_get_hall_a(t_motor_id eMotorId) {
    t_chan_ccx eRetVal = null_cc;

    if (eMotorId < HW_NUMBER_OF_MOTORS) {
        eRetVal = motor_data[eMotorId].e_hall_a_cc;
    }
    return eRetVal;
}

t_chan_ccx FA_MECH_MOTOR_get_hall_b(t_motor_id eMotorId) {
    t_chan_ccx eRetVal = null_cc;

    if (eMotorId < HW_NUMBER_OF_MOTORS) {
        eRetVal = motor_data[eMotorId].e_hall_b_cc;
    }
    return eRetVal;
}

t_adc_id FA_MECH_MOTOR_get_pot_id(t_motor_id eMotorId) {
    t_adc_id eRetVal = adc_null;

    if (eMotorId < HW_NUMBER_OF_MOTORS) {
        eRetVal = motor_data[eMotorId].e_pot_id;
    }
    return eRetVal;
}

void FA_MECH_MOTOR_set_counter_id(t_motor_id eMotorId, uint8_t u8_data) {
    if (eMotorId < HW_NUMBER_OF_MOTORS) {
        motor_data[eMotorId].u8_counter_id = u8_data;
    }
}

void FA_MECH_MOTOR_set_hall_a(t_motor_id eMotorId, t_chan_ccx e_data) {
    if (eMotorId < HW_NUMBER_OF_MOTORS) {
        motor_data[eMotorId].e_hall_a_cc = e_data;
    }
}

void FA_MECH_MOTOR_set_hall_b(t_motor_id eMotorId, t_chan_ccx e_data) {
    if (eMotorId < HW_NUMBER_OF_MOTORS) {
        motor_data[eMotorId].e_hall_b_cc = e_data;
    }
}

void FA_MECH_MOTOR_set_pot_id(t_motor_id eMotorId, t_adc_id e_data) {
    if (eMotorId < HW_NUMBER_OF_MOTORS) {
        motor_data[eMotorId].e_pot_id = e_data;
    }
}

void FA_MECH_MOTOR_start(t_motorAction eMotorAction) {
    ePendingMotor = eMotorAction.eMotorID;
    u8_pendingFn = eMotorAction.u16_Function;
    motor_driver_state_machine(event_Start);
}

void FA_MECH_MOTOR_stop(void) {
    ePendingMotor = motor_null;
    u8_pendingFn = FN_NULL;
    motor_driver_state_machine(event_Stop);
}

void FA_MECH_MOTOR_Tasks(void) {
    if (FA_TIMER_Has_Finished(&u32_motor_transition_timer)) {
        motor_driver_state_machine(event_Timeout);
    }
}
/*******************************************************************************
 End of File
 */
