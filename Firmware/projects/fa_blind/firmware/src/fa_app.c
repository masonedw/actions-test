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

#include <stdint.h>
#include "definitions.h"
#include "fa_at24.h"
#include "fa_usart.h"
#include "global-definitions.h"
#include "fn_codes.h"
#include "fa_ir.h"
#include "fa_timer.h"
#include "fa_tcpip.h"
#include "fa_eeprom.h"
#include "fa_eeprom_map.h"
#include "framework.h"
#include "fa_packet.h"
#include "fa_lamp.h"
#include "protocol_common.h"
#include "fa_protocol.h"
#include "fa_protocol_motor.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
static const uint16_t NodeType = 0x0969;



// *****************************************************************************
// *****************************************************************************
// Section: Forward references
// *****************************************************************************
// *****************************************************************************

void app_Handle_FN_Command(uint16_t u16_CurrentCMD);
void FA_APP_Handle_TCPIP_Command(t_fa_packet_data * fn_TCPIP_packet_ptr);

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
    SYS_CONSOLE_MESSAGE(" FA APP: Initialising\r\n");
    FA_EEPROM_init();
    FA_AT24_Init();
    FA_IR_Init(On_Press);
    FA_TIMER_Init();
    FA_PROTOCOL_COMMON_Initialize();
    FA_PROTOCOL_Motor_INITIALIZE();
    FA_PROTOCOL_Initialize_232(FRAMEWORK_232_PORT);
    FA_PROTOCOL_Initialize_485(FRAMEWORK_485_PORT);
    FA_PROTOCOL_Initialize_TCPIP();
    FA_LAMP_Init();
    FA_LAMP_Flash(1, Time_S);
}

void FA_APP_reset_eeprom(void) {
    SYS_CONSOLE_MESSAGE(" SmartEEPROM: Initialising\r\n");
    FA_PROTOCOL_COMMON_Reset_EEPROM();
    FA_PROTOCOL_Motor_reset_eeprom();
}

void FA_APP_HandleButtonCMD(uint16_t CurrentCMD) {
    //FA_PROTOCOL_Keypad_Handle_CMD(CurrentCMD, On_Press);
}

/*
void Handle_TCPIP_Command(uint8_t Command){
    if (Command != GLB_NULL_INT){
        FA_SDN_Keypad_Handle_CMD(Command,On_Press);
    }
}
 */
//void FA_APP_Handle_TCPIP_Command(t_fa_packet_data * fn_TCPIP_packet_ptr) {
//    uint16_t CurrentCMD = FN_NULL;
//
//    if (fn_TCPIP_packet_ptr->data[fn_TCPIP_packet_ptr->length - 1] == '\r') {
//        fn_TCPIP_packet_ptr->length = fn_TCPIP_packet_ptr->length - 1;
//    }
//    //Check for FA_A type message
//    CurrentCMD = FA_PACKET_extract_fn(fn_TCPIP_packet_ptr);
//    if (CurrentCMD != FN_NULL) {
//        //app_Handle_FN_Command(CurrentCMD);
//    } else {
//        //Check for FA_1 type message
//        CurrentCMD = FA_PACKET_extract_sdn_button(fn_TCPIP_packet_ptr);
//        if (CurrentCMD != GLB_NULL_INT) {
//            FA_APP_HandleButtonCMD(CurrentCMD);
//        }
//    }
//}

/*
FA_APP_Handle_TCPIP_Command
    }
}
 */
bool FA_APP_Check_Stop(t_fa_packet_data * CheckStop) {
    bool CMD_Is_Stop = false;
    //check if cmd is stop
    return CMD_Is_Stop;
}

/******************************************************************************
  Function:
    void FA_APP_Tasks ( void )

  Remarks:
    See prototype in fa_app.h.
 */

void FA_APP_Tasks(void) {
    //    t_fa_packet_data    tcpip_message;

    FA_AT24_Tasks();

    // TODO TCP stuff here commented out can be added back.
    // add it to FA_PROTOCOL_Tasks with the rest of the interfaces.
    /*if (FA_TCPIP_Tasks(&tcpip_message)) {
        if (FA_PACKET_check_header(tcpip_message.data)){
            FA_APP_Handle_TCPIP_Command(&tcpip_message);
        }
    }*/

    FA_IR_Tasks();
    app_Handle_FN_Command(FA_IR_Get_Command());

    FA_LAMP_Tasks();
    FA_PROTOCOL_COMMON_Tasks();
    FA_PROTOCOL_Tasks();

}

void app_Handle_FN_Command(uint16_t u16_CurrentCMD) {

    switch (u16_CurrentCMD) {

        case FN_IN:

            break;
        case FN_OUT:
            break;
        case FN_USER_STOP:
            break;
        case FN_PRESET:
            break;
        case FN_STORE:
            break;
        case FN_HOME:
            break;
        case FN_A:
            break;
        case FN_B:
            break;
        case FN_C:
            break;
        case FN_D:
            break;
        case FN_E:
            break;
        case FN_F:
            break;
        case FN_LEFT:
            break;
        case FN_RIGHT:
            break;
        case FN_RESET: SYS_RESET_SoftwareReset();
            break;
        default:
            break;

    }
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
    //FA_SDN_Keypad_Save_CMDs();
    FA_PROTOCOL_COMMON_InactiveTimeOut();
    FA_PROTOCOL_Motor_InactiveTimeOut();
}

uint16_t FA_APP_GetNodeType(void) {
    return NodeType;
}
/*******************************************************************************
 End of File
 */
