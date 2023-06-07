#include <stdint.h>

/**
 * @brief Initialize the ADC (analog-to-digital converter) control registers.
 * @note The ADC samples a potentiometer at 8-bit resolution.
 */
void adc_init(void);

/**
 * @brief Get the playback duration based on the position of a potentiometer.
 *
 * The value from the ADC (analog-to-digital converter) is interpolated to a duration between 250 and 2000 milliseconds.
 *
 * @return The playback duration in milliseconds
 */
uint16_t get_duration(void);