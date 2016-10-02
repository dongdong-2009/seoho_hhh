//====================================================
//
// Copyright(C) 2008 HanYoung ELEC. Co., LTD.
// Designed by Paul B.H Park(idonno2000)
//
//
//	filename : main.c
//	description : π€úëµZœa
//	last revision : 2008-05-09 4:02îı“Å
//	history : 
//
//====================================================

#define F_CPU 16000000UL // ªÁøÎ«œ¥¬ CPU ≈¨∑∞¿ª º≥¡§
#define __OPTIMIZE__ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <avr/eeprom.h>



//#include "128by64GLCD.h"
#include "16by2clcd.h"
#include "key.h"

#include "rs232_MMItoPANEL.h"
//#include "ProtoMod.h"
#include "display.h"


//#include "ds1307.h"

#include "debug_printf.h"

char PacketBuf[100];
char PacketCMD = 0xFF;
char* Packet;


volatile unsigned char TimerTicFlag = 0;

unsigned char TimeTic_1ms = 0;
unsigned char TimeTic_10ms = 0;
unsigned char TimeTic_100ms = 0;
unsigned char TimeTic_200ms = 0;
unsigned char TimeTic_500ms = 0;
unsigned char TimeTic_1s = 0;

unsigned char _10ms_Cnt = 1;
unsigned char _100ms_Cnt = 1;
unsigned char _200ms_Cnt = 1;
unsigned char _500ms_Cnt = 1;
unsigned char _1s_Cnt = 1;

unsigned char Toggle10ms = 0;
unsigned char Toggle500ms = 0;

unsigned char EventFlagA = 0;
unsigned char EventFlagB = 0;
unsigned char EventFlagC = 0;
unsigned char EventFlagD = 0;
unsigned char EventFlagE= 0; //backlight event


unsigned char EventTimeA = 0;
unsigned char EventTimeB = 0;
unsigned char EventTimeC = 0;
unsigned char EventTimeD = 0;
unsigned char EventTimeE = 0;



void MCU_initialize(void)			/* initialize ATmege1280 MCU & OK-1280 kit */
{ 
	XMCRA = 0xC3;					// 0x2200-0x7FFF=3 wait, 0x8000-0xFFFF=0 wait
	XMCRB = 0x80;					// enable bus keeper, use PC0-PC7 as address

	DDRB = 0xFF;				
	PORTB = 0x00;

	DDRD = 0xF1;	
	PORTD = 0x00;

	DDRE = 0x00;					
	PORTE = 0x00;	
	DDRF = 0xFF;         		
	PORTF = 0x00;

	DDRH = 0xFF;				
	PORTH = 0x00;				
	
	DDRJ = 0x00;				
	PORTJ = 0x00;			
	DDRK = 0x00;          
	PORTK = 0x00;
	
	DDRL = 0xFF;					// PORTL7-0 = output
	PORTL = 0x00;					//    * clear
}

void Timer_IntteruptSet(void)
{
	//timer 1 ¿Œ≈Õ∑¥∆Æ
	TCCR1A = 0x10;
	TCCR1B = 0x0C;
	TCCR1C = 0x00;
	OCR1A = 62;
	TCNT1 = 0x0000;

	TIMSK1 = 0x02;
	TIFR1 = 0x2F;

}




void SystemTimeTic(void)
{
	TimeTic_1ms = 0;
	TimeTic_10ms = 0;
	TimeTic_100ms = 0;
	TimeTic_200ms = 0;
	TimeTic_500ms = 0;
	TimeTic_1s = 0;

	if(TimerTicFlag)TimeTic_1ms = 1;
	TimerTicFlag = 0;

	if(TimeTic_1ms)
	{
		_10ms_Cnt++;
		if(10<_10ms_Cnt)
		{
			_10ms_Cnt = 1;
			TimeTic_10ms = 1;
		}
	}
	
	if(TimeTic_10ms)
	{
		_100ms_Cnt++;
		if(10<_100ms_Cnt)
		{
			_100ms_Cnt = 1;
			TimeTic_100ms = 1;
		}

	}

	if(TimeTic_100ms)
	{
		_500ms_Cnt++;
		if(5<_500ms_Cnt)
		{
			_500ms_Cnt = 1;
			TimeTic_500ms = 1;
		}
		
		_200ms_Cnt++;
		if(2<_200ms_Cnt)
		{
			_200ms_Cnt = 1;
			TimeTic_200ms = 1;
		}
	
	}

	if(TimeTic_500ms)
	{
		_1s_Cnt++;
		if(2<_1s_Cnt)
		{
			_1s_Cnt = 1;
			TimeTic_1s = 1;
		}

	}
		
	

	if(TimeTic_10ms)
	{
		if(Toggle10ms)Toggle10ms = 0;
		else Toggle10ms = 1;
	}
	
	if(TimeTic_500ms)
	{
		if(Toggle500ms)Toggle500ms = 0x00;
		else Toggle500ms = 0xFF;
	}
}

void SystemEventAutoClear(void)
{
	if(TimeTic_1s)
	{
		if(EventFlagA)//using DpMenu5
		{
			EventTimeA++;
			if(1<EventTimeA)//1s
			{
				EventFlagA = 0;
				EventTimeA = 0;

			}
		}

		if(EventFlagB)
		{
			EventTimeB++;
			if(2<EventTimeB)//2s
			{
				EventFlagB = 0;
				EventTimeB = 0;
			}
		}

		if(EventFlagC)
		{
			EventTimeC++;
			if(60<EventTimeC)//60s
			{
				EventFlagC = 0;
				EventTimeC = 0;
			}
		}

		if(EventFlagD)
		{
			EventTimeD++;
			if(120<EventTimeD)//120s alarm
			{
				EventFlagD = 0;
				EventTimeD = 0;
			}
		}

		if(EventFlagE)
		{
			EventTimeE++;
			if(10<EventTimeE)//180s alarm
			{
				EventFlagE = 0;
				EventTimeE = 0;
			}
		}
	}
}


ISR(TIMER1_COMPA_vect)
{
	TimerTicFlag = 1;
}

int main(void)
{
	MCU_initialize();                             // initialize MCU and kit
	
	Delay_ms(200);                                 // wait for system stabilization

  CLCD_initialize();                             // initialize text LCD module

  		CLCD_string(0x80," Seoho Electric");          // display title
		CLCD_string(0xC0,"    AC Drive");

	CLCD_BackLightOnOff(1);

	//GLCD_BuzzerOnOff(1);Delay_ms(50); 
	//GLCD_BuzzerOnOff(0);Delay_ms(50); 
	//GLCD_BuzzerOnOff(1);Delay_ms(50); 
	//GLCD_BuzzerOnOff(0);

	//GLCD_clear();
	//GLCD_print0508(0,0,"");
	//GLCD_print0508(0,1,"   HanYoung MODUS-1000");	// display screen 1
	//GLCD_print0508(0,2,"     AVR_MMI VER1.0  ");
	//GLCD_print0508(0,3," ");
	//GLCD_print0508(0,4,"     Date. 2009/03/31");
	//GLCD_print0508(0,5,"");
	//GLCD_print0508(0,6,"      Programed by");
	//GLCD_print0508(0,7,"  Paul Byung-Hun Park");

	//GLCD_print1616(0, 1, "«—øµ¿¸±‚∞¯æ˜(¡÷)");
	//GLCD_print1616(0, 3, "   HMI-5000P");
	//GLCD_PutStringScrollBuf("LCD TE.STING NOW...");
	//LoadDiagram();
	//CD_print0508(0,1,_readPGM_TEXT(&system_event_inform[0][0]));
	//GLCD_FullUpdate();

	KeyInit();
	Timer_IntteruptSet();
	UART_init();
	DisplayInit();
//	ds1307_init();

//	debug_devopen(TX2_char);

//	RS485_RX_EN0;
	sei();	

	Delay_ms(500); 
	Delay_ms(500); 
	Delay_ms(500); 
	Delay_ms(500);
	
//	ds1307_readTime();

	//CLCD_BackLightOnOff(1);
	EventFlagE = 1;
	//history_veriable_init();
	//history_update(0,0);
	
	while(1)
	{
		//idle_modbus( );


		if(TimeTic_1ms)
		{
			//mseconds++;
		}

		if(TimeTic_10ms)
		{
			KeyProc();
			MainSYSTEM();
			Menu();

			//Check_event();
		}

		if(TimeTic_100ms)
		{
			
			//PAGE_LineScroll(6);
			
		}
		
		if(TimeTic_200ms)
		{
			//Data_Transfer();
			
		}

		if(TimeTic_500ms)
		{
			PORTL = PORTL + 0x01; 
			//All_Value_Refresh();

			communication_fault_cnt++;
			if(5 < communication_fault_cnt)	communication_fault = 1;
			
		}

		if(TimeTic_1s)
		{
	//		ds1307_readTime();
			//GLCD_print0508(7, 7,_TEXT("20%02d/%02d/%02d %02d:%02d:%02d",year,month,date,hour,min,second));

		}
		
		//GLCD_LCDUpdate();
		//GLCD_LCDScrollUpdate(6);

		
		SystemTimeTic();
		SystemEventAutoClear();
	}
}
