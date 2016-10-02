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

extern volatile char communication_fault_cnt;
extern volatile char _communication_fault, communication_fault;



void UART_init(void);

void TX1_char( char data);

void Data_Transfer(void);

//ISR(USART0_RX_vect);				/* UART0 interrupt function */

#endif
