/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    fa_counter.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _fa_timer_H    /* Guard against multiple inclusion */
#define _fa_timer_H


#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

typedef enum enumTimeScale {
    Time_uS, Time_mS, Time_S
} t_TimeScaleType;

extern void FA_TIMER_Init(void);
extern uint32_t FA_TIMER_get_Value(uint32_t TimerNumber_ptr, t_TimeScaleType TimeScale);
extern bool FA_TIMER_Running(uint32_t TimerNumber);
extern void FA_TIMER_Start(uint32_t Target_Value_in_uS, uint32_t * TimerNumber_ptr, t_TimeScaleType TimeScale);
extern void FA_TIMER_Stop(uint32_t * TimerNumber_ptr);
bool FA_TIMER_Has_Finished(uint32_t * TimerNumber);
//bool FA_Timer_Restart(uint32_t TimerNumber);





/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */


#endif