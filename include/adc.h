#include <stdint.h>

/**
 * @brief Initialize the ADC (analog-to-digital converter) control registers.
 * 
 * The prescaler is set to divide the system clock by 2 for the shortest conversion time.
 * The timebase is set to 4 clock cycles for 1us operation at 3.3 MHz and the reference voltage is set to VDD.
 * The sample duration is set to 64 clock cycles.
 * The ADC is set to operate in free-running mode with left adjustment of the result.
 * The positive input of the ADC is selected as AIN2 (potentiometer R1).
 * The ADC is configured to start conversions immediately in single conversion mode with 8-bit resolution.
 * 
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