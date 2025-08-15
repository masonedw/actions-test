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
    application.

  Description:
    This header file provides function prototypes and data type definitions for
    the Future Automation application.  This provides a top-level coding root
    for all product specific functionality
 *******************************************************************************/

#ifndef HW_H
#define HW_H

#include "fn_codes.h"

// Motor descriptions
// --------------------
#define HW_NUMBER_OF_MOTORS   1
#define HW_PSE_MOTOR          motor_1

#define HW_NUM_LIMIT_SWITCHES 2

#define HW_M1_LSW_IN_PIN      FRAMEWORK_read_CCx(cc1)
#define HW_M1_LSW_OUT_PIN     FRAMEWORK_read_CCx(cc2)

#define HW_LSID_M1_IN         0
#define HW_LSID_M1_OUT        1


#define HW_NUMBER_OF_COUNTERS 1


#define HW_CC_PIN_8_SHORTED   FN_IN
#define HW_CC_PIN_7_SHORTED   FN_OUT
#define HW_CC_PIN_6_SHORTED   FN_USER_STOP
#define HW_CC_PIN_5_SHORTED   FN_A
#define HW_CC_PIN_4_SHORTED   FN_B
#define HW_CC_PIN_8_RELEASED  FN_NULL
#define HW_CC_PIN_7_RELEASED  FN_NULL
#define HW_CC_PIN_6_RELEASED  FN_NULL
#define HW_CC_PIN_5_RELEASED  FN_IN
#define HW_CC_PIN_4_RELEASED  FN_USER_STOP


#endif /* HW_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

