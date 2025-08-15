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
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the Future Automation application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "definitions.h"
#include "fa_mech_app.h"
#include "fa_protocol.h"
#include "fa_protocol_motor.h"
#include "GPIO_Control.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

static uint8_t NodeType = 0x0D;

// *****************************************************************************
// *****************************************************************************
// Section: Forward references
// *****************************************************************************
// *****************************************************************************

void app_Handle_IR_Command(void);


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialisation and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialise ( void )

  Remarks:
    See prototype in fa_app.h.
 */

void FA_APP_Initialize(void) {
    // Common mech initialisations
    // -----------------
    FA_MECH_Initialize();
    // Device specific initialisations
    // ---------------------
    // Configure CC4 for analogue input
    FA_ADC_Enable_Channel(adc_dc1);
}

/*******************************************************************************
  Function:
    void FA_APP_reset_eeprom ( void )

  Remarks:
    See prototype in fa_app.h.
 */
void FA_APP_reset_eeprom(void) {
    // Common EEPROM area for all mechanisms
    // -------------------------------------
    FA_MECH_reset_eeprom();

    // Custom EEPROM area for specific device
    // --------------------------------------
    // None
}

/*******************************************************************************
  Function:
    void FA_APP_HandleButtonCMD ( uint16_t CurrentCMD )

  Remarks:
    See prototype in fa_app.h.
 */
void FA_APP_HandleButtonCMD(uint16_t CurrentCMD) {

    // The function call below should be restored mechanisms are required to accept FA_1 to FA_100 commands
    //FA_SDN_Motor_Handle_CMD(CurrentCMD);
}

/******************************************************************************
  Function:
    void FA_APP_Tasks ( void )

  Remarks:
    See prototype in fa_app.h.
 */
void FA_APP_Tasks(void) {
    // Common mech tasks
    // -----------------
    FA_MECH_Tasks();
    // Device specific tasks
    // ---------------------
    FA_ADC_Tasks();
}

void FA_APP_HANDLE_FA_PROTOCOL_MESSAGE(t_InterfaceType e_InterfaceType, t_fa_protocol_message * u8_CurrentRx) {

    if (FA_PROTOCOL_Check_For_DeviceID_Or_Broadcast(e_InterfaceType)) {

        switch (u8_CurrentRx->MessageType) {

                //use this for custom calls

            case 0xEE:

                break;

            default: FA_PROTOCOL_Handle_Motor_Message(e_InterfaceType, u8_CurrentRx);

                break;

        }

    }
}

void FA_APP_FAP_InactiveTimeOut(void) {
    FA_PROTOCOL_COMMON_InactiveTimeOut();
    FA_PROTOCOL_Motor_InactiveTimeOut();
}

uint8_t FA_APP_GetNodeType(void) {
    return NodeType;
}

/*******************************************************************************
 End of File
 */
