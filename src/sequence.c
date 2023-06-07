/**
 * @file sequence.c
 * @brief Sequence game functions.
 */

#include "headers.h"

#include "adc.h"
#include "buzzer.h"
#include "spi.h"
#include "timers.h"
#include "uart.h"

extern volatile uint16_t sequence_len;
extern volatile uint32_t seed;

uint8_t score_segs[] = {
    0x08, 0x6B, 0x44, 0x41, 0x23, 0x11, 0x10, 0x4B, 0x00, 0x01};

volatile STATES state = WAIT;
volatile uint8_t pb_released = 0;
uint16_t duration;

void delay_ms(uint16_t ms);
uint8_t generate_step(uint32_t *lfsr_state);
void display_score(uint16_t len);

/**
 * @brief Display a sequence of tones and corresponding segments.
 * 
 * A linear-feedback shift register (LFSR) via the `generate_step` function is used to
 * generate a sequence of steps based on a given seed value.
 * For each step in the sequence, it plays a corresponding tone and updates the segments to be displayed.
 * After playing the tone and updating the segments, it delays for half of the duration obtained from
 * the `get_duration` function.
 * It then turns off the buzzer and resets the segments to display off for the remaining half of the duration.
 *
 * @param                   len: The amount of steps in the sequence to be displayed.
 */
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

/**
 * @brief Perform the sequence game logic.
 * 
 * It waits for button presses corresponding to the previously displayed sequence via the `display_sequence` function.
 * For each button press, it compares the pressed button to the next step in the sequence. If the button press is correct,
 * the sequence continues until the end. If the button press is incorrect, the game ends.
 * The function maintains different states to control the game flow and handle different scenarios.
 *
 * @param                   len: The amount of steps in the sequence to be played.
 */
void perform_sequence(uint16_t len)
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

        duration = get_duration();

        switch (state)
        {
        case PAUSED:
            pb_released = 0;
            break;
        case WAIT:
            pb_released = 0;
            elapsed_time = 0;

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
            else if (elapsed_time >= duration)
            {
                stop_tone();
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
            else if (elapsed_time >= duration)
            {
                stop_tone();
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
            else if (elapsed_time >= duration)
            {
                stop_tone();
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
            else if (elapsed_time >= duration)
            {
                stop_tone();
                state = step == 3 ? SUCCESS : FAIL;
            }

            break;
        case SUCCESS:
            counter++;
            if (counter == len)
            {
                duration = get_duration();

                // UART output
                printf("SUCCESS\n");
                printf("%d\n", len);

                // Success pattern
                segs[0] = SEGS_ON;
                segs[1] = SEGS_ON;
                delay_ms(duration);

                // Score
                display_score(len);
                delay_ms(duration / 2);

                segs[0] = SEGS_OFF;
                segs[1] = SEGS_OFF;
                delay_ms(duration / 2);

                sequence_len++;
                state = PAUSED;
                game_state = PLAY;
                return;
            }
            else
            {
                step = generate_step(&lfsr_state);
                state = WAIT;
            }
            break;
        case FAIL:
            duration = get_duration();

            // UART output
            printf("GAME OVER\n");
            printf("%d\n", len);

            // Fail pattern
            segs[0] = SEGS_G;
            segs[1] = SEGS_G;
            delay_ms(duration);

            // Score
            display_score(len);
            delay_ms(duration / 2);

            segs[0] = SEGS_OFF;
            segs[1] = SEGS_OFF;
            delay_ms(duration / 2);

            // Reset seed
            counter++;
            for (; counter < len; counter++)
                generate_step(&lfsr_state);
            seed = lfsr_state;

            state = PAUSED;
            game_state = GAMEOVER;
            return;
        case RESET:
            stop_tone();
            reset_tones();

            segs[0] = SEGS_OFF;
            segs[1] = SEGS_OFF;

            sequence_len = 1;
            state = PAUSED;
            game_state = PLAY;
            return;
        default:
            state = PAUSED;
            break;
        }
    }
}

/**
 * @brief Generate a step using a linear-feedback shift register (LFSR).
 * 
 * This function generates a step using a linear-feedback shift register (LFSR) algorithm. It takes a pointer to an LFSR
 * state as input and updates the state by shifting its bits to the right. The least significant bit (LSB) is used to
 * determine whether to apply an XOR operation with a feedback polynomial. The feedback polynomial used is 0xE2023CAB.
 * Finally, the function returns the two least significant bits of the updated LFSR state as the generated step.
 *
 * @param                   lfsr_state: Pointer to the LFSR state.
 * @return                  The generated step, represented by the two least significant bits of the LFSR state.
 */
uint8_t generate_step(uint32_t *lfsr_state)
{
    uint8_t bit = *lfsr_state & 1;
    *lfsr_state >>= 1;

    if (bit)
        *lfsr_state ^= 0xE2023CAB;

    return *lfsr_state & 0b11;
}

/**
 * @brief Display the score on the seven-segment display.
 * 
 * The sequence length is the score of the game. The two least significant digits of the score are displayed on the
 * seven-segment display. The left segment displays the tens digit, and the right segment displays the ones digit.
 * The score is displayed by setting the corresponding segment values in the `segs` array.
 *
 * @param                   len: The length of the sequence.
 */
void display_score(uint16_t len)
{
    if (len > 9)
    {
        uint8_t dividend = len % 100;
        uint8_t quotient = 0;

        while (dividend > 9)
        {
            dividend -= 10;
            quotient++;
        }
        segs[0] = score_segs[quotient];
    }
    else
    {
        segs[0] = SEGS_OFF;
    }

    segs[1] = score_segs[len % 10];
}
