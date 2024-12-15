/*
 * File:   main.c
 * Author: venkat
 *
 * Created on 11 November, 2024, 11:16 PM
 */

#include <xc.h>
#include "adc.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "black_box.h"
#include "ds1307.h"
#include "i2c.h"
#include "ee_24c02.h"
#include "uart.h"

void init_config() {
    init_adc();
    init_clcd();
    init_matrix_keypad();
    init_i2c();
	init_ds1307();
    init_uart();
   
}

void main(void) {
    init_config();
    while (1) {
        key = read_switches(STATE_CHANGE);
        if (screen_flag == DASH_BOARD) {
            dashboard();
            if (key == MK_SW5) {
                screen_flag = MAIN_MENU;
            }
        } else if (screen_flag == MAIN_MENU) {
            mainmenu();
            if (key == MK_SW6) {
                screen_flag = DASH_BOARD;
                m_flag1 = m_flag = 0;
                flag = 0;
            }
        } else if (screen_flag == VIEW_LOG) {
            view_log();
            if (key == MK_SW6) {
                screen_flag = MAIN_MENU;
            }
        } else if (screen_flag == DOWNLOAD_LOG) {
            if (count == 0){
                download_log();
                count = 1;
            }
            if (key == MK_SW6) {
                screen_flag = MAIN_MENU;
                count = 0;
            }
        } else if (screen_flag == CLEAR_LOG) {
            clear_log();
            if (key == MK_SW6) {
                screen_flag = MAIN_MENU;
            }
        } else if (screen_flag == SET_TIME) {
            set_time();
            if (key == MK_SW6) {
                screen_flag = MAIN_MENU;
            }
        }
    }
    return;
}
