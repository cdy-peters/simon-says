/**
 * @file buzzer.c
 * @brief Functions for controlling the buzzer.
 */

#include "buzzer.h"
#include "headers.h"

volatile int8_t octave = 0;
volatile uint16_t tones[] = {TONE1, TONE2, TONE3, TONE4};

void buzzer_init(void)
{
    TCA0.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm;                                    /* TCA0 is enabled and configured to generate a single-slope PWM waveform. */
    TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_SINGLESLOPE_gc | TCA_SINGLE_CMP0EN_bm; /* WO0 is enabled via CMP0 to generate the PWM signal. */
}

void play_tone(uint8_t i)
{
    TCA0.SINGLE.PERBUF = tones[i];                 /* Set with the 3333333 divided by tone frequency */
    TCA0.SINGLE.CMP0BUF = TCA0.SINGLE.PERBUF >> 1; /* Set with half of the period value for duty cycle control. */
}

void stop_tone(void)
{
    TCA0.SINGLE.CMP0BUF = 0; /* The compare buffer register (CMP0BUF) of TCA0 is set to 0, stopping the output waveform on the buzzer. */
}

void inc_tones(void)
{
    if (octave > 3)
        return;

    for (uint8_t i = 0; i < 4; i++)
        tones[i] >>= 1;
    octave++;
}

void dec_tones(void)
{
    if (octave < -2)
        return;

    for (uint8_t i = 0; i < 4; i++)
        tones[i] <<= 1;
    octave--;
}

void reset_tones(void)
{
    tones[0] = TONE1;
    tones[1] = TONE2;
    tones[2] = TONE3;
    tones[3] = TONE4;
    octave = 0;
}