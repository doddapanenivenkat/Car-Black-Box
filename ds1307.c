
#include "main.h"
#include "ds1307.h"
#include "i2c.h"
#include <xc.h>

/* 
 * DS1307 Slave address
 * D0  -  Write Mode
 * D1  -  Read Mode
 */

void init_ds1307(void) {
    unsigned char dummy;

    /* Setting the CH bit of the RTC to Stop the Clock 
            Disable the clock   */
    dummy = read_ds1307(SEC_ADDR);
    write_ds1307(SEC_ADDR, dummy | 0x80);

    /* Setting 12 Hour Format */
    dummy = read_ds1307(HOUR_ADDR);
    write_ds1307(HOUR_ADDR, dummy & 0x40);

    /* 
     * Control Register of DS1307
     * Bit 7 - OUT
     * Bit 6 - 0
     * Bit 5 - OSF
     * Bit 4 - SQWE
     * Bit 3 - 0
     * Bit 2 - 0
     * Bit 1 - RS1
     * Bit 0 - RS0
     * 
     * Seting RS0 and RS1 as 11 to achive SQW out at 32.768 KHz
     */
    write_ds1307(CNTL_ADDR, 0x93);

    /* Clearing the CH bit of the RTC to Start the Clock 
            Enable the clock    */
    dummy = read_ds1307(SEC_ADDR);
    write_ds1307(SEC_ADDR, dummy & 0x7F);

}

void write_ds1307(unsigned char address, unsigned char data) {
    i2c_start(); //start communication
    i2c_write(SLAVE_WRITE); //write unique id
    i2c_write(address); //send data(address)
    i2c_write(data); //send data
    i2c_stop(); //stop communication
}

unsigned char read_ds1307(unsigned char address) {
    unsigned char data;

    i2c_start(); //start communication
    i2c_write(SLAVE_WRITE); //write unique id with write bit = 0
    i2c_write(address); //send data(address)
    i2c_rep_start(); //again repeat start
    i2c_write(SLAVE_READ); //write unique id with read bit = 1
    data = i2c_read(); //perform i2c_read
    i2c_stop(); //stop communication

    return data;
}