#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>

#include "display.h"
#include "timer.h"

uint32_t student_number = 0x12345678; // ! Change to actual student number for final

volatile uint8_t btn_segs[] = {
    0xBE, 0xEB, 0x3E, 0x6B};
uint8_t pins[] = {PIN4_bm, PIN5_bm, PIN6_bm, PIN7_bm};

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
    uint32_t state = student_number;
    for (uint16_t i = 0; i < len; i++)
    {
        uint8_t step = generate_step(&state);
        uint16_t duration = get_duration();

        spi_write(btn_segs[step]); // Show step on display
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
    uint32_t state = student_number;
    for (uint16_t i = 0; i < len; i++)
    {
        uint8_t step = generate_step(&state);
        printf("step: %d ", step);

        int8_t button = -1;
        while (button == -1)
        {
            for (uint8_t i = 0; i < 4; i++)
            {
                if (!(VPORTA.IN & pins[i]))
                {
                    button = i;
                    break;
                }
            }
        }

        while (!(VPORTA.IN & pins[button]))
            ;

        printf("%d\n", button);

        spi_write(btn_segs[button]); // Show step on display
        // Play sound
        delay_ms(get_duration()); // TODO: Either duration or length of button press

        spi_write(0xFF); // Clear display
        // Stop sound

        if (step != button)
        {
            student_number = state;
            return 0;
        }
    }
    printf("\n");
    return 1;
}

void success_pattern(uint16_t len)
{
    set_digits(8, 8);
    TCB0.CTRLA = TCB_ENABLE_bm; // Enable timer
    delay_ms(get_duration());

    uint8_t digit1 = 11;
    if (len > 9)
        digit1 = len % 100 / 10;
    uint8_t digit2 = len % 10;

    set_digits(digit1, digit2);
    delay_ms(get_duration());

    TCB0.CTRLA = 0; // Disable timer
}

void fail_pattern(uint16_t len)
{
    set_digits(10, 10);
    TCB0.CTRLA = TCB_ENABLE_bm; // Enable timer
    delay_ms(get_duration());

    uint8_t digit1 = 11;
    if (len > 9)
        digit1 = len % 100 / 10;
    uint8_t digit2 = len % 10;

    set_digits(digit1, digit2);
    delay_ms(get_duration());

    TCB0.CTRLA = 0; // Disable timer
}
