
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "ds1307.h"

unsigned char week, year, month, date, hour, min, second, ampm;

void ds1307_init(void)
{

  TWBR= 72;    // 100kHz (TWBR = 72, TWPS = 0)
  TWSR= 0x00;
//  TWCR= 0x04;           // TWI Enable

#if 0
  ds1307_write(0x06,0x09);	 // y	09년
  ds1307_write(0x05,0x04);	 // m	04월
  ds1307_write(0x04,0x24);	 // date 24일
  ds1307_write(0x03,0x01);	 // day   
  ds1307_write(0x02,0x13);	 // h	13시
  ds1307_write(0x01,0x27);	 // m	27분
  ds1307_write(0x00,0x39);	 // s	39초
#endif
}


void ds1307_setTime(char Y,char M,char D, char d, char h, char m, char s )
{
	Y= (((Y/10)<<4)&0xF0)|((Y%10)&0x0F);
	M= (((M/10)<<4)&0xF0)|((M%10)&0x0F);
	D= (((D/10)<<4)&0xF0)|((D%10)&0x0F);
	d= (((d/10)<<4)&0xF0)|((d%10)&0x0F);
	h= (((h/10)<<4)&0xF0)|((h%10)&0x0F);
	m= (((m/10)<<4)&0xF0)|((m%10)&0x0F);
	s= (((s/10)<<4)&0xF0)|((s%10)&0x0F);
	ds1307_write(0x06,Y);   // y   09년
	ds1307_write(0x05,M);   // m   04월
	ds1307_write(0x04,D);   // date 24일
	ds1307_write(0x03,d);   // day	
	ds1307_write(0x02,h);   // h   13시
	ds1307_write(0x01,m);   // m   27분
	ds1307_write(0x00,s);   // s   39초

}

void ds1307_readTime(void)
{
	year=	ds1307_read(0x06);
	year=	((year >>4) & 0x0F)*10 + (year & 0x0f);
	
	month=	ds1307_read(0x05);
	month=	((month >>4) & 0x01)*10 + (month & 0x0f);
	
	date=	ds1307_read(0x04);
	date=	((date >>4) & 0x03)*10 + (date & 0x0f);
	
	hour=	ds1307_read(0x02);
	hour=	((hour >>4) & 0x07)*10 + (hour & 0x0f);
	
	min=	ds1307_read(0x01);
	min=	((min >>4) & 0x07)*10 + (min & 0x0f);
	
	second= ds1307_read(0x00);
	second= ((second >>4) & 0x07)*10 + (second & 0x0f);

}




void ds1307_write(unsigned char address, unsigned char Byte)
{
Delay_us(1);    

  TWCR = 0xA4;      // START condition
  while(((TWCR & 0x80) == 0x00) || (TWSR & 0xF8) != 0x08);

  TWDR = 0xD0;                      // device address & write
  TWCR = 0x84;      
  while(((TWCR & 0x80) == 0x00) || (TWSR & 0xF8) != 0x18);

  TWDR = address;                   // Word address     
  TWCR = 0x84;      
  while(((TWCR & 0x80) == 0x00) || (TWSR & 0xF8) != 0x28);

  TWDR = Byte;                      // data write
  TWCR = 0x84;      
  while(((TWCR & 0x80) == 0x00) || (TWSR & 0xF8) != 0x28);

  TWCR = 0x94;      // STOP condition
}

unsigned char ds1307_read(unsigned char address)
{                  
  Delay_us(1);    

  unsigned char byte;

  TWCR = 0xA4;      // START condition
  while(((TWCR & 0x80) == 0x00) || (TWSR & 0xF8) != 0x08);

  TWDR = 0xD0;                      // device address & write  
  TWCR = 0x84;      
  while(((TWCR & 0x80) == 0x00) || (TWSR & 0xF8) != 0x18);

  TWDR = address;                   // Word address 
  TWCR = 0x84;      
  while(((TWCR & 0x80) == 0x00) || (TWSR & 0xF8) != 0x28);

  TWCR = 0xA4;      // RESTART condition
  while(((TWCR & 0x80) == 0x00) || (TWSR & 0xF8) != 0x10);

  TWDR = 0xD1;                      // device address & read  
  TWCR = 0x84;       
  while(((TWCR & 0x80) == 0x00) || (TWSR & 0xF8) != 0x40);

  TWCR = 0x84;      
  while(((TWCR & 0x80) == 0x00) || (TWSR & 0xF8) != 0x58);
  byte = TWDR;                      // data write 

  TWCR = 0x94;      // STOP condition
  return byte;
}





