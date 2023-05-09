#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>

#include "qutyio.h"
#include "sequence.h"

// Constants
#define MAX_SEQUENCE_LEN 10 // ! Change to 2^16 - 1 for final

int main(void)
{
    serial_init();

    uint16_t sequence_len = 1;
    uint8_t valid = 1;

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

    return 0;
}
