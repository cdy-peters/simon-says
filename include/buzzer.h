#include <stdint.h>

/**
 * @brief Initialize the buzzer using TCA0 (Timer Counter A).
 * 
 * TCA0 is enabled and configured to generate a single-slope PWM waveform.
 * WO0 is enabled via CMP0 to generate the PWM signal.
 */
void buzzer_init(void);

/**
 * @brief Play a tone on the buzzer.
 * 
 * The period and compare values of TCA0 are set to generate a specific tone
 * on the buzzer. The period buffer register (PERBUF) is set with the tone frequency,
 * and the compare buffer register (CMP0BUF) is set with half of the period value for duty cycle control.
 *
 * @param i: The index of the desired tone in the `tones` array.
 */
void play_tone(uint8_t i);

/**
 * @brief Stop the currently playing tone on the buzzer.
 * 
 * The compare buffer register (CMP0BUF) of TCA0 is set to 0, stopping the output waveform on the buzzer.
 */
void stop_tone(void);

/**
 * @brief Increase the frequency octave of the tones.
 *
 * @note The highest increment is 3 octaves.
 */
void inc_tones(void);

/**
 * @brief Decrease the frequency octave of the tones.
 *
 * @note The lowest decrement is 2 octaves.
 */
void dec_tones(void);

/**
 * @brief Reset the tones to their default values.
 */
void reset_tones(void);