//=====================================================================================================
//
// Copyright(C) 2008 HanYoung ELEC. Co., LTD.
// Designed by Paul B.H Park(idonno2000)
//
//
//	filename : 16by2CLCD.c
//	description : Ä³¸¯ÅÍLCD
//
//	history : - Designed and programmed by Duck-Yong Yoon in 2006.
//            - Revised by Paul B.H Park(idonno2000) at 2008.06.23
//
//=====================================================================================================
//#include "include.h"

#include <inavr.h>
#include <iom2560.h>

//#include <avr/io.h>
//#include <util/delay.h>
#include <stdio.h>

//#include <avr/interrupt.h>
//#include <avr/pgmspace.h>

#include "16by2CLCD.h"
#include "util.h"

unsigned char posInpage=0;

void CLCD_command(unsigned char command)		/* write a command(instruction) to text LCD */
{
  PORTD = PORTD & 0x40;					// E = 0, Rs = 0, R/-W = 0
  PORTH = command;				// output command
  PORTD =  PORTD & 0x40 | 0x20;					// E = 1

  asm(" PUSH  R0 ");			// delay for about 250 ns
  asm(" POP   R0 ");
  PORTD = PORTD & 0x40;					// E = 0
  Delay_us(50);
}

void CLCD_data(unsigned char data)		/* display a character on text LCD */
{
 PORTD = PORTD & 0x40 | 0x10;					// E = 0, Rs = 1, R/-W = 0

  PORTH = data;					// output data
  PORTD = PORTD & 0x40 | 0x30;					// E = 1

  asm (" PUSH  R0 ");			// delay for about 250 ns
  asm (" POP   R0 ");
  PORTD = PORTD & 0x40 | 0x10;					// E = 0

  Delay_us(50);
}

void CLCD_cursor_ON(unsigned char pos, unsigned char offset)		/* write a command(instruction) to text LCD */
{
	CLCD_command(pos|offset);
  	CLCD_command(0x0F);
}

void CLCD_cursor_OFF(void)		/* write a command(instruction) to text LCD */
{
  	CLCD_command(0x0C);
	posInpage = 0xFF;
}

void CLCD_string(unsigned char command, char *string) /* display a string on LCD */
{
  CLCD_command(command);				// start position of string
  while(*string != 0x00)			// display string
    { CLCD_data(*string);
       string++;
    }
}

void CLCD_initialize(void)			/* initialize text LCD module */
{
  CLCD_command(0x38);				// function set(8 bit, 2 line, 5x7 dot)
  CLCD_command(0x0C);				// display control(display ON, cursor OFF)
  //CLCD_command(0x0F);				// display control(display ON, cursor OFF)
  CLCD_command(0x06);				// entry mode set(increment, not shift)
  CLCD_command(0x01);				// clear display
  Delay_ms(2);
}


void CLCD_BackLightOnOff(char OnOff)//PORD bit 6
{
	if(OnOff) PORTD = PORTD | 0x40;
	else		PORTD = PORTD & 0xBF;
}



