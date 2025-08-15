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
  Future Automation Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    fa_usart.c

  Summary:
    This file contains the source code for the Future Automation USART interface.

  Description:
    This file contains the source code for the Future Automation USART interface.
    This provides all transport layer and command-set functionality

 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "fa_timer.h"
#include "framework.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define USART_RX_BUFFER_SIZE          1


// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

typedef enum {
    tx_mode, rx_mode, tx_break_mode, tx_manual_break_mode
} t_half_duplex_mode;

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

typedef struct {
    void (*FUNC_init)();
    void (*FUNC_RE_Set)(void);
    void (*FUNC_DE_Set)(void);
    void (*FUNC_RE_Clear)(void);
    void (*FUNC_DE_Clear)(void);
    void (*FUNC_Break_Set)(void);
    void (*FUNC_Break_Clear)(void);
    uint16_t(*FUNC_ErrorGet)(void);
    bool(*FUNC_SerialSetup)(USART_SERIAL_CONFIG* serialSetup, uint32_t clkFrequency);
    void (*FUNC_TransmitterEnable)(void);
    void (*FUNC_TransmitterDisable)(void);
    bool(*FUNC_WriteIsBusy)(void);
    bool(*FUNC_ByteTransmittedOrReceived)(void);
    bool(*FUNC_Write)(void *buffer, const size_t size);
    void (*FUNC_ReceiverEnable)(void);
    void (*FUNC_ReceiverDisable)(void);
    bool(*FUNC_ReadAbort)(void);
    bool(*FUNC_Read)(void *, const size_t);
    void (*FUNC_ReadCallbackRegister)(void * callback, uintptr_t context);

    t_half_duplex_mode e_half_duplex_mode;
    USART_CONFIG_DATA e_data_framing;
    uint16_t u16_txbuf_rd_ptr;
    uint16_t u16_txbuf_wr_ptr;
    bool bReceiving;
    bool b_break_received;
    bool b_rx_pkt_waiting;
    volatile uint16_t u16_rx_pkt_wr_ptr;
    uint16_t u16_tx_buffer[FRAMEWORK_MAX_TX_BUFFER_SIZE];
    uint8_t u8_tx_packet[FRAMEWORK_MAX_PACKET_SIZE];
    uint16_t u16_tx_packet[FRAMEWORK_MAX_PACKET_SIZE];
    volatile uint16_t u16_rx_packet[FRAMEWORK_MAX_PACKET_SIZE];
    uint8_t u8_rx_buffer;
    uint16_t u16_rx_buffer;
    uint32_t u32_break_timer;
    uint32_t u32_rx_timer;
    uint32_t u32_rx_timeout;
} t_usart_profile;

uint8_t next_usart_index = 0;
uint8_t profile_index[MAX_USART];
t_usart_profile usart_profile[FRAMEWORK_NO_OF_USARTS];

bool bByteReceived = false;

// *****************************************************************************
// *****************************************************************************
// Section: Forward references
// *****************************************************************************
// *****************************************************************************




// *****************************************************************************
// *****************************************************************************
// Section: USART Initialisation and State Machine Functions
// *****************************************************************************
// *****************************************************************************

static void flush_rx_buffer(t_usart_profile * usart_profile_ptr) {

    // Disable rx and associated interrupt while we clear the buffer ready for reception of the next byte
    usart_profile_ptr->FUNC_ReadAbort();

    // Clear out single byte rx buffer then re-enable rx and associated interrupt
    if (usart_profile_ptr->e_data_framing == USART_CONFIG_DATA_9_BIT) {
        // 8 bit buffer (for 8 bit data width)
        usart_profile_ptr->u16_rx_buffer = 0;
        usart_profile_ptr->FUNC_Read(&(usart_profile_ptr->u16_rx_buffer), USART_RX_BUFFER_SIZE);
    } else {
        // 16 bit buffer (for 8 bit data width)
        usart_profile_ptr->u8_rx_buffer = 0;
        usart_profile_ptr->FUNC_Read(&(usart_profile_ptr->u8_rx_buffer), USART_RX_BUFFER_SIZE);
    }
}

void enable_rx(t_usart_profile * usart_profile_ptr) {

    usart_profile_ptr->FUNC_TransmitterDisable();
    usart_profile_ptr->FUNC_RE_Clear();
    usart_profile_ptr->FUNC_DE_Clear();
    usart_profile_ptr->e_half_duplex_mode = rx_mode;
    usart_profile_ptr->b_break_received = false;
    flush_rx_buffer(usart_profile_ptr);
    usart_profile_ptr->FUNC_ReceiverEnable();
}

void enable_tx(t_usart_profile * usart_profile_ptr, t_half_duplex_mode eMode) {
    usart_profile_ptr->FUNC_ReceiverDisable();
    usart_profile_ptr->FUNC_RE_Set();
    usart_profile_ptr->FUNC_DE_Set();
    usart_profile_ptr->e_half_duplex_mode = eMode;
    usart_profile_ptr->b_break_received = false;
    usart_profile_ptr->FUNC_TransmitterEnable();
}

bool FA_USART_byteReceived(void) {
    bool bRetVal;

    bRetVal = bByteReceived;
    bByteReceived = false;
    return bRetVal;
}

//
// RX INTERUPT SERVICE ROUTINE
//

void FA_SAM_USART_data_notification(uintptr_t context) {
    t_usart_profile * usart_profile_ptr;
    uint16_t u16_rx_byte;

    // point to the usart profile belonging to this SERCOM
    usart_profile_ptr = &(usart_profile[context]);

    // Reserve the USART for RX
    usart_profile_ptr->bReceiving = true;


    // Restart RX Timer (expiry of this timer indicates end of packet))
    FA_TIMER_Start(usart_profile_ptr->u32_rx_timeout, &usart_profile_ptr->u32_rx_timer, Time_uS);

    // extract the received byte from the single byte SERCOM RX buffer
    u16_rx_byte = (uint16_t)((usart_profile_ptr->e_data_framing == USART_CONFIG_DATA_9_BIT) ? usart_profile_ptr->u16_rx_buffer : usart_profile_ptr->u8_rx_buffer);

    // default value for break received token is false
    usart_profile_ptr->b_break_received = false;
    switch (usart_profile_ptr->FUNC_ErrorGet()) {
        case RX_NO_ERROR:
            // add this byte to the received packet cyclic buffer
            usart_profile_ptr->u16_rx_packet[usart_profile_ptr->u16_rx_pkt_wr_ptr++] = u16_rx_byte;
            bByteReceived = true;
            break;

        case RX_FRAMING_ERROR:
            // if there has been a framing error with a value of 0, interpret this as a break character
            if (u16_rx_byte == 0) {
                // set break received token
                usart_profile_ptr->b_break_received = true;
            }
            // otherwise do nothing and reject the byte
            break;

        case RX_PARITY_ERROR:
        case RX_OVERFLOW_ERROR:
            // Do nothing, reject byte
            break;
    }
    // flush SERCOM rx buffer ready for next byte
    flush_rx_buffer(usart_profile_ptr);
}

void FA_PIC_USART_data_notification(uint16_t u16_rx_byte, uint16_t usart_error) {
    t_usart_profile * usart_profile_ptr;

    // point to the usart profile belonging to this SERCOM
    usart_profile_ptr = &(usart_profile[profile_index[pic_usart]]);

    // Reserve the USART for RX
    usart_profile_ptr->bReceiving = true;


    // Restart RX Timer (expiry of this timer indicates end of packet))
    FA_TIMER_Start(usart_profile_ptr->u32_rx_timeout, &usart_profile_ptr->u32_rx_timer, Time_uS);
    switch (usart_error) {
        case RX_NO_ERROR:
            // add this byte to the received packet cyclic buffer
            usart_profile_ptr->u16_rx_packet[usart_profile_ptr->u16_rx_pkt_wr_ptr++] = u16_rx_byte;
            break;

        case RX_FRAMING_ERROR:
            // if there has been a framing error with a value of 0, interpret this as a break character
            if (u16_rx_byte == 0) {
                // set break received token
                usart_profile_ptr->b_break_received = true;
            }
            // otherwise do nothing and reject the byte
            break;

        case RX_PARITY_ERROR:
        case RX_OVERFLOW_ERROR:
            // Do nothing, reject byte
            break;
    }
}



//
// Called after RX timer has expired (time elapsed since last RX interrupt, used to detect the end of a packet))
//

void data_idle(t_usart_profile * usart_profile_ptr) {
    if (usart_profile_ptr->e_half_duplex_mode == tx_mode) {
        enable_rx(usart_profile_ptr);
    } else {
        // Indicate a full packet has been received
        usart_profile_ptr->b_rx_pkt_waiting = true;
        // Allow TX (locked out while bReceiving is true)
        usart_profile_ptr->bReceiving = false;
    }
}

/*******************************************************************************
  Function:
    void FA_USART_Initialise ( void )

  Remarks:
    See prototype in fa_usart.h.
 */
void FA_USART_Initialize(t_usart_id this_usart, USART_SERIAL_CONFIG* usart_config) {
    t_usart_profile * usart_profile_ptr;

    uint8_t u8_this_index;

    if (this_usart < no_usart) {
        u8_this_index = next_usart_index++;
        usart_profile_ptr = &usart_profile[u8_this_index];
        profile_index[this_usart] = u8_this_index;

        // Allocate dynamic function pointers and rx timeout for each usart
        switch (this_usart) {
            case sercom1_usart:
                usart_profile_ptr->FUNC_init = &FRAMEWORK_USART_init_port_1;
                usart_profile_ptr->FUNC_RE_Set = &FRAMEWORK_USART_re_set_port_1;
                usart_profile_ptr->FUNC_RE_Clear = &FRAMEWORK_USART_re_clear_port_1;
                usart_profile_ptr->FUNC_DE_Set = &FRAMEWORK_USART_de_set_port_1;
                usart_profile_ptr->FUNC_DE_Clear = &FRAMEWORK_USART_de_clear_port_1;
                usart_profile_ptr->FUNC_Break_Set = &FRAMEWORK_USART_break_set_port_1;
                usart_profile_ptr->FUNC_Break_Clear = &FRAMEWORK_USART_break_clear_port_1;
                usart_profile_ptr->FUNC_ErrorGet = &FRAMEWORK_USART_ErrorGet_port_1;
                usart_profile_ptr->FUNC_SerialSetup = &FRAMEWORK_USART_SerialSetup_port_1;
                usart_profile_ptr->FUNC_TransmitterEnable = &FRAMEWORK_USART_TransmitterEnable_port_1;
                usart_profile_ptr->FUNC_TransmitterDisable = &FRAMEWORK_USART_TransmitterDisable_port_1;
                usart_profile_ptr->FUNC_WriteIsBusy = &FRAMEWORK_USART_WriteIsBusy_port_1;
                usart_profile_ptr->FUNC_ByteTransmittedOrReceived = &FRAMEWORK_USART_ByteTransmittedOrReceived_port_1;
                usart_profile_ptr->FUNC_Write = &FRAMEWORK_USART_Write_port_1;
                usart_profile_ptr->FUNC_ReceiverEnable = &FRAMEWORK_USART_ReceiverEnable_port_1;
                usart_profile_ptr->FUNC_ReceiverDisable = &FRAMEWORK_USART_ReceiverDisable_port_1;
                usart_profile_ptr->FUNC_ReadAbort = &FRAMEWORK_USART_ReadAbort_port_1;
                usart_profile_ptr->FUNC_Read = &FRAMEWORK_USART_Read_port_1;
                FRAMEWORK_USART_RegisterReadCallback_port_1(FA_SAM_USART_data_notification, u8_this_index);
                usart_profile_ptr->u32_rx_timeout = FRAMEWORK_get_rx_timeout_port_1();
                usart_profile_ptr->u32_break_timer = 0;
                usart_profile_ptr->u32_rx_timer = 0;
                break;

            case sercom5_usart:
                usart_profile_ptr->FUNC_init = &FRAMEWORK_USART_init_port_5;
                usart_profile_ptr->FUNC_RE_Set = &FRAMEWORK_USART_re_set_port_5;
                usart_profile_ptr->FUNC_RE_Clear = &FRAMEWORK_USART_re_clear_port_5;
                usart_profile_ptr->FUNC_DE_Set = &FRAMEWORK_USART_de_set_port_5;
                usart_profile_ptr->FUNC_DE_Clear = &FRAMEWORK_USART_de_clear_port_5;
                usart_profile_ptr->FUNC_Break_Set = &FRAMEWORK_USART_break_set_port_5;
                usart_profile_ptr->FUNC_Break_Clear = &FRAMEWORK_USART_break_clear_port_5;
                usart_profile_ptr->FUNC_ErrorGet = &FRAMEWORK_USART_ErrorGet_port_5;
                usart_profile_ptr->FUNC_SerialSetup = &FRAMEWORK_USART_SerialSetup_port_5;
                usart_profile_ptr->FUNC_TransmitterEnable = &FRAMEWORK_USART_TransmitterEnable_port_5;
                usart_profile_ptr->FUNC_TransmitterDisable = &FRAMEWORK_USART_TransmitterDisable_port_5;
                usart_profile_ptr->FUNC_WriteIsBusy = &FRAMEWORK_USART_WriteIsBusy_port_5;
                usart_profile_ptr->FUNC_ByteTransmittedOrReceived = &FRAMEWORK_USART_ByteTransmittedOrReceived_port_5;
                usart_profile_ptr->FUNC_Write = &FRAMEWORK_USART_Write_port_5;
                usart_profile_ptr->FUNC_ReceiverEnable = &FRAMEWORK_USART_ReceiverEnable_port_5;
                usart_profile_ptr->FUNC_ReceiverDisable = &FRAMEWORK_USART_ReceiverDisable_port_5;
                usart_profile_ptr->FUNC_ReadAbort = &FRAMEWORK_USART_ReadAbort_port_5;
                usart_profile_ptr->FUNC_Read = &FRAMEWORK_USART_Read_port_5;
                FRAMEWORK_USART_RegisterReadCallback_port_5(FA_SAM_USART_data_notification, u8_this_index);
                usart_profile_ptr->u32_rx_timeout = FRAMEWORK_get_rx_timeout_port_5();
                usart_profile_ptr->u32_break_timer = 0;
                usart_profile_ptr->u32_rx_timer = 0;
                break;

            case sercom0_usart:
            case pic_usart:
                usart_profile_ptr->FUNC_init = &FRAMEWORK_USART_init_port_0;
                usart_profile_ptr->FUNC_RE_Set = &FRAMEWORK_USART_re_set_port_0;
                usart_profile_ptr->FUNC_RE_Clear = &FRAMEWORK_USART_re_clear_port_0;
                usart_profile_ptr->FUNC_DE_Set = &FRAMEWORK_USART_de_set_port_0;
                usart_profile_ptr->FUNC_DE_Clear = &FRAMEWORK_USART_de_clear_port_0;
                usart_profile_ptr->FUNC_Break_Set = &FRAMEWORK_USART_break_set_port_0;
                usart_profile_ptr->FUNC_Break_Clear = &FRAMEWORK_USART_break_clear_port_0;
                usart_profile_ptr->FUNC_ErrorGet = &FRAMEWORK_USART_ErrorGet_port_0;
                usart_profile_ptr->FUNC_SerialSetup = &FRAMEWORK_USART_SerialSetup_port_0;
                usart_profile_ptr->FUNC_TransmitterEnable = &FRAMEWORK_USART_TransmitterEnable_port_0;
                usart_profile_ptr->FUNC_TransmitterDisable = &FRAMEWORK_USART_TransmitterDisable_port_0;
                usart_profile_ptr->FUNC_WriteIsBusy = &FRAMEWORK_USART_WriteIsBusy_port_0;
                usart_profile_ptr->FUNC_ByteTransmittedOrReceived = &FRAMEWORK_USART_ByteTransmittedOrReceived_port_0;
                usart_profile_ptr->FUNC_Write = &FRAMEWORK_USART_Write_port_0;
                usart_profile_ptr->FUNC_ReceiverEnable = &FRAMEWORK_USART_ReceiverEnable_port_0;
                usart_profile_ptr->FUNC_ReceiverDisable = &FRAMEWORK_USART_ReceiverDisable_port_0;
                usart_profile_ptr->FUNC_ReadAbort = &FRAMEWORK_USART_ReadAbort_port_0;
                usart_profile_ptr->FUNC_Read = &FRAMEWORK_USART_Read_port_0;
                if (this_usart == pic_usart) {
                    FRAMEWORK_USART_RegisterReadCallback_port_0(FA_PIC_USART_data_notification, u8_this_index);
                } else {
                    FRAMEWORK_USART_RegisterReadCallback_port_0(FA_SAM_USART_data_notification, u8_this_index);
                }
                usart_profile_ptr->u32_rx_timeout = FRAMEWORK_get_rx_timeout_port_0();
                usart_profile_ptr->u32_break_timer = 0;
                usart_profile_ptr->u32_rx_timer = 0;
                break;
            case no_usart:
                // do nothing
                break;
        }

        // sercom5_usart
        //Initialise tx buffer to indicate no data
        usart_profile_ptr->u16_txbuf_rd_ptr = 0;
        usart_profile_ptr->u16_txbuf_wr_ptr = 0;

        // Initialise tx buffer to indicate no data
        usart_profile_ptr->u16_rx_pkt_wr_ptr = 0;
        usart_profile_ptr->b_rx_pkt_waiting = false;


        // indicate associated data width (8 bit for SDN, 9 bit for HQS))
        usart_profile_ptr->e_data_framing = usart_config->dataWidth;
        usart_profile_ptr->FUNC_init();
        // Pass 0 as the clock frequency so it takes the value calculated by harmony.
        usart_profile_ptr->FUNC_SerialSetup(usart_config, 0);
        enable_rx(usart_profile_ptr);
    }
}


//
//  Public function - identifies if bus is currently transmitting (resting state is receiving)
//

_Bool FA_USART_isTransmitting(t_usart_id this_usart) {
    bool bRetVal = false;

    if (this_usart < no_usart) {
        bRetVal = (usart_profile[profile_index[this_usart]].e_half_duplex_mode == tx_mode);
    }
    return bRetVal;
}



//
//  Public function - identifies if a break has been received (implemented as a latch which is reset upon receipt of next byte))
//

bool FA_USART_read_break(t_usart_id this_usart) {
    bool bRetVal = false;
    t_usart_profile * usart_profile_ptr;

    if (this_usart < no_usart) {
        usart_profile_ptr = &usart_profile[profile_index[this_usart]];
        bRetVal = usart_profile_ptr->b_break_received;
        usart_profile_ptr->b_break_received = false;
    }
    return bRetVal;
}


//
//  Public function - generate a break
//

void FA_USART_send_break(t_usart_id this_usart, uint16_t u16_time_us) {
    t_usart_profile * usart_profile_ptr;

    if (this_usart < no_usart) {
        usart_profile_ptr = &usart_profile[profile_index[this_usart]];
        enable_tx(usart_profile_ptr, tx_break_mode);
        usart_profile_ptr->FUNC_Break_Set();
        FA_TIMER_Start(u16_time_us, &usart_profile_ptr->u32_break_timer, Time_uS);
    }
}

void FA_USART_start_break(t_usart_id this_usart) {
    t_usart_profile * usart_profile_ptr;

    if (this_usart < no_usart) {
        usart_profile_ptr = &usart_profile[profile_index[this_usart]];
        enable_tx(usart_profile_ptr, tx_manual_break_mode);
        usart_profile_ptr->FUNC_Break_Set();
    }
}

void FA_USART_release_break(t_usart_id this_usart) {
    t_usart_profile * usart_profile_ptr;

    if (this_usart < no_usart) {
        usart_profile_ptr = &usart_profile[profile_index[this_usart]];
        usart_profile_ptr->FUNC_Break_Set();
        enable_tx(usart_profile_ptr, tx_mode);
    }
}

void FA_USART_stop_break(t_usart_id this_usart) {
    t_usart_profile * usart_profile_ptr;

    if (this_usart < no_usart) {
        usart_profile_ptr = &usart_profile[profile_index[this_usart]];
        usart_profile_ptr->FUNC_Break_Set();
        enable_rx(usart_profile_ptr);
    }
}

//
//  Public function - pre-load a message ready for transmission
//

void FA_USART_send(t_usart_id this_usart, uint8_t * u8_packet_out, uint16_t u16_len) {
    t_usart_profile * usart_profile_ptr;
    uint16_t u16_index;

    if (this_usart < no_usart) {
        usart_profile_ptr = &usart_profile[profile_index[this_usart]];
        // Copy all bytes from u16_packet_out into the tx buffer
        for (u16_index = 0; u16_index < (u16_len - 1); u16_index++) {
            usart_profile_ptr->u16_tx_buffer[usart_profile_ptr->u16_txbuf_wr_ptr] = ((uint16_t)u8_packet_out[u16_index]);
            usart_profile_ptr->u16_txbuf_wr_ptr = ((usart_profile_ptr->u16_txbuf_wr_ptr + 1) % FRAMEWORK_MAX_TX_BUFFER_SIZE);
        };
        usart_profile_ptr->u16_tx_buffer[usart_profile_ptr->u16_txbuf_wr_ptr] = ((uint16_t)u8_packet_out[u16_index]) | 0x8000;
        usart_profile_ptr->u16_txbuf_wr_ptr = ((usart_profile_ptr->u16_txbuf_wr_ptr + 1) % FRAMEWORK_MAX_TX_BUFFER_SIZE);
        // u8_txbuf_wr_ptr now points to the next cyclic buffer byte after
        // the newly queued message
    }
}


//
//  Public function - copy the last received packet into the u16 array pointed to by u16_packet_in
//  Return value is the packet length
//

uint16_t FA_USART_read(t_usart_id this_usart, uint8_t * u8_packet_in) {
    t_usart_profile * usart_profile_ptr;
    uint16_t copy_index = 0;
    //uint8_t yassss;

    if (this_usart < no_usart) {
        usart_profile_ptr = &usart_profile[profile_index[this_usart]];
        // if a full packet has been received
        if (usart_profile_ptr->b_rx_pkt_waiting) {

            //copy the next packet from the local rx packet buffer into the memory area pointed to by u16_packet_in
            while (copy_index < usart_profile_ptr->u16_rx_pkt_wr_ptr) {
                //yassss = ((uint8_t)usart_profile_ptr->u16_rx_packet[copy_index]);
                u8_packet_in[copy_index] = ((uint8_t)usart_profile_ptr->u16_rx_packet[copy_index]);
                copy_index++;
            }

            // clear out the local rx packet buffer
            usart_profile_ptr->b_rx_pkt_waiting = false;
            usart_profile_ptr->u16_rx_pkt_wr_ptr = 0;
        }
    }
    return copy_index;
}

/*
//  Public function - copy the last received packet into the u16 array pointed to by u16_packet_in
//  Return value is the packet length
//
bool FA_USART_read_Struct (t_usart_id  this_usart, t_fa_packet_data  * rx_packet)
{
    t_usart_profile * usart_profile_ptr;
    uint16_t copy_index = 0;
    bool bRetVal = false;
    //uint8_t yassss;

    if (this_usart < no_usart) {
        usart_profile_ptr = &usart_profile[profile_index[this_usart]];
        // if a full packet has been received
        if (usart_profile_ptr->b_rx_pkt_waiting) {

            //copy the next packet from the local rx packet buffer into the memory area pointed to by u16_packet_in
            while (copy_index < usart_profile_ptr->u16_rx_pkt_wr_ptr) {
                //yassss = ((uint8_t)usart_profile_ptr->u16_rx_packet[copy_index]);
                rx_packet->data[copy_index] = ((uint8_t)usart_profile_ptr->u16_rx_packet[copy_index]);
                 copy_index++;
            }

            // clear out the local rx packet buffer
            usart_profile_ptr->b_rx_pkt_waiting = false;
            usart_profile_ptr->u16_rx_pkt_wr_ptr = 0;
            rx_packet->length = copy_index;
            bRetVal = true;
        }
    }
    return bRetVal;
}*/


//
//  Public function - pre-load a message ready for transmission
//

void FA_USART_send_16(t_usart_id this_usart, uint16_t * u16_packet_out, uint16_t u16_len) {
    t_usart_profile * usart_profile_ptr;
    uint16_t u16_index;

    if (this_usart < no_usart) {
        usart_profile_ptr = &usart_profile[profile_index[this_usart]];
        // Copy all bytes from u16_packet_out into the tx buffer
        for (u16_index = 0; u16_index < (u16_len - 1); u16_index++) {
            usart_profile_ptr->u16_tx_buffer[usart_profile_ptr->u16_txbuf_wr_ptr] = u16_packet_out[u16_index];
            usart_profile_ptr->u16_txbuf_wr_ptr = ((usart_profile_ptr->u16_txbuf_wr_ptr + 1) % FRAMEWORK_MAX_TX_BUFFER_SIZE);
        };
        usart_profile_ptr->u16_tx_buffer[usart_profile_ptr->u16_txbuf_wr_ptr] = u16_packet_out[u16_index] | 0x8000;
        usart_profile_ptr->u16_txbuf_wr_ptr = ((usart_profile_ptr->u16_txbuf_wr_ptr + 1) % FRAMEWORK_MAX_TX_BUFFER_SIZE);
        // u8_txbuf_wr_ptr now points to the next cyclic buffer byte after
        // the newly queued message
    }
}


//
//  Public function - copy the last received packet into the u16 array pointed to by u16_packet_in
//  Return value is the packet length
//

uint16_t FA_USART_read_16(t_usart_id this_usart, uint16_t * u16_packet_in) {
    t_usart_profile * usart_profile_ptr;
    uint16_t copy_index = 0;

    if (this_usart < no_usart) {
        usart_profile_ptr = &usart_profile[profile_index[this_usart]];
        // if a full packet has been received
        if (usart_profile_ptr->b_rx_pkt_waiting) {

            //copy the next packet from the local rx packet buffer into the memory area pointed to by u16_packet_in
            while (copy_index < usart_profile_ptr->u16_rx_pkt_wr_ptr) {
                u16_packet_in[copy_index] = usart_profile_ptr->u16_rx_packet[copy_index];
                copy_index++;
            }

            // clear out the local rx packet buffer
            usart_profile_ptr->b_rx_pkt_waiting = false;
            usart_profile_ptr->u16_rx_pkt_wr_ptr = 0;
        }
    }
    return copy_index;
}


//
//  Public function - called from the protocol task
//

void FA_USART_Tasks(t_usart_id this_usart) {
    uint8_t u8_packet_len;
    bool b_end_of_packet;

    t_usart_profile * usart_profile_ptr;

    if (this_usart < no_usart) {
        usart_profile_ptr = &usart_profile[profile_index[this_usart]];
        // Manage expirey of the RX timer
        if (FA_TIMER_Has_Finished(&usart_profile_ptr->u32_rx_timer)) {
            data_idle(usart_profile_ptr);
        }

        // =====================
        // Process the TX buffer
        // ===================
        if ((usart_profile_ptr->u16_txbuf_rd_ptr != usart_profile_ptr->u16_txbuf_wr_ptr) && (usart_profile_ptr->bReceiving == false)) {
            // if the Harmony UART TX process is idle it is safe to transmit the next queued message
            if (usart_profile_ptr->FUNC_WriteIsBusy() == false) {
                // if a message is queued
                enable_tx(usart_profile_ptr, tx_mode);
                // initialise length discovery
                u8_packet_len = 0;
                b_end_of_packet = false;

                // parse each byte of the next queued message into u8_tx_packet
                do {
                    if (usart_profile_ptr->e_data_framing == USART_CONFIG_DATA_8_BIT) {
                        usart_profile_ptr->u8_tx_packet[u8_packet_len++] = (uint8_t)(usart_profile_ptr->u16_tx_buffer[usart_profile_ptr->u16_txbuf_rd_ptr] & 0xFF);
                    } else {
                        usart_profile_ptr->u16_tx_packet[u8_packet_len++] = usart_profile_ptr->u16_tx_buffer[usart_profile_ptr->u16_txbuf_rd_ptr] & 0x01FF;
                    }
                    if (usart_profile_ptr->u16_tx_buffer[usart_profile_ptr->u16_txbuf_rd_ptr] & 0x8000) {
                        b_end_of_packet = true;
                    }
                    usart_profile_ptr->u16_txbuf_rd_ptr = ((usart_profile_ptr->u16_txbuf_rd_ptr + 1) % FRAMEWORK_MAX_TX_BUFFER_SIZE);
                } while (b_end_of_packet == false);

                // hand the parsed message over for transmission by the Harmony UART TX process
                if (usart_profile_ptr->e_data_framing == USART_CONFIG_DATA_8_BIT) {
                    usart_profile_ptr->FUNC_Write(usart_profile_ptr->u8_tx_packet, u8_packet_len);
                } else {
                    usart_profile_ptr->FUNC_Write(usart_profile_ptr->u16_tx_packet, u8_packet_len);
                }
            }
        }// Start RX timer once no longer transmitting. Upon expiry the time the USART shall enter into RX mode
        else if (usart_profile_ptr->e_half_duplex_mode == tx_mode) {
            if (usart_profile_ptr->FUNC_WriteIsBusy() == false) {
                if (FA_TIMER_Running(usart_profile_ptr->u32_rx_timer) == false) {
                    FA_TIMER_Start(usart_profile_ptr->u32_rx_timeout, &usart_profile_ptr->u32_rx_timer, Time_uS);
                }
            }
        }// Handle break generation
        else if (usart_profile_ptr->e_half_duplex_mode == tx_break_mode) {
            if (FA_TIMER_Has_Finished(&usart_profile_ptr->u32_break_timer)) {
                usart_profile_ptr->FUNC_Break_Clear();
                enable_rx(usart_profile_ptr);
            }
        }
    }
}

bool FA_USART_ByteTransmittedOrReceived(t_usart_id usart_id) {
    // Get the USART profile
    t_usart_profile* usart_profile_ptr;

    usart_profile_ptr = &usart_profile[profile_index[usart_id]];

    return usart_profile_ptr->FUNC_ByteTransmittedOrReceived();
}

bool FA_USART_Configure_Serial_Port(t_usart_id usart_id, USART_SERIAL_CONFIG* serial_setup) {
    bool b_retVal = true;
    t_usart_profile* usart_profile_ptr;


    // Abort read so SerialSetup() doesn't think that we are currently reading and fails.
    usart_profile_ptr = &usart_profile[profile_index[usart_id]];
    usart_profile_ptr->FUNC_ReadAbort();

    // Change the configuration.
    // When 0 is passed as clkFrequency the SERCOM module will default to the
    // frequency specified in the harmony files.
    b_retVal = usart_profile_ptr->FUNC_SerialSetup(serial_setup, 0);

    // Flush the rx buffer so the read interrupts get re-enabled since they
    // are disabled during ReadAbort().
    flush_rx_buffer(usart_profile_ptr);

    return b_retVal;
}
/*******************************************************************************
 End of File
 */
