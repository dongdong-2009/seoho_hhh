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
//                              - 기존의 링버퍼를 활용하여 자기자신의 데이터가 변할蒻?//===============================================================================================

//#include <avr/io.h>
//#include <avr/interrupt.h>
//#include <util/delay.h>

#include <inavr.h>
#include <iom2560.h>

#include <string.h>
#include "type.h"
#include "util.h"
#include "rs232_MMItoPANEL.h"

#include "display.h"

CRC_flg	CRC ;
Data_flg	data_flg;

unsigned int Communication_Fault_Cnt = 3;
unsigned char Communication_Fault_Flag = 0;


unsigned int SCI_Registers[BUF_MAX];

unsigned char packet[8];

static const UCHAR aucCRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40
};

static const UCHAR aucCRCLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
    0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
    0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
    0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
    0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
    0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
    0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 
    0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
    0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
    0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
    0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 
    0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
    0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
    0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
    0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
    0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
    0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
    0x41, 0x81, 0x80, 0x40
};


unsigned int CRC16( UCHAR * pucFrame,unsigned int usLen )
{
    UCHAR           ucCRCHi = 0xFF;
    UCHAR           ucCRCLo = 0xFF;
    int             iIndex;

    while( usLen-- )
    {
        iIndex = ucCRCLo ^ *( pucFrame++ );
        ucCRCLo = ( UCHAR )( ucCRCHi ^ aucCRCHi[iIndex] );
        ucCRCHi = aucCRCLo[iIndex];
    }
    return ( unsigned int )( ucCRCHi << 8 | ucCRCLo );
}



//==============================================================
// UART  initialize
void UART_init(void)
{
	UBRR0H = 0; 								// 19200 baud for 16MHz OSC
	UBRR0L = 16;
	UCSR0A = 0x02;								// asynchronous normal mode
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


void WriteDataMem(unsigned int addr, unsigned int dat)
{
	//*(volatile int *)(DATA_REG + (addr<<1)+0) = (char)(dat>>8);
	//*(volatile int *)(DATA_REG + (addr<<1)+1) = (char)dat;
	DATA_Registers[addr] = dat;
}
unsigned int ReadDataMem(unsigned int addr)
{
	unsigned int data_word = 0;
	//data_word =  MAKEWORD( (*(volatile int *)(DATA_REG + (addr<<1)+0 )),(*(volatile int *)(DATA_REG + (addr<<1)+1)) );
	data_word = DATA_Registers[addr];
 	return data_word;
}
//-----------------------------
// 데이타 수신
//-----------------------------
 unsigned char SciC_RxStep=0;
 volatile unsigned char SciC_RxFlag=0;

 unsigned int RxType=0;
 unsigned int RxAddr=0;
 unsigned int RxData=0;
 unsigned int RxCRC=0;
 unsigned char RxBuf[9];
 unsigned char RXD0;

#pragma vector = USART0_RX_vect
__interrupt void USART0_RX_ISR(void)
//ISR(USART0_RX_vect)	
{
	char c;

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
		else if(SciC_RxStep == 2)//type
		{
			RxBuf[2] = RXD0;
			SciC_RxStep++;
		}
		else if(SciC_RxStep == 3)//addr_h
		{
			RxBuf[3] = RXD0;
			SciC_RxStep++;
		}
		else if(SciC_RxStep == 4)//addr_l
		{
			RxBuf[4] = RXD0;
			SciC_RxStep++;
		}
		else if(SciC_RxStep == 5)//data_h
		{
			RxBuf[5] = RXD0;
			SciC_RxStep++;
		}
		else if(SciC_RxStep == 6)//data_l
		{
			RxBuf[6] = RXD0;
			SciC_RxStep++;
		}
		else if(SciC_RxStep == 7)//crc_H
		{
			RxBuf[7] = RXD0;
			SciC_RxStep++;
		}
		else//crc_L
		{
			RxBuf[8] = RXD0;
			//SciC_RxFlag = 1;

			 //if(SciC_RxFlag)
		      	{
					CRC.Word = CRC16(RxBuf,7);

					if((RxBuf[7] == CRC.Byte.b1) && (RxBuf[8] == CRC.Byte.b0))
					{
						RxType = RxBuf[2];
						RxAddr = ((unsigned int)RxBuf[3]<<8) | RxBuf[4] ;
						RxData = ((unsigned int)RxBuf[5]<<8) | RxBuf[6] ;
						RxCRC   = ((unsigned int)RxBuf[7]<<8) | RxBuf[8] ;

						if(RxType == SEND)
						{
							CRC.Word = 0;

							RxBuf[2] = RESPONSE;

							CRC.Word = CRC16(RxBuf,7);

							for(c=0;c<7;c++)	TX0_char(RxBuf[c]);
							TX0_char(CRC.Byte.b1);
							TX0_char(CRC.Byte.b0);

							DATA_Registers[RxAddr] = RxData;
						}
						else if(RxType == REQUEST)
						{
							CRC.Word = 0;

							RxBuf[2] = SEND;
							RxBuf[5] = (char)(DATA_Registers[RxAddr]>>8);
							RxBuf[6] = (char)DATA_Registers[RxAddr];

							CRC.Word = CRC16(RxBuf,7);

							for(c=0;c<7;c++)	TX0_char(RxBuf[c]);
							
							TX0_char(CRC.Byte.b1);
							TX0_char(CRC.Byte.b0);

						}
						else if(RxType == RESPONSE)
						{
							SCI_Registers[RxAddr] = RxData;
						}
						else if(RxType == QUERY)
						{
							Communication_Fault_Cnt = 15;
							device_type = RxData;
						}
						
					}
					
					SciC_RxStep=0;
		      	}
		}
	}

	return ;
}



void TX0_char( char data)		/* transmit a character by USART0 */
{
	while((UCSR0A & 0x20) == 0x00);		// data register empty ?
	UDR0 = data;
}
/*
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
*/


//WORD SCI_Registers[Buf_MAX];
WORD SCI_TxOffset=0;
WORD TxDelyCnt=0;
 unsigned char TxBuf[9];
void SCI_Process(void)
{
unsigned char 	c ;

//Rx================================


	 
//Tx================================
	if(!TxDelyCnt)
	{
		if(DATA_Registers[SCI_TxOffset] != SCI_Registers[SCI_TxOffset])
		{

			CRC.Word = 0;
			TxBuf[0] = 0xAB;
			TxBuf[1] = 0xCD;
			TxBuf[2] = SEND;
			TxBuf[3] = ((char)(SCI_TxOffset>>8));	
			TxBuf[4]=  ((char)SCI_TxOffset);
			TxBuf[5] = (char)(DATA_Registers[SCI_TxOffset]>>8);
			TxBuf[6] = (char)DATA_Registers[SCI_TxOffset];

			CRC.Word = CRC16(TxBuf,7);

			for(c=0;c<7;c++)	TX0_char(TxBuf[c]);
			
			TX0_char(CRC.Byte.b1);
			TX0_char(CRC.Byte.b0);

			
			TxDelyCnt = 10;
		}

		SCI_TxOffset ++;
		if(BUF_MAX <= SCI_TxOffset) 
		{
			SCI_TxOffset = 0;
		}

		if(TimeTic_1s)
		{
			CRC.Word = 0;
			
			TxBuf[0] = 0xAB;
			TxBuf[1] = 0xCD;
			TxBuf[2] = QUERY;
			TxBuf[3] = 25;	
			TxBuf[4]=  0;
			TxBuf[5] = 0;
			TxBuf[6] = 0;

			CRC.Word = CRC16(TxBuf,7);

			for(c=0;c<7;c++)	TX0_char(TxBuf[c]);
			
			TX0_char(CRC.Byte.b1);
			TX0_char(CRC.Byte.b0);

			

			if(!Communication_Fault_Cnt)Communication_Fault_Flag=1;
			else 
			{
				Communication_Fault_Cnt--;
				Communication_Fault_Flag=0;
			}
		}

		if(TimeTic_500ms)
		{
			SCI_RequestData(gRequestAddr);
		}
	}
	else
	{
		TxDelyCnt--;
	}
}


void SCI_RequestData(unsigned int addr)
{
unsigned char 	c ;

				CRC.Word = 0;
			
			TxBuf[0] = 0xAB;
			TxBuf[1] = 0xCD;
			TxBuf[2] = REQUEST;
			TxBuf[3] = (char)(addr>>8);	
			TxBuf[4]=  (char)addr;

			TxBuf[5] = 0;
			TxBuf[6] = 0;

			CRC.Word = CRC16(TxBuf,7);

			for(c=0;c<7;c++)	TX0_char(TxBuf[c]);
			
			TX0_char(CRC.Byte.b1);
			TX0_char(CRC.Byte.b0);

}

void SCI_SendData(unsigned int addr, unsigned int data)
{

unsigned char 	c ;

			CRC.Word = 0;
			
			TxBuf[0] = 0xAB;
			TxBuf[1] = 0xCD;
			TxBuf[2] = SEND;
			TxBuf[3] = (char)(addr>>8);	
			TxBuf[4]=  (char)addr;
			TxBuf[5] = (char)(data>>8);
			TxBuf[6] = (char)data;

			CRC.Word = CRC16(TxBuf,7);

			for(c=0;c<7;c++)	TX0_char(TxBuf[c]);
			
			TX0_char(CRC.Byte.b1);
			TX0_char(CRC.Byte.b0);


	DATA_Registers[addr] = data;


}

void SCI_RegisterRefresh(void)
{
	int i;
	unsigned char 	c ;
	for(i=0;i<BUF_MAX;i++)
	{
		DATA_Registers[i]=0;
		SCI_Registers[i]=0;
	}

		CRC.Word = 0;
			
			TxBuf[0] = 0xAB;
			TxBuf[1] = 0xCD;
			TxBuf[2] = SEND;
			TxBuf[3] = 0x0C;	
			TxBuf[4]=  0x7B;
			TxBuf[5] = 0;
			TxBuf[6] = 1;

			CRC.Word = CRC16(TxBuf,7);

			for(c=0;c<7;c++)	TX0_char(TxBuf[c]);
			
			TX0_char(CRC.Byte.b1);
			TX0_char(CRC.Byte.b0);

	DATA_Registers[0x0C7B]=1;
	SCI_Registers[0x0C7B]=0;

}


#pragma vector = USART0_TX_vect
__interrupt void USART0_TX_ISR(void)
//ISR(USART0_TX_vect)             // USART2 Tx Complete interrupt
{
	UCSR0B = UCSR0B | 0x10;
}





