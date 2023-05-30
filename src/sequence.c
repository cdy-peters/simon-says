#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>

#include "spi.h"
#include "buzzer.h"
#include "timers.h"
#include "uart.h"
#include "types.h"
#include "adc.h"

extern volatile uint8_t pb_debounced;
extern volatile uint32_t seed;

volatile uint8_t segs[] = {SEGS_OFF, SEGS_OFF};
uint8_t score_segs[] = {
    0x08, 0x6B, 0x44, 0x41, 0x23, 0x11, 0x10, 0x4B, 0x00, 0x01};

volatile STATES state = WAIT;
volatile uint8_t pb_released = 0;
uint16_t duration;

void delay_ms(uint16_t ms);
uint8_t generate_step(uint32_t *lfsr_state);
void display_score(uint16_t len);

void display_sequence(uint16_t len)
{
    uint32_t lfsr_state = seed;

    for (uint16_t i = 0; i < len; i++)
    {
        duration = get_duration();
        uint8_t step = generate_step(&lfsr_state);

        switch (step)
        {
        case 0:
            play_tone(0);
            segs[0] = SEGS_EF;
            break;
        case 1:
            play_tone(1);
            segs[0] = SEGS_BC;
            break;
        case 2:
            play_tone(2);
            segs[1] = SEGS_EF;
            break;
        case 3:
            play_tone(3);
            segs[1] = SEGS_BC;
            break;
        default:
            break;
        }

        delay_ms(duration >> 1);

        // Turn off buzzer and display
        stop_tone();

        segs[0] = SEGS_OFF;
        segs[1] = SEGS_OFF;

        delay_ms(duration >> 1);
    }
}

uint8_t perform_sequence(uint16_t len)
{
    state = WAIT;

    uint16_t counter = 0;
    uint32_t lfsr_state = seed;
    uint8_t step = generate_step(&lfsr_state);

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

        allow_updating_playback_delay = 1;
        new_playback_time = get_duration();

        switch (state)
        {
        case PAUSED:
            pb_released = 0;
            break;
        case WAIT:
            pb_released = 0;
            elapsed_time = 0;
            allow_updating_playback_delay = 0;

            stop_tone();

            segs[0] = SEGS_OFF;
            segs[1] = SEGS_OFF;

            if (pb_falling & PIN4_bm)
                state = BTN1;
            else if (pb_falling & PIN5_bm)
                state = BTN2;
            else if (pb_falling & PIN6_bm)
                state = BTN3;
            else if (pb_falling & PIN7_bm)
                state = BTN4;

            break;
        case BTN1:
            play_tone(0);
            segs[0] = SEGS_EF;

            if (!pb_released)
            {
                if (pb_rising & PIN4_bm)
                    pb_released = 1;
            }
            else if (elapsed_time >= playback_time)
            {
                stop_tone();
                allow_updating_playback_delay = 1;
                state = step == 0 ? SUCCESS : FAIL;
            }

            break;
        case BTN2:
            play_tone(1);
            segs[0] = SEGS_BC;

            if (!pb_released)
            {
                if (pb_rising & PIN5_bm)
                    pb_released = 1;
            }
            else if (elapsed_time >= playback_time)
            {
                stop_tone();
                allow_updating_playback_delay = 1;
                state = step == 1 ? SUCCESS : FAIL;
            }

            break;
        case BTN3:
            play_tone(2);
            segs[1] = SEGS_EF;

            if (!pb_released)
            {
                if (pb_rising & PIN6_bm)
                    pb_released = 1;
            }
            else if (elapsed_time >= playback_time)
            {
                stop_tone();
                allow_updating_playback_delay = 1;
                state = step == 2 ? SUCCESS : FAIL;
            }

            break;
        case BTN4:
            play_tone(3);
            segs[1] = SEGS_BC;

            if (!pb_released)
            {
                if (pb_rising & PIN7_bm)
                    pb_released = 1;
            }
            else if (elapsed_time >= playback_time)
            {
                stop_tone();
                allow_updating_playback_delay = 1;
                state = step == 3 ? SUCCESS : FAIL;
            }

            break;
        case SUCCESS:
            counter++;
            if (counter == len)
            {
                // UART output
                printf("SUCCESS\n");
                printf("%d\n", len);

                display_score(len);
                delay_ms(playback_time);

                // Success pattern
                segs[0] = SEGS_ON;
                segs[1] = SEGS_ON;
                delay_ms(playback_time);

                segs[0] = SEGS_OFF;
                segs[1] = SEGS_OFF;

                state = PAUSED;
                return 1;
            }
            else
            {
                step = generate_step(&lfsr_state);
                state = WAIT;
            }
            break;
        case FAIL:
            // UART output
            printf("GAME OVER\n");
            printf("%d\n", len);

            display_score(len);
            delay_ms(playback_time);

            // Fail pattern
            segs[0] = SEGS_G;
            segs[1] = SEGS_G;
            delay_ms(playback_time);

            segs[0] = SEGS_OFF;
            segs[1] = SEGS_OFF;

            // Reset seed
            counter++;
            for (; counter < len; counter++)
                generate_step(&lfsr_state);
            seed = lfsr_state;

            state = PAUSED;
            return 0;
        case RESET:
            stop_tone();

            segs[0] = SEGS_OFF;
            segs[1] = SEGS_OFF;

            state = PAUSED;
            return 0;
        default:
            state = PAUSED;
            break;
        }
    }

    return 1;
}

void delay_ms(uint16_t ms)
{
    elapsed_time = 0;
    while (elapsed_time < ms)
        ;
}

uint8_t generate_step(uint32_t *lfsr_state)
{
    uint8_t bit = *lfsr_state & 1;
    *lfsr_state >>= 1;

    if (bit)
        *lfsr_state ^= 0xE2023CAB;

    return *lfsr_state & 0b11;
}

void display_score(uint16_t len)
{
    segs[0] = SEGS_OFF;
    if (len > 9)
        segs[0] = score_segs[len % 100 / 10];
    segs[1] = score_segs[len % 10];
}
