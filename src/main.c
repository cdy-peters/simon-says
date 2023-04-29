// #include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
// #include "qutyio.h"

// Define bool type
typedef uint8_t bool;
#define true 1
#define false 0

// Constants
#define STUDENT_NUMBER 0x12345678 // ! Change to actual student number for final
#define MAX_SEQUENCE_LEN 10       // ! Change to 2^16 - 1 for final

// Function prototypes
uint8_t generate_step(uint32_t *state);
void display_sequence(uint16_t len);
bool perform_sequence(uint16_t len);

int main(void)
{
    // serial_init();

    uint16_t sequence_len = 1;
    bool valid = true;

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

uint8_t generate_step(uint32_t *state)
{
    uint8_t bit = *state & 1;
    *state >>= 1;

    if (bit)
        *state ^= 0xE2023CAB;

    return *state & 0b11;
}

void display_sequence(uint16_t len)
{
    uint32_t state = STUDENT_NUMBER;
    for (uint16_t i = 0; i < len; i++)
    {
        uint8_t step = generate_step(&state);
        // Show step on display
        // Play sound
        printf("%d ", step);
    }
    printf("\n");
}

bool perform_sequence(uint16_t len)
{
    uint32_t state = STUDENT_NUMBER;
    for (uint16_t i = 0; i < len; i++)
    {
        uint8_t step = generate_step(&state);
        // Wait for corresponding button press, return false if incorrect
        printf("%d ", step);
    }
    printf("\n");
    return true;
}