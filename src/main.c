#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>

#include "sequence.h"
#include "display.h"
#include "timer.h"
#include "uart.h"

#define MAX_SEQUENCE_LEN 65535

void pins_init();
void adc_init();

int main(void)
{
    uint16_t sequence_len = 1;

    uart_init();
    adc_init();
    pins_init();
    spi_init();
    timer_init();

    spi_write(0xFF); // Clear display

    while (1)
    {
        display_sequence(sequence_len);
        if (!perform_sequence(sequence_len))
        {
            fail_pattern(sequence_len);
            sequence_len = 1;
        }
        else
        {
            success_pattern(sequence_len);
            sequence_len++;
        }

        if (sequence_len == (uint32_t)MAX_SEQUENCE_LEN)
        {
            // ? Does a pattern show here?
            uart_puts("Sequence length limit reached\n");
            sequence_len = 1;
        }
    }
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

void pins_init()
{
    // Configure pins as inputs
    PORTA.DIRCLR = PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm;

    // Enable pull-up resistors
    PORTA.PIN4CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN5CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN6CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN7CTRL = PORT_PULLUPEN_bm;
}