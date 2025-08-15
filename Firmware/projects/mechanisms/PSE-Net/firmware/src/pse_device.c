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
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the Future Automation application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>
#include "hw.h"
#include "global-definitions.h"
#include "framework.h"
#include "fa_mech_app.h"
#include "fa_pwm.h"
#include "fa_adc.h"
#include "definitions.h"
#include "GPIO_Control.h"
// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Forward references
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialisation and State Machine Functions
// *****************************************************************************
// *****************************************************************************

void DEVICE_Init(void) {
    FA_MECH_MOTOR_init(HW_PSE_MOTOR, chan_dc1);
    FA_PWM_init_channel(HW_PSE_MOTOR, 0.2);
    FA_MECH_MOTOR_set_pot_id(HW_PSE_MOTOR, adc_dc1);
    FA_MECH_COUNTER_init(HW_PSE_MOTOR, 10);
}

_Bool DEVICE_isMotorOn(t_motor_id eMotorId) {
    return FA_PWM_isOn(eMotorId);
}

uint16_t DEVICE_preprocessUserFunction(uint16_t u16_fn) {
    switch (u16_fn) {
        case FN_STORE_A:
            FA_MECH_COUNTER_set_preset(HW_PSE_MOTOR, EEPROM_InternalPosition_Count);
            break;

        case FN_STORE_B:
            FA_MECH_COUNTER_set_preset(HW_PSE_MOTOR, EEPROM_InternalPosition_Count + 2);
            break;

        case FN_STORE_C:
            FA_MECH_COUNTER_set_preset(HW_PSE_MOTOR, EEPROM_InternalPosition_Count + 4);
            break;

        case FN_STORE_D:
            FA_MECH_COUNTER_set_preset(HW_PSE_MOTOR, EEPROM_InternalPosition_Count + 6);
            break;

        case FN_STORE_E:
            FA_MECH_COUNTER_set_preset(HW_PSE_MOTOR, EEPROM_InternalPosition_Count + 8);
            break;

        case FN_STORE_F:
            FA_MECH_COUNTER_set_preset(HW_PSE_MOTOR, EEPROM_InternalPosition_Count + 10);
            break;

        case FN_STORE_PRESET:
            FA_MECH_COUNTER_set_preset(HW_PSE_MOTOR, EEPROM_InternalPosition_Count + 12);
            break;

    }

    return u16_fn;
}

uint16_t DEVICE_processSwitchFn(uint8_t u8_switch_id, uint8_t u8_level) {
    uint16_t u16_fn;
    int16_t i16_octet;

    if (u8_level) {
        u16_fn = FN_LSW_ON;
        switch (u8_switch_id) {
            case HW_LSID_M1_IN:
                FA_MECH_COUNTER_reference(HW_PSE_MOTOR);
                break;

            case HW_LSID_M1_OUT:
                if (FA_MECH_COUNTER_isReferenced(HW_PSE_MOTOR)) {
                    FA_MECH_MOTOR_stop();
                    FA_MECH_COUNTER_set_out_limit(HW_PSE_MOTOR);
                    i16_octet = (FA_MECH_COUNTER_get_out_limit(HW_PSE_MOTOR) / 8);
                    FA_MECH_PMEM_write_16(EEPROM_InternalPosition_Count, i16_octet);
                    FA_MECH_PMEM_write_16(EEPROM_InternalPosition_Count + 2, i16_octet * 2);
                    FA_MECH_PMEM_write_16(EEPROM_InternalPosition_Count + 4, i16_octet * 3);
                    FA_MECH_PMEM_write_16(EEPROM_InternalPosition_Count + 12, i16_octet * 4);
                    FA_MECH_PMEM_write_16(EEPROM_InternalPosition_Count + 6, i16_octet * 5);
                    FA_MECH_PMEM_write_16(EEPROM_InternalPosition_Count + 8, i16_octet * 6);
                    FA_MECH_PMEM_write_16(EEPROM_InternalPosition_Count + 10, i16_octet * 7);
                }
                break;
        }
    } else {
        u16_fn = FN_LSW_OFF;
    }
    return u16_fn;
}

t_motorAction DEVICE_targetScriptEntry(t_motorAction script_entry) {
    t_motorAction ret_val;

    ret_val = script_entry;
    switch (script_entry.u16_Function) {
        case FN_IN: // do nothing
            if (FRAMEWORK_read_CCx(cc1)) {
                ret_val.u16_Function = FN_HARD_STOP;
            } else {
                FA_MECH_COUNTER_target_in_switch(HW_PSE_MOTOR);
            }
            break;

        case FN_OUT: // do nothing
            if (FRAMEWORK_read_CCx(cc2)) {
                ret_val.u16_Function = FN_HARD_STOP;
            } else {
                FA_MECH_COUNTER_target_out_switch(HW_PSE_MOTOR);
            }
            break;

        case FN_A:
            ret_val.u16_Function = FA_MECH_COUNTER_target_preset(HW_PSE_MOTOR, EEPROM_InternalPosition_Count);
            break;

        case FN_B:
            ret_val.u16_Function = FA_MECH_COUNTER_target_preset(HW_PSE_MOTOR, EEPROM_InternalPosition_Count + 2);
            break;

        case FN_C:
            ret_val.u16_Function = FA_MECH_COUNTER_target_preset(HW_PSE_MOTOR, EEPROM_InternalPosition_Count + 4);
            break;

        case FN_PRESET:
            ret_val.u16_Function = FA_MECH_COUNTER_target_preset(HW_PSE_MOTOR, EEPROM_InternalPosition_Count + 12);
            break;

        case FN_D:
            ret_val.u16_Function = FA_MECH_COUNTER_target_preset(HW_PSE_MOTOR, EEPROM_InternalPosition_Count + 6);
            break;

        case FN_E:
            ret_val.u16_Function = FA_MECH_COUNTER_target_preset(HW_PSE_MOTOR, EEPROM_InternalPosition_Count + 8);
            break;

        case FN_F:
            ret_val.u16_Function = FA_MECH_COUNTER_target_preset(HW_PSE_MOTOR, EEPROM_InternalPosition_Count + 10);
            break;

        default:
            break;
    }
    return ret_val;
}

_Bool DEVICE_targetAtLimit(t_motorAction target_action) {
    _Bool b_ret_val = true;


    if (target_action.eMotorID == HW_PSE_MOTOR) {
        switch (target_action.u16_Function) {
            case FN_IN:
                b_ret_val = FRAMEWORK_read_CCx(cc1);
                break;

            case FN_OUT:
                b_ret_val = FRAMEWORK_read_CCx(cc2);
                break;
        }
    }

    return b_ret_val;
}

void FA_DEVICE_deactivate_motor(t_motor_id eThisMotor) {
    FA_PWM_stop(eThisMotor);
    GPIO_Set_High(GPIO_M1_DIRA);
    GPIO_Set_High(GPIO_M1_DIRB);
}

void FA_DEVICE_set_motor_direction(t_motor_id eThisMotor, uint16_t u16_this_fn) {
    switch (u16_this_fn) {
        case FN_OUT:
            GPIO_Set_Low(GPIO_M1_DIRA);
            GPIO_Set_High(GPIO_M1_DIRB);
            break;
        case FN_IN:
            GPIO_Set_High(GPIO_M1_DIRA);
            GPIO_Set_Low(GPIO_M1_DIRB);
            break;
        case FN_USER_STOP:
            GPIO_Set_High(GPIO_M1_DIRA);
            GPIO_Set_High(GPIO_M1_DIRB);
            break;
    }
}

void FA_DEVICE_activate_motor(t_motor_id eThisMotor) {
    FA_PWM_ramp_over_time(eThisMotor, 1.0, PWM_RAMP_1_SEC);
}

_Bool DEVICE_isAtReferencePoint(t_motor_id eMotorId) {
    return GPIO_Read_Debounced_State(GPIO_CC1);
}

uint16_t DEVICE_handle_counter(void) {
    uint16_t u16_ret_val = FN_NULL;
    if (FA_MECH_COUNTER_handle_counter(HW_PSE_MOTOR) < 0) {
        u16_ret_val = FN_HARD_STOP;
    }
    return u16_ret_val;
}

/*******************************************************************************
 End of File
 */
