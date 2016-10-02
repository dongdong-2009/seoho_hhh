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
		unsigned char b0;	// CRC16 하위 바이트
		unsigned char b1;	// CRC16 상위 바이트
	} Byte;
} CRC_flg ;
CRC_flg	CRC ;

extern unsigned int	ms_cnt ;
extern unsigned int	s_reg ;

extern unsigned char Auto_BR_Detection_Mode;
extern unsigned char Auto_BR_Detection_Cnt;


void UART_init(void);

void TX0_char( char data);

void Initialize_SCI_Stack(void);

void WRITE_Packet_PARAMETER_ACCESS(void);
void WRITE_Packet_PARAMETER_BLOCK_ACCESS(void); 

void Serial_Comm_Service(void); 
void Read_DATA_from_ControlBoard(unsigned char GROUP, unsigned char INDEX);

#endif
