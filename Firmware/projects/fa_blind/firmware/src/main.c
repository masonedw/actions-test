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

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <plib_wdt.h>
#include "definitions.h"                // SYS function prototypes
#include "fa_ir.h"
#include "fa_eeprom_map.h"
#include "fa_eeprom.h"
#include "framework.h"

extern void FA_APP_Initialize ( void );
extern void FA_APP_Tasks ( void );

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************



int main ( void )
{
    /* Initialise watchdog and EEPROM */
    WDT_Disable();
    WDT_TimeoutPeriodSet(FRAMEWORK_WATCHDOG_PERIOD);
    
       
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    
    FA_APP_Initialize( );
    //GPIO_PC02_Set();

    WDT_Enable();
    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
        
        FA_APP_Tasks();
        
        WDT_Clear();
        
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}
/*******************************************************************************
 End of File
*/

