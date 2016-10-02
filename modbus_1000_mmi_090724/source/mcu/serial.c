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
//===============================================================================================

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#include <string.h>
#include "type.h"
#include "util.h"
#include "serial.h"

#define BUFFER_SIZE 100

char RxBuffer[BUFFER_SIZE];
volatile char* pRxRingBufStart;
volatile char* pRxDataStart;
volatile char* pRxDataEnd;
volatile char* pRxRingBufEnd;


volatile char TxBuffer[BUFFER_SIZE];
volatile char* pTxRingBufStart;
volatile char* pTxDataStart;
volatile char* pTxDataEnd;
volatile char* pTxRingBufEnd;



//==============================================================
// UART  initialize
void UART_init(void)
{
	UBRR0H = 0; 								// 19200 baud for 16MHz OSC
	UBRR0L = 51;
	UCSR0A = 0x00;								// asynchronous normal mode
	UCSR0B = 0xD8;								// Rx/Tx enable
	UCSR0C = 0x06;								// 8 data, 1 stop, no parity

	UBRR1H = 0; 								// 19200 baud for 16MHz OSC
	UBRR1L = 51;
	UCSR1A = 0x00;								// asynchronous normal mode
	UCSR1B = 0x18;								// Rx/Tx enbale, Rx Complete interrupt enable
	UCSR1C = 0x06;								// 8 data, 1 stop, no parity


	UBRR2H = 0;                                 // 19200 baud for 16MHz OSC
	UBRR2L = 51;
	UCSR2A = 0x00;                              // asynchronous normal mode
	UCSR2B = 0x18;                              // Rx/Tx enable, Rx Complete, Tx Complete,
	                                            // Data Register Empty interrupt enable
	UCSR2C = 0x06;                              // 8 data, 1 stop, no parity


	UBRR3H = 0;                                 // 19200 baud for 16MHz OSC
	UBRR3L = 51;
	UCSR3A = 0x00;                              // asynchronous normal mode
	UCSR3B = 0x18;                              // Rx/Tx enable, Rx Complete, Tx Complete,
	                                            // Data Register Empty interrupt enable
	UCSR3C = 0x06;                              // 8 data, 1 stop, no parity



	pRxRingBufStart = &RxBuffer[0];
	pRxDataStart = pRxDataEnd = pRxRingBufStart;
	pRxRingBufEnd = &RxBuffer[BUFFER_SIZE];

	pTxRingBufStart = &TxBuffer[0];
	pTxDataStart = pTxDataEnd = pTxRingBufStart;
	pTxRingBufEnd = &TxBuffer[BUFFER_SIZE];
}






//==============================================================
// base function
void TX0_char( char data)		/* transmit a character by USART0 */
{
	while((UCSR0A & 0x20) == 0x00);		// data register empty ?
	UDR0 = data;
}


void TX1_char( char data)		/* transmit a character by USART0 */
{
	while((UCSR1A & 0x20) == 0x00);		// data register empty ?
	UDR1 = data;
}


void TX2_char( char data)		/* transmit a character by USART0 */
{
	while((UCSR2A & 0x20) == 0x00);		// data register empty ?
	UDR2 = data;
}

void TX3_char( char data)		/* transmit a character by USART0 */
{
	while((UCSR3A & 0x20) == 0x00);		// data register empty ?
	UDR3 = data;
}

unsigned char RX3_char( void)
{
	if((UCSR3A & 0x80) == 0x00) return 0xFF;
	else return UDR3;
}

//==============================================================
// UART0 ring buffer 
int ReadRxBuf_Size(char* pBuf, int nSize)
{
	int RxDataSize;

	int nStartPos = pRxDataStart - pRxRingBufStart;
	int nSpareData = pRxRingBufEnd - pRxDataStart;
	//int nEndPos = pRxDataEnd - pRxRingBufStart;

	if(pRxDataEnd < pRxDataStart)
	{
		RxDataSize = pRxDataEnd - pRxRingBufStart;
		RxDataSize += nSpareData;
	}
	else
	{
		RxDataSize = pRxDataEnd - pRxDataStart;
	}

	if( nSize == -1) return RxDataSize;
	if( nSize > RxDataSize || RxDataSize == 0 ) return 0;

	if( nSpareData >= nSize )
	{
		memcpy(pBuf,pRxRingBufStart + nStartPos, nSize);
		pRxDataStart = pRxDataStart + nSize;
	}
	else
	{
		memcpy(pBuf, pRxRingBufStart + nStartPos, nSpareData);
		memcpy(pBuf + nSpareData, pRxRingBufStart, nSize - nSpareData);
		pRxDataStart = pRxRingBufStart + nSize - nSpareData;
	}

	return 1;
}

int WriteTxBuf_Size(char* pBuf, int nSize)
{
	int TxSpareSize;

	int nPos = pTxDataEnd - pTxRingBufStart;
	int nSpare = pTxRingBufEnd - pTxDataEnd;

	if(pTxDataEnd < pTxDataStart)
	{
		TxSpareSize = pTxDataStart -pTxDataStart;
	}
	else
	{
		TxSpareSize = ( pTxDataStart-pTxRingBufStart)+(pTxRingBufEnd-pTxDataEnd);
	}

	if( nSize == -1) return TxSpareSize;


	if( nSpare > nSize)
	{
		memcpy((void*)(pTxRingBufStart + nPos), pBuf, nSize);
		pTxDataEnd = pTxDataEnd + nSize;
	}		
	else
	{
		memcpy(pTxRingBufStart + nPos, pBuf, nSpare);
		memcpy(pTxRingBufStart, pBuf + nSpare, nSize -  nSpare);
		pTxDataEnd = pTxRingBufStart + nSize - nSpare;
	}
}

#define WriteTxBuf_Byte(c) WriteTxBuf_Size(&c,1);



void TxSendLOOP(void)
{
	unsigned char resume_data;


	if(pTxDataStart<=pTxDataEnd)
	{
		resume_data = pTxDataEnd -pTxDataStart;
	}
	else 
	{
		resume_data = (pTxRingBufEnd-pTxDataStart)+(pTxDataEnd-pTxRingBufStart);
	}

	if(resume_data)
	{
		if((UCSR0A & 0x20) == 0x20)
		{
			//_delay_ms(10);   
			UCSR0B = UCSR0B & 0xEF;

			RS485_TX_EN0;
			UDR0 = *pTxDataStart;
			pTxDataStart++;
			if(pTxRingBufEnd<=pTxDataStart)pTxDataStart = pTxRingBufStart;
		}
	}
	else
	{
		if((UCSR0A & 0x20) == 0x20)
		{

		}
	}
	
	
}


ISR(USART0_TX_vect)             // USART2 Tx Complete interrupt
{
	unsigned char resume_data;


	if(pTxDataStart<=pTxDataEnd)
	{
		resume_data = pTxDataEnd -pTxDataStart;
	}
	else 
	{
		resume_data = (pTxRingBufEnd-pTxDataStart)+(pTxDataEnd-pTxRingBufStart);
	}


	if(!resume_data)  
	{
		RS485_RX_EN0;
		UCSR0B = UCSR0B | 0x10;
	}
}

//void RxReceiveByte(void)
ISR(USART0_RX_vect)				/* UART0 interrupt function */
{
	char _byte;
	_byte = UDR0;

	if(pRxRingBufEnd <= pRxDataEnd ) pRxDataEnd =pRxRingBufStart;
	
	*pRxDataEnd = _byte;
	pRxDataEnd++;
}



