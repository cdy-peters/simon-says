#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "types.h"

extern volatile uint32_t seed;
extern volatile uint16_t sequence_len;
extern volatile uint8_t octave;
extern volatile STATES state;
extern volatile uint8_t pb_released;

void uart_init(void)
{
    USART0.BAUD = 1389; // 9600 baud @ 3.3 MHz
    USART0.CTRLA = USART_RXCIE_bm;
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

ISR(USART0_RXC_vect)
{
    static SERIAL_STATE serial_state = AWAITING_COMMAND;

    uint8_t rx_data = USART0.RXDATAL;

    switch (serial_state)
    {
    case AWAITING_COMMAND:
        switch (rx_data)
        {
        case '1':
        case 'q':
            if (state == WAIT)
            {
                state = BTN1;
                pb_released = 1;
            }
            break;
        case '2':
        case 'w':
            if (state == WAIT)
            {
                state = BTN2;
                pb_released = 1;
            }
            break;
        case '3':
        case 'e':
            if (state == WAIT)
            {
                state = BTN3;
                pb_released = 1;
            }
            break;
        case '4':
        case 'r':
            if (state == WAIT)
            {
                state = BTN4;
                pb_released = 1;
            }
            break;
        case ',':
        case 'k':
            if (octave < 3)
                octave++;
            break;
        case '.':
        case 'l':
            if (octave > -2)
                octave--;
            break;
        case '0':
        case 'p':
            octave = 0;
            seed = INITIAL_SEED;
            state = RESET;
            break;
        }
    case AWAITING_PAYLOAD:
        break;
    default:
        break;
    }
}