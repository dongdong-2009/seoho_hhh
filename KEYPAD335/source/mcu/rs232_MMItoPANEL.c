//==============================================================================================
//
// Copyright(C) 2008 HanYoung ELEC. Co., LTD.
// Designed by Paul B.H Park(idonno2000)
//
//
//	filename : serial.c
//	description : 시리얼통신 함수 정의
//
//	history : 2008-05-09 4:03오후
//            2008-06-26 - 링퍼버 수신모듈 구성
//                       - 송신부분은 테스트중
//                       - 관련문서 ( ringbuffer.doc )
//                                  ( 링버퍼 패킷송수신.vsd) 
//           2008-08-20 - printf함수 포함
//                              - 링버퍼 수신모듈 리턴인자 수정 (0or1)
//           1010-03-03 - MINV MMI to CTRL_PANEL간 통신 전용
//                              - 기존의 링버퍼를 활용하여 자기자신의 데이터가 변할蒻�
//===============================================================================================

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#include <string.h>
#include "type.h"
#include "util.h"
#include "rs232_MMItoPANEL.h"

#include "display.h"


//==============================================================
// UART  initialize
void UART_init(void)
{
	UBRR0H = 0; 								// 19200 baud for 16MHz OSC
	UBRR0L = 51;
	UCSR0A = 0x00;								// asynchronous normal mode
	UCSR0B = 0xD8;								// Rx/Tx enbale, Rx Complete interrupt enable
	UCSR0C = 0x06;								// 8 data, 1 stop, no parity

#if 0
	//timer 3 인터럽트
	TCCR3A = 0x10;
	TCCR3B = 0x0C;
	TCCR3C = 0x00;
	OCR3A = 62;//1ms
	TCNT3 = 0x0000;

	TIMSK3 = 0x02;
	TIFR3 = 0x2F;
#endif


	CRC.Word = 0 ;

}



//-----------------------------
// 데이타 수신
//-----------------------------
volatile unsigned char SciC_RxStep=0;
volatile unsigned char SciC_RxFlag=0;

volatile unsigned int RxAddr=0;
volatile unsigned int RxData=0;
volatile unsigned int RxCRC=0;
volatile unsigned char RxBuf[8];
volatile unsigned char RXD0;
ISR(USART0_RX_vect)	
{
	RXD0 = UDR0 ;
	if(!SciC_RxFlag)
	{
		if(SciC_RxStep == 0)//sync1
		{
			if(RXD0 == 0xAB)
			{
				
				RxBuf[0] = 0xAB;
				SciC_RxStep++;
			}
			else SciC_RxStep=0;
		}
		else if(SciC_RxStep == 1)//sync2
		{
			if(RXD0 == 0xCD)
			{
				RxBuf[1] = 0xCD;
				SciC_RxStep++;
			}
			else SciC_RxStep=0;
		}
		else if(SciC_RxStep == 2)//addr_h
		{
			RxBuf[2] = RXD0;
			SciC_RxStep++;
		}
		else if(SciC_RxStep == 3)//addr_l
		{
			RxBuf[3] = RXD0;
			SciC_RxStep++;
		}
		else if(SciC_RxStep == 4)//data_h
		{
			RxBuf[4] = RXD0;
			SciC_RxStep++;
		}
		else if(SciC_RxStep == 5)//data_l
		{
			RxBuf[5] = RXD0;
			SciC_RxStep++;
		}
		else if(SciC_RxStep == 6)//crc_H
		{
			RxBuf[6] = RXD0;
			SciC_RxStep++;
		}
		else//crc_L
		{
			RxBuf[7] = RXD0;

			CRC.Word = 0;
			CRC_16(RxBuf[0]);
			CRC_16(RxBuf[1]);
			CRC_16(RxBuf[2]);
			CRC_16(RxBuf[3]);
			CRC_16(RxBuf[4]);
			CRC_16(RxBuf[5]);

			RxAddr = ((unsigned int)RxBuf[2]<<8) | RxBuf[3] ;
			RxData = ((unsigned int)RxBuf[4]<<8) | RxBuf[5] ;
			RxCRC   = ((unsigned int)RxBuf[6]<<8) | RxBuf[7] ;

			if(RxCRC == CRC.Word) SciC_RxFlag=1;
			
			SciC_RxStep=0;
		}
	}
	else SciC_RxStep=0;


	return ;
}



void TX0_char( char data)		/* transmit a character by USART0 */
{
	while((UCSR0A & 0x20) == 0x00);		// data register empty ?
	UDR0 = data;
}

//===============================================================================================
void CRC_16(unsigned char input)
{
	unsigned char 	i ;
	unsigned int 	tmp_CRC ;

	tmp_CRC=((CRC.Word >> 8) ^ input) << 8 ;
	for (i = 0 ; i < 8 ; i++)
	{
		if (tmp_CRC & 0x8000) tmp_CRC = (tmp_CRC << 1) ^ GEN_POLYNOMAL ;
		else tmp_CRC <<= 1 ;
	}
	CRC.Word = (CRC.Word << 8) ^ tmp_CRC ;
}



//WORD SCI_Registers[Buf_MAX];
WORD SCI_TxOffset=0;
void SCIC_Tx_process(void)
{
	unsigned int data, temp;

	data = MAKEWORD( (*(volatile int *)(DATA_REG + ((SCI_TxOffset<<1) +0)) ),(*(volatile int *)(DATA_REG + (SCI_TxOffset<<1) +1)) ) ;
	temp = MAKEWORD( (*(volatile int *)(TEMP_REG + ((SCI_TxOffset<<1) +0)) ),(*(volatile int *)(TEMP_REG + (SCI_TxOffset<<1) +1)) ) ;	

	if(data != temp)
	{
		//SCI_Registers[SCI_TxOffset] = Data_Registers[SCI_TxOffset];

		CRC.Word = 0;

		TX0_char(0xAB);												CRC_16(0xAB);
		TX0_char(0xCD);												CRC_16(0xCD);

		TX0_char((char)(SCI_TxOffset>>8));							CRC_16((char)(SCI_TxOffset>>8));
		TX0_char((char)SCI_TxOffset);									CRC_16((char)SCI_TxOffset);

		TX0_char(*(volatile int *)(DATA_REG + (SCI_TxOffset<<1)+0));		CRC_16(*(volatile int *)(DATA_REG + (SCI_TxOffset<<1)+0));
		TX0_char(*(volatile int *)(DATA_REG + (SCI_TxOffset<<1)+1));		CRC_16(*(volatile int *)(DATA_REG + (SCI_TxOffset<<1)+1));

		TX0_char(CRC.Byte.b1);
		TX0_char(CRC.Byte.b0);
	}

	SCI_TxOffset ++;
	if(BUF_MAX <= SCI_TxOffset) SCI_TxOffset = 0;
}


void SCIC_Rx_process(void)
{
	unsigned int data, temp;

	 if(SciC_RxFlag)
      	{
      		//SCI_Registers[RxAddr] = RxData;
      		*(volatile int *)(TEMP_REG + (RxAddr<<1)+0) = RxBuf[4];
      		*(volatile int *)(TEMP_REG + (RxAddr<<1)+1) = RxBuf[5];

		data = MAKEWORD( (*(volatile int *)(DATA_REG + ((RxAddr<<1) +0)) ),(*(volatile int *)(DATA_REG + (RxAddr<<1) +1)) ) ;
		temp = MAKEWORD( (*(volatile int *)(TEMP_REG + ((RxAddr<<1) +0)) ),(*(volatile int *)(TEMP_REG + (RxAddr<<1) +1)) ) ;		
		if(data != temp )
		{
			TX0_char(RxBuf[0]);
			TX0_char(RxBuf[1]);
			TX0_char(RxBuf[2]);
			TX0_char(RxBuf[3]);
			TX0_char(RxBuf[4]);
			TX0_char(RxBuf[5]);
			TX0_char(RxBuf[6]);
			TX0_char(RxBuf[7]);

			*(volatile int *)(DATA_REG + (RxAddr<<1)+0) = RxBuf[4];
  			*(volatile int *)(DATA_REG + (RxAddr<<1)+1) = RxBuf[5];
			//Data_Registers[RxAddr] = RxData;
		}
				
		SciC_RxFlag = 0;
      	}
}



ISR(USART0_TX_vect)             // USART2 Tx Complete interrupt
{
	UCSR0B = UCSR0B | 0x10;
}



