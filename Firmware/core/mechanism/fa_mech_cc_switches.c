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
#include "framework.h"
#include "fa_timer.h"
#include "GPIO_Control.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define DEBOUNCE_PERIOD    20
#define MAX_CC_SWITCHES     5

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


uint16_t fn_table[10] = {HW_CC_PIN_8_SHORTED,
    HW_CC_PIN_7_SHORTED,
    HW_CC_PIN_6_SHORTED,
    HW_CC_PIN_5_SHORTED,
    HW_CC_PIN_4_SHORTED,
    HW_CC_PIN_8_RELEASED,
    HW_CC_PIN_7_RELEASED,
    HW_CC_PIN_6_RELEASED,
    HW_CC_PIN_5_RELEASED,
    HW_CC_PIN_4_RELEASED};





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



//*************************************************
// MAIN HANDLER
//*************************************************
//
//
//*************************************************

uint16_t FA_MECH_CC_SWITCH_Tasks(void) {
    uint16_t u16_Return = FN_NULL;

    if (GPIO_Check_For_State_Change(GPIO_RJ45_4)) {
        if (GPIO_Read_Debounced_State(GPIO_RJ45_4)) {
            u16_Return = fn_table[4];
        } else {
            u16_Return = fn_table[9];
        }
    } else if (GPIO_Check_For_State_Change(GPIO_RJ45_5)) {
        if (GPIO_Read_Debounced_State(GPIO_RJ45_5)) {
            u16_Return = fn_table[3];
        } else {
            u16_Return = fn_table[8];
        }
    } else if (GPIO_Check_For_State_Change(GPIO_RJ45_6)) {
        if (GPIO_Read_Debounced_State(GPIO_RJ45_6)) {
            u16_Return = fn_table[2];
        } else {
            u16_Return = fn_table[7];
        }
    } else if (GPIO_Check_For_State_Change(GPIO_RJ45_7)) {
        if (GPIO_Read_Debounced_State(GPIO_RJ45_7)) {
            u16_Return = fn_table[1];
        } else {
            u16_Return = fn_table[6];
        }
    } else if (GPIO_Check_For_State_Change(GPIO_RJ45_8)) {
        if (GPIO_Read_Debounced_State(GPIO_RJ45_8)) {
            u16_Return = fn_table[0];
        } else {
            u16_Return = fn_table[5];
        }
    }
    return u16_Return;
}




/*******************************************************************************
 End of File
 */
