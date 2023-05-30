#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>

#include "types.h"
#include "timers.h"

extern volatile uint32_t init_seed;
extern volatile uint32_t seed;
extern volatile uint16_t sequence_len;

extern volatile uint8_t octave;

extern volatile STATES state;
extern volatile uint8_t pb_released;

extern volatile GAME_STATE game_state;
volatile SERIAL_STATE serial_state = AWAITING_COMMAND;
volatile uint8_t chars_received = 0;
extern volatile char name[20];

int uart_putc_printf(char c, FILE *stream);

static FILE mystdout = FDEV_SETUP_STREAM(uart_putc_printf, NULL, _FDEV_SETUP_WRITE);

void uart_init(void)
{
    USART0.BAUD = 1389; // 9600 baud @ 3.3 MHz
    USART0.CTRLA = USART_RXCIE_bm;
    USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm; // Enable Tx/Rx
    stdout = &mystdout;
}

void uart_putc(uint8_t c)
{
    while (!(USART0.STATUS & USART_DREIF_bm))
        ; // Wait for TXDATA empty
    USART0.TXDATAL = c;
}

int uart_putc_printf(char c, FILE *stream)
{
    uart_putc(c);
    return 0;
}

uint8_t hexchar_to_int(char c)
{
    if ('0' <= c && c <= '9')
        return c - '0';
    else if ('a' <= c && c <= 'f')
        return 10 + c - 'a';
    else
        return 16; // Invalid
}

ISR(USART0_RXC_vect)
{
    static uint16_t payload = 0;
    static uint8_t payload_valid = 1;

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
            seed = init_seed;
            state = RESET;
            break;
        case '9':
        case 'o':
            payload_valid = 1;
            chars_received = 0;
            payload = 0;
            serial_state = AWAITING_PAYLOAD;
            break;
        default:
            break;
        }
        break;
    case AWAITING_PAYLOAD:
    {
        uint8_t parsed_result = hexchar_to_int((char)rx_data);
        if (parsed_result != 16)
            payload = (payload << 4) | parsed_result;
        else
            payload_valid = 0;

        if (++chars_received == 8)
        {
            init_seed = payload_valid ? payload : seed;
            serial_state = AWAITING_COMMAND;
        }
        break;
    }
    case AWAITING_NAME:
        if (rx_data == '\r')
        {
            game_state = SET_NAME;
            serial_state = AWAITING_COMMAND;
        }
        else
        {
            // ? if chars_received = 20, should state be updated?
            // ? Or should it still wait for 5 seconds and truncate the name?
            name[chars_received] = rx_data;
            chars_received++;
            elapsed_time = 0;
        }

        break;
    default:
        break;
    }
}