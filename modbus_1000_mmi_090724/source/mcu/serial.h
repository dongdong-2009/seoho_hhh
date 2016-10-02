//====================================================
//
// Copyright(C) 2008 HanYoung ELEC. Co., LTD.
// Designed by Paul B.H Park(idonno2000)
//
//
//	filename : serial.h
//	description : 시리얼통신 헤더파일
//	last revision : 2008-05-09 3:58오후
//	history : 
//
//====================================================

#ifndef _SERIAL_H_
#define _SERIAL_H_

#define RS485_TX_EN0         (PORTD |= BIT4)
#define RS485_RX_EN0        (PORTD &= (~BIT4))

#define RS485_TX_EN1         (PORTD |= BIT5)
#define RS485_RX_EN1        (PORTD &= (~BIT5))

void UART_init(void);

void TX0_char( char data);
void TX1_char( char data);
void TX2_char( char data);
void TX3_char( char data);

unsigned char RX3_char( void);


int ReadRxBuf_Size(char* pBuf, int nSize);
int WriteTxBuf_Size(char* pBuf, int nSize);


void TxSendByte(void);
void TxSendLOOP(void);

//ISR(USART0_RX_vect);				/* UART0 interrupt function */

#endif
