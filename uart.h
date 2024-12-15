#ifndef SCI_H
#define SCI_H

#define RX_PIN					TRISC7
#define TX_PIN					TRISC6

void init_uart(void);               //for configuration
void putch(unsigned char byte);     //trasmit 1 bye of data
int puts(const char *s);            //transmit as string
unsigned char getch(void);          //receive 1 byte data 
//unsigned char getch_with_timeout(unsigned short max_time);
unsigned char getche(void);         //received 1 byte of data,again transmit that 1 byte

#endif
