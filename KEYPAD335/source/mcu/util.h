//====================================================
//
// Copyright(C) 2008 HanYoung ELEC. Co., LTD.
// Designed by Paul B.H Park(idonno2000)
//
//
//	filename : util,h
//	description : ��ƿ �������
//
//	history : - 2008-05-28 type.c���� �и��Ͽ� ���� ����
//            - ������ �Լ��� ��������å���� ������
//====================================================



#ifndef _UTIL_H_
#define _UTIL_H_


#define MAKEWORD(b, a)      ( (WORD)(a) | ( (WORD)(b) << 8) )
#define SWAPWORD(a)      	(( (((a)&0x00FF)<<8) | (((a)&0xFF00)>>8) ) )
#define LOBYTE(w)           ((BYTE)((w) & 0x00FF))
#define HIBYTE(w)           ((BYTE)(((w) & 0xFF00) >> 8))
#define MAKEDWORD(d,c,b,a)	( (DWORD)(a) | ((DWORD)(b) << 8) | ((DWORD)(c) << 16)  | ((DWORD)(d) << 24)  )
#define SPLITDWORD(d,c,b,a,dw){ 	d = ((dw)&0xFF000000)>>24;c = ((dw)&0x00FF0000)>>16;a = ((dw)&0x000000FF);}
#define BYTE3(a)	( ((a)&0xFF000000)>>24 )	
#define BYTE2(a)	( ((a)&0x00FF0000)>>16 )	
#define BYTE1(a)	( ((a)&0x0000FF00)>>8 )
#define BYTE0(a)	( ((a)&0x000000FF) )


#define	Bit_mask(x)		     (1 << (x))
#define Get_bit(val, bit)	 (((val) & Bit_mask(bit))>>(bit))
#define	Set_bit(val, bit)	 ((val) |= Bit_mask(bit))
#define	Bit_reset(val, bit)	 ((val) &= ~Bit_mask(bit))
#define	Toggle_bit(val, bit) ((val) ^=Bit_mask(bit))

#define BIT7	0x80
#define BIT6	0x40
#define BIT5	0x20
#define BIT4	0x10
#define BIT3	0x08
#define BIT2	0x04
#define BIT1	0x02
#define BIT0	0x01



void Delay_us(unsigned char time_us);		/* time delay for us */
void Delay_ms(unsigned int time_ms);		/* time delay for ms */
void inttoa(int val, char *str, int len);



#endif

