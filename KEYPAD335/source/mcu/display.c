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
#include "util.h"
#include "display.h"
//#include "128by64GLCD.H"
#include "16by2clcd.h"
#include "rs232_MMItoPANEL.h"

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
//#include "ds1307.h"

#include <avr/pgmspace.h>
#include <avr/eeprom.h>

#include "debug_printf.h"



unsigned long naviMENU,old_naveMENU; 
unsigned char RefreshFlag = 0;

volatile unsigned int main_data_array[MX_DATA_ARRAY];
volatile unsigned int main_data_array_old[MX_DATA_ARRAY];

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





void DisplayInit(void)
{
	naviMENU=1;
	old_naveMENU = 0xFF;
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


void PAGE_1(void)
{
	CLCD_string(0x80,"Main Menu Page  ");
	CLCD_string(0xC0,"M0 Operation    ");
}

void PAGE_1_0(void)
{
	CLCD_string(0x80,"[0]Local/Remote ");
	CLCD_string(0xC0,"     [LOCAL]    ");
}

void PAGE_1_0_0(void)
{
	CLCD_string(0x80,"[0]Local/Remote ");
	CLCD_string(0xC0,"    [REMOTE]    ");
}

void PAGE_1_1(void)
{
	CLCD_string(0x80,"[1]Dir. Charge  ");
	CLCD_string(0xC0,"    [FORWARD]   ");
}

void PAGE_1_1_0(void)
{
	CLCD_string(0x80,"[1]Dir. Charge  ");
	CLCD_string(0xC0,"    [REVERSE]   ");
}

void PAGE_1_2(void)
{
	CLCD_string(0x80,"[2]Speed Ref    ");
	CLCD_string(0xC0,"             rpm");
}


void PAGE_1_3(void)
{
	CLCD_string(0x80,"[3]Frequency Ref");
	CLCD_string(0xC0,"              Hz");
}


void PAGE_1_4(void)
{
	CLCD_string(0x80,"[4]Torque Ref   ");
	CLCD_string(0xC0,"              Nm");
}


void PAGE_1_5(void)
{
	CLCD_string(0x80,"[5]PID Ref      ");
	CLCD_string(0xC0,"               %");
}




void PAGE_2(void)
{
	CLCD_string(0x80,"Main Menu Page  ");
	CLCD_string(0xC0,"M1 Drive Monitor");
}


void PAGE_2_0(void)
{
	CLCD_string(0x80,"M1 Drive Monitor");
	CLCD_string(0xC0,"[0]OP Status    ");
}

void PAGE_2_0_00(void)
{
	CLCD_string(0x80,"[0]MOTOR Speed  ");
	CLCD_string(0xC0,"             rpm");
}
void PAGE_2_0_01(void)
{
	CLCD_string(0x80,"[1]Output Freq  ");
	CLCD_string(0xC0,"              Hz");
}
void PAGE_2_0_02(void)
{
	CLCD_string(0x80,"[2]DC Link      ");
	CLCD_string(0xC0,"             Vdc");
}
void PAGE_2_0_03(void)
{
	CLCD_string(0x80,"[3]Motor Current");
	CLCD_string(0xC0,"            Arms");
}
void PAGE_2_0_04(void)
{
	CLCD_string(0x80,"[4]Output Volt  ");
	CLCD_string(0xC0,"            Arms");
}
void PAGE_2_0_05(void)
{
	CLCD_string(0x80,"[5]Actual Torque");
	CLCD_string(0xC0,"              Nm");
}
void PAGE_2_0_06(void)
{
	CLCD_string(0x80,"[6]Torque Curr  ");
	CLCD_string(0xC0,"               A");
}
void PAGE_2_0_07(void)
{
	CLCD_string(0x80,"[7]Flux Current ");
	CLCD_string(0xC0,"               A");
}
void PAGE_2_0_08(void)
{
	CLCD_string(0x80,"[8]Input Power  ");
	CLCD_string(0xC0,"              kW");
}
void PAGE_2_0_09(void)
{
	CLCD_string(0x80,"[9]Output Power ");
	CLCD_string(0xC0,"              kW");
}
void PAGE_2_0_10(void)
{
	CLCD_string(0x80,"[10]PID Ref     ");
	CLCD_string(0xC0,"                ");
}
void PAGE_2_0_11(void)
{
	CLCD_string(0x80,"[11]PID Feedback");
	CLCD_string(0xC0,"                ");
}
void PAGE_2_0_12(void)
{
	CLCD_string(0x80,"[12]PID Error   ");
	CLCD_string(0xC0,"                ");
}
void PAGE_2_0_13(void)
{
	CLCD_string(0x80,"[13]Temperaure  ");
	CLCD_string(0xC0,"              'C");
}

void PAGE_2_1(void)
{
	CLCD_string(0x80,"M1 Drive Monitor");
	CLCD_string(0xC0,"[1]Terminal I/O ");
}
void PAGE_2_1_00(void)
{
	CLCD_string(0x80,"[0]DI[8......1] ");
	CLCD_string(0xC0,"                ");
}
void PAGE_2_1_01(void)
{
	CLCD_string(0x80,"[1]DO[3.1]      ");
	CLCD_string(0xC0,"                ");
}
void PAGE_2_1_02(void)
{
	CLCD_string(0x80,"[2]Analog Inout1");
	CLCD_string(0xC0,"         V or mA");
}
void PAGE_2_1_03(void)
{
	CLCD_string(0x80,"[3]Analog Inout2");
	CLCD_string(0xC0,"         V or mA");
}
void PAGE_2_1_04(void)
{
	CLCD_string(0x80,"[4]Analog Output");
	CLCD_string(0xC0,"              mA");
}


void PAGE_2_2(void)
{
	CLCD_string(0x80,"M1 Drive Monitor");
	CLCD_string(0xC0,"[2]Drive Info   ");
}
void PAGE_2_2_00(void)
{
	CLCD_string(0x80,"[0]MOTOR Sel    ");
	CLCD_string(0xC0,"                ");
}
void PAGE_2_2_01(void)
{
	CLCD_string(0x80,"[1]Ctrl Method  ");
	CLCD_string(0xC0,"                ");
}
void PAGE_2_2_02(void)
{
	CLCD_string(0x80,"[2]RUN/STOP Src ");
	CLCD_string(0xC0,"                ");
}
void PAGE_2_2_03(void)
{
	CLCD_string(0x80,"[3]Ref Method   ");
	CLCD_string(0xC0,"                ");
}
void PAGE_2_2_04(void)
{
	CLCD_string(0x80,"[4]Drive Power  ");
	CLCD_string(0xC0,"              kW");
}
void PAGE_2_2_05(void)
{
	CLCD_string(0x80,"[5]Drive Voltage");
	CLCD_string(0xC0,"               V");
}
void PAGE_2_2_06(void)
{
	CLCD_string(0x80,"[6]Option Card  ");
	CLCD_string(0xC0,"                ");
}
void PAGE_2_2_07(void)
{
	CLCD_string(0x80,"[7]Software Ver.");
	CLCD_string(0xC0,"                ");
}
void PAGE_2_2_08(void)
{
	CLCD_string(0x80,"[8]Software Opt ");
	CLCD_string(0xC0,"                ");
}





void PAGE_3(void)
{
	CLCD_string(0x80,"Main Menu Page  ");
	CLCD_string(0xC0,"M2 Parameter Edt");
}


void PAGE_4(void)
{
	CLCD_string(0x80,"Main Menu Page  ");
	CLCD_string(0xC0,"M3 Auto Tuning  ");
}

void PAGE_5(void)
{
	CLCD_string(0x80,"Main Menu Page  ");
	CLCD_string(0xC0,"M4 Fault Record ");
}

void PAGE_6(void)
{
	CLCD_string(0x80,"Main Menu Page  ");
	CLCD_string(0xC0,"M5 Initialize   ");
}

void PAGE_7(void)
{
	CLCD_string(0x80,"Main Menu Page  ");
	CLCD_string(0xC0,"M6 Password     ");
}





void PAGE_0xFFFFFFFF(void)
{
	//GLCD_BufClear();	
	//GLCD_print0508(2, 1,"for The GOD");
	//GLCD_print0508(2, 2,"for The JESUS");
	//GLCD_print0508(2, 3,"for The PEOPLE");

	CLCD_string(0x80,"        Designed");
	CLCD_string(0xC0," by Paul BH Park");

}





int Temporary=0;
void SYS_1(void)
{
	static unsigned char flag=0;

	//if(EventFlagC)
	//{

		
		//if(KeyState.KeyValue == ESC)naviMENU = naviMENU;
		if(KeyState.KeyValue == UP)naviMENU = 7;
		else if(KeyState.KeyValue == DN)naviMENU = 2;
		else if(KeyState.KeyValue == ENTER)naviMENU = 10;
		else if(KeyState.KeyValue == (ESC  & RUN & STOP & ENTER) )	naviMENU = 0xFFFFFFFF;
		
	//	if(KeyState.KeyValue != 0xFF)EventTimeC=0;
	//}
	//else 
	//{
	//	if(KeyState.KeyValue != 0xFF)EventFlagC=1;
	//	if(naviMENU!=old_naveMENU)EventFlagC=1;
	//}

	//if((!EventFlagC)&&(flag))RefreshFlag=1;// event edge
	//else if((EventFlagC)&&(!flag))RefreshFlag=1;// event edge
	//flag = EventFlagC;
	
}


void SYS_1_0(void)
{
	if(KeyState.KeyValue == ENTER)naviMENU = 100;
	else if(KeyState.KeyValue == ESC)naviMENU = 1;
	else if(KeyState.KeyValue == UP)naviMENU = 15;
	else if(KeyState.KeyValue == DN)naviMENU = 11;
}
void SYS_1_0_0(void)
{
	if(KeyState.KeyValue == ENTER)naviMENU = 10;
	else if(KeyState.KeyValue == ESC)naviMENU = 1;
	else if(KeyState.KeyValue == UP)naviMENU = 15;
	else if(KeyState.KeyValue == DN)naviMENU = 11;
}
void SYS_1_1(void)
{
	if(KeyState.KeyValue == ENTER)naviMENU = 200;
	else if(KeyState.KeyValue == ESC)naviMENU = 1;
	else if(KeyState.KeyValue == UP)naviMENU = 10;
	else if(KeyState.KeyValue == DN)naviMENU = 12;
}
void SYS_1_1_0(void)
{
	if(KeyState.KeyValue == ENTER)naviMENU = 20;
	else if(KeyState.KeyValue == ESC)naviMENU = 1;
	else if(KeyState.KeyValue == UP)naviMENU = 10;
	else if(KeyState.KeyValue == DN)naviMENU = 12;
}
void SYS_1_2(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 1;
	else if(KeyState.KeyValue == UP)naviMENU = 11;
	else if(KeyState.KeyValue == DN)naviMENU = 13;
}
void SYS_1_3(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 1;
	else if(KeyState.KeyValue == UP)naviMENU = 12;
	else if(KeyState.KeyValue == DN)naviMENU = 14;
}
void SYS_1_4(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 1;
	else if(KeyState.KeyValue == UP)naviMENU = 13;
	else if(KeyState.KeyValue == DN)naviMENU = 15;
}

void SYS_1_5(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 1;
	else if(KeyState.KeyValue == UP)naviMENU = 14;
	else if(KeyState.KeyValue == DN)naviMENU = 10;
}



void SYS_2(void)
{
	if(KeyState.KeyValue == UP)naviMENU = 1;
	else if(KeyState.KeyValue == DN)naviMENU = 3;
	else if(KeyState.KeyValue == ENTER)naviMENU = 20;
	else if(KeyState.KeyValue == (ESC  & RUN & STOP & ENTER) )	naviMENU = 0xFFFFFFFF;
}

void SYS_2_0(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 2;
	else if(KeyState.KeyValue == ENTER)naviMENU = 2000;
	else if(KeyState.KeyValue == UP)naviMENU = 22;
	else if(KeyState.KeyValue == DN)naviMENU = 21;
}

void SYS_2_0_00(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 20;
	else if(KeyState.KeyValue == UP)naviMENU = 2013;
	else if(KeyState.KeyValue == DN)naviMENU = 2001;
}
void SYS_2_0_01(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 20;
	else if(KeyState.KeyValue == UP)naviMENU = 2000;
	else if(KeyState.KeyValue == DN)naviMENU = 2002;
}
void SYS_2_0_02(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 20;
	else if(KeyState.KeyValue == UP)naviMENU = 2001;
	else if(KeyState.KeyValue == DN)naviMENU = 2003;
}
void SYS_2_0_03(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 20;
	else if(KeyState.KeyValue == UP)naviMENU = 2002;
	else if(KeyState.KeyValue == DN)naviMENU = 2004;
}
void SYS_2_0_04(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 20;
	else if(KeyState.KeyValue == UP)naviMENU = 2003;
	else if(KeyState.KeyValue == DN)naviMENU = 2005;
}
void SYS_2_0_05(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 20;
	else if(KeyState.KeyValue == UP)naviMENU = 2004;
	else if(KeyState.KeyValue == DN)naviMENU = 2006;
}
void SYS_2_0_06(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 20;
	else if(KeyState.KeyValue == UP)naviMENU = 2005;
	else if(KeyState.KeyValue == DN)naviMENU = 2007;
}
void SYS_2_0_07(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 20;
	else if(KeyState.KeyValue == UP)naviMENU = 2006;
	else if(KeyState.KeyValue == DN)naviMENU = 2008;
}
void SYS_2_0_08(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 20;
	else if(KeyState.KeyValue == UP)naviMENU = 2007;
	else if(KeyState.KeyValue == DN)naviMENU = 2009;
}
void SYS_2_0_09(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 20;
	else if(KeyState.KeyValue == UP)naviMENU = 2008;
	else if(KeyState.KeyValue == DN)naviMENU = 2010;
}
void SYS_2_0_10(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 20;
	else if(KeyState.KeyValue == UP)naviMENU = 2009;
	else if(KeyState.KeyValue == DN)naviMENU = 2011;
}
void SYS_2_0_11(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 20;
	else if(KeyState.KeyValue == UP)naviMENU = 2010;
	else if(KeyState.KeyValue == DN)naviMENU = 2012;
}
void SYS_2_0_12(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 20;
	else if(KeyState.KeyValue == UP)naviMENU = 2011;
	else if(KeyState.KeyValue == DN)naviMENU = 2013;
}
void SYS_2_0_13(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 20;
	else if(KeyState.KeyValue == UP)naviMENU = 2012;
	else if(KeyState.KeyValue == DN)naviMENU = 2000;
}





void SYS_2_1(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 2;
	else if(KeyState.KeyValue == ENTER)naviMENU = 2100;
	else if(KeyState.KeyValue == UP)naviMENU = 20;
	else if(KeyState.KeyValue == DN)naviMENU = 22;
}

void SYS_2_1_00(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 21;
	else if(KeyState.KeyValue == UP)naviMENU = 2104;
	else if(KeyState.KeyValue == DN)naviMENU = 2101;
}
void SYS_2_1_01(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 21;
	else if(KeyState.KeyValue == UP)naviMENU = 2100;
	else if(KeyState.KeyValue == DN)naviMENU = 2102;
}
void SYS_2_1_02(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 21;
	else if(KeyState.KeyValue == UP)naviMENU = 2101;
	else if(KeyState.KeyValue == DN)naviMENU = 2103;
}
void SYS_2_1_03(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 21;
	else if(KeyState.KeyValue == UP)naviMENU = 2102;
	else if(KeyState.KeyValue == DN)naviMENU = 2104;
}
void SYS_2_1_04(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 21;
	else if(KeyState.KeyValue == UP)naviMENU = 2103;
	else if(KeyState.KeyValue == DN)naviMENU = 2100;
}




void SYS_2_2(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 2;
	else if(KeyState.KeyValue == ENTER)naviMENU = 2200;
	else if(KeyState.KeyValue == UP)naviMENU = 21;
	else if(KeyState.KeyValue == DN)naviMENU = 20;
}
void SYS_2_2_00(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 22;
	else if(KeyState.KeyValue == UP)naviMENU = 2208;
	else if(KeyState.KeyValue == DN)naviMENU = 2201;
}
void SYS_2_2_01(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 22;
	else if(KeyState.KeyValue == UP)naviMENU = 2200;
	else if(KeyState.KeyValue == DN)naviMENU = 2202;
}
void SYS_2_2_02(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 22;
	else if(KeyState.KeyValue == UP)naviMENU = 2201;
	else if(KeyState.KeyValue == DN)naviMENU = 2203;
}
void SYS_2_2_03(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 22;
	else if(KeyState.KeyValue == UP)naviMENU = 2202;
	else if(KeyState.KeyValue == DN)naviMENU = 2204;
}
void SYS_2_2_04(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 22;
	else if(KeyState.KeyValue == UP)naviMENU = 2203;
	else if(KeyState.KeyValue == DN)naviMENU = 2205;
}
void SYS_2_2_05(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 22;
	else if(KeyState.KeyValue == UP)naviMENU = 2204;
	else if(KeyState.KeyValue == DN)naviMENU = 2206;
}
void SYS_2_2_06(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 22;
	else if(KeyState.KeyValue == UP)naviMENU = 2205;
	else if(KeyState.KeyValue == DN)naviMENU = 2207;
}
void SYS_2_2_07(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 22;
	else if(KeyState.KeyValue == UP)naviMENU = 2206;
	else if(KeyState.KeyValue == DN)naviMENU = 2208;
}
void SYS_2_2_08(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 22;
	else if(KeyState.KeyValue == UP)naviMENU = 2207;
	else if(KeyState.KeyValue == DN)naviMENU = 2200;
}


void SYS_3(void)
{
	if(KeyState.KeyValue == UP)naviMENU = 2;
	else if(KeyState.KeyValue == DN)naviMENU = 4;
	else if(KeyState.KeyValue == ENTER)naviMENU = 30;
	else if(KeyState.KeyValue == (ESC  & RUN & STOP & ENTER) )	naviMENU = 0xFFFFFFFF;

}

void SYS_4(void)
{
	if(KeyState.KeyValue == UP)naviMENU = 3;
	else if(KeyState.KeyValue == DN)naviMENU = 5;
	else if(KeyState.KeyValue == ENTER)naviMENU = 40;
	else if(KeyState.KeyValue == (ESC  & RUN & STOP & ENTER) )	naviMENU = 0xFFFFFFFF;

}

void SYS_5(void)
{
	if(KeyState.KeyValue == UP)naviMENU = 4;
	else if(KeyState.KeyValue == DN)naviMENU = 6;
	else if(KeyState.KeyValue == ENTER)naviMENU = 50;
	else if(KeyState.KeyValue == (ESC  & RUN & STOP & ENTER) )	naviMENU = 0xFFFFFFFF;

}

void SYS_6(void)
{
	if(KeyState.KeyValue == UP)naviMENU = 5;
	else if(KeyState.KeyValue == DN)naviMENU = 7;
	else if(KeyState.KeyValue == ENTER)naviMENU = 60;
	else if(KeyState.KeyValue == (ESC  & RUN & STOP & ENTER) )	naviMENU = 0xFFFFFFFF;

}

void SYS_7(void)
{
	if(KeyState.KeyValue == UP)naviMENU = 6;
	else if(KeyState.KeyValue == DN)naviMENU = 1;
	else if(KeyState.KeyValue == ENTER)naviMENU = 70;
	else if(KeyState.KeyValue == (ESC  & RUN & STOP & ENTER) )	naviMENU = 0xFFFFFFFF;

}



void SYS_0xFFFFFFFF(void)
{
	if(KeyState.KeyValue ==  (ESC  & RUN & STOP & ENTER) )
	{
		naviMENU = 1;
	}
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
	static unsigned char _EventFlagE=0;

	RefreshFlag=0;
	if(naviMENU!=old_naveMENU)
	{
		 RefreshFlag=1;
	}

	if(KeyState.KeyValue != 0xFFFF)
	{
		EventFlagE = 1;
		EventTimeE = 0;
	}
	else
	{
		if(!_EventFlagE && EventFlagE)
		{
			CLCD_BackLightOnOff(1);
		}
		else if(_EventFlagE && !EventFlagE)
		{
			CLCD_BackLightOnOff(0);
		}
		_EventFlagE=EventFlagE;
					
	}

	switch(naviMENU){
		case  1			 	:				SYS_1						();     		break;

		case  10			 	:				SYS_1_0					();     		break;
		case  100		 	:				SYS_1_0_0					();     		break;
		case  11			 	:				SYS_1_1					();     		break;
		case  110		 	:				SYS_1_1_0					();     		break;
		case  12			 	:				SYS_1_2					();     		break;
		case  13			 	:				SYS_1_3					();     		break;
		case  14			 	:				SYS_1_4					();     		break;
		case  15			 	:				SYS_1_5					();     		break;
		
		case  2			 	:				SYS_2						();     		break;

		case  20			 	:				SYS_2_0					();	     		break;
		case  2000		 	:				SYS_2_0_00					();	     		break;
		case  2001		 	:				SYS_2_0_01					();	     		break;
		case  2002		 	:				SYS_2_0_02					();	     		break;
		case  2003		 	:				SYS_2_0_03					();	     		break;
		case  2004		 	:				SYS_2_0_04					();	     		break;
		case  2005		 	:				SYS_2_0_05					();	     		break;
		case  2006		 	:				SYS_2_0_06					();	     		break;
		case  2007		 	:				SYS_2_0_07					();	     		break;
		case  2008		 	:				SYS_2_0_08					();	     		break;
		case  2009		 	:				SYS_2_0_09					();	     		break;
		case  2010		 	:				SYS_2_0_10					();	     		break;
		case  2011		 	:				SYS_2_0_11					();	     		break;
		case  2012		 	:				SYS_2_0_12					();	     		break;
		case  2013		 	:				SYS_2_0_13					();	     		break;

		case  21			 	:				SYS_2_1					();     		break;
		case  2100		 	:				SYS_2_1_00					();	     		break;
		case  2101		 	:				SYS_2_1_01					();	     		break;
		case  2102		 	:				SYS_2_1_02					();	     		break;
		case  2103		 	:				SYS_2_1_03					();	     		break;
		case  2104		 	:				SYS_2_1_04					();	     		break;

		case  22			 	:				SYS_2_2					();     		break;
		case  2200		 	:				SYS_2_2_00					();	     		break;
		case  2201		 	:				SYS_2_2_01					();	     		break;
		case  2202		 	:				SYS_2_2_02					();	     		break;
		case  2203		 	:				SYS_2_2_03					();	     		break;
		case  2204		 	:				SYS_2_2_04					();	     		break;
		case  2205		 	:				SYS_2_2_05					();	     		break;
		case  2206		 	:				SYS_2_2_06					();	     		break;
		case  2207		 	:				SYS_2_2_07					();	     		break;
		case  2208		 	:				SYS_2_2_08					();	     		break;
		
		case  3			 	:				SYS_3						();     		break;
		case  4			 	:				SYS_4						();     		break;
		case  5			 	:				SYS_5						();     		break;
		case  6			 	:				SYS_6						();     		break;
		case  7			 	:				SYS_7						();     		break;

		case 0xFFFFFFFF :			SYS_0xFFFFFFFF();				break;

		default:		irregularPAGE_handler();		break;
				}

}




void Menu(void)
{
	if(RefreshFlag)
	{
		switch(naviMENU){
			case  1			 	:				PAGE_1					();     		break;   

			case  10			 	:				PAGE_1_0						();     		break;
			case  100		 	:				PAGE_1_0_0					();     		break;
			case  11			 	:				PAGE_1_1						();     		break;
			case  110		 	:				PAGE_1_1_0					();     		break;
			case  12			 	:				PAGE_1_2						();     		break;
			case  13			 	:				PAGE_1_3						();     		break;
			case  14			 	:				PAGE_1_4						();     		break;
			case  15			 	:				PAGE_1_5						();     		break;
						
			case  2			 	:				PAGE_2					();     		break;   

			case  20			 	:				PAGE_2_0					();	     		break;
			case  2000		 	:				PAGE_2_0_00					();	     		break;
			case  2001		 	:				PAGE_2_0_01					();	     		break;
			case  2002		 	:				PAGE_2_0_02					();	     		break;
			case  2003		 	:				PAGE_2_0_03					();	     		break;
			case  2004		 	:				PAGE_2_0_04					();	     		break;
			case  2005		 	:				PAGE_2_0_05					();	     		break;
			case  2006		 	:				PAGE_2_0_06					();	     		break;
			case  2007		 	:				PAGE_2_0_07					();	     		break;
			case  2008		 	:				PAGE_2_0_08					();	     		break;
			case  2009		 	:				PAGE_2_0_09					();	     		break;
			case  2010		 	:				PAGE_2_0_10					();	     		break;
			case  2011		 	:				PAGE_2_0_11					();	     		break;
			case  2012		 	:				PAGE_2_0_12					();	     		break;
			case  2013		 	:				PAGE_2_0_13					();	     		break;
			
			case  21			 	:				PAGE_2_1					();     		break;
			case  2100		 	:				PAGE_2_1_00					();	     		break;
			case  2101		 	:				PAGE_2_1_01					();	     		break;
			case  2102		 	:				PAGE_2_1_02					();	     		break;
			case  2103		 	:				PAGE_2_1_03					();	     		break;
			case  2104		 	:				PAGE_2_1_04					();	     		break;

			case  22			 	:				PAGE_2_2					();     		break;
			case  2200		 	:				PAGE_2_2_00					();	     		break;
			case  2201		 	:				PAGE_2_2_01					();	     		break;
			case  2202		 	:				PAGE_2_2_02					();	     		break;
			case  2203		 	:				PAGE_2_2_03					();	     		break;
			case  2204		 	:				PAGE_2_2_04					();	     		break;
			case  2205		 	:				PAGE_2_2_05					();	     		break;
			case  2206		 	:				PAGE_2_2_06					();	     		break;
			case  2207		 	:				PAGE_2_2_07					();	     		break;
			case  2208		 	:				PAGE_2_2_08					();	     		break;
			
			case  3			 	:				PAGE_3					();     		break;   
			case  4			 	:				PAGE_4					();     		break;   
			case  5			 	:				PAGE_5					();     		break;   
			case  6			 	:				PAGE_6					();     		break;   
			case  7			 	:				PAGE_7					();     		break;   
			
			case 0xFFFFFFFF	:			PAGE_0xFFFFFFFF();break;

			default:		break;
			}

		//debug_printf("\x1B[2J");
		//debug_printf("현재메뉴 : %d", naviMENU);
		//GLCD_print0508(7, 7,(char*)_TEXT("-Page%lu-",naviMENU));
		//if(KeyState.KeyValue != 0xFFFF)	CLCD_string(0xC0,_TEXT("0x%04x          ",KeyState.KeyValue));
		//CLCD_string(0xC0,(char*)_TEXT("         -Page%lu-",naviMENU));
		
		old_naveMENU=naviMENU;
		//LCDUpdateFlag=1;
	}

	if(communication_fault)
	{


	}

}

