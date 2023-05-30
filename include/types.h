#define INITIAL_SEED 0x12345678 // ! Change to student number for final
#define MAX_SEQUENCE_LEN 65535

#define SEGS_EF 0b00111110
#define SEGS_BC 0b01101011
#define SEGS_G 0b01110111
#define SEGS_ON 0b00000000
#define SEGS_OFF 0b01111111

#define TONE1 10959
#define TONE2 13033
#define TONE3 8210
#define TONE4 21919

typedef struct
{
    char name[20];
    uint16_t score;
} high_score_t;

typedef enum
{
    DISPLAY,
    PERFORM,
    GAMEOVER,
    AWAIT_NAME,
    SET_NAME
} GAME_STATE;

typedef enum
{
    PAUSED,
    WAIT,
    BTN1,
    BTN2,
    BTN3,
    BTN4,
    SUCCESS,
    FAIL,
    RESET,
} STATES;

typedef enum
{
    AWAITING_COMMAND,
    AWAITING_PAYLOAD,
    AWAITING_NAME
} SERIAL_STATE;