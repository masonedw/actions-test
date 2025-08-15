/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    GPIO_Control.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _GPIO_CONTROL_H    /* Guard against multiple inclusion */
#define _GPIO_CONTROL_H

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <stdio.h>
#include "framework.h"

typedef enum {
    GPIO_Output,
    GPIO_Input,
    PWM_PAD,
    ADC_PAD,
    DAC_PAD,
    SERCOM_PAD,
    EIC_INT,
    GMAC,
} Pin_Use;

typedef enum {
    No_Soft_Pull,
    Soft_Pull_Up,
    Soft_Pull_Down
} Pin_Soft_Pull;

typedef enum {
    Orientation_NonInverted = 0,
    Orientation_Inverted
} Switch_Orientation;

typedef struct {
    GPIO_PORT struct_Port_Config;

    Pin_Use e_PinFunctiion;

    uint32_t u32_Debounce_Timer;

    uint32_t u32_Error_Timer;

    bool b_Last_Sampled_State;

    bool b_Stable_State;

    bool b_Switch_State_Change;

    Switch_Orientation e_Switch_Orientation;

} GPIO_INFO;


#define FA_GPIO_Error_Timeout           500
#define FA_GPIO_Debounce_Timeout        30



void GPIO_Init(void);
void GPIO_Config_Pin(GPIO_NAME Name, uint8_t Port_Group, uint8_t Port_Pin,
        Pin_Use e_Set_Direction, FA_PERIPHERAL_FUNCTION Select_PERIPHERAL_FUNCTION,
        Pin_Soft_Pull eSoftPull, Switch_Orientation e_Set_Orientation);
bool GPIO_Read_Debounced_State(GPIO_NAME Name);
bool GPIO_Read_Current_State(GPIO_NAME Name);
Pin_Use GPIO_Read_Current_Function(GPIO_NAME Name);
void GPIO_Set_High(GPIO_NAME Name);
void GPIO_Set_Low(GPIO_NAME Name);
void GPIO_Toggle_Output(GPIO_NAME Target);
void GPIO_Task(void);
bool GPIO_Check_For_State_Change(GPIO_NAME Target);
#endif