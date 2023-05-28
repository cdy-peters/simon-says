#define SEGS_EF 0b00111110
#define SEGS_BC 0b01101011
#define SEGS_G 0b01110111
#define SEGS_ON 0b00000000
#define SEGS_OFF 0b01111111

typedef enum
{
    PAUSED,
    WAIT,
    BTN1,
    BTN2,
    BTN3,
    BTN4,
    SUCCESS,
    FAIL
} STATES;

typedef enum
{
    AWAITING_COMMAND,
    AWAITING_PAYLOAD
} SERIAL_STATE;