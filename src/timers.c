/**
 * @file timers.c
 * @brief Timer functions.
 */

#include "timers.h"
#include "headers.h"

#include "spi.h"
#include "timers.h"

volatile uint8_t pb_debounced = 0xFF;
volatile uint16_t elapsed_time = 0;

void timers_init(void)
{
    TCB0.CCMP = 33333; /**< Set interval for 10ms (33333 clocks @ 3.3Mhz) */
    TCB0.INTCTRL = TCB_CAPT_bm;
    TCB0.CTRLA = TCB_ENABLE_bm;

    TCB1.CCMP = 3333; /**< Set interval for 1ms (3333 clocks @ 3.3Mhz) */
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

/**
 * @brief TCB0 interrupt service routine for push button.
 *
 * This ISR is triggered every 10 milliseconds. It calls the `pb_debounce` function to debounce the push button input
 * and also toggles the `digit` variable to alternate between displaying to the two digits on the display.
 */
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

/**
 * @brief TCB1 interrupt service routine for the generic timer.
 *
 * This ISR is triggered every millisecond, incrementing the `elapsed_time` variable to track the elapsed time in milliseconds.
 */
ISR(TCB1_INT_vect)
{
    elapsed_time++;

    TCB1.INTFLAGS = TCB_CAPT_bm;
}

void delay_ms(uint16_t ms)
{
    elapsed_time = 0;
    while (elapsed_time < ms)
        ;
}