/**
 * @file adc.c
 * @brief ADC (analog-to-digital converter) functions.
 */

#include "adc.h"
#include "headers.h"

void adc_init(void)
{
    ADC0.CTRLA = ADC_ENABLE_bm;                                      /** Enable ADC0 */
    ADC0.CTRLB = ADC_PRESC_DIV2_gc;                                  /** Pre-scaler set to divide the system clock by 2. */
    ADC0.CTRLC = (4 << ADC_TIMEBASE_gp) | ADC_REFSEL_VDD_gc;         /** Timebase set to 4 clock cycle for 1us operation and reference voltage is set to VDD */
    ADC0.CTRLE = 64;                                                 /** Set the sampling time to 64 clock cycles. */
    ADC0.CTRLF = ADC_FREERUN_bm | ADC_LEFTADJ_bm;                    /** ADC is set to free-running mode and the result is left adjusted. */
    ADC0.MUXPOS = ADC_MUXPOS_AIN2_gc;                                /** ADC0 is set to read from AIN2. */
    ADC0.COMMAND = ADC_MODE_SINGLE_8BIT_gc | ADC_START_IMMEDIATE_gc; /** ADC is set to 8-bit single conversion mode and the conversion is started. */
}

uint16_t get_duration(void)
{
    uint16_t result = ADC0.RESULT;
    uint16_t duration = (result * ADC_MULTI) + ((uint16_t)(result * ADC_MULTI) >> 8) + 250;

    return duration;
}