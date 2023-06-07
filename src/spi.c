/**
 * @file spi.c
 * @brief SPI (Serial Peripheral Interface) functions.
 */

#include "spi.h"
#include "headers.h"

volatile uint8_t segs[] = {SEGS_OFF, SEGS_OFF};

void spi_init(void)
{
    PORTMUX.SPIROUTEA = PORTMUX_SPI0_ALT1_gc;
    SPI0.CTRLA = SPI_MASTER_bm;
    SPI0.CTRLB = SPI_SSD_bm;
    SPI0.CTRLA |= SPI_ENABLE_bm;
    SPI0.INTCTRL = SPI_IE_bm;
}

void spi_write(uint8_t b)
{
    SPI0.DATA = b;
}

/**
 * @brief SPI0 interrupt service routine.
 *
 * This ISR (Interrupt Service Routine) is triggered when an interrupt occurs on the SPI0 interface.
 * Within the ISR, it drives the DISP LATCH pin high and then low, using PORTA.OUTCLR and PORTA.OUTSET
 * respectively. This action can be used to latch the display data. Afterward, it acknowledges the SPI
 * interrupt by clearing the interrupt flag in the SPI0 interrupt flags register (INTFLAGS).
 */
ISR(SPI0_INT_vect)
{
    PORTA.OUTCLR = PIN1_bm;
    PORTA.OUTSET = PIN1_bm;

    SPI0.INTFLAGS = SPI_IF_bm;
}