#include <stdint.h>
#include <avr/io.h>

volatile int8_t octave = 0;

void buzzer_init(void)
{
    TCA0.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm;
    TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_SINGLESLOPE_gc | TCA_SINGLE_CMP0EN_bm;
}

void play_tone(uint16_t freq)
{
    uint16_t period = 3333333 / freq;
    TCA0.SINGLE.PERBUF = period >> (octave + 2);
    TCA0.SINGLE.CMP0BUF = TCA0.SINGLE.PERBUF >> 1;
}

void stop_tone(void)
{
    TCA0.SINGLE.CMP0BUF = 0;
}