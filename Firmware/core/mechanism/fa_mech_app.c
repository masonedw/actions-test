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
    fa_mech_task_manager.c

  Description:
    This file is the top level task manager for all mechanisms.

    All common tasks are called here from FA_MECH_tasks(). Common tasks are tasks
    which operate in the same way regardless of the target mechanism
    (examples IR, TCPIP, low level PWM handler, low level ADC handler, etc)

    Also called from FA_MECH_tasks() is DEVICE_Tasks(). This function should be supplied by a
    project specific .c module (e.g. pse_device.c) which performs all the tasks which may operate
    dependent on the specific mechanism.

 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "global-definitions.h"
#include "fn_codes.h"
#include "framework.h"
#include "fa_protocol.h"
#include "fa_at24.h"
#include "fa_packet.h"
#include "fa_mech_app.h"
#include "fa_ir.h"
#include "fa_timer.h"
#include "fa_tcpip.h"
#include "fa_pwm.h"
#include "fa_lamp.h"
#include "GPIO_Control.h"
#include "definitions.h"


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




// *****************************************************************************
// *****************************************************************************
// Section: Forward references
// *****************************************************************************
// *****************************************************************************
_Bool is_new_motor_action(t_motorAction action);



// *****************************************************************************
// *****************************************************************************
// Section: Initialisation
// *****************************************************************************
// *****************************************************************************

void HandleTimers(void) {

}

void Handle_TCPIP_Command(uint16_t Command) {
    if (Command != GLB_NULL_INT) {
        //FA_SDN_Handle_Button_Command(Command-1);
    }
}

void FA_MECH_reset_eeprom(void) {
    uint8_t u8_zeros[6] = {0, 0, 0, 0, 0, 0};

    FA_EEPROM_write_8(EEPROM_DHCP, 1); /* DHCH Enabled */
    FA_EEPROM_write_block(EEPROM_STATIC_IP, u8_zeros, 4);
    FA_EEPROM_write_block(EEPROM_STATIC_MASK, u8_zeros, 4);
    FA_EEPROM_write_16(EEPROM_PORT_ADDR, 9670); /* Default Port */
    FA_EEPROM_fill_block(EEPROM_DEVICE_LABEL, 0x00, 16);


    // Reinstate this when we use FAP messages
    //FA_SDN_MOTOR_reset_eeprom();


    //FA_SEEPROM_write_8(EEPROM_SYSTEM_ID, 0x46);  /* KeypadID Enabled */
    //FA_SEEPROM_write_8(EEPROM_SYSTEM_ID +1, 0x41);  /* KeypadID Enabled */
    //FA_SEEPROM_write_8(EEPROM_SYSTEM_ID +2, 0x4B);  /* KeypadID Enabled */

    //FA_SEEPROM_write_8(EEPROM_GroupALL_ID, 0x00);  /* GroupALL ID Enabled */
    //FA_SEEPROM_write_8(EEPROM_GroupALL_ID +1, 0x00);  /* GroupALL ID Enabled */
    //FA_SEEPROM_write_8(EEPROM_GroupALL_ID +2, 0x01);  /* GroupALL ID Enabled */

    //FA_SEEPROM_write_8(EEPROM_MotorAll_ID, 0x66);  /* MotorAll ID Enabled */
    //FA_SEEPROM_write_8(EEPROM_MotorAll_ID +1, 0x00);  /* MotorAll ID Enabled */
    //FA_SEEPROM_write_8(EEPROM_MotorAll_ID +2, 0x66);  /* MotorAll ID Enabled */
    // MAC address should be protected from factory reset - do not reset the 6 bytes from EEPROM_MAC_ADDR to EEPROM_MAC_ADDR + 5

    // Configurable preset positions
    //FA_SEEPROM_write_16(EEPROM_M1_ProgrammedPosition, DEFAULT_PRESET_POS);
    //FA_SEEPROM_write_16(EEPROM_M1_ProgrammedPosition+2, DEFAULT_PRESET_POS);
    //FA_SEEPROM_write_16(EEPROM_M1_ProgrammedPosition+4, DEFAULT_PRESET_POS);
    //FA_SEEPROM_write_16(EEPROM_M1_ProgrammedPosition+6, DEFAULT_PRESET_POS);
    //FA_SEEPROM_write_16(EEPROM_M1_ProgrammedPosition+8, DEFAULT_PRESET_POS);
    //FA_SEEPROM_write_16(EEPROM_M1_ProgrammedPosition+10, DEFAULT_PRESET_POS);
    //FA_SEEPROM_write_16(EEPROM_M1_ProgrammedPosition+12, DEFAULT_PRESET_POS);

    // Counter 1
    FA_EEPROM_write_16(EEPROM_POT_OFFSET_1, DEFAULT_PRESET_POS);
    FA_EEPROM_write_16(EEPROM_OUT_LIMIT_1, DEFAULT_PRESET_POS);
    FA_EEPROM_write_16(EEPROM_COUNTER_1, DEFAULT_OUT_LIMIT);

    // Counter 2
    FA_EEPROM_write_16(EEPROM_POT_OFFSET_2, DEFAULT_PRESET_POS);
    FA_EEPROM_write_16(EEPROM_OUT_LIMIT_2, DEFAULT_PRESET_POS);
    FA_EEPROM_write_16(EEPROM_COUNTER_2, DEFAULT_OUT_LIMIT);

    // Counter 3
    FA_EEPROM_write_16(EEPROM_POT_OFFSET_3, DEFAULT_PRESET_POS);
    FA_EEPROM_write_16(EEPROM_OUT_LIMIT_3, DEFAULT_PRESET_POS);
    FA_EEPROM_write_16(EEPROM_COUNTER_3, DEFAULT_OUT_LIMIT);

    // Counter 4
    FA_EEPROM_write_16(EEPROM_POT_OFFSET_4, DEFAULT_PRESET_POS);
    FA_EEPROM_write_16(EEPROM_OUT_LIMIT_4, DEFAULT_PRESET_POS);
    FA_EEPROM_write_16(EEPROM_COUNTER_4, DEFAULT_OUT_LIMIT);

    // Counter 5
    FA_EEPROM_write_16(EEPROM_POT_OFFSET_5, DEFAULT_PRESET_POS);
    FA_EEPROM_write_16(EEPROM_OUT_LIMIT_5, DEFAULT_PRESET_POS);
    FA_EEPROM_write_16(EEPROM_COUNTER_5, DEFAULT_OUT_LIMIT);

}

void FA_MECH_Initialize(void) {
    // ***********************
    // Hardware initialisation
    // ***********************

    USART_SERIAL_CONFIG USART_Config = {
        .baudRate = 9600,
        .dataWidth = USART_CONFIG_DATA_8_BIT,
        .parity = USART_CONFIG_PARITY_NONE,
        .stopBits = USART_CONFIG_STOP_1_BIT
    };

    // MAC address chip
    FA_AT24_Init();

    // EEPROM
    FA_MECH_PMEM_Init();

    FA_IR_Init(On_Press);
    FA_TIMER_Init();
    //FA_TCPIP_Init ();
    FA_PROTOCOL_COMMON_Initialize();
    // Reinstate this when we use FAP messages
    //FA_SDN_MOTOR_INITIALIZE();
    FA_PROTOCOL_Initialize_TCPIP(false);
    FA_PROTOCOL_Initialize_485(FRAMEWORK_485_PORT, &USART_Config);
    FA_PROTOCOL_Initialize_232(FRAMEWORK_232_PORT, &USART_Config, false);
    FA_PROTOCOL_Initialize();

    // Other
    FA_PWM_init_service();

    // *****************************
    // Common feature initialisation
    // *****************************
    FA_LAMP_Init();
    //FA_MECH_LIMIT_SWITCH_Init();
    //FA_MECH_CC_SWITCH_Init();

    // ******************************
    // Device specific initialisation
    // ******************************
    DEVICE_Init();

    // ******************************
    // Box power-on ready indication
    // ******************************
    FA_LAMP_Flash(250, Time_mS);
}

void FA_MECH_Tasks(void) {
    uint16_t u16_fn;

    //t_fa_packet_data command_packet;
    t_motorAction script_Action;
    uint8_t u8_next_pwm_chan = chan_dc1;

    // Update IO Pin post-debounce state
    GPIO_Task();

    // Handle MAC address read requests
    FA_AT24_Tasks();
    // Manage user input acquisition
    FA_IR_Tasks();
    FA_PROTOCOL_COMMON_Tasks();

    // if a switch has changed state, FN_LSW_ON or FN_LSW_OFF will be returned
    u16_fn = FA_MECH_LIMIT_SWITCH_Tasks();

    // if no function yet, see if the counter has a "reached target" message to report (FN_M1_TARGET, FN_M2_TARGET, etc)
    if (u16_fn == FN_NULL) {
        u16_fn = DEVICE_handle_counter();
    }

    // otherwise see if an IR command is pending
    if (u16_fn == FN_NULL) {
        u16_fn = FA_IR_Get_Command();
    }

    // otherwise see if a Contact Closure command is pending
    if (u16_fn == FN_NULL) {
        u16_fn = FA_MECH_CC_SWITCH_Tasks();
    }

    // otherwise see if an RS232 is pending
    if (u16_fn == FN_NULL) {
        u16_fn = FA_PROTOCOL_Get_FA_CMD();
    }


    // if we have a function to process from this pass of the main loop
    if ((u16_fn != FN_NULL) && (u16_fn != FN_LSW_OFF)) {

        // issue a command acknowledgement flash on the IR led
        FA_LAMP_Flash(250, Time_mS);

        // Perform device specific preprocessing - anything specific to the target mechanism that
        // we want to do in response to the command outside of targeting or activating the motor
        u16_fn = DEVICE_preprocessUserFunction(u16_fn);

        // Select what motor is next to move and where
        script_Action = SCRIPT_processScript(u16_fn);

        // determine what direction the motor needs to move in to get there
        script_Action = DEVICE_targetScriptEntry(script_Action);

        // if we're asking the motor to do something it's not already doing
        if (is_new_motor_action(script_Action)) {
            // stop the mechanism if we have a stop
            if ((script_Action.u16_Function == FN_USER_STOP) || (script_Action.u16_Function == FN_HARD_STOP) || (script_Action.u16_Function == FN_INHIBIT)) {
                FA_MECH_MOTOR_stop();
            }// otherwise if the motor is not against switch or configured limit
            else if (DEVICE_targetAtLimit(script_Action) == false) {
                // Kick off the motor activation state machine for the current motor
                FA_MECH_MOTOR_start(script_Action);
            }
        }
    }

    // handle PWM for each DC channel
    if (FRAMEWORK_NO_OF_DC_CHANNELS > 0) {
        FA_PWM_Handler(u8_next_pwm_chan);
        u8_next_pwm_chan = ((u8_next_pwm_chan + 1) % FRAMEWORK_NO_OF_DC_CHANNELS);
    }

    // manage the front panel LED
    FA_LAMP_Tasks();

    // manage the motor activation state machine
    FA_MECH_MOTOR_Tasks();
    FA_PROTOCOL_Tasks();
}

_Bool is_new_motor_action(t_motorAction action) {
    static t_motorAction last_script_action = {FN_NULL, motor_null};
    _Bool bRetVal;

    bRetVal = ((action.eMotorID != last_script_action.eMotorID) || (action.u16_Function != last_script_action.u16_Function));
    last_script_action.eMotorID = action.eMotorID;
    last_script_action.u16_Function = action.u16_Function;

    return bRetVal;
}

/*******************************************************************************
 End of File
 */
