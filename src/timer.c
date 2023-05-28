#include "timer.h"
#include "spi.h"

extern uint8_t segs[];

volatile uint8_t pb_debounced = 0xFF;

void timer_init(void)
{
    TCB0.CCMP = 33333;               // Set interval for 10ms (33333 clocks @ 3.3 MHz)
    TCB0.INTCTRL = TCB_CAPT_bm;      // CAPT interrupt enable
    TCB0.CTRLA = TCB_ENABLE_bm;      // Enable
}

void pb_debounce(void)
{
    static uint8_t vcount0 = 0, vcount1 = 0;

    uint8_t pb_sample = PORTA.IN;
    uint8_t pb_changed = (pb_sample ^ pb_debounced);

    vcount1 = (vcount1 ^ vcount0) & pb_changed;
    vcount0 = ~vcount0 & pb_changed;

    pb_debounced ^= (vcount0 ^ vcount1);
}

ISR(TCB0_INT_vect)
{
    pb_debounce();
    static uint8_t digit = 0;

    if (digit)
        spi_write(segs[0] | (0x01 << 7));
    else
        spi_write(segs[1]);
    digit = !digit;

    TCB0.INTFLAGS = TCB_CAPT_bm;
}
