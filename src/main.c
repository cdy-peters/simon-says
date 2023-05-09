#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>

#include "qutyserial.h"
#include "sequence.h"
#include "display.h"

// Constants
#define MAX_SEQUENCE_LEN 10 // ! Change to 2^16 - 1 for final

void main(void)
{

    serial_init();

    uint16_t sequence_len = 1;
    uint8_t valid = 1;

    spi_init();      // Initialise SPI
    spi_write(0xFF); // Clear display

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
