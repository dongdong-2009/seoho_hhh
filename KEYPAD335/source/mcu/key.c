//====================================================
//
// Copyright(C) 2008 HanYoung ELEC. Co., LTD.
// Designed by Paul B.H Park(idonno2000)
//
//
//	filename : key.c
//	description : Key or Ir or interrupt Input state
//
//	history : - frist designed at 2008-05-28 4:02¿ÀÈÄ
//
//====================================================

//#include <avr/io.h>
//#include <util/delay.h>

#include <inavr.h>
#include <iom2560.h>

#include "key.h"

#include "debug_printf.h"


unsigned char key_signal_A=0xFF;
unsigned char key_signal_B=0xFF;
unsigned int key_data = 0xFF;
unsigned int 	key_press_time=0;

unsigned char LongKeyFlag=0;

KEY KeyState;

void KeyInit(void)
{
	//ADCSRA = 0xA5;
	//ADCSRB = 0x08;
	//ADMUX = 0x60;
	//_delay_ms(500); 
	//ADCSRA = 0xE5;
	//_delay_ms(100); 
	

	KeyState.KeyValue = 0xFF;
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
	unsigned char key_output=0xFF;
	unsigned int key = 0xFFFF;
        key = PINK | 0xFE;
	key = PINJ | (key<<8);

	if(key != 0xFFFF)key_signal_A=0x00;
	else key_signal_A=0xFF;
	
	if((!key_signal_A)&&(!key_signal_B))//key pressing
	{
		//key_press_time++;
		if( key_press_time <= 10 ) key_data = key;
		else
		{
			key_press_time = 9;

			switch(key_data){
				case 0xFFFD:		key_output = UP;		break;
				case 0xFF7F:		key_output = DN;		break;
				case 0xFFDF:		key_output = RIGHT;	break;
				case 0xFFF7:		key_output = LEFT;	break;
				case 0xFFFE:		key_output = ESC;		break;
				case 0xFFFB:		key_output = ENTER;	break;
				case 0xFFEF:		key_output = MENU;	break;
				case 0xFFBF:		key_output = RUN;		break;
				case 0xFEFF:		key_output = STOP;	break;
				case 0xFEBA:		key_output = 0xFE;	break;
				default:			key_output = 0xFF;	break;
				}
			
			LongKeyFlag=1;
			return key_output;
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
			switch(key_data){
				case 0xFFFD:		key_output = UP;		break;
				case 0xFF7F:		key_output = DN;		break;
				case 0xFFDF:		key_output = RIGHT;	break;
				case 0xFFF7:		key_output = LEFT;	break;
				case 0xFFFE:		key_output = ESC;		break;
				case 0xFFFB:		key_output = ENTER;	break;
				case 0xFFEF:		key_output = MENU;	break;
				case 0xFFBF:		key_output = RUN;		break;
				case 0xFEFF:		key_output = STOP;	break;
				default:			key_output = 0xFF;	break;
				}
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

