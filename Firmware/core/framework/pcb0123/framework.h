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

#include "definitions.h"
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

#define FRAMEWORK_NO_OF_DC_CHANNELS  1

#define FRAMEWORK_SMART_EEPROM_SIZE    EEPROM_2K

#define PF_NotInUse 0xFF


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

typedef struct {
    uint8_t Port_Group, Port_Pin;
} GPIO_PORT;

typedef enum {
    GPIO_CC1,
    GPIO_CC2,
    GPIO_CC3,
    GPIO_CC4,
    GPIO_RJ45_4,
    GPIO_RJ45_5,
    GPIO_RJ45_6,
    GPIO_RJ45_7,
    GPIO_RJ45_8,
    GPIO_Lamp,
    GPIO_DE_SERCOM5,
    GPIO_RE_SERCOM5,
    GPIO_M1_DIRA,
    GPIO_M1_DIRB,
    Sercom_RS232_RX,
    Sercom_RS232_TX,
    Sercom_AT24_RX,
    Sercom_AT24_TX,
    Sercom_RS485_DI,
    Sercom_RS485_DO,
    GPIO_PWM1,
    GPIO_485_Break,
    IR_RX,
    GMAC_GRX0,
    GMAC_GRX1,
    GMAC_GTXCK,
    GMAC_GRXER,
    GMAC_GTXEN,
    GMAC_GTX0,
    GMAC_GTX1,
    GMAC_GMDC,
    GMAC_GMDIO,
    GMAC_GRXDV,

    GPIO_Max_Names
} GPIO_NAME;

typedef enum {
    Group_A = 0,
    Group_B,
    Group_C,
    Group_D
} Group_Letter;

typedef enum {
    PERIPHERAL_FUNCTION_A_EIC = PERIPHERAL_FUNCTION_A,
    PERIPHERAL_FUNCTION_B_ANALOGUE = PERIPHERAL_FUNCTION_B,
    PERIPHERAL_FUNCTION_C_SERCOM = PERIPHERAL_FUNCTION_C,
    PERIPHERAL_FUNCTION_D_SERCOM = PERIPHERAL_FUNCTION_D,
    PERIPHERAL_FUNCTION_E_TC = PERIPHERAL_FUNCTION_E,
    PERIPHERAL_FUNCTION_F_TCC = PERIPHERAL_FUNCTION_F,
    PERIPHERAL_FUNCTION_G_TCC = PERIPHERAL_FUNCTION_G,
    PERIPHERAL_FUNCTION_G_PDEC = PERIPHERAL_FUNCTION_G,
    PERIPHERAL_FUNCTION_H_QSPI = PERIPHERAL_FUNCTION_H,
    PERIPHERAL_FUNCTION_H_CAN1 = PERIPHERAL_FUNCTION_H,
    PERIPHERAL_FUNCTION_H_USB = PERIPHERAL_FUNCTION_H,
    PERIPHERAL_FUNCTION_H_CORETEX_CM4 = PERIPHERAL_FUNCTION_H,
    PERIPHERAL_FUNCTION_I_SDHC = PERIPHERAL_FUNCTION_I,
    PERIPHERAL_FUNCTION_I_CAN0 = PERIPHERAL_FUNCTION_I,
    PERIPHERAL_FUNCTION_J_I2C = PERIPHERAL_FUNCTION_J,
    PERIPHERAL_FUNCTION_K_PCC = PERIPHERAL_FUNCTION_K,
    PERIPHERAL_FUNCTION_L_GMAC = PERIPHERAL_FUNCTION_L,
    PERIPHERAL_FUNCTION_M_GCLK = PERIPHERAL_FUNCTION_M,
    PERIPHERAL_FUNCTION_M_AC = PERIPHERAL_FUNCTION_M,
    PERIPHERAL_FUNCTION_N_CCL = PERIPHERAL_FUNCTION_N,
    PERIPHERAL_FUNCTION_IO = PF_NotInUse,

} FA_PERIPHERAL_FUNCTION;

//void FRAMEWORK_INIT(void);
void FRAMEWORK_init_pins(void);
void FRAMEWORK_PinPeripheralFunctionConfig(GPIO_PORT GPIO_PIN, FA_PERIPHERAL_FUNCTION function);
void FRAMEWORK_GPIO_SET(GPIO_PORT GPIO_PIN);
void FRAMEWORK_GPIO_Clear(GPIO_PORT GPIO_PIN);
void FRAMEWORK_GPIO_Toggle(GPIO_PORT GPIO_PIN);
void FRAMEWORK_GPIO_OutputEnable(GPIO_PORT GPIO_PIN);
void FRAMEWORK_GPIO_InputEnable(GPIO_PORT GPIO_PIN);
void FRAMEWORK_GPIO_InputDontPull(GPIO_PORT GPIO_PIN);
void FRAMEWORK_GPIO_InputPullUp(GPIO_PORT GPIO_PIN);
void FRAMEWORK_GPIO_InputPullDown(GPIO_PORT GPIO_PIN);
bool FRAMEWORK_GPIO_Get(GPIO_PORT GPIO_PIN);
bool FRAMEWORK_USART_SerialSetup_port_0(USART_SERIAL_CONFIG* serialSetup, uint32_t clkFrequency);
bool FRAMEWORK_USART_SerialSetup_port_1(USART_SERIAL_CONFIG* serialSetup, uint32_t clkFrequency);
bool FRAMEWORK_USART_SerialSetup_port_5(USART_SERIAL_CONFIG* serialSetup, uint32_t clkFrequency);
GPIO_NAME FRAMEWORK_get_adc_pin(t_adc_id adc_id);
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */


#endif