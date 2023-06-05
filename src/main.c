#include "headers.h"

#include "adc.h"
#include "buzzer.h"
#include "sequence.h"
#include "spi.h"
#include "timers.h"
#include "uart.h"

volatile uint32_t seed = INITIAL_SEED;
volatile uint32_t temp_seed;
volatile uint16_t sequence_len = 1;
volatile GAME_STATE game_state = DISPLAY;

high_score_t high_scores[5];
extern volatile SERIAL_STATE serial_state;
extern volatile uint8_t chars_received;
volatile char name[20];

void pins_init();
void update_high_scores(uint16_t score);
void display_high_scores();

int main(void)
{
    cli();
    pins_init();
    adc_init();
    spi_init();
    timers_init();
    buzzer_init();
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
            perform_sequence(sequence_len);
            break;
        case GAMEOVER:
            for (uint8_t i = 0; i < 5; i++)
            {
                if (sequence_len > high_scores[i].score)
                {
                    printf("Enter name: ");
                    serial_state = AWAITING_NAME;
                    game_state = AWAIT_NAME;
                    break;
                }
            }

            if (game_state != AWAIT_NAME)
            {
                display_high_scores();
                game_state = DISPLAY;
            }

            if (temp_seed)
            {
                seed = temp_seed;
                temp_seed = 0;
            }

            break;
        case AWAIT_NAME:
            if (elapsed_time > 5000)
            {
                serial_state = AWAITING_COMMAND;
                game_state = SET_NAME;
            }
            break;
        case SET_NAME:
            name[chars_received] = '\0';

            update_high_scores(sequence_len);
            display_high_scores();

            chars_received = 0;
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

void update_high_scores(uint16_t score)
{
    for (uint8_t i = 0; i < 5; i++)
    {
        if (score > high_scores[i].score)
        {
            for (uint8_t j = 4; j > i; j--)
                high_scores[j] = high_scores[j - 1];

            for (uint8_t j = 0; j < 20; j++)
                high_scores[i].name[j] = name[j];
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

        printf("%s %d\n", high_scores[i].name, high_scores[i].score);
    }
}