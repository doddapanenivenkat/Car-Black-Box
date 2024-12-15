#include <xc.h>

void init_i2c(void) {
    /* Set SCL and SDA pins as inputs */
    TRISC3 = 1;
    TRISC4 = 1;
    /* Set I2C master mode */
    SSPCON1 = 0x28;
    /* Set address register by calculating clock pulse using formula-> CP = fosc/4*(SSPADD+1) */
    SSPADD = 0x31;
    /* Use I2C levels, worked also with '0' */
    CKE = 0;
    /* Disable slew rate control  worked also with '0' */
    SMP = 1;
    /* Clear SSPIF interrupt flag */
    SSPIF = 0;
    /* Clear bus collision flag */
    BCLIF = 0;
}

void i2c_idle(void) {
    while (!SSPIF); //SSPIF -> 0 instruction is not completed 
    SSPIF = 0; //SSPIF -> 1 instruction is completed
}

void i2c_ack(void) {
    if (ACKSTAT) {
        /* Do debug print here if required */
    }
}

void i2c_start(void) {
    SEN = 1; //To start the communication 
    i2c_idle();
}

void i2c_stop(void) {
    PEN = 1; //To check stop the communication or not
    i2c_idle();
}

void i2c_rep_start(void) {
    RSEN = 1; //To check repeat start communication or not
    i2c_idle();
}

void i2c_write(unsigned char data) {
    SSPBUF = data; //Transmit the data
    i2c_idle();
}

void i2c_rx_mode(void) {
    RCEN = 1; //Receive the MicroController
    i2c_idle();
}

void i2c_no_ack(void) {
    ACKDT = 1;
    ACKEN = 1;
}

unsigned char i2c_read(void) {
    i2c_rx_mode(); //enable the Rx mode
    i2c_no_ack();

    return SSPBUF;
}