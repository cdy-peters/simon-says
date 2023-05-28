#include <avr/io.h>

#include <stdint.h>

void uart_init(void)
{
    PORTB.DIRSET = PIN2_bm;                       // Enable PB2 as output (USART0 TXD)
    USART0.BAUD = 1389;                           // 9600 baud @ 3.3 MHz
    USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm; // Enable Tx/Rx
}

void uart_putc(uint8_t c)
{
    while (!(USART0.STATUS & USART_DREIF_bm))
        ; // Wait for TXDATA empty
    USART0.TXDATAL = c;
}

void uart_puts(char *string)
{
    while (*string)
        uart_putc(*string++);
}

void uart_putd(uint16_t d)
{
    char buf[5];
    uint8_t i = 0;

    do
    {
        buf[i++] = d % 10 + '0';
        d /= 10;
    } while (d);

    while (i)
        uart_putc(buf[--i]);
}