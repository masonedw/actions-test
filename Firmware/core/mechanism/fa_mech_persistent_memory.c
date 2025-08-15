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
#include "fa_eeprom_map.h"
#include "fa_eeprom.h"
#include "fa_mech_app.h"


// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************

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

struct {
    int16_t i16_a_pos;
    int16_t i16_b_pos;
    int16_t i16_c_pos;
    int16_t i16_d_pos;
    int16_t i16_e_pos;
    int16_t i16_f_pos;
    int16_t i16_preset_pos;

    int16_t i16_pot_offset_1;
    int16_t i16_out_limit_1;
    int16_t i16_counter_1;

    int16_t i16_pot_offset_2;
    int16_t i16_out_limit_2;
    int16_t i16_counter_2;

    int16_t i16_pot_offset_3;
    int16_t i16_out_limit_3;
    int16_t i16_counter_3;

    int16_t i16_pot_offset_4;
    int16_t i16_out_limit_4;
    int16_t i16_counter_4;

    int16_t i16_pot_offset_5;
    int16_t i16_out_limit_5;
    int16_t i16_counter_5;
} persistent_memory;


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

void FA_MECH_PMEM_Init(void) {
    FA_EEPROM_init();
    persistent_memory.i16_a_pos = FA_EEPROM_read_16(EEPROM_InternalPosition_Count);
    persistent_memory.i16_b_pos = FA_EEPROM_read_16(EEPROM_InternalPosition_Count + 2);
    persistent_memory.i16_c_pos = FA_EEPROM_read_16(EEPROM_InternalPosition_Count + 4);
    persistent_memory.i16_d_pos = FA_EEPROM_read_16(EEPROM_InternalPosition_Count + 6);
    persistent_memory.i16_e_pos = FA_EEPROM_read_16(EEPROM_InternalPosition_Count + 8);
    persistent_memory.i16_f_pos = FA_EEPROM_read_16(EEPROM_InternalPosition_Count + 10);
    persistent_memory.i16_preset_pos = FA_EEPROM_read_16(EEPROM_InternalPosition_Count + 12);

    //Counter 1
    persistent_memory.i16_pot_offset_1 = FA_EEPROM_read_16(EEPROM_POT_OFFSET_1);
    persistent_memory.i16_out_limit_1 = FA_EEPROM_read_16(EEPROM_OUT_LIMIT_1);
    persistent_memory.i16_counter_1 = FA_EEPROM_read_16(EEPROM_COUNTER_1);

    //Counter 2
    persistent_memory.i16_pot_offset_2 = FA_EEPROM_read_16(EEPROM_POT_OFFSET_2);
    persistent_memory.i16_out_limit_2 = FA_EEPROM_read_16(EEPROM_OUT_LIMIT_2);
    persistent_memory.i16_counter_2 = FA_EEPROM_read_16(EEPROM_COUNTER_2);

    //Counter 3
    persistent_memory.i16_pot_offset_3 = FA_EEPROM_read_16(EEPROM_POT_OFFSET_3);
    persistent_memory.i16_out_limit_3 = FA_EEPROM_read_16(EEPROM_OUT_LIMIT_3);
    persistent_memory.i16_counter_3 = FA_EEPROM_read_16(EEPROM_COUNTER_3);

    //Counter 4
    persistent_memory.i16_pot_offset_4 = FA_EEPROM_read_16(EEPROM_POT_OFFSET_4);
    persistent_memory.i16_out_limit_4 = FA_EEPROM_read_16(EEPROM_OUT_LIMIT_4);
    persistent_memory.i16_counter_4 = FA_EEPROM_read_16(EEPROM_COUNTER_4);

    //Counter 5
    persistent_memory.i16_pot_offset_5 = FA_EEPROM_read_16(EEPROM_POT_OFFSET_5);
    persistent_memory.i16_out_limit_5 = FA_EEPROM_read_16(EEPROM_OUT_LIMIT_5);
    persistent_memory.i16_counter_5 = FA_EEPROM_read_16(EEPROM_COUNTER_5);
}

void FA_MECH_PMEM_write_16(uint16_t u16_addr, uint16_t u16_data) {
    FA_EEPROM_write_16(u16_addr, u16_data);

    switch (u16_addr) {
        case EEPROM_InternalPosition_Count:
            persistent_memory.i16_a_pos = u16_data;
            break;

        case EEPROM_InternalPosition_Count + 2:
            persistent_memory.i16_b_pos = u16_data;
            break;

        case EEPROM_InternalPosition_Count + 4:
            persistent_memory.i16_c_pos = u16_data;
            break;

        case EEPROM_InternalPosition_Count + 6:
            persistent_memory.i16_d_pos = u16_data;
            break;

        case EEPROM_InternalPosition_Count + 8:
            persistent_memory.i16_e_pos = u16_data;
            break;

        case EEPROM_InternalPosition_Count + 10:
            persistent_memory.i16_f_pos = u16_data;
            break;

        case EEPROM_InternalPosition_Count + 12:
            persistent_memory.i16_preset_pos = u16_data;
            break;

            // Counter 1
        case EEPROM_POT_OFFSET_1:
            persistent_memory.i16_pot_offset_1 = u16_data;
            break;
        case EEPROM_OUT_LIMIT_1:
            persistent_memory.i16_out_limit_1 = u16_data;
            break;
        case EEPROM_COUNTER_1:
            persistent_memory.i16_counter_1 = u16_data;
            break;

            // Counter 2
        case EEPROM_POT_OFFSET_2:
            persistent_memory.i16_pot_offset_2 = u16_data;
            break;
        case EEPROM_OUT_LIMIT_2:
            persistent_memory.i16_out_limit_2 = u16_data;
            break;
        case EEPROM_COUNTER_2:
            persistent_memory.i16_counter_2 = u16_data;
            break;

            // Counter 3
        case EEPROM_POT_OFFSET_3:
            persistent_memory.i16_pot_offset_3 = u16_data;
            break;
        case EEPROM_OUT_LIMIT_3:
            persistent_memory.i16_out_limit_3 = u16_data;
            break;
        case EEPROM_COUNTER_3:
            persistent_memory.i16_counter_3 = u16_data;
            break;

            // Counter 4
        case EEPROM_POT_OFFSET_4:
            persistent_memory.i16_pot_offset_4 = u16_data;
            break;
        case EEPROM_OUT_LIMIT_4:
            persistent_memory.i16_out_limit_4 = u16_data;
            break;
        case EEPROM_COUNTER_4:
            persistent_memory.i16_counter_4 = u16_data;
            break;

            // Counter 5
        case EEPROM_POT_OFFSET_5:
            persistent_memory.i16_pot_offset_5 = u16_data;
            break;
        case EEPROM_OUT_LIMIT_5:
            persistent_memory.i16_out_limit_5 = u16_data;
            break;
        case EEPROM_COUNTER_5:
            persistent_memory.i16_counter_5 = u16_data;
            break;
    }
}

uint16_t FA_MECH_PMEM_read_16(uint16_t u16_addr) {
    uint16_t u16_ret_val;
    switch (u16_addr) {
        case EEPROM_InternalPosition_Count:
            u16_ret_val = persistent_memory.i16_a_pos;
            break;

        case EEPROM_InternalPosition_Count + 2:
            u16_ret_val = persistent_memory.i16_b_pos;
            break;

        case EEPROM_InternalPosition_Count + 4:
            u16_ret_val = persistent_memory.i16_c_pos;
            break;

        case EEPROM_InternalPosition_Count + 6:
            u16_ret_val = persistent_memory.i16_d_pos;
            break;

        case EEPROM_InternalPosition_Count + 8:
            u16_ret_val = persistent_memory.i16_e_pos;
            break;

        case EEPROM_InternalPosition_Count + 10:
            u16_ret_val = persistent_memory.i16_f_pos;
            break;

        case EEPROM_InternalPosition_Count + 12:
            u16_ret_val = persistent_memory.i16_preset_pos;
            break;

            // Counter 1
        case EEPROM_POT_OFFSET_1:
            u16_ret_val = persistent_memory.i16_pot_offset_1;
            break;
        case EEPROM_OUT_LIMIT_1:
            u16_ret_val = persistent_memory.i16_out_limit_1;
            break;
        case EEPROM_COUNTER_1:
            u16_ret_val = persistent_memory.i16_counter_1;
            break;

            // Counter 2
        case EEPROM_POT_OFFSET_2:
            u16_ret_val = persistent_memory.i16_pot_offset_2;
            break;
        case EEPROM_OUT_LIMIT_2:
            u16_ret_val = persistent_memory.i16_out_limit_2;
            break;
        case EEPROM_COUNTER_2:
            u16_ret_val = persistent_memory.i16_counter_2;
            break;

            // Counter 3
        case EEPROM_POT_OFFSET_3:
            u16_ret_val = persistent_memory.i16_pot_offset_3;
            break;
        case EEPROM_OUT_LIMIT_3:
            u16_ret_val = persistent_memory.i16_out_limit_3;
            break;
        case EEPROM_COUNTER_3:
            u16_ret_val = persistent_memory.i16_counter_3;
            break;

            // Counter 4
        case EEPROM_POT_OFFSET_4:
            u16_ret_val = persistent_memory.i16_pot_offset_4;
            break;
        case EEPROM_OUT_LIMIT_4:
            u16_ret_val = persistent_memory.i16_out_limit_4;
            break;
        case EEPROM_COUNTER_4:
            u16_ret_val = persistent_memory.i16_counter_4;
            break;

            // Counter 5
        case EEPROM_POT_OFFSET_5:
            u16_ret_val = persistent_memory.i16_pot_offset_5;
            break;
        case EEPROM_OUT_LIMIT_5:
            u16_ret_val = persistent_memory.i16_out_limit_5;
            break;
        case EEPROM_COUNTER_5:
            u16_ret_val = persistent_memory.i16_counter_5;
            break;
    }
    return u16_ret_val;
}


/*******************************************************************************
 End of File
 */
