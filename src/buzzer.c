#include <stdint.h>

#include <avr/io.h>

void buzzer_init(void) {
    TCA0.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm;
    TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_SINGLESLOPE_gc | TCA_SINGLE_CMP0EN_bm;
}

void play_tone(uint16_t freq)
{
    TCA0.SINGLE.PERBUF = 3333333 / freq;
    TCA0.SINGLE.CMP0BUF = freq >> 1;
}

void stop_tone(void)
{
    TCA0.SINGLE.CMP0BUF = 0;
}