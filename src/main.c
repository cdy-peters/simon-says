#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>

#include "sequence.h"
#include "spi.h"
#include "timers.h"
#include "buzzer.h"
#include "uart.h"
#include "types.h"

#define MAX_SEQUENCE_LEN 65535

volatile uint32_t seed = 0x12345678; // ! Change to actual student number for final

void pins_init();
void adc_init();

int main(void)
{
    uint16_t sequence_len = 1;

    cli();
    pins_init();
    adc_init();
    spi_init();
    timers_init();
    buzzer_init();
    uart_init();
    sei();

    get_duration(); // ! First call always returns 0

    while (1)
    {
        display_sequence(sequence_len);
        if (!perform_sequence(sequence_len))
            sequence_len = 1;
        else
            sequence_len++;

        if (sequence_len == (uint32_t)MAX_SEQUENCE_LEN)
        {
            // TODO: Handle this case
            sequence_len = 1;
        }
    }
}

void pins_init()
{
    // Buttons
    PORTA.PIN4CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN5CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN6CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN7CTRL = PORT_PULLUPEN_bm;

    // Buzzer
    PORTB.DIRSET = PIN0_bm;

    // USART0 TXD
    PORTB.DIRSET = PIN2_bm;
}

void adc_init()
{
    ADC0.CTRLA = ADC_ENABLE_bm;
    ADC0.CTRLB = ADC_PRESC_DIV2_gc;
    ADC0.CTRLC = (4 << ADC_TIMEBASE_gp) | ADC_REFSEL_VDD_gc;
    ADC0.CTRLE = 64;
    ADC0.CTRLF = ADC_FREERUN_bm | ADC_LEFTADJ_bm;
    ADC0.MUXPOS = ADC_MUXPOS_AIN2_gc;
    ADC0.COMMAND = ADC_MODE_SINGLE_8BIT_gc | ADC_START_IMMEDIATE_gc;
}
