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
#include "util.h"


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

extern unsigned int 	key_press_time;


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
		key_press_time++;
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
			if(5<EventTimeB)//5s
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

	KeyInit();
	Timer_IntteruptSet();
	UART_init();
	DisplayInit();
	
	// º€ºˆΩ≈ Ω∫≈√ √ ±‚»≠
	Initialize_SCI_Stack() ;					//Serial_Comm.c

	s_reg = ms_cnt + 2000 ;
	
//	debug_devopen(TX2_char);

//	RS485_RX_EN0;
	sei();	
	wait() ;

	//Delay_ms(500); 
	//Delay_ms(500); 
	//Delay_ms(500); 
	//Delay_ms(500);
	
	EventFlagE = 1;
	
	while(1)
	{
		if(TimeTic_1ms)
		{
			//mseconds++;
		}

		if(TimeTic_10ms)
		{
			KeyProc();
			MainSYSTEM();
			Menu();
		}

		if(TimeTic_100ms)
		{
		}
		
		if(TimeTic_200ms)
		{
		}

		if(TimeTic_500ms)
		{
			//PORTL = PORTL ^ 0x0F; 
			//All_Value_Refresh();

			//communication_fault_cnt++;
			//if(5 < communication_fault_cnt)	communication_fault = 1;

			//TX0_char(0x55);
			// Write_TransmitSerialStack(0x55);
			
		}

		if(TimeTic_1s)
		{
			//GLCD_print0508(7, 7,_TEXT("20%02d/%02d/%02d %02d:%02d:%02d",year,month,date,hour,min,second));

		}

		Serial_Comm_Service(); 
		
		SystemTimeTic();
		SystemEventAutoClear();
	}
}
