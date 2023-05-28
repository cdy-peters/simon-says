#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>

#include "spi.h"
#include "buzzer.h"

#define SEGS_EF 0b00111110
#define SEGS_BC 0b01101011
#define SEGS_G 0b01110111
#define SEGS_ON 0b00000000
#define SEGS_OFF 0b01111111

typedef enum
{
    WAIT,
    BTN1,
    BTN2,
    BTN3,
    BTN4,
    SUCCESS,
    FAIL
} STATES;

extern volatile uint8_t pb_debounced;
volatile uint8_t segs[] = {SEGS_OFF, SEGS_OFF};
uint8_t score_segs[] = {
    0x08, 0x6B, 0x44, 0x41, 0x23, 0x11, 0x10, 0x4B, 0x00, 0x01};
uint32_t seed = 0x12345678; // ! Change to actual student number for final

void delay_ms(uint16_t ms);
uint16_t get_duration();
uint8_t generate_step(uint32_t *lfsr_state);
void set_tone_freq(uint16_t freq);
void display_score(uint16_t len);

void display_sequence(uint16_t len)
{
    uint32_t lfsr_state = seed;

    for (uint16_t i = 0; i < len; i++)
    {
        uint16_t duration = get_duration();
        uint8_t step = generate_step(&lfsr_state);

        switch (step)
        {
        case 0:
            set_tone_freq(TONE1_FREQ);
            segs[0] = SEGS_EF;
            break;
        case 1:
            set_tone_freq(TONE2_FREQ);
            segs[0] = SEGS_BC;
            break;
        case 2:
            set_tone_freq(TONE3_FREQ);
            segs[1] = SEGS_EF;
            break;
        case 3:
            set_tone_freq(TONE4_FREQ);
            segs[1] = SEGS_BC;
            break;
        default:
            break;
        }

        delay_ms(duration / 2);

        // Turn off buzzer and display
        TCA0.SINGLE.CTRLA = ~TCA_SINGLE_ENABLE_bm;

        segs[0] = SEGS_OFF;
        segs[1] = SEGS_OFF;

        delay_ms(duration / 2);
    }
}

uint8_t perform_sequence(uint16_t len)
{
    uint16_t counter = 0;
    uint32_t lfsr_state = seed;
    uint8_t step = generate_step(&lfsr_state);

    STATES pb_state = WAIT;

    uint8_t pb_sample = 0xFF;
    uint8_t pb_sample_r = 0xFF;
    uint8_t pb_changed, pb_rising, pb_falling;

    while (1)
    {
        pb_sample_r = pb_sample;
        pb_sample = pb_debounced;
        pb_changed = pb_sample ^ pb_sample_r;

        pb_falling = pb_changed & pb_sample_r;
        pb_rising = pb_changed & pb_sample;

        switch (pb_state)
        {
        case WAIT:
            TCA0.SINGLE.CTRLA = ~TCA_SINGLE_ENABLE_bm;

            segs[0] = SEGS_OFF;
            segs[1] = SEGS_OFF;

            if (pb_falling & PIN4_bm)
                pb_state = BTN1;
            else if (pb_falling & PIN5_bm)
                pb_state = BTN2;
            else if (pb_falling & PIN6_bm)
                pb_state = BTN3;
            else if (pb_falling & PIN7_bm)
                pb_state = BTN4;

            break;
        case BTN1:
            set_tone_freq(TONE1_FREQ);

            segs[0] = SEGS_EF;

            if (pb_rising & PIN4_bm)
                pb_state = step == 0 ? SUCCESS : FAIL;

            break;
        case BTN2:
            set_tone_freq(TONE2_FREQ);

            segs[0] = SEGS_BC;

            if (pb_rising & PIN5_bm)
                pb_state = step == 1 ? SUCCESS : FAIL;

            break;
        case BTN3:
            set_tone_freq(TONE3_FREQ);

            segs[1] = SEGS_EF;

            if (pb_rising & PIN6_bm)
                pb_state = step == 2 ? SUCCESS : FAIL;

            break;
        case BTN4:
            set_tone_freq(TONE4_FREQ);

            segs[1] = SEGS_BC;

            if (pb_rising & PIN7_bm)
                pb_state = step == 3 ? SUCCESS : FAIL;

            break;
        case SUCCESS:
            delay_ms(get_duration());

            TCA0.SINGLE.CTRLA = ~TCA_SINGLE_ENABLE_bm;

            counter++;
            if (counter == len)
            {
                display_score(len);
                delay_ms(get_duration());

                // Success pattern
                segs[0] = SEGS_ON;
                segs[1] = SEGS_ON;
                delay_ms(get_duration());

                segs[0] = SEGS_OFF;
                segs[1] = SEGS_OFF;

                return 1;
            }
            else
            {
                step = generate_step(&lfsr_state);
                pb_state = WAIT;
            }
            break;
        case FAIL:
            delay_ms(get_duration());

            TCA0.SINGLE.CTRLA = ~TCA_SINGLE_ENABLE_bm;

            display_score(len);
            delay_ms(get_duration());

            // Fail pattern
            segs[0] = SEGS_G;
            segs[1] = SEGS_G;
            delay_ms(get_duration());

            segs[0] = SEGS_OFF;
            segs[1] = SEGS_OFF;

            seed = lfsr_state; // TODO: Doesn't seem to work

            return 0;
        default:
            pb_state = WAIT;
            break;
        }
    }

    return 1;
}

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

uint8_t generate_step(uint32_t *lfsr_state)
{
    uint8_t bit = *lfsr_state & 1;
    *lfsr_state >>= 1;

    if (bit)
        *lfsr_state ^= 0xE2023CAB;

    return *lfsr_state & 0b11;
}

void set_tone_freq(uint16_t freq)
{
    TCA0.SINGLE.PERBUF = 3333333 / freq;
    TCA0.SINGLE.CMP0BUF = freq >> 1;
    TCA0.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm;
}

void display_score(uint16_t len)
{
    // Score
    segs[0] = SEGS_OFF;
    if (len > 9)
        segs[0] = score_segs[len % 100 / 10];
    segs[1] = score_segs[len % 10];
}
