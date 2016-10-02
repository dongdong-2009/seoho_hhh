//====================================================
//
// Copyright(C) 2008 HanYoung ELEC. Co., LTD.
// Designed by Paul B.H Park(idonno2000)
//
//
//	filename : type.h
//	description : 코딩습관은 어쩔수 없다
//                내맘대로 쓰려고 여러가지를 정의해뒀다
//
//	history : frist designed at 2008-05-28 3:58오후
//
//====================================================

#ifndef _TYPE_H_
#define _TYPE_H_

//#include <avr/sfr_defs.h>

#ifndef BYTE
	#define	BYTE unsigned char
#endif

#ifndef UCHAR
	#define	UCHAR unsigned char
#endif

#ifndef CHAR
	#define	CHAR unsigned char
#endif

#ifndef BOOL
	#define BOOL unsigned char
#endif

#ifndef INT8
	#define	INT8 char
#endif

#ifndef INT16
	#define	INT16 short
#endif

#ifndef INT32
	#define	INT32 long
#endif

#ifndef LONG
	#define	LONG long
#endif

#ifndef SBYTE
	#define SBYTE char
#endif

#ifndef SWORD
	#define SWORD int
#endif

#ifndef SDWORD
	#define SDWORD long
#endif

#ifndef UINT16
	#define	UINT16 unsigned int 
#endif

#ifndef UINT32
	#define	UINT32 unsigned long 
#endif

#ifndef WORD
	#define	WORD unsigned int
#endif

#ifndef DWORD
	#define	DWORD unsigned long
#endif

#ifndef byte
	#define	byte unsigned char
#endif

#ifndef word
	#define	word unsigned int
#endif

#ifndef FLOAT
	#define	FLOAT float 
#endif

#ifndef DOUBLE
	#define	DOUBLE double  
#endif


#ifndef TRUE
	#define TRUE 1
	#define true 1
#endif

#ifndef FALSE
	#define FALSE 0
	#define false 0
#endif


#ifndef ON
	#define ON		1
#endif

#ifndef OFF
	#define OFF		0
#endif

#ifndef NULL
//#define NULL 0
#endif




#if 0
//BIT redefine SFR_REG for Hsu300d_mmi
//PORT A
#define TRIP			0x80//out
#define ALARM		0x40//out
#define BYP_FEED	0x20//out
#define INV_FEED	0x10//out
//#define BIT3	0x08
//#define BIT2	0x04
//#define BIT1	0x02
//#define BIT0	0x01

//PORT B
#define TRIP			0x80
#define ALARM		0x40
#define BYP_FEED	0x20
#define INV_FEED	0x10
//#define BIT3	0x08
//#define BIT2	0x04
//#define BIT1	0x02
//#define BIT0	0x01

//PORT C
//#define BIT7	0x80
//#define BIT6	0x40
//#define BIT5	0x20
//#define BIT4	0x10
//#define BIT3	0x08
//#define BIT2	0x04
//#define BIT1	0x02
//#define BIT0	0x01

//PORT D
#define BUZZ			0x80//out
#define LCD_LIGHT	0x40//out
//#define BIT5	0x20
//#define BIT4	0x10
//#define BIT3	0x08
//#define BIT2	0x04
//#define BIT1	0x02
//#define BIT0	0x01

//PORD E
#define INV_FEED_LED		0x80
#define INV_ON_LED			0x40
#define BAT_dCHARGE_LED	0x20
#define BAT_CHARGE_LED		0x10
#define CONV_LED			0x08
#define INPUT_LED			0x04
//#define BIT1	0x02
//#define BIT0	0x01

//PORD F
//#define BIT7	0x80
//#define BIT6	0x40
//#define BIT5	0x20
//#define BIT4	0x10
#define DIP_SW4	0x08
#define DIP_SW3	0x04
#define DIP_SW2	0x02
#define DIP_SW1	0x01

//PORD G
//#define BIT7	0x80
//#define BIT6	0x40
#define RS485_EN1	0x20
#define RS485_EN0	0x10
#define TEST_LED	0x08
//#define BIT2	0x04
//#define BIT1	0x02
//#define BIT0	0x01

//PORD H
#define CB0_LED			0x80
#define LED11			0x40
#define ALARM_LED		0x20
#define NORMAL_LED		0x10
#define SYNC_LED		0x08
#define BYP_FEED_LED	0x04
//#define BIT1	0x02
//#define BIT0	0x01


//PORD J
//#define BIT7	0x80
//#define BIT6	0x40
#define DC_LV4	0x20
#define DC_LV3	0x10
#define DC_LV2	0x08
#define DC_LV1	0x04
//#define BIT1	0x02
//#define BIT0	0x01


//PORD K
//#define BIT7	0x80
//#define BIT6	0x40
#define KEY_ENTER	0x20
#define KEY_RIGHT	0x10
#define KEY_LEFT		0x08
#define KEY_DN		0x04
#define KEY_UP		0x02
#define KEY_ESC		0x01


//PORD L
#define DC_LV0		0x80
#define BYP_OUT		0x40
#define SYSTEM_O	0x20
#define CB5			0x10
#define CB4			0x08
#define CB3			0x04
#define CB2			0x02
#define CB1			0x01
#endif

#endif
