/*******************************************************************************
 * Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
 *
 * Subject to your compliance with these terms, you may use Microchip software
 * and any derivatives exclusively with Microchip products. It is your
 * responsibility to comply with third party license terms applicable to your
 * use of third party software (including open source software) that may
 * accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
 * PARTICULAR PURPOSE.
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
 * ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/

/*******************************************************************************
  Future Automation Application Header File

  Company:
    Future Automation.

  File Name:
    fa_rs485.h

  Summary:
    This header file provides prototypes and definitions for the Future Automation
    RS485 interface.

  Description:
    This header file provides function prototypes and data type definitions for
    the Future Automation RS485 interface.  This provides all transport layer
    and command-set functionality
 *******************************************************************************/

#ifndef FA_RS485_H
#define FA_RS485_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#define RS485_TERMINATOR             0x7F00

#define RX_NO_ERROR                  0
#define RX_PARITY_ERROR              1
#define RX_FRAMING_ERROR             2
#define RX_OVERFLOW_ERROR            4

typedef enum usart_index {
    sercom0_usart = 0, sercom1_usart, sercom5_usart, /* add any new usart before this coment */ pic_usart, no_usart
} t_usart_id;
#define MAX_USART   pic_usart

/**
 * USART_CONFIG_PARITY
 *
 * The USART config enums for the parity.
 */
typedef enum {
    USART_CONFIG_PARITY_EVEN,
    USART_CONFIG_PARITY_ODD,
    USART_CONFIG_PARITY_NONE,
    USART_CONFIG_PARITY_INVALID
} USART_CONFIG_PARITY;

/**
 * USART_CONFIG_PARITY
 *
 * The USART config enums for the data bits.
 */
typedef enum {
    USART_CONFIG_DATA_5_BIT,
    USART_CONFIG_DATA_6_BIT,
    USART_CONFIG_DATA_7_BIT,
    USART_CONFIG_DATA_8_BIT,
    USART_CONFIG_DATA_9_BIT,
    USART_CONFIG_DATA_INVALID
} USART_CONFIG_DATA;

/**
 * USART_CONFIG_PARITY
 *
 * The USART config enums for the stop bits.
 */
typedef enum {
    USART_CONFIG_STOP_1_BIT,
    USART_CONFIG_STOP_2_BIT,
    USART_CONFIG_STOP_INVALID
} USART_CONFIG_STOP;

/**
 * USART_SERIAL_CONFIG contains the configuration options for a USART serial connection.
 * This struct is a copy of USART_SERIAL_SETUP from the SERCOM files, but we need our own
 * copy of this structure that can be used accross the SAME and PIC chips.
 *
 * So this structure should be used in all cases and not USART_SERIAL_SETUP.
 */
typedef struct USART_SERIAL_CONFIG {
    uint32_t baudRate;
    USART_CONFIG_PARITY parity;
    USART_CONFIG_DATA dataWidth;
    USART_CONFIG_STOP stopBits;
} USART_SERIAL_CONFIG;

void FA_USART_Initialize(t_usart_id this_usart_index, USART_SERIAL_CONFIG* USART_Config);
void FA_USART_Tasks(t_usart_id this_usart_index);
_Bool FA_USART_isTransmitting(t_usart_id this_usart);
void FA_USART_send(t_usart_id this_usart, uint8_t * output_str, uint16_t u16_len);
uint16_t FA_USART_read(t_usart_id this_usart, uint8_t * u8_packet_in);
//bool FA_USART_read_Struct (t_usart_id  this_usart, t_fa_packet_data * rx_packet);
void FA_USART_send_16(t_usart_id this_usart, uint16_t * output_str, uint16_t u16_len);
uint16_t FA_USART_read_16(t_usart_id this_usart, uint16_t * u8_packet_in);
void FA_USART_send_break(t_usart_id this_usart_index, uint16_t u16_time_us);
bool FA_USART_read_break(t_usart_id this_usart);
void FA_USART_start_break(t_usart_id this_usart);
void FA_USART_release_break(t_usart_id this_usart);
void FA_USART_stop_break(t_usart_id this_usart);
void FA_PIC_USART_data_notification(uint16_t u16_rx_byte, uint16_t usart_error);
bool FA_USART_byteReceived(void);

/**
 * FA_USART_Configure_Serial_Port configures a serial port during runtime.
 * The data that can be configured is:
 * - Baud rate.
 * - Parity.
 * - Data bits.
 * - Stop bits.
 *
 * @param usart_id The USART id of the port to configure. (E.G sercom0_usart, etc.)
 * @param serial_setup The USART_SERIAL_SETUP struct that contains the data to be configured.
 * @return True if the port was configured successfully, false for failure.
 */
bool FA_USART_Configure_Serial_Port(t_usart_id usart_id, USART_SERIAL_CONFIG* serial_setup);

/**
 * Indicates whether a byte has been transmitted or received over USART.
 *
 * @param usart_id The USART id of the interface. I.E "FA_Protocol_485_Port", "FA_Protocol_232_Port" etc.
 * @return Boolean representing if the specified USART has transmitted or received a byte.
 *
 */
bool FA_USART_ByteTransmittedOrReceived(t_usart_id usart_id);
#endif /* FA_RS485_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

