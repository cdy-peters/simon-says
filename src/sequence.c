#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>

#include "display.h"

#define STUDENT_NUMBER 0x12345678 // ! Change to actual student number for final
#define DURATION 1000             // Duration in ms // TODO: Change to read potentiometer value

volatile uint8_t segs[] = {
    0xBE, 0xEB, 0x3E, 0x6B};

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

        spi_write(segs[step]); // Show step on display
        // Play sound
        _delay_ms(DURATION / 2);

        spi_write(0xFF); // Clear display
        // Stop sound
        _delay_ms(DURATION / 2);

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
        printf("step: %d ", step);

        uint8_t button;

        while (1)
        {
            if (!(VPORTA.IN & PIN4_bm))
            {
                button = 0;
                break;
            }
            else if (!(VPORTA.IN & PIN5_bm))
            {
                button = 1;
                break;
            }
            else if (!(VPORTA.IN & PIN6_bm))
            {
                button = 2;
                break;
            }
            else if (!(VPORTA.IN & PIN7_bm))
            {
                button = 3;
                break;
            }
        }

        printf("%d\n", button);

        spi_write(segs[button]); // Show step on display
        // Play sound
        _delay_ms(DURATION); // TODO: Either duration or length of button press

        spi_write(0xFF); // Clear display
        // Stop sound

        if (step != button)
            return 0;
    }
    printf("\n");
    return 1;
}