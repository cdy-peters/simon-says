#include <avr/interrupt.h>
#include <stdint.h>

extern volatile uint16_t elapsed_time;
extern volatile uint16_t playback_time;
extern volatile uint16_t new_playback_time;
extern volatile uint8_t allow_updating_playback_delay;

void timers_init(void);
