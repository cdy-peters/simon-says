#include <stdint.h>
#include <stdio.h>

#define STUDENT_NUMBER 0x12345678 // ! Change to actual student number for final

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

uint8_t perform_sequence(uint16_t len)
{
    uint32_t state = STUDENT_NUMBER;
    for (uint16_t i = 0; i < len; i++)
    {
        uint8_t step = generate_step(&state);
        // Wait for corresponding button press, return false if incorrect
        printf("%d ", step);
    }
    printf("\n");
    return 1;
}