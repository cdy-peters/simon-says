#include <stdint.h>

extern volatile uint8_t segs[];

/**
 * @brief Initialize the SPI (Serial Peripheral Interface) control registers.
 * 
 * The SPI route is set to alternate mode 1.
 * The SPI host/client operation is set to master mode.
 * The SPI Slave Select Disable (SSD) mode is enabled.
 * SPI is then enabled, along with SPI interrupts.
 */
void spi_init(void);

/**
 * @brief Write a byte to the SPI (Serial Peripheral Interface).
 * 
 * This function writes a single byte to the SPI data register (DATA) to initiate a transmission
 * over the SPI interface. The byte passed as a parameter is sent over the SPI bus.
 *
 * @param b: The byte to be transmitted over SPI.
 */
void spi_write(uint8_t data);