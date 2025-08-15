/*
 * File:   main.c
 * Author: graeme.capes
 *
 * Created on 03 July 2025, 13:35
 */


#include <xc.h>
#include "definitions.h"
#include "framework.h"
#include "fa_app.h"

int main(void) {
    /* Initialise watchdog and EEPROM */
    WDT_Disable();
    WDT_TimeoutPeriodSet(FRAMEWORK_WATCHDOG_PERIOD);

    FRAMEWORK_init_pins();

    /* Initialize all modules */
    SYS_Initialize(NULL);

    FA_APP_Initialize();

    WDT_Enable();
    while (true) {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks();

        FA_APP_Tasks();

        WDT_Clear();

    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE);
}
