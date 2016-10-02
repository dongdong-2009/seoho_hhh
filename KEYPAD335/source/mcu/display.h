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



#define UP		0xFFFD
#define DN		0xFF7F
#define RIGHT	0xFFDF
#define LEFT		0xFFF7
#define ESC		0xFFFE
#define ENTER	0xFFFB
#define MENU	0xFFEF
#define RUN		0xFFBF
#define STOP		0xFEFF


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

extern unsigned char TimeTic_500ms;

extern unsigned int Temporary;

void DisplayInit(void);
void MainSYSTEM(void);
//void Menu(void);

//void Check_event(void);


void Point_inversion(unsigned char x, unsigned char y,  char cnt);
void Point_ONOFF(unsigned char bit, unsigned char OnOff);
void LoadDiagram(void);	

void PAGE_LineScroll(unsigned char line);
void irregularPAGE_handler(void);
void refreshPAGE_handler(void);


void SYS_password_clr(void);
void SYS_Base_KeyFunction(void);

const char* _readPGM_TEXT(PGM_P p);
const char* _cpy_flash2memory(char const *format);
const char* _TEXT(char const *format, ...);




#endif
