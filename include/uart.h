#include <stdint.h>
#include <stdio.h>

extern volatile uint8_t chars_received;

/**
 * @brief Initialize the UART module for serial communication.
 *
 * @note This function also redirects the standard output stream (`stdout`) to the UART, allowing printf and
 * other standard output functions to use the UART for output.
 */
void uart_init(void);

/**
 * @brief Transmit a single character over UART.
 *
 * This function waits until the UART's transmit data empty flag (`USART_DREIF_bm`) is set,
 * indicating that the transmit data register is empty and ready to receive new data.
 * Once the flag is set, the character is written to the transmit data register (`USART0.TXDATAL`) to initiate the
 * transmission.
 *
 * @param c: The character to be transmitted.
 */
void uart_putc(uint8_t c);

/**
 * @brief Helper function for printf to transmit a character over UART.
 *
 * This function is used as a callback by the printf function to transmit a single character (`c`) over the UART.
 * It calls the `uart_putc` function to perform the actual transmission.
 *
 * @param c: The character to be transmitted.
 * @param stream: The output stream to be used. This parameter is ignored.
 * @return 0 to indicate success.
 */
int uart_putc_printf(char c, FILE *stream);

/**
 * @brief Converts a hexadecimal character to its integer value.
 *
 * @param c: The hexadecimal character to be converted.
 * @return The integer value of the hexadecimal character or 16 if the character is invalid.
 */
uint8_t hexchar_to_int(char c);