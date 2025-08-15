#include <stdbool.h>                    // Defines true
#include "fa_timer.h"
#include "framework.h"


//#define FLASH_TIMEOUT    250  /* ms */

// Forward references
void FA_LAMP_Clear(void);


// Global variables
_Bool bLampStatus;
uint32_t u32_lamp_timer;

/******************************************************************************
  internal Function:
    FA_LAMP_Init

  Remarks:

 */
void FA_LAMP_Init(void) {
    FA_LAMP_Clear();
}

void FA_LAMP_Set(void) {
    FRAMEWORK_lamp_on();
    bLampStatus = true;
}

void FA_LAMP_Clear(void) {
    FRAMEWORK_lamp_off();
    bLampStatus = false;
}

void FA_LAMP_Toggle(void) {
    if (bLampStatus) {
        FRAMEWORK_lamp_off();
        bLampStatus = false;
    } else {
        FRAMEWORK_lamp_on();
        bLampStatus = true;
    }
}

void FA_LAMP_Flash(uint32_t Flash_Time, t_TimeScaleType TimeScale) {
    FA_LAMP_Set();
    FA_TIMER_Start(Flash_Time, &u32_lamp_timer, TimeScale);
}

/******************************************************************************
  internal Function:
    FA_ADC_Tasks

  Remarks:

 */
void FA_LAMP_Tasks(void) {
    if (FA_TIMER_Has_Finished(&u32_lamp_timer)) {
        FA_LAMP_Clear();
    }
}

