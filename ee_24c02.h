#define EE_READ     0xA1
#define EE_WRITE    0xA0

void write_24c02(unsigned char address, unsigned char data);
unsigned char read_24c02(unsigned char address);