//====================================================
//
// Copyright(C) 2008 HanYoung ELEC. Co., LTD.
// Designed by Paul B.H Park(idonno2000)
//
//
//	filename : key.c
//	description : Key or Ir or interrupt Input state
//
//	history : - frist designed at 2008-05-28 4:02오후
//
//====================================================

#include <avr/io.h>
#include <util/delay.h>

#include "key.h"

#include "debug_printf.h"


unsigned char key_signal_A=0xFF;
unsigned char key_signal_B=0xFF;
unsigned int key_data = 0xFF;
unsigned int 	key_press_time=0;

unsigned char LongKeyFlag=0;


void KeyInit(void)
{
	//ADCSRA = 0xA5;
	//ADCSRB = 0x08;
	//ADMUX = 0x60;
	//_delay_ms(500); 
	//ADCSRA = 0xE5;
	//_delay_ms(100); 
	

	KeyState.KeyValue = 0xFFFF;
	KeyState.LongKeyFlag = 0;

}

void KeyProc(void)
{
	//KeyState.KeyValue = RX3_char();

	KeyState.KeyValue = KeyScan();
	KeyState.LongKeyFlag = LongKeyFlag;
}

unsigned int KeyScan(void)
{
	unsigned int key_output=0xFFFF;
	unsigned int key = 0xFFFF;

	key = PINJ | ((PINK | 0xFE)<<8);

	if(key != 0xFFFF)key_signal_A=0x00;
	else key_signal_A=0xFFFF;
	
	if((!key_signal_A)&&(!key_signal_B))//key pressing
	{
		key_press_time++;
		if((2<key_press_time)&&(key_press_time<4))
		{
			key_data = key;
		}
		
		if(300<key_press_time)//시스템 틱에 연동하여 하는게 졸을것 같기도
		{
			if(key_press_time<=301)
			{
				//key_press_time=100;
				key_output = key_data;
				LongKeyFlag=1;
				return key_output;
			}
		}
	}
	else if((!key_signal_A)&&(key_signal_B))// key down edge
	{
		key_press_time=0;
		LongKeyFlag=0;
	}
	else if((key_signal_A)&&(!key_signal_B))// key up edge
	{

		if(!LongKeyFlag)
		{
			key_output = key_data;
			key_press_time=0;
		}
	//debug_printf("\x1B[2J");
	//debug_printf("0x%02x", key_output );
	}
	else if((key_signal_A)&&(key_signal_B))// key release
	{
	}
	
	key_signal_B = key_signal_A;

	return key_output;
}

