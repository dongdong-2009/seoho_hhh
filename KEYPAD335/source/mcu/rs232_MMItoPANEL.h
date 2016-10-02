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


#define BUF_MAX 3400

#define DATA_REG 0x8000
#define TEMP_REG 0xB000

#define	GEN_POLYNOMAL	0x8821		// CRC ����

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

void WriteDataMem(unsigned int addr, unsigned int dat);
unsigned int ReadDataMem(unsigned int addr);

void UART_init(void);

void TX0_char( char data);

void CRC_16(unsigned char input);
void SCIC_Tx_process(void);
void SCIC_Rx_process(void);

void InputDataReg(unsigned int addr, unsigned int dat);

#endif
