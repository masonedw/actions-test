/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    fa_lamp.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _FA_LAMP_H    /* Guard against multiple inclusion */
#define _FA_LAMP_H


void FA_LAMP_Init(void);
void FA_LAMP_Tasks(void);
void FA_LAMP_Set(void);
void FA_LAMP_Clear(void);
void FA_LAMP_Toggle(void);
void FA_LAMP_Flash(uint32_t Flash_Time, t_TimeScaleType TimeScale);
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */


#endif