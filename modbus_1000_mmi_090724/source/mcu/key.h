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

typedef struct{

	unsigned char	KeyValue;
	unsigned char	LongKeyFlag;

	unsigned char	KeyHandle;

}KEY;
KEY KeyState;

void KeyInit(void);
unsigned char KeyScan(void);
void KeyProc(void);


#endif

