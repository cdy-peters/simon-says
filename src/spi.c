/**
 * @file spi.c
 * @brief SPI (Serial Peripheral Interface) functions.
 */

#include "spi.h"
#include "headers.h"

volatile uint8_t segs[] = {SEGS_OFF, SEGS_OFF};

void spi_init(void)
{
    PORTMUX.SPIROUTEA = PORTMUX_SPI0_ALT1_gc; /** Set route to alternate mode 1 */
    SPI0.CTRLA = SPI_MASTER_bm;               /** Set host/client operation to master mode */
    SPI0.CTRLB = SPI_SSD_bm;                  /** Enable Slave Select Disable */
    SPI0.CTRLA |= SPI_ENABLE_bm;              /** Enable SPI */
    SPI0.INTCTRL = SPI_IE_bm;                 /** Enable SPI interrupts */
}

void spi_write(uint8_t b)
{
    SPI0.DATA = b;
}

/**
 * @brief SPI0 interrupt service routine.
 *
 * This ISR is triggered when an interrupt occurs on the SPI0 interface via the `spi_write` function.
 * The ISR drives the DISP LATCH pin high and then low to latch the display data.
 */
ISR(SPI0_INT_vect)
{
    PORTA.OUTCLR = PIN1_bm;
    PORTA.OUTSET = PIN1_bm;

    SPI0.INTFLAGS = SPI_IF_bm;
}