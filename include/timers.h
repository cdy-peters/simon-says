#include <stdint.h>

extern volatile uint16_t elapsed_time;
extern volatile uint8_t pb_debounced;

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
void timers_init(void);

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
void pb_debounce(void);

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
void delay_ms(uint16_t ms);