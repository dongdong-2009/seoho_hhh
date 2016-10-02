//====================================================
//
// Copyright(C) 2008 HanYoung ELEC. Co., LTD.
// Designed by Paul B.H Park(idonno2000)
//
//
//	filename : display.h
//	description : display 헤더파일
//
//	history : - frist designed at 2008-05-28 3:58오후
//
//====================================================



#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <avr/pgmspace.h>



#define UP 0xFD
#define DN 0xFB
#define RIGHT 0xEF
#define LIFT 0xF7
#define ESC 0xFE
#define ENTER 0xDF


#define HISTORY_MAX 40
#define HISTORY_CHAR_MAX 40

//extern unsigned long naviMENU;
extern unsigned char RefreshFlag;

extern unsigned char EventFlagA;
extern unsigned char EventFlagB;
extern unsigned char EventFlagC;
extern unsigned char EventFlagD;
extern unsigned char EventFlagE;

extern unsigned char EventTimeA;
extern unsigned char EventTimeB;
extern unsigned char EventTimeC;
extern unsigned char EventTimeD;
extern unsigned char EventTimeE;






void DisplayInit(void);
void MainSYSTEM(void);
void MENU(void);

void Point_inversion(unsigned char x, unsigned char y,  char cnt);
void Point_ONOFF(unsigned char bit, unsigned char OnOff);
void LoadDiagram(void);	

void PAGE_LineScroll(unsigned char line);
void irregularPAGE_handler(void);
void refreshPAGE_handler(void);

void history_veriable_init(void);
void history_update(char history_eventCode);

const char* _readPGM_TEXT(PGM_P p);
const char* _TEXT(char const *format, ...);




#endif
