#include <stdint.h>

extern volatile uint16_t elapsed_time;
extern volatile uint8_t pb_debounced;

/**
 * @brief Initialize the timers for push button handling (TCB0) and general timing (TCB1).
 * @note TCB0 interval is 10ms, TCB1 interval is 1ms.
 */
void timers_init(void);

/**
 * @brief Debounce the push button input.
 *
 * This function implements a debounce algorithm to stabilize the push button input,
 * filtering out any short-term fluctuations or bounces.
 *
 * @note The debounced state is stored in the `pb_debounced` variable.
 */
void pb_debounce(void);

/**
 * @brief Delay the execution for a specified number of milliseconds.
 *
 * This function provides a blocking delay that waits until the `elapsed_time` variable, which is set to 0,
 * equals the specified number of milliseconds (`ms`) before returning from the function.
 *
 * @note The `elapsed_time` variable is incremented every millisecond by the TCB1 interrupt service routine.
 *
 * @param ms: The number of milliseconds to delay.
 */
void delay_ms(uint16_t ms);