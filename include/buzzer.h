#include <avr/io.h>
#include <avr/interrupt.h>

#define TONE1_FREQ 304
#define TONE2_FREQ 256
#define TONE3_FREQ 406
#define TONE4_FREQ 152

void buzzer_init(void);
void play_tone(uint16_t freq);
void stop_tone(void);
