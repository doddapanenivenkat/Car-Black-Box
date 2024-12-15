#include <xc.h>
#include "matrix_keypad.h"

void init_matrix_keypad(void) {
    /* Config PORTB as digital */
    ADCON1 = 0x0F;

    /* Set Rows (RB7 - RB5) as Outputs and Columns (RB4 - RB1) as Inputs */
    TRISB = 0x1E;

    /* Set PORTB input as pull up for columns */
    RBPU = 0;

    MATRIX_KEYPAD_PORT = MATRIX_KEYPAD_PORT | 0xE0;
}

unsigned char scan_key(void) {
    ROW1 = LO; //R1 = 0; R2 = R3 = 1;
    ROW2 = HI;
    ROW3 = HI;

    if (COL1 == LO) //C1 = 0
    {
        return 1; //SW1
    } else if (COL2 == LO) //C2 = 0
    {
        return 4; //SW4
    } else if (COL3 == LO) //C3 = 0
    {
        return 7; //SW7
    } else if (COL4 == LO) //C4 = 0
    {
        return 10; //SW10
    }

    ROW1 = HI; //R2 = 0;R1 = R3 = 1;
    ROW2 = LO;
    ROW3 = HI;

    if (COL1 == LO) //C1 = 0
    {
        return 2; //SW2
    } else if (COL2 == LO) //C2 = 0
    {
        return 5; //SW5
    } else if (COL3 == LO) //C3 = 0
    {
        return 8; //SW8
    } else if (COL4 == LO) //C4 = 0
    {
        return 11; //SW11
    }

    ROW1 = HI;
    ROW2 = HI;
    ROW3 = LO;
    /* TODO: Why more than 2 times? */
    ROW3 = LO;

    if (COL1 == LO) {
        return 3;
    } else if (COL2 == LO) {
        return 6;
    } else if (COL3 == LO) {
        return 9;
    } else if (COL4 == LO) {
        return 12;
    }

    return 0xFF;
}

unsigned char read_switches(unsigned char detection_type) {
    static unsigned char once = 1, key;

    if (detection_type == STATE_CHANGE) {
        key = scan_key(); //It's Edge triggering,they return (0-12) or all released one time
        if (key != 0xFF && once) {
            once = 0;
            return key;
        } else if (key == 0xFF) {
            once = 1;
        }
    } else if (detection_type == LEVEL_CHANGE) {
        return scan_key(); //return (0-12) or all released repeatedly
    }

    return 0xFF;
}
