//====================================================
//
// Copyright(C) 2008 HanYoung ELEC. Co., LTD.
// Designed by Paul B.H Park(idonno2000)
//
//
//	filename : key,h
//	description : key 헤더파일
//
//	history : - frist designed at 2008-05-28 3:58오후
//
//====================================================



#ifndef _KEY_H_
#define _KEY_H_

#define UP		0x01
#define DN		0x02
#define RIGHT	0x03
#define LEFT		0x04
#define ESC		0x05
#define ENTER	0x06
#define MENU	0x07
#define RUN		0x08
#define STOP		0x09

typedef struct{

	unsigned char	KeyValue;
	unsigned char	LongKeyFlag;

	unsigned char	KeyHandle;

}KEY;
extern KEY KeyState;

void KeyInit(void);
unsigned int KeyScan(void);
void KeyProc(void);


#endif

