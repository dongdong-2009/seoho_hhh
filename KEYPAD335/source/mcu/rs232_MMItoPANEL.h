//====================================================
//
// Copyright(C) 2008 HanYoung ELEC. Co., LTD.
// Designed by Paul B.H Park(idonno2000)
//
//
//	filename : serial.h
//	description : �ø������ �������
//	last revision : 2008-05-09 3:58����
//	history : 
//
//====================================================

#ifndef _SERIAL_H_
#define _SERIAL_H_

//---------------------------------------
typedef	struct
{
	unsigned	char OP;	//MSB
	unsigned	char OBJ;	//LSB
} Packet_Head_flg ;
Packet_Head_flg	Packet_Head ;

//---------------------------------------
typedef	union
{
	unsigned int Word;
	struct
	{
		unsigned char b0;	// CRC16 ���� ����Ʈ
		unsigned char b1;	// CRC16 ���� ����Ʈ
	} Byte;
} CRC_flg ;
CRC_flg	CRC ;



void UART_init(void);

void TX1_char( char data);

#endif
