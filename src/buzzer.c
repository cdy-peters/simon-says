/**
 * @file buzzer.c
 * @brief Functions for controlling the buzzer.
 */

#include "headers.h"

volatile int8_t octave = 0;
volatile uint16_t tones[] = {TONE1, TONE2, TONE3, TONE4};

/**
 * @brief Initialize the buzzer using TCA0 (Timer Counter A).
 * 
 * TCA0 is enabled and configured to generate a single-slope PWM waveform.
 * WO0 is enabled via CMP0 to generate the PWM signal.
 */
void buzzer_init(void)
{
    TCA0.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm;
    TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_SINGLESLOPE_gc | TCA_SINGLE_CMP0EN_bm;
}

/**
 * @brief Play a tone on the buzzer.
 * 
 * The period and compare values of TCA0 are set to generate a specific tone
 * on the buzzer. The period buffer register (PERBUF) is set with the tone frequency,
 * and the compare buffer register (CMP0BUF) is set with half of the period value for duty cycle control.
 *
 * @param i: The index of the desired tone in the `tones` array.
 */
void play_tone(uint8_t i)
{
    TCA0.SINGLE.PERBUF = tones[i];
    TCA0.SINGLE.CMP0BUF = TCA0.SINGLE.PERBUF >> 1;
}

/**
 * @brief Stop the currently playing tone on the buzzer.
 * 
 * The compare buffer register (CMP0BUF) of TCA0 is set to 0, stopping the output waveform on the buzzer.
 */
void stop_tone(void)
{
    TCA0.SINGLE.CMP0BUF = 0;
}

/**
 * @brief Increase the frequency octave of the tones.
 *
 * @note The highest increment is 3 octaves.
 */
void inc_tones(void)
{
    if (octave > 3)
        return;

    for (uint8_t i = 0; i < 4; i++)
        tones[i] >>= 1;
    octave++;
}

/**
 * @brief Decrease the frequency octave of the tones.
 *
 * @note The lowest decrement is 2 octaves.
 */
void dec_tones(void)
{
    if (octave < -2)
        return;

    for (uint8_t i = 0; i < 4; i++)
        tones[i] <<= 1;
    octave--;
}

/**
 * @brief Reset the tones to their default values.
 */
void reset_tones(void)
{
    tones[0] = TONE1;
    tones[1] = TONE2;
    tones[2] = TONE3;
    tones[3] = TONE4;
    octave = 0;
}