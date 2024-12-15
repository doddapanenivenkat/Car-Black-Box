#include "ee_24c02.h"
#include "i2c.h"
#include <xc.h>

void write_24c02(unsigned char address, unsigned char data) {
    i2c_start(); //start communication
    i2c_write(EE_WRITE); //write unique id
    i2c_write(address); //send data(address)
    i2c_write(data); //send data
    i2c_stop(); //stop communication
    for (int i = 5000;i--;);
}

unsigned char read_24c02(unsigned char address) {
    unsigned char data;

    i2c_start(); //start communication
    i2c_write(EE_WRITE); //write unique id with write bit = 0
    i2c_write(address); //send data(address)
    i2c_rep_start(); //again repeat start
    i2c_write(EE_READ); //write unique id with read bit = 1
    data = i2c_read(); //perform i2c_read
    i2c_stop(); //stop communication

    return data;
}