/*
 * File:   framework_common.h
 * Author: graeme.capes
 *
 * Created on February 28, 2025, 11:17 AM
 */

#ifndef FRAMEWORK_COMMON_H
#define	FRAMEWORK_COMMON_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "fa_usart.h"
#include "fa_eeprom.h"

#define RJ45_PIN_8_INDEX     0
#define RJ45_PIN_7_INDEX     1
#define RJ45_PIN_6_INDEX     2
#define RJ45_PIN_5_INDEX     3
#define RJ45_PIN_4_INDEX     4

    typedef enum {
        null_cc, cc1, cc2, cc3, cc4, cc5, cc6, cc7, cc8, cc9, cc10, cc11, cc12, cc13, cc14, cc15, cc16, cc17, cc18, cc19, cc20, cc21, cc22, cc23, cc24
    } t_chan_ccx;

    typedef enum {
        adc_dc1, adc_dc2, adc_dc3, adc_dc4, adc_null
    } t_adc_id;

    typedef enum {
        /* All DC channels channels must be listed first */
        chan_dc1, chan_dc2, chan_dc3, chan_dc4,
        /* All AC channels channels must be listed after DC channels */
        chan_ac1, chan_ac2, chan_ac3,

        chan_null
    } t_channel_id;


    void FRAMEWORK_MAIN_init_processor(void);

    void FRAMEWORK_write_eeprom_8(uint16_t u16_addr, uint8_t u8_data);
    void FRAMEWORK_write_eeprom_16(uint16_t u16_addr, uint16_t u16_data);
    void FRAMEWORK_write_eeprom_32(uint16_t u16_addr, uint32_t u32_data);
    void FRAMEWORK_write_eeprom_block(uint16_t u16_addr, uint8_t * u8_data, uint16_t u16_length);
    void FRAMEWORK_fill_eeprom_block(uint16_t u16_addr, uint8_t u8_data, uint16_t u16_length);
    uint8_t FRAMEWORK_read_eeprom_8(uint16_t u16_addr);
    uint16_t FRAMEWORK_read_eeprom_16(uint16_t u16_addr);
    uint32_t FRAMEWORK_read_eeprom_32(uint16_t u16_addr);
    void FRAMEWORK_read_eeprom_block(uint16_t u16_addr, uint8_t * u8_data, uint16_t u16_length);

    void FRAMEWORK_TIMER_init(void); // return value is timer overflow value
    float FRAMEWORK_TIMER_get_resolution(void);
    void FRAMEWORK_TIMER_start(void);
    void FRAMEWORK_TIMER_stop(void);
    bool FRAMEWORK_TIMER_running(void);

    void FRAMEWORK_USART_init_port_0();
    void FRAMEWORK_USART_init_port_1();
    void FRAMEWORK_USART_init_port_5();
    void FRAMEWORK_USART_re_set_port_0(void);
    void FRAMEWORK_USART_re_clear_port_0(void);
    void FRAMEWORK_USART_de_set_port_0(void);
    void FRAMEWORK_USART_de_clear_port_0(void);
    void FRAMEWORK_USART_re_set_port_1(void);
    void FRAMEWORK_USART_re_clear_port_1(void);
    void FRAMEWORK_USART_de_set_port_1(void);
    void FRAMEWORK_USART_de_clear_port_1(void);
    void FRAMEWORK_USART_re_set_port_5(void);
    void FRAMEWORK_USART_re_clear_port_5(void);
    void FRAMEWORK_USART_de_set_port_5(void);
    void FRAMEWORK_USART_de_clear_port_5(void);
    void FRAMEWORK_USART_break_set_port_0(void);
    void FRAMEWORK_USART_break_set_port_1(void);
    void FRAMEWORK_USART_break_set_port_5(void);
    void FRAMEWORK_USART_break_clear_port_0(void);
    void FRAMEWORK_USART_break_clear_port_1(void);
    void FRAMEWORK_USART_break_clear_port_5(void);
    uint16_t FRAMEWORK_USART_ErrorGet_port_0(void);
    uint16_t FRAMEWORK_USART_ErrorGet_port_1(void);
    uint16_t FRAMEWORK_USART_ErrorGet_port_5(void);
    void FRAMEWORK_USART_TransmitterEnable_port_0(void);
    void FRAMEWORK_USART_TransmitterEnable_port_1(void);
    void FRAMEWORK_USART_TransmitterEnable_port_5(void);
    void FRAMEWORK_USART_TransmitterDisable_port_0(void);
    void FRAMEWORK_USART_TransmitterDisable_port_1(void);
    void FRAMEWORK_USART_TransmitterDisable_port_5(void);
    bool FRAMEWORK_USART_WriteIsBusy_port_0(void);
    bool FRAMEWORK_USART_WriteIsBusy_port_1(void);
    bool FRAMEWORK_USART_WriteIsBusy_port_5(void);
    bool FRAMEWORK_USART_Write_port_0(void *buffer, const size_t size);
    bool FRAMEWORK_USART_Write_port_1(void *buffer, const size_t size);
    bool FRAMEWORK_USART_Write_port_5(void *buffer, const size_t size);
    void FRAMEWORK_USART_ReceiverEnable_port_0(void);
    void FRAMEWORK_USART_ReceiverEnable_port_1(void);
    void FRAMEWORK_USART_ReceiverEnable_port_5(void);
    void FRAMEWORK_USART_ReceiverDisable_port_0(void);
    void FRAMEWORK_USART_ReceiverDisable_port_1(void);
    void FRAMEWORK_USART_ReceiverDisable_port_5(void);
    bool FRAMEWORK_USART_ReadAbort_port_0(void);
    bool FRAMEWORK_USART_ReadAbort_port_1(void);
    bool FRAMEWORK_USART_ReadAbort_port_5(void);
    bool FRAMEWORK_USART_Read_port_0(void *buffer, const size_t size);
    bool FRAMEWORK_USART_Read_port_1(void *buffer, const size_t size);
    bool FRAMEWORK_USART_Read_port_5(void *buffer, const size_t size);
    void FRAMEWORK_USART_RegisterReadCallback_port_0(void * callback_func, uintptr_t context);
    void FRAMEWORK_USART_RegisterReadCallback_port_1(void * callback_func, uintptr_t context);
    void FRAMEWORK_USART_RegisterReadCallback_port_5(void * callback_func, uintptr_t context);
    bool FRAMEWORK_USART_ByteTransmittedOrReceived_port_0(void);
    bool FRAMEWORK_USART_ByteTransmittedOrReceived_port_1(void);
    bool FRAMEWORK_USART_ByteTransmittedOrReceived_port_5(void);
    uint32_t FRAMEWORK_get_rx_timeout_port_0(void);
    uint32_t FRAMEWORK_get_rx_timeout_port_1(void);
    uint32_t FRAMEWORK_get_rx_timeout_port_5(void);
    uint8_t FRAMEWORK_read_CCx(t_chan_ccx ccx);
    uint8_t FRAMEWORK_read_RJ45(uint8_t u8_pin_index);
    void FRAMEWORK_lamp_on(void);
    void FRAMEWORK_lamp_off(void);
    _Bool FRAMEWORK_get_mac(uint8_t * dest);
    uint32_t FRAMEWORK_init_pwm(void);
    _Bool FRAMEWORK_set_pwm_duty(t_channel_id pwm_chan, uint32_t u32_duty_val);
    void FRAMEWORK_activate_tcc_pwm(t_channel_id pwm_chan);
    void FRAMEWORK_deactivate_tcc_pwm(t_channel_id pwm_chan);
    void FRAMEWORK_Enable_adc_block(void);
    void FRAMEWORK_Disable_adc_block(void);
    void FRAMEWORK_Enable_adc_channel(t_adc_id adc_id);
    void FRAMEWORK_Disable_adc_channel(t_adc_id adc_id);
    void FRAMEWORK_select_adc_channel(t_adc_id adc_id);
    void FRAMEWORK_start_adc_conversion(t_adc_id adc_id);
    bool FRAMEWORK_get_adc_conversion_status(t_adc_id adc_id);
    uint16_t FRAMEWORK_get_last_adc_conversion_result(t_adc_id adc_id);



#ifdef	__cplusplus
}
#endif

#endif	/* FRAMEWORK_COMMON_H */

