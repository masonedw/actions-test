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
    fa_sdn_keypad.h

  Summary:
    This header file provides prototypes and definitions for the Future Automation
    RS485 interface.

  Description:
    This header file provides function prototypes and data type definitions for
    the Future Automation SDN interface.
 *******************************************************************************/

#ifndef FA_SDN_KEYPAD_H
#define FA_SDN_KEYPAD_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "fa_sdn.h"
//#include "fa_app.h"
#include "fa_ir.h"



// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************
#pragma pack(push, 1)

typedef struct {
    uint8_t u8_OnPress_Command;
    uint8_t u8_OnPress_Target_Value_L;
    uint8_t u8_OnPress_Target_Value_H;
    uint8_t u8_OnPress_Targeting;

    uint8_t u8_OnHold_Command;
    uint8_t u8_OnHold_Target_Value_L;
    uint8_t u8_OnHold_Target_Value_H;
    uint8_t u8_OnHold_Targeting;

    uint8_t u8_OnRelease_Command;
    uint8_t u8_OnRelease_Target_Value_L;
    uint8_t u8_OnRelease_Target_Value_H;
    uint8_t u8_OnRelease_Targeting;

    uint8_t u8_Target_ID_1;
    uint8_t u8_Target_ID_2;
    uint8_t u8_Target_ID_3;

} t_SDN_KEYPAD_MessageStructure;
#pragma pack(pop)
// *****************************************************************************
// *****************************************************************************
// Section: Function declarations
// *****************************************************************************
// *****************************************************************************

void FA_SDN_KEYPAD_reset_eeprom(void);
void FA_SDN_KEYPAD_INITIALIZE(void);
void FA_SDN_Handle_Keypad_Message(t_InterfaceType e_InterfaceType, t_SDN_message* u16_CurrentRx);
void FA_SDN_Keypad_Handle_CMD(uint16_t CurrentCMD, t_IR_Button_State Press_Type);
void FA_SDN_Keypad_Get_Custom_Targeting(uint8_t u8_MessageTarget, uint8_t CurrentCMD, t_USART_ID* source, t_USART_ID* dest);
void FA_SDN_Keypad_Send_Stop(void);
void FA_SDN_Keypad_Save_CMDs(void);
void FA_SDN_Keypad_InactiveTimeOut(void);


// *****************************************************************************
// *****************************************************************************
// Functions
// *****************************************************************************
// *****************************************************************************


#endif

//DOM-IGNORE-BEGIN
#ifdef __cplusplus

#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

