#include <stdint.h>

extern volatile uint8_t segs[];

/**
 * @brief Initialize the SPI (Serial Peripheral Interface) control registers.
 */
void spi_init(void);

/**
 * @brief Write a byte to the SPI (Serial Peripheral Interface).
 *
 * This function writes a single byte to the SPI data register (DATA) to initiate a transmission
 * over the SPI interface.
 *
 * @param b: The byte to be transmitted over SPI.
 */
void spi_write(uint8_t data);