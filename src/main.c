#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>

#include "qutyserial.h"
#include "sequence.h"
#include "spi.h"
#include "timer.h"

#define MAX_SEQUENCE_LEN 65535

volatile uint8_t pb_debounced = 0xFF;

void pins_init();
void adc_init();

int main(void)
{
    uint16_t sequence_len = 1;

    serial_init();
    adc_init();
    pins_init();
    spi_init();
    timer_init();

    get_duration();  // ! First get_duration is always 0, temp solution

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

void pb_debounce(void)
{
    static uint8_t vcount0 = 0, vcount1 = 0;

    uint8_t pb_sample = PORTA.IN;
    uint8_t pb_changed = (pb_sample ^ pb_debounced);

    vcount1 = (vcount1 ^ vcount0) & pb_changed;
    vcount0 = ~vcount0 & pb_changed;

    pb_debounced ^= (vcount0 ^ vcount1);
}
