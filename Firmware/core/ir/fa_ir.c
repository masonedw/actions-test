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
    This file contains the source code for the Future Automation application. It
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
#include "global-definitions.h"
#include "fn_codes.h"
#include "fa_timer.h"
#include "fa_ir.h"
// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************


int actual_Time = 0;
int counter_value = 0;


bool MessageReady = false;
uint16_t StoredCommand = 0;


uint32_t Message = 0;
uint32_t WhatBitIsIt = 0;
uint32_t StoredMessage = 0;


uint32_t Address_Low = 0;
uint32_t Address_High = 0;
uint32_t Command_Low = 0;
uint32_t Command_High = 0;
uint32_t Address_Sum = 0;
uint32_t Command_Sum = 0;

uint32_t IR_Timer_Number = 0;
uint32_t IR_Multi_Timer_Number = 0;

enum IR_States {
    Timer_off, Timer_on
};
enum IR_States IR_State;

enum Multi_States {
    First_Command, Second_Command, Third_Command
};
enum Multi_States Multi_State;

enum Bit_Type {
    Start, One, Zero, Fault, Repeat
};
enum Bit_Type Bit_Rec;

enum Message_States {
    IR_Idle, IR_Repeat, IR_Release, IR_Fault, IR_Message_Ready
};
enum Message_States Message_State;



//enum Button_States Button_State;

/*
typedef struct {
    enum Button_States Button_State;
} t_IR_Button_State;
 */
//23 because the number representing each button aren't 1 to 15
t_IR_Button_State IR_Button_State[23];

// *****************************************************************************
// *****************************************************************************
// Section: Forward references
// *****************************************************************************
// *****************************************************************************

void ClearValues(void);
void CheckOverrun(void);
void SetCommand(uint16_t Command);

/******************************************************************************
  internal Function:
    MessageVerification

  Remarks:

 */
bool MessageVerification(uint32_t CurrentMessage) {

    bool retVal = false;

    Address_Low = CurrentMessage;
    Address_High = CurrentMessage;

    Address_High = Address_High >> 8;
    Address_High = Address_High & 0xFF;

    Address_Low = Address_Low & 0xFF;

    Command_Low = CurrentMessage;
    Command_High = CurrentMessage;

    Command_High = Command_High >> 24;
    Command_High = Command_High & 0xFF;
    Command_Low = Command_Low >> 16;
    Command_Low = Command_Low & 0xFF;

    Address_Sum = Address_Low | Address_High;
    Command_Sum = Command_Low | Command_High;

    if ((Address_Sum == 0xFF) && (Command_Sum == 0xFF) && (Address_High == 0xFF)) {
        retVal = true;
    }

    return retVal;
}

/******************************************************************************
  internal Function:
    Message_Builder

  Remarks:

 */
void Message_Builder(enum Bit_Type Message_Bits) {

    switch (Message_Bits) {
        case Start: Message = 0;
            WhatBitIsIt = 0;
            break;
        case One: Message = Message + (1 << WhatBitIsIt);
            WhatBitIsIt++;
            break;
        case Zero:
            WhatBitIsIt++;
            break;
        case Repeat: Message = 0;
            WhatBitIsIt = 0;
            break;
        case Fault: Message = 0;
            WhatBitIsIt = 0;
            break;
    }
    if (WhatBitIsIt == 32) {
        StoredMessage = Message;
        Message = 0;
        WhatBitIsIt = 0;
        Message_State = IR_Message_Ready;
    }
}

/******************************************************************************
  internal Function:
    FA_IR_Edge_InterruptHandler

  Remarks:

 */
void IR_Edge_InterruptHandler(uintptr_t context) {
    //static uint8_t ThisB = 0;
    //GPIO_PC18_LAMP_Toggle();
    switch (IR_State) {
        case Timer_off: IR_State = Timer_on;
            FA_TIMER_Start(HW_IR_TIMER_uS, &IR_Timer_Number, Time_uS);
            break;
        case Timer_on: actual_Time = FA_TIMER_get_Value(IR_Timer_Number, Time_uS);
            FA_TIMER_Start(HW_IR_TIMER_uS, &IR_Timer_Number, Time_uS);
            /*ThisB++;

            if (ThisB > 3){
               GPIO_PC18_LAMP_Toggle();
               ThisB = 0;
            }*/
            if ((actual_Time >= 12650)&&(actual_Time <= 13700)) {
                //start bit
                Message_Builder(Start);
                break;
            } else if ((actual_Time >= 1850)&&(actual_Time <= 2450)) {
                //save as a 1
                Message_Builder(One);
                break;
            } else if ((actual_Time >= 850)&&(actual_Time <= 1450)) {
                //save as a 0
                Message_Builder(Zero);
                break;
            } else if ((actual_Time >= 10550)&&(actual_Time <= 11650)) {
                Message_Builder(Repeat);
                Message_State = IR_Repeat;
                break;
            } else if ((actual_Time >= 97000)&&(actual_Time <= 111650)) {
                Message_Builder(Repeat);
                Message_State = IR_Repeat;

                break;
            } else if ((actual_Time >= 41000)&&(actual_Time <= 43000)) {
                Message_Builder(Repeat);
                Message_State = IR_Repeat;

                break;
            } else if ((actual_Time >= 11000)&&(actual_Time <= 11500)) {
                //This is the small ignored dip during repeat
                //transmission it hast to be included for fault
                //checking
                break;
            } else {
                ClearValues();
                Message_State = IR_Fault;
                break;
            }
            break;
    }
}

/******************************************************************************
  internal Function:
    FA_IR_Init

  Remarks:

 */
void FA_IR_Init(t_IR_Button_State Pass_State) {
    uint8_t ButtonInitilise = 0;
    //Interrupt Initialise
    EIC_InterruptEnable(EIC_PIN_6);
    EIC_CallbackRegister(EIC_PIN_6, IR_Edge_InterruptHandler, (uintptr_t)NULL);
    IR_State = Timer_off;
    Multi_State = First_Command;
    Message_State = IR_Idle;

    for (ButtonInitilise = 0; ButtonInitilise < 23; ButtonInitilise++) {
        IR_Button_State[ButtonInitilise] = Pass_State;
    }


}

/******************************************************************************
  internal Function:
    FA_IR_Tasks

  Remarks:

 */
void FA_IR_Tasks(void) {
    static uint16_t Check_For_Multi = 0;
    static uint16_t LastCommand = 0;

    CheckOverrun();

    switch (Message_State) {
        case IR_Idle:
            break;
        case IR_Repeat: if (IR_Button_State[LastCommand] == On_Hold) {
                //Logic Problem? If on hold and repeat cmd then message could be whatever was last cmd
                //todo, should clear if not repeat directly afte cmd
                SetCommand(LastCommand);
            }
            Message_State = IR_Idle;
            break;
        case IR_Release: if (IR_Button_State[LastCommand] == Car_Window) {
                //send button command when button pressed then stop when button released
                SetCommand(FN_USER_STOP);
            } else if (IR_Button_State[LastCommand] == On_Release) {
                //Send original command but after release
                SetCommand(LastCommand);
            } else if (IR_Button_State[LastCommand] == Press_and_Release) {
                //send separate command for button pressed and button released
                SetCommand(LastCommand + IR_RELEASE_OFFSET);
            }

            LastCommand = GLB_NULL_INT;
            Message_State = IR_Idle;
            break;
        case IR_Fault: Message_State = IR_Idle;
            break;
        case IR_Message_Ready: //Check message follows the correct mirroring format of the IR Protocol
            if (MessageVerification(StoredMessage) == true) {
                //check its not the start of or part of a multi button command
                if ((Command_Low != FN_STORE)&&(Multi_State == First_Command)) {
                    //Store the command
                    LastCommand = Command_Low;
                    //check the state to see if the button should be sent now
                    if ((IR_Button_State[LastCommand] == On_Press) || (IR_Button_State[LastCommand] == Car_Window) || (IR_Button_State[LastCommand] == Press_and_Release)) {
                        //set message ready
                        SetCommand(Command_Low);
                    }
                } else {
                    FA_TIMER_Start(HW_IR_MULTI_TIMER_mS, &IR_Multi_Timer_Number, Time_mS);
                    switch (Multi_State) {
                        case First_Command: Check_For_Multi = 0x80;
                            Multi_State = Second_Command;
                            break;
                        case Second_Command: switch (Command_Low) {
                                case FN_STORE: Check_For_Multi = 0xC0;
                                    Multi_State = Third_Command;
                                    break;
                                case FN_USER_STOP: Check_For_Multi = 0xA0;
                                    Multi_State = Third_Command;
                                    break;
                                case FN_HOME: Check_For_Multi = 0xE0;
                                    Multi_State = Third_Command;
                                    break;
                                default: Check_For_Multi = Check_For_Multi + (0xFF & Command_Low);
                                    SetCommand(Check_For_Multi);
                                    FA_TIMER_Stop(&IR_Multi_Timer_Number);
                                    Multi_State = First_Command;
                                    break;
                            }
                            break;
                        case Third_Command: Check_For_Multi = Check_For_Multi + (0xFF & Command_Low);
                            Multi_State = First_Command;
                            SetCommand(Check_For_Multi);
                            FA_TIMER_Stop(&IR_Multi_Timer_Number);
                            break;
                        default:
                            break;
                    }
                }
            }
            Message_State = IR_Idle;
            break;
    }

    if (FA_TIMER_Has_Finished(&IR_Multi_Timer_Number)) {
        Multi_State = First_Command;
        SetCommand(Check_For_Multi);
        Check_For_Multi = 0;
    }
}

/******************************************************************************
  internal Function:
    CheckOverrun

  Remarks:


 */
void CheckOverrun(void) {
    if (FA_TIMER_Has_Finished(&IR_Timer_Number)) {
        ClearValues();
        Message_State = IR_Release;
    }
}

/******************************************************************************
  internal Function:
    ClearValues

  Remarks:


 */
void ClearValues(void) {
    Message_Builder(Fault);
    IR_State = Timer_off;
    FA_TIMER_Stop(&IR_Timer_Number);
    actual_Time = 0;
}

/******************************************************************************
  internal Function:
    SetCommand

  Remarks:


 */
void SetCommand(uint16_t Command) {
    StoredCommand = Command;
    MessageReady = true;
}

/******************************************************************************
  internal Function:
    SetCommand

  Remarks:


 */
uint16_t FA_IR_Get_Command(void) {
    uint16_t uRetVal = FN_NULL;
    if (MessageReady) {
        MessageReady = false;
        uRetVal = StoredCommand;
        StoredCommand = FN_NULL;
    }
    return uRetVal;
}






/*******************************************************************************
 End of File
 */
