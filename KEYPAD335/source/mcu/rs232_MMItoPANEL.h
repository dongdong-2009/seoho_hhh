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

//---------------------------------------
typedef	union
{
	unsigned int Word;
	struct
	{
		unsigned char b0;	// CRC16 ���� ����Ʈ
		unsigned char b1;	// CRC16 ���� ����Ʈ
	} Byte;
}Data_flg;
Data_flg	data_flg;

extern unsigned int	ms_cnt ;
extern unsigned int	s_reg ;

extern unsigned char Read_GROUP;
extern unsigned char Read_INDEX;

extern unsigned char NewDataFlag;


void Wait(void); 

void UART_init(void);

void TX0_char( char data);

void Initialize_SCI_Stack(void);

void Send_Parameter(unsigned char GROUP,unsigned char INDEX,unsigned DATA) ;

void WRITE_Packet_PARAMETER_ACCESS(void);
void WRITE_Packet_PARAMETER_BLOCK_ACCESS(void); 

void Serial_Comm_Protocol(void);
void Serial_Comm_Service(void); 
void Read_DATA_from_ControlBoard(unsigned char GROUP, unsigned char INDEX);

#endif
