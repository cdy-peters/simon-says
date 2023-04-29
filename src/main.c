// #include <avr/io.h>
#include <stdio.h>
#include <stdint.h>

// #include "qutyio.h"

uint8_t generate_step(uint32_t *state);

uint32_t student_number = 0x12345678; // ! Change to actual student number for final

int main(void)
{
    // serial_init();

    for (int i = 0; i < 10; i++)
    {
        uint8_t step = generate_step(&student_number);
        printf("%d", step);
    }
    printf("\n");

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