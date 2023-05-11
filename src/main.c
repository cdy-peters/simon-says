#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>

#include "sequence.h"
#include "display.h"
#include "timer.h"
#include "uart.h"

#define MAX_SEQUENCE_LEN 65535

typedef struct
{
    char name[20];
    uint16_t score;
} high_score_t;

void pins_init();
void adc_init();
void update_high_scores(high_score_t *high_scores, uint16_t len);
void display_high_scores(high_score_t *high_scores);

int main(void)
{
    uint16_t sequence_len = 1;
    high_score_t high_scores[5] = {
        {"", 0},
        {"", 0},
        {"", 0},
        {"", 0},
        {"", 0}};

    uart_init();
    adc_init();
    pins_init();
    spi_init();
    timer_init();

    spi_write(0xFF); // Clear display

    while (1)
    {
        display_sequence(sequence_len);
        if (!perform_sequence(sequence_len))
        {
            fail_pattern(sequence_len);

            update_high_scores(&high_scores, sequence_len);
            display_high_scores(&high_scores);

            sequence_len = 1;
        }
        else
        {
            success_pattern(sequence_len);
            sequence_len++;
        }

        if (sequence_len == (uint32_t)MAX_SEQUENCE_LEN)
        {
            // ? Does a pattern show here?
            // TODO: Handle this case
            uart_puts("Sequence length limit reached\n");
            sequence_len = 1;
        }
    }
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

void pins_init()
{
    // Configure pins as inputs
    PORTA.DIRCLR = PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm;

    // Enable pull-up resistors
    PORTA.PIN4CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN5CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN6CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN7CTRL = PORT_PULLUPEN_bm;
}

void update_high_scores(high_score_t *high_scores, uint16_t len)
{
    for (uint8_t i = 0; i < 5; i++)
    {
        if (len > high_scores[i].score)
        {
            uart_puts("Enter your name: ");
            char name[20];
            uart_gets(name);
            uart_puts("\n");

            for (uint8_t j = 4; j > i; j--)
            {
                high_scores[j] = high_scores[j - 1];
            }

            high_scores[i].score = len;
            for (uint8_t j = 0; j < 20; j++)
            {
                high_scores[i].name[j] = name[j];
            }

            break;
        }
    }
}

void display_high_scores(high_score_t *high_scores)
{
    for (uint8_t i = 0; i < 5; i++)
    {
        if (high_scores[i].score == 0)
            break;

        uart_puts(high_scores[i].name);
        uart_putc(' ');
        uart_putd(high_scores[i].score);
        uart_puts("\n");
    }
}