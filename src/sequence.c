#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>

#include "display.h"

#define STUDENT_NUMBER 0x12345678 // ! Change to actual student number for final

volatile uint8_t segs[] = {
    0xBE, 0xEB, 0x3E, 0x6B};

void delay_ms(uint16_t ms)
{
    for (uint16_t i = 0; i < ms; i++)
        _delay_ms(1);
}

uint16_t get_duration()
{
    uint16_t result = ADC0.RESULT;
    uint16_t duration = result / 255.0 * 1750 + 250;
    return duration;
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
        uint16_t duration = get_duration();

        spi_write(segs[step]); // Show step on display
        // Play sound
        delay_ms(duration / 2);

        spi_write(0xFF); // Clear display
        // Stop sound
        delay_ms(duration / 2);

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

        int8_t button = -1;
        while (button == -1)
        {
            if (!(VPORTA.IN & PIN4_bm))
            {
                button = 0;
            }
            else if (!(VPORTA.IN & PIN5_bm))
            {
                button = 1;
            }
            else if (!(VPORTA.IN & PIN6_bm))
            {
                button = 2;
            }
            else if (!(VPORTA.IN & PIN7_bm))
            {
                button = 3;
            }
        }

        printf("%d\n", button);

        spi_write(segs[button]); // Show step on display
        // Play sound
        delay_ms(get_duration()); // TODO: Either duration or length of button press

        spi_write(0xFF); // Clear display
        // Stop sound

        if (step != button)
            return 0;
    }
    printf("\n");
    return 1;
}
