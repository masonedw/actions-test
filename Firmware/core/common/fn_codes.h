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
    fn_codes.h

  Summary:
    This header file provides all common Future Automation project global definitions.

  Description:

 *******************************************************************************/

#ifndef _FN_CODES_H
#define _FN_CODES_H

// General user commands / IR codes - 0 to 22 reserved
// ---------------------------------------------------

#define  IR_RELEASE_OFFSET            30

#define  FN_RIGHT             0
#define  FN_LEFT              1
#define  FN_F                 4
#define  FN_E                 5
#define  FN_D                 6
#define  FN_C                 8
#define  FN_B                 9
#define  FN_A                 10
#define  FN_HOME              12
#define  FN_STORE             13
#define  FN_PRESET            14
#define  FN_USER_STOP         20
#define  FN_OUT               21
#define  FN_IN                22
#define  FN_HARD_STOP         92
#define  FN_REPEAT            111
#define  FN_LSW_OFF           117
#define  FN_LSW_ON            118
#define  FN_RELEASE           112
#define  FN_ADVANCE_SCRIPT    124
#define  FN_INHIBIT           125


#define  FN_RIGHT_RELEASED             (FN_RIGHT + IR_RELEASE_OFFSET)
#define  FN_LEFT_RELEASED              (FN_LEFT + IR_RELEASE_OFFSET)
#define  FN_F_RELEASED                 (FN_F + IR_RELEASE_OFFSET)
#define  FN_E_RELEASED                 (FN_E + IR_RELEASE_OFFSET)
#define  FN_D_RELEASED                 (FN_D + IR_RELEASE_OFFSET)
#define  FN_C_RELEASED                 (FN_C + IR_RELEASE_OFFSET)
#define  FN_B_RELEASED                 (FN_B + IR_RELEASE_OFFSET)
#define  FN_A_RELEASED                 (FN_A + IR_RELEASE_OFFSET)
#define  FN_HOME_RELEASED              (FN_HOME + IR_RELEASE_OFFSET)
#define  FN_STORE_RELEASED             (FN_PRESET + IR_RELEASE_OFFSET)
#define  FN_USER_STOP_RELEASED         (FN_USER_STOP + IR_RELEASE_OFFSET)
#define  FN_OUT_RELEASED               (FN_OUT + IR_RELEASE_OFFSET)
#define  FN_IN_RELEASED                (FN_IN + IR_RELEASE_OFFSET)


/*
    Multi Button Commands
    0x80 represents [STORE]
    0xC0 represents [STORE]  [STORE]
    0xA0 represents [STORE] [STOP]
    0xE0 represents [STORE] [HOME]
 */

// [STORE] + Command
#define  FN_STORE_RIGHT             0x80
#define  FN_STORE_LEFT              0x81
#define  FN_STORE_F                 0x84
#define  FN_STORE_E                 0x85
#define  FN_STORE_D                 0x86
#define  FN_STORE_C                 0x88
#define  FN_STORE_B                 0x89
#define  FN_STORE_A                 0x8A
//#define  FN_STORE_HOME              0x8C tripple Button Command
//#define  FN_STORE_STORE             0x8D tripple Button Command
#define  FN_STORE_PRESET            0x8E
//#define  FN_STORE_USER_STOP         0x94 tripple Button Command
#define  FN_STORE_OUT               0x95
#define  FN_STORE_IN                0x96

// [STORE] + [STORE] + Command
#define  FN_RESET                   0xCC
#define  FN_NULL                    0xFF

#endif /* _FN_CODES_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

