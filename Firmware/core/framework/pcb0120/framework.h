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

#include "pcb0120_config.h"
#include "framework_common.h"

#define FRAMEWORK_PCB_0120     1
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

#define TRANSISTOR_PIN          PORTDbits.RD4

//Switch
#define SWITCH_1_PIN   PORTBbits.RB3
#define SWITCH_2_PIN   PORTBbits.RB2
#define SWITCH_3_PIN   PORTBbits.RB1
#define SWITCH_4_PIN   PORTAbits.RA5
#define SWITCH_5_PIN   PORTDbits.RD7
#define SWITCH_6_PIN   PORTDbits.RD6
#define SWITCH_7_PIN   PORTCbits.RC0
#define SWITCH_8_PIN   PORTCbits.RC4
#define SWITCH_9_PIN   PORTDbits.RD2
#define SWITCH_10_PIN  PORTDbits.RD0
#define SWITCH_11_PIN  PORTBbits.RB4
#define SWITCH_12_PIN  PORTBbits.RB6

//Switch LED
#define SWITCH_1_LED_PIN   PORTAbits.RA1
#define SWITCH_2_LED_PIN   PORTAbits.RA2
#define SWITCH_3_LED_PIN   PORTAbits.RA3
#define SWITCH_4_LED_PIN   PORTAbits.RA4
#define SWITCH_5_LED_PIN   PORTEbits.RE0
#define SWITCH_6_LED_PIN   PORTEbits.RE1
#define SWITCH_7_LED_PIN   PORTDbits.RD5
#define SWITCH_8_LED_PIN   PORTCbits.RC2
#define SWITCH_9_LED_PIN   PORTDbits.RD3
#define SWITCH_10_LED_PIN  PORTCbits.RC3
#define SWITCH_11_LED_PIN  PORTBbits.RB5
#define SWITCH_12_LED_PIN  PORTBbits.RB7



/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */


#endif