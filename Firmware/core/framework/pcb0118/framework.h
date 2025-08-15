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
#include "pcb0118_config.h"
#include "fa_usart.h"

#define FRAMEWORK_PIC_BUILD         1

#define FRAMEWORK_PCB_0118          1
#define FRAMEWORK_TIMER_PERIOD_US   100
#define FRAMEWORK_TMR_INTS_PER_MS   10
#define FRAMEWORK_TMR_INTS_PER_S    10000

#define FRAMEWORK_SMART_EEPROM_SIZE 1024

//#define FRAMEWORK_HAS_TCPIP  0
//#define FRAMEWORK_HAS_RS232  0
//#define FRAMEWORK_HAS_IR     0

#define FRAMEWORK_NO_OF_USARTS        1
#define FRAMEWORK_485_PORT            pic_usart
#define FRAMEWORK_MAX_TX_BUFFER_SIZE  64
#define FRAMEWORK_MAX_PACKET_SIZE     130


// GPIO assignments
#define FRONT_LED_PIN           PORTCbits.RC1
#define IR_PIN                  PORTBbits.RB0

#define RE_PIN                  PORTEbits.RE2
#define DE_PIN                  PORTAbits.RA0

#define E_STOP_PIN              PORTDbits.RD1

#define TRANSISTOR_PIN          PORTDbits.RD4

//DIRECTION
#define SHADE_1_DIRECTION_PIN   PORTCbits.RC0
#define SHADE_2_DIRECTION_PIN   PORTAbits.RA5
#define SHADE_3_DIRECTION_PIN   PORTDbits.RD5
#define SHADE_4_DIRECTION_PIN   PORTDbits.RD0
#define SHADE_5_DIRECTION_PIN   PORTCbits.RC3
#define SHADE_6_DIRECTION_PIN   PORTCbits.RC2
#define SHADE_7_DIRECTION_PIN   PORTAbits.RA4
#define SHADE_8_DIRECTION_PIN   PORTAbits.RA3
#define SHADE_9_DIRECTION_PIN   PORTAbits.RA2
#define SHADE_10_DIRECTION_PIN  PORTAbits.RA1

//POWER ON/OFF
#define SHADE_1_POWER_PIN       PORTDbits.RD3
#define SHADE_2_POWER_PIN       PORTEbits.RE0
#define SHADE_3_POWER_PIN       PORTEbits.RE1
#define SHADE_4_POWER_PIN       PORTDbits.RD2
#define SHADE_5_POWER_PIN       PORTCbits.RC4
#define SHADE_6_POWER_PIN       PORTDbits.RD6
#define SHADE_7_POWER_PIN       PORTDbits.RD7
#define SHADE_8_POWER_PIN       PORTBbits.RB1
#define SHADE_9_POWER_PIN       PORTBbits.RB2
#define SHADE_10_POWER_PIN      PORTBbits.RB3

#define TEST_PIN_1              PORTBbits.RB7
#define TEST_PIN_2              PORTBbits.RB6
#define TEST_PIN_3              PORTBbits.RB5
#define TEST_PIN_4              PORTBbits.RB4



/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */


#endif