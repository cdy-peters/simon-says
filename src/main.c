#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>

#include "qutyserial.h"
#include "sequence.h"
#include "spi.h"
#include "timer.h"
#include "buzzer.h"

#define MAX_SEQUENCE_LEN 65535

void pins_init();
void adc_init();

int main(void)
{
    uint16_t sequence_len = 1;

    cli();
    serial_init(); // ! Remove for final
    pins_init();
    adc_init();
    spi_init();
    timer_init();
    buzzer_init();
    sei();

    while (1)
    {
        display_sequence(sequence_len);
        if (!perform_sequence(sequence_len))
        {
            printf("Sequence failed\n");
            sequence_len = 1;
        }
        else
        {
            printf("Sequence success\n");
            sequence_len++;
        }

        if (sequence_len == (uint32_t)MAX_SEQUENCE_LEN)
        {
            printf("Sequence length limit reached\n");
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
