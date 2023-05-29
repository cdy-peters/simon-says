#include <stdint.h>
#include <avr/io.h>

#include "types.h"

volatile int8_t octave = 0;
volatile uint16_t tones[] = {TONE1, TONE2, TONE3, TONE4};

void buzzer_init(void)
{
    TCA0.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm;
    TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_SINGLESLOPE_gc | TCA_SINGLE_CMP0EN_bm;
}

void play_tone(uint8_t i)
{
    uint16_t period = tones[i];
    TCA0.SINGLE.PERBUF = period >> (octave + 2);
    TCA0.SINGLE.CMP0BUF = TCA0.SINGLE.PERBUF >> 1;
}

void stop_tone(void)
{
    TCA0.SINGLE.CMP0BUF = 0;
}