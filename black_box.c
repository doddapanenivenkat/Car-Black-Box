#include <xc.h>
#include "adc.h"
#include "black_box.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "ds1307.h"
#include "i2c.h"
#include "ee_24c02.h"
#include "uart.h"

void dashboard() {
    if (key == MK_SW1 && e_flag < 7) {
        if (event_count < 10) {
            event_count++;
        }
        if (event_count > 0) {
            e_flag++;
        }
        store_event(speed / 10, events[e_flag]);
    }
    if (key == MK_SW2 && e_flag > 1) {
        e_flag--;
        if (event_count < 10) {
            event_count++;
        }
        store_event(speed / 10, events[e_flag]);
    }
    if (key == MK_SW3) {
        if (event_count < 10) {
            event_count++;
        }
        e_flag = 0;
        clcd_print(events[e_flag], LINE2(10));
        store_event(speed / 10, events[e_flag]);
    }
    clcd_print("  TIME    EV  SP", LINE1(0));
    get_time();
    display_time();
    if (event_count == 0) {
        clcd_print("ON", LINE2(10));
    } else {
        clcd_print(events[e_flag], LINE2(10));
    }
    speed = read_adc(CHANNEL4);
    display_speed(speed / 10);
}

void display_time() {
    clcd_print(time, LINE2(0));
}

void display_speed(int speed) {
    if (speed < 100) {
        clcd_putch(('0' + (speed / 10)), LINE2(14));
        clcd_putch(('0' + (speed % 10)), LINE2(15));
    }
}

void display_menu(int m_flag, int m_flag1) {
    if (m_flag1) {
        clcd_putch(' ', LINE1(0));
        clcd_putch('*', LINE2(0));
    } else {
        clcd_putch('*', LINE1(0));
        clcd_putch(' ', LINE2(0));
    }
    clcd_print(menu[m_flag], LINE1(2));
    clcd_print(menu[m_flag + 1], LINE2(2));
    for (delay = 5000; delay--;);
}

void mainmenu() {
    display_menu(m_flag, m_flag1);
    CLEAR_DISP_SCREEN;
    if (key == MK_SW1) {
        flag++;
        if (m_flag1 == 0) {
            m_flag1 = 1;
        } else if (m_flag < 2) {
            m_flag++;
        }
    } else if (key == MK_SW2) {
        flag--;
        if (m_flag1 == 1) {
            m_flag1 = 0;
        } else if (m_flag > 0) {
            m_flag--;
        }
    } else if (key == MK_SW5) {
        if (flag == 0) {
            screen_flag = VIEW_LOG;
        }
        if (flag == 1) {
            screen_flag = DOWNLOAD_LOG;
        }
        if (flag == 2) {
            screen_flag = CLEAR_LOG;
        }
        if (flag == 3) {
            screen_flag = SET_TIME;
        }
    }
}

void view_log() {
    if (1) {
        clcd_putch('#', LINE1(0));
        clcd_print("view log", LINE1(2));
        if (key == MK_SW1 && view_count < ((event_count - 1) * 11)) {
            view_count = view_count + 12;
        } else if (key == MK_SW2 && view_count > 0) {
            view_count = view_count - 12;
        }
        int i;
        x1 = view_count;
        for (i = 0; i < 8; i++) {
            view[i] = read_24c02(0x00 + x1);
            x1++;
        }
        view[i++] = ' ';
        view[i++] = read_24c02(0x00 + x1);
        x1++;
        view[i++] = read_24c02(0x00 + x1);
        x1++;
        view[i++] = ' ';
        view[i++] = read_24c02(0x00 + x1);
        x1++;
        view[i++] = read_24c02(0x00 + x1);
        x1++;
        if (event_count > 0) {
            clcd_putch('0' + ((x1 % 11) - 1), LINE2(0));
            clcd_print(view, LINE2(2));
        } else {
            clcd_print("No Events", LINE2(2));
        }
    }
}

void download_log() {
    puts("Car Events\n\r");
    clcd_print("Logs are", LINE1(3));
    clcd_print("Downloading...", LINE2(1));
    unsigned char y1 = 0;
    unsigned char log[13];
    int i;
    while (y1 < x) {
        for (i = 0; i < 8; i++) {
            log[i] = read_24c02(0x00 + y1);
            y1++;
        }
        log[i++] = read_24c02(0x00 + y1);
        y1++;
        log[i++] = read_24c02(0x00 + y1);
        y1++;
        log[i++] = read_24c02(0x00 + y1);
        y1++;
        log[i] = read_24c02(0x00 + y1);
        y1++;
        log[12] = '\0';
        puts(log);
        puts("\n\r");
    }
    if (event_count == 0) {
        puts("No logs to download\n\r");
    }
}

void clear_log() {
    if (1) {
        clcd_print("Logs are", LINE1(3));
        clcd_print("Clearing....", LINE2(2));
        x = 0;
        event_count = 0;
    }
}

void set_time() {
    if (1) {
        clcd_print("HH:MM:SS", LINE1(2));
        clcd_print(time, LINE2(2));
        if (key == MK_SW2) {
            field_flag++;
            if (field_flag == 3) {
                field_flag = 0;
            }
        }
        if (field_flag == 0) {
            if (++delay < 500) {
                clcd_print(time, LINE2(2));
            } else if (++delay < 1000) {
                clcd_print("  ", LINE2(2));
            } else {
                delay = 0;
            }
            if (key == MK_SW1) {
                time[1]++;
                if (time[1] > '9') {
                    time[0]++;
                    time[1] = '0';
                }
                if (time[0] == '2' && time[1] == '4') {
                    time[0] = '0';
                    time[1] = '0';
                }
            }
        }
        if (field_flag == 1) {
            if (++delay < 100) {
                clcd_print(time, LINE2(2));
            } else if (++delay < 200) {
                clcd_print("  ", LINE2(5));
            } else {
                delay = 0;
            }
            if (key == MK_SW1) {
                time[4]++;
                if (time[4] > '9') {
                    time[3]++;
                    time[4] = '0';
                }
                if (time[3] == '6' && time[4] == '0') {
                    time[3] = '0';
                    time[4] = '0';
                }
            }
        }
        if (field_flag == 2) {
            if (++delay < 100) {
                clcd_print(time, LINE2(2));
            } else if (++delay < 200) {
                clcd_print("  ", LINE2(8));
            } else {
                delay = 0;
            }
            if (key == MK_SW1) {
                time[7]++;
                if (time[7] > '9') {
                    time[6]++;
                    time[7] = '0';
                }
                if (time[6] == '6' && time[7] == '0') {
                    time[6] = '0';
                    time[7] = '0';
                }
            }
        }
        if (key == MK_SW5) {
            h1 = time[0] - '0';
            h2 = time[1] - '0';
            m1 = time[3] - '0';
            m2 = time[4] - '0';
            s1 = time[6] - '0';
            s2 = time[7] - '0';
            data1 = (h1 << 4) | h2;
            data2 = (m1 << 4) | m2;
            data3 = (s1 << 4) | s2;
            write_ds1307(HOUR_ADDR, data1);
            write_ds1307(MIN_ADDR, data2);
            write_ds1307(SEC_ADDR, data3);
            screen_flag = MAIN_MENU;
        }
        if (key == MK_SW6) {
            field_flag = 0;
        }
    }
}

void get_time(void) {
    clock_reg[0] = read_ds1307(HOUR_ADDR);
    clock_reg[1] = read_ds1307(MIN_ADDR);
    clock_reg[2] = read_ds1307(SEC_ADDR);

    if (clock_reg[0] & 0x40) //check 12hour or 24hour 
    {
        time[0] = '0' + ((clock_reg[0] >> 4) & 0x01); //if 12hour-> bit4 store 10's in time[0]and bit3-0 store 1's in time[1]
        time[1] = '0' + (clock_reg[0] & 0x0F);
    } else {
        time[0] = '0' + ((clock_reg[0] >> 4) & 0x03); //for 24 hour-> bit4,5 stores 10's place in time[0] and bit3-0 store 1's place in time[1]
        time[1] = '0' + (clock_reg[0] & 0x0F);
    }
    time[2] = ':';
    time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F); //for minutes
    time[4] = '0' + (clock_reg[1] & 0x0F);
    time[5] = ':';
    time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F); //for seconds
    time[7] = '0' + (clock_reg[2] & 0x0F);
    time[8] = '\0';
}

void store_event(int speed, char *event) {
    if (x == 120) {
        static unsigned int k = 12;
        unsigned char data;
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 12; j++) {
                data = read_24c02(0x00 + k + j);
                write_24c02(0x00 + k + j - 12, data);
            }
            k = k + 12;
        }
        x = x - 12;
    }
    for (int i = 0; i < 8; i++) {
        write_24c02(0x00 + x, time[i]);
        x++;
    }
    int j = 0;
    while (event[j] != '\0') {
        write_24c02(0x00 + x, event[j++]);
        x++;
    }
    write_24c02(0x00 + x, ('0' + speed / 10));
    x++;
    write_24c02(0x00 + x, ('0' + speed % 10));
    x++;
}

