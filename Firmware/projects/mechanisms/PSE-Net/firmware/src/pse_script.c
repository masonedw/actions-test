/*
 * Project name:
     Future Automation Motor I/O PIC family
 * File name:
     script_PL.c
 * Copyright:
     (c) Future Automation.
 * Revision History:
     Created: 01/03/2010 - G. Capes
 * Description:
     - Function scripts for for PL sequential AC motors.

 * Test configuration:
     MCU:             PIC16F887
                      http://ww1.microchip.com/downloads/en/DeviceDoc/41291F.pdf
     Dev.Board:       EasyPIC6
                      http://www.mikroe.com/en/tools/easypic6/
     Oscillator:      HS, 20.0000 MHz
     Ext. Modules:    -
     SW:              mikroC PRO for PIC
                      http://www.mikroe.com/en/compilers/mikroc/pro/pic/
 */

#include "hw.h"
#include "global-definitions.h"
#include "fa_packet.h"
#include "fa_mech_app.h"

//*************************************************************************
// Output Script
//-------------------------------------------------------------------------
// This chip coordinates the movement of 3 motors in response to a user IN or OUT command.
// These complex movements are scripted below. Each script is advanced upon receipt of an
// inhibit stop for the currently active motor. When the script has completed, any inhibit
// stop will continue the final OUTP_NULL command until a new script is started. This results
// in all post-script inhibit stops actually executing an motor inhibit rather than a motor
// transition
//*************************************************************************
const t_motorAction scriptOut[3] = {
    {FN_OUT, HW_PSE_MOTOR},
    {FN_HARD_STOP, HW_PSE_MOTOR},
};
const t_motorAction scriptIn[3] = {
    {FN_IN, HW_PSE_MOTOR},
    {FN_HARD_STOP, HW_PSE_MOTOR},
};
//const t_motorAction scriptHome[2] = { {FN_PAIRED_HOME, HW_ARM_MOTOR}, {FN_INHIBIT, HW_ARM_MOTOR}};
const t_motorAction scriptA[3] = {
    {FN_A, HW_PSE_MOTOR},
    {FN_HARD_STOP, HW_PSE_MOTOR},
};
const t_motorAction scriptB[3] = {
    {FN_B, HW_PSE_MOTOR},
    {FN_HARD_STOP, HW_PSE_MOTOR},
};
const t_motorAction scriptC[3] = {
    {FN_C, HW_PSE_MOTOR},
    {FN_HARD_STOP, HW_PSE_MOTOR},
};
const t_motorAction scriptD[3] = {
    {FN_D, HW_PSE_MOTOR},
    {FN_HARD_STOP, HW_PSE_MOTOR},
};
const t_motorAction scriptE[3] = {
    {FN_E, HW_PSE_MOTOR},
    {FN_HARD_STOP, HW_PSE_MOTOR},
};
const t_motorAction scriptF[3] = {
    {FN_F, HW_PSE_MOTOR},
    {FN_HARD_STOP, HW_PSE_MOTOR},
};
const t_motorAction scriptPreset[3] = {
    {FN_PRESET, HW_PSE_MOTOR},
    {FN_HARD_STOP, HW_PSE_MOTOR},
};
const t_motorAction scriptNone[3] = {
    {FN_NULL, motor_null},
};
const t_motorAction scriptUserStop[3] = {
    {FN_USER_STOP, AnyMotor},
};
const t_motorAction scriptInhibit[3] = {
    {FN_HARD_STOP, AnyMotor},
};


const t_motorAction * activeScript = scriptNone;
uint8_t u8_scriptIdx = 0;
uint8_t u8_lastScriptIdx;

//************************************
// PROCESS AND ADVANCE SCRIPT
//************************************

t_motorAction SCRIPT_processScript(uint8_t u16_function) {

    // test for invalid limit switch states
    //if (LIMIT_SWITCH_criticalError()) {
    //    activeScript = scriptInhibit;
    //    scriptIdx = 0;
    //}
    if (u16_function == FN_USER_STOP) {
        activeScript = scriptUserStop;
        u8_scriptIdx = 0;
    }// Function: IN  - start the In script
    else if (u16_function == FN_IN) {
        activeScript = scriptIn;
        u8_scriptIdx = 0;
    }// Function: OUT  - start the Out script
    else if (u16_function == FN_OUT) {
        activeScript = scriptOut;
        u8_scriptIdx = 0;
    }// Function: OUT  - start the preset D script
    else if (u16_function == FN_A) {
        activeScript = scriptA;
        u8_scriptIdx = 0;
    }// Function: OUT  - start the preset E script
    else if (u16_function == FN_B) {
        activeScript = scriptB;
        u8_scriptIdx = 0;
    }// Function: OUT  - start the preset F script
    else if (u16_function == FN_C) {
        activeScript = scriptC;
        u8_scriptIdx = 0;
    }// Function: OUT  - start the preset D script
    else if (u16_function == FN_D) {
        activeScript = scriptD;
        u8_scriptIdx = 0;
    }// Function: OUT  - start the preset E script
    else if (u16_function == FN_E) {
        activeScript = scriptE;
        u8_scriptIdx = 0;
    }// Function: OUT  - start the preset F script
    else if (u16_function == FN_F) {
        activeScript = scriptF;
        u8_scriptIdx = 0;
    }// Function: OUT  - start the preset F script
    else if (u16_function == FN_PRESET) {
        activeScript = scriptPreset;
        u8_scriptIdx = 0;
    } else if (u16_function == FN_ADVANCE_SCRIPT) {
        if (u8_scriptIdx < (sizeof (activeScript) / sizeof (t_motorAction)) - 1) {
            u8_scriptIdx++;
        }
    }// Function: INHIBIT STOP - advance the current script (ie transfer control to the next motor)
        //else if (u8_function == FN_LSW_ON) {
        //    // advance script if not at end
        //    if (activeScript[u8_scriptIdx].sSubFunction != FN_INHIBIT) {
        //        u8_scriptIdx++;
        //    }
        //}
        // Unexpected limit switch activity - typically would be an engineer manually operating the switches
        //else if (u8_function == FN_LSW_MANUAL) {
        //    activeScript = scriptInhibit;
        //    u8_scriptIdx = 0;
        //}
        // Function: ANY OTHER
    else {
        activeScript = scriptInhibit;
        u8_scriptIdx = 0;
    }


    return activeScript[u8_scriptIdx];
}