/**
 * @file adc.c
 * @brief ADC (analog-to-digital converter) functions.
 */

#include "headers.h"

/**
 * @brief Initialize the ADC (analog-to-digital converter) control registers.
 * 
 * The prescaler is set to divide the system clock by 2 for the shortest conversion time.
 * The timebase is set to 4 clock cycles for 1us operation at 3.3 MHz and the reference voltage is set to VDD.
 * The sample duration is set to 64 clock cycles.
 * The ADC is set to operate in free-running mode with left adjustment of the result.
 * The positive input of the ADC is selected as AIN2 (potentiometer R1).
 * The ADC is configured to start conversions immediately in single conversion mode with 8-bit resolution.
 * 
 * @note The ADC samples a potentiometer at 8-bit resolution.
 */
void adc_init(void)
{
    ADC0.CTRLA = ADC_ENABLE_bm;
    ADC0.CTRLB = ADC_PRESC_DIV2_gc;
    ADC0.CTRLC = (4 << ADC_TIMEBASE_gp) | ADC_REFSEL_VDD_gc;
    ADC0.CTRLE = 64;
    ADC0.CTRLF = ADC_FREERUN_bm | ADC_LEFTADJ_bm;
    ADC0.MUXPOS = ADC_MUXPOS_AIN2_gc;
    ADC0.COMMAND = ADC_MODE_SINGLE_8BIT_gc | ADC_START_IMMEDIATE_gc;
}

/**
 * @brief Get the playback duration based on the position of a potentiometer.
 * 
 * The value from the ADC (analog-to-digital converter) is interpolated to a duration between 250 and 2000 milliseconds.
 * 
 * @return The playback duration in milliseconds
 */
uint16_t get_duration(void)
{
    uint16_t result = ADC0.RESULT;
    uint16_t duration = (result * ADC_MULTI) + ((uint16_t)(result * ADC_MULTI) >> 8) + 250;

    return duration;
}