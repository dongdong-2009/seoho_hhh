//==============================================================================================
//
// Copyright(C) 2008 HanYoung ELEC. Co., LTD.
// Designed by Paul B.H Park(idonno2000)
//
//
//	filename : serial.c
//	description : �ø������ �Լ� ����
//
//	history : 2008-05-09 4:03����
//            2008-06-26 - ���۹� ���Ÿ�� ����
//                       - �۽źκ��� �׽�Ʈ��
//                       - ���ù��� ( ringbuffer.doc )
//                                  ( ������ ��Ŷ�ۼ���.vsd) 
//           2008-08-20 - printf�Լ� ����
//                              - ������ ���Ÿ�� �������� ���� (0or1)
//           1010-03-03 - MINV MMI to CTRL_PANEL�� ��� ����
//                              - ������ �����۸� Ȱ���Ͽ� �ڱ��ڽ��� �����Ͱ� ����嶧
//===============================================================================================

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#include <string.h>
#include "type.h"
#include "util.h"
#include "rs232_MMItoPANEL.h"

#include "display.h"


 volatile char _communication_fault =0, communication_fault=0;
 volatile char communication_fault_cnt=0;

//==============================================================
// UART  initialize
void UART_init(void)
{
	UBRR1H = 0; 								// 19200 baud for 16MHz OSC
	UBRR1L = 51;
	UCSR1A = 0x00;								// asynchronous normal mode
	UCSR1B = 0xD8;								// Rx/Tx enbale, Rx Complete interrupt enable
	UCSR1C = 0x06;								// 8 data, 1 stop, no parity

}




//==============================================================
// base function

void TX1_char( char data)		/* transmit a character by USART0 */
{
	while((UCSR1A & 0x20) == 0x00);		// data register empty ?
	UDR1 = data;
}





unsigned char data_cnt=0;
void mmiSetData(unsigned char addr, unsigned int data)
{
	BYTE cnt;
	while((UCSR1A & 0x20) == 0x00);		// data register empty ?
		UDR1 = 0xAB;

	while((UCSR1A & 0x20) == 0x00);		// data register empty ?
		UDR1 = 0xCD;

	while((UCSR1A & 0x20) == 0x00);		// data register empty ?
		UDR1 = addr;

	while((UCSR1A & 0x20) == 0x00);		// data register empty ?
		UDR1 = (char)(data>>8);
		
	while((UCSR1A & 0x20) == 0x00);		// data register empty ?
		UDR1 =(char)data;

}


ISR(USART1_TX_vect)             // USART2 Tx Complete interrupt
{
		UCSR0B = UCSR0B | 0x10;
}


volatile unsigned char RxBuf=0;
volatile unsigned char RxStep=0;
volatile unsigned int buf_offset=0;
volatile unsigned char crc=0;
ISR(USART1_RX_vect)				/* UART0 interrupt function */
{
	BYTE i;
	BYTE one_byte;            
	one_byte = UDR1;

	if(RxStep==0)
	{
		if(one_byte == 0xAB)RxStep++;
		else RxStep=0;

		crc=0;
	}
	else if(RxStep==1)
	{
		if(one_byte == 0xCD)RxStep++;
		else RxStep=0;
	}
	else if(RxStep==2)
	{
		RxBuf = (WORD)one_byte;
		crc = crc ^ one_byte;
		RxStep++;
	}
	else if(RxStep==3)
	{
		main_data_array[buf_offset] = ((WORD)RxBuf<<8) | (WORD)one_byte;
		crc = crc ^ one_byte;
		RxStep=2;
		buf_offset++;
		if(MX_DATA_ARRAY <= buf_offset)
		{
			RxStep=4;
			buf_offset=0;
			communication_fault_cnt = 0;
			communication_fault = 0;
		}
		
	}
	else if(RxStep==4)
	{
		if(crc != one_byte)
		{
			for(i=0;i<MX_DATA_ARRAY;i++)
			{
				main_data_array[i] = main_data_array_old[i];
			}
		}
		RxStep = 0;
	}
}






