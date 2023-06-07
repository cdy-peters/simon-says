#include <stdint.h>

/**
 * @brief Initialize the buzzer using TCA0 (Timer Counter A).
 */
void buzzer_init(void);

/**
 * @brief Play a tone on the buzzer.
 * @param i: The index (0-3) of the desired tone in the `tones` array.
 */
void play_tone(uint8_t i);

/**
 * @brief Stop the currently playing tone on the buzzer.
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