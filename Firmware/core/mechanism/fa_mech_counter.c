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
    fa_mech_limit_switches.c

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
#include "global-definitions.h"
#include "fa_mech_app.h"
#
// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define DEBOUNCE_PERIOD    20

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
// Global variables

struct {
    t_motor_id e_motor_id;
    t_adc_id e_pot_id;
    _Bool bReferenced;
    int16_t i16_ram_counter;
    int16_t i16_target;
    int16_t i16_target_direction;
    _Bool b_targetting_switch;
    int16_t i16_latched_target;
    int16_t i16_latched_target_direction;
    _Bool b_latched_targetting_switch;
    int16_t i16_deadband;
    _Bool bActive;
} counter_data[HW_NUMBER_OF_COUNTERS];

uint8_t u8_next_counter_number = 0;

// *****************************************************************************
// *****************************************************************************
// Section: Forward references
// *****************************************************************************
// *****************************************************************************
int16_t get_persistent_counter(uint8_t u8_counter_id);
int16_t get_persistent_out_limit(uint8_t u8_counter_id);
void start_counter(uint8_t u8_counter_id);
void stop_counter(uint8_t u8_counter_id);
void set_persistent_counter(uint8_t u8_counter_id);
void set_persistent_out_limit(uint8_t u8_counter_id);
void clear_persistent_counter(uint8_t u8_counter_id);
int16_t get_persistent_pot_offset(uint8_t u8_counter_id);
void set_persistent_pot_offset(uint8_t u8_counter_id);


// *****************************************************************************
// *****************************************************************************
// Section: Initialisation
// *****************************************************************************
// *****************************************************************************

void FA_MECH_COUNTER_init(t_motor_id eMotorId, int16_t i16_deadband_val) {
    if (u8_next_counter_number < HW_NUMBER_OF_COUNTERS) {
        FA_MECH_MOTOR_set_counter_id(eMotorId, u8_next_counter_number);
        counter_data[u8_next_counter_number].i16_target = 0;
        counter_data[u8_next_counter_number].i16_target_direction = 0;
        counter_data[u8_next_counter_number].b_targetting_switch = false;
        counter_data[u8_next_counter_number].bActive = false;
        counter_data[u8_next_counter_number].i16_deadband = i16_deadband_val;

        counter_data[u8_next_counter_number].i16_ram_counter = get_persistent_counter(u8_next_counter_number);
        counter_data[u8_next_counter_number].bReferenced = (counter_data[u8_next_counter_number].i16_ram_counter < DEFAULT_COUNTER);

        if (DEVICE_isAtReferencePoint(eMotorId)) {
            FA_MECH_COUNTER_reference(eMotorId);
        }
        u8_next_counter_number++;
    }
}

void FA_MECH_COUNTER_reference(t_motor_id eMotorId) {
    uint8_t u8_index;

    u8_index = FA_MECH_MOTOR_get_counter_id(eMotorId);
    if (u8_index < u8_next_counter_number) {
        counter_data[u8_index].i16_ram_counter = 0;
        set_persistent_counter(u8_index);

        if (FA_MECH_MOTOR_get_pot_id(eMotorId) != adc_null) {
            set_persistent_pot_offset(u8_index);
        }
        counter_data[u8_index].bReferenced = true;
    }
}

_Bool FA_MECH_COUNTER_isReferenced(t_motor_id eMotorId) {
    _Bool bRetVal = false;
    uint8_t u8_index;

    u8_index = FA_MECH_MOTOR_get_counter_id(eMotorId);
    if (u8_index < u8_next_counter_number) {
        bRetVal = counter_data[u8_index].bReferenced;
    }
    return bRetVal;
}

void FA_MECH_COUNTER_set_out_limit(t_motor_id eMotorId) {
    uint8_t u8_index;

    u8_index = FA_MECH_MOTOR_get_counter_id(eMotorId);
    if (u8_index < u8_next_counter_number) {
        if (counter_data[u8_index].bReferenced) {
            set_persistent_out_limit(u8_index);
        }
    }
}

void FA_MECH_COUNTER_set_preset(t_motor_id eMotorId, uint16_t u16_ee_address) {
    uint8_t u8_index;

    u8_index = FA_MECH_MOTOR_get_counter_id(eMotorId);
    if (u8_index < u8_next_counter_number) {
        if (counter_data[u8_index].bReferenced) {
            FA_MECH_PMEM_write_16(u16_ee_address, counter_data[u8_index].i16_ram_counter);
        }
    }
}

int16_t FA_MECH_COUNTER_get_out_limit(t_motor_id eMotorId) {
    int16_t i16_retVal = DEFAULT_OUT_LIMIT;
    uint8_t u8_index;

    u8_index = FA_MECH_MOTOR_get_counter_id(eMotorId);
    if (u8_index < u8_next_counter_number) {
        i16_retVal = get_persistent_out_limit(u8_index);
    }
    return i16_retVal;
}

int16_t FA_MECH_COUNTER_getCounter(t_motor_id eMotorId) {
    int16_t i16_ret_val = DEFAULT_COUNTER;
    uint8_t u8_index;

    u8_index = FA_MECH_MOTOR_get_counter_id(eMotorId);
    if (u8_index < u8_next_counter_number) {
        i16_ret_val = counter_data[u8_index].i16_ram_counter;
    }
    return i16_ret_val;
}

uint16_t FA_MECH_COUNTER_target_count(t_motor_id eMotorId, int16_t i16_target_pos) {
    uint16_t u16_ret_fn = FN_HARD_STOP;
    uint8_t u8_index;

    u8_index = FA_MECH_MOTOR_get_counter_id(eMotorId);
    if (u8_index < u8_next_counter_number) {
        if (counter_data[u8_index].bReferenced) {
            if (i16_target_pos < (counter_data[u8_index].i16_ram_counter - counter_data[u8_index].i16_deadband)) {
                u16_ret_fn = FN_IN;
                counter_data[u8_index].i16_latched_target = i16_target_pos;
                counter_data[u8_index].i16_latched_target_direction = -1;
                counter_data[u8_index].b_latched_targetting_switch = false;
            } else if (i16_target_pos > (counter_data[u8_index].i16_ram_counter + counter_data[u8_index].i16_deadband)) {
                u16_ret_fn = FN_OUT;
                counter_data[u8_index].i16_latched_target = i16_target_pos;
                counter_data[u8_index].i16_latched_target_direction = 1;
                counter_data[u8_index].b_latched_targetting_switch = false;
            } else {
                u16_ret_fn = FN_HARD_STOP;
            }
        }
    }
    return u16_ret_fn;
}

uint16_t FA_MECH_COUNTER_target_preset(t_motor_id eMotorId, uint16_t u16_ee_address) {
    uint16_t u16_ret_val = FN_HARD_STOP;

    int16_t i16_target;

    uint8_t u8_index;

    u8_index = FA_MECH_MOTOR_get_counter_id(eMotorId);
    if (u8_index < u8_next_counter_number) {
        i16_target = FA_MECH_PMEM_read_16(u16_ee_address);
        if (i16_target < DEFAULT_PRESET_POS) {
            u16_ret_val = FA_MECH_COUNTER_target_count(eMotorId, i16_target);
        }
    }
    return u16_ret_val;
}

void FA_MECH_COUNTER_target_in_switch(t_motor_id eMotorId) {
    uint8_t u8_index;

    u8_index = FA_MECH_MOTOR_get_counter_id(eMotorId);
    if (u8_index < u8_next_counter_number) {
        if (counter_data[u8_index].i16_target_direction == -1) {
            counter_data[u8_index].i16_target = 0;
            counter_data[u8_index].b_targetting_switch = true;
        } else {
            counter_data[u8_index].i16_latched_target = 0;
            counter_data[u8_index].i16_latched_target_direction = -1;
            counter_data[u8_index].b_latched_targetting_switch = true;
        }
    }
}

void FA_MECH_COUNTER_target_out_switch(t_motor_id eMotorId) {
    uint8_t u8_index;

    u8_index = FA_MECH_MOTOR_get_counter_id(eMotorId);
    if (u8_index < u8_next_counter_number) {
        if (counter_data[u8_index].i16_target_direction == -1) {
            counter_data[u8_index].i16_target = FA_MECH_COUNTER_get_out_limit(eMotorId);
            counter_data[u8_index].b_targetting_switch = true;
        } else {
            counter_data[u8_index].i16_latched_target = FA_MECH_COUNTER_get_out_limit(eMotorId);
            counter_data[u8_index].i16_latched_target_direction = 1;
            counter_data[u8_index].b_latched_targetting_switch = true;
        }
    }
}


//*************************************************
// ENQUIRY INTERFACE
//*************************************************
//
// report status of individual limit switch
//*************************************************



//*************************************************
// MAIN HANDLER
//*************************************************
//
//
//*************************************************

int16_t FA_MECH_COUNTER_handle_counter(t_motor_id eMotorId) {
    int16_t iDTT = 0x7FFF;
    uint8_t u8_index;

    u8_index = FA_MECH_MOTOR_get_counter_id(eMotorId);
    if (u8_index < u8_next_counter_number) {
        if (counter_data[u8_index].bReferenced) {
            if (DEVICE_isMotorOn(counter_data[u8_index].e_motor_id) && (counter_data[u8_index].bActive == false)) {
                start_counter(u8_index);
            } else if ((DEVICE_isMotorOn(counter_data[u8_index].e_motor_id) == false) && counter_data[u8_index].bActive) {
                stop_counter(u8_index);
            }

            if (counter_data[u8_index].e_pot_id != adc_null) {
                counter_data[u8_index].i16_ram_counter = FA_ADC_get_Value(counter_data[u8_index].e_pot_id) - get_persistent_pot_offset(u8_index);
            } else {
                // hall counter code goes here
            }
            if (counter_data[u8_index].bActive) {
                if (counter_data[u8_index].i16_target_direction == -1) {
                    iDTT = (counter_data[u8_index].i16_ram_counter - counter_data[u8_index].i16_target);
                } else if (counter_data[u8_index].i16_target_direction == 1) {
                    iDTT = (counter_data[u8_index].i16_target - counter_data[u8_index].i16_ram_counter);
                }
                if ((iDTT <= 0) && counter_data[u8_index].b_targetting_switch) {
                    iDTT = 0x7FFF;
                }
            }
        }
    }
    return iDTT;
}



//*************************************************
// PRIVATE FUNCTIONS
//*************************************************

void start_counter(uint8_t u8_counter_id) {
    counter_data[u8_counter_id].i16_target = counter_data[u8_counter_id].i16_latched_target;
    counter_data[u8_counter_id].i16_target_direction = counter_data[u8_counter_id].i16_latched_target_direction;
    counter_data[u8_counter_id].b_targetting_switch = counter_data[u8_counter_id].b_latched_targetting_switch;
    clear_persistent_counter(u8_counter_id);
    counter_data[u8_counter_id].bActive = true;
}

void stop_counter(uint8_t u8_counter_id) {
    counter_data[u8_counter_id].i16_target_direction = 0;
    counter_data[u8_counter_id].b_targetting_switch = false;
    if (counter_data[u8_counter_id].bReferenced) {
        set_persistent_counter(u8_counter_id);
    }
    counter_data[u8_counter_id].bActive = false;
}
//
// get relevant counter value from persistent memory
//*************************************************

int16_t get_persistent_counter(uint8_t u8_counter_id) {
    int16_t i16_ret_val;
    switch (u8_counter_id) {
        case 0:
            i16_ret_val = FA_MECH_PMEM_read_16(EEPROM_COUNTER_1);
            break;
        case 1:
            i16_ret_val = FA_MECH_PMEM_read_16(EEPROM_COUNTER_2);
            break;
        case 2:
            i16_ret_val = FA_MECH_PMEM_read_16(EEPROM_COUNTER_3);
            break;
        case 3:
            i16_ret_val = FA_MECH_PMEM_read_16(EEPROM_COUNTER_4);
            break;
        case 4:
            i16_ret_val = FA_MECH_PMEM_read_16(EEPROM_COUNTER_5);
            break;
        default:
            i16_ret_val = DEFAULT_COUNTER;
            break;
    }
    return i16_ret_val;
}
//
// read relevant out limit from persistent memory
//*************************************************

int16_t get_persistent_out_limit(uint8_t u8_counter_id) {
    int16_t i16_ret_val;
    switch (u8_counter_id) {
        case 0:
            i16_ret_val = FA_MECH_PMEM_read_16(EEPROM_OUT_LIMIT_1);
            break;
        case 1:
            i16_ret_val = FA_MECH_PMEM_read_16(EEPROM_OUT_LIMIT_2);
            break;
        case 2:
            i16_ret_val = FA_MECH_PMEM_read_16(EEPROM_OUT_LIMIT_3);
            break;
        case 3:
            i16_ret_val = FA_MECH_PMEM_read_16(EEPROM_OUT_LIMIT_4);
            break;
        case 4:
            i16_ret_val = FA_MECH_PMEM_read_16(EEPROM_OUT_LIMIT_5);
            break;
        default:
            i16_ret_val = DEFAULT_OUT_LIMIT;
            break;
    }
    return i16_ret_val;
}
//
// read relevant pot offset from persistent memory
//*************************************************

int16_t get_persistent_pot_offset(uint8_t u8_counter_id) {
    int16_t i16_ret_val;
    switch (u8_counter_id) {
        case 0:
            i16_ret_val = FA_MECH_PMEM_read_16(EEPROM_POT_OFFSET_1);
            break;
        case 1:
            i16_ret_val = FA_MECH_PMEM_read_16(EEPROM_POT_OFFSET_2);
            break;
        case 2:
            i16_ret_val = FA_MECH_PMEM_read_16(EEPROM_POT_OFFSET_3);
            break;
        case 3:
            i16_ret_val = FA_MECH_PMEM_read_16(EEPROM_POT_OFFSET_4);
            break;
        case 4:
            i16_ret_val = FA_MECH_PMEM_read_16(EEPROM_POT_OFFSET_5);
            break;
        default:
            i16_ret_val = DEFAULT_COUNTER;
            break;
    }
    return i16_ret_val;
}
//
// save relevant counter value to persistent memory
//*************************************************

void set_persistent_counter(uint8_t u8_counter_id) {
    switch (u8_counter_id) {
        case 0:
            FA_MECH_PMEM_write_16(EEPROM_COUNTER_1, counter_data[u8_counter_id].i16_ram_counter);
            break;
        case 1:
            FA_MECH_PMEM_write_16(EEPROM_COUNTER_2, counter_data[u8_counter_id].i16_ram_counter);
            break;
        case 2:
            FA_MECH_PMEM_write_16(EEPROM_COUNTER_3, counter_data[u8_counter_id].i16_ram_counter);
            break;
        case 3:
            FA_MECH_PMEM_write_16(EEPROM_COUNTER_4, counter_data[u8_counter_id].i16_ram_counter);
            break;
        case 4:
            FA_MECH_PMEM_write_16(EEPROM_COUNTER_5, counter_data[u8_counter_id].i16_ram_counter);
            break;
        default:
            // Do nothing
            break;
    }
}
//
// clear relevant counter value in persistent memory
//*************************************************

void clear_persistent_counter(uint8_t u8_counter_id) {
    switch (u8_counter_id) {
        case 0:
            FA_MECH_PMEM_write_16(EEPROM_COUNTER_1, DEFAULT_COUNTER);
            break;
        case 1:
            FA_MECH_PMEM_write_16(EEPROM_COUNTER_2, DEFAULT_COUNTER);
            break;
        case 2:
            FA_MECH_PMEM_write_16(EEPROM_COUNTER_3, DEFAULT_COUNTER);
            break;
        case 3:
            FA_MECH_PMEM_write_16(EEPROM_COUNTER_4, DEFAULT_COUNTER);
            break;
        case 4:
            FA_MECH_PMEM_write_16(EEPROM_COUNTER_5, DEFAULT_COUNTER);
            break;
        default:
            // Do nothing
            break;
    }
}

//
// save relevant out limit to persistent memory
//*************************************************

void set_persistent_out_limit(uint8_t u8_counter_id) {
    switch (u8_counter_id) {
        case 0:
            FA_MECH_PMEM_write_16(EEPROM_OUT_LIMIT_1, counter_data[u8_counter_id].i16_ram_counter);
            break;
        case 1:
            FA_MECH_PMEM_write_16(EEPROM_OUT_LIMIT_2, counter_data[u8_counter_id].i16_ram_counter);
            break;
        case 2:
            FA_MECH_PMEM_write_16(EEPROM_OUT_LIMIT_3, counter_data[u8_counter_id].i16_ram_counter);
            break;
        case 3:
            FA_MECH_PMEM_write_16(EEPROM_OUT_LIMIT_4, counter_data[u8_counter_id].i16_ram_counter);
            break;
        case 4:
            FA_MECH_PMEM_write_16(EEPROM_OUT_LIMIT_5, counter_data[u8_counter_id].i16_ram_counter);
            break;
        default:
            // Do nothing
            break;
    }
}
//
// save relevant pot offset to persistent memory
//*************************************************

void set_persistent_pot_offset(uint8_t u8_counter_id) {
    uint16_t u16_offset;

    u16_offset = FA_ADC_get_Value(counter_data[u8_counter_id].e_pot_id);
    switch (u8_counter_id) {
        case 0:
            FA_MECH_PMEM_write_16(EEPROM_POT_OFFSET_1, u16_offset);
            break;
        case 1:
            FA_MECH_PMEM_write_16(EEPROM_POT_OFFSET_2, u16_offset);
            break;
        case 2:
            FA_MECH_PMEM_write_16(EEPROM_POT_OFFSET_3, u16_offset);
            break;
        case 3:
            FA_MECH_PMEM_write_16(EEPROM_POT_OFFSET_4, u16_offset);
            break;
        case 4:
            FA_MECH_PMEM_write_16(EEPROM_POT_OFFSET_5, u16_offset);
            break;
        default:
            // Do nothing
            break;
    }
}

/*******************************************************************************
 End of File
 */
