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
    // Push button handler
    TCB0.CCMP = 33333;          // Set interval for 10ms (33333 clocks @ 3.3 MHz)
    TCB0.INTCTRL = TCB_CAPT_bm; // CAPT interrupt enable
    TCB0.CTRLA = TCB_ENABLE_bm; // Enable

    // Generic Timer
    TCB1.CCMP = 3333; // Set interval for 1ms (3333 clocks @ 3.3 MHz)
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
 * This ISR (Interrupt Service Routine) is triggered when an interrupt occurs on the TCB0 interface,
 * which is responsible for handling the push button. Within the ISR, it calls the `pb_debounce` function
 * to debounce the push button input. It also toggles the `digit` variable to alternate between displaying
 * two different segments on the SPI bus using the `spi_write` function. Afterward, it acknowledges the TCB0
 * interrupt by clearing the interrupt flag in the TCB0 interrupt flags register (INTFLAGS).
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
 * This ISR (Interrupt Service Routine) is triggered when an interrupt occurs on the TCB1 interface,
 * which is responsible for the timer functionality. Within the ISR, it increments the `elapsed_time`
 * variable to track the elapsed time in milliseconds. Afterward, it acknowledges the TCB1 interrupt by clearing the
 * interrupt flag in the TCB1 interrupt flags register (INTFLAGS).
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