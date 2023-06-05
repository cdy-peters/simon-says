#include "headers.h"

volatile uint8_t segs[] = {SEGS_OFF, SEGS_OFF};

void spi_init(void)
{
    PORTMUX.SPIROUTEA = PORTMUX_SPI0_ALT1_gc; // SPI pins on PC0-3
    PORTC.DIR |= (PIN0_bm | PIN2_bm);         // Set SCK (PC0) and MOSI (PC2) as outputs
    PORTB.DIR |= PIN1_bm;                     // Set DISP EN (PB1) as output
    PORTB.OUT |= PIN1_bm;                     // Drive DISP EN high
    PORTA.DIR |= PIN1_bm;                     // Set DISP LATCH (PA1) as output

    SPI0.CTRLA = SPI_MASTER_bm;  // Master, /4 prescaler, MSB first
    SPI0.CTRLB = SPI_SSD_bm;     // Mode 0, client select disable, unbuffered
    SPI0.CTRLA |= SPI_ENABLE_bm; // Enable
    SPI0.INTCTRL = SPI_IE_bm;
}

void spi_write(uint8_t b)
{
    SPI0.DATA = b; // Note DATA register used for both Tx and Rx
}

ISR(SPI0_INT_vect)
{
    PORTA.OUTCLR = PIN1_bm; // Drive DISP LATCH high
    PORTA.OUTSET = PIN1_bm; // Drive DISP LATCH low

    SPI0.INTFLAGS = SPI_IF_bm; // Acknowledge interrupt
}