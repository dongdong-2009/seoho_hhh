//================================================================
//
// Copyright(C) 2008 HanYoung ELEC. Co., LTD.
// Designed by Paul B.H Park(idonno2000)
//
//
//	filename : display.c
//	description : glcd display
//
//	history :  - frist designed at 2008-05-28 4:02오후
//
//================================================================

#include "key.h"
#include "display.h"
#include "128by64GLCD.H"
#include "ProtoMod.h"

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "ds1307.h"

#include <avr/pgmspace.h>
#include <avr/eeprom.h>

#include "debug_printf.h"



unsigned long naviMENU,old_naveMENU; 
unsigned char RefreshFlag = 0;

unsigned int data_array[20];
unsigned int event_array[4]={0,0,0,0};



char password[4]={0,0,0,0};
char _password[4]={0,0,0,0};

unsigned char pass_pos=0;
char confirm = 0;

char posInpage=6;

char setTime[7]; 
unsigned char Tset_pos=0;


char text_buf[256];

char history_paper[6][50];

unsigned char history_Xoffset=7;
unsigned char history_Yoffset=0;

unsigned char history_paper_line=0;
unsigned char history_initFlag=1;
unsigned int eeprom_addr=0;

unsigned int diagram_condition=0x0C00;

prog_uchar diagram[128*8] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x10, 0x10, 	0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 	0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,	0x10, 0x10, 0x14, 0x12, 0x02, 0x02, 0x02, 0x12, 0x14, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 	0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 	0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	0x00, 0x00, 0x00, 0x00, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 	0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 	0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x0F, 0x10, 0x10, 	0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x14, 0x12, 0x02, 0x02, 0x02, 0x12, 	0x14, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 	0x10, 0x7C, 0x82, 0x82, 0xDF, 0xEF, 0xF7, 0x82, 0x82, 0x7C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 	0x10, 0x10, 0x10, 0x10, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	0x00, 0x00, 0x00, 0x00, 0xC1, 0x83, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x0F, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x14, 0x12, 0x02, 	0x02, 0x02, 0x12, 0x14, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x3C, 	0x42, 0x42, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0xE0, 0x10, 0x10, 	0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x42, 0x7E, 0x42, 0x00, 	0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 	0x10, 0x7C, 0x82, 0x82, 0xDF, 0xEF, 0xF7, 0x82, 0x82, 0x7C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 	0x10, 0x10, 0x10, 0x10, 0x0F, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x14, 0x12, 0x02, 0x02, 0x02, 	0x12, 0x14, 0x10, 0x10, 0x10, 0x10, 0x0F, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x08, 0x14, 0xFF, 0x14, 0x08, 	0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x10, 0x10, 	0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x14, 0x12, 0x02, 0x02, 0x02, 0x12, 	0x14, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 	0x7E, 0x81, 0xBD, 0xBD, 0x81, 0xBD, 0xBD, 0x81, 0xBD, 0xBD, 0x81, 0x7E, 0x00, 0x00, 0x00, 0x00, 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};




void DisplayInit(void)
{
	memset( history_paper, 0x00, sizeof(history_paper));
	naviMENU = 1;
	old_naveMENU = 0xFF;
}


void Point_inversion(unsigned char x, unsigned char y,  char cnt)
{
	unsigned int i; 
	for(i=x;i<(cnt+x);i++)
	{
		LCD_buffer[((y-1)<<7)+i] = LCD_buffer[((y-1)<<7)+i] |0x80;
		LCD_buffer[(y<<7)+i] = ~LCD_buffer[(y<<7)+i];
	}
	
	LCD_buffer[((y-1)<<7)+x] = LCD_buffer[((y-1)<<7)+x] & 0x7F;
	LCD_buffer[(y<<7)+x] = LCD_buffer[(y<<7)+x] & 0x7F;
	LCD_buffer[((y-1)<<7)+cnt+x-1] = LCD_buffer[((y-1)<<7)+cnt+x-1] & 0x7F;
	LCD_buffer[(y<<7)+cnt+x-1] = LCD_buffer[(y<<7)+cnt+x-1] & 0x7F;
}

void Point_ONOFF(unsigned char bit, unsigned char OnOff)
{
	unsigned int temp = 0x0001;
	if(OnOff)
	{
		temp = temp<<bit;
		diagram_condition = diagram_condition | temp;
	}
	else
	{
		temp = ~(temp<<bit);
		diagram_condition = diagram_condition & temp;
	}
}

void LoadDiagram(void)
{
	int i;
	for(i=0;i<(128*8);i++)
	{
		LCD_buffer[i]= pgm_read_byte(&diagram[i]);
	}

	if(diagram_condition & 0x8000)Point_inversion(81,0,9);
	if(diagram_condition & 0x4000)Point_inversion(57,2,9);
	//if(diagram_condition & 0x2000)Point_inversion(70,2,9);
	if(diagram_condition & 0x1000)Point_inversion(12,4,9);
	if(diagram_condition & 0x0800)Point_inversion(28,4,9);
	if(diagram_condition & 0x0400)Point_inversion(57,4,9);
	if(diagram_condition & 0x0200)Point_inversion(106,4,9);
	//if(diagram_condition & 0x0100)Point_inversion(70,0,9);
	//if(diagram_condition & 0x0080)Point_inversion(70,0,9);
	//if(diagram_condition & 0x0040)Point_inversion(70,0,9);
	//if(diagram_condition & 0x0020)Point_inversion(70,0,9);
	//if(diagram_condition & 0x0010)Point_inversion(70,0,9);
	//if(diagram_condition & 0x0008)Point_inversion(70,0,9);
	//if(diagram_condition & 0x0004)Point_inversion(70,0,9);
	//if(diagram_condition & 0x0002)Point_inversion(70,0,9);
	if(diagram_condition & 0x0001)Point_inversion(57,6,9);
}


void irregularPAGE_handler(void)
{
	naviMENU = old_naveMENU;
}

void cpy_flash2memory(char* target, char* source)
{
	memset(target, 0x00, sizeof(target)); 
	while(1)
	{
		*target =  pgm_read_byte(source);
		if(*target == '\0')
		{
			*target = '\0';
			break;
		}
		target++;
		source++;
	}
}

prog_uchar  PAGE_ROOT[5][21]=
{
	"--MODUS-1000 UPS--",
	"1.SYSTEM MEASURE",
	"2.SYSTEM OPERATION",
	"3.SYSTEM HISTORY",
	"4.PARAMETER SETTING"				
};

void PAGE_1(void)
{
	if(EventFlagC)
	{
		GLCD_BufClear();	
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_ROOT[0][0]);		GLCD_print0508(5, 0, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_ROOT[1][0]);		GLCD_print0508(2, 2, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_ROOT[2][0]);		GLCD_print0508(2, 3, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_ROOT[3][0]);		GLCD_print0508(2, 4, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_ROOT[4][0]);		GLCD_print0508(2, 5, text_buf);
		
		GLCD_inversion(12,2,112);
	}
	else
	{
		LoadDiagram();
	}
}
void PAGE_10(void)
{
	GLCD_BufClear();	
	GLCD_print0508(5, 0,"--SYSTEM MEASURE--");
	GLCD_print0508(0, 1, (char*)_TEXT("Vin :%03dV",main_data_array[0]));		GLCD_print0508(64, 1,(char*) _TEXT("Ain :%02d.%dA", main_data_array[1]/10,main_data_array[1]%10));
	GLCD_print0508(0, 2, (char*)_TEXT("Vout:%03dV", main_data_array[2]));	GLCD_print0508(64, 2,(char*) _TEXT("Aout:%02d.%dA", main_data_array[3]/10,main_data_array[3]%10));
	GLCD_print0508(0, 3, (char*)_TEXT("VBat:%03dV", main_data_array[4]));	GLCD_print0508(64, 3,(char*) _TEXT("Abat:%02d.%dA", main_data_array[5]/10,main_data_array[5]%10));

	GLCD_print0508(0, 4, (char*)_TEXT("Fin :%02d.%dHz", main_data_array[8]/10,main_data_array[8]%10));
	GLCD_print0508(64, 4,(char*) _TEXT("Fout:%02d.%dHz", main_data_array[9]/10,main_data_array[9]%10));
	GLCD_print0508(0, 5, (char*)_TEXT("Load Rate:%03d%", main_data_array[10] ));

	
}
void PAGE_11(void)
{
	GLCD_BufClear();	
	GLCD_print0508(5, 0,"--SYSTEM MEASURE--");
	GLCD_print0508(0, 1, (char*)_TEXT("IVA:%04dVA",main_data_array[13]));	GLCD_print0508(64, 1, (char*)_TEXT("OVA:%04dVA",main_data_array[16]));
	GLCD_print0508(0, 2, (char*)_TEXT("IW :%04dW",main_data_array[14]));	GLCD_print0508(64, 2, (char*)_TEXT("OW :%04dW",main_data_array[17]));
	GLCD_print0508(0, 3, (char*)_TEXT("IPF:%04d",main_data_array[15]));		GLCD_print0508(64, 3, (char*)_TEXT("OPF:%04d",main_data_array[18]));
	GLCD_print0508(0, 4, (char*)_TEXT("charge Rate:%d%",main_data_array[11]));
	GLCD_print0508(0, 5, (char*)_TEXT("Vdc :%03dV", main_data_array[6]));	GLCD_print0508(64, 5, (char*)_TEXT("Ainv:%02d.%dA", main_data_array[7]/10,main_data_array[7]%10));
	GLCD_print0508(0, 6, (char*)_TEXT("Temp:%03d.%dC", main_data_array[12]/10,main_data_array[12]%10));
}


void PAGE_2(void)
{
	if(EventFlagC)
	{
		GLCD_BufClear();	
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_ROOT[0][0]);		GLCD_print0508(5, 0, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_ROOT[1][0]);		GLCD_print0508(2, 2, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_ROOT[2][0]);		GLCD_print0508(2, 3, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_ROOT[3][0]);		GLCD_print0508(2, 4, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_ROOT[4][0]);		GLCD_print0508(2, 5, text_buf);
		GLCD_inversion(12,3,112);
	}
	else
	{
		LoadDiagram();
	}
}


prog_uchar  PAGE_SYSTEM_OPERATION[6][21]=
{
	"--SYSTEM OPERATION--",
	 "1.CONV Manual OP",
	"2.INV Manual OP",
	"3.Auto Operation",
	"4.Transfer to BYP",
	"5.Transfer to INV"			
};


void PAGE_20(void)
{
	GLCD_BufClear();	

	cpy_flash2memory(&text_buf[0], (char*)&PAGE_SYSTEM_OPERATION[0][0]);		GLCD_print0508(5, 0, text_buf);
	cpy_flash2memory(&text_buf[0], (char*)&PAGE_SYSTEM_OPERATION[1][0]);		GLCD_print0508(2, 1, text_buf);
	cpy_flash2memory(&text_buf[0], (char*)&PAGE_SYSTEM_OPERATION[2][0]);		GLCD_print0508(2, 2, text_buf);
	cpy_flash2memory(&text_buf[0], (char*)&PAGE_SYSTEM_OPERATION[3][0]);		GLCD_print0508(2, 3, text_buf);
	cpy_flash2memory(&text_buf[0], (char*)&PAGE_SYSTEM_OPERATION[4][0]);		GLCD_print0508(2, 4, text_buf);
	cpy_flash2memory(&text_buf[0], (char*)&PAGE_SYSTEM_OPERATION[5][0]);		GLCD_print0508(2, 5, text_buf);

	GLCD_inversion(12,1,112);
}
void PAGE_200(void)
{
	GLCD_BufClear();	
	GLCD_print0508(5, 0,"--CONV Manual OP--");
	GLCD_print0508(6, 2, "Would you like to");
	GLCD_print0508(6, 3, "   RUN or STOP CONV?");

	GLCD_print0508(0, 5, "    -RUN-  -STOP-");

	GLCD_inversion(65,5,37);
}
void PAGE_201(void)
{
	GLCD_BufClear();	
	GLCD_print0508(5, 0,"--CONV Manual OP--");
	GLCD_print0508(6, 2, "Would you like to");
	GLCD_print0508(6, 3, "   RUN or STOP CONV?");

	GLCD_print0508(0, 5, "    -RUN-  -STOP-");

	GLCD_inversion(23,5,31);
}

void PAGE_2000(void)
{
	GLCD_BufClear();	
	GLCD_print1616(6, 3, " STOP");
}

void PAGE_2010(void)
{
	GLCD_BufClear();	
	GLCD_print1616(6, 3, "RUN");
}



void PAGE_21(void)
{
	GLCD_BufClear();	
	cpy_flash2memory(&text_buf[0], (char*)&PAGE_SYSTEM_OPERATION[0][0]);		GLCD_print0508(5, 0, text_buf);
	cpy_flash2memory(&text_buf[0], (char*)&PAGE_SYSTEM_OPERATION[1][0]);		GLCD_print0508(2, 1, text_buf);
	cpy_flash2memory(&text_buf[0], (char*)&PAGE_SYSTEM_OPERATION[2][0]);		GLCD_print0508(2, 2, text_buf);
	cpy_flash2memory(&text_buf[0], (char*)&PAGE_SYSTEM_OPERATION[3][0]);		GLCD_print0508(2, 3, text_buf);
	cpy_flash2memory(&text_buf[0], (char*)&PAGE_SYSTEM_OPERATION[4][0]);		GLCD_print0508(2, 4, text_buf);
	cpy_flash2memory(&text_buf[0], (char*)&PAGE_SYSTEM_OPERATION[5][0]);		GLCD_print0508(2, 5, text_buf);


	GLCD_inversion(12,2,112);
}

void PAGE_210(void)
{
	GLCD_BufClear();	
	GLCD_print0508(5, 0,"--INV Manual OP--");
	GLCD_print0508(6, 2, "Would you like to");
	GLCD_print0508(6, 3, "   RUN or STOP INV?");

	GLCD_print0508(0, 5, "    -RUN-  -STOP-");


	GLCD_inversion(65,5,37);
}
void PAGE_211(void)
{
	GLCD_BufClear();	
	GLCD_print0508(5, 0,"--INV Manual OP--");
	GLCD_print0508(6, 2, "Would you like to");
	GLCD_print0508(6, 3, "   RUN or STOP INV?");

	GLCD_print0508(0, 5, "    -RUN-  -STOP-");

	GLCD_inversion(23,5,31);
}
void PAGE_2100(void)
{
	GLCD_BufClear();	
	GLCD_print1616(6, 3, " STOP");

}

void PAGE_2110(void)
{
	GLCD_BufClear();	
	GLCD_print1616(6, 3, "RUN");

}




void PAGE_22(void)
{
	GLCD_BufClear();	
	cpy_flash2memory(&text_buf[0], (char*)&PAGE_SYSTEM_OPERATION[0][0]);		GLCD_print0508(5, 0, text_buf);
	cpy_flash2memory(&text_buf[0], (char*)&PAGE_SYSTEM_OPERATION[1][0]);		GLCD_print0508(2, 1, text_buf);
	cpy_flash2memory(&text_buf[0], (char*)&PAGE_SYSTEM_OPERATION[2][0]);		GLCD_print0508(2, 2, text_buf);
	cpy_flash2memory(&text_buf[0], (char*)&PAGE_SYSTEM_OPERATION[3][0]);		GLCD_print0508(2, 3, text_buf);
	cpy_flash2memory(&text_buf[0], (char*)&PAGE_SYSTEM_OPERATION[4][0]);		GLCD_print0508(2, 4, text_buf);
	cpy_flash2memory(&text_buf[0], (char*)&PAGE_SYSTEM_OPERATION[5][0]);		GLCD_print0508(2, 5, text_buf);

	GLCD_inversion(12,3,112);
}

void PAGE_220(void)
{
	GLCD_BufClear();	
	GLCD_print0508(0, 0,"-Auto Operation-");
	GLCD_print0508(0, 2, "Would you like to");
	GLCD_print0508(0, 3, "RUN or STOP Auto Operation?");

	GLCD_print0508(0, 5, "    -RUN-  -STOP-");


	GLCD_inversion(65,5,37);
}
void PAGE_221(void)
{
	GLCD_BufClear();	
	GLCD_print0508(0, 0,"-Auto Operation-");
	GLCD_print0508(0, 2, "Would you like to");
	GLCD_print0508(0, 3, "RUN or STOP Auto Operation?");

	GLCD_print0508(0, 5, "    -RUN-  -STOP-");

	GLCD_inversion(23,5,31);
}
void PAGE_2200(void)
{
	GLCD_BufClear();	
	GLCD_print1616(6, 3, " STOP");
}

void PAGE_2210(void)
{
	GLCD_BufClear();	
	GLCD_print1616(6, 3, "RUN");
}




void PAGE_23(void)
{
	GLCD_BufClear();	
	cpy_flash2memory(&text_buf[0], (char*)&PAGE_SYSTEM_OPERATION[0][0]);		GLCD_print0508(5, 0, text_buf);
	cpy_flash2memory(&text_buf[0], (char*)&PAGE_SYSTEM_OPERATION[1][0]);		GLCD_print0508(2, 1, text_buf);
	cpy_flash2memory(&text_buf[0], (char*)&PAGE_SYSTEM_OPERATION[2][0]);		GLCD_print0508(2, 2, text_buf);
	cpy_flash2memory(&text_buf[0], (char*)&PAGE_SYSTEM_OPERATION[3][0]);		GLCD_print0508(2, 3, text_buf);
	cpy_flash2memory(&text_buf[0], (char*)&PAGE_SYSTEM_OPERATION[4][0]);		GLCD_print0508(2, 4, text_buf);
	cpy_flash2memory(&text_buf[0], (char*)&PAGE_SYSTEM_OPERATION[5][0]);		GLCD_print0508(2, 5, text_buf);
	GLCD_inversion(12,4,112);
}
void PAGE_230(void)
{
	GLCD_BufClear();	
	GLCD_print0508(5, 0,"--CONV MANUAL MODE--");
	GLCD_print0508(6, 2, "Would you like to");
	GLCD_print0508(6, 3, "   Transfer to BYP?");

	GLCD_print0508(0, 5, "    -YES-  -NO-");

	GLCD_inversion(65,5,24);
}

void PAGE_231(void)
{
	GLCD_BufClear();	
	GLCD_print0508(5, 0,"--CONV MANUAL MODE--");
	GLCD_print0508(6, 2, "Would you like to");
	GLCD_print0508(6, 3, "   Transfer to BYP?");

	GLCD_print0508(0, 5, "    -YES-  -NO-");
	GLCD_inversion(23,5,31);
}
void PAGE_2300(void)
{
	GLCD_BufClear();	
	GLCD_print1616(20, 3,"--CANCLE--");
}

void PAGE_2310(void)
{
	GLCD_BufClear();	
	GLCD_print1616(20, 3,"--SAVED--");
}



void PAGE_24(void)
{
	GLCD_BufClear();	
	cpy_flash2memory(&text_buf[0], (char*)&PAGE_SYSTEM_OPERATION[0][0]);		GLCD_print0508(5, 0, text_buf);
	cpy_flash2memory(&text_buf[0], (char*)&PAGE_SYSTEM_OPERATION[1][0]);		GLCD_print0508(2, 1, text_buf);
	cpy_flash2memory(&text_buf[0], (char*)&PAGE_SYSTEM_OPERATION[2][0]);		GLCD_print0508(2, 2, text_buf);
	cpy_flash2memory(&text_buf[0], (char*)&PAGE_SYSTEM_OPERATION[3][0]);		GLCD_print0508(2, 3, text_buf);
	cpy_flash2memory(&text_buf[0], (char*)&PAGE_SYSTEM_OPERATION[4][0]);		GLCD_print0508(2, 4, text_buf);
	cpy_flash2memory(&text_buf[0], (char*)&PAGE_SYSTEM_OPERATION[5][0]);		GLCD_print0508(2, 5, text_buf);
	GLCD_inversion(12,5,112);
}
void PAGE_240(void)
{
	GLCD_BufClear();	
	GLCD_print0508(5, 0,"--CONV MANUAL MODE--");
	GLCD_print0508(6, 2, "Would you like to");
	GLCD_print0508(6, 3, "   Transfer to INV?");

	GLCD_print0508(0, 5, "    -YES-  -NO-");
	GLCD_inversion(65,5,24);
}
void PAGE_241(void)
{
	GLCD_BufClear();	
	GLCD_print0508(5, 0,"--CONV MANUAL MODE--");
	GLCD_print0508(6, 2, "Would you like to");
	GLCD_print0508(6, 3, "   Transfer to INV?");

	GLCD_print0508(0, 5, "    -YES-  -NO-");

	GLCD_inversion(23,5,31);
}
void PAGE_2400(void)
{
	GLCD_BufClear();	
	GLCD_print1616(20, 3,"--CANCLE--");
}

void PAGE_2410(void)
{
	GLCD_BufClear();	
	GLCD_print1616(20, 3,"--SAVED--");
}



void PAGE_4(void)
{
	if(EventFlagC)
	{
		GLCD_BufClear();	
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_ROOT[0][0]);		GLCD_print0508(5, 0, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_ROOT[1][0]);		GLCD_print0508(2, 2, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_ROOT[2][0]);		GLCD_print0508(2, 3, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_ROOT[3][0]);		GLCD_print0508(2, 4, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_ROOT[4][0]);		GLCD_print0508(2, 5, text_buf);
		GLCD_inversion(12,5,112);
	}
	else
	{
		LoadDiagram();
	}
}


prog_uchar  PAGE_PARAMETER_SETTING[7][21]=
{
		"--PARAMETER SETTING--",
		"1.SET VIN LIMIT",
		"2.SET VOUT VALUE",
		"3.SET BATT VALUE ",
		"4.SET ASYNC FREQ",
		"5.SET GAIN/OFFSET",
		"6.SET PASSWORD/TIME"		
};

void PAGE_40(void)
{
	GLCD_BufClear();	

	if(confirm)
	{
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[0][0]);		GLCD_print0508(5, 0, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[1][0]);		GLCD_print0508(2, 1, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[2][0]);		GLCD_print0508(2, 2, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[3][0]);		GLCD_print0508(2, 3, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[4][0]);		GLCD_print0508(2, 4, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[5][0]);		GLCD_print0508(2, 5, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[6][0]);		GLCD_print0508(2, 6, text_buf);

		GLCD_inversion(12,1,112);
	}
	else
	{
		GLCD_print0508(2, 0,"--PASSWORD--");
		GLCD_print0508(40, 3,(char*) _TEXT("%d%d%d%d",_password[0],_password[1],_password[2],_password[3]));
		if(pass_pos==0)	GLCD_inversion(39,3,7);
		else if(pass_pos==1)GLCD_inversion(45,3,7);
		else if(pass_pos==2)GLCD_inversion(51,3,7);
		else if(pass_pos==3)GLCD_inversion(57,3,7);
	}
}


int Vin_upper_limit = 200;
int Vin_lower_limit = 100;
void PAGE_400(void)
{
	GLCD_BufClear();	

	if(confirm)
	{
		GLCD_print0508(5, 0,"--SET VIN LIMIT--");
		GLCD_print0508(6, 2, "UPPER LIMIT");
		GLCD_print0508(6, 3, (char*)_TEXT("%d[V]",Vin_upper_limit));
		GLCD_print0508(6, 4, "LOWER LIMIT");
		GLCD_print0508(6, 5,(char*) _TEXT("%d[V]",Vin_lower_limit));

		GLCD_inversion(5,3,40);
	}
	else
	{
		if((password[0]==_password[0])&&
		(password[1]==_password[1])&&
		(password[2]==_password[2])&&
		(password[3]==_password[3]))
		{
			GLCD_print0508(6, 2, "CONFIRMED");
		}
		else
		{
			GLCD_print0508(6, 2, "PASSWORD MISMATCH");
		}
	}
}

void PAGE_401(void)
{
	GLCD_BufClear();	
	GLCD_print0508(5, 0,"--SET VIN LIMIT--");
	GLCD_print0508(6, 2, "UPPER LIMIT");
	GLCD_print0508(6, 3, (char*)_TEXT("%d[V]",Vin_upper_limit));
	GLCD_print0508(6, 4, "LOWER LIMIT");
	GLCD_print0508(6, 5, (char*)_TEXT("%d[V]",Vin_lower_limit));

	GLCD_inversion(5,5,40);
}
void PAGE_4000(void)
{
	GLCD_BufClear();	
	GLCD_print1616(26, 3,"-SAVED-");
}


void PAGE_41(void)
{
	GLCD_BufClear();	
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[0][0]);		GLCD_print0508(5, 0, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[1][0]);		GLCD_print0508(2, 1, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[2][0]);		GLCD_print0508(2, 2, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[3][0]);		GLCD_print0508(2, 3, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[4][0]);		GLCD_print0508(2, 4, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[5][0]);		GLCD_print0508(2, 5, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[6][0]);		GLCD_print0508(2, 6, text_buf);

	GLCD_inversion(12,2,112);
}


prog_uchar  PAGE_VOUT_VALUE[3][21]=
{
	"--SET VOUT VALUE--",
	"1.SET VOUT LIMIT",
	"2.SET VOUT"
};

void PAGE_410(void)
{
	GLCD_BufClear();	
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_VOUT_VALUE[0][0]);		GLCD_print0508(5, 0, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_VOUT_VALUE[1][0]);		GLCD_print0508(2, 1, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_VOUT_VALUE[2][0]);		GLCD_print0508(2, 2, text_buf);
	GLCD_inversion(12,1,112);
}

int Vout_upper_limit = 200;
int Vout_lower_limit = 100;

void PAGE_4100(void)
{
	GLCD_BufClear();	
	GLCD_print0508(5, 0,"--SET VOUT LIMIT--");
	GLCD_print0508(6, 2, "UPPER LIMIT");
	GLCD_print0508(6, 3, (char*)_TEXT("%d[V]",Vout_upper_limit));
	GLCD_print0508(6, 4, "LOWER LIMIT");
	GLCD_print0508(6, 5, (char*)_TEXT("%d[V]",Vout_lower_limit));

	GLCD_inversion(5,3,40);
}

void PAGE_41000(void)
{
	GLCD_BufClear();	
	GLCD_print1616(20, 3,"--SAVED--");
}


void PAGE_4101(void)
{
	GLCD_BufClear();	
	GLCD_print0508(5, 0,"--SET VOUT LIMIT--");
	GLCD_print0508(6, 2, "UPPER LIMIT");
	GLCD_print0508(6, 3, (char*)_TEXT("%d[V]",Vout_upper_limit));
	GLCD_print0508(6, 4, "LOWER LIMIT");
	GLCD_print0508(6, 5, (char*)_TEXT("%d[V]",Vout_lower_limit));

	GLCD_inversion(5,5,40);
}

void PAGE_41010(void)
{
	GLCD_BufClear();	
	GLCD_print1616(20, 3,"--SAVED--");
}




void PAGE_411(void)
{
	GLCD_BufClear();	
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_VOUT_VALUE[0][0]);		GLCD_print0508(5, 0, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_VOUT_VALUE[1][0]);		GLCD_print0508(2, 1, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_VOUT_VALUE[2][0]);		GLCD_print0508(2, 2, text_buf);
	GLCD_inversion(12,2,112);
}



int Vinv_ref=220;
void PAGE_4110(void)
{
	GLCD_BufClear();	
	GLCD_print0508(5, 0,"--SET VOUT--");
	GLCD_print0508(5, 2, (char*)_TEXT("Vout:%03dV", main_data_array[2]));

	GLCD_print0508(5, 4, "UP : Inc VOUT");
	GLCD_print0508(5, 5, "DN : Dec VOUT");
	
}

void PAGE_41100(void)
{
	GLCD_BufClear();	
	GLCD_print1616(20, 3,"--SAVED--");
}





void PAGE_42(void)
{         
	GLCD_BufClear();	
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[0][0]);		GLCD_print0508(5, 0, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[1][0]);		GLCD_print0508(2, 1, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[2][0]);		GLCD_print0508(2, 2, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[3][0]);		GLCD_print0508(2, 3, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[4][0]);		GLCD_print0508(2, 4, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[5][0]);		GLCD_print0508(2, 5, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[6][0]);		GLCD_print0508(2, 6, text_buf);

	GLCD_inversion(12,3,112);
}


prog_uchar  PAGE_BATT_VALUE[6][21]=
{
	"--SET BATT VALUE--",
	"1.SET CURRENT LIMIT",
	"2.SET VOLTAGE LIMIT",
	"3.SET EQUAL VOLTAGE",
	"4.SET FLOAT VOLTAGE",
	"5.SET EQUAL CURRENT"
};

void PAGE_420(void)
{
	GLCD_BufClear();	
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_BATT_VALUE[0][0]);		GLCD_print0508(5, 0, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_BATT_VALUE[1][0]);		GLCD_print0508(2, 1, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_BATT_VALUE[2][0]);		GLCD_print0508(2, 2, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_BATT_VALUE[3][0]);		GLCD_print0508(2, 3, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_BATT_VALUE[4][0]);		GLCD_print0508(2, 4, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_BATT_VALUE[5][0]);		GLCD_print0508(2, 5, text_buf);

	GLCD_inversion(12,1,112);
}

int Abat_limit = 200;
void PAGE_4200(void)
{
	GLCD_BufClear();	
	GLCD_print0508(5, 0,"-SET CURRENT LIMIT-");
	GLCD_print0508(6, 2, "UPPER LIMIT");
	GLCD_print0508(6, 3, (char*)_TEXT("%d[V]",Abat_limit));

	GLCD_inversion(5,3,40);
}

void PAGE_42000(void)
{
	GLCD_BufClear();	
	GLCD_print1616(20, 3,"--SAVED--");
}




void PAGE_421(void)
{
	GLCD_BufClear();	
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_BATT_VALUE[0][0]);		GLCD_print0508(5, 0, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_BATT_VALUE[1][0]);		GLCD_print0508(2, 1, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_BATT_VALUE[2][0]);		GLCD_print0508(2, 2, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_BATT_VALUE[3][0]);		GLCD_print0508(2, 3, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_BATT_VALUE[4][0]);		GLCD_print0508(2, 4, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_BATT_VALUE[5][0]);		GLCD_print0508(2, 5, text_buf);

	GLCD_inversion(12,2,112);
}
int Vbat_upper_limit = 200;
int Vbat_lower_limit = 100;

void PAGE_4210(void)
{
	GLCD_BufClear();	
	GLCD_print0508(5, 0,"--SET VOLTAGE LIMIT--");
	GLCD_print0508(6, 2, "UPPER LIMIT");
	GLCD_print0508(6, 3, (char*)_TEXT("%d[V]",Vbat_upper_limit));
	GLCD_print0508(6, 4, "LOWER LIMIT");
	GLCD_print0508(6, 5, (char*)_TEXT("%d[V]",Vbat_lower_limit));

	GLCD_inversion(5,3,40);
}
void PAGE_4211(void)
{
	GLCD_BufClear();	
	GLCD_print0508(5, 0,"--SET CURRENT LIMIT--");
	GLCD_print0508(6, 2, "UPPER LIMIT");
	GLCD_print0508(6, 3, (char*)_TEXT("%d[V]",Vbat_upper_limit));
	GLCD_print0508(6, 4, "LOWER LIMIT");
	GLCD_print0508(6, 5, (char*)_TEXT("%d[V]",Vbat_lower_limit));

	GLCD_inversion(5,5,40);
}

void PAGE_42100(void)
{
	GLCD_BufClear();	
	GLCD_print1616(20, 3,"--SAVED--");
}

void PAGE_42110(void)
{
	GLCD_BufClear();	
	GLCD_print1616(20, 3,"--SAVED--");
}


void PAGE_422(void)
{
	GLCD_BufClear();	
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_BATT_VALUE[0][0]);		GLCD_print0508(5, 0, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_BATT_VALUE[1][0]);		GLCD_print0508(2, 1, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_BATT_VALUE[2][0]);		GLCD_print0508(2, 2, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_BATT_VALUE[3][0]);		GLCD_print0508(2, 3, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_BATT_VALUE[4][0]);		GLCD_print0508(2, 4, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_BATT_VALUE[5][0]);		GLCD_print0508(2, 5, text_buf);

	GLCD_inversion(12,3,112);

}
int Bat_Equal_volt = 200;
void PAGE_4220(void)
{
	GLCD_BufClear();	
	GLCD_print0508(5, 0,"--SET EQUAL VOLTAGE--");

	GLCD_print0508(6, 3, (char*)_TEXT("%d[V]",Bat_Equal_volt));


	GLCD_inversion(5,3,40);
}
void PAGE_42200(void)
{
	GLCD_BufClear();	
	GLCD_print1616(20, 3,"--SAVED--");
}


void PAGE_423(void)
{
	GLCD_BufClear();	
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_BATT_VALUE[0][0]);		GLCD_print0508(5, 0, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_BATT_VALUE[1][0]);		GLCD_print0508(2, 1, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_BATT_VALUE[2][0]);		GLCD_print0508(2, 2, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_BATT_VALUE[3][0]);		GLCD_print0508(2, 3, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_BATT_VALUE[4][0]);		GLCD_print0508(2, 4, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_BATT_VALUE[5][0]);		GLCD_print0508(2, 5, text_buf);

	GLCD_inversion(12,4,112);
}
int Bat_Float_Volt = 200;
void PAGE_4230(void)
{
	GLCD_BufClear();	
	GLCD_print0508(5, 0,"SET FLOAT VOLTAGE");
	GLCD_print0508(6, 3, (char*)_TEXT("%d[V]",Bat_Float_Volt));


	GLCD_inversion(5,3,40);
}
void PAGE_42300(void)
{
	GLCD_BufClear();	
	GLCD_print1616(20, 3,"--SAVED--");
}


void PAGE_424(void)
{
	GLCD_BufClear();	
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_BATT_VALUE[0][0]);		GLCD_print0508(5, 0, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_BATT_VALUE[1][0]);		GLCD_print0508(2, 1, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_BATT_VALUE[2][0]);		GLCD_print0508(2, 2, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_BATT_VALUE[3][0]);		GLCD_print0508(2, 3, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_BATT_VALUE[4][0]);		GLCD_print0508(2, 4, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_BATT_VALUE[5][0]);		GLCD_print0508(2, 5, text_buf);

	GLCD_inversion(12,5,112);
}
int Bat_Equal_Curr = 200;
void PAGE_4240(void)
{
	GLCD_BufClear();	
	GLCD_print0508(5, 0,"SET EQUAL CURRENT");

	GLCD_print0508(6, 3, (char*)_TEXT("%d[A]",Bat_Equal_Curr));


	GLCD_inversion(5,3,40);
}
void PAGE_42400(void)
{
	GLCD_BufClear();	
	GLCD_print1616(20, 3,"--SAVED--");
}



void PAGE_43(void)
{
	GLCD_BufClear();	
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[0][0]);		GLCD_print0508(5, 0, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[1][0]);		GLCD_print0508(2, 1, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[2][0]);		GLCD_print0508(2, 2, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[3][0]);		GLCD_print0508(2, 3, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[4][0]);		GLCD_print0508(2, 4, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[5][0]);		GLCD_print0508(2, 5, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[6][0]);		GLCD_print0508(2, 6, text_buf);

	GLCD_inversion(12,4,112);
}

int Async_Upper_Limit=60;
int Async_Lower_Limit=60;
void PAGE_430(void)
{
	GLCD_BufClear();	
	GLCD_print0508(5, 0,"--SET ASYNC FREQ--");
	GLCD_print0508(6, 2, "UPPER LIMIT");
	GLCD_print0508(6, 3, (char*)_TEXT("%d[Hz]",Async_Upper_Limit));
	GLCD_print0508(6, 4, "LOWER LIMIT");
	GLCD_print0508(6, 5, (char*)_TEXT("%d[Hz]",Async_Lower_Limit));

	GLCD_inversion(5,3,40);
}
void PAGE_431(void)
{
	GLCD_BufClear();	
	GLCD_print0508(5, 0,"--SET ASYNC FREQ--");
	GLCD_print0508(6, 2, "UPPER LIMIT");
	GLCD_print0508(6, 3, (char*)_TEXT("%d[Hz]",Async_Upper_Limit));
	GLCD_print0508(6, 4, "LOWER LIMIT");
	GLCD_print0508(6, 5, (char*)_TEXT("%d[Hz]",Async_Lower_Limit));

	GLCD_inversion(5,3,40);
}

void PAGE_4300(void)
{
	GLCD_BufClear();	
	GLCD_print1616(20, 3,"--SAVED--");
}


void PAGE_44(void)
{
	GLCD_BufClear();	
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[0][0]);		GLCD_print0508(5, 0, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[1][0]);		GLCD_print0508(2, 1, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[2][0]);		GLCD_print0508(2, 2, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[3][0]);		GLCD_print0508(2, 3, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[4][0]);		GLCD_print0508(2, 4, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[5][0]);		GLCD_print0508(2, 5, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[6][0]);		GLCD_print0508(2, 6, text_buf);

	GLCD_inversion(12,5,112);
}


void PAGE_440(void)
{
	GLCD_BufClear();	
	GLCD_print0508(5, 0,"--SET GAIN/OFFSET--");
	GLCD_print0508(2, 2,"1.SET GAIN/OFFSET");
	GLCD_print0508(2, 3,"2.AUTO SET VALUE");

	GLCD_inversion(12,2,112);
}


int Vin_gain=0;
unsigned int Vin_offset=0;
void PAGE_4400(void)
{
	GLCD_BufClear();	
	GLCD_print0508(2, 0,"--VIN GAIN/OFFSET--");
	GLCD_print0508(2, 2,(char*)_TEXT("%dV",main_data_array[0]));
	GLCD_print0508(2, 3,(char*)_TEXT("OFFSET:%+05u",Vin_offset));
	GLCD_print0508(2, 4,(char*)_TEXT("GAIN  :%+05d",Vin_gain));

}
void PAGE_44000(void)
{
	GLCD_BufClear();	
	GLCD_print0508(2, 0,"--VIN GAIN/OFFSET--");
	GLCD_print0508(2, 2,(char*)_TEXT("%dV",main_data_array[0]));
	GLCD_print0508(2, 3,(char*)_TEXT("OFFSET:%+05u",Vin_offset));
	GLCD_print0508(2, 4,(char*)_TEXT("GAIN  :%+05d",Vin_gain));

	if(posInpage==0)GLCD_inversion(0,3,128);
	else if(posInpage==1)GLCD_inversion(49,3,7);
	else if(posInpage==2)GLCD_inversion(55,3,7);
	else if(posInpage==3)GLCD_inversion(61,3,7);
	else if(posInpage==4)GLCD_inversion(67,3,7);
	else if(posInpage==5)GLCD_inversion(73,3,7);
	else if(posInpage==6)GLCD_inversion(79,3,7);
}
void PAGE_44001(void)
{
	GLCD_BufClear();	
	GLCD_print0508(2, 0,"--VIN GAIN/OFFSET--");
	GLCD_print0508(2, 2,(char*)_TEXT("%dV",main_data_array[0]));
	GLCD_print0508(2, 3,(char*)_TEXT("OFFSET:%+05u",Vin_offset));
	GLCD_print0508(2, 4,(char*)_TEXT("GAIN  :%+05d",Vin_gain));

	if(posInpage==0)GLCD_inversion(0,4,128);
	else if(posInpage==1)GLCD_inversion(49,4,7);
	else if(posInpage==2)GLCD_inversion(55,4,7);
	else if(posInpage==3)GLCD_inversion(61,4,7);
	else if(posInpage==4)GLCD_inversion(67,4,7);
	else if(posInpage==5)GLCD_inversion(73,4,7);
	else if(posInpage==6)GLCD_inversion(79,4,7);

}

void PAGE_440000(void)
{
	GLCD_BufClear();	
	GLCD_print1616(20, 3,"--SAVED--");
	GLCD_print0508(2, 5,(char*)_TEXT("OFFSET:%+05u",Vin_offset));
	GLCD_print0508(2, 6,(char*)_TEXT("GAIN  :%+05d",Vin_gain));
}


int Ain_gain=0;
unsigned int Ain_offset=0;
void PAGE_4401(void)
{
	GLCD_BufClear();	
	GLCD_print0508(2, 0,"--AIN GAIN/OFFSET--");
	GLCD_print0508(2, 2,(char*) _TEXT("%02d.%dA", main_data_array[1]/10,main_data_array[1]%10));
	GLCD_print0508(2, 3,(char*)_TEXT("OFFSET:%+05u", Ain_offset));
	GLCD_print0508(2, 4,(char*)_TEXT("GAIN  :%+05d", Ain_gain));

}
void PAGE_44010(void)
{
	GLCD_BufClear();	
	GLCD_print0508(2, 0,"--AIN GAIN/OFFSET--");
	GLCD_print0508(2, 2,(char*) _TEXT("%02d.%dA", main_data_array[1]/10,main_data_array[1]%10));
	GLCD_print0508(2, 3,(char*)_TEXT("OFFSET:%+05u",Ain_offset));
	GLCD_print0508(2, 4,(char*)_TEXT("GAIN  :%+05d",Ain_gain));

	if(posInpage==0)GLCD_inversion(0,3,128);
	else if(posInpage==1)GLCD_inversion(49,3,7);
	else if(posInpage==2)GLCD_inversion(55,3,7);
	else if(posInpage==3)GLCD_inversion(61,3,7);
	else if(posInpage==4)GLCD_inversion(67,3,7);
	else if(posInpage==5)GLCD_inversion(73,3,7);
	else if(posInpage==6)GLCD_inversion(79,3,7);
}
void PAGE_44011(void)
{
	GLCD_BufClear();	
	GLCD_print0508(2, 0,"--AIN GAIN/OFFSET--");
	GLCD_print0508(2, 2,(char*) _TEXT("%02d.%dA", main_data_array[1]/10,main_data_array[1]%10));
	GLCD_print0508(2, 3,(char*)_TEXT("OFFSET:%+05u",Ain_offset));
	GLCD_print0508(2, 4,(char*)_TEXT("GAIN  :%+05d",Ain_gain));

	if(posInpage==0)GLCD_inversion(0,4,128);
	else if(posInpage==1)GLCD_inversion(49,4,7);
	else if(posInpage==2)GLCD_inversion(55,4,7);
	else if(posInpage==3)GLCD_inversion(61,4,7);
	else if(posInpage==4)GLCD_inversion(67,4,7);
	else if(posInpage==5)GLCD_inversion(73,4,7);
	else if(posInpage==6)GLCD_inversion(79,4,7);
}
void PAGE_440100(void)
{
	GLCD_BufClear();	
	GLCD_print1616(20, 3,"--SAVED--");
	GLCD_print0508(2, 5,(char*)_TEXT("OFFSET:%+05u",Ain_offset));
	GLCD_print0508(2, 6,(char*)_TEXT("GAIN  :%+05d",Ain_gain));
}


int Vout_gain=0;
unsigned int Vout_offset=0;
void PAGE_4402(void)
{
	GLCD_BufClear();	
	GLCD_print0508(2, 0,"--VOUT GAIN/OFFSET--");
	GLCD_print0508(2, 2,(char*)_TEXT("%dV",main_data_array[2]));
	GLCD_print0508(2, 3,(char*)_TEXT("OFFSET:%+05u",Vout_offset));
	GLCD_print0508(2, 4,(char*)_TEXT("GAIN  :%+05d",Vout_gain));

}
void PAGE_44020(void)
{
	GLCD_BufClear();	
	GLCD_print0508(2, 0,"--VOUT GAIN/OFFSET--");
	GLCD_print0508(2, 2,(char*)_TEXT("%dV",main_data_array[2]));
	GLCD_print0508(2, 3,(char*)_TEXT("OFFSET:%+05u",Vout_offset));
	GLCD_print0508(2, 4,(char*)_TEXT("GAIN  :%+05d",Vout_gain));

	if(posInpage==0)GLCD_inversion(0,3,128);
	else if(posInpage==1)GLCD_inversion(49,3,7);
	else if(posInpage==2)GLCD_inversion(55,3,7);
	else if(posInpage==3)GLCD_inversion(61,3,7);
	else if(posInpage==4)GLCD_inversion(67,3,7);
	else if(posInpage==5)GLCD_inversion(73,3,7);
	else if(posInpage==6)GLCD_inversion(79,3,7);
}
void PAGE_44021(void)
{
	GLCD_BufClear();	
	GLCD_print0508(2, 0,"--VOUT GAIN/OFFSET--");
	GLCD_print0508(2, 2,(char*)_TEXT("%dV",main_data_array[2]));
	GLCD_print0508(2, 3,(char*)_TEXT("OFFSET:%+05u",Vout_offset));
	GLCD_print0508(2, 4,(char*)_TEXT("GAIN  :%+05d",Vout_gain));

	if(posInpage==0)GLCD_inversion(0,4,128);
	else if(posInpage==1)GLCD_inversion(49,4,7);
	else if(posInpage==2)GLCD_inversion(55,4,7);
	else if(posInpage==3)GLCD_inversion(61,4,7);
	else if(posInpage==4)GLCD_inversion(67,4,7);
	else if(posInpage==5)GLCD_inversion(73,4,7);
	else if(posInpage==6)GLCD_inversion(79,4,7);
}
void PAGE_440200(void)
{
	GLCD_BufClear();	
	GLCD_print1616(20, 3,"--SAVED--");
	GLCD_print0508(2, 5,(char*)_TEXT("OFFSET:%+05u",Vout_offset));
	GLCD_print0508(2, 6,(char*)_TEXT("GAIN  :%+05d",Vout_gain));
}


int Aout_gain=0;
unsigned int Aout_offset=0;
void PAGE_4403(void)
{
	GLCD_BufClear();	
	GLCD_print0508(2, 0,"--AOUT GAIN/OFFSET--");
	GLCD_print0508(2, 2,(char*)_TEXT("%02d.%dA", main_data_array[3]/10,main_data_array[3]%10));
	GLCD_print0508(2, 3,(char*)_TEXT("OFFSET:%+05u",Aout_offset));
	GLCD_print0508(2, 4,(char*)_TEXT("GAIN  :%+05d",Aout_gain));
}
void PAGE_44030(void)
{
	GLCD_BufClear();	
	GLCD_print0508(2, 0,"--AOUT GAIN/OFFSET--");
	GLCD_print0508(2, 2,(char*)_TEXT("%02d.%dA", main_data_array[3]/10,main_data_array[3]%10));
	GLCD_print0508(2, 3,(char*)_TEXT("OFFSET:%+05u",Aout_offset));
	GLCD_print0508(2, 4,(char*)_TEXT("GAIN  :%+05d",Aout_gain));

	if(posInpage==0)GLCD_inversion(0,3,128);
	else if(posInpage==1)GLCD_inversion(49,3,7);
	else if(posInpage==2)GLCD_inversion(55,3,7);
	else if(posInpage==3)GLCD_inversion(61,3,7);
	else if(posInpage==4)GLCD_inversion(67,3,7);
	else if(posInpage==5)GLCD_inversion(73,3,7);
	else if(posInpage==6)GLCD_inversion(79,3,7);
}
void PAGE_44031(void)
{
	GLCD_BufClear();	
	GLCD_print0508(2, 0,"--AOUT GAIN/OFFSET--");
	GLCD_print0508(2, 2,(char*)_TEXT("%02d.%dA", main_data_array[3]/10,main_data_array[3]%10));
	GLCD_print0508(2, 3,(char*)_TEXT("OFFSET:%+05u",Aout_offset));
	GLCD_print0508(2, 4,(char*)_TEXT("GAIN  :%+05d",Aout_gain));

	if(posInpage==0)GLCD_inversion(0,4,128);
	else if(posInpage==1)GLCD_inversion(49,4,7);
	else if(posInpage==2)GLCD_inversion(55,4,7);
	else if(posInpage==3)GLCD_inversion(61,4,7);
	else if(posInpage==4)GLCD_inversion(67,4,7);
	else if(posInpage==5)GLCD_inversion(73,4,7);
	else if(posInpage==6)GLCD_inversion(79,4,7);
}
void PAGE_440300(void)
{
	GLCD_BufClear();	
	GLCD_print1616(20, 3,"--SAVED--");
	GLCD_print0508(2, 5,(char*)_TEXT("OFFSET:%+05u",Aout_offset));
	GLCD_print0508(2, 6,(char*)_TEXT("GAIN  :%+05d",Aout_gain));
}



int Vbat_gain=0;
unsigned int Vbat_offset=0;
void PAGE_4404(void)
{
	GLCD_BufClear();	
	GLCD_print0508(2, 0,"-BATT V GAIN/OFFSET-");
	GLCD_print0508(2, 2,(char*)_TEXT("%dV",main_data_array[4]));
	GLCD_print0508(2, 3,(char*)_TEXT("OFFSET:%+05u",Vbat_offset));
	GLCD_print0508(2, 4,(char*)_TEXT("GAIN  :%+05d",Vbat_gain));
}
void PAGE_44040(void)
{
	GLCD_BufClear();	
	GLCD_print0508(2, 0,"-BATT V GAIN/OFFSET-");
	GLCD_print0508(2, 2,(char*)_TEXT("%dV",main_data_array[4]));
	GLCD_print0508(2, 3,(char*)_TEXT("OFFSET:%+05u",Vbat_offset));
	GLCD_print0508(2, 4,(char*)_TEXT("GAIN  :%+05d",Vbat_gain));

	if(posInpage==0)GLCD_inversion(0,3,128);
	else if(posInpage==1)GLCD_inversion(49,3,7);
	else if(posInpage==2)GLCD_inversion(55,3,7);
	else if(posInpage==3)GLCD_inversion(61,3,7);
	else if(posInpage==4)GLCD_inversion(67,3,7);
	else if(posInpage==5)GLCD_inversion(73,3,7);
	else if(posInpage==6)GLCD_inversion(79,3,7);
}
void PAGE_44041(void)
{
	GLCD_BufClear();	
	GLCD_print0508(2, 0,"-BATT V GAIN/OFFSET-");
	GLCD_print0508(2, 2,(char*)_TEXT("%dV",main_data_array[4]));
	GLCD_print0508(2, 3,(char*)_TEXT("OFFSET:%+05u",Vbat_offset));
	GLCD_print0508(2, 4,(char*)_TEXT("GAIN  :%+05d",Vbat_gain));

	if(posInpage==0)GLCD_inversion(0,4,128);
	else if(posInpage==1)GLCD_inversion(49,4,7);
	else if(posInpage==2)GLCD_inversion(55,4,7);
	else if(posInpage==3)GLCD_inversion(61,4,7);
	else if(posInpage==4)GLCD_inversion(67,4,7);
	else if(posInpage==5)GLCD_inversion(73,4,7);
	else if(posInpage==6)GLCD_inversion(79,4,7);
}
void PAGE_440400(void)
{
	GLCD_BufClear();	
	GLCD_print1616(20, 3,"--SAVED--");
	GLCD_print0508(2, 5,(char*)_TEXT("OFFSET:%+05u",Vbat_offset));
	GLCD_print0508(2, 6,(char*)_TEXT("GAIN  :%+05d",Vbat_gain));
}


int Abat_gain=0;
unsigned int Abat_offset=0;
void PAGE_4405(void)
{
	GLCD_BufClear();	
	GLCD_print0508(2, 0,"-BATT I GAIN/OFFSET-");
	GLCD_print0508(2, 2,(char*)_TEXT("%02d.%dA", main_data_array[5]/10,main_data_array[5]%10));
	GLCD_print0508(2, 3,(char*)_TEXT("OFFSET:%+05u",Abat_offset));
	GLCD_print0508(2, 4,(char*)_TEXT("GAIN  :%+05d",Abat_gain));

}
void PAGE_44050(void)
{
	GLCD_BufClear();	
	GLCD_print0508(2, 0,"-BATT I GAIN/OFFSET-");
	GLCD_print0508(2, 2,(char*)_TEXT("%02d.%dA", main_data_array[5]/10,main_data_array[5]%10));
	GLCD_print0508(2, 3,(char*)_TEXT("OFFSET:%+05u",Abat_offset));
	GLCD_print0508(2, 4,(char*)_TEXT("GAIN  :%+05d",Abat_gain));

	if(posInpage==0)GLCD_inversion(0,3,128);
	else if(posInpage==1)GLCD_inversion(49,3,7);
	else if(posInpage==2)GLCD_inversion(55,3,7);
	else if(posInpage==3)GLCD_inversion(61,3,7);
	else if(posInpage==4)GLCD_inversion(67,3,7);
	else if(posInpage==5)GLCD_inversion(73,3,7);
	else if(posInpage==6)GLCD_inversion(79,3,7);
}
void PAGE_44051(void)
{
	GLCD_BufClear();	
	GLCD_print0508(2, 0,"-BATT I GAIN/OFFSET-");
	GLCD_print0508(2, 2,(char*)_TEXT("%02d.%dA", main_data_array[5]/10,main_data_array[5]%10));
	GLCD_print0508(2, 3,(char*)_TEXT("OFFSET:%+05u",Abat_offset));
	GLCD_print0508(2, 4,(char*)_TEXT("GAIN  :%+05d",Abat_gain));

	if(posInpage==0)GLCD_inversion(0,4,128);
	else if(posInpage==1)GLCD_inversion(49,4,7);
	else if(posInpage==2)GLCD_inversion(55,4,7);
	else if(posInpage==3)GLCD_inversion(61,4,7);
	else if(posInpage==4)GLCD_inversion(67,4,7);
	else if(posInpage==5)GLCD_inversion(73,4,7);
	else if(posInpage==6)GLCD_inversion(79,4,7);

}
void PAGE_440500(void)
{
	GLCD_BufClear();	
	GLCD_print1616(20, 3,"--SAVED--");
	GLCD_print0508(2, 5,(char*)_TEXT("OFFSET:%+05u",Abat_offset));
	GLCD_print0508(2, 6,(char*)_TEXT("GAIN  :%+05d",Abat_gain));
}



int Vbyp_gain=0;
unsigned int Vbyp_offset=0;
void PAGE_4406(void)
{
	GLCD_BufClear();	
	GLCD_print0508(2, 0,"-BYPASS GAIN/OFFSET-");
	GLCD_print0508(2, 2,(char*)_TEXT("%dV",main_data_array[2]));
	GLCD_print0508(2, 3,(char*)_TEXT("OFFSET:%+05u",Vbyp_offset));
	GLCD_print0508(2, 4,(char*)_TEXT("GAIN  :%+05d",Vbyp_gain));


}
void PAGE_44060(void)
{
	GLCD_BufClear();	
	GLCD_print0508(2, 0,"-BYPASS GAIN/OFFSET-");
	GLCD_print0508(2, 2,(char*)_TEXT("%dV",main_data_array[2]));
	GLCD_print0508(2, 3,(char*)_TEXT("OFFSET:%+05u",Vbyp_offset));
	GLCD_print0508(2, 4,(char*)_TEXT("GAIN  :%+05d",Vbyp_gain));

	if(posInpage==0)GLCD_inversion(0,3,128);
	else if(posInpage==1)GLCD_inversion(49,3,7);
	else if(posInpage==2)GLCD_inversion(55,3,7);
	else if(posInpage==3)GLCD_inversion(61,3,7);
	else if(posInpage==4)GLCD_inversion(67,3,7);
	else if(posInpage==5)GLCD_inversion(73,3,7);
	else if(posInpage==6)GLCD_inversion(79,3,7);

}
void PAGE_44061(void)
{
	GLCD_BufClear();	
	GLCD_print0508(2, 0,"-BYPASS GAIN/OFFSET-");
	GLCD_print0508(2, 2,(char*)_TEXT("%dV",main_data_array[2]));
	GLCD_print0508(2, 3,(char*)_TEXT("OFFSET:%+05u",Vbyp_offset));
	GLCD_print0508(2, 4,(char*)_TEXT("GAIN  :%+05d",Vbyp_gain));

	if(posInpage==0)GLCD_inversion(0,4,128);
	else if(posInpage==1)GLCD_inversion(49,4,7);
	else if(posInpage==2)GLCD_inversion(55,4,7);
	else if(posInpage==3)GLCD_inversion(61,4,7);
	else if(posInpage==4)GLCD_inversion(67,4,7);
	else if(posInpage==5)GLCD_inversion(73,4,7);
	else if(posInpage==6)GLCD_inversion(79,4,7);
}
void PAGE_440600(void)
{
	GLCD_BufClear();	
	GLCD_print1616(20, 3,"--SAVED--");
	GLCD_print0508(2, 5,(char*)_TEXT("OFFSET:%+05u",Vbyp_offset));
	GLCD_print0508(2, 6,(char*)_TEXT("GAIN  :%+05d",Vbyp_gain));
}


int Vdc_gain=0;
unsigned int Vdc_offset=0;
void PAGE_4407(void)
{
	GLCD_BufClear();	
	GLCD_print0508(2, 0,"-VDC GAIN/OFFSET-");
	GLCD_print0508(2, 2,(char*)_TEXT("%dV",main_data_array[6]));
	GLCD_print0508(2, 3,(char*)_TEXT("OFFSET:%+05u",Vdc_offset));
	GLCD_print0508(2, 4,(char*)_TEXT("GAIN  :%+05d",Vdc_gain));


}
void PAGE_44070(void)
{
	GLCD_BufClear();	
	GLCD_print0508(2, 0,"-VDC GAIN/OFFSET-");
	GLCD_print0508(2, 2,(char*)_TEXT("%dV",main_data_array[6]));
	GLCD_print0508(2, 3,(char*)_TEXT("OFFSET:%+05u",Vdc_offset));
	GLCD_print0508(2, 4,(char*)_TEXT("GAIN  :%+05d",Vdc_gain));

	if(posInpage==0)GLCD_inversion(0,3,128);
	else if(posInpage==1)GLCD_inversion(49,3,7);
	else if(posInpage==2)GLCD_inversion(55,3,7);
	else if(posInpage==3)GLCD_inversion(61,3,7);
	else if(posInpage==4)GLCD_inversion(67,3,7);
	else if(posInpage==5)GLCD_inversion(73,3,7);
	else if(posInpage==6)GLCD_inversion(79,3,7);
}
void PAGE_44071(void)
{
	GLCD_BufClear();	
	GLCD_print0508(2, 0,"-VDC GAIN/OFFSET-");
	GLCD_print0508(2, 2,(char*)_TEXT("%dV",main_data_array[6]));
	GLCD_print0508(2, 3,(char*)_TEXT("OFFSET:%+05u",Vdc_offset));
	GLCD_print0508(2, 4,(char*)_TEXT("GAIN  :%+05d",Vdc_gain));

	if(posInpage==0)GLCD_inversion(0,4,128);
	else if(posInpage==1)GLCD_inversion(49,4,7);
	else if(posInpage==2)GLCD_inversion(55,4,7);
	else if(posInpage==3)GLCD_inversion(61,4,7);
	else if(posInpage==4)GLCD_inversion(67,4,7);
	else if(posInpage==5)GLCD_inversion(73,4,7);
	else if(posInpage==6)GLCD_inversion(79,4,7);
}
void PAGE_440700(void)
{
	GLCD_BufClear();	
	GLCD_print1616(20, 3,"--SAVED--");
	GLCD_print0508(2, 5,(char*)_TEXT("OFFSET:%+05u",Vdc_offset));
	GLCD_print0508(2, 6,(char*)_TEXT("GAIN  :%+05d",Vdc_gain));
}

void PAGE_441(void)
{
	GLCD_BufClear();	
	GLCD_print0508(5, 0,"--SET GAIN/OFFSET--");
	GLCD_print0508(2, 2,"1.SET GAIN/OFFSET");
	GLCD_print0508(2, 3,"2.AUTO SET VALUE");
	
	GLCD_inversion(12,3,112);
}

void PAGE_4410(void)
{
	GLCD_BufClear();	
	GLCD_print0508(2, 3,"--AUTO SETTING--");
}


void PAGE_45(void)
{
	GLCD_BufClear();	
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[0][0]);		GLCD_print0508(5, 0, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[1][0]);		GLCD_print0508(2, 1, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[2][0]);		GLCD_print0508(2, 2, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[3][0]);		GLCD_print0508(2, 3, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[4][0]);		GLCD_print0508(2, 4, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[5][0]);		GLCD_print0508(2, 5, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_PARAMETER_SETTING[6][0]);		GLCD_print0508(2, 6, text_buf);

	GLCD_inversion(12,6,112);
}
void PAGE_450(void)
{
	GLCD_BufClear();	
	GLCD_print0508(5, 0,"SET PASSWORD/TIME");
	GLCD_print0508(2, 1, "1.SET PASSWORD");
	GLCD_print0508(2, 2, "2.SET TIME");

	GLCD_inversion(12,1,112);
}
void PAGE_451(void)
{
	GLCD_BufClear();	
	GLCD_print0508(5, 0,"SET PASSWORD/TIME");
	GLCD_print0508(2, 1, "1.SET PASSWORD");
	GLCD_print0508(2, 2, "2.SET TIME");

	GLCD_inversion(12,2,112);
}

void PAGE_4500(void)
{
	GLCD_BufClear();	
	GLCD_print0508(5, 0,"--SET PASSWORD--");
	GLCD_print0508(40, 3,(char*) _TEXT("%d%d%d%d",_password[0],_password[1],_password[2],_password[3]));
	if(pass_pos==0)	GLCD_inversion(39,3,7);
	else if(pass_pos==1)GLCD_inversion(45,3,7);
	else if(pass_pos==2)GLCD_inversion(51,3,7);
	else if(pass_pos==3)GLCD_inversion(57,3,7);

}
void PAGE_4510(void)
{
	GLCD_BufClear();	
	GLCD_print0508(5, 0,"--SET TIME--");
	GLCD_print0508(7, 3,(char*)_TEXT("20%02d/%02d/%02d %02d:%02d:%02d",setTime[0],setTime[1],setTime[2],setTime[3],setTime[4],setTime[5]));
	if(Tset_pos==0)	GLCD_inversion(6,3,25);
	else if(Tset_pos==1)GLCD_inversion(36,3,13);
	else if(Tset_pos==2)GLCD_inversion(54,3,13);
	else if(Tset_pos==3)GLCD_inversion(73,3,13);
	else if(Tset_pos==4)GLCD_inversion(90,3,13);
	else if(Tset_pos==5)GLCD_inversion(108,3,13);

}

void PAGE_45000(void)
{
	GLCD_BufClear();	
	GLCD_print1616(20, 3,"--SAVED--");

}
void PAGE_45100(void)
{
	GLCD_BufClear();	
	GLCD_print1616(20, 3,"--SAVED--");

}

void PAGE_3(void)
{
	if(EventFlagC)
	{
		GLCD_BufClear();	
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_ROOT[0][0]);		GLCD_print0508(5, 0, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_ROOT[1][0]);		GLCD_print0508(2, 2, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_ROOT[2][0]);		GLCD_print0508(2, 3, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_ROOT[3][0]);		GLCD_print0508(2, 4, text_buf);
		cpy_flash2memory(&text_buf[0], (char*)&PAGE_ROOT[4][0]);		GLCD_print0508(2, 5, text_buf);
		GLCD_inversion(12,4,112);
	}
	else
	{
		LoadDiagram();
	}
}



void PAGE_30(void)
{
	unsigned int i;
	unsigned char c;
	unsigned char offset1,offset2;
	
	GLCD_BufClear();	
	
	while(1)
	{
		if(0xFF == eeprom_read_byte ((char*)((history_Yoffset%HISTORY_MAX)*HISTORY_CHAR_MAX)))
		{
			if(history_Yoffset==0)break;
			 history_Yoffset++;
			if( HISTORY_MAX < history_Yoffset ) history_Yoffset = 0;
		}
		else
		{
			break;
		}
	}

	for(c=0;c<40;c++)
	{
		history_paper[0][c] = eeprom_read_byte ((char*)((history_Yoffset%HISTORY_MAX)*HISTORY_CHAR_MAX)+c) ;
		history_paper[1][c] = eeprom_read_byte ((char*)(((history_Yoffset+1)%HISTORY_MAX)*HISTORY_CHAR_MAX)+c) ;
		history_paper[2][c] = eeprom_read_byte ((char*)(((history_Yoffset+2)%HISTORY_MAX)*HISTORY_CHAR_MAX)+c) ;
		history_paper[3][c] = eeprom_read_byte ((char*)(((history_Yoffset+3)%HISTORY_MAX)*HISTORY_CHAR_MAX)+c) ;
		history_paper[4][c] = eeprom_read_byte ((char*)(((history_Yoffset+4)%HISTORY_MAX)*HISTORY_CHAR_MAX)+c) ;
		history_paper[5][c] = eeprom_read_byte ((char*)(((history_Yoffset+5)%HISTORY_MAX)*HISTORY_CHAR_MAX)+c) ;
	}

	GLCD_print0508(5, 0,"--HISTORY--");
	GLCD_print0508(0, 1, &history_paper[0][history_Xoffset] );
	GLCD_print0508(0, 2, &history_paper[1][history_Xoffset] );
	GLCD_print0508(0, 3, &history_paper[2][history_Xoffset] );
	GLCD_print0508(0, 4, &history_paper[3][history_Xoffset] );
	GLCD_print0508(0, 5, &history_paper[4][history_Xoffset] );
	GLCD_print0508(0, 6, &history_paper[5][history_Xoffset] );

	//if(history_paper[5][0]=='E' )
	//{
	//	GLCD_print0508(0, 6, "EEPROM FULL!!");
	//}
}

void PAGE_0xFFFFFFFF(void)
{
	GLCD_BufClear();	
	GLCD_print1616(20, 3,"박병훈 만세");

}






void SYS_1(void)
{
	static unsigned char flag=0;

	if(EventFlagC)
	{
		if(KeyState.KeyValue == ESC)
		{
			//naviMENU = naviMENU / 10;
			EventFlagC=0;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
		else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
		else if(KeyState.KeyValue == ENTER)naviMENU = naviMENU * 10;
		else if(KeyState.KeyValue == (ESC  & UP & DN & ENTER))
		{
			naviMENU = 0xFFFFFFFF;
		}
		
		if(KeyState.KeyValue != 0xFF)EventTimeC=0;
	}
	else 
	{
		if(KeyState.KeyValue != 0xFF)EventFlagC=1;
		if(naviMENU!=old_naveMENU)EventFlagC=1;
	}

	if((!EventFlagC)&&(flag))RefreshFlag=1;// event edge
	else if((EventFlagC)&&(!flag))RefreshFlag=1;// event edge
	flag = EventFlagC;
}

void SYS_10(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == (ESC&ENTER))naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == ENTER)naviMENU = naviMENU * 10;

	if((data_array[0] != main_data_array[0]) ||(data_array[1] != main_data_array[1])||
		(data_array[2] != main_data_array[2])||(data_array[3] != main_data_array[3])||
		(data_array[4] != main_data_array[4])||(data_array[5] != main_data_array[5])||
		(data_array[6] != main_data_array[6])||(data_array[7] != main_data_array[7])||
		(data_array[8] != main_data_array[8])||(data_array[9] != main_data_array[9])||
		(data_array[12] != main_data_array[12]))
		{
			data_array[0] = main_data_array[0];
			data_array[1] = main_data_array[1];
			data_array[2] = main_data_array[2];
			data_array[3] = main_data_array[3];
			data_array[4] = main_data_array[4];
			data_array[5] = main_data_array[5];
			data_array[6] = main_data_array[6];
			data_array[7] = main_data_array[7];
			data_array[8] = main_data_array[8];
			data_array[9] = main_data_array[9];
			data_array[12] = main_data_array[12];
			RefreshFlag=1;
		}
}

void SYS_11(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == ENTER)naviMENU = naviMENU * 10;

	if((data_array[10] != main_data_array[10]) ||(data_array[11] != main_data_array[11])||
		(data_array[13] != main_data_array[13])||(data_array[14] != main_data_array[14])||
		(data_array[15] != main_data_array[15])||(data_array[16] != main_data_array[16])||
		(data_array[17] != main_data_array[17])||(data_array[18] != main_data_array[18]))
		{
			data_array[10] = main_data_array[10];
			data_array[11] = main_data_array[11];
			data_array[13] = main_data_array[13];
			data_array[14] = main_data_array[14];
			data_array[15] = main_data_array[15];
			data_array[16] = main_data_array[16];
			data_array[17] = main_data_array[17];
			data_array[18] = main_data_array[18];
			RefreshFlag=1;
		}
}

void SYS_2(void)
{
	static unsigned char flag=0;

	if(EventFlagC)
	{
		if(KeyState.KeyValue == ESC)
		{
			EventFlagC=0;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
		else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
		else if(KeyState.KeyValue == ENTER)naviMENU = naviMENU * 10;
		else if(KeyState.KeyValue == (ESC  & UP & DN & ENTER))
		{
			naviMENU = 0xFFFFFFFF;
		}
		
		if(KeyState.KeyValue != 0xFF)EventTimeC=0;
	}
	else 
	{
		if(KeyState.KeyValue != 0xFF)EventFlagC=1;
		if(naviMENU!=old_naveMENU)EventFlagC=1;
	}

	if((!EventFlagC)&&(flag))RefreshFlag=1;// event edge
	else if((EventFlagC)&&(!flag))RefreshFlag=1;// event edge
	flag = EventFlagC;
}

void SYS_20(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == ENTER)naviMENU = naviMENU * 10;
}

void SYS_200(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == RIGHT)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == LIFT)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == ENTER)
	{
		naviMENU = naviMENU * 10;
		QUERY_buf1.address = 94;
		QUERY_buf1.length = 0x00;
		QUERY_buf1.function = MODBUS_WRITE_REGISTER;
		QUERY_buf1.node_id = MODBUS_NODE_ID;
		packet_handler = 1;
		EventFlagB=1;
	}

}
void SYS_201(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == RIGHT)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == LIFT)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == ENTER)
	{
		naviMENU = naviMENU * 10;
		QUERY_buf1.address = 94;
		QUERY_buf1.length = 0x01;
		QUERY_buf1.function = MODBUS_WRITE_REGISTER;
		QUERY_buf1.node_id = MODBUS_NODE_ID;
		packet_handler = 1;
		EventFlagB=1;
	}

}
void SYS_2000(void)
{
	static unsigned char flag=0;

	if((!EventFlagB)&&(flag))// event down edge
	{
		naviMENU = naviMENU / 10;
	}
	flag = EventFlagB;
}

void SYS_2010(void)
{
	static unsigned char flag=0;

	if((!EventFlagB)&&(flag))// event down edge
	{
		naviMENU = naviMENU / 10;
	}
	flag = EventFlagB;
}

void SYS_21(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == ENTER)naviMENU = naviMENU * 10;

}

void SYS_210(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == RIGHT)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == LIFT)naviMENU = naviMENU + 1;

	else if(KeyState.KeyValue == ENTER)
	{
		naviMENU = naviMENU * 10;
		QUERY_buf1.address = 93;
		QUERY_buf1.length = 0x00;
		QUERY_buf1.function = MODBUS_WRITE_REGISTER;
		QUERY_buf1.node_id = MODBUS_NODE_ID;
		packet_handler = 1;
		EventFlagB=1;
	}

}
void SYS_211(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == RIGHT)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == LIFT)naviMENU = naviMENU + 1;

	else if(KeyState.KeyValue == ENTER)
	{
		naviMENU = naviMENU * 10;
		QUERY_buf1.address = 93;
		QUERY_buf1.length = 0x01;
		QUERY_buf1.function = MODBUS_WRITE_REGISTER;
		QUERY_buf1.node_id = MODBUS_NODE_ID;
		packet_handler = 1;
		EventFlagB=1;
	}

}
void SYS_2100(void)
{
	static unsigned char flag=0;

	if((!EventFlagB)&&(flag))// event down edge
	{
		naviMENU = naviMENU / 10;
	}
	flag = EventFlagB;
}

void SYS_2110(void)
{
	static unsigned char flag=0;

	if((!EventFlagB)&&(flag))// event down edge
	{
		naviMENU = naviMENU / 10;
	}
	flag = EventFlagB;
}


void SYS_22(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == ENTER)naviMENU = naviMENU * 10;

}
void SYS_220(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == RIGHT)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == LIFT)naviMENU = naviMENU + 1;

	else if(KeyState.KeyValue == ENTER)
	{
		//naviMENU = naviMENU / 10;
		naviMENU = naviMENU * 10;
		QUERY_buf1.address = 92;
		QUERY_buf1.length = 0x00;
		QUERY_buf1.function = MODBUS_WRITE_REGISTER;
		QUERY_buf1.node_id = MODBUS_NODE_ID;
		packet_handler = 1;
		EventFlagB=1;
	}


}
void SYS_221(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == RIGHT)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == LIFT)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == ENTER)
	{
		naviMENU = naviMENU * 10;
		QUERY_buf1.address = 92;
		QUERY_buf1.length = 0x01;
		QUERY_buf1.function = MODBUS_WRITE_REGISTER;
		QUERY_buf1.node_id = MODBUS_NODE_ID;
		packet_handler = 1;
		EventFlagB=1;
	}
}

void SYS_2200(void)
{
	static unsigned char flag=0;

	if((!EventFlagB)&&(flag))// event down edge
	{
		naviMENU = naviMENU / 10;
	}
	flag = EventFlagB;
}

void SYS_2210(void)
{
	static unsigned char flag=0;

	if((!EventFlagB)&&(flag))// event down edge
	{
		naviMENU = naviMENU / 10;
	}
	flag = EventFlagB;
}

void SYS_23(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == ENTER)naviMENU = naviMENU * 10;

}

void SYS_230(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == RIGHT)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == LIFT)naviMENU = naviMENU + 1;

	else if(KeyState.KeyValue == ENTER)
	{
		naviMENU = naviMENU / 10;
	//	naviMENU = naviMENU * 10;
	//	QUERY_buf1.address = 76;
	//	QUERY_buf1.length = 0x00;
	//	QUERY_buf1.function = MODBUS_WRITE_REGISTER;
	//	QUERY_buf1.node_id = MODBUS_NODE_ID;
	//	packet_handler = 1;
	//	EventFlagB=1;
	}

}
void SYS_231(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == RIGHT)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == LIFT)naviMENU = naviMENU + 1;

	else if(KeyState.KeyValue == ENTER)
	{
		naviMENU = naviMENU * 10;
		QUERY_buf1.address = 90;
		QUERY_buf1.length = 0x01;
		QUERY_buf1.function = MODBUS_WRITE_REGISTER;
		QUERY_buf1.node_id = MODBUS_NODE_ID;
		packet_handler = 1;
		EventFlagB=1;
	}

}
void SYS_2300(void)
{
	static unsigned char flag=0;

	if((!EventFlagB)&&(flag))// event down edge
	{
		naviMENU = naviMENU / 10;
	}
	flag = EventFlagB;
}

void SYS_2310(void)
{
	static unsigned char flag=0;

	if((!EventFlagB)&&(flag))// event down edge
	{
		naviMENU = naviMENU / 10;
	}
	flag = EventFlagB;
}

void SYS_24(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == ENTER)naviMENU = naviMENU * 10;

}

void SYS_240(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == RIGHT)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == LIFT)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == ENTER)
	{
		naviMENU = naviMENU / 10;
		//naviMENU = naviMENU * 10;
		//QUERY_buf1.address = 77;
		//QUERY_buf1.length = 0x00;
		//QUERY_buf1.function = MODBUS_WRITE_REGISTER;
		//QUERY_buf1.node_id = MODBUS_NODE_ID;
		//packet_handler = 1;
		//EventFlagB=1;
	}
}

void SYS_241(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == RIGHT)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == LIFT)naviMENU = naviMENU + 1;

	else if(KeyState.KeyValue == ENTER)
	{
		naviMENU = naviMENU * 10;
		QUERY_buf1.address = 91;
		QUERY_buf1.length = 0x01;
		QUERY_buf1.function = MODBUS_WRITE_REGISTER;
		QUERY_buf1.node_id = MODBUS_NODE_ID;
		packet_handler = 1;
		EventFlagB=1;
	}

}

void SYS_2400(void)
{
	static unsigned char flag=0;

	if((!EventFlagB)&&(flag))// event down edge
	{
		naviMENU = naviMENU / 10;
	}
	flag = EventFlagB;
}

void SYS_2410(void)
{
	static unsigned char flag=0;

	if((!EventFlagB)&&(flag))// event down edge
	{
		naviMENU = naviMENU / 10;
	}
	flag = EventFlagB;
}







void SYS_4(void)
{
	static unsigned char flag=0;

	if(EventFlagC)
	{
		if(KeyState.KeyValue == ESC)
		{
			EventFlagC=0;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
		else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
		else if(KeyState.KeyValue == ENTER)naviMENU = naviMENU * 10;
		else if(KeyState.KeyValue == (ESC  & UP & DN & ENTER))
		{
			naviMENU = 0xFFFFFFFF;
		}
		
		if(KeyState.KeyValue != 0xFF)EventTimeC=0;
	}
	else 
	{
		if(KeyState.KeyValue != 0xFF)EventFlagC=1;
		if(naviMENU!=old_naveMENU)EventFlagC=1;
	}

	if((!EventFlagC)&&(flag))RefreshFlag=1;// event edge
	else if((EventFlagC)&&(!flag))RefreshFlag=1;// event edge
	flag = EventFlagC;
}


void SYS_40(void)
{
	if(confirm)
	{
		if(KeyState.KeyValue == ESC)
		{
			naviMENU = naviMENU / 10;
			confirm=0;
		}
		else if(KeyState.KeyValue == ENTER)
		{
Vin_upper_limit=main_data_array[51];
Vin_lower_limit=main_data_array[50];
		
			naviMENU = naviMENU * 10;
		}
		else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
		else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	}
	else
	{
			if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
			else if(KeyState.KeyValue == ENTER)
			{
				naviMENU = naviMENU * 10;
				EventFlagB=1;
			}
			else if(KeyState.KeyValue == UP)
			{
				_password[pass_pos]++;
				if(9 < _password[pass_pos])_password[pass_pos]=0;

				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(_password[pass_pos])_password[pass_pos]--;
				else _password[pass_pos] = 9;

				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				pass_pos++;
				if(3 <pass_pos)pass_pos=0;
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == LIFT)
			{
				if(pass_pos)pass_pos--;
				else pass_pos = 3;

				RefreshFlag=1;
			}
	}
}


void SYS_400(void)
{
	static unsigned char flag=0;

	if(confirm)
	{
		if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
		else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
		else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
		else if(KeyState.KeyValue == RIGHT){Vin_upper_limit++;RefreshFlag=1;}
		else if(KeyState.KeyValue == LIFT){Vin_upper_limit--;RefreshFlag=1;}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			EventFlagB=1;
		}
	}
	else
	{
		if((!EventFlagB)&&(flag))// event down edge
		{
			naviMENU = naviMENU / 10;
			if((password[0]==_password[0])&&
			(password[1]==_password[1])&&
			(password[2]==_password[2])&&
			(password[3]==_password[3]))
			{
				confirm=1;
			}
		}
		else if((EventFlagB)&&(!flag))// event up edge
		{

		}
		flag = EventFlagB;
	}
}



void SYS_401(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == RIGHT){Vin_lower_limit++;RefreshFlag=1;}
	else if(KeyState.KeyValue == LIFT){Vin_lower_limit--;RefreshFlag=1;}
	else if(KeyState.KeyValue == ENTER)
	{
		naviMENU = (naviMENU-1) * 10;
		EventFlagB=1;
	}

}

void SYS_4000(void)
{
	static unsigned char flag=0;

	if((!EventFlagB)&&(flag))// event down edge
	{
		naviMENU = naviMENU / 10;
	}
	else if((EventFlagB)&&(!flag))// event up edge
	{
		QUERY_buf1.address = 51;
		QUERY_buf1.length = Vin_upper_limit;
		QUERY_buf1.function = MODBUS_WRITE_REGISTER;
		QUERY_buf1.node_id = MODBUS_NODE_ID;

		QUERY_buf2.address = 50;
		QUERY_buf2.length = Vin_lower_limit;
		QUERY_buf2.function = MODBUS_WRITE_REGISTER;
		QUERY_buf2.node_id = MODBUS_NODE_ID;

		packet_handler =2;
	}
	flag = EventFlagB;
}



void SYS_41(void)
{
	if(KeyState.KeyValue == ESC)
	{
		naviMENU = naviMENU / 10;
		confirm=0;
	}
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == ENTER)
	{
		Vout_upper_limit=main_data_array[53];
		Vout_lower_limit=main_data_array[52];
		Vinv_ref =main_data_array[65];
		
		naviMENU = naviMENU * 10;
	}

}



void SYS_410(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == ENTER)naviMENU = (naviMENU) * 10;
}

void SYS_4100(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == RIGHT){Vout_upper_limit++;RefreshFlag=1;}
	else if(KeyState.KeyValue == LIFT){Vout_upper_limit--;RefreshFlag=1;}
	else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = (naviMENU) * 10;
			EventFlagB=1;
		}
}
void SYS_4101(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == RIGHT){Vout_lower_limit++;RefreshFlag=1;}
	else if(KeyState.KeyValue == LIFT){Vout_lower_limit--;RefreshFlag=1;}
	else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = (naviMENU-1) * 10;
			EventFlagB=1;
		}
}

void SYS_41000(void)
{
	static unsigned char flag=0;

	if((!EventFlagB)&&(flag))// event down edge
	{
		naviMENU = naviMENU / 10;
	}
	else if((EventFlagB)&&(!flag))// event up edge
	{
		QUERY_buf1.address = 53;
		QUERY_buf1.length = Vout_upper_limit;
		QUERY_buf1.function = MODBUS_WRITE_REGISTER;
		QUERY_buf1.node_id = MODBUS_NODE_ID;

		QUERY_buf2.address = 52;
		QUERY_buf2.length = Vout_lower_limit;
		QUERY_buf2.function = MODBUS_WRITE_REGISTER;
		QUERY_buf2.node_id = MODBUS_NODE_ID;

		packet_handler =2;
	}
	flag = EventFlagB;
}







void SYS_411(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == ENTER)naviMENU = (naviMENU) * 10;
}


void SYS_4110(void)
{
		if(KeyState.KeyValue == ESC)
		{
			naviMENU = naviMENU / 10;
			//makeQuery1(98,0xFFFF,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			makeQuery1(98,((int)65<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			Vinv_ref = Vinv_ref+1;
			makeQuery1(65,Vinv_ref,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			RefreshFlag=1;

		}
		else if(KeyState.KeyValue == DN)
		{
			Vinv_ref = Vinv_ref-1;
			makeQuery1(65,Vinv_ref,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			RefreshFlag=1;

		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			//makeQuery1(97,0xFFFF,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			makeQuery1(97,((int)65<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
}

void SYS_41100(void)
{
	static unsigned char flag=0;

	if((!EventFlagB)&&(flag))// event down edge
	{
		naviMENU = naviMENU / 10;
	}
	else if((EventFlagB)&&(!flag))// event up edge
	{
	}
	flag = EventFlagB;
}


void SYS_42(void)
{
	if(KeyState.KeyValue == ESC)
	{
		naviMENU = naviMENU / 10;
		confirm=0;
	}
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == ENTER)
	{
		Abat_limit=main_data_array[60];
		Vbat_upper_limit = main_data_array[55];
		Vbat_lower_limit = main_data_array[54];
		Bat_Equal_Curr = main_data_array[63];
		Bat_Equal_volt = main_data_array[61];
		Bat_Float_Volt = main_data_array[62];
		naviMENU = naviMENU * 10;
		
	}

}

void SYS_420(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == ENTER)naviMENU = naviMENU * 10;

}
void SYS_4200(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == RIGHT){Abat_limit++;RefreshFlag=1;}
	else if(KeyState.KeyValue == LIFT){Abat_limit--;RefreshFlag=1;}
	else if(KeyState.KeyValue == ENTER)
	{
		naviMENU = (naviMENU) * 10;
		EventFlagB=1;
	}
}

void SYS_42000(void)
{
	static unsigned char flag=0;

	if((!EventFlagB)&&(flag))// event down edge
	{
		naviMENU = naviMENU / 10;
	}
	else if((EventFlagB)&&(!flag))// event up edge
	{
		QUERY_buf1.address = 60;
		QUERY_buf1.length = Abat_limit;
		QUERY_buf1.function = MODBUS_WRITE_REGISTER;
		QUERY_buf1.node_id = MODBUS_NODE_ID;

		packet_handler =1;
	}
	flag = EventFlagB;
}






void SYS_421(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == ENTER)naviMENU = naviMENU * 10;
}




void SYS_4210(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == RIGHT){Vbat_upper_limit++;RefreshFlag=1;}
	else if(KeyState.KeyValue == LIFT){Vbat_upper_limit--;RefreshFlag=1;}
	else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = (naviMENU) * 10;
			EventFlagB=1;
		}
}

void SYS_4211(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == RIGHT){Vbat_lower_limit++;RefreshFlag=1;}
	else if(KeyState.KeyValue == LIFT){Vbat_lower_limit--;RefreshFlag=1;}
	else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = (naviMENU-1) * 10;
			EventFlagB=1;
		}
}

void SYS_42100(void)
{
	static unsigned char flag=0;

	if((!EventFlagB)&&(flag))// event down edge
	{
		naviMENU = naviMENU / 10;
	}
	else if((EventFlagB)&&(!flag))// event up edge
	{
		QUERY_buf1.address = 55;
		QUERY_buf1.length = Vbat_upper_limit;
		QUERY_buf1.function = MODBUS_WRITE_REGISTER;
		QUERY_buf1.node_id = MODBUS_NODE_ID;

		QUERY_buf2.address = 54;
		QUERY_buf2.length = Vbat_lower_limit;
		QUERY_buf2.function = MODBUS_WRITE_REGISTER;
		QUERY_buf2.node_id = MODBUS_NODE_ID;

		packet_handler =2;
	}
	flag = EventFlagB;
}

void SYS_422(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == ENTER)naviMENU = naviMENU * 10;
}

void SYS_4220(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == RIGHT){Bat_Equal_volt++;RefreshFlag=1;}
	else if(KeyState.KeyValue == LIFT){Bat_Equal_volt--;RefreshFlag=1;}
	else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = (naviMENU) * 10;
			EventFlagB=1;
		}
}

void SYS_42200(void)
{
	static unsigned char flag=0;

	if((!EventFlagB)&&(flag))// event down edge
	{
		naviMENU = naviMENU / 10;
	}
	else if((EventFlagB)&&(!flag))// event up edge
	{
		QUERY_buf1.address = 61;
		QUERY_buf1.length = Bat_Equal_volt;
		QUERY_buf1.function = MODBUS_WRITE_REGISTER;
		QUERY_buf1.node_id = MODBUS_NODE_ID;
		packet_handler = 1;

	}
	flag = EventFlagB;
}


void SYS_423(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == ENTER)naviMENU = naviMENU * 10;
}


void SYS_4230(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == RIGHT){Bat_Float_Volt++;RefreshFlag=1;}
	else if(KeyState.KeyValue == LIFT){Bat_Float_Volt--;RefreshFlag=1;}
	else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = (naviMENU) * 10;
			EventFlagB=1;
		}



}
void SYS_42300(void)
{
	static unsigned char flag=0;

	if((!EventFlagB)&&(flag))// event down edge
	{
		naviMENU = naviMENU / 10;
	}
	else if((EventFlagB)&&(!flag))// event up edge
	{
			QUERY_buf1.address = 62;
			QUERY_buf1.length = Bat_Float_Volt;
			QUERY_buf1.function = MODBUS_WRITE_REGISTER;
			QUERY_buf1.node_id = MODBUS_NODE_ID;
			packet_handler = 1;
	}
	flag = EventFlagB;
}

void SYS_424(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == ENTER)naviMENU = naviMENU * 10;
}

void SYS_4240(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == RIGHT){Bat_Equal_Curr++;RefreshFlag=1;}
	else if(KeyState.KeyValue == LIFT){Bat_Equal_Curr--;RefreshFlag=1;}
	else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = (naviMENU) * 10;
			EventFlagB=1;
		}


}
void SYS_42400(void)
{
	static unsigned char flag=0;

	if((!EventFlagB)&&(flag))// event down edge
	{
		naviMENU = naviMENU / 10;
	}
	else if((EventFlagB)&&(!flag))// event up edge
	{
		QUERY_buf1.address = 63;
		QUERY_buf1.length = Bat_Equal_Curr;
		QUERY_buf1.function = MODBUS_WRITE_REGISTER;
		QUERY_buf1.node_id = MODBUS_NODE_ID;
		packet_handler = 1;

	}
	flag = EventFlagB;
}

void SYS_43(void)
{
	if(KeyState.KeyValue == ESC)
	{
		naviMENU = naviMENU / 10;
		confirm=0;
	}
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == ENTER)
	{
		Async_Upper_Limit=main_data_array[59];
		Async_Lower_Limit=main_data_array[58];
		naviMENU = naviMENU * 10;
	}
}


void SYS_430(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == RIGHT){Async_Upper_Limit++;RefreshFlag=1;}
	else if(KeyState.KeyValue == LIFT){Async_Upper_Limit--;RefreshFlag=1;}
	else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = (naviMENU) * 10;
			EventFlagB=1;
		}


}
void SYS_431(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == RIGHT){Async_Lower_Limit++;RefreshFlag=1;}
	else if(KeyState.KeyValue == LIFT){Async_Lower_Limit--;RefreshFlag=1;}
	else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = (naviMENU-1) * 10;
			EventFlagB=1;
		}

}


void SYS_4300(void)
{
	static unsigned char flag=0;

	if((!EventFlagB)&&(flag))// event down edge
	{
		naviMENU = naviMENU / 10;
	}
	else if((EventFlagB)&&(!flag))// event up edge
	{
		makeQuery1(59,Async_Upper_Limit,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		makeQuery2(58,Async_Lower_Limit,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
	}
	flag = EventFlagB;
}


void SYS_44(void)
{
	if(KeyState.KeyValue == ESC)
	{
		naviMENU = naviMENU / 10;
		confirm=0;
	}
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == ENTER)
	{
		naviMENU = naviMENU * 10;
	}

}


void SYS_441(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == ENTER)
	{
		naviMENU = naviMENU * 10;
		EventFlagB++;
	}

}

void SYS_4410(void)
{
	static unsigned char flag=0;

	if((!EventFlagB)&&(flag))// event down edge
	{
		naviMENU = naviMENU / 10;
	}
	else if((EventFlagB)&&(!flag))// event up edge
	{
		QUERY_buf1.address = 99;
		QUERY_buf1.length = 0x01;
		QUERY_buf1.function = MODBUS_WRITE_REGISTER;
		QUERY_buf1.node_id = MODBUS_NODE_ID;
		packet_handler = 1;
	}
	flag = EventFlagB;

}

void SYS_440(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == ENTER)
	{
		Vin_offset = main_data_array[70];
		Vin_gain = main_data_array[71];
		Ain_offset = main_data_array[72];
		Ain_gain = main_data_array[73];
		Vout_offset = main_data_array[74];
		Vout_gain = main_data_array[75];
		Aout_offset = main_data_array[76];
		Aout_gain = main_data_array[77];
		Vbat_offset = main_data_array[80];
		Vbat_gain = main_data_array[81];
		Vbyp_offset = main_data_array[78];
		Vbyp_gain = main_data_array[79];
		Vdc_offset= main_data_array[84];
		Vdc_gain = main_data_array[85];
		Abat_offset = main_data_array[82];
		Abat_gain = main_data_array[83];
		
		naviMENU = naviMENU * 10;
	}

}
void SYS_4400(void)
{
	if(data_array[0] != main_data_array[0])
	{
		data_array[0] = main_data_array[0];
		RefreshFlag=1;
	}

	if((unsigned int)Vin_offset != (unsigned int)main_data_array[70]){Vin_offset = main_data_array[70];RefreshFlag=1;}
	if((unsigned int)Vin_gain != (unsigned int)main_data_array[71]){Vin_gain = main_data_array[71];RefreshFlag=1;}

	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == ENTER)
	{
		Vin_offset = main_data_array[70];
		Vin_gain = main_data_array[71];
		naviMENU = naviMENU * 10;
		posInpage=0;
	}
}

void SYS_44000(void)
{
	if(data_array[0] != main_data_array[0])
	{
		data_array[0] = main_data_array[0];
		RefreshFlag=1;
	}

	if(posInpage==0)
	{
		if((unsigned int)Vin_offset != (unsigned int)main_data_array[70])RefreshFlag=1;
		Vin_offset = main_data_array[70];
		
		if(KeyState.KeyValue == ESC){			naviMENU = naviMENU / 10;}
		else if(KeyState.KeyValue == UP)		naviMENU = naviMENU - 1;
		else if(KeyState.KeyValue == DN)		naviMENU = naviMENU + 1;
		else if(KeyState.KeyValue == RIGHT){	posInpage=5;RefreshFlag=1;}
		else if(KeyState.KeyValue == LIFT)	{	posInpage=0;RefreshFlag=1;}
		else if(KeyState.KeyValue == ENTER){	posInpage=5;RefreshFlag=1;}
	}
	else if(posInpage==1)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)70<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Vin_offset<55535)
			{
				Vin_offset = Vin_offset+10000;
				makeQuery1(70,Vin_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(10000 <= Vin_offset)
			{
				Vin_offset = Vin_offset-10000;
				makeQuery1(70,Vin_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)70<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==2)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)70<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if((Vin_offset%10000)<9000)
			{
				if((Vin_offset/1000)<65)
				{
					Vin_offset = Vin_offset+1000;
					makeQuery1(70,Vin_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
					RefreshFlag=1;
				}
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(1000 <= (Vin_offset%10000))
			{
				Vin_offset = Vin_offset-1000;
				makeQuery1(70,Vin_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)70<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}

	}
	else if(posInpage==3)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)70<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if((Vin_offset%1000)<900)
			{
				if((Vin_offset/100)<655)
				{
					Vin_offset = Vin_offset+100;
					makeQuery1(70,Vin_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
					RefreshFlag=1;
				}
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(100 <= (Vin_offset%1000))
			{
				Vin_offset = Vin_offset-100;
				makeQuery1(70,Vin_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)70<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==4)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)70<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if((Vin_offset%100)<90)
			{
				if((Vin_offset/10)<6553)
				{
					Vin_offset = Vin_offset+10;
					makeQuery1(70,Vin_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
					RefreshFlag=1;
				}
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(10 <= (Vin_offset%100))
			{
				Vin_offset = Vin_offset-10;
				makeQuery1(70,Vin_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)70<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==5)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)70<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if((Vin_offset%10)<9)
			{
				if((Vin_offset/1)<65535)
				{
					Vin_offset = Vin_offset+1;
					makeQuery1(70,Vin_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
					RefreshFlag=1;
				}
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(1 <= (Vin_offset%10))
			{
				Vin_offset = Vin_offset-1;
				makeQuery1(70,Vin_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)70<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	
}


void SYS_44001(void)
{
	if(data_array[0] != main_data_array[0])
	{
		data_array[0] = main_data_array[0];
		RefreshFlag=1;
	}

	if(posInpage==0)
	{
		if((unsigned int)Vin_gain != (unsigned int)main_data_array[71])RefreshFlag=1;
		Vin_gain = main_data_array[71];
		
		if(KeyState.KeyValue == ESC){			naviMENU = naviMENU / 10;}
		else if(KeyState.KeyValue == UP)		naviMENU = naviMENU - 1;
		else if(KeyState.KeyValue == DN)		naviMENU = naviMENU + 1;
		else if(KeyState.KeyValue == RIGHT){	posInpage=5;RefreshFlag=1;}
		else if(KeyState.KeyValue == LIFT)	{	posInpage=0;RefreshFlag=1;}
		else if(KeyState.KeyValue == ENTER){	posInpage=5;RefreshFlag=1;}
	}
	else if(posInpage==1)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)71<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Vin_gain<(32767-10000))
			{
				Vin_gain = Vin_gain+10000;
				makeQuery1(71,Vin_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((10000 -32768) <= Vin_gain)
			{
				Vin_gain = Vin_gain-10000;
				makeQuery1(71,Vin_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)71<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==2)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)71<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Vin_gain<(32767-1000))
			{
				Vin_gain = Vin_gain+1000;
				makeQuery1(71,Vin_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((1000 -32768) <= Vin_gain)
			{
				Vin_gain = Vin_gain-1000;
				makeQuery1(71,Vin_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)71<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}

	}
	else if(posInpage==3)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)71<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Vin_gain<(32767-100))
			{
				Vin_gain = Vin_gain+100;
				makeQuery1(71,Vin_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((100 -32768) <= Vin_gain)
			{
				Vin_gain = Vin_gain-100;
				makeQuery1(71,Vin_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)71<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==4)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)71<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Vin_gain<(32767-10))
			{
				Vin_gain = Vin_gain+10;
				makeQuery1(71,Vin_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((10 -32768) <= Vin_gain)
			{
				Vin_gain = Vin_gain-10;
				makeQuery1(71,Vin_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)71<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==5)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)71<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Vin_gain<(32767-1))
			{
				Vin_gain = Vin_gain+1;
				makeQuery1(71,Vin_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((1 -32768) <= Vin_gain)
			{
				Vin_gain = Vin_gain-1;
				makeQuery1(71,Vin_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)71<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	
	if(KeyState.KeyValue == ESC)
	{
		Vin_offset = main_data_array[70];
		Vin_gain = main_data_array[71];
		RefreshFlag=1;
	}
}
void SYS_440000(void)
{
	static unsigned char flag=0;

	if((!EventFlagB)&&(flag))// event down edge
	{
		naviMENU = naviMENU / 10;
	}
	else if((EventFlagB)&&(!flag))// event up edge
	{
	}
	flag = EventFlagB;
}



void SYS_4401(void)
{
	if(data_array[1] != main_data_array[1])
	{
		data_array[1] = main_data_array[1];
		RefreshFlag=1;
	}

	if((unsigned int)Ain_offset != (unsigned int)main_data_array[72]){Ain_offset = main_data_array[72]; RefreshFlag=1;}
	if((unsigned int)Ain_gain != (unsigned int)main_data_array[73]){Ain_gain = main_data_array[73];RefreshFlag=1;}

	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == ENTER)
	{
		Ain_offset = main_data_array[72];
		Ain_gain = main_data_array[73];
		naviMENU = naviMENU * 10;
		posInpage=0;
	}
}

//Ain offset
void SYS_44010(void)
{
	if(data_array[1] != main_data_array[1])
	{
		data_array[1] = main_data_array[1];
		RefreshFlag=1;
	}

	if(posInpage==0)
	{
		if((unsigned int)Ain_offset != (unsigned int)main_data_array[72]){Ain_offset = main_data_array[72]; RefreshFlag=1;}
		
		if(KeyState.KeyValue == ESC){			naviMENU = naviMENU / 10;}
		else if(KeyState.KeyValue == UP)		naviMENU = naviMENU - 1;
		else if(KeyState.KeyValue == DN)		naviMENU = naviMENU + 1;
		else if(KeyState.KeyValue == RIGHT){	posInpage=5;RefreshFlag=1;}
		else if(KeyState.KeyValue == LIFT)	{	posInpage=0;RefreshFlag=1;}
		else if(KeyState.KeyValue == ENTER){	posInpage=5;RefreshFlag=1;}
	}
	else if(posInpage==1)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)72<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Ain_offset<55535)
			{
				Ain_offset = Ain_offset+10000;
				makeQuery1(72,Ain_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(10000 <= Ain_offset)
			{
				Ain_offset = Ain_offset-10000;
				makeQuery1(72,Ain_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)72<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==2)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)72<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if((Ain_offset%10000)<9000)
			{
				if((Ain_offset/1000)<65)
				{
					Ain_offset = Ain_offset+1000;
					makeQuery1(72,Ain_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
					RefreshFlag=1;
				}
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(1000 <= (Ain_offset%10000))
			{
				Ain_offset = Ain_offset-1000;
				makeQuery1(72,Ain_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)72<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}

	}
	else if(posInpage==3)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)72<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if((Ain_offset%1000)<900)
			{
				if((Ain_offset/100)<655)
				{
					Ain_offset = Ain_offset+100;
					makeQuery1(72,Ain_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
					RefreshFlag=1;
				}
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(100 <= (Ain_offset%1000))
			{
				Ain_offset = Ain_offset-100;
				makeQuery1(72,Ain_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)72<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==4)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)72<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if((Ain_offset%100)<90)
			{
				if((Ain_offset/10)<6553)
				{
					Ain_offset = Ain_offset+10;
					makeQuery1(72,Ain_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
					RefreshFlag=1;
				}
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(10 <= (Ain_offset%100))
			{
				Ain_offset = Ain_offset-10;
				makeQuery1(72,Ain_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)72<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==5)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)72<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if((Ain_offset%10)<9)
			{
				if((Ain_offset/1)<65535)
				{
					Ain_offset = Ain_offset+1;
					makeQuery1(72,Ain_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
					RefreshFlag=1;
				}
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(1 <= (Ain_offset%10))
			{
				Ain_offset = Ain_offset-1;
				makeQuery1(72,Ain_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)72<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
}

//Ain gain
void SYS_44011(void)
{
	if(data_array[1] != main_data_array[1])
	{
		data_array[1] = main_data_array[1];
		RefreshFlag=1;
	}
	
	if(posInpage==0)
	{
		if((unsigned int)Ain_gain != (unsigned int)main_data_array[73]){Ain_gain = main_data_array[73];RefreshFlag=1;}
		
		if(KeyState.KeyValue == ESC){			naviMENU = naviMENU / 10;}
		else if(KeyState.KeyValue == UP)		naviMENU = naviMENU - 1;
		else if(KeyState.KeyValue == DN)		naviMENU = naviMENU + 1;
		else if(KeyState.KeyValue == RIGHT){	posInpage=5;RefreshFlag=1;}
		else if(KeyState.KeyValue == LIFT)	{	posInpage=0;RefreshFlag=1;}
		else if(KeyState.KeyValue == ENTER){	posInpage=5;RefreshFlag=1;}
	}
	else if(posInpage==1)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)73<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Ain_gain<(32767-10000))
			{
				Ain_gain = Ain_gain+10000;
				makeQuery1(73,Ain_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((10000 -32768) <= Ain_gain)
			{
				Ain_gain = Ain_gain-10000;
				makeQuery1(73,Ain_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)73<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==2)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)73<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Ain_gain<(32767-1000))
			{
				Ain_gain = Ain_gain+1000;
				makeQuery1(73,Ain_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((1000 -32768) <= Ain_gain)
			{
				Ain_gain = Ain_gain-1000;
				makeQuery1(73,Ain_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)73<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}

	}
	else if(posInpage==3)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)73<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Ain_gain<(32767-100))
			{
				Ain_gain = Ain_gain+100;
				makeQuery1(73,Ain_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((100 -32768) <= Ain_gain)
			{
				Ain_gain = Ain_gain-100;
				makeQuery1(73,Ain_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)73<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==4)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)73<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Ain_gain<(32767-10))
			{
				Ain_gain = Ain_gain+10;
				makeQuery1(73,Ain_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((10 -32768) <= Ain_gain)
			{
				Ain_gain = Ain_gain-10;
				makeQuery1(73,Ain_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)73<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==5)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)73<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Vin_gain<(32767-1))
			{
				Ain_gain = Ain_gain+1;
				makeQuery1(73,Ain_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((1 -32768) <= Ain_gain)
			{
				Ain_gain = Ain_gain-1;
				makeQuery1(73,Ain_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)73<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}

}


void SYS_440100(void)
{
	static unsigned char flag=0;

	if((!EventFlagB)&&(flag))// event down edge
	{
		naviMENU = naviMENU / 10;
	}
	else if((EventFlagB)&&(!flag))// event up edge
	{
	}
	flag = EventFlagB;
}

void SYS_4402(void)
{
	if(data_array[2] != main_data_array[2])
	{
		data_array[2] = main_data_array[2];
		RefreshFlag=1;
	}

	if((unsigned int)Vout_offset != (unsigned int)main_data_array[74]){Vout_offset = main_data_array[74]; RefreshFlag=1;}
	if((unsigned int)Vout_gain != (unsigned int)main_data_array[75]){Vout_gain = main_data_array[75];RefreshFlag=1;}

	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == ENTER)
	{
		Vout_offset = main_data_array[74];
		Vout_gain = main_data_array[75];
		naviMENU = naviMENU * 10;
		posInpage=0;
	}
}

//Vout offset
void SYS_44020(void)
{
	if(data_array[2] != main_data_array[2])
	{
		data_array[2] = main_data_array[2];
		RefreshFlag=1;
	}
	
	if(posInpage==0)
	{
		if((unsigned int)Vout_offset != (unsigned int)main_data_array[74]){Vout_offset = main_data_array[74]; RefreshFlag=1;}
		
		if(KeyState.KeyValue == ESC){			naviMENU = naviMENU / 10;}
		else if(KeyState.KeyValue == UP)		naviMENU = naviMENU - 1;
		else if(KeyState.KeyValue == DN)		naviMENU = naviMENU + 1;
		else if(KeyState.KeyValue == RIGHT){	posInpage=5;RefreshFlag=1;}
		else if(KeyState.KeyValue == LIFT)	{	posInpage=0;RefreshFlag=1;}
		else if(KeyState.KeyValue == ENTER){	posInpage=5;RefreshFlag=1;}
	}
	else if(posInpage==1)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)74<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Vout_offset<55535)
			{
				Vout_offset = Vout_offset+10000;
				makeQuery1(74,Vout_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(10000 <= Vout_offset)
			{
				Vout_offset = Vout_offset-10000;
				makeQuery1(74,Vout_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)74<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==2)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)74<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if((Vout_offset%10000)<9000)
			{
				if((Vout_offset/1000)<65)
				{
					Vout_offset = Vout_offset+1000;
					makeQuery1(74,Vout_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
					RefreshFlag=1;
				}
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(1000 <= (Vout_offset%10000))
			{
				Vout_offset = Vout_offset-1000;
				makeQuery1(74,Vout_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)74<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}

	}
	else if(posInpage==3)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)74<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if((Vout_offset%1000)<900)
			{
				if((Vout_offset/100)<655)
				{
					Vout_offset = Vout_offset+100;
					makeQuery1(74,Vout_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
					RefreshFlag=1;
				}
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(100 <= (Vout_offset%1000))
			{
				Vout_offset = Vout_offset-100;
				makeQuery1(74,Vout_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)74<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==4)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)74<<8 || 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if((Vout_offset%100)<90)
			{
				if((Vout_offset/10)<6553)
				{
					Vout_offset = Vout_offset+10;
					makeQuery1(74,Vout_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
					RefreshFlag=1;
				}
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(10 <= (Vout_offset%100))
			{
				Vout_offset = Vout_offset-10;
				makeQuery1(74,Vout_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)74<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==5)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)74<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if((Vout_offset%10)<9)
			{
				if((Vout_offset/1)<65535)
				{
					Vout_offset = Vout_offset+1;
					makeQuery1(74,Vout_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
					RefreshFlag=1;
				}
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(1 <= (Vout_offset%10))
			{
				Vout_offset = Vout_offset-1;
				makeQuery1(74,Vout_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)74<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
}

//Vout gain
void SYS_44021(void)
{
	if(data_array[2] != main_data_array[2])
	{
		data_array[2] = main_data_array[2];
		RefreshFlag=1;
	}

	if(posInpage==0)
	{
		if((unsigned int)Vout_gain != (unsigned int)main_data_array[75]){Vout_gain = main_data_array[75];RefreshFlag=1;}
		
		if(KeyState.KeyValue == ESC){			naviMENU = naviMENU / 10;}
		else if(KeyState.KeyValue == UP)		naviMENU = naviMENU - 1;
		else if(KeyState.KeyValue == DN)		naviMENU = naviMENU + 1;
		else if(KeyState.KeyValue == RIGHT){	posInpage=5;RefreshFlag=1;}
		else if(KeyState.KeyValue == LIFT)	{	posInpage=0;RefreshFlag=1;}
		else if(KeyState.KeyValue == ENTER){	posInpage=5;RefreshFlag=1;}
	}
	else if(posInpage==1)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)75<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Vout_gain<(32767-10000))
			{
				Vout_gain = Vout_gain+10000;
				makeQuery1(75,Vout_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((10000 -32768) <= Vout_gain)
			{
				Vout_gain = Vout_gain-10000;
				makeQuery1(75,Vout_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)75<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==2)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)75<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Vout_gain<(32767-1000))
			{
				Vout_gain = Vout_gain+1000;
				makeQuery1(75,Vout_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((1000 -32768) <= Vout_gain)
			{
				Vout_gain = Vout_gain-1000;
				makeQuery1(75,Vout_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)75<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}

	}
	else if(posInpage==3)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)75<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Vout_gain<(32767-100))
			{
				Vout_gain = Vout_gain+100;
				makeQuery1(75,Vout_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((100 -32768) <= Vout_gain)
			{
				Vout_gain = Vout_gain-100;
				makeQuery1(75,Vout_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)75<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==4)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)75<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Vout_gain<(32767-10))
			{
				Vout_gain = Vout_gain+10;
				makeQuery1(75,Vout_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((10 -32768) <= Vout_gain)
			{
				Vout_gain = Vout_gain-10;
				makeQuery1(75,Vout_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)75<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==5)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)75<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Vout_gain<(32767-1))
			{
				Vout_gain = Vout_gain+1;
				makeQuery1(75,Vout_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((1 -32768) <= Vout_gain)
			{
				Vout_gain = Vout_gain-1;
				makeQuery1(75,Vout_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)75<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
}

void SYS_440200(void)
{
	static unsigned char flag=0;

	if((!EventFlagB)&&(flag))// event down edge
	{
		naviMENU = naviMENU / 10;
	}
	else if((EventFlagB)&&(!flag))// event up edge
	{
	}
	flag = EventFlagB;
}

void SYS_4403(void)
{
	if(data_array[3] != main_data_array[3])
	{
		data_array[3] = main_data_array[3];
		RefreshFlag=1;
	}

	if((unsigned int)Aout_offset != (unsigned int)main_data_array[76]){Aout_offset = main_data_array[76];RefreshFlag=1;}
	if((unsigned int)Aout_gain != (unsigned int)main_data_array[77]){Aout_gain = main_data_array[77];RefreshFlag=1;}

	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == ENTER)
	{
		Aout_offset = main_data_array[76];
		Aout_gain = main_data_array[77];
		naviMENU = naviMENU * 10;
		posInpage=0;
	}
}

//Aout offset
void SYS_44030(void)
{
	if(data_array[3] != main_data_array[3])
	{
		data_array[3] = main_data_array[3];
		RefreshFlag=1;
	}

	if(posInpage==0)
	{
		if((unsigned int)Aout_offset != (unsigned int)main_data_array[76]){Aout_offset = main_data_array[76];RefreshFlag=1;}
				
		if(KeyState.KeyValue == ESC){			naviMENU = naviMENU / 10;}
		else if(KeyState.KeyValue == UP)		naviMENU = naviMENU - 1;
		else if(KeyState.KeyValue == DN)		naviMENU = naviMENU + 1;
		else if(KeyState.KeyValue == RIGHT){	posInpage=5;RefreshFlag=1;}
		else if(KeyState.KeyValue == LIFT)	{	posInpage=0;RefreshFlag=1;}
		else if(KeyState.KeyValue == ENTER){	posInpage=5;RefreshFlag=1;}
	}
	else if(posInpage==1)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)76<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Aout_offset<55535)
			{
				Aout_offset = Aout_offset+10000;
				makeQuery1(76,Aout_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(10000 <= Aout_offset)
			{
				Aout_offset = Aout_offset-10000;
				makeQuery1(76,Aout_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)76<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==2)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)76<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if((Aout_offset%10000)<9000)
			{
				if((Aout_offset/1000)<65)
				{
					Aout_offset = Aout_offset+1000;
					makeQuery1(76,Aout_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
					RefreshFlag=1;
				}
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(1000 <= (Aout_offset%10000))
			{
				Aout_offset = Aout_offset-1000;
				makeQuery1(76,Aout_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)76<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}

	}
	else if(posInpage==3)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)76<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if((Aout_offset%1000)<900)
			{
				if((Aout_offset/100)<655)
				{
					Aout_offset = Aout_offset+100;
					makeQuery1(76,Aout_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
					RefreshFlag=1;
				}
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(100 <= (Aout_offset%1000))
			{
				Aout_offset = Aout_offset-100;
				makeQuery1(76,Aout_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)76<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==4)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)76<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if((Aout_offset%100)<90)
			{
				if((Aout_offset/10)<6553)
				{
					Aout_offset = Aout_offset+10;
					makeQuery1(76,Aout_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
					RefreshFlag=1;
				}
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(10 <= (Aout_offset%100))
			{
				Aout_offset = Aout_offset-10;
				makeQuery1(76,Aout_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)76<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==5)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)76<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if((Aout_offset%10)<9)
			{
				if((Aout_offset/1)<65535)
				{
					Aout_offset = Aout_offset+1;
					makeQuery1(76,Aout_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
					RefreshFlag=1;
				}
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(1 <= (Aout_offset%10))
			{
				Aout_offset = Aout_offset-1;
				makeQuery1(76,Aout_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)76<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
}

//Aout gain
void SYS_44031(void)
{
	if(data_array[3] != main_data_array[3])
	{
		data_array[3] = main_data_array[3];
		RefreshFlag=1;
	}

	if(posInpage==0)
	{
		if((unsigned int)Aout_gain != (unsigned int)main_data_array[77]){Aout_gain = main_data_array[77];RefreshFlag=1;}
		
		if(KeyState.KeyValue == ESC){			naviMENU = naviMENU / 10;}
		else if(KeyState.KeyValue == UP)		naviMENU = naviMENU - 1;
		else if(KeyState.KeyValue == DN)		naviMENU = naviMENU + 1;
		else if(KeyState.KeyValue == RIGHT){	posInpage=5;RefreshFlag=1;}
		else if(KeyState.KeyValue == LIFT)	{	posInpage=0;RefreshFlag=1;}
		else if(KeyState.KeyValue == ENTER){	posInpage=5;RefreshFlag=1;}
	}
	else if(posInpage==1)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)77<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Aout_gain<(32767-10000))
			{
				Aout_gain = Aout_gain+10000;
				makeQuery1(77,Aout_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((10000 -32768) <= Aout_gain)
			{
				Aout_gain = Aout_gain-10000;
				makeQuery1(77,Aout_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)77<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==2)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)77<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Aout_gain<(32767-1000))
			{
				Aout_gain = Aout_gain+1000;
				makeQuery1(77,Aout_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((1000 -32768) <= Aout_gain)
			{
				Aout_gain = Aout_gain-1000;
				makeQuery1(77,Aout_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)77<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}

	}
	else if(posInpage==3)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)77<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Aout_gain<(32767-100))
			{
				Aout_gain = Aout_gain+100;
				makeQuery1(77,Aout_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((100 -32768) <= Aout_gain)
			{
				Aout_gain = Aout_gain-100;
				makeQuery1(77,Aout_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)77<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==4)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)77<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Aout_gain<(32767-10))
			{
				Aout_gain = Aout_gain+10;
				makeQuery1(77,Aout_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((10 -32768) <= Vout_gain)
			{
				Vout_gain = Vout_gain-10;
				makeQuery1(77,Aout_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)77<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==5)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)77<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Aout_gain<(32767-1))
			{
				Aout_gain = Aout_gain+1;
				makeQuery1(77,Aout_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((1 -32768) <= Aout_gain)
			{
				Aout_gain = Aout_gain-1;
				makeQuery1(77,Aout_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)77<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
}

void SYS_440300(void)
{
	static unsigned char flag=0;

	if((!EventFlagB)&&(flag))// event down edge
	{
		naviMENU = naviMENU / 10;
	}
	else if((EventFlagB)&&(!flag))// event up edge
	{
	}
	flag = EventFlagB;
}

void SYS_4404(void)
{
	if(data_array[6] != main_data_array[6])
	{
		data_array[6] = main_data_array[6];
		RefreshFlag=1;
	}

	if((unsigned int)Vbat_gain != (unsigned int)main_data_array[81]){Vbat_gain = main_data_array[81];RefreshFlag=1;}
	if((unsigned int)Vbat_offset != (unsigned int)main_data_array[80]){Vbat_offset = main_data_array[80];RefreshFlag=1;}

	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == ENTER)
	{
		Vbat_offset = main_data_array[80];
		Vbat_gain = main_data_array[81];
		naviMENU = naviMENU * 10;
		posInpage=0;
	}

}

//Vbat_offset
void SYS_44040(void)
{
	if(data_array[6] != main_data_array[6])
	{
		data_array[6] = main_data_array[6];
		RefreshFlag=1;
	}

	if(posInpage==0)
	{
		if((unsigned int)Vbat_offset != (unsigned int)main_data_array[80]){Vbat_offset = main_data_array[80];RefreshFlag=1;}
		
		if(KeyState.KeyValue == ESC){			naviMENU = naviMENU / 10;}
		else if(KeyState.KeyValue == UP)		naviMENU = naviMENU - 1;
		else if(KeyState.KeyValue == DN)		naviMENU = naviMENU + 1;
		else if(KeyState.KeyValue == RIGHT){	posInpage=5;RefreshFlag=1;}
		else if(KeyState.KeyValue == LIFT)	{	posInpage=0;RefreshFlag=1;}
		else if(KeyState.KeyValue == ENTER){	posInpage=5;RefreshFlag=1;}
	}
	else if(posInpage==1)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)80<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Vbat_offset<55535)
			{
				Vbat_offset = Vbat_offset+10000;
				makeQuery1(80,Vbat_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(10000 <= Vbat_offset)
			{
				Vbat_offset = Vbat_offset-10000;
				makeQuery1(80,Vbat_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)80<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==2)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)80<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if((Vbat_offset%10000)<9000)
			{
				if((Vbat_offset/1000)<65)
				{
					Vbat_offset = Vbat_offset+1000;
					makeQuery1(80,Vbat_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
					RefreshFlag=1;
				}
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(1000 <= (Vbat_offset%10000))
			{
				Vbat_offset = Vbat_offset-1000;
				makeQuery1(80,Vbat_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)80<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}

	}
	else if(posInpage==3)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)80<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if((Vbat_offset%1000)<900)
			{
				if((Vbat_offset/100)<655)
				{
					Vbat_offset = Vbat_offset+100;
					makeQuery1(80,Vbat_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
					RefreshFlag=1;
				}
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(100 <= (Vbat_offset%1000))
			{
				Vbat_offset = Vbat_offset-100;
				makeQuery1(80,Vbat_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)80<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==4)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)80<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if((Vbat_offset%100)<90)
			{
				if((Vbat_offset/10)<6553)
				{
					Vbat_offset = Vbat_offset+10;
					makeQuery1(80,Vbat_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
					RefreshFlag=1;
				}
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(10 <= (Vbat_offset%100))
			{
				Vbat_offset = Vbat_offset-10;
				makeQuery1(80,Vbat_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)80<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==5)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)80<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if((Vbat_offset%10)<9)
			{
				if((Vbat_offset/1)<65535)
				{
					Vbat_offset = Vbat_offset+1;
					makeQuery1(80,Vbat_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
					RefreshFlag=1;
				}
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(1 <= (Vbat_offset%10))
			{
				Vbat_offset = Vbat_offset-1;
				makeQuery1(80,Vbat_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)80<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}

	if(KeyState.KeyValue == ESC)
	{
		Vbat_offset = main_data_array[80];
		Vbat_gain = main_data_array[81];
		RefreshFlag=1;
	}
}

//Vbat_gain
void SYS_44041(void)
{
	if(data_array[6] != main_data_array[6])
	{
		data_array[6] = main_data_array[6];
		RefreshFlag=1;
	}

	if(posInpage==0)
	{
		if((unsigned int)Vbat_gain != (unsigned int)main_data_array[81]){Vbat_gain = main_data_array[81];RefreshFlag=1;}
		
		if(KeyState.KeyValue == ESC){			naviMENU = naviMENU / 10;}
		else if(KeyState.KeyValue == UP)		naviMENU = naviMENU - 1;
		else if(KeyState.KeyValue == DN)		naviMENU = naviMENU + 1;
		else if(KeyState.KeyValue == RIGHT){	posInpage=5;RefreshFlag=1;}
		else if(KeyState.KeyValue == LIFT)	{	posInpage=0;RefreshFlag=1;}
		else if(KeyState.KeyValue == ENTER){	posInpage=5;RefreshFlag=1;}
	}
	else if(posInpage==1)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)81<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Vbat_gain<(32767-10000))
			{
				Vbat_gain = Vbat_gain+10000;
				makeQuery1(81,Vbat_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((10000 -32768) <= Vbat_gain)
			{
				Vbat_gain = Vbat_gain-10000;
				makeQuery1(81,Vbat_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)81<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==2)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)81<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Vbat_gain<(32767-1000))
			{
				Vbat_gain = Vbat_gain+1000;
				makeQuery1(81,Vbat_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((1000 -32768) <= Vbat_gain)
			{
				Vbat_gain = Vbat_gain-1000;
				makeQuery1(81,Vbat_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)81<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}

	}
	else if(posInpage==3)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)81<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Vbat_gain<(32767-100))
			{
				Vbat_gain = Vbat_gain+100;
				makeQuery1(81,Vbat_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((100 -32768) <= Vbat_gain)
			{
				Vbat_gain = Vbat_gain-100;
				makeQuery1(81,Vbat_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)81<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==4)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)81<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Vbat_gain<(32767-10))
			{
				Vbat_gain = Vbat_gain+10;
				makeQuery1(81,Vbat_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((10 -32768) <= Vbat_gain)
			{
				Vbat_gain = Vbat_gain-10;
				makeQuery1(81,Vbat_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)81<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==5)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)81<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Vbat_gain<(32767-1))
			{
				Vbat_gain = Vbat_gain+1;
				makeQuery1(81,Vbat_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((1 -32768) <= Vbat_gain)
			{
				Vbat_gain = Vbat_gain-1;
				makeQuery1(81,Vbat_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)81<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
}

void SYS_440400(void)
{
	static unsigned char flag=0;

	if((!EventFlagB)&&(flag))// event down edge
	{
		naviMENU = naviMENU / 10;
	}
	else if((EventFlagB)&&(!flag))// event up edge
	{
	}
	flag = EventFlagB;
}

void SYS_4405(void)
{
	if(data_array[5] != main_data_array[5])
	{
		data_array[5] = main_data_array[5];
		RefreshFlag=1;
	}
	
	if((unsigned int)Abat_offset != (unsigned int)main_data_array[82]){Abat_offset = main_data_array[82];RefreshFlag=1;}
	if((unsigned int)Abat_gain != (unsigned int)main_data_array[83]){Abat_gain = main_data_array[83];RefreshFlag=1;}

	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == ENTER)
	{
		Abat_offset = main_data_array[82];
		Abat_gain = main_data_array[83];
		naviMENU = naviMENU * 10;
		posInpage=0;
	}
}

//Abat_offset
void SYS_44050(void)
{
	if(data_array[5] != main_data_array[5])
	{
		data_array[5] = main_data_array[5];
		RefreshFlag=1;
	}

	if(posInpage==0)
	{
		if((unsigned int)Abat_offset != (unsigned int)main_data_array[82]){Abat_offset = main_data_array[82];RefreshFlag=1;}
		
		if(KeyState.KeyValue == ESC){			naviMENU = naviMENU / 10;}
		else if(KeyState.KeyValue == UP)		naviMENU = naviMENU - 1;
		else if(KeyState.KeyValue == DN)		naviMENU = naviMENU + 1;
		else if(KeyState.KeyValue == RIGHT){	posInpage=5;RefreshFlag=1;}
		else if(KeyState.KeyValue == LIFT)	{	posInpage=0;RefreshFlag=1;}
		else if(KeyState.KeyValue == ENTER){	posInpage=5;RefreshFlag=1;}
	}
	else if(posInpage==1)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)82<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Abat_offset<55535)
			{
				Abat_offset = Abat_offset+10000;
				makeQuery1(82,Abat_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(10000 <= Abat_offset)
			{
				Abat_offset = Abat_offset-10000;
				makeQuery1(82,Abat_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)82<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==2)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)82<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if((Abat_offset%10000)<9000)
			{
				if((Abat_offset/1000)<65)
				{
					Abat_offset = Abat_offset+1000;
					makeQuery1(82,Abat_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
					RefreshFlag=1;
				}
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(1000 <= (Abat_offset%10000))
			{
				Abat_offset = Abat_offset-1000;
				makeQuery1(82,Abat_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)82<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}

	}
	else if(posInpage==3)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)82<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if((Abat_offset%1000)<900)
			{
				if((Abat_offset/100)<655)
				{
					Abat_offset = Abat_offset+100;
					makeQuery1(82,Abat_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
					RefreshFlag=1;
				}
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(100 <= (Abat_offset%1000))
			{
				Abat_offset = Abat_offset-100;
				makeQuery1(82,Abat_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)82<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==4)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)82<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if((Abat_offset%100)<90)
			{
				if((Abat_offset/10)<6553)
				{
					Abat_offset = Abat_offset+10;
					makeQuery1(82,Abat_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
					RefreshFlag=1;
				}
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(10 <= (Abat_offset%100))
			{
				Abat_offset = Abat_offset-10;
				makeQuery1(82,Abat_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)82<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==5)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)82<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if((Abat_offset%10)<9)
			{
				if((Abat_offset/1)<65535)
				{
					Abat_offset = Abat_offset+1;
					makeQuery1(82,Abat_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
					RefreshFlag=1;
				}
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(1 <= (Abat_offset%10))
			{
				Abat_offset = Abat_offset-1;
				makeQuery1(82,Abat_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)82<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
}

//Abat_gain
void SYS_44051(void)
{
	if(data_array[5] != main_data_array[5])
	{
		data_array[5] = main_data_array[5];
		RefreshFlag=1;
	}

	if(posInpage==0)
	{
		if((unsigned int)Abat_gain != (unsigned int)main_data_array[83]){Abat_gain = main_data_array[83];RefreshFlag=1;}
		
		if(KeyState.KeyValue == ESC){			naviMENU = naviMENU / 10;}
		else if(KeyState.KeyValue == UP)		naviMENU = naviMENU - 1;
		else if(KeyState.KeyValue == DN)		naviMENU = naviMENU + 1;
		else if(KeyState.KeyValue == RIGHT){	posInpage=5;RefreshFlag=1;}
		else if(KeyState.KeyValue == LIFT)	{	posInpage=0;RefreshFlag=1;}
		else if(KeyState.KeyValue == ENTER){	posInpage=5;RefreshFlag=1;}
	}
	else if(posInpage==1)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)83<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Abat_gain<(32767-10000))
			{
				Abat_gain = Abat_gain+10000;
				makeQuery1(83,Abat_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((10000 -32768) <= Abat_gain)
			{
				Abat_gain = Abat_gain-10000;
				makeQuery1(83,Abat_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)83<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==2)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)83<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Abat_gain<(32767-1000))
			{
				Abat_gain = Abat_gain+1000;
				makeQuery1(83,Abat_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((1000 -32768) <= Abat_gain)
			{
				Abat_gain = Abat_gain-1000;
				makeQuery1(83,Abat_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)83<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}

	}
	else if(posInpage==3)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)83<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Abat_gain<(32767-100))
			{
				Abat_gain = Abat_gain+100;
				makeQuery1(83,Abat_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((100 -32768) <= Abat_gain)
			{
				Abat_gain = Abat_gain-100;
				makeQuery1(83,Abat_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)83<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==4)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)83<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Abat_gain<(32767-10))
			{
				Abat_gain = Abat_gain+10;
				makeQuery1(83,Abat_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((10 -32768) <= Abat_gain)
			{
				Abat_gain = Abat_gain-10;
				makeQuery1(83,Abat_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)83<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==5)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)83<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Abat_gain<(32767-1))
			{
				Abat_gain = Abat_gain+1;
				makeQuery1(83,Abat_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((1 -32768) <= Abat_gain)
			{
				Abat_gain = Abat_gain-1;
				makeQuery1(83,Abat_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)83<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
}

void SYS_440500(void)
{
	static unsigned char flag=0;

	if((!EventFlagB)&&(flag))// event down edge
	{
		naviMENU = naviMENU / 10;
	}
	else if((EventFlagB)&&(!flag))// event up edge
	{
	}
	flag = EventFlagB;
}


void SYS_4406(void)
{
	if(data_array[0] != main_data_array[0])
	{
		data_array[0] = main_data_array[0];
		RefreshFlag=1;
	}
	
if((unsigned int)Vbyp_offset != (unsigned int)main_data_array[78]){Vbyp_offset = main_data_array[78];RefreshFlag=1;}
if((unsigned int)Vbyp_gain != (unsigned int)main_data_array[79]){Vbyp_gain = main_data_array[79];RefreshFlag=1;}

	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == ENTER)
	{
		Vbyp_offset = main_data_array[78];
		Vbyp_gain = main_data_array[79];
		naviMENU = naviMENU * 10;
		posInpage=0;
	}
}

//Vbyp_offset
void SYS_44060(void)
{
	if(data_array[0] != main_data_array[0])
	{
		data_array[0] = main_data_array[0];
		RefreshFlag=1;
	}

	if(posInpage==0)
	{
		if((unsigned int)Vbyp_offset != (unsigned int)main_data_array[78]){Vbyp_offset = main_data_array[78];RefreshFlag=1;}
		
		if(KeyState.KeyValue == ESC){			naviMENU = naviMENU / 10;}
		else if(KeyState.KeyValue == UP)		naviMENU = naviMENU - 1;
		else if(KeyState.KeyValue == DN)		naviMENU = naviMENU + 1;
		else if(KeyState.KeyValue == RIGHT){	posInpage=5;RefreshFlag=1;}
		else if(KeyState.KeyValue == LIFT)	{	posInpage=0;RefreshFlag=1;}
		else if(KeyState.KeyValue == ENTER){	posInpage=5;RefreshFlag=1;}
	}
	else if(posInpage==1)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)78<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Vbyp_offset<55535)
			{
				Vbyp_offset = Vbyp_offset+10000;
				makeQuery1(78,Vbyp_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(10000 <= Vbyp_offset)
			{
				Vbyp_offset = Vbyp_offset-10000;
				makeQuery1(78,Vbyp_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)78<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==2)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)78<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if((Vbyp_offset%10000)<9000)
			{
				if((Vbyp_offset/1000)<65)
				{
					Vbyp_offset = Vbyp_offset+1000;
					makeQuery1(78,Vbyp_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
					RefreshFlag=1;
				}
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(1000 <= (Vbyp_offset%10000))
			{
				Vbyp_offset = Vbyp_offset-1000;
				makeQuery1(78,Vbyp_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)78<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}

	}
	else if(posInpage==3)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)78<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if((Vbyp_offset%1000)<900)
			{
				if((Vbyp_offset/100)<655)
				{
					Vbyp_offset = Vbyp_offset+100;
					makeQuery1(78,Vbyp_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
					RefreshFlag=1;
				}
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(100 <= (Vbyp_offset%1000))
			{
				Vbyp_offset = Vbyp_offset-100;
				makeQuery1(78,Vbyp_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)78<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==4)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)78<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if((Vbyp_offset%100)<90)
			{
				if((Vbyp_offset/10)<6553)
				{
					Vbyp_offset = Vbyp_offset+10;
					makeQuery1(78,Vbyp_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
					RefreshFlag=1;
				}
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(10 <= (Vbyp_offset%100))
			{
				Vbyp_offset = Vbyp_offset-10;
				makeQuery1(78,Vbyp_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)78<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==5)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)78<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if((Vbyp_offset%10)<9)
			{
				if((Vbyp_offset/1)<65535)
				{
					Vbyp_offset = Vbyp_offset+1;
					makeQuery1(78,Vbyp_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
					RefreshFlag=1;
				}
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(1 <= (Vbyp_offset%10))
			{
				Vbyp_offset = Vbyp_offset-1;
				makeQuery1(78,Vbyp_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)78<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
}

void SYS_44061(void)
{
	if(data_array[0] != main_data_array[0])
	{
		data_array[0] = main_data_array[0];
		RefreshFlag=1;
	}

	if(posInpage==0)
	{
		if((unsigned int)Vbyp_gain != (unsigned int)main_data_array[79]){Vbyp_gain = main_data_array[79];RefreshFlag=1;}
		
		if(KeyState.KeyValue == ESC){			naviMENU = naviMENU / 10;}
		else if(KeyState.KeyValue == UP)		naviMENU = naviMENU - 1;
		else if(KeyState.KeyValue == DN)		naviMENU = naviMENU + 1;
		else if(KeyState.KeyValue == RIGHT){	posInpage=5;RefreshFlag=1;}
		else if(KeyState.KeyValue == LIFT)	{	posInpage=0;RefreshFlag=1;}
		else if(KeyState.KeyValue == ENTER){	posInpage=5;RefreshFlag=1;}
	}
	else if(posInpage==1)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)79<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Vbyp_gain<(32767-10000))
			{
				Vbyp_gain = Vbyp_gain+10000;
				makeQuery1(79,Vbyp_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((10000 -32768) <= Vbyp_gain)
			{
				Vbyp_gain = Vbyp_gain-10000;
				makeQuery1(79,Vbyp_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)79<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==2)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)79<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Vbyp_gain<(32767-1000))
			{
				Vbyp_gain = Vbyp_gain+1000;
				makeQuery1(79,Vbyp_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((1000 -32768) <= Vbyp_gain)
			{
				Vbyp_gain = Vbyp_gain-1000;
				makeQuery1(79,Vbyp_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)79<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}

	}
	else if(posInpage==3)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)79<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Vbyp_gain<(32767-100))
			{
				Vbyp_gain = Vbyp_gain+100;
				makeQuery1(79,Vbyp_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((100 -32768) <= Vbyp_gain)
			{
				Vbyp_gain = Vbyp_gain-100;
				makeQuery1(79,Vbyp_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)79<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==4)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)79<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Vbyp_gain<(32767-10))
			{
				Vbyp_gain = Vbyp_gain+10;
				makeQuery1(79,Vbyp_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((10 -32768) <= Vbyp_gain)
			{
				Vbyp_gain = Vbyp_gain-10;
				makeQuery1(79,Vbyp_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)79<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==5)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)79<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Vbyp_gain<(32767-1))
			{
				Vbyp_gain = Vbyp_gain+1;
				makeQuery1(79,Vbyp_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((1 -32768) <= Vbyp_gain)
			{
				Vbyp_gain = Vbyp_gain-1;
				makeQuery1(79,Vbyp_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)79<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
}

void SYS_440600(void)
{
	static unsigned char flag=0;

	if((!EventFlagB)&&(flag))// event down edge
	{
		naviMENU = naviMENU / 10;
	}
	else if((EventFlagB)&&(!flag))// event up edge
	{

	}
	flag = EventFlagB;
}

void SYS_4407(void)
{
	if(data_array[0] != main_data_array[0])
	{
		data_array[0] = main_data_array[0];
		RefreshFlag=1;
	}

if((unsigned int)Vdc_offset != (unsigned int)main_data_array[84]){Vdc_offset= main_data_array[84];RefreshFlag=1;}
if((unsigned int)Vdc_gain != (unsigned int)main_data_array[85]){Vdc_gain = main_data_array[85];RefreshFlag=1;}

	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == ENTER)
	{
		Vdc_offset= main_data_array[84];
		Vdc_gain = main_data_array[85];
		naviMENU = naviMENU * 10;
		posInpage=0;
	}
}

//Vdc_offset
void SYS_44070(void)
{
	if(data_array[6] != main_data_array[6])
	{
		data_array[6] = main_data_array[6];
		RefreshFlag=1;
	}

	if(posInpage==0)
	{
		if((unsigned int)Vdc_offset != (unsigned int)main_data_array[84]){Vdc_offset= main_data_array[84];RefreshFlag=1;}
		
		if(KeyState.KeyValue == ESC){			naviMENU = naviMENU / 10;}
		else if(KeyState.KeyValue == UP)		naviMENU = naviMENU - 1;
		else if(KeyState.KeyValue == DN)		naviMENU = naviMENU + 1;
		else if(KeyState.KeyValue == RIGHT){	posInpage=5;RefreshFlag=1;}
		else if(KeyState.KeyValue == LIFT)	{	posInpage=0;RefreshFlag=1;}
		else if(KeyState.KeyValue == ENTER){	posInpage=5;RefreshFlag=1;}
	}
	else if(posInpage==1)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)84<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Vdc_offset<55535)
			{
				Vdc_offset = Vdc_offset+10000;
				makeQuery1(84,Vdc_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(10000 <= Vdc_offset)
			{
				Vdc_offset = Vdc_offset-10000;
				makeQuery1(84,Vdc_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)84<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==2)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)84<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if((Vdc_offset%10000)<9000)
			{
				if((Vdc_offset/1000)<65)
				{
					Vdc_offset = Vdc_offset+1000;
					makeQuery1(84,Vdc_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
					RefreshFlag=1;
				}
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(1000 <= (Vdc_offset%10000))
			{
				Vdc_offset = Vdc_offset-1000;
				makeQuery1(84,Vdc_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)84<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}

	}
	else if(posInpage==3)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)84<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if((Vdc_offset%1000)<900)
			{
				if((Vdc_offset/100)<655)
				{
					Vdc_offset = Vdc_offset+100;
					makeQuery1(84,Vdc_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
					RefreshFlag=1;
				}
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(100 <= (Vdc_offset%1000))
			{
				Vdc_offset = Vdc_offset-100;
				makeQuery1(84,Vdc_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)84<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==4)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)84<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if((Vdc_offset%100)<90)
			{
				if((Vdc_offset/10)<6553)
				{
					Vdc_offset = Vdc_offset+10;
					makeQuery1(84,Vdc_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
					RefreshFlag=1;
				}
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(10 <= (Vdc_offset%100))
			{
				Vdc_offset = Vdc_offset-10;
				makeQuery1(84,Vdc_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)84<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==5)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)84<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if((Vdc_offset%10)<9)
			{
				if((Vdc_offset/1)<65535)
				{
					Vdc_offset = Vdc_offset+1;
					makeQuery1(84,Vdc_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
					RefreshFlag=1;
				}
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if(1 <= (Vdc_offset%10))
			{
				Vdc_offset = Vdc_offset-1;
				makeQuery1(84,Vdc_offset,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)84<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
}

void SYS_44071(void)
{
	if(data_array[0] != main_data_array[0])
	{
		data_array[0] = main_data_array[0];
		RefreshFlag=1;
	}

	if(posInpage==0)
	{
		if((unsigned int)Vdc_gain != (unsigned int)main_data_array[85]){Vdc_gain = main_data_array[85];RefreshFlag=1;}
		
		if(KeyState.KeyValue == ESC){			naviMENU = naviMENU / 10;}
		else if(KeyState.KeyValue == UP)		naviMENU = naviMENU - 1;
		else if(KeyState.KeyValue == DN)		naviMENU = naviMENU + 1;
		else if(KeyState.KeyValue == RIGHT){	posInpage=5;RefreshFlag=1;}
		else if(KeyState.KeyValue == LIFT)	{	posInpage=0;RefreshFlag=1;}
		else if(KeyState.KeyValue == ENTER){	posInpage=5;RefreshFlag=1;}
	}
	else if(posInpage==1)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)85<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Vdc_gain<(32767-10000))
			{
				Vdc_gain = Vdc_gain+10000;
				makeQuery1(85,Vdc_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((10000 -32768) <= Vdc_gain)
			{
				Vdc_gain = Vdc_gain-10000;
				makeQuery1(85,Vdc_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)85<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==2)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)85<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Vdc_gain<(32767-1000))
			{
				Vdc_gain = Vdc_gain+1000;
				makeQuery1(85,Vdc_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((1000 -32768) <= Vdc_gain)
			{
				Vdc_gain = Vdc_gain-1000;
				makeQuery1(85,Vdc_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)85<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}

	}
	else if(posInpage==3)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)85<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Vdc_gain<(32767-100))
			{
				Vdc_gain = Vdc_gain+100;
				makeQuery1(85,Vdc_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((100 -32768) <= Vdc_gain)
			{
				Vdc_gain = Vdc_gain-100;
				makeQuery1(85,Vdc_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)85<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==4)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)85<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Vdc_gain<(32767-10))
			{
				Vdc_gain = Vdc_gain+10;
				makeQuery1(85,Vdc_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((10 -32768) <= Vdc_gain)
			{
				Vdc_gain = Vdc_gain-10;
				makeQuery1(85,Vdc_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == RIGHT)
		{
			posInpage++;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)85<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
	else if(posInpage==5)
	{
		if(KeyState.KeyValue == ESC)
		{
			posInpage=0;RefreshFlag=1;
			makeQuery1(98,((int)85<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
		}
		else if(KeyState.KeyValue == UP)
		{
			if(Vdc_gain<(32767-1))
			{
				Vdc_gain = Vdc_gain+1;
				makeQuery1(85,Vdc_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == DN)
		{
			if((1 -32768) <= Vdc_gain)
			{
				Vdc_gain = Vdc_gain-1;
				makeQuery1(85,Vdc_gain,MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
				RefreshFlag=1;
			}
		}
		else if(KeyState.KeyValue == LIFT)
		{
			posInpage--;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			naviMENU = naviMENU * 10;
			makeQuery1(97,((int)85<<8 | 0x00FF),MODBUS_WRITE_REGISTER,MODBUS_NODE_ID);
			EventFlagB++;
		}
	}
}

void SYS_440700(void)
{
	static unsigned char flag=0;

	if((!EventFlagB)&&(flag))// event down edge
	{
		naviMENU = naviMENU / 10;
	}
	else if((EventFlagB)&&(!flag))// event up edge
	{

	}
	flag = EventFlagB;
}



void SYS_45(void)
{
	if(KeyState.KeyValue == ESC)
	{
		naviMENU = naviMENU / 10;
		confirm=0;
	}
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == ENTER)naviMENU = naviMENU * 10;
}


void SYS_450(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == ENTER)
	{
		naviMENU = naviMENU * 10;
		_password[0]=0;
		_password[1]=0;
		_password[2]=0;
		_password[3]=0;
	}

}
void SYS_4500(void)
{
	if(KeyState.KeyValue == ESC)
	{
		naviMENU = naviMENU / 10;
		_password[0]=0;
		_password[1]=0;
		_password[2]=0;
		_password[3]=0;
	}
	else if(KeyState.KeyValue == ENTER)
	{
		naviMENU = naviMENU * 10;
		EventFlagB=1;
	}
	else if(KeyState.KeyValue == UP)
	{
		_password[pass_pos]++;
		if(9 < _password[pass_pos])_password[pass_pos]=0;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == DN)
	{
		if(_password[pass_pos])_password[pass_pos]--;
		else _password[pass_pos] = 9;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == RIGHT)
	{
		pass_pos++;
		if(3 <pass_pos)pass_pos=0;
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == LIFT)
	{
		if(pass_pos)pass_pos--;
		else pass_pos = 3;
	
		RefreshFlag=1;
	}


}

void SYS_45000(void)
{
	static unsigned char flag=0;

	if((!EventFlagB)&&(flag))// event down edge
	{
		naviMENU = naviMENU / 10;
	}
	else if((EventFlagB)&&(!flag))// event up edge
	{
		password[0]=_password[0];
		password[1]=_password[1];
		password[2]=_password[2];
		password[3]=_password[3];
	}
	flag = EventFlagB;
}

void SYS_451(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
	else if(KeyState.KeyValue == ENTER)
	{
			setTime[0] = year;
			setTime[1] = month;
			setTime[2] = date;
			setTime[3] = hour;
			setTime[4] = min;
			setTime[5] = second;
			setTime[6] = 0x01;
		naviMENU = naviMENU * 10;

	}
}
void SYS_4510(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == ENTER)
	{
		if(KeyState.LongKeyFlag)
		{

			setTime[0]=0x09;	 // y	09년
			setTime[1]=0x05;	 // m	04월
			setTime[2]=0x27;	 // date 24일
			setTime[3]=0x03;	 // day   
			setTime[4]=0x00;	 // h	13시
			setTime[5]=0x00;	 // m	27분
			setTime[6]=0x00;	 // s	39초

		}
		naviMENU = naviMENU * 10;
		EventFlagB=1;
	}
	else if(KeyState.KeyValue == UP)
	{
		setTime[Tset_pos]++;
		if(Tset_pos==0)
		{
			if(99 < setTime[Tset_pos])setTime[Tset_pos]=0;
		}
		else if(Tset_pos==1)
		{
			if(12 < setTime[Tset_pos])setTime[Tset_pos]=1;
		}
		else if(Tset_pos==2)
		{
			if(31 < setTime[Tset_pos])setTime[Tset_pos]=1;
		}
		else if(Tset_pos==3)
		{
			if(23 < setTime[Tset_pos])setTime[Tset_pos]=0;
		}
		else if(Tset_pos==4)
		{
			if(59 < setTime[Tset_pos])setTime[Tset_pos]=0;
		}
		else if(Tset_pos==5)
		{
			setTime[Tset_pos]=0;
		}
		
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == DN)
	{
		if(Tset_pos==0)
		{
			if(setTime[Tset_pos])setTime[Tset_pos]--;
			else setTime[Tset_pos] = 99;
		}
		else if(Tset_pos==1)
		{
			if(setTime[Tset_pos])setTime[Tset_pos]--;
			else setTime[Tset_pos] = 12;
		}
		else if(Tset_pos==2)
		{
			if(setTime[Tset_pos])setTime[Tset_pos]--;
			else setTime[Tset_pos] = 31;
		}
		else if(Tset_pos==3)
		{
			if(setTime[Tset_pos])setTime[Tset_pos]--;
			else setTime[Tset_pos] = 23;
		}
		else if(Tset_pos==4)
		{
			if(setTime[Tset_pos])setTime[Tset_pos]--;
			else setTime[Tset_pos] = 59;
		}
		else if(Tset_pos==5)
		{
			setTime[Tset_pos]=0;
		}

	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == RIGHT)
	{
		Tset_pos++;
		if(5 <Tset_pos)Tset_pos=0;
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == LIFT)
	{
		if(Tset_pos)Tset_pos--;
		else Tset_pos = 5;
	
		RefreshFlag=1;
	}

}

void SYS_45100(void)
{
	static unsigned char flag=0;

	if((!EventFlagB)&&(flag))// event down edge
	{
		naviMENU = naviMENU / 10;
	}
	else if((EventFlagB)&&(!flag))// event up edge
	{
		ds1307_setTime(setTime[0],setTime[1],setTime[2], setTime[6], setTime[3], setTime[4], setTime[5] );
	}
	flag = EventFlagB;
}



void SYS_3(void)
{
	static unsigned char flag=0;
	unsigned char c;

	if(EventFlagC)
	{
		if(KeyState.KeyValue == ESC)
		{
			EventFlagC=0;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
		else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
		else if(KeyState.KeyValue == ENTER)
		{
			history_Xoffset=9;

			if(6 <= history_paper_line)	history_Yoffset = history_paper_line-6;
			else history_Yoffset = HISTORY_MAX - (6 - history_paper_line);

			naviMENU = naviMENU * 10;
		}
		else if(KeyState.KeyValue == (ESC  & UP & DN & ENTER))
		{
			naviMENU = 0xFFFFFFFF;
		}
		
		if(KeyState.KeyValue != 0xFF)EventTimeC=0;
	}
	else 
	{
		if(KeyState.KeyValue != 0xFF)EventFlagC=1;
		if(naviMENU!=old_naveMENU)EventFlagC=1;
	}

	if((!EventFlagC)&&(flag))RefreshFlag=1;// event edge
	else if((EventFlagC)&&(!flag))RefreshFlag=1;// event edge
	flag = EventFlagC;
}


void SYS_30(void)
{
	unsigned char offset;
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
	else if(KeyState.KeyValue == UP)
	{
	
		if(history_Yoffset!=history_paper_line)
		{
			if( history_Yoffset == 0)history_Yoffset = HISTORY_MAX - 1;
			else history_Yoffset--;
		}
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == DN)
	{
		offset = history_Yoffset+6;
		if(HISTORY_MAX <= offset) offset = offset % HISTORY_MAX;
		if(offset != history_paper_line)
		{
			if( HISTORY_MAX <= history_Yoffset ) history_Yoffset = 0;
			else	 history_Yoffset++;
		}
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == LIFT)
	{
		if(history_Xoffset)history_Xoffset--;
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == RIGHT)
	{
		if(history_Xoffset < 45)history_Xoffset++;
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == ENTER)
	{
		//history_update(0);
		//RefreshFlag=1;
	}


}


void SYS_0xFFFFFFFF(void)
{
	if(KeyState.KeyValue == (0xFE  & 0xFD & 0xFB & 0xDF))
	{
		naviMENU = 1;
	}
}

prog_uchar  system_event_inform[54][40]=
{
	"MODBUS ON",							//0
	              		
	"Battery OC"	,											//    1
	"Output OC"		,											//    2
	"Input OC"		,											//    3
	"DC LINK OV"	,											//    4
	"DC LINK UV"	,											//    5
	"Battery OV"	,											//    6
	"Battery UV"	,											//    7
	"Output OV"		,											//    8
	"Output UV"		,											//    9
	"Input OV"		,											//    10
	"Input UV"		,											//    11
	
	"Heat Sink OT"			,								//	12
	"Fan Fault"					,								//13
	"Bat Fuse Fault"		,								//14
	"Output Fuse Fault"	,								//15
	"Input Fuse Fault"	,								//16
	"DC_DC GDU Fault"		,								//17
	"INV GDU Fault"			,								//18
	"CONV GDU Fault"		,								//19
	"Power Stack Fault"	,								//20
	"INV Freq Fault"		,								//21
	"Input Freq Fault"	,								//22
	
	"Charge Mode Float"	,								//23
	"Charge Mode Equal"	,								//24
	"Async"							,								//25
	"Sync"							,								//26
	"Inverter OFF"			,								//27
	"Inverter ON"				,								//28
	"Converter OFF"			,								//29
	"Converter ON"			,								// 30                  
	"Battery Discharge"	,								//  31                 
	"Battery Charge"		,								//   32                     
	"Utility Outage"		,								//    33                      
	"Utility ON"				,								//     34                     
	"Inverter Feeding"	,								//      35                    
	"Bypass Feeding"		,								//       36  
	
	"Bat NFB Trip"		,									//        37                  
	"Input NFB Trip"	,									//         38                   
	"Bat MC Fault"		,									//          39                  
	"Input MC Fault"	,									//           40                 
	"Emergency NFB ON"				,					//    41        
	"Emergency NFB OFF"				,					//    42       
	"Output NFB ON"					,					//     43 
	"Output NFB OFF"					,					//     44      
	"Bypass Feeding NFB ON"	,					//      45      
	"Bypass Feeding NFB OFF"	,					//      46      
	"Battery RELAY ON"					,					//       47     
	"Battery RELAY OFF"					,					//       48     
	"Input NFB ON"						,					//        49   
	"Input NFB OFF"						,					//        50    
	"Bypass NFB ON"						,				//         51
	"Bypass NFB OFF"			,							//         52
	"MODBUS Fault"									//53
};   
                                                                                           

void All_Value_Refresh(void)
{
	Vin_lower_limit=main_data_array[50];
	Vin_upper_limit=main_data_array[51];
	Vout_lower_limit=main_data_array[52];
	Vout_upper_limit=main_data_array[53];
	Vbat_lower_limit = main_data_array[54];
	Vbat_upper_limit = main_data_array[55];
	Async_Lower_Limit=main_data_array[58];
	Async_Upper_Limit=main_data_array[59];
	
	Abat_limit=main_data_array[60];
	Bat_Equal_volt = main_data_array[61];
	Bat_Float_Volt = main_data_array[62];
	Bat_Equal_Curr = main_data_array[63];
	Vinv_ref =main_data_array[65];
	
	Vin_offset = main_data_array[70];
	Vin_gain = main_data_array[71];
	Ain_offset = main_data_array[72];
	Ain_gain = main_data_array[73];
	Vout_offset = main_data_array[74];
	Vout_gain = main_data_array[75];
	Aout_offset = main_data_array[76];
	Aout_gain = main_data_array[77];
	Vbyp_offset = main_data_array[78];
	Vbyp_gain = main_data_array[79];	
	Vbat_offset = main_data_array[80];
	Vbat_gain = main_data_array[81];
	Abat_offset = main_data_array[82];
	Abat_gain = main_data_array[83];
	Vdc_offset= main_data_array[84];
	Vdc_gain = main_data_array[85];

}


void Check_event(void)
{ 
	unsigned char BuzzerFlag=0;
	static unsigned char AlarmLED;
	static unsigned char _EventFlagE=0;
	
	if((event_array[0] != main_data_array[40]))
	{
		//if(event == 0x8000)history_update(16);						//
		//if(event == 0x4000)history_update(15);					//
		//if(event == 0x2000)history_update(14);					//
		//if(event == 0x1000)history_update(13);					//
		//if(event == 0x0800)history_update(12);						//

		if((main_data_array[40] & 0x0400) && (!(event_array[0]&0x0400))){history_update(1);BuzzerFlag=1;AlarmLED=1;}				//
		else if(!(main_data_array[40] & 0x0400) && ((event_array[0]&0x0400))){AlarmLED=0;GLCD_BuzzerOnOff(0);}	

		if((main_data_array[40] & 0x0200) && (!(event_array[0]&0x0200))){history_update(2);BuzzerFlag=1;AlarmLED=1;}				//
		else if(!(main_data_array[40] & 0x0200) && ((event_array[0]&0x0200))){AlarmLED=0;GLCD_BuzzerOnOff(0);}	

		if((main_data_array[40] & 0x0100) && (!(event_array[0]&0x0100))){history_update(3);BuzzerFlag=1;AlarmLED=1;}				//
		else if(!(main_data_array[40] & 0x0100) && ((event_array[0]&0x0100))){AlarmLED=0;GLCD_BuzzerOnOff(0);}	

		if((main_data_array[40] & 0x0080) && (!(event_array[0]&0x0080))){history_update(4);BuzzerFlag=1;AlarmLED=1;}				//
		else if(!(main_data_array[40] & 0x0080) && ((event_array[0]&0x0080))){AlarmLED=0;GLCD_BuzzerOnOff(0);}	

		if((main_data_array[40] & 0x0040) && (!(event_array[0]&0x0040))){history_update(5);BuzzerFlag=1;AlarmLED=1;}				//
		else if(!(main_data_array[40] & 0x0040) && ((event_array[0]&0x0040))){AlarmLED=0;GLCD_BuzzerOnOff(0);}	

		if((main_data_array[40] & 0x0020) && (!(event_array[0]&0x0020))){history_update(6);BuzzerFlag=1;AlarmLED=1;}				//
		else if(!(main_data_array[40] & 0x0020) && ((event_array[0]&0x0020))){AlarmLED=0;GLCD_BuzzerOnOff(0);}	

		if((main_data_array[40] & 0x0010) && (!(event_array[0]&0x0010))){history_update(7);BuzzerFlag=1;AlarmLED=1;}				//
		else if(!(main_data_array[40] & 0x0010) && ((event_array[0]&0x0010))){AlarmLED=0;GLCD_BuzzerOnOff(0);}	

		if((main_data_array[40] & 0x0008) && (!(event_array[0]&0x0008))){history_update(8);BuzzerFlag=1;AlarmLED=1;}				//
		else if(!(main_data_array[40] & 0x0008) && ((event_array[0]&0x0008))){AlarmLED=0;GLCD_BuzzerOnOff(0);}	

		if((main_data_array[40] & 0x0004) && (!(event_array[0]&0x0004))){history_update(9);BuzzerFlag=1;AlarmLED=1;}				//
		else if(!(main_data_array[40] & 0x0004) && ((event_array[0]&0x0004))){AlarmLED=0;GLCD_BuzzerOnOff(0);}	

		if((main_data_array[40] & 0x0002) && (!(event_array[0]&0x0002))){history_update(10);BuzzerFlag=1;AlarmLED=1;}				//
		else if(!(main_data_array[40] & 0x0002) && ((event_array[0]&0x0002))){AlarmLED=0;GLCD_BuzzerOnOff(0);}	

		if((main_data_array[40] & 0x0001) && (!(event_array[0]&0x0001))){history_update(11);BuzzerFlag=1;AlarmLED=1;}				//
		else if(!(main_data_array[40] & 0x0001) && ((event_array[0]&0x0001))){AlarmLED=0;GLCD_BuzzerOnOff(0);}	

		
		event_array[0] = main_data_array[40];
		RefreshFlag=1;
	}
	else if((event_array[1] != main_data_array[41]))
	{
		//if(event == 0x8000)history_update(32);						//
		//if(event == 0x4000)history_update(31);					//
		//if(event == 0x2000)history_update(26);     					//  

		if((main_data_array[41] & 0x1000) && (!(event_array[1]&0x1000))){history_update(12);BuzzerFlag=1;AlarmLED=1;}				//
		else if(!(main_data_array[41] & 0x1000) && ((event_array[1]&0x1000))){AlarmLED=0;GLCD_BuzzerOnOff(0);}	

		if((main_data_array[41] & 0x0800) && (!(event_array[1]&0x0800))){history_update(13);BuzzerFlag=1;AlarmLED=1;}				//
		else if(!(main_data_array[41] & 0x0800) && ((event_array[1]&0x0800))){AlarmLED=0;GLCD_BuzzerOnOff(0);}	

		if((main_data_array[41] & 0x0400) && (!(event_array[1]&0x0400))){history_update(14);BuzzerFlag=1;AlarmLED=1;}				//
		else if(!(main_data_array[41] & 0x0400) && ((event_array[1]&0x0400))){AlarmLED=0;GLCD_BuzzerOnOff(0);}	

		if((main_data_array[41] & 0x0200) && (!(event_array[1]&0x0200))){history_update(15);BuzzerFlag=1;AlarmLED=1;}				//
		else if(!(main_data_array[41] & 0x0200) && ((event_array[1]&0x0200))){AlarmLED=0;GLCD_BuzzerOnOff(0);}	

		if((main_data_array[41] & 0x0100) && (!(event_array[1]&0x0100))){history_update(16);BuzzerFlag=1;AlarmLED=1;}				//
		else if(!(main_data_array[41] & 0x0100) && ((event_array[1]&0x0100))){AlarmLED=0;GLCD_BuzzerOnOff(0);}	

		//if(event == 0x0080)history_update(20);					//         
		//if(event == 0x0040)history_update(19);					//  
		if((main_data_array[41] & 0x0020) && (!(event_array[1]&0x0020))){history_update(17);BuzzerFlag=1;AlarmLED=1;}				//
		else if(!(main_data_array[41] & 0x0020) && ((event_array[1]&0x0020))){AlarmLED=0;GLCD_BuzzerOnOff(0);}	

		if((main_data_array[41] & 0x0010) && (!(event_array[1]&0x0010))){history_update(18);BuzzerFlag=1;AlarmLED=1;}				//
		else if(!(main_data_array[41] & 0x0010) && ((event_array[1]&0x0010))){AlarmLED=0;GLCD_BuzzerOnOff(0);}	

		if((main_data_array[41] & 0x0008) && (!(event_array[1]&0x0008))){history_update(19);BuzzerFlag=1;AlarmLED=1;}				//
		else if(!(main_data_array[41] & 0x0008) && ((event_array[1]&0x0008))){AlarmLED=0;GLCD_BuzzerOnOff(0);}	

		if((main_data_array[41] & 0x0004) && (!(event_array[1]&0x0004))){history_update(20);BuzzerFlag=1;AlarmLED=1;}				//
		else if(!(main_data_array[41] & 0x0004) && ((event_array[1]&0x0004))){AlarmLED=0;GLCD_BuzzerOnOff(0);}	

		if((main_data_array[41] & 0x0002) && (!(event_array[1]&0x0002))){history_update(21);BuzzerFlag=1;AlarmLED=1;}				//
		else if(!(main_data_array[41] & 0x0002) && ((event_array[1]&0x0002))){AlarmLED=0;GLCD_BuzzerOnOff(0);}	

		if((main_data_array[41] & 0x0001) && (!(event_array[1]&0x0001))){history_update(22);BuzzerFlag=1;AlarmLED=1;}				//
		else if(!(main_data_array[41] & 0x0001) && ((event_array[1]&0x0001))){AlarmLED=0;GLCD_BuzzerOnOff(0);}	
			
		event_array[1] = main_data_array[41];      
		RefreshFlag=1;
	}
	else if((event_array[2] != main_data_array[42]))
	{
		//if(event == 0x8000)history_update(1);						//
		//if(event == 0x4000)history_update(23);					//	

		if((main_data_array[42] & 0x2000) && (!(event_array[2]&0x2000))){history_update(23);}				//
		else if(!(main_data_array[42] & 0x2000) && ((event_array[2]&0x2000))){}	

		if((main_data_array[42] & 0x1000) && (!(event_array[2]&0x1000))){history_update(24);}				//
		else if(!(main_data_array[42] & 0x1000) && ((event_array[2]&0x1000))){}	

		if((main_data_array[42] & 0x0800) && (!(event_array[2]&0x0800))){history_update(25);BuzzerFlag=1;AlarmLED=1;}				//
		else if(!(main_data_array[42] & 0x0800) && ((event_array[2]&0x0800))){AlarmLED=0;GLCD_BuzzerOnOff(0);}	

		if((main_data_array[42] & 0x0400) && (!(event_array[2]&0x0400))){history_update(26);  PORTJ = PORTJ | 0x80;}				//
		else if(!(main_data_array[42] & 0x0400) && ((event_array[2]&0x0400))){PORTJ = PORTJ & (~0x80);}	

		if((main_data_array[42] & 0x0200) && (!(event_array[2]&0x0200))){history_update(27);BuzzerFlag=1;AlarmLED=1;}				//
		else if(!(main_data_array[42] & 0x0200) && ((event_array[2]&0x0200))){AlarmLED=0;GLCD_BuzzerOnOff(0);}	

		if((main_data_array[42] & 0x0100) && (!(event_array[2]&0x0100))){history_update(28);}				//
		else if(!(main_data_array[42] & 0x0100) && ((event_array[2]&0x0100))){}	

//===================================================
//Vin OUT
		if((main_data_array[42] & 0x0008) && (!(event_array[2]&0x0008))){history_update(33);BuzzerFlag=1;AlarmLED=1;}				//
		else if(!(main_data_array[42] & 0x0008) && ((event_array[2]&0x0008))){AlarmLED=0;GLCD_BuzzerOnOff(0);}	

		if((main_data_array[42] & 0x0080) && (!(event_array[2]&0x0080))){history_update(29);BuzzerFlag=1;AlarmLED=1;}				//
		else if(!(main_data_array[42] & 0x0080) && ((event_array[2]&0x0080))){AlarmLED=0;GLCD_BuzzerOnOff(0);}	

		if((main_data_array[42] & 0x0020) && (!(event_array[2]&0x0020))){history_update(31);}				//
		else if(!(main_data_array[42] & 0x0020) && ((event_array[2]&0x0020))){}	
//====================================================

//====================================================
//Vin IN
		if((main_data_array[42] & 0x0004) && (!(event_array[2]&0x0004))){history_update(34);}				//
		else if(!(main_data_array[42] & 0x0004) && ((event_array[2]&0x0004))){}	

		if((main_data_array[42] & 0x0040) && (!(event_array[2]&0x0040))){history_update(30);}				//
		else if(!(main_data_array[42] & 0x0040) && ((event_array[2]&0x0040))){}	

		if((main_data_array[42] & 0x0010) && (!(event_array[2]&0x0010))){history_update(32);}				//
		else if(!(main_data_array[42] & 0x0010) && ((event_array[2]&0x0010))){}	
//====================================================

		if((main_data_array[42] & 0x0002) && (!(event_array[2]&0x0002))){history_update(35);}				//
		else if(!(main_data_array[42] & 0x0002) && ((event_array[2]&0x0002))){}	

		if((main_data_array[42] & 0x0001) && (!(event_array[2]&0x0001))){history_update(36);BuzzerFlag=1;AlarmLED=1;}				//
		else if(!(main_data_array[42] & 0x0001) && ((event_array[2]&0x0001))){AlarmLED=0;GLCD_BuzzerOnOff(0);}	

		
		event_array[2] = main_data_array[42];
		RefreshFlag=1;
	}
	else if((event_array[3] != main_data_array[43]))
	{

		if((main_data_array[43] & 0x8000) && (!(event_array[3]&0x8000))){history_update(37);BuzzerFlag=1;AlarmLED=1;}				//
		else if(!(main_data_array[43] & 0x8000) && ((event_array[3]&0x8000))){AlarmLED=0;}	

		if((main_data_array[43] & 0x4000) && (!(event_array[3]&0x4000))){history_update(38);BuzzerFlag=1;AlarmLED=1;}	
		else if(!(main_data_array[43] & 0x4000) && ((event_array[3]&0x4000))){AlarmLED=0;}	

		if((main_data_array[43] & 0x2000) && (!(event_array[3]&0x2000))){history_update(39);BuzzerFlag=1;AlarmLED=1;}	
		else if(!(main_data_array[43] & 0x2000) && ((event_array[3]&0x2000))){AlarmLED=0;GLCD_BuzzerOnOff(0);}	

		if((main_data_array[43] & 0x1000) && (!(event_array[3]&0x1000))){history_update(40);BuzzerFlag=1;AlarmLED=1;}				//
		else if(!(main_data_array[43] & 0x1000) && ((event_array[3]&0x1000))){AlarmLED=0;GLCD_BuzzerOnOff(0);}	

		//else if(event == 0x0800)history_update(52);					//
		//else if(event == 0x0400)history_update(53);					//
		//else if(event == 0x0200)history_update(54);					//
		//else if(event == 0x0100)history_update(55);					//
		//if(event == 0x0080)history_update(39);						//         
		//else if(event == 0x0040)history_update(38);					// 


		if((main_data_array[43] & 0x0020) && (!(event_array[3]&0x0020))){history_update(41);BuzzerFlag=1;AlarmLED=1; Point_ONOFF(15, 1);}					//
		else if(!(main_data_array[43] & 0x0020) && ((event_array[3]&0x0020))){history_update(42);AlarmLED=0;GLCD_BuzzerOnOff(0); Point_ONOFF(15, 0);}	

		if((main_data_array[43] & 0x0010) && (!(event_array[3]&0x0010))){history_update(43);AlarmLED=0;GLCD_BuzzerOnOff(0);Point_ONOFF(9, 1);}	
		else if(!(main_data_array[43] & 0x0010) && ((event_array[3]&0x0010))){history_update(44);BuzzerFlag=1;AlarmLED=1;Point_ONOFF(9, 0);}
		
		if((main_data_array[43] & 0x0008) && (!(event_array[3]&0x0008))){history_update(45);AlarmLED=0;GLCD_BuzzerOnOff(0);Point_ONOFF(14, 1);}	
		else if(!(main_data_array[43] & 0x0008) && ((event_array[3]&0x0008))){history_update(46);BuzzerFlag=1;AlarmLED=1;Point_ONOFF(14, 0);}
		
		if((main_data_array[43] & 0x0004) && (!(event_array[3]&0x0004))){history_update(47);AlarmLED=0;GLCD_BuzzerOnOff(0);Point_ONOFF(0, 1);}				// 
		else if(!(main_data_array[43] & 0x0004) && ((event_array[3]&0x0004))){history_update(48);BuzzerFlag=1;AlarmLED=1;Point_ONOFF(0, 0);}
		
		if((main_data_array[43] & 0x0002) && (!(event_array[3]&0x0002))){history_update(49);AlarmLED=0;GLCD_BuzzerOnOff(0);Point_ONOFF(12, 1);}					//     
		else if(!(main_data_array[43] & 0x0002) && ((event_array[3]&0x0002))){history_update(50);BuzzerFlag=1;AlarmLED=1;Point_ONOFF(12, 0);}

		if((main_data_array[43] & 0x0001) && (!(event_array[3]&0x0001))){history_update(51);AlarmLED=0;GLCD_BuzzerOnOff(0);Point_ONOFF(14, 1);}					//
		else if(!(main_data_array[43] & 0x0001) && ((event_array[3]&0x0001))){history_update(52);BuzzerFlag=1;AlarmLED=1;Point_ONOFF(14, 0);}

		
		event_array[3] = main_data_array[43];
		RefreshFlag=1;
	}


	if(!_communication_fault && communication_fault){history_update(53);BuzzerFlag=1;AlarmLED=1;RefreshFlag=1;}
	else if(_communication_fault && !communication_fault)
	{
		history_update(0);
		GLCD_BuzzerOnOff(0);
		AlarmLED=0;
		RefreshFlag=1;
	}
	_communication_fault=communication_fault;


	if(100<main_data_array[0])	PORTJ = PORTJ | 0x20;	//in
	else PORTJ = PORTJ & (~0x20);
	if(100<main_data_array[2])	PORTJ = PORTJ | 0x40;
	else PORTJ = PORTJ & (~0x40);//out


	if(KeyState.KeyValue != 0xFF)
	{
		EventFlagE = 1;
		EventTimeE = 0;
	}
	else
	{
		if(!_EventFlagE && EventFlagE)GLCD_BackLightOnOff(1);
		else if(_EventFlagE && !EventFlagE)GLCD_BackLightOnOff(0);
		_EventFlagE=EventFlagE;
					
	}
	

	if(AlarmLED)
	{
			PORTJ = PORTJ | 0x10;
			PORTJ = PORTJ & (~0x08);
	}
	else
	{
			PORTJ = PORTJ & (~0x10);
			PORTJ = PORTJ |0x08;
	}


	if(BuzzerFlag)
	{
		EventFlagD = 1;
		EventTimeD = 0;
		GLCD_BuzzerOnOff(1);
	}
	else
	{
		if(EventFlagD)
		{
			if(KeyState.KeyValue == ESC)
			{
				KeyState.KeyValue = 0xFF;
				EventFlagD = 0;
				EventTimeD = 0;
				GLCD_BuzzerOnOff(0);
			}
		}
		else
		{
			GLCD_BuzzerOnOff(0);
		}
	}
}






void history_veriable_init(void)
{
	unsigned int i;
	unsigned char c;

	unsigned char _byte1,_byte2;
	
	for(i=0; i<HISTORY_MAX;i++)
	{
		for(c=0; c<17; c++)
		{
			_byte1 = eeprom_read_byte ((char*)((history_paper_line * HISTORY_CHAR_MAX)+c));
			_byte2 = eeprom_read_byte ((char*)((              i                 * HISTORY_CHAR_MAX)+c));

			if(_byte1 < _byte2)
			{
				history_paper_line = i;
				break;
			}
			else if(_byte2 < _byte1)
			{
			 	break;
			}
		}
	}

	if(eeprom_read_byte( (char*)(history_paper_line * HISTORY_CHAR_MAX)) != 0xFF)	history_paper_line ++;
	if(HISTORY_MAX <=  history_paper_line)history_paper_line=0;

	if(6 <= history_paper_line)	history_Yoffset = history_paper_line-6;
	else history_Yoffset = HISTORY_MAX - (6 - history_paper_line);
	
}


void history_update(char history_eventCode)
{
	char *source;
	char *target;
	char _byte;
	char tmp_buf[256];
	source = &text_buf[0];

	eeprom_addr = history_paper_line*HISTORY_CHAR_MAX;
	source = (char*)_TEXT("%02d/%02d/%02d %02d:%02d:%02d",year,month,date,hour,min,second);
	while(1)
	{
		if(*source == '\0')
		{
			eeprom_write_byte ((char*)eeprom_addr++, ' ');
			source++;
			break;
		}
		eeprom_write_byte ((char*)eeprom_addr++, *source);
		source++;
	}

	source = &system_event_inform[history_eventCode][0];
	while(1)
	{
		_byte =  pgm_read_byte(source);
		if(_byte== '\0')
		{
			eeprom_write_byte ((char*)eeprom_addr++, _byte );
			break;
		}
		eeprom_write_byte ((char*)eeprom_addr++, _byte );
		source++;
	}

	history_paper_line++;
	if(HISTORY_MAX <=  history_paper_line)history_paper_line=0;
	

	if(6 <= history_paper_line)	history_Yoffset = history_paper_line-6;
	else history_Yoffset = HISTORY_MAX - (6 - history_paper_line);

}

void PAGE_LineScroll(unsigned char line)
{
	unsigned char i;
	static char order=0;
	if(LCDScrollOnOff)
	{
		for(i=0;i<128;i++)
		{
			if((pScrollDataStart <= (pScrollDataOffset+i)) &&( (pScrollDataOffset+i)<pScrollDataEnd) )
			{
				LCD_buffer[(line<<7)+i] = *(pScrollDataOffset+i) ; 
				LCD_buffer[((line+1)<<7)+i] = *(pScrollDataOffset+i+SCRLLBUF_SIZE) ; 
			}
			else
			{
				LCD_buffer[(line<<7)+i] = 0x00;
				LCD_buffer[((line+1)<<7)+i]= 0x00;
			}
		}
		pScrollDataOffset++;
		if((pScrollDataEnd) < pScrollDataOffset)
		{
			//pScrollDataOffset = pScrollDataStart - 128;

			order++;
			if(5<order)order=0;
			if(order==0)	GLCD_PutStringScrollBuf((char*)_TEXT("현재메뉴 : %d", naviMENU));
			else if(order==1)	GLCD_PutStringScrollBuf("한영전기공업 기술연구소");
			else if(order==2)	GLCD_PutStringScrollBuf("장비이상은 이곳으로 문의하세요 (Tel:031-672-2222)");
			else if(order==3)	GLCD_PutStringScrollBuf("");
			else if(order==4)	GLCD_PutStringScrollBuf("시험가동중입니다");
		}
	}
}

const char* _readPGM_TEXT(PGM_P p)
{
	char cnt;
	char* string = text_buf;	
	for(cnt=0; cnt<35; cnt++)
	{
		*string = pgm_read_byte(p+cnt);
		if(*string=='\0')break;
		string++;
		
	}
	string = &text_buf[0];
	return string;
}


#define TEXT_SCRATCH 12
const char* _TEXT(char const *format, ...)
{
	unsigned char scratch[TEXT_SCRATCH];
	unsigned char format_flag;
	unsigned short base;
	unsigned char *ptr;
	//double dou;
	unsigned char issigned=0;

	va_list ap;

	unsigned char markpluse=0;
	unsigned char islong=0;
	unsigned long u_val=0;
	long s_val=0;

	unsigned char fill;
	unsigned char width;

char* string = text_buf;


	va_start (ap, format);
	for (;;)
	{
		while ((format_flag = *(format++)) != '%')   // Until '%' or '\0' 
		{   
			if (!format_flag)
			{
				va_end (ap);
				*string = 0x00;
				string = &text_buf[0];
				return string;
			}
			//________________________________________________
			*string = format_flag;
			string ++;
			//________________________________________________
		}

		issigned=0; //default unsigned
		base = 10;

		format_flag = *format++; //get char after '%'

		width=0; //no formatting
		fill=0;  //no formatting
		if(format_flag=='+')
		{
			markpluse=1;
			format_flag = *format++; 
		}
		
		if(format_flag=='0' || format_flag==' ') //SPACE or ZERO padding  ?
		{
			fill=format_flag;
			format_flag = *format++; //get char after padding char
			if(format_flag>='0' && format_flag<='9')
			{
				width=format_flag-'0';
				format_flag = *format++; //get char after width char
			}
		}

		islong=0; //default int value

		if(format_flag=='l' || format_flag=='L') //Long value 
		{
			islong=1;
			format_flag = *format++; //get char after 'l' or 'L'
		}


		switch (format_flag)
		{
			case 'c':
				format_flag = va_arg(ap,int);
				// no break -> run into default
			default:
				//________________________________________________
				*string = '%';
				string ++;
				*string = format_flag;
				string ++;
				//________________________________________________

			continue;
		/*	case 'f':
				dou = va_arg(ap,double);
				gcvt(dou, 6, ptr); 
				while(*ptr) 
				{
					//________________________________________________
					*string = *ptr;
					string ++;
					ptr++;
					//________________________________________________
				}
			continue;*/

			case 's':
				ptr = (unsigned char*)va_arg(ap,char *);
				while(*ptr) 
				{
					//________________________________________________
					*string = *ptr;
					string ++;
					ptr++;
					//________________________________________________
				}
			continue;

			case 'o':
				base = 8;
				//________________________________________________
				*string = '0';
				string ++;
				//________________________________________________
			goto CONVERSION_LOOP;

			case 'd':
			issigned=1;
			// no break -> run into next case
			case 'u':


		//don't insert some case below this if USE_HEX is undefined !
		//or put       goto CONVERSION_LOOP;  before next case.

			goto CONVERSION_LOOP;
			case 'x':
				base = 16;


				CONVERSION_LOOP:

				if(issigned) //Signed types
				{
					if(islong) { s_val = va_arg(ap,long); }//
					else { s_val = va_arg(ap,int); }

					if(s_val < 0) //Value negativ ?
					{
						s_val = - s_val; //Make it positiv
						//________________________________________________
						*string = '-';
						string ++;
						//________________________________________________

					}
					else
					{
						if(markpluse)
						{
							//________________________________________________
							*string = '+';
							string ++;
							//________________________________________________
						}
					}
					u_val = (unsigned long)s_val;
				}
				else //Unsigned types
				{
					if(markpluse)
					{
						//________________________________________________
						*string = '+';
						string ++;
						//________________________________________________
					}
					if(islong) { u_val = va_arg(ap,unsigned long); }
					else { u_val = va_arg(ap,unsigned int); }
				}

				ptr = scratch + TEXT_SCRATCH;
				*--ptr = 0;

				
				do
				{
					char ch = u_val % base + '0';
					if (ch > '9')//USE_HEX
					{
					ch += 'a' - '9' - 1;

					ch-=0x20;

					}
		         
					*--ptr = ch;
					u_val /= base;

					if(width) width--; //calculate number of padding chars

				} while (u_val);

				while(width--) *--ptr = fill; //insert padding chars		      

				while(*ptr) 
				{
					//________________________________________________
					*string = *ptr;
					string ++;
					ptr++;
					//________________________________________________
				}
		}
	}
	*string = 0x00;
	string = &text_buf[0];
	return string;
}






void MainSYSTEM(void)
{		
	RefreshFlag=0;
	if(naviMENU!=old_naveMENU)
	{
		 RefreshFlag=1;
	}

	Check_event();

	switch(naviMENU){
case  1			 	:				SYS_1					();     		break;
case  10			:				SYS_10				();     		break;
case  11			:				SYS_11				();     		break;

case  2			 	:				SYS_2					();     		break;
case  20			:				SYS_20				();     			break;
case  200		 	:				SYS_200				();     		break;
case  201		 	:				SYS_201				();     	break;
case  2000		 	:				SYS_2000				();     		break;
case  2010		 	:				SYS_2010				();     	break;

case  21			:				SYS_21				();     		break;
case  210		 	:				SYS_210				();     		break;
case  211		 	:				SYS_211				();     	break;
case  2100		 	:				SYS_2100				();     		break;
case  2110		 	:				SYS_2110				();     	break;

case  22			:				SYS_22				();     		break;
case  220		 	:				SYS_220				();     		break;
case  221		 	:				SYS_221				();     	break;
case  2200		 	:				SYS_2200				();     		break;
case  2210		 	:				SYS_2210				();     	break;

case  23			:				SYS_23				();     		break;
case  230		 	:				SYS_230				();     		break;
case  231		 	:				SYS_231				();     	break;
case  2300		:				SYS_2300			();     	break;
case  2310		:				SYS_2310			();     		break;
case  24			:				SYS_24				();     			break;
case  240		 	:				SYS_240				();     		break;
case  241		 	:				SYS_241				();     	break;
case  2400		:				SYS_2400			();     	break;
case  2410		:				SYS_2410			();     		break;

case  4			 	:				SYS_4					();     			break;
case  40			:				SYS_40				();     		break;
case  400		 	:				SYS_400				();     		break;
case  401		 	:				SYS_401				();     	break;
case  4000		:				SYS_4000			();     	break;

case  41			:				SYS_41				();     			break;
case  410		 	:				SYS_410				();     		break;
case  4100		 	:				SYS_4100				();     		break;
case  4101		 	:				SYS_4101				();     		break;
case  41000		 	:				SYS_41000				();     		break;

case  411		 	:				SYS_411				();     		break;
case  4110		 	:				SYS_4110				();     		break;
case  41100		 	:				SYS_41100				();     		break;

case  42			:				SYS_42				();     		break;
case  420		 	:				SYS_420				();     		break;
case  4200		:				SYS_4200			();     		break;
case  42000	 	:				SYS_42000			();     break;
case  421		 	:				SYS_421				();     		break;
case  4210		:				SYS_4210			();     		break;
case  4211		:				SYS_4211			();     	break;
case  42100	 	:				SYS_42100			();     break;
case  422		 	:				SYS_422				();     		break;   
case  4220		:				SYS_4220			();     		break; 
case  42200		:				SYS_42200			();     	break;  
case  423		 	:				SYS_423				();     	break;     
case  4230		:				SYS_4230			();     	break;   
case  42300		:				SYS_42300			();     	break;  
case  424		 	:				SYS_424				();     		break;   
case  4240		:				SYS_4240			();     	break;     
case  42400		:				SYS_42400			();     	break;  

case  43			:				SYS_43				();     			break;
case  430		 	:				SYS_430				();     			break;
case  431		 	:				SYS_431				();     			break;
case  4300		:				SYS_4300			();     	break;

case  44			:				SYS_44				();     			break;
case  441		 	:				SYS_441				();     		break;
case  4410		:				SYS_4410			();     	break;
case  440		 	:				SYS_440				();     		break;
case  4400		:				SYS_4400			();     		break;
case  44000	 	:				SYS_44000			();     break;
case  44001	 	:				SYS_44001			();     break;
case  440000	:				SYS_440000		();     	break;
case  440010	:				SYS_440000		();     	break;

case  4401		:				SYS_4401			();     	break;
case  44010	 	:				SYS_44010			();     break;
case  44011	 	:				SYS_44011			();     	break;
case  440100	:				SYS_440100		();     	break;
case  440110	:				SYS_440100		();     	break;

case  4402		:				SYS_4402			();     	break;
case  44020	 	:				SYS_44020			();     break;
case  44021	 	:				SYS_44021			();     	break;
case  440200	:				SYS_440200		();     	break;
case  440210	:				SYS_440200		();     	break;

case  4403		:				SYS_4403			();     	break;
case  44030	 	:				SYS_44030			();     break;
case  44031	 	:				SYS_44031			();     	break;
case  440300	:				SYS_440300		();     	break;
case  440310	:				SYS_440300		();     	break;

case  4404		:				SYS_4404			();     	break;
case  44040	 	:				SYS_44040			();     break;
case  44041	 	:				SYS_44041			();     	break;
case  440400	:				SYS_440400		();     	break;
case  440410	:				SYS_440400		();     	break;

case  4405		:				SYS_4405			();     	break;
case  44050	 	:				SYS_44050			();     break;
case  44051	 	:				SYS_44051			();     	break;
case  440500	:				SYS_440500		();     	break;
case  440510	:				SYS_440500		();     	break;

case  4406		:				SYS_4406			();     	break;
case  44060	 	:				SYS_44060			();     break;
case  44061	 	:				SYS_44061			();     	break;
case  440600	:				SYS_440600		();     	break;
case  440610	:				SYS_440600		();     	break;

case  4407		:				SYS_4407			();     	break;
case  44070	 	:				SYS_44070			();     break;
case  44071	 	:				SYS_44071			();     	break;
case  440700	:				SYS_440700		();     	break;
case  440710	:				SYS_440700		();     	break;



case  45			:				SYS_45				();     		break;
case  450		 	:				SYS_450				();     	break;
case  451		 	:				SYS_451				();     		break;
case  4500		 	:				SYS_4500				();     	break;
case  4510		 	:				SYS_4510				();     		break;
case  45000		 	:				SYS_45000				();     	break;
case  45100		 	:				SYS_45100				();     		break;


case  3			 	:				SYS_3					();     			break;
case  30			:				SYS_30				();     		break;
case 0xFFFFFFFF :			SYS_0xFFFFFFFF();				break;


			default:		irregularPAGE_handler();		break;
				}

}    


void MENU(void)
{
	if(RefreshFlag)
	{
		switch(naviMENU){
case  1			 	:				PAGE_1					();     		break;   
case  10			:				PAGE_10				();     		break;   
case  11			:				PAGE_11				();     		break;

case  2			 	:				PAGE_2					();     		break;   
case  20			:				PAGE_20				();     			break; 
case  200		 	:				PAGE_200				();     		break;   
case  201		 	:				PAGE_201				();     	break;   
case  2000		 	:				PAGE_2000				();     		break;   
case  2010		 	:				PAGE_2010				();     	break;   
case  21			:				PAGE_21				();     		break;   
case  210		 	:				PAGE_210				();     		break;   
case  211		 	:				PAGE_211				();     	break;   
case  2100		 	:				PAGE_2100				();     		break;   
case  2110		 	:				PAGE_2110				();     	break;   
case  22			:				PAGE_22				();     		break;   
case  220		 	:				PAGE_220				();     		break;   
case  221		 	:				PAGE_221				();     	break;     
case  2200		 	:				PAGE_2200				();     		break;   
case  2210		 	:				PAGE_2210				();     	break;   
case  23			:				PAGE_23				();     		break;   
case  230		 	:				PAGE_230				();     		break;   
case  231		 	:				PAGE_231				();     	break;    
case  2300		:				PAGE_2300			();     	break;     
case  2310		:				PAGE_2310			();     		break;   
case  24			:				PAGE_24				();     			break; 
case  240		 	:				PAGE_240				();     		break;   
case  241		 	:				PAGE_241				();     	break;     
case  2400		:				PAGE_2400			();     	break;     
case  2410		:				PAGE_2410			();     		break;  

case  4			 	:				PAGE_4					();     			break; 
case  40			:				PAGE_40				();     		break;   
case  400		 	:				PAGE_400				();     		break;   
case  401		 	:				PAGE_401				();     	break;     
case  4000		:				PAGE_4000			();     	break;     

case  41			:				PAGE_41				();     			break; 
case  410		 	:				PAGE_410				();     		break;
case  4100		 	:				PAGE_4100				();     		break;
case  4101		 	:				PAGE_4101				();     		break;
case  41000		 	:				PAGE_41000				();     		break;

case  411		 	:				PAGE_411				();     		break;
case  4110		 	:				PAGE_4110				();     		break;
case  41100		 	:				PAGE_41100				();     		break;

case  42			:				PAGE_42				();     		break;   
case  420		 	:				PAGE_420				();     		break;   
case  4200		:				PAGE_4200			();     		break;   
case  42000	 	:				PAGE_42000			();     break;       
case  421		 	:				PAGE_421				();     		break;   
case  4210		:				PAGE_4210			();     		break;   
case  4211		:				PAGE_4211			();     	break;     
case  42100	 	:				PAGE_42100			();     break;       
case  422		 	:				PAGE_422				();     		break;   
case  4220		:				PAGE_4220			();     		break; 
case  42200		:				PAGE_42200			();     	break;  
case  423		 	:				PAGE_423				();     	break;     
case  4230		:				PAGE_4230			();     	break;   
case  42300		:				PAGE_42300			();     	break;  
case  424		 	:				PAGE_424				();     		break;   
case  4240		:				PAGE_4240			();     	break;     
case  42400		:				PAGE_42400			();     	break;  
case  43			:				PAGE_43				();     			break; 
case  430		 	:				PAGE_430				();     			break; 
case  431		 	:				PAGE_431				();     			break; 
case  4300		:				PAGE_4300			();     	break;     

case  44			:				PAGE_44				();     			break; 
case  441		 	:				PAGE_441				();     		break;   
case  4410		:				PAGE_4410			();     	break;     
case  440		 	:				PAGE_440				();     		break;   
case  4400		:				PAGE_4400			();     		break;   
case  44000	 	:				PAGE_44000			();     break;       
case  44001	 	:				PAGE_44001			();     break;       
case  440000	:				PAGE_440000		();     	break;   
case  440010	:				PAGE_440000		();     	break;   

case  4401		:				PAGE_4401			();     	break;     
case  44010	 	:				PAGE_44010			();     break;       
case  44011	 	:				PAGE_44011			();     	break;     
case  440100	:				PAGE_440100		();     	break;    
case  440110	:				PAGE_440100		();     	break;   

case  4402		:				PAGE_4402			();     	break;     
case  44020	 	:				PAGE_44020			();     break;       
case  44021	 	:				PAGE_44021			();     	break;     
case  440200	:				PAGE_440200		();     	break;     
case  440210	:				PAGE_440200		();     	break;    

case  4403		:				PAGE_4403			();     	break;     
case  44030	 	:				PAGE_44030			();     break;       
case  44031	 	:				PAGE_44031			();     	break;     
case  440300	:				PAGE_440300		();     	break;     
case  440310	:				PAGE_440300		();     	break;    
     
case  4404		:				PAGE_4404			();     	break;     
case  44040	 	:				PAGE_44040			();     break;       
case  44041	 	:				PAGE_44041			();     	break;     
case  440400	:				PAGE_440400		();     	break;   
case  440410	:				PAGE_440400		();     	break;  

case  4405		:				PAGE_4405			();     	break;     
case  44050	 	:				PAGE_44050			();     break;       
case  44051	 	:				PAGE_44051			();     	break;     
case  440500	:				PAGE_440500		();     	break;     
case  440510	:				PAGE_440500		();     	break;  

case  4406		:				PAGE_4406			();     	break;     
case  44060	 	:				PAGE_44060			();     break;       
case  44061	 	:				PAGE_44061			();     	break;     
case  440600	:				PAGE_440600		();     	break;  
case  440610	:				PAGE_440600		();     	break;  
     
case  4407		:				PAGE_4407			();     	break;     
case  44070	 	:				PAGE_44070			();     break;       
case  44071	 	:				PAGE_44071			();     	break;     
case  440700	:				PAGE_440700		();     	break;     
case  440710	:				PAGE_440700		();     	break; 

case  45			:				PAGE_45				();     		break;   
case  450		 	:				PAGE_450				();     	break;     
case  451		 	:				PAGE_451				();     		break;   
case  4500		 	:				PAGE_4500				();     	break;     
case  4510		 	:				PAGE_4510				();     		break;   
case  45000		 	:				PAGE_45000				();     	break;     
case  45100		 	:				PAGE_45100				();     		break;   

case  3			 	:				PAGE_3					();     			break; 
case  30			:				PAGE_30				();     		break;   


case 0xFFFFFFFF	:			PAGE_0xFFFFFFFF();break;


		default:		break;
			}

		//debug_printf("\x1B[2J");
		//debug_printf("현재메뉴 : %d", naviMENU);
		//GLCD_print0508(7, 7,(char*)_TEXT("-Page%lu-",naviMENU));
		
		old_naveMENU=naviMENU;
		LCDUpdateFlag=1;
	}

	if(communication_fault)
	{
		GLCD_BufClear();	
		GLCD_print0508(5, 0, "--MODUS-1000 UPS--");
		GLCD_print1616(24, 3,"통신 끊김 ");
	}
	GLCD_print0508(7, 7,_TEXT("20%02d/%02d/%02d %02d:%02d:%02d",year,month,date,hour,min,second));
}

