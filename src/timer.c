#include <avr/io.h>
#include <avr/interrupt.h>

#include "display.h"

// Segs for each number, g seg and clear
volatile uint8_t score_segs[] = {
    0x08, 0x6B, 0x44, 0x41, 0x23, 0x11, 0x10, 0x4B, 0x00, 0x01, 0x77, 0xFF};
volatile uint8_t digit1 = 0;
volatile uint8_t digit2 = 0;

void timer_init(void)
{
    cli();

    TCB0.CTRLB = TCB_CNTMODE_INT_gc; // Configure TCB0 in periodic interrupt mode
    TCB0.CCMP = 3333;                // Set interval for 1ms (3333 clocks @ 3.3 MHz)
    TCB0.INTCTRL = TCB_CAPT_bm;      // CAPT interrupt enable

    sei();
}

void set_digits(uint8_t new_digit1, uint8_t new_digit2)
{
    digit1 = new_digit1;
    digit2 = new_digit2;
}

ISR(TCB0_INT_vect)
{
    static int digit = 0;

    if (digit)
    {
        spi_write(score_segs[digit1] | (0x01 << 7));
    }
    else
    {
        spi_write(score_segs[digit2]);
    }

    digit = !digit;
    TCB0.INTFLAGS = TCB_CAPT_bm;
}