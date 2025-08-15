#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <stdio.h>
#include "GPIO_Control.h"
#include "fa_timer.h"

static GPIO_INFO GPIO_Settings[GPIO_Max_Names];

void GPIO_UpdateDebounceStates(uint8_t Current_Pin);

void GPIO_Init(void) {

}

/**
 * @brief Configure an individual GPIO pin.
 *
 * @param name         The GPIO name enum identifying the pin.
 * @param portGroup    The port group number.
 * @param pinNumber    The specific pin number within the port group.
 * @param direction    Direction of the pin (input or output).
 */
void GPIO_Config_Pin(GPIO_NAME Name, uint8_t Port_Group, uint8_t Port_Pin,
                     Pin_Use e_PIN_Function, FA_PERIPHERAL_FUNCTION Selected_PERIPHERAL_FUNCTION,
                     Pin_Soft_Pull eSoftPull, Switch_Orientation e_Set_Orientation) {
    if (Name < GPIO_Max_Names) {
        GPIO_Settings[Name].struct_Port_Config.Port_Group = Port_Group;
        GPIO_Settings[Name].struct_Port_Config.Port_Pin = Port_Pin;
        GPIO_Settings[Name].e_PinFunctiion = e_PIN_Function;
        GPIO_Settings[Name].e_Switch_Orientation = e_Set_Orientation;

        FRAMEWORK_PinPeripheralFunctionConfig(GPIO_Settings[Name].struct_Port_Config, Selected_PERIPHERAL_FUNCTION);

        switch (e_PIN_Function) {
            case GPIO_Output:
                FRAMEWORK_GPIO_OutputEnable(GPIO_Settings[Name].struct_Port_Config);
                GPIO_Set_Low(Name);
                break;
            case GPIO_Input:
                FRAMEWORK_GPIO_InputEnable(GPIO_Settings[Name].struct_Port_Config);
                GPIO_Settings[Name].b_Stable_State = false;
                GPIO_Settings[Name].b_Switch_State_Change = false;
                GPIO_Settings[Name].b_Last_Sampled_State = GPIO_Read_Current_State(Name);
                switch (eSoftPull) {
                    case No_Soft_Pull:
                        FRAMEWORK_GPIO_InputDontPull(GPIO_Settings[Name].struct_Port_Config);
                        break;
                    case Soft_Pull_Up:
                        FRAMEWORK_GPIO_InputPullUp(GPIO_Settings[Name].struct_Port_Config);
                        break;
                    case Soft_Pull_Down:
                        FRAMEWORK_GPIO_InputPullDown(GPIO_Settings[Name].struct_Port_Config);
                        break;
                }
                break;
            case PWM_PAD:
                break;
            case ADC_PAD:
                break;
            case DAC_PAD:
                break;
            case SERCOM_PAD:
                break;
            case EIC_INT:
                break;
            case GMAC:
                break;
        }
    }
}

/**
 * @brief Read the debounced (stable) state of an input pin.
 *
 * @param name    The GPIO name enum identifying the pin.
 * @return true if the last stable pin state is high (after debounce), false otherwise.
 */
bool GPIO_Read_Debounced_State(GPIO_NAME Name) {
    return GPIO_Settings[Name].b_Stable_State;
}

/**
 * @brief Read the debounced (stable) state of an input pin.
 *
 * @param name    The GPIO name enum identifying the pin.
 * @return the current pin state, inverted if necessary.
 */
bool GPIO_Read_Current_State(GPIO_NAME Name) {
    //Invert the switch orientation if necessary
    return FRAMEWORK_GPIO_Get(GPIO_Settings[Name].struct_Port_Config) ^ GPIO_Settings[Name].e_Switch_Orientation;
}

/**
 * @brief Read the currently selected multiplex function of a GPIO capable pin.
 *
 * @param name    The GPIO name enum identifying the pin.
 * @return the current multiplex function.
 */
Pin_Use GPIO_Read_Current_Function(GPIO_NAME Name) {
    return GPIO_Settings[Name].e_PinFunctiion;
}

/**
 * @brief Set a GPIO output pin to logic high.
 *
 * @param name    The GPIO name enum identifying the pin.
 */
void GPIO_Set_High(GPIO_NAME Name) {
    if (GPIO_Settings[Name].e_Switch_Orientation == Orientation_NonInverted) {
        FRAMEWORK_GPIO_SET(GPIO_Settings[Name].struct_Port_Config);
    } else {
        FRAMEWORK_GPIO_Clear(GPIO_Settings[Name].struct_Port_Config);
    }
}

/**
 * @brief Set a GPIO output pin to logic low.
 *
 * @param name    The GPIO name enum identifying the pin.
 */
void GPIO_Set_Low(GPIO_NAME Name) {
    if (GPIO_Settings[Name].e_Switch_Orientation == Orientation_NonInverted) {
        FRAMEWORK_GPIO_Clear(GPIO_Settings[Name].struct_Port_Config);
    } else {
        FRAMEWORK_GPIO_SET(GPIO_Settings[Name].struct_Port_Config);
    }
}

/**
 * @brief Toggle the current state of a GPIO output pin.
 *
 * @param name    The GPIO name enum identifying the pin.
 */
void GPIO_Toggle_Output(GPIO_NAME Target) {

    FRAMEWORK_GPIO_Toggle(GPIO_Settings[Target].struct_Port_Config);
}

/**
 * @brief Periodic task to handle GPIO Control
 *
 * This function should be called regularly
 */
void GPIO_Task(void) {
    for (uint8_t Current_Pin = 0; Current_Pin < GPIO_Max_Names; Current_Pin++) {
        switch (GPIO_Settings[Current_Pin].e_PinFunctiion) {

            case GPIO_Output:
                break;
            case GPIO_Input:
                GPIO_UpdateDebounceStates(Current_Pin);
                break;
            case PWM_PAD:
                break;
            case ADC_PAD:
                break;
            case DAC_PAD:
                break;
            case SERCOM_PAD:
                break;
            case EIC_INT:
                break;
            case GMAC:
                break;
        }
    }
}

/**
 * @brief Periodic task to update the debounce logic for all GPIO input pins.
 *
 * This function should be called regularly to process input changes,
 * update stable states, and handle error timing.
 */
void GPIO_UpdateDebounceStates(uint8_t Current_Pin) {
    if (GPIO_Read_Current_State(Current_Pin) != GPIO_Settings[Current_Pin].b_Last_Sampled_State) {
        FA_TIMER_Start(FA_GPIO_Debounce_Timeout, &GPIO_Settings[Current_Pin].u32_Debounce_Timer, Time_mS);
        GPIO_Settings[Current_Pin].b_Last_Sampled_State = GPIO_Read_Current_State(Current_Pin);
        if (!FA_TIMER_Running(GPIO_Settings[Current_Pin].u32_Error_Timer)) {
            FA_TIMER_Start(FA_GPIO_Error_Timeout, &GPIO_Settings[Current_Pin].u32_Error_Timer, Time_mS);
        }
    } else if (FA_TIMER_Has_Finished(&GPIO_Settings[Current_Pin].u32_Debounce_Timer)) {
        if (GPIO_Settings[Current_Pin].b_Last_Sampled_State != GPIO_Settings[Current_Pin].b_Stable_State) {
            GPIO_Settings[Current_Pin].b_Stable_State = GPIO_Settings[Current_Pin].b_Last_Sampled_State;
            GPIO_Settings[Current_Pin].b_Switch_State_Change = true;
        }
        FA_TIMER_Stop(&GPIO_Settings[Current_Pin].u32_Error_Timer);
    }
    if (FA_TIMER_Has_Finished(&GPIO_Settings[Current_Pin].u32_Error_Timer)) {

        FA_TIMER_Stop(&GPIO_Settings[Current_Pin].u32_Debounce_Timer);
        //call an error
    }
}

bool GPIO_Check_For_State_Change(GPIO_NAME Target) {
    bool bRetVal = false;
    if (GPIO_Settings[Target].b_Switch_State_Change) {
        bRetVal = true;
        GPIO_Settings[Target].b_Switch_State_Change = false;
    }
    return bRetVal;
}