#include "definitions.h"                // SYS function prototypes
#include "framework_common.h"
#include "fa_adc.h"
#include "GPIO_Control.h"

typedef enum {
    inactive,
    starting,
    waiting
} t_adc_conversion_status;

typedef struct {
    t_adc_conversion_status e_state; // current conversion status
    uint16_t u16_latched_value; // most recently captured digital value
} t_adc_conversion_data;


t_adc_conversion_data adc_conversion[FRAMEWORK_NO_OF_DC_CHANNELS];


// used to track which, if any ADC channels are currently enabled
uint8_t u8_channel_bitmap = 0;

/******************************************************************************
  Exported Function:
    FA_ADC_Enable

  Remarks: Enable ADC logic and (re)initialise conversion state machine ready for
           reading
 */
void FA_ADC_Enable_Channel(t_adc_id adc_id) {
    uint8_t u8_index;

    // if no ADC channels are currently enabled
    if (u8_channel_bitmap == 0) {
        //Initialise conversion state machine for all channels
        for (u8_index = adc_dc1; u8_index < FRAMEWORK_NO_OF_DC_CHANNELS; u8_index++) {
            adc_conversion[u8_index].e_state = inactive;
        }
        // Imnitialie the processor's ADC block
        FRAMEWORK_Enable_adc_block();
    }

    // enable and initialise the requested channel (adc_id))
    adc_conversion[adc_id].e_state = starting;
    adc_conversion[adc_id].u16_latched_value = 0;
    // select ADC for the respective pin's multiplexed function
    FRAMEWORK_Enable_adc_channel(adc_id);
    // log the requested channel as active in the channel bitmap
    u8_channel_bitmap |= (1 << adc_id);
}

/******************************************************************************
  Exported Function:
    FA_ADC_Disable

  Remarks: Deactivate ADC block and pause conversion state machine

 */
void FA_ADC_Disable_Channel(t_adc_id adc_id) {
    // make the conversion state machine inactive for the requested channel
    adc_conversion[adc_id].e_state = inactive;
    // log the requested channel as inactive in the channel bitmap
    u8_channel_bitmap &= ~(1 << adc_id);
    // Reset the respective pin's multiplexed function to default (dependent on framework but usually GPIO)
    FRAMEWORK_Disable_adc_channel(adc_id);
    // Disable ADC block if there are no longer any active ADC channels
    if (u8_channel_bitmap == 0) {
        FRAMEWORK_Disable_adc_block();
    }
}

/******************************************************************************
  Exported Function:
    FA_ADC_get_Value

  Remarks: Enquiry function - report the most recently latched digital ADC reading

 */
uint16_t FA_ADC_get_Value(t_adc_id adc_id) {
    return adc_conversion[adc_id].u16_latched_value;
}

/******************************************************************************
  Internal Function:
    void conversion_state_machine(t_adc_id adc_id)

  Remarks: ADC conversion state machine

 */
static void conversion_state_machine(t_adc_id adc_id) {
    switch (adc_conversion[adc_id].e_state) {
            // ADC is turned off
        case inactive:
            // Do nothing
            break;

            // Ready to start next conversion
        case starting:
            FRAMEWORK_select_adc_channel(adc_id);
            FRAMEWORK_start_adc_conversion(adc_id);
            adc_conversion[adc_id].e_state = waiting;
            break;

            // Waiting for the result of the current conversion
        case waiting:
            if (FRAMEWORK_get_adc_conversion_status(adc_id) != 0) {
                // record acquired digital value
                adc_conversion[adc_id].u16_latched_value = FRAMEWORK_get_last_adc_conversion_result(adc_id);
                // switch to initiating the next conversion
                adc_conversion[adc_id].e_state = starting;
            }
            break;
    }
}

/******************************************************************************
  Internal Function:
    FA_ADC_Tasks(void)

  Remarks: Manages ADC acquisition as a function of the main look

 */
void FA_ADC_Tasks(void) {
    uint8_t u8_index;

    // For all DC channels on the board
    for (u8_index = adc_dc1; u8_index < FRAMEWORK_NO_OF_DC_CHANNELS; u8_index++) {
        // if the channel's ADC capable pin is active
        if (adc_conversion[u8_index].e_state != inactive) {
            // handle ADC conversion
            conversion_state_machine(u8_index);
        }
    }
}