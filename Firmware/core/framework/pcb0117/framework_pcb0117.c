#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <stdio.h>
#include "definitions.h"                // SYS function prototypes
#include "fa_timer.h"
#include "fa_usart.h"
#include "fa_at24.h"

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
void Timer_InterruptHandler(TC_TIMER_STATUS status, uintptr_t context) {
    u32_timer_tick_data++;
}

void FRAMEWORK_TIMER_init(void) {
    //Timer Initialise
    TC2_TimerInitialize();
    //Register callback function for TC2 period interrupt
    TC2_TimerCallbackRegister(Timer_InterruptHandler, (uintptr_t) NULL);
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
    SmartEEPROM8[u16_addr] = ((uint8_t) (u16_data & 0xFF));
    SmartEEPROM8[u16_addr + 1] = ((uint8_t) ((u16_data & 0xFF00) >> 8));
}

void FRAMEWORK_write_eeprom_32(uint16_t u16_addr, uint32_t u32_data) {
    while (NVMCTRL_SmartEEPROM_IsBusy());
    SmartEEPROM8[u16_addr] = ((uint8_t) (u32_data & 0x000000FF));
    SmartEEPROM8[u16_addr + 1] = ((uint8_t) ((u32_data & 0x0000FF00) >> 8));
    SmartEEPROM8[u16_addr + 2] = ((uint8_t) ((u32_data & 0x00FF0000) >> 16));
    SmartEEPROM8[u16_addr + 3] = ((uint8_t) ((u32_data & 0xFF000000) >> 24));
}

void FRAMEWORK_write_eeprom_block(uint16_t u16_addr, uint8_t * u8_data, uint16_t u16_length) {
    uint16_t u16_index;

    while (NVMCTRL_SmartEEPROM_IsBusy());
    for (u16_index = 0; u16_index < u16_length; u16_index++) {
        SmartEEPROM8[u16_addr + u16_index] = u8_data[u16_index];
    }
}

void FRAMEWORK_fill_eeprom_block(uint16_t u16_addr, uint8_t u8_data, uint16_t u16_length) {
    uint16_t u16_index;

    while (NVMCTRL_SmartEEPROM_IsBusy());
    for (u16_index = 0; u16_index < u16_length; u16_index++) {
        SmartEEPROM8[u16_addr + u16_index] = u8_data;
    }
}

uint8_t FRAMEWORK_read_eeprom_8(uint16_t u16_addr) {
    while (NVMCTRL_SmartEEPROM_IsBusy());
    return SmartEEPROM8[u16_addr];
}

uint16_t FRAMEWORK_read_eeprom_16(uint16_t u16_addr) {
    while (NVMCTRL_SmartEEPROM_IsBusy());
    return ( SmartEEPROM8[u16_addr] +
            (SmartEEPROM8[u16_addr + 1] << 8)
            );
}

uint32_t FRAMEWORK_read_eeprom_32(uint16_t u16_addr) {
    while (NVMCTRL_SmartEEPROM_IsBusy());
    return ( SmartEEPROM8[u16_addr] +
            (SmartEEPROM8[u16_addr + 1] << 8) +
            (SmartEEPROM8[u16_addr + 2] << 16) +
            (SmartEEPROM8[u16_addr + 3] << 24)
            );
}

void FRAMEWORK_read_eeprom_block(uint16_t u16_addr, uint8_t * u8_data, uint16_t u16_length) {
    uint16_t u16_index;

    while (NVMCTRL_SmartEEPROM_IsBusy());
    for (u16_index = 0; u16_index < u16_length; u16_index++) {
        u8_data[u16_index] = SmartEEPROM8[u16_addr + u16_index];
    }
}





//RS485
//--------

void FRAMEWORK_USART_init_port_0(t_data_framing eFraming) {
    // Do nothing
}

void FRAMEWORK_USART_init_port_1(t_data_framing eFraming) {
    // Do nothing
}

void FRAMEWORK_USART_init_port_5(t_data_framing eFraming) {
    // Do nothing
}

void FRAMEWORK_USART_re_set_port_0(void) {
    // Do nothing
}

void FRAMEWORK_USART_re_set_port_1(void) {
    // Do nothing
}

void FRAMEWORK_USART_re_set_port_5(void) {
    GPIO_PB02_485_RE_SERCOM5_Set();
}

void FRAMEWORK_USART_re_clear_port_0(void) {
    // Do nothing
}

void FRAMEWORK_USART_re_clear_port_1(void) {
    // Do nothing
}

void FRAMEWORK_USART_re_clear_port_5(void) {
    GPIO_PB02_485_RE_SERCOM5_Clear();
}

void FRAMEWORK_USART_de_set_port_0(void) {
    // Do nothing
}

void FRAMEWORK_USART_de_set_port_1(void) {
    // Do Nothing
}

void FRAMEWORK_USART_de_set_port_5(void) {
    GPIO_PB01_485_DE_SERCOM5_Set();
}

void FRAMEWORK_USART_de_clear_port_0(void) {
    // Do Nothing
}

void FRAMEWORK_USART_de_clear_port_1(void) {
    // Do Nothing
}

void FRAMEWORK_USART_de_clear_port_5(void) {
    GPIO_PB01_485_DE_SERCOM5_Clear();
}

void FRAMEWORK_USART_break_set_port_0(void) {
    // Do Nothing
}

void FRAMEWORK_USART_break_set_port_1(void) {
    // Do nothing
}

void FRAMEWORK_USART_break_set_port_5(void) {
    //GPIO_PC07_485_BREAK_Set();
}

void FRAMEWORK_USART_break_clear_port_0(void) {
    // Do nothing
}

void FRAMEWORK_USART_break_clear_port_1(void) {
    // Do nothing
}

void FRAMEWORK_USART_break_clear_port_5(void) {
    //GPIO_PC07_485_BREAK_Clear();
}

uint16_t FRAMEWORK_USART_ErrorGet_port_0(void) {
    return (uint16_t) SERCOM0_USART_ErrorGet();
}

uint16_t FRAMEWORK_USART_ErrorGet_port_1(void) {
    return 0;
}

uint16_t FRAMEWORK_USART_ErrorGet_port_5(void) {
    return (uint16_t) SERCOM5_USART_ErrorGet();
}

bool FRAMEWORK_USART_SerialSetup_port_0(USART_SERIAL_SETUP* serialSetup, uint32_t clkFrequency) {
    return SERCOM0_USART_SerialSetup(serialSetup, clkFrequency);
}

bool FRAMEWORK_USART_SerialSetup_port_1(USART_SERIAL_SETUP* serialSetup, uint32_t clkFrequency) {
    return SERCOM1_USART_SerialSetup(serialSetup, clkFrequency);
}

bool FRAMEWORK_USART_SerialSetup_port_5(USART_SERIAL_SETUP* serialSetup, uint32_t clkFrequency) {
    return SERCOM5_USART_SerialSetup(serialSetup, clkFrequency);
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

bool FRAMEWORK_USART_WriteIsBusy_port_0(void) {
    return SERCOM0_USART_WriteIsBusy();
}

bool FRAMEWORK_USART_WriteIsBusy_port_1(void) {
    return false;
}

bool FRAMEWORK_USART_WriteIsBusy_port_5(void) {
    return SERCOM5_USART_WriteIsBusy();
}

bool FRAMEWORK_USART_Write_port_0(void *buffer, const size_t size, t_data_framing eFraming) {
    return SERCOM0_USART_Write(buffer, size);
}

bool FRAMEWORK_USART_Write_port_1(void *buffer, const size_t size, t_data_framing eFraming) {
    return false;
}

bool FRAMEWORK_USART_Write_port_5(void *buffer, const size_t size, t_data_framing eFraming) {
    return SERCOM5_USART_Write(buffer, size);
}

void FRAMEWORK_USART_ReceiverEnable_port_0(void) {
    SERCOM0_USART_ReceiverEnable();
}

void FRAMEWORK_USART_ReceiverEnable_port_1(void) {
    // Do nothing
}

void FRAMEWORK_USART_ReceiverEnable_port_5(void) {
    SERCOM5_USART_ReceiverEnable();
}

void FRAMEWORK_USART_ReceiverDisable_port_0(void) {
    SERCOM0_USART_ReceiverDisable();
}

void FRAMEWORK_USART_ReceiverDisable_port_1(void) {
    // Do nothing
}

void FRAMEWORK_USART_ReceiverDisable_port_5(void) {
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

bool FRAMEWORK_USART_Read_port_0(void *buffer, const size_t size) {
    return SERCOM0_USART_Read(buffer, size);
}

bool FRAMEWORK_USART_Read_port_1(void *buffer, const size_t size) {
    return false;
}

bool FRAMEWORK_USART_Read_port_5(void *buffer, const size_t size) {
    return SERCOM5_USART_Read(buffer, size);
}

void FRAMEWORK_USART_RegisterReadCallback_port_0(void * callback_func, uintptr_t context) {
    SERCOM0_USART_ReadCallbackRegister(callback_func, context);
}

void FRAMEWORK_USART_RegisterReadCallback_port_1(void * callback_func, uintptr_t context) {
    // Do nothing
}

void FRAMEWORK_USART_RegisterReadCallback_port_5(void * callback_func, uintptr_t context) {
    SERCOM5_USART_ReadCallbackRegister(callback_func, context);
}

bool FRAMEWORK_USART_ByteTransmittedOrReceived_port_0(void) {
    return SERCOM0_USART_ByteTransmittedOrReceived();
}

bool FRAMEWORK_USART_ByteTransmittedOrReceived_port_1(void) {
    return SERCOM1_USART_ByteTransmittedOrReceived();
}

bool FRAMEWORK_USART_ByteTransmittedOrReceived_port_5(void) {
    return SERCOM5_USART_ByteTransmittedOrReceived();
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

uint8_t FRAMEWORK_read_RJ45(uint8_t u8_pin_index) {
    return 0;
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