#include <stdint.h>

#define INITIAL_SEED 0x12345678 /**< Initial seed for the linear-feedback shift register. */
#define MAX_SEQUENCE_LEN 65535  /**< Max sequence is a 16 bit unsigned integer */
#define ADC_MULTI 6.8359375     /**< Multiplier for ADC value, 1750 / 256 */

#define SEGS_EF 0b00111110  /**< Binary representation of the segments E and F turned on. */
#define SEGS_BC 0b01101011  /**< Binary representation of the segments B and C turned on. */
#define SEGS_G 0b01110111   /**< Binary representation of the segment G turned on. */
#define SEGS_ON 0b00000000  /**< Binary representation of all segments turned on. */
#define SEGS_OFF 0b01111111 /**< Binary representation of all segments turned off. */

#define TONE1 10959 /**< Tone E high: 3333333 / 304Hz */
#define TONE2 13033 /**< Tone C#: 3333333 / 256Hz */
#define TONE3 8210  /**< Tone A: 3333333 / 406Hz */
#define TONE4 21919 /**< Tone E low: 3333333 / 152Hz */

/**
 * @brief Structure representing a high score entry.
 */
typedef struct
{
    char name[20];  /**< The player's name. */
    uint16_t score; /**< The player's score. */
} high_score_t;

/**
 * @brief Enumeration representing the game states.
 */
typedef enum
{
    PLAY,       /**< The game is in the play state. */
    GAMEOVER,   /**< The game is in the game over state. */
    AWAIT_NAME, /**< The game is awaiting the player's name input. */
    SET_NAME    /**< The game is in the process of setting the player's name. */
} GAME_STATE;

/**
 * @brief Enumeration representing the states of the game.
 */
typedef enum
{
    PAUSED,  /**< The game is in the paused state. */
    WAIT,    /**< The game is in the waiting state. */
    BTN1,    /**< The button 1 state. */
    BTN2,    /**< The button 2 state. */
    BTN3,    /**< The button 3 state. */
    BTN4,    /**< The button 4 state. */
    SUCCESS, /**< The success state. */
    FAIL,    /**< The fail state. */
    RESET    /**< The reset state. */
} STATES;

/**
 * @brief Enumeration representing the states of the serial communication.
 */
typedef enum
{
    AWAITING_COMMAND, /**< Waiting for a command state. */
    AWAITING_PAYLOAD, /**< Waiting for payload state. */
    AWAITING_NAME     /**< Waiting for name state. */
} SERIAL_STATE;

extern volatile GAME_STATE game_state;
extern volatile STATES state;
extern volatile SERIAL_STATE serial_state;