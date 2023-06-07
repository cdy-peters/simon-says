/**
 * @file timers.c
 * @brief Timer functions.
 */

#include "headers.h"

#include "spi.h"
#include "timers.h"

volatile uint8_t pb_debounced = 0xFF;
volatile uint16_t elapsed_time = 0;

/**
 * @brief Initialize the timers for push button handling and general timing.
 *
 * This function configures two Timer Counter B (TCB) instances. The first TCB instance (TCB0)
 * is used for the handling the push buttons, and the second TCB instance (TCB1) is used for general timing.
 * For the push button timer (TCB0), it sets the compare value (CCMP) to 33333, corresponding to an
 * interval of 10ms at a clock frequency of 3.3 MHz. It enables the CAPT interrupt and enables the TCB0.
 * For the generic timer (TCB1), it sets the compare value (CCMP) to 3333, corresponding to an
 * interval of 1ms at a clock frequency of 3.3 MHz. It also enables the CAPT interrupt and enables the TCB1.
 */
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

/**
 * @brief Debounce the push button input.
 * 
 * This function implements a debounce algorithm to stabilize the push button input. It uses
 * a static variable `vcount0` and `vcount1` to keep track of the previous and current button
 * states. The function reads the current state of the push button (`pb_sample`) and calculates
 * the changed bits between the current state and the debounced state (`pb_changed`). It updates
 * the `vcount0` and `vcount1` variables based on the changed bits. Finally, it updates the debounced
 * state (`pb_debounced`) by XORing the changed bits with the `vcount0` and `vcount1` values.
 * The debounced state represents a stable state of the push button input, filtering out any
 * short-term fluctuations or bounces.
 */
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

/**
 * @brief Delay the execution for a specified number of milliseconds.
 *
 * This function provides a blocking delay by using the `elapsed_time` variable to track the elapsed time.
 * It resets the `elapsed_time` variable to 0 and then enters a loop that continues until the elapsed time
 * reaches the specified number of milliseconds (`ms`). During the delay, the function waits and does nothing.
 * Once the desired delay is achieved, the function returns, allowing the program execution to continue.
 *
 * @param                   ms: The number of milliseconds to delay.
 */
void delay_ms(uint16_t ms)
{
    elapsed_time = 0;
    while (elapsed_time < ms)
        ;
}