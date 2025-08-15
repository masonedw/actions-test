/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    fa_counter.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _FRAMEWORK_H    /* Guard against multiple inclusion */
#define _FRAMEWORK_H


#include "framework_common.h"

#define FRAMEWORK_PCB_0123          1
#define FRAMEWORK_TIMER_PERIOD_US   51.2
#define FRAMEWORK_TMR_INTS_PER_MS   19.53125
#define FRAMEWORK_TMR_INTS_PER_S    19531.25

#define FRAMEWORK_HAS_TCPIP  1
#define FRAMEWORK_HAS_RS232  1
#define FRAMEWORK_HAS_IR     1

#define FRAMEWORK_NO_OF_USARTS        2
#define FRAMEWORK_485_PORT            sercom5_usart
#define FRAMEWORK_232_PORT            sercom0_usart
#define FRAMEWORK_MAX_TX_BUFFER_SIZE  500
#define FRAMEWORK_MAX_PACKET_SIZE     100

#define FRAMEWORK_NO_OF_DC_CHANNELS  4  

#define FRAMEWORK_SMART_EEPROM_SIZE    EEPROM_2K


// WATCHDOG TIMER PERIODS - cycle = 1.024khz
//------------------------------------------
// Select one - 
//-------------
//#define FA_APP_WATCHDOG_PERIOD      WDT_CONFIG_WINDOW_CYC8_Val        /*   7.81 ms */
//#define FA_APP_WATCHDOG_PERIOD      WDT_CONFIG_WINDOW_CYC16_Val       /*  15.63 ms */
//#define FA_APP_WATCHDOG_PERIOD      WDT_CONFIG_WINDOW_CYC32_Val       /*  31.25 ms */
//#define FA_APP_WATCHDOG_PERIOD      WDT_CONFIG_WINDOW_CYC64_Val       /*  62.50 ms */
//#define FA_APP_WATCHDOG_PERIOD      WDT_CONFIG_WINDOW_CYC128_Val      /* 125.00 ms */
//#define FA_APP_WATCHDOG_PERIOD      WDT_CONFIG_WINDOW_CYC256_Val      /* 250.00 ms */
#define FRAMEWORK_WATCHDOG_PERIOD      WDT_CONFIG_WINDOW_CYC512_Val        /* 500.00 ms */
//#define FA_APP_WATCHDOG_PERIOD      WDT_CONFIG_WINDOW_CYC1024_Val     /*  1 second */
//#define FA_APP_WATCHDOG_PERIOD      WDT_CONFIG_WINDOW_CYC2048_Val     /*  2 second */
//#define FA_APP_WATCHDOG_PERIOD      WDT_CONFIG_WINDOW_CYC4096_Val     /*  4 second */
//#define FA_APP_WATCHDOG_PERIOD      WDT_CONFIG_WINDOW_CYC8192_Val     /*  8 second */
//#define FA_APP_WATCHDOG_PERIOD      WDT_CONFIG_WINDOW_CYC16384_Val    /* 16 second */


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */


#endif