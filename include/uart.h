#include <stdint.h>

void uart_init(void);
uint8_t uart_getc(void);
void uart_gets(char *string);
void uart_putc(uint8_t c);
void uart_puts(char *string);
void uart_putd(uint16_t d);