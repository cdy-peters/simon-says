#include <avr/io.h>
#include <stdint.h>

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

uint16_t get_duration(void)
{
    uint32_t result = ADC0.RESULT;
    uint32_t duration = ((result * 1750) >> 8) + 250;

    return duration;
}