#include <stdint.h>

void uart_init(void);
void uart_putc(uint8_t c);
void uart_puts(char* string);