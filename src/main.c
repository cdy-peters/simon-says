#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>

#include "sequence.h"
#include "spi.h"
#include "timers.h"
#include "buzzer.h"
#include "uart.h"
#include "types.h"

volatile uint32_t seed = INITIAL_SEED;
volatile uint16_t sequence_len;
volatile GAME_STATE game_state;

high_score_t high_scores[5];
extern volatile SERIAL_STATE serial_state;
volatile char name[20];

void pins_init();
void adc_init();
void update_high_scores(uint16_t score);
void display_high_scores();

int main(void)
{
    game_state = DISPLAY;

    sequence_len = 1;

    cli();
    pins_init();
    adc_init();
    spi_init();
    timers_init();
    // buzzer_init();
    uart_init();
    sei();

    get_duration(); // ! First call always returns 0

    while (1)
    {
        switch (game_state)
        {
        case DISPLAY:
            display_sequence(sequence_len);
            game_state = PERFORM;
            break;
        case PERFORM:
            if (!perform_sequence(sequence_len))
                game_state = GAMEOVER;
            else
            {
                sequence_len++;
                game_state = DISPLAY;
            }
            break;
        case GAMEOVER:
            for (uint8_t i = 0; i < 5; i++)
            {
                if (sequence_len > high_scores[i].score)
                {
                    // ! Sometimes an input can't be entered
                    printf("Enter name: \n");
                    serial_state = AWAITING_NAME;
                    game_state = AWAIT_NAME;
                    break;
                }
            }

            if (game_state != AWAIT_NAME)
                game_state = DISPLAY;

            break;
        case AWAIT_NAME:
            // TODO: 5 second timeout
            break;
        case SET_NAME:
            update_high_scores(sequence_len);
            display_high_scores();

            sequence_len = 1;
            game_state = DISPLAY;
            break;
        default:
            break;
        }
    }
}

void pins_init()
{
    // Buttons
    PORTA.PIN4CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN5CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN6CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN7CTRL = PORT_PULLUPEN_bm;

    // Buzzer
    PORTB.DIRSET = PIN0_bm;

    // USART0 TXD
    PORTB.DIRSET = PIN2_bm;
}

void adc_init()
{
    ADC0.CTRLA = ADC_ENABLE_bm;
    ADC0.CTRLB = ADC_PRESC_DIV2_gc;
    ADC0.CTRLC = (4 << ADC_TIMEBASE_gp) | ADC_REFSEL_VDD_gc;
    ADC0.CTRLE = 64;
    ADC0.CTRLF = ADC_FREERUN_bm | ADC_LEFTADJ_bm;
    ADC0.MUXPOS = ADC_MUXPOS_AIN2_gc;
    ADC0.COMMAND = ADC_MODE_SINGLE_8BIT_gc | ADC_START_IMMEDIATE_gc;
}

void update_high_scores(uint16_t score)
{
    for (uint8_t i = 0; i < 5; i++)
    {
        if (score > high_scores[i].score)
        {
            for (uint8_t j = 4; j > i; j--)
            {
                high_scores[j] = high_scores[j - 1];
            }

            int name_len = strlen(name) - 1;
            for (uint8_t j = 0; j < name_len; j++)
            {
                high_scores[i].name[j] = name[j];
            }

            high_scores[i].score = score;
            break;
        }
    }
}

void display_high_scores()
{
    for (uint8_t i = 0; i < 5; i++)
    {
        if (high_scores[i].score == 0)
            break;

        // ! 5th score doesn't print correctly
        printf("%s %d\n", high_scores[i].name, high_scores[i].score);
    }
}