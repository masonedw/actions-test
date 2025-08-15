/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "framework.h"
#include "logger/logger.h"
#include "logger/serial_sink.h"

extern void FA_APP_Initialize(void);
extern void FA_APP_Tasks(void);

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

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
/*******************************************************************************
 End of File
 */

