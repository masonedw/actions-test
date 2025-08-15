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
    fa_app.h

  Summary:
    This header file provides prototypes and definitions for the Future Automation
    RS485 interface.

  Description:
    This header file provides function prototypes and data type definitions for
    the Future Automation SDN interface.
 *******************************************************************************/

#ifndef FA_APP_H
#define FA_APP_H

#include "global-definitions.h"
#include "fa_protocol.h"

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************



// *****************************************************************************
// *****************************************************************************
// Section: Function declarations
// *****************************************************************************
// *****************************************************************************

void FA_APP_Initialize(void);
void FA_APP_Tasks(void);
void FA_APP_HANDLE_FA_PROTOCOL_MESSAGE(t_InterfaceType e_InterfaceType, t_fa_protocol_message * u8_CurrentRx);
void FA_APP_HandleButtonCMD(uint16_t CurrentCMD);
void FA_APP_FAP_InactiveTimeOut(void);
uint8_t FA_APP_Get_Device_ID(uint8_t uID_Byte);
void FA_APP_Set_Device_ID(uint8_t uID_Byte, uint8_t uID_Data);
uint8_t FA_APP_GetNodeType(void);


// Stub functions
void FA_App_Save_MotorSpeeds(void);
uint8_t FA_App_Get_SlowSpeed(void);
uint8_t FA_App_Get_FastDownSpeed(void);
uint8_t FA_App_Get_FastUpSpeed(void);
void FA_App_Set_SlowSpeed(uint8_t SlowSpeed);
void FA_App_Set_FastDownSpeed(uint8_t FastDownSpeed);
void FA_App_Set_FastUpSpeed(uint8_t FastUpSpeed);
bool FA_App_SystemHas_Direction(void);
bool FA_App_SystemHas_Speeds(void);
bool FA_App_SystemHas_IPs(void);
bool FA_App_Get_Direction(void);
void FA_App_Set_Direction(bool bDirectionalBool);
void FA_App_Save_Direction(void);
uint8_t FA_App_Get_Up_Limit(void);
uint8_t FA_App_Get_Down_Limit(void);
void FA_App_Set_Up_Limit(void);
void FA_App_Set_Down_Limit(void);
void FA_App_Save_MotorLimits(void);
void FA_App_Set_InternalPos(uint8_t u_TargetPos, uint16_t InternalPos);
uint16_t FA_App_Get_InternalPos(uint8_t u_TargetPos);
void FA_App_Save_InternalPos(uint8_t u_TargetPos);

#endif /* FA_APP */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus

#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

