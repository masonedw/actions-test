#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <stdio.h>
#include "definitions.h"                // SYS function prototypes
#include "fa_timer.h"
#include "fa_usart.h"
#include "fa_global.h"
#include "fa_at24.h"
#include "fa_mech.h"

extern uint32_t u32_timer_tick_data;

volatile uint8_t *SmartEEPROM8 = (uint8_t *) SEEPROM_ADDR; // virtual start address 0x44000000
bool bTimerOn = false;


void FRAMEWORK_MAIN_init_pins(void) {
    // Do nothing
}
        
/****************************************************************************
 * Interrupt service routine for the timer 
 * If the timer is off this will be called 
 * every 51.2uS 
*/
void Timer_InterruptHandler(TC_TIMER_STATUS status, uintptr_t context)
{
    u32_timer_tick_data++;
}


void FRAMEWORK_TIMER_init (void) {
    //Timer Initialise
    TC2_TimerInitialize();
    //Register callback function for TC2 period interrupt
    TC2_TimerCallbackRegister(Timer_InterruptHandler, (uintptr_t)NULL); 
}


float FRAMEWORK_TIMER_get_resolution(void) {
    return 51.2;
}


void FRAMEWORK_TIMER_start(void) {
    TC2_TimerStart();
    bTimerOn = true;    
}

void FRAMEWORK_TIMER_stop(void) {
    TC2_TimerStop();
    bTimerOn = false;    
}

bool FRAMEWORK_TIMER_running(void) {
    return bTimerOn;    
}



// EEPROM
// ------
void FRAMEWORK_write_eeprom_8(uint16_t u16_addr, uint8_t u8_data) {
    while (NVMCTRL_SmartEEPROM_IsBusy());    
    SmartEEPROM8[u16_addr] = u8_data;
}



void FRAMEWORK_write_eeprom_16(uint16_t u16_addr, uint16_t u16_data) {
    while (NVMCTRL_SmartEEPROM_IsBusy());
    SmartEEPROM8[u16_addr] = ((uint8_t)(u16_data & 0xFF));
    SmartEEPROM8[u16_addr+1] = ((uint8_t)((u16_data & 0xFF00) >> 8));
}



void FRAMEWORK_write_eeprom_32(uint16_t u16_addr, uint32_t u32_data) {    
    while (NVMCTRL_SmartEEPROM_IsBusy());
    SmartEEPROM8[u16_addr] = ((uint8_t)(u32_data & 0x000000FF));
    SmartEEPROM8[u16_addr+1] = ((uint8_t)((u32_data & 0x0000FF00) >> 8));
    SmartEEPROM8[u16_addr+2] = ((uint8_t)((u32_data & 0x00FF0000) >> 16));
    SmartEEPROM8[u16_addr+3] = ((uint8_t)((u32_data & 0xFF000000) >> 24));
}



void FRAMEWORK_write_eeprom_block(uint16_t u16_addr, uint8_t * u8_data, uint16_t u16_length) {    
    uint16_t u16_index;
    
    while (NVMCTRL_SmartEEPROM_IsBusy());
    for (u16_index = 0; u16_index < u16_length; u16_index++) {
        SmartEEPROM8[u16_addr+u16_index] = u8_data[u16_index];
    }
}


void FRAMEWORK_fill_eeprom_block(uint16_t u16_addr, uint8_t u8_data, uint16_t u16_length) {    
    uint16_t u16_index;
    
    while (NVMCTRL_SmartEEPROM_IsBusy());
    for (u16_index = 0; u16_index < u16_length; u16_index++) {
        SmartEEPROM8[u16_addr+u16_index] = u8_data;
    }
}



uint8_t FRAMEWORK_read_eeprom_8(uint16_t u16_addr) {
    while (NVMCTRL_SmartEEPROM_IsBusy());
    return SmartEEPROM8[u16_addr];
}



uint16_t FRAMEWORK_read_eeprom_16(uint16_t u16_addr) {
    while (NVMCTRL_SmartEEPROM_IsBusy());
    return ( SmartEEPROM8[u16_addr] + 
            (SmartEEPROM8[u16_addr+1] << 8)
           );
}



uint32_t FRAMEWORK_read_eeprom_32(uint16_t u16_addr) {
    while (NVMCTRL_SmartEEPROM_IsBusy());
    return ( SmartEEPROM8[u16_addr] + 
            (SmartEEPROM8[u16_addr+1] << 8) +
            (SmartEEPROM8[u16_addr+2] << 16) +
            (SmartEEPROM8[u16_addr+3] << 24)
           );
}



void FRAMEWORK_read_eeprom_block(uint16_t u16_addr, uint8_t * u8_data, uint16_t u16_length) {
    uint16_t u16_index;
    
    while (NVMCTRL_SmartEEPROM_IsBusy());
    for (u16_index = 0; u16_index < u16_length; u16_index++) {
        u8_data[u16_index] = SmartEEPROM8[u16_addr+u16_index];
    }
}





//RS485
//--------
void FRAMEWORK_USART_init_port_0 (t_data_framing eFraming) {
    // Do nothing
}


void FRAMEWORK_USART_init_port_1 (t_data_framing eFraming) {
    // Do nothing
}


void FRAMEWORK_USART_init_port_5 (t_data_framing eFraming) {
    // Do nothing
}



void FRAMEWORK_USART_re_set_port_0(void) {
    // Do nothing
}

void FRAMEWORK_USART_re_set_port_1(void) {
    // Do nothing
}

void FRAMEWORK_USART_re_set_port_5(void) {
    GPIO_PA25_485_RE_SERCOM5_Set();
}


void FRAMEWORK_USART_re_clear_port_0(void) {
    // Do nothing
}

void FRAMEWORK_USART_re_clear_port_1(void) {
    // Do nothing
}

void FRAMEWORK_USART_re_clear_port_5(void) {
    GPIO_PA25_485_RE_SERCOM5_Clear();
}


void FRAMEWORK_USART_de_set_port_0(void) {
    // Do nothing
}

void FRAMEWORK_USART_de_set_port_1(void) {
    // Do Nothing
}

void FRAMEWORK_USART_de_set_port_5(void) {
    GPIO_PA24_485_DE_SERCOM5_Set();
}


void FRAMEWORK_USART_de_clear_port_0(void) {
    // Do Nothing
}

void FRAMEWORK_USART_de_clear_port_1(void) {
    // Do Nothing
}

void FRAMEWORK_USART_de_clear_port_5(void) {
    GPIO_PA24_485_DE_SERCOM5_Clear();
}


void FRAMEWORK_USART_break_set_port_0(void) {
    // Do Nothing
}


void FRAMEWORK_USART_break_set_port_1(void) {
    // Do nothing
}

void FRAMEWORK_USART_break_set_port_5(void) {
    GPIO_PC07_485_BREAK_Set();
}


void FRAMEWORK_USART_break_clear_port_0(void) {
    // Do nothing
}

void FRAMEWORK_USART_break_clear_port_1(void) {
    // Do nothing
}

void FRAMEWORK_USART_break_clear_port_5(void) {
    GPIO_PC07_485_BREAK_Clear();
}


uint16_t FRAMEWORK_USART_ErrorGet_port_0(void) {
    return (uint16_t)SERCOM0_USART_ErrorGet();
}


uint16_t FRAMEWORK_USART_ErrorGet_port_1(void) {
    return 0;
}


uint16_t FRAMEWORK_USART_ErrorGet_port_5(void) {
    return (uint16_t)SERCOM5_USART_ErrorGet();
}


void FRAMEWORK_USART_TransmitterEnable_port_0(void) {
    SERCOM0_USART_TransmitterEnable();
}


void FRAMEWORK_USART_TransmitterEnable_port_1(void) {
    // Do nothing
}


void FRAMEWORK_USART_TransmitterEnable_port_5(void) {
    SERCOM5_USART_TransmitterEnable();
}


void FRAMEWORK_USART_TransmitterDisable_port_0(void) {
    return SERCOM0_USART_TransmitterDisable();
}


void FRAMEWORK_USART_TransmitterDisable_port_1(void) {
    // Do nothing
}


void FRAMEWORK_USART_TransmitterDisable_port_5(void) {
    return SERCOM5_USART_TransmitterDisable();
}


bool FRAMEWORK_USART_WriteIsBusy_port_0( void ) {
    return SERCOM0_USART_WriteIsBusy();
}

bool FRAMEWORK_USART_WriteIsBusy_port_1( void ) {
    return false;
}

bool FRAMEWORK_USART_WriteIsBusy_port_5( void ) {
    return SERCOM5_USART_WriteIsBusy();
}


bool FRAMEWORK_USART_Write_port_0( void *buffer, const size_t size, t_data_framing eFraming ) {
    return SERCOM0_USART_Write(buffer, size);
}


bool FRAMEWORK_USART_Write_port_1( void *buffer, const size_t size, t_data_framing eFraming ) {
    return false;
}

bool FRAMEWORK_USART_Write_port_5( void *buffer, const size_t size, t_data_framing eFraming ) {
    return SERCOM5_USART_Write(buffer, size);
}

void FRAMEWORK_USART_ReceiverEnable_port_0( void ) {
    SERCOM0_USART_ReceiverEnable();
}

void FRAMEWORK_USART_ReceiverEnable_port_1( void ) {
    // Do nothing
}

void FRAMEWORK_USART_ReceiverEnable_port_5( void ) {
    SERCOM5_USART_ReceiverEnable();
}

void FRAMEWORK_USART_ReceiverDisable_port_0( void ) {
    SERCOM0_USART_ReceiverDisable();
}

void FRAMEWORK_USART_ReceiverDisable_port_1( void ) {
    // Do nothing
}

void FRAMEWORK_USART_ReceiverDisable_port_5( void ) {
    SERCOM5_USART_ReceiverDisable();
}


bool FRAMEWORK_USART_ReadAbort_port_0(void) {
    return SERCOM0_USART_ReadAbort();
}


bool FRAMEWORK_USART_ReadAbort_port_1(void) {
    return false;
}

bool FRAMEWORK_USART_ReadAbort_port_5(void) {
    return SERCOM5_USART_ReadAbort();
}


bool FRAMEWORK_USART_Read_port_0( void *buffer, const size_t size ) {
    return SERCOM0_USART_Read( buffer, size );
}


bool FRAMEWORK_USART_Read_port_1( void *buffer, const size_t size ) {
    return false;
}

bool FRAMEWORK_USART_Read_port_5( void *buffer, const size_t size ) {
    return SERCOM5_USART_Read( buffer, size );
}


void FRAMEWORK_USART_RegisterReadCallback_port_0 (void * callback_func, uintptr_t context) {
    SERCOM0_USART_ReadCallbackRegister(callback_func, context);
}


void FRAMEWORK_USART_RegisterReadCallback_port_1 (void * callback_func, uintptr_t context) {
    // Do nothing
}

void FRAMEWORK_USART_RegisterReadCallback_port_5 (void * callback_func, uintptr_t context) {
    SERCOM5_USART_ReadCallbackRegister(callback_func, context);
}


uint32_t FRAMEWORK_get_rx_timeout_port_0(void) {
    return 5000;
}


uint32_t FRAMEWORK_get_rx_timeout_port_1(void) {
    return 0;
}

uint32_t FRAMEWORK_get_rx_timeout_port_5(void) {
    return 5000;
}


//GPIO
//--------
uint8_t FRAMEWORK_read_CCx(t_chan_ccx ccx) {
    uint8_t u8_ret_val = 1;
    switch (ccx) {
        case cc1: 
            u8_ret_val = GPIO_PC30_DC1_CC1_Get();
            break;
        case cc2: 
            u8_ret_val = GPIO_PC31_DC1_CC2_Get();
            break;
        case cc3: 
            u8_ret_val = GPIO_PC02_DC1_CC3_Get();
            break;
        case cc4: 
            u8_ret_val = GPIO_PC03_DC1_CC4_Get();
            break;
        case cc5: 
            u8_ret_val = GPIO_PA09_DC1_CC5_Get();
            break;
        case cc6: 
            // Analogue
            break;
        case cc7: 
            u8_ret_val = GPIO_PC13_DC2_CC7_Get();
            break;
        case cc8: 
            u8_ret_val = GPIO_PC14_DC2_CC8_Get();
            break;
        case cc9:
            u8_ret_val = GPIO_PB15_DC2_CC9_Get();
            break;
        case cc10:
            u8_ret_val = GPIO_PA16_DC2_CC10_Get();
            break;
        case cc11:
            u8_ret_val = GPIO_PC16_DC2_CC11_Get();
            break;
        case cc12: 
            // Analogue
            break;
        case cc13:
            u8_ret_val = GPIO_PC17_DC3_CC13_Get();
            break;
        case cc14: 
            u8_ret_val = GPIO_PC28_DC3_CC14_Get();
            break;
        case cc15:
            u8_ret_val = GPIO_PC00_DC3_CC15_Get();
            break;
        case cc16:
            u8_ret_val = GPIO_PD20_DC3_CC16_Get();
            break;
        case cc17:
            u8_ret_val = GPIO_PD21_DC3_CC17_Get();
            break;
        case cc18: 
            // Analogue
            break;
        case cc19:
            u8_ret_val = GPIO_PB18_DC4_CC19_Get();
            break;
        case cc20:
            u8_ret_val = GPIO_PB19_DC4_CC20_Get();
            break;
        case cc21:
            u8_ret_val = GPIO_PB20_DC4_CC21_Get();
            break;
        case cc22:
            u8_ret_val = GPIO_PB21_DC4_CC22_Get();
            break;
        case cc23: 
            u8_ret_val = GPIO_PA20_DC4_CC23_Get();    
            break;
        case cc24: 
            // Analogue
            break;
        default:
            break;
    }
    return u8_ret_val;
}

uint8_t FRAMEWORK_read_RJ45(uint8_t u8_pin_index) {
    uint8_t u8_ret_val;
    
    switch (u8_pin_index) {
        case RJ45_PIN_8_INDEX:
            u8_ret_val = GPIO_PB25_RJ45_PIN_8_Get();
            break;
        case RJ45_PIN_7_INDEX:
            u8_ret_val = GPIO_PB24_RJ45_PIN_7_Get();
            break;
        case RJ45_PIN_6_INDEX:
            u8_ret_val = GPIO_PB26_RJ45_PIN_6_Get();
            break;
        case RJ45_PIN_5_INDEX:
            u8_ret_val = GPIO_PB27_RJ45_PIN_5_Get();
            break;
        case RJ45_PIN_4_INDEX:
            u8_ret_val = GPIO_PB28_RJ45_PIN_4_Get();
            break;
    }
    return u8_ret_val;
}


void FRAMEWORK_lamp_on(void) {
    GPIO_PC18_LAMP_Clear();
}


void FRAMEWORK_lamp_off(void) {
    GPIO_PC18_LAMP_Set();        
}



_Bool FRAMEWORK_get_mac(uint8_t * dest) {
    return FA_AT24_get_mac(dest);
}


uint32_t FRAMEWORK_get_tcc_period(t_channel_id pwm_chan) {
    return TCC0_PWM24bitPeriodGet();
}



_Bool FRAMEWORK_set_pwm_duty(t_channel_id pwm_chan, uint32_t u32_duty_val) {
    _Bool b_retval;
    
    switch (pwm_chan) {
        case chan_dc1:
            b_retval = TCC0_PWM24bitDutySet(TCC0_CHANNEL3, u32_duty_val);
            break;
            
        case chan_dc2:
            b_retval = TCC0_PWM24bitDutySet(TCC0_CHANNEL0, u32_duty_val);
            break;
           
        case chan_dc3:
            b_retval = TCC0_PWM24bitDutySet(TCC0_CHANNEL1, u32_duty_val);
            break;

        case chan_dc4:
            b_retval = TCC0_PWM24bitDutySet(TCC0_CHANNEL2, u32_duty_val);
            break;

        default:
            b_retval = false;
            break;
    } 
    return b_retval;
}



void FRAMEWORK_activate_tcc_pwm(t_channel_id dc_chan) {
    TCC0_PWMStart();
 }



void FRAMEWORK_deactivate_tcc_pwm(t_channel_id dc_chan) {
    TCC0_PWMStop();
 }


#ifdef PLIB_ADC_COMMON_H
void FRAMEWORK_init_adc(void) {
    ADC0_Enable();
}

_Bool FRAMEWORK_select_adc_channel(t_adc_id adc_id) {
    _Bool b_ret_val = true;
    
    switch(adc_id){
        case adc_dc1: 
            ADC0_ChannelSelect(ADC_INPUTCTRL_MUXPOS_AIN1, ADC_INPUTCTRL_MUXNEG_AIN1);
            break;
        case adc_dc2: 
            ADC0_ChannelSelect(ADC_INPUTCTRL_MUXPOS_AIN2, ADC_INPUTCTRL_MUXNEG_AIN2);
            break;  
        case adc_dc3:
            ADC0_ChannelSelect(ADC_INPUTCTRL_MUXPOS_AIN3, ADC_INPUTCTRL_MUXNEG_AIN3);
            break;
        case adc_dc4:
            ADC0_ChannelSelect(ADC_INPUTCTRL_MUXPOS_AIN7, ADC_INPUTCTRL_MUXNEG_AIN7);
            break;
        default:
            b_ret_val = false;
            break;
    }
    return b_ret_val;
}

void FRAMEWORK_start_adc_conversion(t_adc_id adc_id) {
    switch(adc_id){
        case adc_dc1: 
        case adc_dc2: 
        case adc_dc3: 
        case adc_dc4: 
            ADC0_ConversionStart();
            break;
        default:
            break;
    }
}


bool FRAMEWORK_get_adc_conversion_status(t_adc_id adc_id) {
    bool b_ret_val  = false;
    switch(adc_id){
        case adc_dc1: 
        case adc_dc2: 
        case adc_dc3: 
        case adc_dc4: 
            b_ret_val = ADC0_ConversionStatusGet();
            break;
        default:
            break;
    }
    return b_ret_val;
}


uint16_t FRAMEWORK_get_last_adc_conversion_result(t_adc_id adc_id) {
    uint16_t  u16_ret_val = 0;
    switch(adc_id){
        case adc_dc1: 
        case adc_dc2: 
        case adc_dc3: 
        case adc_dc4: 
            u16_ret_val = ADC0_LastConversionResultGet();
            break;
        default:
            break;
    }
    return u16_ret_val;

}
#endif