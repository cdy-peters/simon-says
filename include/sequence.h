#include <stdint.h>

uint8_t generate_step(uint32_t *state);
void display_sequence(uint16_t len);
uint8_t perform_sequence(uint16_t len);
void success_pattern(uint16_t len);
void fail_pattern(uint16_t len);