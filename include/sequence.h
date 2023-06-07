#include <stdint.h>

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
void display_sequence(uint16_t len);

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
void perform_sequence(uint16_t len);

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
uint8_t generate_step(uint32_t *lfsr_state);

/**
 * @brief Display the score on the seven-segment display.
 * 
 * The sequence length is the score of the game. The two least significant digits of the score are displayed on the
 * seven-segment display. The left segment displays the tens digit, and the right segment displays the ones digit.
 * The score is displayed by setting the corresponding segment values in the `segs` array.
 *
 * @param                   len: The length of the sequence.
 */
void display_score(uint16_t len);