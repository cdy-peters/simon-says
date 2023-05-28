#include <avr/io.h>
#include <avr/interrupt.h>

#include "timers.h"
#include "spi.h"

extern uint8_t segs[];

volatile uint8_t pb_debounced = 0xFF;
volatile uint16_t elapsed_time = 0;
volatile uint16_t playback_time = 2000;
volatile uint16_t new_playback_time = 2000;
volatile uint8_t allow_updating_playback_delay = 0;

void timers_init(void)
{
    TCB0.CCMP = 33333;          // Set interval for 10ms (33333 clocks @ 3.3 MHz)
    TCB0.INTCTRL = TCB_CAPT_bm; // CAPT interrupt enable
    TCB0.CTRLA = TCB_ENABLE_bm; // Enable

    TCB1.CCMP = 3333;
    TCB1.INTCTRL = TCB_CAPT_bm;
    TCB1.CTRLA = TCB_ENABLE_bm;
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

// Push button
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

// Timer
ISR(TCB1_INT_vect)
{
    elapsed_time++;

    if (allow_updating_playback_delay)
    {
        playback_time = new_playback_time;
        allow_updating_playback_delay = 0;
    }

    TCB1.INTFLAGS = TCB_CAPT_bm;
}