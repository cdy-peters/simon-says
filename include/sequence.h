#include <stdint.h>

/**
 * @brief Display a sequence of tones and corresponding segments.
 *
 * A sequence of steps are generated iteratively, calling the `generate_step` function.
 * For each step, a tone is played and the corresponding segments are displayed.
 *
 * @param len: The amount of steps in the sequence to be displayed.
 */
void display_sequence(uint16_t len);

/**
 * @brief Perform the sequence game logic.
 *
 * A sequence of steps are generated iteratively, calling the `generate_step` function.
 * For each step, the program waits for the user to press the push button.
 * The pressed button is then compared to the generated step, and the game state is updated accordingly.
 *
 * @param len: The amount of steps in the sequence to be played.
 */
void perform_sequence(uint16_t len);

/**
 * @brief Generate a step using a linear-feedback shift register (LFSR).
 *
 * This function generates a step using a linear-feedback shift register (LFSR) algorithm.
 * Shifting `lfsr_state` to the right by one bit and applying an XOR operation with
 * a feedback polynomial if the least significant bit is set.
 *
 * @param lfsr_state: Pointer to the LFSR state.
 * @return The generated step, represented by the two least significant bits of the LFSR state.
 */
uint8_t generate_step(uint32_t *lfsr_state);

/**
 * @brief Display the score on the seven-segment display.
 *
 * The two least significant digits of the score (`len`) are displayed on the seven-segment display.
 *
 * @param len: The length of the sequence.
 */
void display_score(uint16_t len);