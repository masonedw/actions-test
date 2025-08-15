/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    ADC_Task.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _ADC_TASK_H    /* Guard against multiple inclusion */
#define _ADC_TASK_H

#include "framework.h"

void FA_ADC_Enable_Channel(t_adc_id adc_id);
void FA_ADC_Disable_Channel(t_adc_id adc_id);
void FA_ADC_Tasks(void);
uint16_t FA_ADC_get_Value(t_adc_id adc_id);
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */


#endif