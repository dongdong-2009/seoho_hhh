//============================================================================
//
// Copyright(C) 2008 HanYoung ELEC. Co., LTD.
// Designed by Paul B.H Park(idonno2000)
//
//	filename : 128by64GLCD.C
//	description : 그래픽LCD
//
//	history : - Designed and programmed by Duck-Yong Yoon in 2006.
//            - Revised by Paul B.H Park(idonno2000) at 2008.06.17
//               ㅏ윤교수님 소스는 lcd를 직접 제어하는 방식이었음
//               ㅏ그러한 이유로 lcd를 자유롭게 제어하기 위하여는
//               ㅏ복잡한 명령어 조합이 필수~~
//               ㅏ그리하여 수정을 한것이 LCD_buffer라는 배열을 잡아
//               ㅏ메모리의 값들을 수정하고 한방에 GLCD에 뿌리는 방식
//               ㅏ그래서 추가한 것이 FullUpdate함수임 이점 참고..
//               ㅏ현재 putchar0508(),print0508()함수만 사용가능
//               ㅏ폰트를 추가하여야함
//               ㅏ참고자료(KH351_SDK GLCD소스)
//
//============================================================================

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#include "128by64GLCD.H"
#include "FONT.H"

char LCD_buffer[128*8];
char Korean_buffer[32];			/* 32 byte Korean font buffer */


 char ScrollBuffer[2*SCRLLBUF_SIZE];
 char* pScrollDataStart;
 char* pScrollDataEnd;
 char* pScrollDataOffset;

unsigned char LCDSemaphore=0;
unsigned char LCDUpdateFlag=0;
unsigned char LCDScrollOnOff=0;



void GLCD_command(BYTE cs,BYTE command)       /* write a command */
{ 
	PORTH = cs & 0xC0;			// D/-I = 0, R/-W = 0
	asm volatile(" PUSH  R0 ");                   // delay for 500 ns
	asm volatile(" POP   R0 ");
	asm volatile(" PUSH  R0 ");
	asm volatile(" POP   R0 ");
	PORTL = command;				// write command
	PORTH = (cs & 0xC0)|0x20;			// E = 1
	asm volatile(" PUSH  R0 ");                   // delay for 500 ns
	asm volatile(" POP   R0 ");
	asm volatile(" PUSH  R0 ");
	asm volatile(" POP   R0 ");
	PORTH = cs & 0xC0;			// E = 0
	asm volatile(" NOP      ");
	PORTH = 0x00;					// clear all control signals
	_delay_us(10);                                 // wait for GLCD busy
}

void GLCD_data(BYTE signal,BYTE character)        /* write a data */
{
	PORTH = (signal & 0xC0)|0x10;			// D/-I = 1, R/-W = 0
	asm volatile(" PUSH  R0 ");                   // delay for 500 ns
	asm volatile(" POP   R0 ");
	asm volatile(" PUSH  R0 ");
	asm volatile(" POP   R0 ");
	PORTL = character;				// write data
	PORTH = (signal & 0xC0)|0x30;			// E = 1
	asm volatile(" PUSH  R0 ");                   // delay for 500 ns
	asm volatile(" POP   R0 ");
	asm volatile(" PUSH  R0 ");
	asm volatile(" POP   R0 ");
	PORTH = (signal & 0xC0)|0x10;			// E = 0
	asm volatile(" NOP      ");
	PORTH = 0x00;					// clear all control signals
	_delay_us(10);                                 // wait GLCD busy
}


void GLCD_clear(void)				/* clear GLCD screen */
{
	unsigned char i, j, x;

	PORTH = 0x00;					// clear all control signals
	GLCD_command(0xC0,0x3F);			// CS1,CS2 display ON
	GLCD_command(0xC0,0xC0);			// CS1,CS2 display position

	x = 0xB8;
	for(i = 0; i <= 7; i++)
	{ GLCD_command(0xC0,x);                     // X start addtess
	  GLCD_command(0xC0,0x40);                  // Y start address
	  for(j = 0; j <= 63; j++)
	    GLCD_data(0xC0,0x00);			// clear CS1 and CS2 screen
	  x++;
	}
}

void GLCD_BufClear(void)	
{
int i;
for(i=0;i<(128*8);i++)
{
	LCD_buffer[i]=0x00;
}

}




void GLCD_FullUpdate(void)
{
	unsigned int i, j;

	for (i=0;i<8;i++) 
	{
		GLCD_command(0xC0,0xB8 + i); 
		for (j=0;j<64;j++) 
		{
			GLCD_data(0x40,LCD_buffer[(i<<7)+j]);       // write the data 
			GLCD_data(0x80,LCD_buffer[(i<<7)+j+64]);       // write the data 
		}
	}
}


void GLCD_LCDScrollUpdate(unsigned char line)
{
	static unsigned char _cnt=0;
	static unsigned char _step=0;

	if(LCDScrollOnOff ||  _step)
	{
			if(_step==0)
			{
				if(!LCDSemaphore)
				{
					LCDSemaphore = 1;
					_step++;
				}
			}
			else if(_step==1)
			{
				PORTH =  0xC0;			// D/-I = 0, R/-W = 0

				PORTL = 0xB8 + line;				// line
				PORTH =  0xC0|0x20;			// E = 1

				PORTH = 0xC0;			// E = 0

				PORTH = 0x00;					// clear all control signals
				_delay_us(10);								   // wait for GLCD busy

				PORTH =  0xC0;			// D/-I = 0, R/-W = 0

				PORTL = 0x40 + _cnt;				// y address
				PORTH =  0xC0|0x20;			// E = 1

				PORTH = 0xC0;			// E = 0

				PORTH = 0x00;					// clear all control signals
				_delay_us(10);					// wait for GLCD busy

				PORTH = 0x40|0x10;			// D/-I = 1, R/-W = 0
				PORTL = LCD_buffer[(line<<7)+_cnt];	// write data
				PORTH = 0x40|0x30;			// E = 1
				PORTH = 0x40|0x10;			// E = 0

				PORTH = 0x00;					// clear all control signals
				_delay_us(10);								   // wait GLCD busy

				PORTH = 0x80|0x10;			// D/-I = 1, R/-W = 0
				PORTL = LCD_buffer[(line<<7)+_cnt+64];	// write data
				PORTH = 0x80|0x30;			// E = 1
				PORTH = 0x80|0x10;			// E = 0

				PORTH = 0x00;					// clear all control signals
				_delay_us(10);								   // wait GLCD busy

				 _step++;
			}
			else if(_step==2)
			{
				PORTH =  0xC0;			// D/-I = 0, R/-W = 0
				
					PORTL = 0xB8 + line +1;				// line
					PORTH =  0xC0|0x20; 		// E = 1
				
					PORTH = 0xC0;			// E = 0
				
					PORTH = 0x00;					// clear all control signals
					_delay_us(10);								   // wait for GLCD busy
				
					PORTH =  0xC0;			// D/-I = 0, R/-W = 0
				
					PORTL = 0x40 + _cnt;				// y address
					PORTH =  0xC0|0x20; 		// E = 1
				
					PORTH = 0xC0;			// E = 0
				
					PORTH = 0x00;					// clear all control signals
					_delay_us(10);					// wait for GLCD busy
				
					PORTH = 0x40|0x10;			// D/-I = 1, R/-W = 0
					PORTL = LCD_buffer[((line+1)<<7)+_cnt]; // write data
					PORTH = 0x40|0x30;			// E = 1
					PORTH = 0x40|0x10;			// E = 0
				
					PORTH = 0x00;					// clear all control signals
					_delay_us(10);								   // wait GLCD busy
				
					PORTH = 0x80|0x10;			// D/-I = 1, R/-W = 0
					PORTL = LCD_buffer[((line+1)<<7)+_cnt+64];	// write data
					PORTH = 0x80|0x30;			// E = 1
					PORTH = 0x80|0x10;			// E = 0
				
					PORTH = 0x00;					// clear all control signals
					_delay_us(10);								   // wait GLCD busy

				_cnt++;
				_step++;
			}
			else
			{
				if(63<_cnt) _cnt=0;
				_step=0;	
				LCDSemaphore = 0;
			}
	}
}



void GLCD_LCDUpdate(void)
{
	static unsigned char col=0;
	static unsigned char cnt=0;
	static unsigned char step=0;


			if(LCDUpdateFlag)
			{
				cnt=0;
				col=0;
				step=0;
				LCDSemaphore = 0;
			}


			if(step==0)
			{
				if(LCDUpdateFlag)
				{
					LCDUpdateFlag = 0;
			
					if(!LCDSemaphore)
					{
						LCDSemaphore = 1;
						step++;
					}
				}
			}
			else if(step==1)
			{
				PORTH =  0xC0;			// D/-I = 0, R/-W = 0

				PORTL = 0xB8 + col;				// line
				PORTH =  0xC0|0x20;			// E = 1

				PORTH = 0xC0;			// E = 0

				PORTH = 0x00;					// clear all control signals
				_delay_us(10);								   // wait for GLCD busy

				PORTH =  0xC0;			// D/-I = 0, R/-W = 0

				PORTL = 0x40;				// y address
				PORTH =  0xC0|0x20;			// E = 1

				PORTH = 0xC0;			// E = 0

				PORTH = 0x00;					// clear all control signals
				_delay_us(10);					// wait for GLCD busy
				GLCD_command(0xC0,0xB8 + col); 
				step++;

			}
			else if(step==2)
			{
				GLCD_data(0x40,LCD_buffer[(col<<7)+cnt]);	   // write the data 
				GLCD_data(0x80,LCD_buffer[(col<<7)+cnt+64]); 	  // write the data 

				cnt++;
				if(63<cnt)
				{
					cnt=0;
					col++;
					if(7<col)
					{
						col=0;
						step++;
					}
					else step=1;
				}
				
			}
			else
			{
				cnt=0;
				col=0;
				step=0;
				LCDSemaphore = 0;
			}
}



void GLCD_BackLightOnOff(char OnOff)//PORD bit 6
{
	if(OnOff) PORTD = PORTD | 0x40;
	else		PORTD = PORTD & 0xBF;
}

void GLCD_BuzzerOnOff(char OnOff)//PORD bit 7 
{
	if(OnOff) PORTD = PORTD | 0x80;
	else		PORTD = PORTD & 0x7F;
}






void GLCD_PutStringScrollBuf(char* str)
{
		unsigned char tu = 0;

	unsigned char character1;
	unsigned char character2;
	unsigned int character3;

	unsigned char tx = 0;


	pScrollDataStart =pScrollDataEnd=&ScrollBuffer[0];

	while (*str != 0x00) 
	{
		character1 = *str ;
		if((character1 & 0x80) == 0x00)
		{
			for( tx = 0; tx < 8 ; tx++)
			{
				*pScrollDataEnd  =  pgm_read_byte(&Font0816[character1][tx]);	
				*(pScrollDataEnd+SCRLLBUF_SIZE)  =  pgm_read_byte(&Font0816[character1][tx+8]);	
				pScrollDataEnd++;
			}
		}
		else
		 { 
		 	str ++;
			character2 = *str;
			character3 = ((WORD)character1<<8) + character2;

			GLCD_Korean(character3) ;
			for( tx = 0; tx < 16 ; tx++)
			{
				*pScrollDataEnd = Korean_buffer[tx];		
				*(pScrollDataEnd+SCRLLBUF_SIZE) = Korean_buffer[tx+16];
				pScrollDataEnd++;
			}
			tu++;
		 }
		str++;
		tu++;				
	}

pScrollDataOffset = pScrollDataStart - 128;
}








void GLCD_putchar1632(unsigned char x, unsigned char y,  char dat_c)
{
	unsigned char tx = 0;
	for( tx = 0; tx < 64 ; tx++)
	{
		LCD_buffer[(tx%16+x)+(128*(tx/16))+128*y] =pgm_read_byte(& Font1632[dat_c][tx]);	
	}
}


void GLCD_putchar0816(unsigned char x, unsigned char y,  char dat_c)
{
	unsigned char tx = 0;

	for( tx = 0; tx < 16 ; tx++)
	{
		LCD_buffer[(tx%8+x)+(128*(tx/8))+128*y] = pgm_read_byte(&Font0816[dat_c][tx]);		
	}
}


void GLCD_putchar0508(unsigned char x, unsigned char y,  char dat_c)
{
	unsigned char tx = 0;
	if(x<125)
	{
		for( tx = 0; tx < 5 ; tx++)
		{
			LCD_buffer[(tx+x)+128*y] = pgm_read_byte(&Font0508[dat_c][tx]);		
		}
	}
}


void GLCD_putchar1616(unsigned char x, unsigned char y,  int dat_c)
{
	unsigned char tx = 0;

	GLCD_Korean(dat_c) ;

	for( tx = 0; tx < 32 ; tx++)
	{
		LCD_buffer[((tx%16)+x) + (128*(y+tx/16))] = Korean_buffer[tx];			
	}
}



//_________________________________________________________________________________________


void GLCD_print0508(unsigned char x, unsigned char y,  char* str)
{
	unsigned char tu = 0;

	while (*str != 0x00) 
	{
		if(*str == 0xff)*str=' ';
		GLCD_putchar0508(x+(tu*6),y,*str-0x20);
		str++;
		tu++;				
	}
}

void GLCD_print0816(unsigned char x, unsigned char y,  char* str)
{
	unsigned char tu = 0;

	while (*str != 0x00) 
	{
		if(*str == 0xff)*str=' ';
		GLCD_putchar0816(x+tu*8,y,*str-0x20);
		str++;
		tu++;				
	}
}

void GLCD_print1616(unsigned char x, unsigned char y,  char* str)
{
	unsigned char tu = 0;

	unsigned char character1;
	unsigned char character2;
	unsigned int character3;

	while (*str != 0x00) 
	{
		character1 = *str ;
		if((character1 & 0x80) == 0x00)	 GLCD_putchar0816(x+tu*8,y,character1);
		else
		 { 
		 	str ++;
			character2 = *str;
			character3 = ((WORD)character1<<8) + character2;
			GLCD_putchar1616(x+tu*8,y,character3);
			tu++;
		 }
		str++;
		tu++;				
	}
}




void GLCD_inversion(unsigned char x, unsigned char y,  char cnt)
{
	unsigned int i; 
	for(i=x;i<(cnt+x);i++)
	{
		LCD_buffer[((y-1)<<7)+i] = LCD_buffer[((y-1)<<7)+i] |0x80;
		LCD_buffer[(y<<7)+i] = ~LCD_buffer[(y<<7)+i];
	}
	
	//LCD_buffer[((y-1)<<7)+x] = LCD_buffer[((y-1)<<7)+x] & 0x7F;
	//LCD_buffer[(y<<7)+x] = LCD_buffer[(y<<7)+x] & 0x7F;
	//LCD_buffer[((y-1)<<7)+cnt+x-1] = LCD_buffer[((y-1)<<7)+cnt+x-1] & 0x7F;
	//LCD_buffer[(y<<7)+cnt+x-1] = LCD_buffer[(y<<7)+cnt+x-1] & 0x7F;
}




void GLCD_Korean(WORD Kcode)		/* display a 16x16 Dot Korean on GLCD */
{
	BYTE i;
	BYTE cho_5bit, joong_5bit, jong_5bit;
	BYTE cho_bul, joong_bul, jong_bul, jong_flag;
	WORD character;

	WORD mix_code;

	if(1)	 mix_code = kssm_convert(Kcode);
	else	 mix_code = Kcode;
	
	cho_5bit = pgm_read_byte(&table_cho[(mix_code>>10)&0x001F]);	/* get cho, joong, jong 5 bit */
	joong_5bit = pgm_read_byte(&table_joong[(mix_code>>5)&0x001F]);
	jong_5bit = pgm_read_byte(&table_jong[mix_code & 0x001F]);

	if(jong_5bit == 0)				/* don't have jongsung */
	{ 
		jong_flag = 0;
		cho_bul = pgm_read_byte(&bul_cho1[joong_5bit]);
		if((cho_5bit == 1) || (cho_5bit == 16)) joong_bul = 0;
		else  joong_bul = 1;
	}
	else						/* have jongsung */
	{ 
		jong_flag = 1;
		cho_bul = pgm_read_byte(&bul_cho2[joong_5bit]);
		if((cho_5bit == 1) || (cho_5bit == 16)) joong_bul = 2;
		else     joong_bul = 3;
		jong_bul = pgm_read_byte(&bul_jong[joong_5bit]);
	}

	character = cho_bul*20 + cho_5bit;		/* copy chosung font */
	for(i = 0; i <= 31; i++)
	Korean_buffer[i] =pgm_read_byte(& K_font[character][i]);

	character = 8*20 + joong_bul*22 + joong_5bit;	/* OR joongsung font */
	for(i = 0; i <= 31; i++)
	Korean_buffer[i] |= pgm_read_byte(&K_font[character][i]);

	if(jong_flag == 1)				/* if jongsung, OR jongsung font */
	{ 
		character = 8*20 + 4*22 +jong_bul*28 + jong_5bit;
		for(i = 0; i <= 31; i++)
		Korean_buffer[i] |= pgm_read_byte(&K_font[character][i]);
	}

}









unsigned int kssm_convert(unsigned int kssm)
{
	unsigned char high,low;
	unsigned int index;
    unsigned int return_data;

	high = (kssm>>8)&0xff;
	low  =  kssm    &0xff;

	if ((kssm >= 0xb0a1)&&(kssm <= 0xc8fe))
        {
		index  = (high - 0xb0) * 94 + low - 0xa1;
		return_data  =  (pgm_read_byte(&KSTable[index][0]))*256;
		return_data  |=  pgm_read_byte(&KSTable[index][1]);
		return return_data;
	}
}



