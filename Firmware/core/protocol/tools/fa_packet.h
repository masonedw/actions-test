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
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    fa_packet.h

  Summary:
    This header file provides all common Future Automation project global definitions.

  Description:

 *******************************************************************************/

#ifndef _FA_FN_H
#define _FA_FN_H

#include "global-definitions.h"
#include "framework.h"

typedef struct {
    uint8_t length;
    uint8_t data[FRAMEWORK_MAX_PACKET_SIZE];
} t_fa_packet_data;


extern bool FA_PACKET_check_header(uint8_t * u8_buffer_ptr);
_Bool FA_PACKET_convert_fa_rs232(uint8_t * in_packet, uint16_t in_packet_len, t_fa_packet_data * command_packet);
_Bool FA_PACKET_convert_sdn_button(uint8_t * in_packet, uint16_t in_packet_len, t_fa_packet_data * command_packet);
extern uint8_t FA_PACKET_extract_fn(t_fa_packet_data * fn_packet_ptr);
extern uint16_t FA_PACKET_extract_sdn_button(t_fa_packet_data * fn_packet_ptr);

#endif /* _FA_FN_H */


//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

