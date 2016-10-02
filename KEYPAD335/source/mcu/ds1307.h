#ifndef _DS1307_H_
#define _DS1307_H_

extern unsigned char week, year, month, date, hour, min, second, ampm;

void ds1307_init(void);
void ds1307_setTime(char Y,char M,char D, char d, char h, char m, char s );
void ds1307_readTime(void);
void ds1307_write(unsigned char address ,unsigned char Byte);
unsigned char ds1307_read(unsigned char address);

#endif

