#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>

#include "qutyserial.h"
#include "sequence.h"
#include "display.h"

// Constants
#define MAX_SEQUENCE_LEN 10 // ! Change to 2^16 - 1 for final


void adc_init()
{
    // Enable ADC
    ADC0.CTRLA = ADC_ENABLE_bm;
    // /2 clock prescaler
    ADC0.CTRLB = ADC_PRESC_DIV2_gc;
    // Need 4 CLK_PER cycles @ 3.3 MHz for 1us, select VDD as ref
    ADC0.CTRLC = (4 << ADC_TIMEBASE_gp) | ADC_REFSEL_VDD_gc;
    // Sample duration of 64
    ADC0.CTRLE = 64;
    // Free running, left adjust result
    ADC0.CTRLF = ADC_FREERUN_bm | ADC_LEFTADJ_bm;
    // Select AIN2 (potentiomenter R1)
    ADC0.MUXPOS = ADC_MUXPOS_AIN2_gc;
    // Select 8-bit resolution, single-ended
    ADC0.COMMAND = ADC_MODE_SINGLE_8BIT_gc | ADC_START_IMMEDIATE_gc;
}


void main(void)
{

    serial_init();

    uint16_t sequence_len = 1;
    uint8_t valid = 1;

    spi_init();      // Initialise SPI
    spi_write(0xFF); // Clear display

    adc_init();

    PORTA.PIN4CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN5CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN6CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN7CTRL = PORT_PULLUPEN_bm;

    while (valid)
    {
        if (sequence_len == (uint32_t)MAX_SEQUENCE_LEN)
            break;

        display_sequence(sequence_len);
        if (!perform_sequence(sequence_len))
            break; // Break if sequence fails

        sequence_len++;
    }
    printf("Sequence length: %d\n", sequence_len);

    while (1)
        ;
}
