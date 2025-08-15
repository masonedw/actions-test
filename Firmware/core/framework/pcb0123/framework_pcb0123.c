#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <stdio.h>
#include "definitions.h"
#include "framework.h"                // SYS function prototypes
#include "fa_timer.h"
#include "fa_usart.h"
#include "fa_at24.h"
#include "GPIO_Control.h"

volatile uint8_t *SmartEEPROM8 = (uint8_t *)SEEPROM_ADDR; // virtual start address 0x44000000
bool bTimerOn = false;


void FRAMEWORK_init_Common_pins(void);
static USART_SERIAL_SETUP ConvertSerialConfig(USART_SERIAL_CONFIG* Usart_Config);

extern void FA_PWM_period(void);
extern uint32_t u32_timer_tick_data;

/**
 * @brief Initialize all project-specific GPIO pins.
 *
 * This function configures all required GPIO pins with their respective port group,
 * pin number, and direction (input/output). It is typically called during system startup.
 */
void FRAMEWORK_init_pins(void) {
    GPIO_Config_Pin(GPIO_CC1, Group_C, 30, GPIO_Input, PERIPHERAL_FUNCTION_IO, No_Soft_Pull, Orientation_NonInverted);
    GPIO_Config_Pin(GPIO_CC2, Group_C, 31, GPIO_Input, PERIPHERAL_FUNCTION_IO, No_Soft_Pull, Orientation_NonInverted);
    GPIO_Config_Pin(GPIO_CC3, Group_C, 2, GPIO_Input, PERIPHERAL_FUNCTION_IO, No_Soft_Pull, Orientation_NonInverted);
    GPIO_Config_Pin(GPIO_CC4, Group_C, 3, GPIO_Input, PERIPHERAL_FUNCTION_IO, No_Soft_Pull, Orientation_NonInverted);
    GPIO_Config_Pin(GPIO_RJ45_7, Group_B, 24, GPIO_Input, PERIPHERAL_FUNCTION_IO, Soft_Pull_Up, Orientation_NonInverted);
    GPIO_Config_Pin(GPIO_RJ45_8, Group_B, 25, GPIO_Input, PERIPHERAL_FUNCTION_IO, Soft_Pull_Up, Orientation_NonInverted);
    GPIO_Config_Pin(GPIO_RJ45_6, Group_B, 26, GPIO_Input, PERIPHERAL_FUNCTION_IO, Soft_Pull_Up, Orientation_NonInverted);
    GPIO_Config_Pin(GPIO_RJ45_5, Group_B, 27, GPIO_Input, PERIPHERAL_FUNCTION_IO, Soft_Pull_Up, Orientation_NonInverted);
    GPIO_Config_Pin(GPIO_RJ45_4, Group_B, 28, GPIO_Input, PERIPHERAL_FUNCTION_IO, Soft_Pull_Up, Orientation_NonInverted);
    GPIO_Config_Pin(GPIO_Lamp, Group_C, 18, GPIO_Output, PERIPHERAL_FUNCTION_IO, No_Soft_Pull, Orientation_Inverted);
    GPIO_Config_Pin(GPIO_DE_SERCOM5, Group_A, 24, GPIO_Output, PERIPHERAL_FUNCTION_IO, No_Soft_Pull, Orientation_NonInverted);
    GPIO_Config_Pin(GPIO_RE_SERCOM5, Group_A, 25, GPIO_Output, PERIPHERAL_FUNCTION_IO, No_Soft_Pull, Orientation_NonInverted);
    GPIO_Config_Pin(GPIO_M1_DIRA, Group_C, 1, GPIO_Output, PERIPHERAL_FUNCTION_IO, No_Soft_Pull, Orientation_NonInverted);
    GPIO_Config_Pin(GPIO_M1_DIRB, Group_C, 10, GPIO_Output, PERIPHERAL_FUNCTION_IO, No_Soft_Pull, Orientation_NonInverted);
    GPIO_Config_Pin(GPIO_PWM1, Group_D, 10, PWM_PAD, PERIPHERAL_FUNCTION_F_TCC, No_Soft_Pull, Orientation_NonInverted);
    GPIO_Config_Pin(GPIO_485_Break, Group_C, 7, GPIO_Output, PERIPHERAL_FUNCTION_IO, No_Soft_Pull, Orientation_NonInverted);
    //IR Interrupt Pin INIT
    GPIO_Config_Pin(IR_RX, Group_A, 06, EIC_INT, PERIPHERAL_FUNCTION_A_EIC, No_Soft_Pull, Orientation_NonInverted);

    // GMAC Pins
    GPIO_Config_Pin(GMAC_GRX1, Group_A, 12, GMAC, PERIPHERAL_FUNCTION_L_GMAC, No_Soft_Pull, Orientation_NonInverted);
    GPIO_Config_Pin(GMAC_GRX0, Group_A, 13, GMAC, PERIPHERAL_FUNCTION_L_GMAC, No_Soft_Pull, Orientation_NonInverted);
    GPIO_Config_Pin(GMAC_GTXCK, Group_A, 14, GMAC, PERIPHERAL_FUNCTION_L_GMAC, No_Soft_Pull, Orientation_NonInverted);
    GPIO_Config_Pin(GMAC_GRXER, Group_A, 15, GMAC, PERIPHERAL_FUNCTION_L_GMAC, No_Soft_Pull, Orientation_NonInverted);
    GPIO_Config_Pin(GMAC_GTXEN, Group_A, 17, GMAC, PERIPHERAL_FUNCTION_L_GMAC, No_Soft_Pull, Orientation_NonInverted);
    GPIO_Config_Pin(GMAC_GTX0, Group_A, 18, GMAC, PERIPHERAL_FUNCTION_L_GMAC, No_Soft_Pull, Orientation_NonInverted);
    GPIO_Config_Pin(GMAC_GTX1, Group_A, 19, GMAC, PERIPHERAL_FUNCTION_L_GMAC, No_Soft_Pull, Orientation_NonInverted);
    GPIO_Config_Pin(GMAC_GMDC, Group_C, 11, GMAC, PERIPHERAL_FUNCTION_L_GMAC, No_Soft_Pull, Orientation_NonInverted);
    GPIO_Config_Pin(GMAC_GMDIO, Group_C, 12, GMAC, PERIPHERAL_FUNCTION_L_GMAC, No_Soft_Pull, Orientation_NonInverted);
    GPIO_Config_Pin(GMAC_GRXDV, Group_C, 20, GMAC, PERIPHERAL_FUNCTION_L_GMAC, No_Soft_Pull, Orientation_NonInverted);


    //Sercom Pins INIT
    GPIO_Config_Pin(Sercom_RS232_RX, Group_A, 4, SERCOM_PAD, PERIPHERAL_FUNCTION_D_SERCOM, No_Soft_Pull, Orientation_NonInverted);
    GPIO_Config_Pin(Sercom_RS232_TX, Group_A, 5, SERCOM_PAD, PERIPHERAL_FUNCTION_D_SERCOM, No_Soft_Pull, Orientation_NonInverted);
    GPIO_Config_Pin(Sercom_AT24_RX, Group_D, 8, SERCOM_PAD, PERIPHERAL_FUNCTION_C_SERCOM, No_Soft_Pull, Orientation_NonInverted);
    GPIO_Config_Pin(Sercom_AT24_TX, Group_D, 9, SERCOM_PAD, PERIPHERAL_FUNCTION_C_SERCOM, No_Soft_Pull, Orientation_NonInverted);
    GPIO_Config_Pin(Sercom_RS485_DI, Group_B, 16, SERCOM_PAD, PERIPHERAL_FUNCTION_C_SERCOM, No_Soft_Pull, Orientation_NonInverted);
    GPIO_Config_Pin(Sercom_RS485_DO, Group_B, 17, SERCOM_PAD, PERIPHERAL_FUNCTION_C_SERCOM, No_Soft_Pull, Orientation_NonInverted);
}

// *****************************************************************************

/* Function:
    void PORT_PinPeripheralFunctionConfig(PORT_PIN pin, PERIPHERAL_FUNCTION function)

  Summary:
    Configures the peripheral function on the selected port pin

  Description:
    This function configures the selected peripheral function on the given port pin.

  Remarks:
    Refer plib_port.h file for more information.
 */
void FRAMEWORK_PinPeripheralFunctionConfig(GPIO_PORT GPIO_PIN, FA_PERIPHERAL_FUNCTION function) {
    uint32_t periph_func = (uint32_t)function;
    PORT_GROUP group = ((PORT_GROUP)(PORT_BASE_ADDRESS + (GPIO_PIN.Port_Group * 0x80)));
    uint32_t pinmux_val;

    if (function == PERIPHERAL_FUNCTION_IO) {
        ((port_group_registers_t*)group)->PORT_PINCFG[GPIO_PIN.Port_Pin] &= ~PORT_PINCFG_PMUXEN_Msk;
    } else {
        pinmux_val = (uint32_t)((port_group_registers_t*)group)->PORT_PMUX[(GPIO_PIN.Port_Pin >> 1)];
        /* For odd pins */
        if (0U != (GPIO_PIN.Port_Pin & 0x01U)) {
            pinmux_val = (pinmux_val & ~0xF0U) | (periph_func << 4);
        } else {
            pinmux_val = (pinmux_val & ~0x0FU) | periph_func;
        }
        ((port_group_registers_t*)group)->PORT_PMUX[(GPIO_PIN.Port_Pin >> 1)] = (uint8_t)pinmux_val;

        /* Enable peripheral control of the pin */
        ((port_group_registers_t*)group)->PORT_PINCFG[GPIO_PIN.Port_Pin] |= (uint8_t)PORT_PINCFG_PMUXEN_Msk;
    }
}

/**
 * @brief Set a GPIO output pin to logic high.
 *
 * @param GPIO_PIN  The GPIO_PORT structure containing port group and pin number.
 */
void FRAMEWORK_GPIO_SET(GPIO_PORT GPIO_PIN) {
    (PORT_REGS->GROUP[GPIO_PIN.Port_Group].PORT_OUTSET = ((uint32_t)1U << GPIO_PIN.Port_Pin));
}

/**
 * @brief Set a GPIO output pin to logic low.
 *
 * @param GPIO_PIN  The GPIO_PORT structure containing port group and pin number.
 */
void FRAMEWORK_GPIO_Clear(GPIO_PORT GPIO_PIN) {
    (PORT_REGS->GROUP[GPIO_PIN.Port_Group].PORT_OUTCLR = ((uint32_t)1U << GPIO_PIN.Port_Pin));
}

/**
 * @brief Toggle the current logic state of a GPIO output pin.
 *
 * @param GPIO_PIN  The GPIO_PORT structure containing port group and pin number.
 */
void FRAMEWORK_GPIO_Toggle(GPIO_PORT GPIO_PIN) {
    (PORT_REGS->GROUP[GPIO_PIN.Port_Group].PORT_OUTTGL = ((uint32_t)1U << GPIO_PIN.Port_Pin));
}

/**
 * @brief Configure a GPIO pin as an output.
 *
 * @param GPIO_PIN  The GPIO_PORT structure containing port group and pin number.
 */
void FRAMEWORK_GPIO_OutputEnable(GPIO_PORT GPIO_PIN) {
    (PORT_REGS->GROUP[GPIO_PIN.Port_Group].PORT_DIRSET = ((uint32_t)1U << GPIO_PIN.Port_Pin));
}

/**
 * @brief Configure a GPIO pin as an input.
 *
 * @param GPIO_PIN  The GPIO_PORT structure containing port group and pin number.
 */
void FRAMEWORK_GPIO_InputEnable(GPIO_PORT GPIO_PIN) {
    (PORT_REGS->GROUP[GPIO_PIN.Port_Group].PORT_DIRCLR = ((uint32_t)1U << GPIO_PIN.Port_Pin));
}

/**
 * @brief Configure a input to have a pull up resistor.
 *
 * @param GPIO_PIN  The GPIO_PORT structure containing port group and pin number.
 */
void FRAMEWORK_GPIO_InputDontPull(GPIO_PORT GPIO_PIN) {
    PORT_REGS->GROUP[GPIO_PIN.Port_Group].PORT_OUTSET = ((uint32_t)1U << GPIO_PIN.Port_Pin);
    PORT_REGS->GROUP[GPIO_PIN.Port_Group].PORT_PINCFG[GPIO_PIN.Port_Pin] =
            PORT_PINCFG_INEN(1) |
            PORT_PINCFG_PULLEN(0) |
            PORT_PINCFG_PMUXEN(0);
}

/**
 * @brief Configure a input to have a pull up resistor.
 *
 * @param GPIO_PIN  The GPIO_PORT structure containing port group and pin number.
 */
void FRAMEWORK_GPIO_InputPullUp(GPIO_PORT GPIO_PIN) {
    PORT_REGS->GROUP[GPIO_PIN.Port_Group].PORT_OUTSET = ((uint32_t)1U << GPIO_PIN.Port_Pin);
    PORT_REGS->GROUP[GPIO_PIN.Port_Group].PORT_PINCFG[GPIO_PIN.Port_Pin] =
            PORT_PINCFG_INEN(1) |
            PORT_PINCFG_PULLEN(1) |
            PORT_PINCFG_PMUXEN(0);
}

/**
 * @brief Configure a input to have a pull up resistor.
 *
 * @param GPIO_PIN  The GPIO_PORT structure containing port group and pin number.
 */
void FRAMEWORK_GPIO_InputPullDown(GPIO_PORT GPIO_PIN) {
    PORT_REGS->GROUP[GPIO_PIN.Port_Group].PORT_OUTSET = ((uint32_t)0U << GPIO_PIN.Port_Pin);
    PORT_REGS->GROUP[GPIO_PIN.Port_Group].PORT_PINCFG[GPIO_PIN.Port_Pin] =
            PORT_PINCFG_INEN(1) |
            PORT_PINCFG_PULLEN(1) |
            PORT_PINCFG_PMUXEN(0);
}

/**
 * @brief Read the current logic level of a GPIO pin.
 *
 * @param GPIO_PIN  The GPIO_PORT structure containing port group and pin number.
 * @return true if the pin is high, false if low.
 */
bool FRAMEWORK_GPIO_Get(GPIO_PORT GPIO_PIN) {
    return (bool)(((PORT_REGS->GROUP[GPIO_PIN.Port_Group].PORT_IN >> GPIO_PIN.Port_Pin)) & 0x01U);
}

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
    SmartEEPROM8[u16_addr + 1] = ((uint8_t)((u16_data & 0xFF00) >> 8));
}

void FRAMEWORK_write_eeprom_32(uint16_t u16_addr, uint32_t u32_data) {
    while (NVMCTRL_SmartEEPROM_IsBusy());
    SmartEEPROM8[u16_addr] = ((uint8_t)(u32_data & 0x000000FF));
    SmartEEPROM8[u16_addr + 1] = ((uint8_t)((u32_data & 0x0000FF00) >> 8));
    SmartEEPROM8[u16_addr + 2] = ((uint8_t)((u32_data & 0x00FF0000) >> 16));
    SmartEEPROM8[u16_addr + 3] = ((uint8_t)((u32_data & 0xFF000000) >> 24));
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

void FRAMEWORK_USART_init_port_0() {
    // Do nothing
}

void FRAMEWORK_USART_init_port_1() {
    // Do nothing
}

void FRAMEWORK_USART_init_port_5() {
    // Do nothing
}

void FRAMEWORK_USART_re_set_port_0(void) {
    // Do nothing
}

void FRAMEWORK_USART_re_set_port_1(void) {
    // Do nothing
}

void FRAMEWORK_USART_re_set_port_5(void) {
    //GPIO_PC23_485_RE_SERCOM5_Set();
    GPIO_Set_High(GPIO_RE_SERCOM5);
}

void FRAMEWORK_USART_re_clear_port_0(void) {
    // Do nothing
}

void FRAMEWORK_USART_re_clear_port_1(void) {
    // Do nothing
}

void FRAMEWORK_USART_re_clear_port_5(void) {
    //    GPIO_PC23_485_RE_SERCOM5_Clear();
    GPIO_Set_Low(GPIO_RE_SERCOM5);
}

void FRAMEWORK_USART_de_set_port_0(void) {
    // Do nothing
}

void FRAMEWORK_USART_de_set_port_1(void) {
    // Do Nothing
}

void FRAMEWORK_USART_de_set_port_5(void) {
    //    GPIO_PC22_485_DE_SERCOM5_Set();
    GPIO_Set_High(GPIO_DE_SERCOM5);
}

void FRAMEWORK_USART_de_clear_port_0(void) {
    // Do Nothing
}

void FRAMEWORK_USART_de_clear_port_1(void) {
    // Do Nothing
}

void FRAMEWORK_USART_de_clear_port_5(void) {
    //    GPIO_PC22_485_DE_SERCOM5_Clear();
    GPIO_Set_Low(GPIO_DE_SERCOM5);
}

void FRAMEWORK_USART_break_set_port_0(void) {
    // Do Nothing
}

void FRAMEWORK_USART_break_set_port_1(void) {
    // Do nothing
}

void FRAMEWORK_USART_break_set_port_5(void) {
    //    GPIO_PC07_485_BREAK_Set();
    GPIO_Set_High(GPIO_485_Break);
}

void FRAMEWORK_USART_break_clear_port_0(void) {
    // Do nothing
}

void FRAMEWORK_USART_break_clear_port_1(void) {
    // Do nothing
}

void FRAMEWORK_USART_break_clear_port_5(void) {
    //    GPIO_PC07_485_BREAK_Clear();
    GPIO_Set_Low(GPIO_485_Break);
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

bool FRAMEWORK_USART_SerialSetup_port_0(USART_SERIAL_CONFIG* serialSetup, uint32_t clkFrequency) {
    // Convert from our USART_SERIAL_CONFIG to the SERCOM structure.
    USART_SERIAL_SETUP sercomConfig = ConvertSerialConfig(serialSetup);

    return SERCOM0_USART_SerialSetup(&sercomConfig, clkFrequency);
}

bool FRAMEWORK_USART_SerialSetup_port_1(USART_SERIAL_CONFIG* serialSetup, uint32_t clkFrequency) {
    // Convert from our USART_SERIAL_CONFIG to the SERCOM structure.
    USART_SERIAL_SETUP sercomConfig = ConvertSerialConfig(serialSetup);

    return SERCOM1_USART_SerialSetup(&sercomConfig, clkFrequency);
}

bool FRAMEWORK_USART_SerialSetup_port_5(USART_SERIAL_CONFIG* serialSetup, uint32_t clkFrequency) {
    // Convert from our USART_SERIAL_CONFIG to the SERCOM structure.
    USART_SERIAL_SETUP sercomConfig = ConvertSerialConfig(serialSetup);

    return SERCOM5_USART_SerialSetup(&sercomConfig, clkFrequency);
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

bool FRAMEWORK_USART_Write_port_0(void *buffer, const size_t size) {
    return SERCOM0_USART_Write(buffer, size);
}

bool FRAMEWORK_USART_Write_port_1(void *buffer, const size_t size) {
    return false;
}

bool FRAMEWORK_USART_Write_port_5(void *buffer, const size_t size) {
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

static USART_SERIAL_SETUP ConvertSerialConfig(USART_SERIAL_CONFIG* Usart_Config) {
    USART_SERIAL_SETUP sercomConfig;

    sercomConfig.baudRate = Usart_Config->baudRate;

    switch (Usart_Config->dataWidth) {
        case USART_CONFIG_DATA_5_BIT:
            sercomConfig.dataWidth = USART_DATA_5_BIT;
            break;
        case USART_CONFIG_DATA_6_BIT:
            sercomConfig.dataWidth = USART_DATA_6_BIT;
            break;
        case USART_CONFIG_DATA_7_BIT:
            sercomConfig.dataWidth = USART_DATA_7_BIT;
            break;
        case USART_CONFIG_DATA_8_BIT:
            sercomConfig.dataWidth = USART_DATA_8_BIT;
            break;
        case USART_CONFIG_DATA_9_BIT:
            sercomConfig.dataWidth = USART_DATA_9_BIT;
            break;
        case USART_CONFIG_DATA_INVALID:
            sercomConfig.dataWidth = USART_DATA_INVALID;
            break;
    }

    switch (Usart_Config->parity) {
        case USART_CONFIG_PARITY_EVEN:
            sercomConfig.parity = USART_PARITY_EVEN;
            break;
        case USART_CONFIG_PARITY_ODD:
            sercomConfig.parity = USART_PARITY_ODD;
            break;
        case USART_CONFIG_PARITY_NONE:
            sercomConfig.parity = USART_PARITY_NONE;
            break;
        case USART_CONFIG_PARITY_INVALID:
            sercomConfig.parity = USART_PARITY_INVALID;
            break;
    }

    switch (Usart_Config->stopBits) {
        case USART_CONFIG_STOP_1_BIT:
            sercomConfig.stopBits = USART_STOP_1_BIT;
            break;
        case USART_CONFIG_STOP_2_BIT:
            sercomConfig.stopBits = USART_STOP_2_BIT;
            break;
        case USART_CONFIG_STOP_INVALID:
            sercomConfig.stopBits = USART_STOP_INVALID;
            break;
    }

    return sercomConfig;
}

//GPIO
//--------

uint8_t FRAMEWORK_read_CCx(t_chan_ccx ccx) {
    uint8_t u8_ret_val = 1;
    switch (ccx) {
        case cc1:
            u8_ret_val = GPIO_Read_Debounced_State(GPIO_CC1);
            break;
        case cc2:
            u8_ret_val = GPIO_Read_Debounced_State(GPIO_CC2);
            break;
        case cc3:
            u8_ret_val = GPIO_Read_Debounced_State(GPIO_CC3);
            break;
#ifndef PLIB_ADC_COMMON_H
        case cc4:
            u8_ret_val = GPIO_Read_Debounced_State(GPIO_CC4);
            break;
#endif
        default:
            break;
    }
    return u8_ret_val;
}

/*
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
}*/

void FRAMEWORK_lamp_on(void) {
    GPIO_Set_High(GPIO_Lamp);
}

void FRAMEWORK_lamp_off(void) {
    GPIO_Set_Low(GPIO_Lamp);
}

_Bool FRAMEWORK_get_mac(uint8_t * dest) {
    return FA_AT24_get_mac(dest);
}

#ifdef PLIB_TCC_COMMON_H

/* This function is called after TCC period event */
void TCC_PeriodEventHandler(uint32_t status, uintptr_t context) {
    FA_PWM_period();
}

uint32_t FRAMEWORK_init_pwm(void) {
    uint32_t u32_retVal;
    /* Register callback function for period event */
    TCC0_PWMCallbackRegister(TCC_PeriodEventHandler, (uintptr_t)NULL);

    /* Read the period */
    u32_retVal = TCC0_PWM24bitPeriodGet();

    /* Start PWM*/
    TCC0_PWMStart();
    return u32_retVal;
}

_Bool FRAMEWORK_set_pwm_duty(t_channel_id pwm_chan, uint32_t u32_duty_val) {
    _Bool b_retval;
    switch (pwm_chan) {
        case chan_dc1:
            b_retval = TCC0_PWM24bitDutySet(TCC0_CHANNEL3, u32_duty_val);
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
#endif

#ifdef PLIB_ADC_COMMON_H

/*******************************************************************************
  Function:
    void FRAMEWORK_Enable_adc_block(void)

  Remarks:
      Enables the ADC block(s) aqssociated with the board
 */
void FRAMEWORK_Enable_adc_block(void) {
    ADC1_Enable();
}

/*******************************************************************************
  Function:
    void FRAMEWORK_Disable_adc(void)

  Remarks:
      Disables the ADC block(s) aqssociated with the board
 */
void FRAMEWORK_Disable_adc_block(void) {
    ADC1_Disable();
}

/*******************************************************************************
  Function:
    void FRAMEWORK_Enable_adc_channel(t_adc_id adc_id)

  Remarks:
      Enables the ADC block(s) aqssociated with the board
 */
void FRAMEWORK_Enable_adc_channel(t_adc_id adc_id) {
    switch (adc_id) {
        case adc_dc1:
            GPIO_Config_Pin(GPIO_CC4, Group_C, 3, ADC_PAD, PERIPHERAL_FUNCTION_B_ANALOGUE, false, Orientation_NonInverted);
            break;
        default:
            // Do nothing
            break;
    }
}

/*******************************************************************************
  Function:
    void FRAMEWORK_Disable_adc_channel(t_adc_id adc_id)

  Remarks:
      Disables the ADC block(s) aqssociated with the board
 */
void FRAMEWORK_Disable_adc_channel(t_adc_id adc_id) {
    switch (adc_id) {
        case adc_dc1:
            GPIO_Config_Pin(GPIO_CC4, Group_C, 3, GPIO_Input, PERIPHERAL_FUNCTION_B_ANALOGUE, false, Orientation_NonInverted);
            break;
        default:
            // Do nothing
            break;
    }
}

/*******************************************************************************
  Function:
    void FRAMEWORK_select_adc_channel(t_adc_id adc_id)

  Remarks:
        selects an ADC channel prior to reading/conversion
 */
void FRAMEWORK_select_adc_channel(t_adc_id adc_id) {
    switch (adc_id) {
        case adc_dc1:
            ADC1_ChannelSelect(ADC_INPUTCTRL_MUXPOS_AIN5, ADC_INPUTCTRL_MUXNEG_AIN5);
            break;
        default:
            // Do nothing
            break;
    }
}

/*******************************************************************************
  Function:
    void FRAMEWORK_start_adc_conversion(t_adc_id adc_id)

  Remarks:
      start analogue to digital conversion of current ADC channel
 */
void FRAMEWORK_start_adc_conversion(t_adc_id adc_id) {
    switch (adc_id) {
        case adc_dc1:
            ADC1_ConversionStart();
            break;
        default:
            break;
    }
}

/*******************************************************************************
  Function:
    void FRAMEWORK_get_conversion_status(t_adc_id adc_id)

  Remarks:
      returns true when current conversion is complete
 */
bool FRAMEWORK_get_adc_conversion_status(t_adc_id adc_id) {
    bool b_ret_val = false;
    switch (adc_id) {
        case adc_dc1:
            b_ret_val = ADC1_ConversionStatusGet();
            break;
        default:
            break;
    }
    return b_ret_val;
}

/*******************************************************************************
  Function:
    void FRAMEWORK_get_conversion_status(t_adc_id adc_id)

  Remarks:
      return digital result of the most recent ADC conversion
 */
uint16_t FRAMEWORK_get_last_adc_conversion_result(t_adc_id adc_id) {
    uint16_t u16_ret_val = 0;
    switch (adc_id) {
        case adc_dc1:
            u16_ret_val = ADC1_LastConversionResultGet();
            break;
        default:
            break;
    }
    return u16_ret_val;

}
#endif