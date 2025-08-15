#include "fa_timer.h"
#include "framework.h"

#define MAX_32bit_RESOLUTION 0xFFFFFFFF
#define MAX_TIMER_NUMBER 31

uint32_t u32_timer_tick_data;

uint32_t u32_active_timer_mask = 0;

typedef struct {
    uint32_t Timer_Start_Value;
    uint32_t Timer_Target_Value;
    t_TimeScaleType Timer_Scale;
} t_timer_data;


t_timer_data TimersInUse[MAX_TIMER_NUMBER + 1];


void Timer_Restart(uint32_t TimerNumber);

/****************************************************************************
 * Call this when Initialising the system, before using any other part of the
 * timer
 */
void FA_TIMER_Init(void) {
    u32_timer_tick_data = 0;
    FRAMEWORK_TIMER_init();
}

/****************************************************************************

 * Returns current time of the respective timer scaled to timerScale rounded to the nearest whole number

 */

uint32_t FA_TIMER_get_Value(uint32_t TimerNumber, t_TimeScaleType TimeScale) { //this needs fixing
    uint32_t u32_RetVal = 0;
    uint32_t ticks = u32_timer_tick_data - TimersInUse[TimerNumber].Timer_Start_Value;

    switch (TimeScale) {
        case Time_uS:
            u32_RetVal = ticks * FRAMEWORK_TIMER_PERIOD_US;
            break;

        case Time_mS:
            u32_RetVal = (uint32_t) ((ticks + (FRAMEWORK_TMR_INTS_PER_MS / 2)) / FRAMEWORK_TMR_INTS_PER_MS);
            break;

        case Time_S:
            u32_RetVal = (uint32_t) ((ticks + (FRAMEWORK_TMR_INTS_PER_S / 2)) / FRAMEWORK_TMR_INTS_PER_S);
            break;
    }

    return u32_RetVal;
}

/****************************************************************************
 * Returns true if the timer has finished
 */
bool FA_TIMER_Has_Finished(uint32_t * TimerNumber_ptr) {

    bool bRetVal = false;
    //this if statement checks if the timer number passed in corresponds
    //to a running timer, it is used a lot
    if (FA_TIMER_Running(*TimerNumber_ptr)) {
        if (u32_timer_tick_data >= TimersInUse[*TimerNumber_ptr].Timer_Target_Value) {
            bRetVal = true;
            FA_TIMER_Stop(TimerNumber_ptr);
        }
    }
    return bRetVal;
}

/****************************************************************************
 * Returns true if a timer is running
 */
bool FA_TIMER_Running(uint32_t TimerNumber) {

    bool bRetVal = false;

    if ((TimerNumber > 0) && (TimerNumber < MAX_TIMER_NUMBER)) {
        bRetVal = ((1 << (TimerNumber - 1)) & u32_active_timer_mask);
    }
    return bRetVal;
}

/****************************************************************************
 * Starts a timer and returns a timer number to access it
 * The target value is used when checking for an over run
 * This is done by polling has timer finished
 * I should add a way to run the timer without the ability to check for overrun
 */
void FA_TIMER_Start(uint32_t u32_Target_Value, uint32_t * TimerNumber_ptr, t_TimeScaleType TimeScale) {
    uint32_t u32_CurrentTimerNumber = *TimerNumber_ptr;
    t_timer_data * timer_data_ptr;
    bool b_Timer_Is_Running = false;
    uint32_t u32_tick_sample;
    uint32_t u32_scaled_target_value;

    u32_tick_sample = u32_timer_tick_data;
    // Check if timer is already running
    if ((u32_CurrentTimerNumber > 0) && (u32_CurrentTimerNumber < MAX_TIMER_NUMBER)) {
        if (u32_active_timer_mask & (1 << (u32_CurrentTimerNumber - 1))) {
            b_Timer_Is_Running = true;
        }
    }

    // if timer isn't running reserve a place for it in the active timer bit mask
    if (b_Timer_Is_Running == false) {
        u32_CurrentTimerNumber = 1;
        //finds an available timer number
        while (u32_active_timer_mask & (1 << (u32_CurrentTimerNumber - 1)) && (u32_CurrentTimerNumber <= MAX_TIMER_NUMBER)) {
            u32_CurrentTimerNumber++;
        }

        if (u32_CurrentTimerNumber <= MAX_TIMER_NUMBER) {
            // if no timers are running the allocated processor timer is disabled so enable it
            if ((u32_active_timer_mask == 0) || (FRAMEWORK_TIMER_running() == false)) {
                FRAMEWORK_TIMER_start();
            }
            //add current timer to the bitmask
            u32_active_timer_mask = u32_active_timer_mask | (1 << (u32_CurrentTimerNumber - 1));
        } else {
            // Signify that time could not be started
            u32_CurrentTimerNumber = 0;
        }
    }

    // if this is an existing active timer or there was a place for it in the active timer bit mask
    if (u32_CurrentTimerNumber > 0) {
        // point to the correct  location in the TimersInUse array
        timer_data_ptr = TimersInUse + u32_CurrentTimerNumber;

        // set expected expiry time based on time scale used
        // Note: this takes advantage of unsigned integers wrapping round to 0 to avoid boundary issues
        // An unsigned 32 bit integer allows for a bandwidth of 119 hours for any one timer with a tick period of 100us (PIC)
        // and 61 hours with a tick period of 51.2us (SAME54)
#ifdef FRAMEWORK_PIC_BUILD
        switch (TimeScale) {
            case Time_uS:
                u32_scaled_target_value = u32_Target_Value / FRAMEWORK_TIMER_PERIOD_US;
                break;
            case Time_mS:
                u32_scaled_target_value = u32_Target_Value * FRAMEWORK_TMR_INTS_PER_MS;
                break;
            case Time_S:
                u32_scaled_target_value = u32_Target_Value * FRAMEWORK_TMR_INTS_PER_S;
                break;
        }
#else
        switch (TimeScale) {
            case Time_uS:
                u32_scaled_target_value = (uint32_t) (((float) u32_Target_Value) / FRAMEWORK_TIMER_PERIOD_US) + 1;
                break;
            case Time_mS:
                u32_scaled_target_value = (uint32_t) (((float) u32_Target_Value) * FRAMEWORK_TMR_INTS_PER_MS) + 1;
                break;
            case Time_S:
                u32_scaled_target_value = (uint32_t) (((float) u32_Target_Value) * FRAMEWORK_TMR_INTS_PER_S) + 1;
                break;
        }
#endif
        timer_data_ptr->Timer_Start_Value = u32_tick_sample;
        timer_data_ptr->Timer_Target_Value = u32_tick_sample + u32_scaled_target_value;
        timer_data_ptr->Timer_Scale = TimeScale;
    }
    // return allocated timer number
    *TimerNumber_ptr = u32_CurrentTimerNumber;
}

/****************************************************************************
 * stops the specified timer and TC2 if necessary
 * if stop is called remove the timer number that called it
 */
void FA_TIMER_Stop(uint32_t * TimerNumber_ptr) {
    uint32_t TimerNumber = *TimerNumber_ptr;
    if (FA_TIMER_Running(TimerNumber)) {
        u32_active_timer_mask &= ~(1 << (TimerNumber - 1));
        if (u32_active_timer_mask == 0) {
            FRAMEWORK_TIMER_stop();
            u32_timer_tick_data = 0;
        }
        *TimerNumber_ptr = 0;
    }
}

