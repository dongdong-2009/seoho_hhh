//================================================================
//
// Copyright(C) 2008 HanYoung ELEC. Co., LTD.
// Designed by Paul B.H Park(idonno2000)
//
//
//	filename : display.c
//	description : glcd display
//
//	history :  - frist designed at 2008-05-28 4:02¿ÀÈÄ
//
//================================================================

#include "key.h"
#include "util.h"
#include "display.h"
//#include "128by64GLCD.H"
#include "16by2clcd.h"
#include "rs232_MMItoPANEL.h"

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
//#include "ds1307.h"

#include <avr/pgmspace.h>
#include <avr/eeprom.h>

#include "debug_printf.h"



unsigned long naviMENU,old_naveMENU; 
unsigned char RefreshFlag = 0;


char password[4]={0,0,0,0};
char _password[4]={0,0,0,0};

unsigned char pass_pos=0;

char Edit_flag = 0;
char posInpage=6;

char EnterFlag=0;
char RunFlag=0;
char StopFlag=0;

char setTime[7]; 
unsigned char Tset_pos=0;


char text_buf[256];

unsigned int eeprom_addr=0;

char waiting_flag=0;



void (*MenuDisplay)(void);
void (*LCDDisplay)(void);

void DisplayInit(void)
{
	naviMENU=1;
	old_naveMENU = 0xFF;
}



void irregularPAGE_handler(void)
{
	naviMENU = old_naveMENU;
}

prog_char  PAGE_ROOT[8][17]=
{
	"   Main  Menu   ",
	"M0 Operation    ",
	"M1 Drive Monitor",
	"M2 Parameter Edt",
	"M3 Auto Tuning  ",	
	"M4 Fault Record ",
	"M5 Initialize   ",
	"M6 Password     "
};


unsigned int edit_Temp=0;
void PAGE_1(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[0][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_ROOT[1][0]));
}
void PAGE_2(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[0][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_ROOT[2][0]));
}
void PAGE_3(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[0][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
}
void PAGE_4(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[0][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_ROOT[4][0]));
}
void PAGE_5(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[0][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_ROOT[5][0]));
}
void PAGE_6(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[0][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_ROOT[6][0]));
}
void PAGE_7(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[0][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_ROOT[7][0]));
}


prog_char  PAGE_DIR_1_X[6][17]={
	" 0 Local_Remot ",
	" 1 Dir Change  ",
	" 2 Speed Ref.  ",
	" 3 Freqe Ref.  ",
	" 4 Torque Ref. ",
	" 5 PID Ref.    "
};


void PAGE_1_0(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_X[0][0]));
	CLCD_string(0xC0,"   [0] Local    ");
	//CLCD_string(0xC0,(char*)_TEXT("%d",Temporary)); 
}

void PAGE_1_0_0(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_X[0][0]));
	CLCD_string(0xC0,"   [1] Remote   ");
}

void PAGE_1_1(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_X[1][0]));
	CLCD_string(0xC0,"  0  Forward   ");
}

void PAGE_1_1_0(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_X[1][0]));
	CLCD_string(0xC0,"  1  Reverse   ");
}

void PAGE_1_2(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_X[2][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u rpm ",edit_Temp));
}


void PAGE_1_3(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_X[3][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u Hz  ",edit_Temp));
}


void PAGE_1_4(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_X[4][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u Nm  ",edit_Temp));
}


void PAGE_1_5(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_X[5][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u %   ",edit_Temp));
}





prog_char  PAGE_DIR_2_X[3][17]={
	" 0 Opr Status   ",
	" 1 Terminal IO  ",
	" 2 Drive Inform "
};
void PAGE_2_0(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[2][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_2_X[0][0]));
}
void PAGE_2_1(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[2][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_2_X[1][0]));
}
void PAGE_2_2(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[2][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_2_X[2][0]));
}




prog_char  PAGE_DIR_2_0_XX[14][17]={
	" 0 Motor Speed  ",
	" 1 Output Freq  ",
	" 2 DC_Link Vtg  ",
	" 3 Motor Currnt ",
	" 4 Output Vtg   ",
	" 5 Actual Trq   ",
	" 6 Torqu Currnt ",
	" 7 Flux Current ",
	" 8 Input Power  ",
	" 9 Output Power ",
	"10 PID Reference",
	"11 PID FeedBack ",
	" 12 PID Error   ",
	" 13 Temperature ",
};

void PAGE_2_0_00(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_0_XX[0][0]));
	CLCD_string(0xC0,"             rpm");
}
void PAGE_2_0_01(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_0_XX[1][0]));
	CLCD_string(0xC0,"              Hz");
}
void PAGE_2_0_02(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_0_XX[2][0]));
	CLCD_string(0xC0,"             Vdc");
}
void PAGE_2_0_03(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_0_XX[3][0]));
	CLCD_string(0xC0,"            Arms");
}
void PAGE_2_0_04(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_0_XX[4][0]));
	CLCD_string(0xC0,"            Arms");
}
void PAGE_2_0_05(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_0_XX[5][0]));
	CLCD_string(0xC0,"              Nm");
}
void PAGE_2_0_06(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_0_XX[6][0]));
	CLCD_string(0xC0,"               A");
}
void PAGE_2_0_07(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_0_XX[7][0]));
	CLCD_string(0xC0,"               A");
}
void PAGE_2_0_08(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_0_XX[8][0]));
	CLCD_string(0xC0,"              kW");
}
void PAGE_2_0_09(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_0_XX[9][0]));
	CLCD_string(0xC0,"              kW");
}
void PAGE_2_0_10(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_0_XX[10][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_2_0_11(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_0_XX[11][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_2_0_12(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_0_XX[12][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_2_0_13(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_0_XX[13][0]));
	CLCD_string(0xC0,"              'C");
}



prog_char  PAGE_DIR_2_1_XX[5][17]={
" 0 DI[8......1] ",
" 1 DO[3.1]      ",
"2 Analog Input1 ",
"3 Analog Input2 ",
"4 Analog Output "
};


void PAGE_2_1_00(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_1_XX[0][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_2_1_01(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_1_XX[1][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_2_1_02(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_1_XX[2][0]));
	CLCD_string(0xC0,"         V or mA");
}
void PAGE_2_1_03(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_1_XX[3][0]));
	CLCD_string(0xC0,"         V or mA");
}
void PAGE_2_1_04(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_1_XX[4][0]));
	CLCD_string(0xC0,"              mA");
}




prog_char  PAGE_DIR_2_2_XX[9][17]={
" 0 Motor Sel    ",
"1 Control Method",
" 2 RUN/STOP Src ",
" 3 Ref Method   ",
" 4 Drive Power  ",
" 5 Drive Voltag ",
" 6 Option Card  ",
" 7 Software Ver ",
" 8 Software Opt ",
};

void PAGE_2_2_00(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_2_XX[0][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_2_2_01(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_2_XX[1][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_2_2_02(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_2_XX[2][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_2_2_03(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_2_XX[3][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_2_2_04(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_2_XX[4][0]));
	CLCD_string(0xC0,"              kW");
}
void PAGE_2_2_05(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_2_XX[5][0]));
	CLCD_string(0xC0,"               V");
}
void PAGE_2_2_06(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_2_XX[6][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_2_2_07(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_2_XX[7][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_2_2_08(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_2_XX[8][0]));
	CLCD_string(0xC0,"                ");
}


prog_char  PAGE_DIR_3_XX[25][17]={
	" 0 Program Ctrl ",
	"1 Ctrl Setup[M1]",
	"2 Ctrl Setup[M2]",
	"3 Ref Setup[M1] ",
	"4 Ref Setup[M2] ",
	" 5 Protection   ",
	" 6 Analog Input ",
	" 7 PID Control  ",
	"8 Digital Input ",
	"9  MultiStep[M1]",
	"10 MultiStep[M2]",
	"11 Analog Output",
	"12 DigitalOutput",
	"13 Motor Brake  ",
	"14 Auto Tuning  ",
	"15 V/F Ctrl[M1] ",
	"16 V/F Ctrl[M2] ",
	"17 SL Vector[M1]",
	"18 SL Vector[M2]",
	" 19 Vector[M1]  ",
	" 20 Vector[M2]  ",
	" 21 M1 Constant ",
	" 22 M2 Constant ",
	"23 Comm Control ",
	"24 Monitor Setup"
};

void PAGE_3_00(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX[0][0]));
}
void PAGE_3_01(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX[1][0]));
}
void PAGE_3_02(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX[2][0]));
}
void PAGE_3_03(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX[3][0]));
}
void PAGE_3_04(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX[4][0]));
}
void PAGE_3_05(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX[5][0]));
}
void PAGE_3_06(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX[6][0]));
}
void PAGE_3_07(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX[7][0]));
}
void PAGE_3_08(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX[8][0]));
}
void PAGE_3_09(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX[9][0]));
}
void PAGE_3_10(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX[10][0]));
}
void PAGE_3_11(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX[11][0]));
}
void PAGE_3_12(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX[12][0]));
}
void PAGE_3_13(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX[13][0]));
}
void PAGE_3_14(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX[14][0]));
}
void PAGE_3_15(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX[15][0]));
}
void PAGE_3_16(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX[16][0]));
}
void PAGE_3_17(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX[17][0]));
}
void PAGE_3_18(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX[18][0]));
}
void PAGE_3_19(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX[19][0]));
}
void PAGE_3_20(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX[20][0]));
}
void PAGE_3_21(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX[21][0]));
}
void PAGE_3_22(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX[22][0]));
}
void PAGE_3_23(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX[23][0]));
}
void PAGE_3_24(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX[24][0]));
}



prog_char  PAGE_DIR_3_00_XX[16][17]={
	"P0.0 Ini. Flag  ",
	"P0.1 Prog.Key 1 ",
	"P0.2 Prog.Key 2 ",
	"P0.3 Prog.Key 3 ",
	"P0.4 PWD (L1)   ",
	"P0.5 PWD (L2)   ",
	"P0.6 PWD (L3)   ",
	"P0.7 PWD (L4)   ",
	"P0.8 PWD (L5)   ",
	"P0.9 Prog_Key 4 ",
	"P0.10 Prog_Key 5",
	"P0.11 Prog_Key 6",
	"P0.12 Parlni_Key",
	"P0.13 Volt_Clas ",
	"P0.14 Freq_Class",
	"P0.15 ThermalMon"
};


void PAGE_3_00_00(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_00_XX[0][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_00_01(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_00_XX[1][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_00_02(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_00_XX[2][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_00_03(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_00_XX[3][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_00_04(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_00_XX[4][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_00_05(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_00_XX[5][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_00_06(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_00_XX[6][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_00_07(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_00_XX[7][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_00_08(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_00_XX[8][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_00_09(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_00_XX[9][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_00_10(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_00_XX[10][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_00_11(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_00_XX[11][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_00_12(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_00_XX[12][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_00_13(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_00_XX[13][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_00_14(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_00_XX[14][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_00_15(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_00_XX[15][0]));
	CLCD_string(0xC0,"                ");
}

prog_char  PAGE_DIR_3_01_XX[10][17]={
"P1.0 M1_Rtd_Pwr ",
"P1.1 M1_Rtd_Volt",
"P1.2 M1_Rtd_Curr",
"P1.3 M1_Rtd_Freq",
"P1.4 M1_Pole    ",
"P1.5 M1_Rtd_Spd ",
"P1.6 M1_Control ",
"P1.7 M1_PWM_freq",
"P1.8 M1_AT_PWM_f",
"P1.9 Supply_Volt"
};


void PAGE_3_01_00(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_01_XX[0][0]));
	CLCD_string(0xC0,(char*)_TEXT("      % 3u.% 1u kW  ",(edit_Temp/10),(edit_Temp%10)));
}
void PAGE_3_01_01(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_01_XX[1][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u Vrms",edit_Temp));
}
void PAGE_3_01_02(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_01_XX[2][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u Arms",edit_Temp));
}
void PAGE_3_01_03(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_01_XX[3][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u Hz   ",edit_Temp));
}
void PAGE_3_01_04(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_01_XX[4][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u pole",edit_Temp));
}
void PAGE_3_01_05(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_01_XX[5][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u rpm ",edit_Temp));
}
void PAGE_3_01_06(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_01_XX[6][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_01_07(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_01_XX[7][0]));
	CLCD_string(0xC0,(char*)_TEXT("      % 4u kHz ",edit_Temp));
}
void PAGE_3_01_08(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_01_XX[8][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_01_09(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_01_XX[9][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u Vrms",edit_Temp));
}


prog_char  PAGE_DIR_3_02_XX[10][17]={
"P2.0 M2_Rtd_Pwr ",
"P2.1 M2_Rtd_Volt",
"P2.2 M2_Rtd_Curr",
"P2.3 M2_Rtd_Freq",
"P2.4 M2_Pole    ",
"P2.5 M2_Rtd_Spd ",
"P2.6 M2_Control ",
"P2.7 M2_PWM_freq",
"P2.8 M2_AT_PWM_f",
"P2.9 Supply_Volt"
};



void PAGE_3_02_00(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_02_XX[0][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_02_01(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_02_XX[1][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_02_02(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_02_XX[2][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_02_03(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_02_XX[3][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_02_04(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_02_XX[4][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_02_05(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_02_XX[5][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_02_06(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_02_XX[6][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_02_07(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_02_XX[7][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_02_08(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_02_XX[8][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_02_09(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_02_XX[9][0]));
	CLCD_string(0xC0,"                ");
}


prog_char  PAGE_DIR_3_03_XX[53][17]={
"P3.0 RUN/STOP   ",
"P3.1 Ramp_Input ",
"P3.2 STOP Detect",
"P3.3 STOP Mode  ",
"P3.4 StopHold_Tm",
"P3.5 Out_Off_Tm ",
"P3.6 Out_Off_Ref",
"P3.7 Acc/Dec_En ",
"P3.8 AccTm_Range",
"P3.9 AccSw 1-2  ",
"P3.10 AccSw 2-3 ",
"P3.11 AccSw 3-4 ",
"P3.12 AccSw 4-5 ",
"P3.13 AccSw 5-6 ",
"P3.14 AccSw 6-7 ",
"P3.15 AccSw 7-8 ",
"P3.16 Acc_Tm I.1",
"P3.17 Acc_Tm I.2",
"P3.18 Acc_Tm I.3",
"P3.19 Acc_Tm I.4",
"P3.20 Acc_Tm I.5",
"P3.21 Acc_Tm I.6",
"P3.22 Acc_Tm I.7",
"P3.23 Acc_Tm I.8",
"P3.24 Acc_Tm II ",
"P3.25 DecTm_Rng ",
"P3.26 DecSw 1-2 ",
"P3.27 DecSw 2-3 ",
"P3.28 DecSw 3-4 ",
"P3.29 DecSw 4-5 ",
"P3.30 DecSw 5-6 ",
"P3.31 DecSw 6-7 ",
"P3.32 DecSw 7-8 ",
"P3.33 Dec_Tm I.1",
"P3.34 Dec_Tm I.2",
"P3.35 Dec_Tm I.3",
"P3.36 Dec_Tm I.4",
"P3.37 Dec_Tm I.5",
"P3.38 Dec_Tm I.6",
"P3.39 Dec_Tm I.7",
"P3.40 Dec_Tm I.8",
"P3.41 Dec_Tm II ",
"P3.42 C_Decel_En",
"P3.43 C_Decel_Tm",
"P3.44 S-Curve_En",
"P3.45 S-CrvAcc_S",
"P3.46 S-CrvAcc_E",
"P3.47 S-CrvDec_S",
"P3.48 S-CrvDec_E",
"P3.49 ES_Mode   ",
"P3.50 ES_DecTime",
"P3.51 CONTINU_OP",
"P3.52 Rev_DIR_En"
};



void PAGE_3_03_00(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[0][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_01(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[1][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_02(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[2][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_03(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[3][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_04(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[4][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_05(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[5][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_06(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[6][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_07(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[7][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_08(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[8][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_09(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[9][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_10(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[10][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_11(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[11][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_12(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[12][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_13(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[13][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_14(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[14][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_15(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[15][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_16(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[16][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_17(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[17][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_18(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[18][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_19(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[19][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_20(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[20][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_21(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[21][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_22(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[22][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_23(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[23][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_24(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[24][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_25(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[25][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_26(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[26][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_27(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[27][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_28(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[28][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_29(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[29][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_30(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[30][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_31(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[31][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_32(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[32][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_33(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[33][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_34(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[34][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_35(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[35][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_36(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[36][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_37(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[37][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_38(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[38][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_39(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[39][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_40(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[40][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_41(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[41][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_42(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[42][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_43(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[43][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_44(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[44][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_45(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[45][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_46(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[46][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_47(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[47][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_48(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[48][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_49(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[49][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_50(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[50][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_51(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[51][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_03_52(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_03_XX[52][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}


prog_char  PAGE_DIR_3_04_XX[53][17]={
"P4.0 RUN/STOP   ",
"P4.1 Ramp_Input ",
"P4.2 STOP Detect",
"P4.3 STOP Mode  ",
"P4.4 StopHold_Tm",
"P4.5 Out_Off_Tm ",
"P4.6 Out_Off_Ref",
"P4.7 Acc/Dec_En ",
"P4.8 AccTm_Range",
"P4.9 AccSw 1-2  ",
"P4.10 AccSw 2-3 ",
"P4.11 AccSw 3-4 ",
"P4.12 AccSw 4-5 ",
"P4.13 AccSw 5-6 ",
"P4.14 AccSw 6-7 ",
"P4.15 AccSw 7-8 ",
"P4.16 Acc_Tm I.1",
"P4.17 Acc_Tm I.2",
"P4.18 Acc_Tm I.3",
"P4.19 Acc_Tm I.4",
"P4.20 Acc_Tm I.5",
"P4.21 Acc_Tm I.6",
"P4.22 Acc_Tm I.7",
"P4.23 Acc_Tm I.8",
"P4.24 Acc_Tm II ",
"P4.25 DecTm_Rng ",
"P4.26 DecSw 1-2 ",
"P4.27 DecSw 2-3 ",
"P4.28 DecSw 3-4 ",
"P4.29 DecSw 4-5 ",
"P4.30 DecSw 5-6 ",
"P4.31 DecSw 6-7 ",
"P4.32 DecSw 7-8 ",
"P4.33 Dec_Tm I.1",
"P4.34 Dec_Tm I.2",
"P4.35 Dec_Tm I.3",
"P4.36 Dec_Tm I.4",
"P4.37 Dec_Tm I.5",
"P4.38 Dec_Tm I.6",
"P4.39 Dec_Tm I.7",
"P4.40 Dec_Tm I.8",
"P4.41 Dec_Tm II ",
"P4.42 C_Decel_En",
"P4.43 C_Decel_Tm",
"P4.44 S-Curve_En",
"P4.45 S-CrvAcc_S",
"P4.46 S-CrvAcc_E",
"P4.47 S-CrvDec_S",
"P4.48 S-CrvDec_E",
"P4.49 ES_Mode   ",
"P4.50 ES_DecTime",
"P4.51 CONTINU_OP",
"P4.52 Rev_DIR_En"
};



void PAGE_3_04_00(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[0][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_01(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[1][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_02(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[2][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_03(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[3][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_04(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[4][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_05(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[5][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_06(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[6][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_07(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[7][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_08(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[8][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_09(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[9][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_10(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[10][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_11(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[11][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_12(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[12][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_13(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[13][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_14(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[14][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_15(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[15][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_16(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[16][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_17(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[17][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_18(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[18][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_19(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[19][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_20(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[20][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_21(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[21][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_22(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[22][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_23(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[23][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_24(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[24][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_25(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[25][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_26(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[26][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_27(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[27][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_28(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[28][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_29(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[29][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_30(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[30][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_31(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[31][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_32(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[32][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_33(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[33][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_34(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[34][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_35(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[35][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_36(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[36][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_37(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[37][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_38(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[38][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_39(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[39][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_40(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[40][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_41(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[41][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_42(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[42][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_43(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[43][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_44(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[44][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_45(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[45][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_46(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[46][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_47(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[47][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_48(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[48][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_49(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[49][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_50(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[50][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_51(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[51][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_04_52(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_04_XX[52][0]));
	CLCD_string(0xC0,"                ");
}

prog_char  PAGE_DIR_3_05_XX[45][17]={
	"P5.0 I_Limit[M1]",
	"P5.1 I_Limit[M2]",
	"P5.2 Reserved   ",
	"P5.3 OL3 Ref    ",
	"P5.4 OL3 Time   ",
	"P5.5 OL2 Ref    ",
	"P5.6 OL2 Time   ",
	"P5.7 MaxCon_Curr",
	"P5.8 Over_Load  ",
	"P5.9 OL_TimeOver",
	"P5.10 OL_Action ",
	"P5.11 OC_Trip_M1",
	"P5.12 ZC_Trip   ",
	"P5.13 OV_Ltd_Fn ",
	"P5.14 OV Limit  ",
	"P5.15 OV Trip   ",
	"P5.16 UV_Comp_Fn",
	"P5.17 UV_Comp_V ",
	"P5.18 UV_Trip   ",
	"P5.19 OP_Ph_Trip",
	"P5.20 Input_Freq",
	"P5.21 Blt-in_DB ",
	"P5.22 DB_Freq   ",
	"P5.23 DB_Start_V",
	"P5.24 DB_Full_V ",
	"P5.25 OT_Action ",
	"P5.26 HOV_Protec",
	"P5.27 HOC_Protec",
	"P5.28 HZC_Protec",
	"P5.29 Drv_OL_Fn ",
	"P5.30 RestartCnt",
	"P5.31 Retry_Dly ",
	"P5.32 A.Rst_OC  ",
	"P5.33 A.Rst_OV  ",
	"P5.34 A.Rst_UV  ",
	"P5.35 A.Rst_HOV ",
	"P5.36 A.Rst_HOC ",
	"P5.37 A.Rst_CnEr",
	"P5.38 CtrlErr_Tm",
	"P5.39 Ctrl_Err_I",
	"P5.40 Over_Temp ",
	"P5.41 OC_Trip_M2",
	"P5.42 Ln_UV     ",
	"P5.43 Rst_Ln_UV ",
	"P5.44 Rst_Ln_UB "
};

void PAGE_3_05_00(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[0][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u  %  ",edit_Temp));
}
void PAGE_3_05_01(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[1][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_05_02(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[2][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_05_03(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[3][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_05_04(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[4][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_05_05(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[5][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_05_06(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[6][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_05_07(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[7][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u  %  ",edit_Temp));
}
void PAGE_3_05_08(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[8][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u  %  ",edit_Temp));
}
void PAGE_3_05_09(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[9][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_05_10(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[10][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_05_11(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[11][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u  %  ",edit_Temp));
}
void PAGE_3_05_12(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[12][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u  %  ",edit_Temp));
}
void PAGE_3_05_13(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[13][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_05_14(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[14][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u  V  ",edit_Temp));
}
void PAGE_3_05_15(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[15][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u  V  ",edit_Temp));
}
void PAGE_3_05_16(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[16][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_05_17(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[17][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_05_18(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[18][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u  V  ",edit_Temp));
}
void PAGE_3_05_19(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[19][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_05_20(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[20][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_05_21(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[21][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_05_22(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[22][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_05_23(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[23][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_05_24(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[24][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_05_25(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[25][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_05_26(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[26][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_05_27(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[27][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_05_28(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[28][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_05_29(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[29][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_05_30(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[30][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_05_31(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[31][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_05_32(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[32][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_05_33(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[33][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_05_34(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[34][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_05_35(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[35][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_05_36(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[36][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_05_37(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[37][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_05_38(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[38][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u  s  ",edit_Temp));
}
void PAGE_3_05_39(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[39][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_05_40(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[40][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_05_41(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[41][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_05_42(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[42][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_05_43(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[43][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_05_44(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_05_XX[44][0]));
	CLCD_string(0xC0,"                ");
}

prog_char  PAGE_DIR_3_06_XX[71][17]={
	"P6.0 AI_Ref_Src ",
	"P6.1 AI1 Func.  ",
	"P6.2 AI1 Type   ",
	"P6.3 AI1 Filter ",
	"P6.4 AI1 Tm_Ct  ",
	"P6.5 AI1 Offset ",
	"P6.6 AI1 Min_V  ",
	"P6.7 AI1 Min_mA ",
	"P6.8 AI1 Min.   ",
	"P6.9 AI1 Max_V  ",
	"P6.10 AI1 Mx_mA ",
	"P6.11 AI1 Max.  ",
	"P6.12 AI1 Inv.  ",
	"P6.13 AI1 D_Step",
	"P6.14 AI1 Dead-Z",
	"P6.15 AI2 Func. ",
	"P6.16 AI2 Type  ",
	"P6.17 AI2 Filter",
	"P6.18 AI2 Tm_Ct ",
	"P6.19 AI2 Offset",
	"P6.20 AI2 Min_V ",
	"P6.21 AI2 Min_mA",
	"P6.22 AI2 Min.  ",
	"P6.23 AI2 Max_V ",
	"P6.24 AI2 Mx_mA ",
	"P6.25 AI2 Max.  ",
	"P6.26 AI2 Inv.  ",
	"P6.27 AI2 D_Step",
	"P6.28 AI2 Dead-Z",
	"P6.29 AI3 Func. ",
	"P6.30 AI3 Type  ",
	"P6.31 AI3 Filter",
	"P6.32 AI3 Tm_Ct ",
	"P6.33 AI3 Offset",
	"P6.34 AI3 Min_V ",
	"P6.35 AI3 Min_mA",
	"P6.36 AI3 Min.  ",
	"P6.37 AI3 Max_V ",
	"P6.38 AI3 Mx_mA ",
	"P6.39 AI3 Max.  ",
	"P6.40 AI3 Inv.  ",
	"P6.41 AI3 D_Step",
	"P6.42 AI3 Dead-Z",
	"P6.43 AI4 Func. ",
	"P6.44 AI4 Type  ",
	"P6.45 AI4 Filter",
	"P6.46 AI4 Tm_Ct ",
	"P6.47 AI4 Offset",
	"P6.48 AI4 Min_V ",
	"P6.49 AI4 Min_mA",
	"P6.50 AI4 Min.  ",
	"P6.51 AI4 Max_V ",
	"P6.52 AI4 Mx_mA ",
	"P6.53 AI4 Max.  ",
	"P6.54 AI4 Inv.  ",
	"P6.55 AI4 D_Step",
	"P6.56 AI4 Dead-Z",
	"P6.57 AI5 Func. ",
	"P6.58 AI5 Type  ",
	"P6.59 AI5 Filter",
	"P6.60 AI5 Tm_Ct ",
	"P6.61 AI5 Offset",
	"P6.62 AI5 Min_V ",
	"P6.63 AI5 Min_mA",
	"P6.64 AI5 Min.  ",
	"P6.65 AI5 Max_V ",
	"P6.66 AI5 Mx_mA ",
	"P6.67 AI5 Max.  ",
	"P6.68 AI5 Inv.  ",
	"P6.69 AI5 D_Step",
	"P6.70 AI5 Dead-Z"
};

void PAGE_3_06_00(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[0][0]));
	CLCD_string(0xC0,"                ");
	//CLCD_string(0xC0,(char*)_TEXT("-Page%lu-",naviMENU));
}
void PAGE_3_06_01(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[1][0]));
	CLCD_string(0xC0,"                ");
	//CLCD_string(0xC0,(char*)_TEXT("-Page%lu-",naviMENU));
}
void PAGE_3_06_02(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[2][0]));
	CLCD_string(0xC0,"                ");
	//CLCD_string(0xC0,(char*)_TEXT("-Page%lu-",naviMENU));
}
void PAGE_3_06_03(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[3][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_04(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[4][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_05(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[5][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_06(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[6][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_07(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[7][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_08(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[8][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_09(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[9][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_10(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[10][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_11(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[11][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_12(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[12][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_13(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[13][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_14(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[14][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_15(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[15][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_16(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[16][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_17(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[17][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_18(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[18][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_19(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[19][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_20(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[20][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_21(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[21][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_22(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[22][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_23(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[23][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_24(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[24][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_25(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[25][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_26(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[26][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_27(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[27][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_28(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[28][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_29(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[29][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_30(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[30][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_31(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[31][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_32(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[32][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_33(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[33][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_34(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[34][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_35(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[35][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_36(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[36][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_37(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[37][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_38(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[38][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_39(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[39][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_40(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[40][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_41(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[41][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_42(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[42][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_43(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[43][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_44(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[44][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_45(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[45][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_46(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[46][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_47(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[47][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_48(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[48][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_49(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[49][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_50(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[50][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_51(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[51][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_52(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[52][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_53(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[53][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_54(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[54][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_55(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[55][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_56(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[56][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_57(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[57][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_58(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[58][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_59(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[59][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_60(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[60][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_61(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[61][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_62(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[62][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_63(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[63][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_64(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[64][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_65(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[65][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_66(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[66][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_67(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[67][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_68(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[68][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_69(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[69][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_06_70(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_06_XX[70][0]));
	CLCD_string(0xC0,"                ");
}

prog_char  PAGE_DIR_3_07_XX[29][17]={
"P7.0 PID_Mode   ",
"P7.1 Reference  ",
"P7.2 Fixed_SetPt",
"P7.3 Feedback   ",
"P7.4 REF_Sgn_Neg",
"P7.5 FB_Sgn_Neg ",
"P7.6 Cntl Period",
"P7.7 P-Gain     ",
"P7.8 Integ_Time ",
"P7.9 Diff_Time  ",
"P7.10 FF-Gain   ",
"P7.11 ZERO_Adj 1",
"P7.12 P-Gain 2  ",
"P7.13 Int_Time 2",
"P7.14 Dif_Time 2",
"P7.15 FF-Gain 2 ",
"P7.16 ZERO_Adj 2",
"P7.17 Output_INV",
"P7.18 Int_Lo_Lmt",
"P7.19 Int_Up_Lmt",
"P7.20 Out_Lo_Lmt",
"P7.21 Out_Up_Lmt",
"P7.22 Out_Scale ",
"P7.23 Int_St_Val",
"P7.24 Auto_RN_ST",
"P7.25 AutoSt_Dly",
"P7.26 AutoSt_Err",
"P7.27 Ref_Fn_Src",
"P7.28 Fbk_Fn_Src",
};

void PAGE_3_07_00(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_07_XX[0][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_07_01(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_07_XX[1][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_07_02(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_07_XX[2][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_07_03(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_07_XX[3][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_07_04(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_07_XX[4][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_07_05(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_07_XX[5][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_07_06(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_07_XX[6][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_07_07(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_07_XX[7][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_07_08(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_07_XX[8][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_07_09(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_07_XX[9][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_07_10(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_07_XX[10][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_07_11(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_07_XX[11][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_07_12(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_07_XX[12][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_07_13(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_07_XX[13][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_07_14(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_07_XX[14][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_07_15(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_07_XX[15][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_07_16(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_07_XX[16][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_07_17(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_07_XX[17][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_07_18(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_07_XX[18][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_07_19(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_07_XX[19][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_07_20(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_07_XX[20][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_07_21(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_07_XX[21][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_07_22(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_07_XX[22][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_07_23(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_07_XX[23][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_07_24(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_07_XX[24][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_07_25(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_07_XX[25][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_07_26(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_07_XX[26][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_07_27(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_07_XX[27][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_07_28(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_07_XX[28][0]));
	CLCD_string(0xC0,"                ");
}

prog_char  PAGE_DIR_3_08_XX[20][17]={
"P8.0 RUN/STOP   ",
"P8.1 DI.3 Func. ",
"P8.2 DI.4 Func. ",
"P8.3 DI.5 Func. ",
"P8.4 DI.6 Func. ",
"P8.5 DI.7 Func. ",
"P8.6 DI.8 Func. ",
"P8.7 DI.9 Func. ",
"P8.8 DI.10 Func.",
"P8.9 DI.11 Func.",
"P8.10 DI.12 Func",
"P8.11 DI.13 Func",
"P8.12 DI.14 Func",
"P8.13 DI.15 Func",
"P8.14 DI.16 Func",
"P8.15 M.C. Blank",
"P8.16 Ref_UP/DN ",
"P8.17 Fly_START ",
"P8.18 RUN_Delay ",
"P8.19 Inching_Tm"
};
void PAGE_3_08_00(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_08_XX[0][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_08_01(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_08_XX[1][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_08_02(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_08_XX[2][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_08_03(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_08_XX[3][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_08_04(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_08_XX[4][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_08_05(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_08_XX[5][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_08_06(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_08_XX[6][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_08_07(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_08_XX[7][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_08_08(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_08_XX[8][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_08_09(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_08_XX[9][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_08_10(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_08_XX[10][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_08_11(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_08_XX[11][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_08_12(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_08_XX[12][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_08_13(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_08_XX[13][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_08_14(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_08_XX[14][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_08_15(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_08_XX[15][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_08_16(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_08_XX[16][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_08_17(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_08_XX[17][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_08_18(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_08_XX[18][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_08_19(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_08_XX[19][0]));
	CLCD_string(0xC0,"                ");
}

prog_char  PAGE_DIR_3_09_XX[17][17]={
"P9.0 JOG_SetPt  ",
"P9.1 Step 1     ",
"P9.2 Step 2     ",
"P9.3 Step 3     ",
"P9.4 Step 4     ",
"P9.5 Step 5     ",
"P9.6 Step 6     ",
"P9.7 Step 7     ",
"P9.8 Step 8     ",
"P9.9 Step 9     ",
"P9.10 Step 10   ",
"P9.11 Step 11   ",
"P9.12 Step 12   ",
"P9.13 Step 13   ",
"P9.14 Step 14   ",
"P9.15 Step 15   ",
"P9.16 Unit[%/Hz]"
};
void PAGE_3_09_00(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_09_XX[0][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_09_01(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_09_XX[1][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_09_02(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_09_XX[2][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_09_03(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_09_XX[3][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_09_04(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_09_XX[4][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_09_05(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_09_XX[5][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_09_06(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_09_XX[6][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_09_07(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_09_XX[7][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_09_08(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_09_XX[8][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_09_09(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_09_XX[9][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_09_10(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_09_XX[10][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_09_11(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_09_XX[11][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_09_12(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_09_XX[12][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_09_13(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_09_XX[13][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_09_14(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_09_XX[14][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_09_15(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_09_XX[15][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_09_16(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_09_XX[16][0]));
	CLCD_string(0xC0,"                ");
}


prog_char  PAGE_DIR_3_10_XX[17][17]={
"P10.0 JOG_SetPt ",
"P10.1 Step 1    ",
"P10.2 Step 2    ",
"P10.3 Step 3    ",
"P10.4 Step 4    ",
"P10.5 Step 5    ",
"P10.6 Step 6    ",
"P10.7 Step 7    ",
"P10.8 Step 8    ",
"P10.9 Step 9    ",
"P10.10 Step 10  ",
"P10.11 Step 11  ",
"P10.12 Step 12  ",
"P10.13 Step 13  ",
"P10.14 Step 14  ",
"P10.15 Step 15  ",
"P10.16 Unit_Sel "
};

void PAGE_3_10_00(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_10_XX[0][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_10_01(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_10_XX[1][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_10_02(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_10_XX[2][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_10_03(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_10_XX[3][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_10_04(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_10_XX[4][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_10_05(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_10_XX[5][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_10_06(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_10_XX[6][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_10_07(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_10_XX[7][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_10_08(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_10_XX[8][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_10_09(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_10_XX[9][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_10_10(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_10_XX[10][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_10_11(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_10_XX[11][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_10_12(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_10_XX[12][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_10_13(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_10_XX[13][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_10_14(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_10_XX[14][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_10_15(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_10_XX[15][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_10_16(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_10_XX[16][0]));
	CLCD_string(0xC0,"                ");
}

prog_char  PAGE_DIR_3_11_XX[7][17]={
"P11.0 AO1 Output",
"P11.1 AO1 Type  ",
"P11.2 AO1 0_Adj ",
"P11.3 AO1 4_Adj ",
"P11.4 AO1 20_Adj",
"P11.5 AO1 Scale ",
"P11.6 AO1 Inv.  "
};
void PAGE_3_11_00(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_11_XX[0][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_11_01(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_11_XX[1][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_11_02(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_11_XX[2][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_11_03(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_11_XX[3][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_11_04(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_11_XX[4][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_11_05(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_11_XX[5][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_11_06(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_11_XX[6][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_11_07(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_11_XX[7][0]));
	CLCD_string(0xC0,"                ");
}
prog_char  PAGE_DIR_3_12_XX[8][17]={
"P12.0 DO.1 Func ",
"P12.1 DO.2 Func ",
"P12.2 DO.3 Func ",
"P12.3 DO.4 Func ",
"P12.4 DO.5 Func ",
"P12.5 DO.6 Func ",
"P12.6 DO.7 Func ",
"P12.7 DO.8 Func "
};
void PAGE_3_12_00(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_12_XX[0][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_12_01(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_12_XX[1][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_12_02(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_12_XX[2][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_12_03(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_12_XX[3][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_12_04(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_12_XX[4][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_12_05(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_12_XX[5][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_12_06(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_12_XX[6][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_12_07(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_12_XX[7][0]));
	CLCD_string(0xC0,"                ");
}


prog_char  PAGE_DIR_3_13_XX[12][17]={
"P13.0 B1_OP_RefU",
"P13.1 B1_OP_RefD",
"P13.2 B1_OP_I   ",
"P13.3 B1_OP_Time",
"P13.4 B1_CL_Spd ",
"P13.5 B1_Trq_Tm ",
"P13.6 B2_OP_RefU",
"P13.7 B2_OP_RefD",
"P13.8 B2_OP_I   ",
"P13.9 B2_OP_Time",
"P13.10 B2_CL_Spd",
"P13.11 B2_Trq_Tm"
};
void PAGE_3_13_00(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_13_XX[0][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_13_01(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_13_XX[1][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_13_02(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_13_XX[2][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_13_03(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_13_XX[3][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_13_04(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_13_XX[4][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_13_05(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_13_XX[5][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_13_06(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_13_XX[6][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_13_07(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_13_XX[7][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_13_08(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_13_XX[8][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_13_09(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_13_XX[9][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_13_10(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_13_XX[10][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_13_11(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_13_XX[11][0]));
	CLCD_string(0xC0,"                ");
}
prog_char  PAGE_DIR_3_14_XX[9][17]={
"P14.0 Tuning_Con",
"P14.1 Excit_Slip",
"P14.2 Tune_Spd_L",
"P14.3 Tune_Spd_H",
"P14.4 HFI_Freq  ",
"P14.5 HFI_Curr  ",
"P14.6 Exc_StCurr",
"P14.7 Excit_Flux",
"P14.8 Excit_Freq"
};
void PAGE_3_14_00(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_14_XX[0][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_14_01(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_14_XX[1][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_14_02(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_14_XX[2][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_14_03(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_14_XX[3][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_14_04(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_14_XX[4][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_14_05(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_14_XX[5][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_14_06(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_14_XX[6][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_14_07(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_14_XX[7][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_14_08(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_14_XX[8][0]));
	CLCD_string(0xC0,"                ");
}
prog_char  PAGE_DIR_3_15_XX[29][17]={
"P15.0 Trq_Comp  ",
"P15.1 Min_Freq  ",
"P15.2 Max_Freq  ",
"P15.3 Flux_Out_I",
"P15.4 Trq_Out_Tm",
"P15.5 Spd_Det_Tm",
"P15.6 V/F Curve ",
"P15.7 Zr_Freq_V ",
"P15.8 Mid_Freq  ",
"P15.9 Mid_Freq_V",
"P15.10 Max_V_Frq",
"P15.11 Max_Volt ",
"P15.12 Max_V_Ltd",
"P15.13 ff_V_Comp",
"P15.14 Sq_Crv_V ",
"P15.15 St_Brk_Tm",
"P15.16 St_Brk_B ",
"P15.17 St_Brk_I ",
"P15.18 Sp_Brk_Tm",
"P15.19 Sp_Brk_B ",
"P15.20 Sp_Brk_Ih",
"P15.21 Sp_Brk_Is",
"P15.22 CC P-Gain",
"P15.23 CC I-Gain",
"P15.24 StbT_Cons",
"P15.25 Stb_Gain ",
"P15.26 Stb_Limit",
"P15.27 Unity_I_f",
"P15.28 Acc_OC_Gn"
};
void PAGE_3_15_00(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_15_XX[0][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_15_01(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_15_XX[1][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_15_02(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_15_XX[2][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_15_03(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_15_XX[3][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_15_04(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_15_XX[4][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_15_05(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_15_XX[5][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_15_06(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_15_XX[6][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_15_07(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_15_XX[7][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_15_08(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_15_XX[8][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_15_09(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_15_XX[9][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_15_10(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_15_XX[10][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_15_11(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_15_XX[11][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_15_12(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_15_XX[12][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_15_13(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_15_XX[13][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_15_14(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_15_XX[14][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_15_15(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_15_XX[15][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_15_16(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_15_XX[16][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_15_17(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_15_XX[17][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_15_18(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_15_XX[18][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_15_19(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_15_XX[19][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_15_20(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_15_XX[20][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_15_21(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_15_XX[21][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_15_22(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_15_XX[22][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_15_23(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_15_XX[23][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_15_24(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_15_XX[24][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_15_25(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_15_XX[25][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_15_26(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_15_XX[26][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_15_27(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_15_XX[27][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_15_28(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_15_XX[28][0]));
	CLCD_string(0xC0,"                ");
}
prog_char  PAGE_DIR_3_16_XX[29][17]={
"P16.0 Trq_Comp  ",
"P16.1 Min_Freq  ",
"P16.2 Max_Freq  ",
"P16.3 Flux_Out_I",
"P16.4 Trq_Out_Tm",
"P16.5 Spd_Det_Tm",
"P16.6 V/F Curve ",
"P16.7 Zr_Freq_V ",
"P16.8 Mid_Freq  ",
"P16.9 Mid_Freq_V",
"P16.10 Max_V_Frq",
"P16.11 Max_Volt ",
"P16.12 Max_V_Ltd",
"P16.13 ff_V_Comp",
"P16.14 Sq_Crv_V ",
"P16.15 St_Brk_Tm",
"P16.16 St_Brk_B ",
"P16.17 St_Brk_I ",
"P16.18 Sp_Brk_Tm",
"P16.19 Sp_Brk_B ",
"P16.20 Sp_Brk_Ih",
"P16.21 Sp_Brk_Is",
"P16.22 CC P-Gain",
"P16.23 CC I-Gain",
"P16.24 StbT_Cons",
"P16.25 Stb_Gain ",
"P16.26 Stb_Limit",
"P16.27 Unity_I_f",
"P16.28 Acc_OC_Gn"
};
void PAGE_3_16_00(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_16_XX[0][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_16_01(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_16_XX[1][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_16_02(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_16_XX[2][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_16_03(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_16_XX[3][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_16_04(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_16_XX[4][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_16_05(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_16_XX[5][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_16_06(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_16_XX[6][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_16_07(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_16_XX[7][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_16_08(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_16_XX[8][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_16_09(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_16_XX[9][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_16_10(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_16_XX[10][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_16_11(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_16_XX[11][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_16_12(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_16_XX[12][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_16_13(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_16_XX[13][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_16_14(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_16_XX[14][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_16_15(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_16_XX[15][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_16_16(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_16_XX[16][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_16_17(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_16_XX[17][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_16_18(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_16_XX[18][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_16_19(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_16_XX[19][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_16_20(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_16_XX[20][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_16_21(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_16_XX[21][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_16_22(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_16_XX[22][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_16_23(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_16_XX[23][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_16_24(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_16_XX[24][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_16_25(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_16_XX[25][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_16_26(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_16_XX[26][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_16_27(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_16_XX[27][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_16_28(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_16_XX[28][0]));
	CLCD_string(0xC0,"                ");
}
prog_char  PAGE_DIR_3_17_XX[52][17]={
"P17.0 Spd_Dt_Tm ",
"P17.1 Min. Speed",
"P17.2 Max. Speed",
"P17.3 OS_Limit  ",
"P17.4 Rsv_04    ",
"P17.5 Start_Flux",
"P17.6 Base Flux ",
"P17.7 SF_End_Spd",
"P17.8 BF_St_Spd ",
"P17.9 FW_Voltage",
"P17.10 FW_Tm_Con",
"P17.11 CC P-Gain",
"P17.12 CC I-Gain",
"P17.13 CC Zero_S",
"P17.14 Spd_Gain ",
"P17.15 Load_Comp",
"P17.16 LC_Tm_Con",
"P17.17 LC_Freq  ",
"P17.18 SC P-Gain",
"P17.19 SC I-Gain",
"P17.20 SC Zero_S",
"P17.21 Gam_lam  ",
"P17.22 Rs_Ada_En",
"P17.23 Rs_Ada_Gn",
"P17.24 Gam_theta",
"P17.25 Del_lam  ",
"P17.26 Del_the  ",
"P17.27 Lm_Ada_En",
"P17.28 Start_FGn",
"P17.29 Zr_Hold_G",
"P17.30 Zr_Hold_F",
"P17.31 Brk_Flux ",
"P17.32 SC_G_Adj ",
"P17.33 Trq_R_Src",
"P17.34 Trq_Os_Sc",
"P17.35 Trq_L_Src",
"P17.36 Spd_Limit",
"P17.37 S_Ltd_Act",
"P17.38 S_Ltd_Off",
"P17.39 Spd_Ltd_G",
"P17.40 Trq_Comp ",
"P17.41 Trq_F_Src",
"P17.42 TC_P_Gain",
"P17.43 TC_I_Time",
"P17.44 TC_OutLmt",
"P17.45 Active_Ld",
"P17.46 Rs_AdptEn",
"P17.47 Unity_I_f",
"P17.48 Iner_Trq ",
"P17.49 InerDif_T",
"P17.50 SpdCtl_B ",
"P17.51 CurCtl_B "
};
void PAGE_3_17_00(void)
{              
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[0][0]));
	CLCD_string(0xC0,"                ");
}              
void PAGE_3_17_01(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[1][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u rpm ",edit_Temp));
}
void PAGE_3_17_02(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[2][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u  %  ",edit_Temp));
}
void PAGE_3_17_03(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[3][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u  %  ",edit_Temp));
}
void PAGE_3_17_04(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[4][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_05(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[5][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_06(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[6][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_07(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[7][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_08(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[8][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_09(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[9][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_10(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[10][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_11(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[11][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u  %  ",edit_Temp));
}
void PAGE_3_17_12(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[12][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u  %  ",edit_Temp));
}
void PAGE_3_17_13(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[13][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_14(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[14][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_15(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[15][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_16(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[16][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_17(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[17][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_18(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[18][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u  %  ",edit_Temp));
}
void PAGE_3_17_19(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[19][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u  %  ",edit_Temp));
}
void PAGE_3_17_20(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[20][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_21(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[21][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_22(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[22][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_23(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[23][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_24(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[24][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_25(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[25][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_26(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[26][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_27(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[27][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_28(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[28][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_29(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[29][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_30(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[30][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_31(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[31][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_32(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[32][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_33(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[33][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_34(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[34][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_35(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[35][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_36(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[36][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_37(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[37][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_38(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[38][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_39(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[39][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_40(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[40][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_41(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[41][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_42(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[42][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_43(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[43][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_44(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[44][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_45(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[45][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_46(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[46][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_47(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[47][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_48(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[48][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_49(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[49][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_17_50(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[50][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_17_51(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_17_XX[51][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}


prog_char  PAGE_DIR_3_18_XX[50][17]={
"P18.0 Spd_Dt_Tm ",
"P18.1 Min. Speed",
"P18.2 Max. Speed",
"P18.3 OS_Limit  ",
"P18.4 Rsv_04    ",
"P18.5 Start_Flux",
"P18.6 Base Flux ",
"P18.7 SF_End_Spd",
"P18.8 BF_St_Spd ",
"P18.9 FW_Voltage",
"P18.10 FW_Tm_Con",
"P18.11 CC P-Gain",
"P18.12 CC I-Gain",
"P18.13 CC Zero_S",
"P18.14 Spd_Gain ",
"P18.15 Load_Comp",
"P18.16 LC_Tm_Con",
"P18.17 LC_Freq  ",
"P18.18 SC P-Gain",
"P18.19 SC I-Gain",
"P18.20 SC Zero_S",
"P18.21 Gam_lam  ",
"P18.22 Rs_Ada_En",
"P18.23 Rs_Ada_Gn",
"P18.24 Gam_theta",
"P18.25 Del_lam  ",
"P18.26 Del_the  ",
"P18.27 Lm_Ada_En",
"P18.28 Start_FGn",
"P18.29 Zr_Hold_G",
"P18.30 Zr_Hold_F",
"P18.31 Brk_Flux ",
"P18.32 SC_G_Adj ",
"P18.33 Trq_R_Src",
"P18.34 Trq_Os_Sc",
"P18.35 Trq_L_Src",
"P18.36 Spd_Limit",
"P18.37 S_Ltd_Act",
"P18.38 S_Ltd_Off",
"P18.39 Spd_Ltd_G",
"P18.40 Trq_Comp ",
"P18.41 Trq_F_Src",
"P18.42 TC_P_Gain",
"P18.43 TC_I_Time",
"P18.44 TC_OutLmt",
"P18.45 Active_Ld",
"P18.46 Rs_AdptEn",
"P18.47 Unity_I_f",
"P18.48 Iner_Trq ",
"P18.49 InerDif_T"
};
void PAGE_3_18_00(void)
{              
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[0][0]));
	CLCD_string(0xC0,"                ");
}              
void PAGE_3_18_01(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[1][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_02(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[2][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_03(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[3][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_04(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[4][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_05(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[5][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_06(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[6][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_07(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[7][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_08(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[8][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_09(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[9][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_10(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[10][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_11(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[11][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_12(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[12][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_13(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[13][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_14(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[14][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_15(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[15][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_16(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[16][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_17(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[17][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_18(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[18][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_19(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[19][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_20(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[20][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_21(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[21][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_22(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[22][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_23(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[23][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_24(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[24][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_25(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[25][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_26(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[26][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_27(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[27][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_28(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[28][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_29(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[29][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_30(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[30][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_31(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[31][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_32(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[32][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_33(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[33][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_34(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[34][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_35(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[35][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_36(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[36][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_37(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[37][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_38(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[38][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_39(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[39][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_40(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[40][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_41(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[41][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_42(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[42][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_43(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[43][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_44(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[44][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_45(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[45][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_46(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[46][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_47(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[47][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_48(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[48][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_18_49(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_18_XX[49][0]));
	CLCD_string(0xC0,"                ");
}

prog_char  PAGE_DIR_3_19_XX[42][17]={
"P19.0 N_PG_Pulse",
"P19.1 PG_DIR_Inv",
"P19.2 Spd_Det_tm",
"P19.3 Min_Speed ",
"P19.4 Max_Speed ",
"P19.5 OS_Limit  ",
"P19.6 Rsv_06    ",
"P19.7 Start_Flux",
"P19.8 Base_Flux ",
"P19.9 SF_E_Spd  ",
"P19.10 BF_St_Spd",
"P19.11 FW_Volt  ",
"P19.12 FW_Tm_Con",
"P19.13 CC P-Gain",
"P19.14 CC I-Gain",
"P19.15 CC Zero_S",
"P19.16 FC_P-Gain",
"P19.17 FC_I-Gain",
"P19.18 MxField_I",
"P19.19 Spd_Gain ",
"P19.20 Load_Comp",
"P19.21 LC_Tm_Con",
"P19.22 SC P-Gain",
"P19.23 SC I-Gain",
"P19.24 SC Zero_S",
"P19.25 SC_G_Adj ",
"P19.26 Trq_R_Src",
"P19.27 Trq_Os_Sc",
"P19.28 Trq_L_Src",
"P19.29 Spd_Limit",
"P19.30 S_Ltd_Act",
"P19.31 S_Ltd_Off",
"P19.32 Spd_Ltd_G",
"P19.33 Trq_Comp ",
"P19.34 Trq_F_Src",
"P19.35 TC_P_Gain",
"P19.36 TC_I_Time",
"P19.37 TC_OutLmt",
"P19.38 Iner_Trq ",
"P19.39 InerDif_T",
"P19.40 Adap_Ctrl",
"P19.41 Adapt_Spd"
};
void PAGE_3_19_00(void)
{              
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[0][0]));
	CLCD_string(0xC0,"                ");
}              
void PAGE_3_19_01(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[1][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_02(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[2][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_03(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[3][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_04(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[4][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_05(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[5][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_06(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[6][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_07(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[7][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_08(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[8][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_09(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[9][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_10(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[10][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_11(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[11][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_12(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[12][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_13(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[13][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_14(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[14][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_15(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[15][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_16(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[16][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_17(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[17][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_18(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[18][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_19(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[19][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_20(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[20][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_21(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[21][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_22(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[22][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_23(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[23][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_24(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[24][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_25(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[25][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_26(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[26][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_27(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[27][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_28(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[28][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_29(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[29][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_30(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[30][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_31(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[31][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_32(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[32][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_33(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[33][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_34(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[34][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_35(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[35][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_36(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[36][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_37(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[37][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_38(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[38][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_39(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[39][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_40(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[40][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_19_41(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_19_XX[41][0]));
	CLCD_string(0xC0,"                ");
}
prog_char  PAGE_DIR_3_20_XX[42][17]={
"P20.0 N_PG_Pulse",
"P20.1 PG_DIR_Inv",
"P20.2 Spd_Det_tm",
"P20.3 Min_Speed ",
"P20.4 Max_Speed ",
"P20.5 OS_Limit  ",
"P20.6 Rsv_06    ",
"P20.7 Start_Flux",
"P20.8 Base_Flux ",
"P20.9 SF_E_Spd  ",
"P20.10 BF_St_Spd",
"P20.11 FW_Volt  ",
"P20.12 FW_Tm_Con",
"P20.13 CC P-Gain",
"P20.14 CC I-Gain",
"P20.15 CC Zero_S",
"P20.16 FC_P-Gain",
"P20.17 FC_I-Gain",
"P20.18 MxField_I",
"P20.19 Spd_Gain ",
"P20.20 Load_Comp",
"P20.21 LC_Tm_Con",
"P20.22 SC P-Gain",
"P20.23 SC I-Gain",
"P20.24 SC Zero_S",
"P20.25 SC_G_Adj ",
"P20.26 Trq_R_Src",
"P20.27 Trq_Os_Sc",
"P20.28 Trq_L_Src",
"P20.29 Spd_Limit",
"P20.30 S_Ltd_Act",
"P20.31 S_Ltd_Off",
"P20.32 Spd_Ltd_G",
"P20.33 Trq_Comp ",
"P20.34 Trq_F_Src",
"P20.35 TC_P_Gain",
"P20.36 TC_I_Time",
"P20.37 TC_OutLmt",
"P20.38 Iner_Trq ",
"P20.39 InerDif_T",
"P20.40 Adap_Ctrl",
"P20.41 Adapt_Spd"
};
void PAGE_3_20_00(void)
{              
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[0][0]));
	CLCD_string(0xC0,"                ");
}              
void PAGE_3_20_01(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[1][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_02(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[2][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_03(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[3][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_04(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[4][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_05(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[5][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_06(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[6][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_07(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[7][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_08(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[8][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_09(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[9][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_10(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[10][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_11(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[11][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_12(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[12][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_13(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[13][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_14(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[14][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_15(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[15][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_16(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[16][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_17(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[17][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_18(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[18][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_19(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[19][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_20(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[20][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_21(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[21][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_22(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[22][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_23(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[23][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_24(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[24][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_25(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[25][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_26(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[26][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_27(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[27][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_28(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[28][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_29(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[29][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_30(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[30][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_31(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[31][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_32(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[32][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_33(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[33][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_34(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[34][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_35(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[35][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_36(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[36][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_37(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[37][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_38(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[38][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_39(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[39][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_40(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[40][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_20_41(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_20_XX[41][0]));
	CLCD_string(0xC0,"                ");
}
prog_char  PAGE_DIR_3_21_XX[16][17]={
"P21.0 Pri_Res 0 ",
"P21.1 Pri_Res 1 ",
"P21.2 Sec_Res   ",
"P21.3 Stator_Ind",
"P21.4 Rotor_Ind ",
"P21.5 Lkg_Ind   ",
"P21.6 Inertia_Tm",
"P21.7 Iron_Loss ",
"P21.8 Bis_Damp  ",
"P21.9 Stator R  ",
"P21.10 D_axis L ",
"P21.11 Q_axis L ",
"P21.12 Back EMF ",
"P21.13 Interia  ",
"P21.14 Friction ",
"P21.15 Base_Spd "
};
void PAGE_3_21_00(void)
{              
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_21_XX[0][0]));
	CLCD_string(0xC0,"                ");
}              
void PAGE_3_21_01(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_21_XX[1][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_21_02(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_21_XX[2][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_21_03(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_21_XX[3][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_21_04(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_21_XX[4][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_21_05(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_21_XX[5][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_21_06(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_21_XX[6][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_21_07(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_21_XX[7][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_21_08(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_21_XX[8][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_21_09(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_21_XX[9][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_21_10(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_21_XX[10][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_21_11(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_21_XX[11][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_21_12(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_21_XX[12][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_21_13(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_21_XX[13][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_21_14(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_21_XX[14][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
void PAGE_3_21_15(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_21_XX[15][0]));
	CLCD_string(0xC0,(char*)_TEXT("       % 4u     ",edit_Temp));
}
prog_char  PAGE_DIR_3_22_XX[9][17]={
"P22.0 Pri_Res 0 ",
"P22.1 Pri_Res 1 ",
"P22.2 Sec_Res   ",
"P22.3 Stator_Ind",
"P22.4 Rotor_Ind ",
"P22.5 Lkg_Ind   ",
"P22.6 Inertia_Tm",
"P22.7 Iron_Loss ",
"P22.8 Bis_Damp  "
};
void PAGE_3_22_00(void)
{              
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_22_XX[0][0]));
	CLCD_string(0xC0,"                ");
}              
void PAGE_3_22_01(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_22_XX[1][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_22_02(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_22_XX[2][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_22_03(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_22_XX[3][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_22_04(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_22_XX[4][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_22_05(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_22_XX[5][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_22_06(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_22_XX[6][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_22_07(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_22_XX[7][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_22_08(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_22_XX[8][0]));
	CLCD_string(0xC0,"                ");
}

prog_char  PAGE_DIR_3_23_XX[12][17]={
"P23.0 Dead_Time ",
"P23.1 Lin_Ratio ",
"P23.2 V_Sens_Scl",
"P23.3 I_Scale_A ",
"P23.4 I_Scale_B ",
"P23.5 I_Scale_C ",
"P23.6 I_Offs_A1 ",
"P23.7 I_Offs_B1 ",
"P23.8 I_Offs_C1 ",
"P23.9 I_Offs_A2 ",
"P23.10 I_Offs_B2",
"P23.11 I_Offs_C2"
};
void PAGE_3_23_00(void)
{              
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_23_XX[0][0]));
	CLCD_string(0xC0,"                ");
}              
void PAGE_3_23_01(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_23_XX[1][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_23_02(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_23_XX[2][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_23_03(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_23_XX[3][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_23_04(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_23_XX[4][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_23_05(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_23_XX[5][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_23_06(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_23_XX[6][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_23_07(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_23_XX[7][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_23_08(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_23_XX[8][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_23_09(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_23_XX[9][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_23_10(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_23_XX[10][0]));
	CLCD_string(0xC0,"                ");
}
void PAGE_3_23_11(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_23_XX[11][0]));
	CLCD_string(0xC0,"                ");
}


prog_char  PAGE_DIR_4_XX_3_4[5][17]={
"0 Drive_Calibra ",
"1 Motor Tuning  ",
"2 Speed Tuning  ",
" Processing...  ",
" Completed...  "
};

void PAGE_4_00(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[4][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_4_XX_3_4[0][0]));
}
void PAGE_4_01(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[4][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_4_XX_3_4[1][0]));
}
void PAGE_4_02(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[4][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_4_XX_3_4[2][0]));
}



void PAGE_4_00_3(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_4_XX_3_4[0][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_4_XX_3_4[3][0]));
}
void PAGE_4_01_3(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_4_XX_3_4[1][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_4_XX_3_4[3][0]));
}
void PAGE_4_02_3(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_4_XX_3_4[2][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_4_XX_3_4[3][0]));
}



void PAGE_4_00_3_4(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_4_XX_3_4[0][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_4_XX_3_4[4][0]));
}
void PAGE_4_01_3_4(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_4_XX_3_4[1][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_4_XX_3_4[4][0]));
}
void PAGE_4_02_3_4(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_4_XX_3_4[2][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_4_XX_3_4[4][0]));
}


prog_char  PAGE_DIR_5_XX[10][17]={
"   0 Record(0)  ",
"   1 Record(1)  ",
"   2 Record(2)  ",
"   3 Record(3)  ",
"   4 Record(4)  ",
"   5 Record(5)  ",
"   6 Record(6)  ",
"   7 Record(7)  ",
"   8 Record(8)  "
};


void PAGE_5_00(void)
{
	CLCD_string(0x80,"Total Fault = X");
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_5_XX[0][0]));
}
void PAGE_5_01(void)
{
	CLCD_string(0x80,"Total Fault = X");
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_5_XX[1][0]));
}
void PAGE_5_02(void)
{
	CLCD_string(0x80,"Total Fault = X");
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_5_XX[2][0]));
}
void PAGE_5_03(void)
{
	CLCD_string(0x80,"Total Fault = X");
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_5_XX[3][0]));
}
void PAGE_5_04(void)
{
	CLCD_string(0x80,"Total Fault = X");
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_5_XX[4][0]));
}
void PAGE_5_05(void)
{
	CLCD_string(0x80,"Total Fault = X");
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_5_XX[5][0]));
}
void PAGE_5_06(void)
{
	CLCD_string(0x80,"Total Fault = X");
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_5_XX[6][0]));
}
void PAGE_5_07(void)
{
	CLCD_string(0x80,"Total Fault = X");
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_5_XX[7][0]));
}
void PAGE_5_08(void)
{
	CLCD_string(0x80,"Total Fault = X");
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_5_XX[8][0]));
}


prog_char  PAGE_DIR_5_XX_XX[11][18]={
"[%d.1]Fault Code ",
"[%d.2]Motor Sel  ",
"[%d.3]Ctrl Mthd  ",
"[%d.4]Speed CMD  ",
"[%d.5]Motor Spd  ",
"[%d.6]Frequency  ",
"[%d.7]Temp       ",
"[%d.8]Actl Torque",
"[%d.9]DC Link V  ",
"[%d.10]Motor Curr",
"[%d.11]Motor Volt"
};


void PAGE_5_00_00(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[0][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"                ");
}
void PAGE_5_00_01(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[1][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"                ");
}
void PAGE_5_00_02(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[2][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"                ");
}
void PAGE_5_00_03(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[3][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"             rpm");
}
void PAGE_5_00_04(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[4][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"             rpm");
}
void PAGE_5_00_05(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[5][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"              Hz");
}
void PAGE_5_00_06(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[6][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"              'C");
}
void PAGE_5_00_07(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[7][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"              Nm");
}
void PAGE_5_00_08(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[8][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"             Vdc");
}
void PAGE_5_00_09(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[9][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"            Arms");
}
void PAGE_5_00_10(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[10][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"            Vrms");
}

void PAGE_5_01_00(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[0][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"                ");
}
void PAGE_5_01_01(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[1][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"                ");
}
void PAGE_5_01_02(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[2][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"                ");
}
void PAGE_5_01_03(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[3][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"             rpm");
}
void PAGE_5_01_04(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[4][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"             rpm");
}
void PAGE_5_01_05(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[5][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"              Hz");
}
void PAGE_5_01_06(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[6][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"              'C");
}
void PAGE_5_01_07(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[7][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"              Nm");
}
void PAGE_5_01_08(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[8][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"             Vdc");
}
void PAGE_5_01_09(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[9][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"            Arms");
}
void PAGE_5_01_10(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[10][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"            Vrms");
}

void PAGE_5_02_00(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[0][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"                ");
}
void PAGE_5_02_01(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[1][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"                ");
}
void PAGE_5_02_02(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[2][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"                ");
}
void PAGE_5_02_03(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[3][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"             rpm");
}
void PAGE_5_02_04(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[4][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"             rpm");
}
void PAGE_5_02_05(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[5][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"              Hz");
}
void PAGE_5_02_06(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[6][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"              'C");
}
void PAGE_5_02_07(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[7][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"              Nm");
}
void PAGE_5_02_08(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[8][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"             Vdc");
}
void PAGE_5_02_09(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[9][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"            Arms");
}
void PAGE_5_02_10(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[10][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"            Vrms");
}
void PAGE_5_03_00(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[0][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"                ");
}
void PAGE_5_03_01(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[1][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"                ");
}
void PAGE_5_03_02(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[2][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"                ");
}
void PAGE_5_03_03(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[3][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"             rpm");
}
void PAGE_5_03_04(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[4][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"             rpm");
}
void PAGE_5_03_05(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[5][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"              Hz");
}
void PAGE_5_03_06(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[6][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"              'C");
}
void PAGE_5_03_07(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[7][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"              Nm");
}
void PAGE_5_03_08(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[8][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"             Vdc");
}
void PAGE_5_03_09(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[9][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"            Arms");
}
void PAGE_5_03_10(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[10][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"            Vrms");
}
void PAGE_5_04_00(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[0][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"                ");
}
void PAGE_5_04_01(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[1][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"                ");
}
void PAGE_5_04_02(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[2][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"                ");
}
void PAGE_5_04_03(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[3][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"             rpm");
}
void PAGE_5_04_04(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[4][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"             rpm");
}
void PAGE_5_04_05(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[5][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"              Hz");
}
void PAGE_5_04_06(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[6][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"              'C");
}
void PAGE_5_04_07(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[7][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"              Nm");
}
void PAGE_5_04_08(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[8][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"             Vdc");
}
void PAGE_5_04_09(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[9][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"            Arms");
}
void PAGE_5_04_10(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[10][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"            Vrms");
}
void PAGE_5_05_00(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[0][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"                ");
}
void PAGE_5_05_01(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[1][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"                ");
}
void PAGE_5_05_02(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[2][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"                ");
}
void PAGE_5_05_03(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[3][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"             rpm");
}
void PAGE_5_05_04(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[4][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"             rpm");
}
void PAGE_5_05_05(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[5][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"              Hz");
}
void PAGE_5_05_06(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[6][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"              'C");
}
void PAGE_5_05_07(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[7][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"              Nm");
}
void PAGE_5_05_08(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[8][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"             Vdc");
}
void PAGE_5_05_09(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[9][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"            Arms");
}
void PAGE_5_05_10(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[10][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"            Vrms");
}
void PAGE_5_06_00(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[0][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"                ");
}
void PAGE_5_06_01(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[1][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"                ");
}
void PAGE_5_06_02(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[2][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"                ");
}
void PAGE_5_06_03(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[3][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"             rpm");
}
void PAGE_5_06_04(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[4][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"             rpm");
}
void PAGE_5_06_05(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[5][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"              Hz");
}
void PAGE_5_06_06(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[6][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"              'C");
}
void PAGE_5_06_07(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[7][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"              Nm");
}
void PAGE_5_06_08(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[8][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"             Vdc");
}
void PAGE_5_06_09(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[9][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"            Arms");
}
void PAGE_5_06_10(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[10][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"            Vrms");
}
void PAGE_5_07_00(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[0][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"                ");
}
void PAGE_5_07_01(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[1][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"                ");
}
void PAGE_5_07_02(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[2][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"                ");
}
void PAGE_5_07_03(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[3][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"             rpm");
}
void PAGE_5_07_04(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[4][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"             rpm");
}
void PAGE_5_07_05(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[5][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"              Hz");
}
void PAGE_5_07_06(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[6][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"              'C");
}
void PAGE_5_07_07(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[7][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"              Nm");
}
void PAGE_5_07_08(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[8][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"             Vdc");
}
void PAGE_5_07_09(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[9][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"            Arms");
}
void PAGE_5_07_10(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[10][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"            Vrms");
}
void PAGE_5_08_00(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[0][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"                ");
}
void PAGE_5_08_01(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[1][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"                ");
}
void PAGE_5_08_02(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[2][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"                ");
}
void PAGE_5_08_03(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[3][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"             rpm");
}
void PAGE_5_08_04(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[4][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"             rpm");
}
void PAGE_5_08_05(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[5][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"              Hz");
}
void PAGE_5_08_06(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[6][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"              'C");
}
void PAGE_5_08_07(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[7][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"              Nm");
}
void PAGE_5_08_08(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[8][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"             Vdc");
}
void PAGE_5_08_09(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[9][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"            Arms");
}
void PAGE_5_08_10(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[10][0]), ( ((naviMENU/100)%5)+1)))) ;
	CLCD_string(0xC0,"            Vrms");
}



prog_char  PAGE_DIR_6_XX_XX[8][20]={
"[0]Clr FaultList    ",
"[1]System Reset ",
"[2]Parameter    ",
"Reset->[Enter] ",
"  Complete...   ",
"Initializing...",
"ERR [Warning]",
"W13 Drive Cal."
};


void PAGE_6_00(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[6][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[0][0]));
}
void PAGE_6_01(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[6][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[1][0]));
}
void PAGE_6_02(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[6][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[2][0]));
}

void PAGE_6_00_04(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[0][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[4][0]));
}
void PAGE_6_01_03(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[1][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[3][0]));
}
void PAGE_6_01_05(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[1][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[5][0]));
}
void PAGE_6_02_05(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[2][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[5][0]));
}


prog_char  PAGE_DIR_7_XX_XX[8][20]={
"[0]Access=L%d    ",
"[1]Admission    ",
"[2]Pwd Change   ",
"  Enter: %d%d%d%d   ",
"  Complete...   ",
"  Password[L%d]  ",
" Admitted.[L%d] ",
"New Password[L%d]"
};

void PAGE_7_00(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[7][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[0][0]));
}
void PAGE_7_01(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[7][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[1][0]));
}
void PAGE_7_02(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[7][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[2][0]));
}

void PAGE_7_01_00(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[1][0]));
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[5][0]), ( (naviMENU%701)+1)))) ;
}
void PAGE_7_01_01(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[1][0]));
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[5][0]), ( (naviMENU%701)+1)))) ;
}
void PAGE_7_01_02(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[1][0]));
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[5][0]), ( (naviMENU%701)+1)))) ;
}
void PAGE_7_01_03(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[1][0]));
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[5][0]), ( (naviMENU%701)+1)))) ;
}
void PAGE_7_01_04(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[1][0]));
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[5][0]), ( (naviMENU%701)+1)))) ;
}


void PAGE_7_01_00_05(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[5][0]), (((naviMENU%701)/100)+1)))) ;
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[3][0]),_password[0],_password[1],_password[2],_password[3]))) ;

	if(pass_pos==0)			CLCD_cursor_ON(0xC0,9);
	else if(pass_pos==1)		CLCD_cursor_ON(0xC0,10);
	else if(pass_pos==2)		CLCD_cursor_ON(0xC0,11);
	else if(pass_pos==3)		CLCD_cursor_ON(0xC0,12);
}
void PAGE_7_01_00_06(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[5][0]), (((naviMENU%701)/100)+1)))) ;
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[6][0]), (((naviMENU%701)/100)+1)))) ;
	CLCD_cursor_OFF();
}
void PAGE_7_01_01_05(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[5][0]), (((naviMENU%701)/100)+1)))) ;
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[3][0]),_password[0],_password[1],_password[2],_password[3]))) ;

	if(pass_pos==0)			CLCD_cursor_ON(0xC0,9);
	else if(pass_pos==1)		CLCD_cursor_ON(0xC0,10);
	else if(pass_pos==2)		CLCD_cursor_ON(0xC0,11);
	else if(pass_pos==3)		CLCD_cursor_ON(0xC0,12);
}
void PAGE_7_01_01_06(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[5][0]), (((naviMENU%701)/100)+1)))) ;
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[6][0]), (((naviMENU%701)/100)+1)))) ;
	CLCD_cursor_OFF();
}
void PAGE_7_01_02_05(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[5][0]), (((naviMENU%701)/100)+1)))) ;
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[3][0]),_password[0],_password[1],_password[2],_password[3]))) ;

	if(pass_pos==0)			CLCD_cursor_ON(0xC0,9);
	else if(pass_pos==1)		CLCD_cursor_ON(0xC0,10);
	else if(pass_pos==2)		CLCD_cursor_ON(0xC0,11);
	else if(pass_pos==3)		CLCD_cursor_ON(0xC0,12);
}
void PAGE_7_01_02_06(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[5][0]), (((naviMENU%701)/100)+1)))) ;
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[6][0]), (((naviMENU%701)/100)+1)))) ;
	CLCD_cursor_OFF();
}
void PAGE_7_01_03_05(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[5][0]), (((naviMENU%701)/100)+1)))) ;
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[3][0]),_password[0],_password[1],_password[2],_password[3]))) ;

	if(pass_pos==0)			CLCD_cursor_ON(0xC0,9);
	else if(pass_pos==1)		CLCD_cursor_ON(0xC0,10);
	else if(pass_pos==2)		CLCD_cursor_ON(0xC0,11);
	else if(pass_pos==3)		CLCD_cursor_ON(0xC0,12);
}
void PAGE_7_01_03_06(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[5][0]), (((naviMENU%701)/100)+1)))) ;
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[6][0]), (((naviMENU%701)/100)+1)))) ;
	CLCD_cursor_OFF();
}
void PAGE_7_01_04_05(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[5][0]), (((naviMENU%701)/100)+1)))) ;
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[3][0]),_password[0],_password[1],_password[2],_password[3]))) ;

	if(pass_pos==0)			CLCD_cursor_ON(0xC0,9);
	else if(pass_pos==1)		CLCD_cursor_ON(0xC0,10);
	else if(pass_pos==2)		CLCD_cursor_ON(0xC0,11);
	else if(pass_pos==3)		CLCD_cursor_ON(0xC0,12);
}
void PAGE_7_01_04_06(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[5][0]), (((naviMENU%701)/100)+1)))) ;
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[6][0]), (((naviMENU%701)/100)+1)))) ;
	CLCD_cursor_OFF();
}

void PAGE_7_02_00(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[2][0]));
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[5][0]), ( (naviMENU%702)+1)))) ;
}
void PAGE_7_02_01(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[2][0]));
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[5][0]), ( (naviMENU%702)+1)))) ;
}
void PAGE_7_02_02(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[2][0]));
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[5][0]), ( (naviMENU%702)+1)))) ;
}
void PAGE_7_02_03(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[2][0]));
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[5][0]), ( (naviMENU%702)+1)))) ;
}
void PAGE_7_02_04(void)
{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[2][0]));
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[5][0]), ( (naviMENU%702)+1)))) ;
}

void PAGE_7_02_00_05(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[5][0]), (((naviMENU%702)/100)+1)))) ;
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[3][0]),_password[0],_password[1],_password[2],_password[3]))) ;

	if(pass_pos==0)			CLCD_cursor_ON(0xC0,9);
	else if(pass_pos==1)		CLCD_cursor_ON(0xC0,10);
	else if(pass_pos==2)		CLCD_cursor_ON(0xC0,11);
	else if(pass_pos==3)		CLCD_cursor_ON(0xC0,12);
}
void PAGE_7_02_00_06(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[7][0]), (((naviMENU%702)/100)+1)))) ;
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[3][0]),_password[0],_password[1],_password[2],_password[3]))) ;

	if(pass_pos==0)			CLCD_cursor_ON(0xC0,9);
	else if(pass_pos==1)		CLCD_cursor_ON(0xC0,10);
	else if(pass_pos==2)		CLCD_cursor_ON(0xC0,11);
	else if(pass_pos==3)		CLCD_cursor_ON(0xC0,12);
}
void PAGE_7_02_00_07(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[7][0]), (((naviMENU%702)/100)+1)))) ;
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[4][0]));
	CLCD_cursor_OFF();
}
void PAGE_7_02_01_05(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[5][0]), (((naviMENU%702)/100)+1)))) ;
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[3][0]),_password[0],_password[1],_password[2],_password[3]))) ;

	if(pass_pos==0)			CLCD_cursor_ON(0xC0,9);
	else if(pass_pos==1)		CLCD_cursor_ON(0xC0,10);
	else if(pass_pos==2)		CLCD_cursor_ON(0xC0,11);
	else if(pass_pos==3)		CLCD_cursor_ON(0xC0,12);
}
void PAGE_7_02_01_06(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[7][0]), (((naviMENU%702)/100)+1)))) ;
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[3][0]),_password[0],_password[1],_password[2],_password[3]))) ;

	if(pass_pos==0)			CLCD_cursor_ON(0xC0,9);
	else if(pass_pos==1)		CLCD_cursor_ON(0xC0,10);
	else if(pass_pos==2)		CLCD_cursor_ON(0xC0,11);
	else if(pass_pos==3)		CLCD_cursor_ON(0xC0,12);
}
void PAGE_7_02_01_07(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[7][0]), (((naviMENU%702)/100)+1)))) ;
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[4][0]));
	CLCD_cursor_OFF();
}

void PAGE_7_02_02_05(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[5][0]), (((naviMENU%702)/100)+1)))) ;
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[3][0]),_password[0],_password[1],_password[2],_password[3]))) ;

	if(pass_pos==0)			CLCD_cursor_ON(0xC0,9);
	else if(pass_pos==1)		CLCD_cursor_ON(0xC0,10);
	else if(pass_pos==2)		CLCD_cursor_ON(0xC0,11);
	else if(pass_pos==3)		CLCD_cursor_ON(0xC0,12);
}
void PAGE_7_02_02_06(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[7][0]), (((naviMENU%702)/100)+1)))) ;
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[3][0]),_password[0],_password[1],_password[2],_password[3]))) ;

	if(pass_pos==0)			CLCD_cursor_ON(0xC0,9);
	else if(pass_pos==1)		CLCD_cursor_ON(0xC0,10);
	else if(pass_pos==2)		CLCD_cursor_ON(0xC0,11);
	else if(pass_pos==3)		CLCD_cursor_ON(0xC0,12);
}
void PAGE_7_02_02_07(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[7][0]), (((naviMENU%702)/100)+1)))) ;
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[4][0]));
	CLCD_cursor_OFF();
}
void PAGE_7_02_03_05(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[5][0]), (((naviMENU%702)/100)+1)))) ;
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[3][0]),_password[0],_password[1],_password[2],_password[3]))) ;

	if(pass_pos==0)			CLCD_cursor_ON(0xC0,9);
	else if(pass_pos==1)		CLCD_cursor_ON(0xC0,10);
	else if(pass_pos==2)		CLCD_cursor_ON(0xC0,11);
	else if(pass_pos==3)		CLCD_cursor_ON(0xC0,12);
}
void PAGE_7_02_03_06(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[7][0]), (((naviMENU%702)/100)+1)))) ;
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[3][0]),_password[0],_password[1],_password[2],_password[3]))) ;

	if(pass_pos==0)			CLCD_cursor_ON(0xC0,9);
	else if(pass_pos==1)		CLCD_cursor_ON(0xC0,10);
	else if(pass_pos==2)		CLCD_cursor_ON(0xC0,11);
	else if(pass_pos==3)		CLCD_cursor_ON(0xC0,12);
}
void PAGE_7_02_03_07(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[7][0]), (((naviMENU%702)/100)+1)))) ;
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[4][0]));
	CLCD_cursor_OFF();
}
void PAGE_7_02_04_05(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[5][0]), (((naviMENU%702)/100)+1)))) ;
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[3][0]),_password[0],_password[1],_password[2],_password[3]))) ;

	if(pass_pos==0)			CLCD_cursor_ON(0xC0,9);
	else if(pass_pos==1)		CLCD_cursor_ON(0xC0,10);
	else if(pass_pos==2)		CLCD_cursor_ON(0xC0,11);
	else if(pass_pos==3)		CLCD_cursor_ON(0xC0,12);
}
void PAGE_7_02_04_06(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[7][0]), (((naviMENU%702)/100)+1)))) ;
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[3][0]),_password[0],_password[1],_password[2],_password[3]))) ;

	if(pass_pos==0)			CLCD_cursor_ON(0xC0,9);
	else if(pass_pos==1)		CLCD_cursor_ON(0xC0,10);
	else if(pass_pos==2)		CLCD_cursor_ON(0xC0,11);
	else if(pass_pos==3)		CLCD_cursor_ON(0xC0,12);
}
void PAGE_7_02_04_07(void)
{
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[7][0]), (((naviMENU%702)/100)+1)))) ;
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_7_XX_XX[4][0]));
	CLCD_cursor_OFF();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PAGE_0xFFFFFFFF(void)
{
	//GLCD_BufClear();	
	//GLCD_print0508(2, 1,"for The GOD");
	//GLCD_print0508(2, 2,"for The JESUS");
	//GLCD_print0508(2, 3,"for The PEOPLE");

	CLCD_string(0x80,"        Designed");
	CLCD_string(0xC0," by Paul BH Park");

}






unsigned int Temporary=0;

void SYS_1(void)
{
	if(KeyState.KeyValue == UP)MenuDisplay = SYS_7;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2;
	else if(KeyState.KeyValue == ENTER) MenuDisplay = SYS_1_0;
	else if(KeyState.KeyValue == (ESC  & RUN & STOP & ENTER) )	naviMENU = 0xFFFFFFFF;


	if(RefreshFlag) PAGE_1();

}
void SYS_2(void)
{
	if(KeyState.KeyValue == UP)naviMENU = 1;
	else if(KeyState.KeyValue == DN)naviMENU = 3;
	else if(KeyState.KeyValue == ENTER)naviMENU = 20;
	else if(KeyState.KeyValue == (ESC  & RUN & STOP & ENTER) )	naviMENU = 0xFFFFFFFF;

	if(RefreshFlag) PAGE_2();
}
void SYS_3(void)
{
	if(KeyState.KeyValue == UP)naviMENU = 2;
	else if(KeyState.KeyValue == DN)naviMENU = 4;
	else if(KeyState.KeyValue == ENTER)naviMENU = 300;
	else if(KeyState.KeyValue == (ESC  & RUN & STOP & ENTER) )	naviMENU = 0xFFFFFFFF;

	if(RefreshFlag) PAGE_3();
}
void SYS_4(void)
{
	if(KeyState.KeyValue == UP)naviMENU = 3;
	else if(KeyState.KeyValue == DN)naviMENU = 5;
	else if(KeyState.KeyValue == ENTER)naviMENU = 400;
	else if(KeyState.KeyValue == (ESC  & RUN & STOP & ENTER) )	naviMENU = 0xFFFFFFFF;

	if(RefreshFlag) PAGE_4();
}
void SYS_5(void)
{
	if(KeyState.KeyValue == UP)naviMENU = 4;
	else if(KeyState.KeyValue == DN)naviMENU = 6;
	else if(KeyState.KeyValue == ENTER)naviMENU = 500;
	else if(KeyState.KeyValue == (ESC  & RUN & STOP & ENTER) )	naviMENU = 0xFFFFFFFF;

	if(RefreshFlag) PAGE_5();
}
void SYS_6(void)
{
	if(KeyState.KeyValue == UP)naviMENU = 5;
	else if(KeyState.KeyValue == DN)naviMENU = 7;
	else if(KeyState.KeyValue == ENTER)naviMENU = 600;
	else if(KeyState.KeyValue == (ESC  & RUN & STOP & ENTER) )	naviMENU = 0xFFFFFFFF;

	if(RefreshFlag) PAGE_6();
}
void SYS_7(void)
{
	if(KeyState.KeyValue == UP)naviMENU = 6;
	else if(KeyState.KeyValue == DN)naviMENU = 1;
	else if(KeyState.KeyValue == ENTER)naviMENU = 700;
	else if(KeyState.KeyValue == (ESC  & RUN & STOP & ENTER) )	naviMENU = 0xFFFFFFFF;

	if(RefreshFlag) PAGE_7();
}




void SYS_1_0(void)
{
//	unsigned char c;

	if(KeyState.KeyValue == ENTER)EnterFlag = 1;
	else if(KeyState.KeyValue == ESC)naviMENU = 1;
	else if(KeyState.KeyValue == UP)naviMENU = 15;
	else if(KeyState.KeyValue == DN)naviMENU = 11;

	if(RefreshFlag) PAGE_1_0();
}
void SYS_1_0_0(void)
{
	if(KeyState.KeyValue == ENTER)EnterFlag = 1;
	else if(KeyState.KeyValue == ESC)naviMENU = 1;
	else if(KeyState.KeyValue == UP)naviMENU = 15;
	else if(KeyState.KeyValue == DN)naviMENU = 11;

	if(RefreshFlag) PAGE_1_0_0();
}
void SYS_1_1(void)
{
	if(KeyState.KeyValue == ENTER)naviMENU = 200;
	else if(KeyState.KeyValue == ESC)naviMENU = 1;
	else if(KeyState.KeyValue == UP)naviMENU = 10;
	else if(KeyState.KeyValue == DN)naviMENU = 12;

	if(RefreshFlag) PAGE_1_1();
}
void SYS_1_1_0(void)
{
	if(KeyState.KeyValue == ENTER)naviMENU = 20;
	else if(KeyState.KeyValue == ESC)naviMENU = 1;
	else if(KeyState.KeyValue == UP)naviMENU = 10;
	else if(KeyState.KeyValue == DN)naviMENU = 12;

	if(RefreshFlag) PAGE_1_1();
}
void SYS_1_2(void)
{
	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
		
		if(KeyState.KeyValue == ESC)naviMENU = 1;
		else if(KeyState.KeyValue == UP)naviMENU = 11;
		else if(KeyState.KeyValue == DN)naviMENU = 13;
		else if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,9);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,7);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
	}

	if(RefreshFlag) PAGE_1_2();
}
void SYS_1_3(void)
{
	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
		
		if(KeyState.KeyValue == ESC)naviMENU = 1;
		else if(KeyState.KeyValue == UP)naviMENU = 12;
		else if(KeyState.KeyValue == DN)naviMENU = 14;
		else if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,9);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,7);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}

	}

	if(RefreshFlag) PAGE_1_3();
}
void SYS_1_4(void)
{
	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
		
		if(KeyState.KeyValue == ESC)naviMENU = 1;
		else if(KeyState.KeyValue == UP)naviMENU = 13;
		else if(KeyState.KeyValue == DN)naviMENU = 15;
		else if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,9);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,7);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}

	}

	if(RefreshFlag) PAGE_1_4();
}

void SYS_1_5(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 1;
	else if(KeyState.KeyValue == UP)naviMENU = 14;
	else if(KeyState.KeyValue == DN)naviMENU = 10;

	if(RefreshFlag) PAGE_1_5();
}





void SYS_2_0(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 2;
	else if(KeyState.KeyValue == ENTER)naviMENU = 2000;
	else if(KeyState.KeyValue == UP)naviMENU = 22;
	else if(KeyState.KeyValue == DN)naviMENU = 21;

	if(RefreshFlag) PAGE_2_0();
}

void SYS_2_0_00(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU = 2013;

	if(RefreshFlag) PAGE_2_0_00();
}
void SYS_2_0_01(void)
{
	SYS_Base_KeyFunction();

	if(RefreshFlag) PAGE_2_0_01();
}
void SYS_2_0_02(void)
{
	SYS_Base_KeyFunction();

	if(RefreshFlag) PAGE_2_0_02();
}
void SYS_2_0_03(void)
{
	SYS_Base_KeyFunction();

	if(RefreshFlag) PAGE_2_0_03();
}
void SYS_2_0_04(void)
{
	SYS_Base_KeyFunction();

	if(RefreshFlag) PAGE_2_0_04();
}
void SYS_2_0_05(void)
{
	SYS_Base_KeyFunction();

	if(RefreshFlag) PAGE_2_0_05();
}
void SYS_2_0_06(void)
{
	SYS_Base_KeyFunction();

	if(RefreshFlag) PAGE_2_0_06();
}
void SYS_2_0_07(void)
{
	SYS_Base_KeyFunction();

	if(RefreshFlag) PAGE_2_0_07();
}
void SYS_2_0_08(void)
{
	SYS_Base_KeyFunction();

	if(RefreshFlag) PAGE_2_0_08();
}
void SYS_2_0_09(void)
{
	SYS_Base_KeyFunction();

	if(RefreshFlag) PAGE_2_0_09();
}
void SYS_2_0_10(void)
{
	SYS_Base_KeyFunction();

	if(RefreshFlag) PAGE_2_0_10();
}
void SYS_2_0_11(void)
{
	SYS_Base_KeyFunction();

	if(RefreshFlag) PAGE_2_0_11();
}
void SYS_2_0_12(void)
{
	SYS_Base_KeyFunction();

	if(RefreshFlag) PAGE_2_0_12();
}
void SYS_2_0_13(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == DN)naviMENU = 2000;

	if(RefreshFlag) PAGE_2_0_13();
}





void SYS_2_1(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 2;
	else if(KeyState.KeyValue == ENTER)naviMENU = 2100;
	else if(KeyState.KeyValue == UP)naviMENU = 20;
	else if(KeyState.KeyValue == DN)naviMENU = 22;

	if(RefreshFlag) PAGE_2_1();
}

void SYS_2_1_00(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU = 2104;
	if(RefreshFlag) PAGE_2_1_00();
}
void SYS_2_1_01(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_2_1_01();
}
void SYS_2_1_02(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_2_1_02();
}
void SYS_2_1_03(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_2_1_03();
}
void SYS_2_1_04(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == DN)naviMENU = 2100;

	if(RefreshFlag) PAGE_2_1_04();
}




void SYS_2_2(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 2;
	else if(KeyState.KeyValue == ENTER)naviMENU = 2200;
	else if(KeyState.KeyValue == UP)naviMENU = 21;
	else if(KeyState.KeyValue == DN)naviMENU = 20;

	if(RefreshFlag) PAGE_2_2();
}
void SYS_2_2_00(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU = 2208;
	if(RefreshFlag) PAGE_2_2_00();
}
void SYS_2_2_01(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_2_2_01();
}
void SYS_2_2_02(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_2_2_02();
}
void SYS_2_2_03(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_2_2_03();
}
void SYS_2_2_04(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_2_2_04();
}
void SYS_2_2_05(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_2_2_05();
}
void SYS_2_2_06(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_2_2_06();
}
void SYS_2_2_07(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_2_2_07();
}
void SYS_2_2_08(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == DN)naviMENU = 2200;
	if(RefreshFlag) PAGE_2_2_08();
}




void SYS_3_00(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU = 324;
	if(RefreshFlag) PAGE_3_00();
}
void SYS_3_01(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_01();
}
void SYS_3_02(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_02();
}
void SYS_3_03(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03();
}
void SYS_3_04(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04();
}
void SYS_3_05(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_05();
}
void SYS_3_06(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06();
}
void SYS_3_07(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_07();
}
void SYS_3_08(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_08();
}
void SYS_3_09(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_09();
}
void SYS_3_10(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_10();
}
void SYS_3_11(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_11();
}
void SYS_3_12(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_12();
}
void SYS_3_13(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_13();
}
void SYS_3_14(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_14();
}
void SYS_3_15(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_15();
}
void SYS_3_16(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_16();
}
void SYS_3_17(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17();
}
void SYS_3_18(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18();
}
void SYS_3_19(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19();
}
void SYS_3_20(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20();
}
void SYS_3_21(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_21();
}
void SYS_3_22(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_22();
}
void SYS_3_23(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_23();
}
void SYS_3_24(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == DN)naviMENU = 300;
	if(RefreshFlag) PAGE_3_24();

}



void SYS_3_00_00(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU = 30015;
	if(RefreshFlag) PAGE_3_00_00();

}
void SYS_3_00_01(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_00_01();
	 
	 
	 
}
void SYS_3_00_02(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_00_02();
	 
	 
	 
}
void SYS_3_00_03(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_00_03();
	 
	 
	 
}
void SYS_3_00_04(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_00_04();
	 
	 
	 
}
void SYS_3_00_05(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_00_05(); 
	 
	 
}
void SYS_3_00_06(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_00_06(); 
	 
	 
}
void SYS_3_00_07(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_00_07(); 
	 
	 
}
void SYS_3_00_08(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_00_08(); 
	 
	 
}
void SYS_3_00_09(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_00_09(); 
	 
	 
}
void SYS_3_00_10(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_00_10(); 
	 
	 
}
void SYS_3_00_11(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_00_11(); 
	 
	 
}
void SYS_3_00_12(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_00_12(); 
	 
	 
}
void SYS_3_00_13(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_00_13(); 
	 
	 
}
void SYS_3_00_14(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_00_14(); 
	 
	 
}
void SYS_3_00_15(void)
{
 	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == DN)naviMENU = 30000;
	if(RefreshFlag) PAGE_3_00_15(); 
}
 



void SYS_3_01_00(void)
{
	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
		
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == UP)naviMENU = 30109;
		else if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,7);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,6);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}

	}
	 
	 if(RefreshFlag) PAGE_3_01_00(); 
}
void SYS_3_01_01(void)
{
 	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,9);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,7);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}

	}
	 if(RefreshFlag) PAGE_3_01_01(); 
	 
	 
}
void SYS_3_01_02(void)
{
 	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,9);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,7);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
	}
	 if(RefreshFlag) PAGE_3_01_02(); 
	 
	 
}
void SYS_3_01_03(void)
{
 	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,9);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,7);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
	}
	 if(RefreshFlag) PAGE_3_01_03(); 
	 
	 
}
void SYS_3_01_04(void)
{
 	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,9);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,7);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
	}
	 if(RefreshFlag) PAGE_3_01_04(); 
	 
	 
}
void SYS_3_01_05(void)
{
 	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,9);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,7);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
	}
	 if(RefreshFlag) PAGE_3_01_05(); 
	 
	 
}
void SYS_3_01_06(void)
{
 	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,9);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,7);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
	}
	 if(RefreshFlag) PAGE_3_01_06(); 
	 
	 
}
void SYS_3_01_07(void)
{
 	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,9);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,7);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}

	}
	if(RefreshFlag) PAGE_3_01_07();  
	 
	 
}
void SYS_3_01_08(void)
{
 	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,9);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,7);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
	}
	if(RefreshFlag) PAGE_3_01_08();  
	 
	 
}
void SYS_3_01_09(void)
{
 	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == DN)naviMENU = 30100;
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,9);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,7);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
	}
	
	if(RefreshFlag) PAGE_3_01_09();  
}
 


void SYS_3_02_00(void)
{
  	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU = 30209;
	if(RefreshFlag) PAGE_3_02_00(); 
	 
	 
}
void SYS_3_02_01(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_02_01();  
	 
	 
}
void SYS_3_02_02(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_02_02();  
	 
	 
}
void SYS_3_02_03(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_02_03();  
	 
	 
}
void SYS_3_02_04(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_02_04();  
	 
	 
}
void SYS_3_02_05(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_02_05();  
	 
	 
}
void SYS_3_02_06(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_02_06();  
	 
	 
}
void SYS_3_02_07(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_02_07();  
	 
	 
}
void SYS_3_02_08(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_02_08();  
	 
	 
}
void SYS_3_02_09(void)
{
  	SYS_Base_KeyFunction();
	 if(KeyState.KeyValue == DN)naviMENU = 30200;
	 if(RefreshFlag) PAGE_3_02_09(); 
	 
}
 


void SYS_3_03_00(void)
{
  	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == UP)naviMENU =30352;
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,9);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,7);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}

	}
	
	if(RefreshFlag) PAGE_3_03_00(); 
	 
	 
}
void SYS_3_03_01(void)
{
  	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,9);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,7);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
	}
	if(RefreshFlag) PAGE_3_03_01();
	 
	 
}
void SYS_3_03_02(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_02(); 
	 
	 
}
void SYS_3_03_03(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_03(); 
	 
	 
}
void SYS_3_03_04(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_04(); 
	 
	 
}
void SYS_3_03_05(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_05(); 
	 
	 
}
void SYS_3_03_06(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_06(); 
	 
	 
}
void SYS_3_03_07(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_07(); 
	 
	 
}
void SYS_3_03_08(void)
{
  	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,9);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,7);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
	}
	if(RefreshFlag) PAGE_3_03_08(); 
	 
	 
}
void SYS_3_03_09(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_09(); 
	 
	 
}
void SYS_3_03_10(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_10(); 
	 
	 
}
void SYS_3_03_11(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_11(); 
	 
	 
}
void SYS_3_03_12(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_12(); 
	 
	 
}
void SYS_3_03_13(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_13(); 
	 
	 
}
void SYS_3_03_14(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_14(); 
	 
	 
}
void SYS_3_03_15(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_15(); 
	 
	 
}
void SYS_3_03_16(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_16(); 
	 
	 
}
void SYS_3_03_17(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_17(); 
	 
	 
}
void SYS_3_03_18(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_18(); 
	 
	 
}
void SYS_3_03_19(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_19(); 
	 
	 
}
void SYS_3_03_20(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_20(); 
	 
	 
}
void SYS_3_03_21(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_21(); 
	 
	 
}
void SYS_3_03_22(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_22(); 
	 
	 
}
void SYS_3_03_23(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_23(); 
	 
	 
}
void SYS_3_03_24(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_24(); 
	 
	 
}
void SYS_3_03_25(void)
{
  	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,9);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,7);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}


	}
	if(RefreshFlag) PAGE_3_03_25(); 
	 
	 
}
void SYS_3_03_26(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_26(); 
	 
	 
}
void SYS_3_03_27(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_27(); 
	 
	 
}
void SYS_3_03_28(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_28(); 
	 
	 
}
void SYS_3_03_29(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_29(); 
	 
	 
}
void SYS_3_03_30(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_30(); 
	 
	 
}
void SYS_3_03_31(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_31(); 
	 
	 
}
void SYS_3_03_32(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_32(); 
	 
	 
}
void SYS_3_03_33(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_33(); 
	 
	 
}
void SYS_3_03_34(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_34(); 
	 
	 
}
void SYS_3_03_35(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_35(); 
	 
	 
}
void SYS_3_03_36(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_36(); 
	 
	 
}
void SYS_3_03_37(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_37(); 
	 
	 
}
void SYS_3_03_38(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_38(); 
	 
	 
}
void SYS_3_03_39(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_39(); 
	 
	 
}
void SYS_3_03_40(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_40(); 
	 
	 
}
void SYS_3_03_41(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_41(); 
	 
	 
}
void SYS_3_03_42(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_42(); 
	 
	 
}
void SYS_3_03_43(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_43(); 
	 
	 
}
void SYS_3_03_44(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_44(); 
	 
	 
}
void SYS_3_03_45(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_45(); 
	 
	 
}
void SYS_3_03_46(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_46(); 
	 
	 
}
void SYS_3_03_47(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_47(); 
	 
	 
}
void SYS_3_03_48(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_48(); 
	 
	 
}
void SYS_3_03_49(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_49(); 
	 
	 
}
void SYS_3_03_50(void)
{
  	SYS_Base_KeyFunction();
	 if(RefreshFlag) PAGE_3_03_50();
	 
	 
}
void SYS_3_03_51(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_03_51(); 
	 
	 
}
void SYS_3_03_52(void)
{
  	SYS_Base_KeyFunction();
	 
	if(KeyState.KeyValue == DN)naviMENU = 30300;
	if(RefreshFlag) PAGE_3_03_52();
}
 



void SYS_3_04_00(void)
{
  	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU = 30452;
	if(RefreshFlag) PAGE_3_04_00(); 
	 
}
void SYS_3_04_01(void)
{
  	SYS_Base_KeyFunction();
	 if(RefreshFlag) PAGE_3_04_01();
	 
	 
}
void SYS_3_04_02(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_02(); 
	 
	 
}
void SYS_3_04_03(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_03(); 
	 
	 
}
void SYS_3_04_04(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_04(); 
	 
	 
}
void SYS_3_04_05(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_05(); 
	 
	 
}
void SYS_3_04_06(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_06(); 
	 
	 
}
void SYS_3_04_07(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_07(); 
	 
	 
}
void SYS_3_04_08(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_08(); 
	 
	 
}
void SYS_3_04_09(void)
{
  	SYS_Base_KeyFunction();
	 if(RefreshFlag) PAGE_3_04_09();
	 
	 
}
void SYS_3_04_10(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_10(); 
	 
	 
}
void SYS_3_04_11(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_11(); 
	 
	 
}
void SYS_3_04_12(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_12(); 
	 
	 
}
void SYS_3_04_13(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_13(); 
	 
	 
}
void SYS_3_04_14(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_14(); 
	 
	 
}
void SYS_3_04_15(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_15(); 
	 
	 
}
void SYS_3_04_16(void)
{
  	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_16(); 
	 
	 
}
void SYS_3_04_17(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_17(); 
	 
	 
}
void SYS_3_04_18(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_18(); 
	 
	 
}
void SYS_3_04_19(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_19(); 
	 
	 
}
void SYS_3_04_20(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_20(); 
	 
	 
}
void SYS_3_04_21(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_21(); 
	 
	 
}
void SYS_3_04_22(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_22(); 
	 
	 
}
void SYS_3_04_23(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_23(); 
	 
	 
}
void SYS_3_04_24(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_24(); 
	 
	 
}
void SYS_3_04_25(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_25(); 
	 
	 
}
void SYS_3_04_26(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_26(); 
	 
	 
}
void SYS_3_04_27(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_27(); 
	 
	 
}
void SYS_3_04_28(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_28(); 
	 
	 
}
void SYS_3_04_29(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_29(); 
	 
	 
}
void SYS_3_04_30(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_30(); 
	 
	 
}
void SYS_3_04_31(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_31(); 
	 
	 
}
void SYS_3_04_32(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_32(); 
	 
	 
}
void SYS_3_04_33(void)
{
 	SYS_Base_KeyFunction();
	 if(RefreshFlag) PAGE_3_03_33();
	 
	 
}
void SYS_3_04_34(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_34(); 
	 
	 
}
void SYS_3_04_35(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_35(); 
	 
	 
}
void SYS_3_04_36(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_36(); 
	 
	 
}
void SYS_3_04_37(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_37(); 
	 
	 
}
void SYS_3_04_38(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_38();
	 
	 
}
void SYS_3_04_39(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_39(); 
	 
	 
}
void SYS_3_04_40(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_40(); 
	 
	 
}
void SYS_3_04_41(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_41(); 
	 
	 
}
void SYS_3_04_42(void)
{
 	SYS_Base_KeyFunction();
	 if(RefreshFlag) PAGE_3_04_42();
	 
	 
}
void SYS_3_04_43(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_43(); 
	 
	 
}
void SYS_3_04_44(void)
{
 	SYS_Base_KeyFunction();
	 if(RefreshFlag) PAGE_3_04_44();
	 
	 
}
void SYS_3_04_45(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_45(); 
	 
	 
}
void SYS_3_04_46(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_46(); 
	 
	 
}
void SYS_3_04_47(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_47(); 
	 
	 
}
void SYS_3_04_48(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_48(); 
	 
	 
}
void SYS_3_04_49(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_49(); 
	 
	 
}
void SYS_3_04_50(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_50(); 
	 
	 
}
void SYS_3_04_51(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_04_51(); 
	 
	 
}
void SYS_3_04_52(void)
{
 	SYS_Base_KeyFunction();
	 
	if(KeyState.KeyValue == DN)naviMENU =30400;
	if(RefreshFlag) PAGE_3_04_52(); 
}



 
void SYS_3_05_00(void)
{
 	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == UP)naviMENU = 30544;
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{

			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,9);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,7);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}

	}
	if(RefreshFlag) PAGE_3_05_00();
	 
	 
}
void SYS_3_05_01(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_05_01(); 
	 
	 
}
void SYS_3_05_02(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_05_02(); 
	 
	 
}
void SYS_3_05_03(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_05_03(); 
	 
	 
}
void SYS_3_05_04(void)
{
 	SYS_Base_KeyFunction();
	 if(RefreshFlag) PAGE_3_05_04();
	 
	 
}
void SYS_3_05_05(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_05_05(); 
	 
	 
}
void SYS_3_05_06(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_05_06(); 
	 
	 
}
void SYS_3_05_07(void)
{
 	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,9);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,7);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
	}
	if(RefreshFlag) PAGE_3_05_07(); 
	 
	 
}
void SYS_3_05_08(void)
{
 	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,9);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,7);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
	}
	if(RefreshFlag) PAGE_3_05_08(); 
	 
	 
}
void SYS_3_05_09(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_05_09(); 
	 
	 
}
void SYS_3_05_10(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_05_10(); 
	 
	 
}
void SYS_3_05_11(void)
{
 	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,9);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,7);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
		}
	 if(RefreshFlag) PAGE_3_05_11();
	 
	 
}
void SYS_3_05_12(void)
{
 	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,9);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,7);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
		}
	


	if(RefreshFlag) PAGE_3_05_12(); 
	 
	 
}
void SYS_3_05_13(void)
{
 	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,9);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,7);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
		}
	
	if(RefreshFlag) PAGE_3_05_13(); 
	 
	 
}
void SYS_3_05_14(void)
{
 	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,9);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,7);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
		}
	
	if(RefreshFlag) PAGE_3_05_14(); 
	 
	 
}
void SYS_3_05_15(void)
{
 	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,9);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,7);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
		}
	
	if(RefreshFlag) PAGE_3_05_15(); 
	 
	 
}
void SYS_3_05_16(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_05_16(); 
	 
	 
}
void SYS_3_05_17(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_05_17(); 
	 
	 
}
void SYS_3_05_18(void)
{
 	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,9);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,7);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
		}
	

	if(RefreshFlag) PAGE_3_05_18(); 
	 
	 
}
void SYS_3_05_19(void)
{
 	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,9);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,7);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
		}
	
	if(RefreshFlag) PAGE_3_05_19(); 
	 
	 
}
void SYS_3_05_20(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_05_20(); 
	 
	 
}
void SYS_3_05_21(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_05_21(); 
	 
	 
}
void SYS_3_05_22(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_05_22(); 
	 
	 
}
void SYS_3_05_23(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_05_23(); 
	 
	 
}
void SYS_3_05_24(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_05_24(); 
	 
	 
}
void SYS_3_05_25(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_05_25(); 
	 
	 
}
void SYS_3_05_26(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_05_26(); 
	 
	 
}
void SYS_3_05_27(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_05_27(); 
	 
	 
}
void SYS_3_05_28(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_05_28(); 
	 
	 
}
void SYS_3_05_29(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_05_29(); 
	 
	 
}
void SYS_3_05_30(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_05_30(); 
	 
	 
}
void SYS_3_05_31(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_05_31(); 
	 
	 
}
void SYS_3_05_32(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_05_32(); 
	 
	 
}
void SYS_3_05_33(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_05_33(); 
	 
	 
}
void SYS_3_05_34(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_05_34(); 
	 
	 
}
void SYS_3_05_35(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_05_35(); 
	 
	 
}
void SYS_3_05_36(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_05_36(); 
	 
	 
}
void SYS_3_05_37(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_05_37(); 
	 
	 
}
void SYS_3_05_38(void)
{
 	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,9);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,7);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
		}
	
	if(RefreshFlag) PAGE_3_05_38(); 
	 
	 
}
void SYS_3_05_39(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_05_39(); 
	 
	 
}
void SYS_3_05_40(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_05_40(); 
	 
	 
}
void SYS_3_05_41(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_05_41(); 
	 
	 
}
void SYS_3_05_42(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_05_42(); 
	 
	 
}
void SYS_3_05_43(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_05_43(); 
	 
	 
}
void SYS_3_05_44(void)
{
 	SYS_Base_KeyFunction();
	 
	if(KeyState.KeyValue == DN)naviMENU = 30500;
	if(RefreshFlag) PAGE_3_05_44();
	 
}


 
void SYS_3_06_00(void)
{
 	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU = 30670;
	if(RefreshFlag) PAGE_3_06_00();
	 
	 
}
void SYS_3_06_01(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_01(); 
	 
	 
}
void SYS_3_06_02(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_02(); 
	 
	 
}
void SYS_3_06_03(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_03(); 
	 
	 
}
void SYS_3_06_04(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_04(); 
	 
	 
}
void SYS_3_06_05(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_05(); 
	 
	 
}
void SYS_3_06_06(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_06(); 
	 
	 
}
void SYS_3_06_07(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_07(); 
	 
	 
}
void SYS_3_06_08(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_08(); 
	 
	 
}
void SYS_3_06_09(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_09(); 
	 
	 
}
void SYS_3_06_10(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_10(); 
	 
	 
}
void SYS_3_06_11(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_11(); 
	 
	 
}
void SYS_3_06_12(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_12(); 
	 
	 
}
void SYS_3_06_13(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_13(); 
	 
	 
}
void SYS_3_06_14(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_14(); 
	 
	 
}
void SYS_3_06_15(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_15(); 
	 
	 
}
void SYS_3_06_16(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_16(); 
	 
	 
}
void SYS_3_06_17(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_17(); 
	 
	 
}
void SYS_3_06_18(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_18(); 
	 
	 
}
void SYS_3_06_19(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_19();
	 
	 
}
void SYS_3_06_20(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_20(); 
	 
	 
}
void SYS_3_06_21(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_21(); 
	 
	 
}
void SYS_3_06_22(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_22(); 
	 
	 
}
void SYS_3_06_23(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_23(); 
	 
	 
}
void SYS_3_06_24(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_24(); 
	 
	 
}
void SYS_3_06_25(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_25(); 
	 
	 
}
void SYS_3_06_26(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_26(); 
	 
	 
}
void SYS_3_06_27(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_27(); 
	 
	 
}
void SYS_3_06_28(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_28(); 
	 
	 
}
void SYS_3_06_29(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_29(); 
	 
	 
}
void SYS_3_06_30(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_30(); 
	 
	 
}
void SYS_3_06_31(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_31(); 
	 
	 
}
void SYS_3_06_32(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_32(); 
	 
	 
}
void SYS_3_06_33(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_33(); 
	 
	 
}
void SYS_3_06_34(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_34(); 
	 
	 
}
void SYS_3_06_35(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_35(); 
	 
	 
}
void SYS_3_06_36(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_36(); 
	 
	 
}
void SYS_3_06_37(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_37(); 
	 
	 
}
void SYS_3_06_38(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_38(); 
	 
	 
}
void SYS_3_06_39(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_39(); 
	 
	 
}
void SYS_3_06_40(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_40(); 
	 
	 
}
void SYS_3_06_41(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_41(); 
	 
	 
}
void SYS_3_06_42(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_42(); 
	 
	 
}
void SYS_3_06_43(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_43(); 
	 
	 
}
void SYS_3_06_44(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_44(); 
	 
	 
}
void SYS_3_06_45(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_45(); 
	 
	 
}
void SYS_3_06_46(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_46(); 
	 
	 
}
void SYS_3_06_47(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_47(); 
	 
	 
}
void SYS_3_06_48(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_48(); 
	 
	 
}
void SYS_3_06_49(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_49(); 
	 
	 
}
void SYS_3_06_50(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_50(); 
	 
	 
}
void SYS_3_06_51(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_51(); 
	 
	 
}
void SYS_3_06_52(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_52(); 
	 
	 
}
void SYS_3_06_53(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_53(); 
	 
	 
}
void SYS_3_06_54(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_54(); 
	 
	 
}
void SYS_3_06_55(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_55(); 
	 
	 
}
void SYS_3_06_56(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_56(); 
	 
	 
}
void SYS_3_06_57(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_57(); 
	 
	 
}
void SYS_3_06_58(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_58(); 
	 
	 
}
void SYS_3_06_59(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_59(); 
	 
	 
}
void SYS_3_06_60(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_60(); 
	 
	 
}
void SYS_3_06_61(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_61(); 
	 
	 
}
void SYS_3_06_62(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_62(); 
	 
	 
}
void SYS_3_06_63(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_63(); 
	 
	 
}
void SYS_3_06_64(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_64(); 
	 
	 
}
void SYS_3_06_65(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_65(); 
	 
	 
}
void SYS_3_06_66(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_66();
	 
	 
}
void SYS_3_06_67(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_67(); 
	 
	 
}
void SYS_3_06_68(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_68(); 
	 
	 
}
void SYS_3_06_69(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_06_69(); 
	 
	 
}
void SYS_3_06_70(void)
{
 	SYS_Base_KeyFunction();
	 
	if(KeyState.KeyValue == DN)naviMENU = 30600;
	if(RefreshFlag) PAGE_3_06_70(); 
}



 
void SYS_3_07_00(void)
{
 	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU = 30728;
	if(RefreshFlag) PAGE_3_07_00(); 
	 
	 
}
void SYS_3_07_01(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_07_01();  
	 
	 
}
void SYS_3_07_02(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_07_02();  
	 
	 
}
void SYS_3_07_03(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_07_03();  
	 
	 
}
void SYS_3_07_04(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_07_04();  
	 
	 
}
void SYS_3_07_05(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_07_05();  
	 
	 
}
void SYS_3_07_06(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_07_06();  
	 
	 
}
void SYS_3_07_07(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_07_07();  
	 
	 
}
void SYS_3_07_08(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_07_08();  
	 
	 
}
void SYS_3_07_09(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_07_09();  
	 
	 
}
void SYS_3_07_10(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_07_10(); 
	 
	 
	 
}
void SYS_3_07_11(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_07_11();  
	 
	 
}
void SYS_3_07_12(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_07_12();  
	 
	 
}
void SYS_3_07_13(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_07_13();  
	 
	 
}
void SYS_3_07_14(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_07_14();  
	 
	 
}
void SYS_3_07_15(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_07_15();  
	 
	 
}
void SYS_3_07_16(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_07_16();  
	 
	 
}
void SYS_3_07_17(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_07_17();  
	 
	 
}
void SYS_3_07_18(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_07_18();  
	 
	 
}
void SYS_3_07_19(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_07_19();  
	 
	 
}
void SYS_3_07_20(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_07_20();  
	 
	 
}
void SYS_3_07_21(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_07_21();  
	 
	 
}
void SYS_3_07_22(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_07_22();  
	 
	 
}
void SYS_3_07_23(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_07_23();  
	 
	 
}
void SYS_3_07_24(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_07_24();  
	 
	 
}
void SYS_3_07_25(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_07_25();  
	 
	 
}
void SYS_3_07_26(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_07_26();  
	 
	 
}
void SYS_3_07_27(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_07_27();  
	 
	 
}
void SYS_3_07_28(void)
{
 	SYS_Base_KeyFunction();
	 
	if(KeyState.KeyValue == DN)naviMENU = 30700;
	if(RefreshFlag) PAGE_3_07_28();  
}


 void SYS_3_08_00(void)
{
 	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU = 30819;
	if(RefreshFlag) PAGE_3_08_00(); 
	 
	 
}
void SYS_3_08_01(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_08_01();  
	 
	 
}
void SYS_3_08_02(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_08_02();  
	 
	 
}
void SYS_3_08_03(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_08_03();  
	 
	 
}
void SYS_3_08_04(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_08_04(); 
	 
	 
}
void SYS_3_08_05(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_08_05();  
	 
	 
}
void SYS_3_08_06(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_08_06();  
	 
	 
}
void SYS_3_08_07(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_08_07();  
	 
	 
}
void SYS_3_08_08(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_08_08();  
	 
	 
}
void SYS_3_08_09(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_08_09();  
	 
	 
}
void SYS_3_08_10(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_08_10();  
	 
	 
}
void SYS_3_08_11(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_08_11(); 
	 
	 
}
void SYS_3_08_12(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_08_12();  
	 
	 
}
void SYS_3_08_13(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_08_13();  
	 
	 
}
void SYS_3_08_14(void)
{
 	SYS_Base_KeyFunction();
	 if(RefreshFlag) PAGE_3_08_14(); 
	 
	 
}
void SYS_3_08_15(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_08_15();  
	 
	 
}
void SYS_3_08_16(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_08_16();  
	 
	 
}
void SYS_3_08_17(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_08_17();  
	 
	 
}
void SYS_3_08_18(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_08_18();  
	 
	 
}
void SYS_3_08_19(void)
{
 	SYS_Base_KeyFunction();
	 
	if(KeyState.KeyValue == DN)naviMENU =30800;
	if(RefreshFlag) PAGE_3_08_19();  
}


void SYS_3_09_00(void)
{
 	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU = 30916;
	 if(RefreshFlag) PAGE_3_09_00(); 
	 
}
void SYS_3_09_01(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_09_01();  
	 
	 
}
void SYS_3_09_02(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_09_02();  
	 
	 
}
void SYS_3_09_03(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_09_03();  
	 
	 
}
void SYS_3_09_04(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_09_04();  
	 
	 
}
void SYS_3_09_05(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_09_05();  
	 
	 
}
void SYS_3_09_06(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_09_06();  
	 
	 
}
void SYS_3_09_07(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_09_07();  
	 
	 
}
void SYS_3_09_08(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_09_08();  
	 
	 
}
void SYS_3_09_09(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_09_09();  
	 
	 
}
void SYS_3_09_10(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_09_10();  
	 
	 
}
void SYS_3_09_11(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_09_11();  
	 
	 
}
void SYS_3_09_12(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_09_12();  
	 
	 
}
void SYS_3_09_13(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_09_13();  
	 
	 
}
void SYS_3_09_14(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_09_14();  
	 
	 
}
void SYS_3_09_15(void)
{
 	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_09_15();  
	 
	 
}
void SYS_3_09_16(void)
{
 	SYS_Base_KeyFunction();
	 
	if(KeyState.KeyValue == DN)naviMENU = 30900;
	if(RefreshFlag) PAGE_3_09_16(); 
	 
}


 
void SYS_3_10_00(void)
{
 	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU = 31016;
	if(RefreshFlag) PAGE_3_10_00(); 
	 
}
void SYS_3_10_01(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_10_01();  
	 
	 
}
void SYS_3_10_02(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_10_02();  
	 
	 
}
void SYS_3_10_03(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_10_03();  
	 
	 
}
void SYS_3_10_04(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_10_04();  
	 
	 
}
void SYS_3_10_05(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_10_05();  
	 
	 
}
void SYS_3_10_06(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_10_06();  
	 
	 
}
void SYS_3_10_07(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_10_07();  
	 
	 
}
void SYS_3_10_08(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_10_08();  
	 
	 
}
void SYS_3_10_09(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_10_09();  
	 
	 
}
void SYS_3_10_10(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_10_10();  
	 
	 
}
void SYS_3_10_11(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_10_11();  
	 
	 
}
void SYS_3_10_12(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_10_12();  
	 
	 
}
void SYS_3_10_13(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_10_13();  
	 
	 
}
void SYS_3_10_14(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_10_14();  
	 
	 
}
void SYS_3_10_15(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_10_15();  
	 
	 
}
void SYS_3_10_16(void)
{
	SYS_Base_KeyFunction();
	 
	if(KeyState.KeyValue == DN)naviMENU = 31000;
	if(RefreshFlag) PAGE_3_10_16();  
}


void SYS_3_11_00(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU = 31106;
	if(RefreshFlag) PAGE_3_11_00();
	 
}
void SYS_3_11_01(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_11_01(); 
	 
	 
}
void SYS_3_11_02(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_11_02(); 
	 
	 
}
void SYS_3_11_03(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_11_03();
	 
	 
}
void SYS_3_11_04(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_11_04(); 
	 
	 
}
void SYS_3_11_05(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_11_05(); 
	 
	 
}
void SYS_3_11_06(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == DN)naviMENU = 31100;
	if(RefreshFlag) PAGE_3_11_06();
	 
}



 void SYS_3_12_00(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU = 31207;
	if(RefreshFlag) PAGE_3_12_00(); 
	 
}
void SYS_3_12_01(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_12_01(); 
	 
	 
}
void SYS_3_12_02(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_12_02();  
	 
	 
}
void SYS_3_12_03(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_12_03();  
	 
	 
}
void SYS_3_12_04(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_12_04();  
	 
	 
}
void SYS_3_12_05(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_12_05();  
	 
	 
}
void SYS_3_12_06(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_12_06();  
	 
	 
}
void SYS_3_12_07(void)
{
	SYS_Base_KeyFunction();
	 if(KeyState.KeyValue == DN)naviMENU = 31200;
	 if(RefreshFlag) PAGE_3_12_07(); 
	 
}



 void SYS_3_13_00(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU = 31311;
	 
	 if(RefreshFlag) PAGE_3_13_00(); 
}
void SYS_3_13_01(void)
{
	SYS_Base_KeyFunction();
	 if(RefreshFlag) PAGE_3_13_01(); 
	 
	 
}
void SYS_3_13_02(void)
{
	SYS_Base_KeyFunction();
	 if(RefreshFlag) PAGE_3_13_02(); 
	 
	 
}
void SYS_3_13_03(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_13_03();  
	 
	 
}
void SYS_3_13_04(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_13_04();  
	 
	 
}
void SYS_3_13_05(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_13_05();  
	 
	 
}
void SYS_3_13_06(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_13_06();  
	 
	 
}
void SYS_3_13_07(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_13_07();  
	 
	 
}
void SYS_3_13_08(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_13_08();  
	 
	 
}
void SYS_3_13_09(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_13_08();  
	 
	 
}
void SYS_3_13_10(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_13_10();  
	 
	 
}
void SYS_3_13_11(void)
{
	SYS_Base_KeyFunction();
	 
	if(KeyState.KeyValue == DN)naviMENU = 31300;
	if(RefreshFlag) PAGE_3_13_11();  
}



void SYS_3_14_00(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU = 31408;
	if(RefreshFlag) PAGE_3_14_00();  
	 
}
void SYS_3_14_01(void)
{
	SYS_Base_KeyFunction();
	 if(RefreshFlag) PAGE_3_14_01(); 
	 
	 
}
void SYS_3_14_02(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_14_02();  
	 
	 
}
void SYS_3_14_03(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_14_03();  
	 
	 
}
void SYS_3_14_04(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_14_04();  
	 
	 
}
void SYS_3_14_05(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_14_05();  
	 
	 
}
void SYS_3_14_06(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_14_06();  
	 
	 
}
void SYS_3_14_07(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_14_07();  
	 
	 
}
void SYS_3_14_08(void)
{
	SYS_Base_KeyFunction();
	 
	if(KeyState.KeyValue == DN)naviMENU = 31400;
	if(RefreshFlag) PAGE_3_14_08();  
}




void SYS_3_15_00(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU = 31528;
	if(RefreshFlag) PAGE_3_15_00();  
	 
}
void SYS_3_15_01(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_15_01(); 
	 
	 
}
void SYS_3_15_02(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_15_02();  
	 
	 
}
void SYS_3_15_03(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_15_03(); 
	 
	 
}
void SYS_3_15_04(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_15_04();  
	 
	 
}
void SYS_3_15_05(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_15_05();  
	 
	 
}
void SYS_3_15_06(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_15_06();  
	 
	 
}
void SYS_3_15_07(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_15_07();  
	 
	 
}
void SYS_3_15_08(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_15_08();  
	 
	 
}
void SYS_3_15_09(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_15_09();  
	 
	 
}
void SYS_3_15_10(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_15_10();  
	 
	 
}
void SYS_3_15_11(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_15_11();  
	 
	 
}
void SYS_3_15_12(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_15_12();  
	 
	 
}
void SYS_3_15_13(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_15_13();  
	 
	 
}
void SYS_3_15_14(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_15_14();  
	 
	 
}
void SYS_3_15_15(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_15_15();  
	 
	 
}
void SYS_3_15_16(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_15_16();  
	 
	 
}
void SYS_3_15_17(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_15_17(); 
	 
	 
}
void SYS_3_15_18(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_15_18();  
	 
	 
}
void SYS_3_15_19(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_15_19();  
	 
	 
}
void SYS_3_15_20(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_15_20();  
	 
	 
}
void SYS_3_15_21(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_15_21();  
	 
	 
}
void SYS_3_15_22(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_15_22();  
	 
	 
}
void SYS_3_15_23(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_15_23();  
	 
	 
}
void SYS_3_15_24(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_15_24();  
	 
	 
}
void SYS_3_15_25(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_15_25();  
	 
	 
}
void SYS_3_15_26(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_15_26();  
	 
	 
}
void SYS_3_15_27(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_15_27();  
	 
	 
}
void SYS_3_15_28(void)
{
	SYS_Base_KeyFunction();
	 
	if(KeyState.KeyValue == DN)naviMENU = 31500;
	if(RefreshFlag) PAGE_3_15_28();  
}



void SYS_3_16_00(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU = 31628;
	if(RefreshFlag) PAGE_3_16_00();  
	 
}
void SYS_3_16_01(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_16_01();   
	 
	 
}
void SYS_3_16_02(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_16_02();   
	 
	 
}
void SYS_3_16_03(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_16_03();   
	 
	 
}
void SYS_3_16_04(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_16_04();   
	 
	 
}
void SYS_3_16_05(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_16_05();   
	 
	 
}
void SYS_3_16_06(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_16_06();   
	 
	 
}
void SYS_3_16_07(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_16_07();   
	 
	 
}
void SYS_3_16_08(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_16_08();   
	 
	 
}
void SYS_3_16_09(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_16_09();   
	 
	 
}
void SYS_3_16_10(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_16_10();   
	 
	 
}
void SYS_3_16_11(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_16_11();   
	 
	 
}
void SYS_3_16_12(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_16_12();   
	 
	 
}
void SYS_3_16_13(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_16_13();   
	 
	 
}
void SYS_3_16_14(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_16_14();   
	 
	 
}
void SYS_3_16_15(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_16_15();   
	 
	 
}
void SYS_3_16_16(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_16_16();   
	 
	 
}
void SYS_3_16_17(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_16_17();   
	 
	 
}
void SYS_3_16_18(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_16_18();   
	 
	 
}
void SYS_3_16_19(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_16_19();   
	 
	 
}
void SYS_3_16_20(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_16_20();   
	 
	 
}
void SYS_3_16_21(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_16_21();   
	 
	 
}
void SYS_3_16_22(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_16_22();   
	 
	 
}
void SYS_3_16_23(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_16_23();   
	 
	 
}
void SYS_3_16_24(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_16_24();   
	 
	 
}
void SYS_3_16_25(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_16_25();   
	 
	 
}
void SYS_3_16_26(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_16_26();   
	 
	 
}
void SYS_3_16_27(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_16_27();   
	 
	 
}
void SYS_3_16_28(void)
{
	SYS_Base_KeyFunction();
	 
	if(KeyState.KeyValue == DN)naviMENU = 31600;
	if(RefreshFlag) PAGE_3_16_28();  
}



void SYS_3_17_00(void)
{         
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU =31751;
	if(RefreshFlag) PAGE_3_17_00();   
	      
}              
void SYS_3_17_01(void)
{
	if(!Edit_flag)
	{

		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
		
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,7);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,6);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
		}
	

	if(RefreshFlag) PAGE_3_17_01();    
	 
	 
}
void SYS_3_17_02(void)
{
	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
		
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,7);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,6);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
		}
	

	if(RefreshFlag) PAGE_3_17_02();    
	 
	 
}
void SYS_3_17_03(void)
{
	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
		
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,7);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,6);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
		}
	
	if(RefreshFlag) PAGE_3_17_03();    
	 
	 
}
void SYS_3_17_04(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_04();    
	 
	 
}
void SYS_3_17_05(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_05();    
	 
	 
}
void SYS_3_17_06(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_06();    
	 
	 
}
void SYS_3_17_07(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_07();    
	 
	 
}
void SYS_3_17_08(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_08();    
	 
	 
}
void SYS_3_17_09(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_09();    
	 
	 
}
void SYS_3_17_10(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_10();    
	 
	 
}
void SYS_3_17_11(void)
{
	if(!Edit_flag)
	{
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,7);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,6);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
		}
	


	if(RefreshFlag) PAGE_3_17_11();    
	 
	 
}
void SYS_3_17_12(void)
{
	if(!Edit_flag)
	{
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{

			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,7);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,6);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
		}
	

	if(RefreshFlag) PAGE_3_17_12();    
	 
	 
}
void SYS_3_17_13(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_13();    
	 
	 
}
void SYS_3_17_14(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_14();    
	 
	 
}
void SYS_3_17_15(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_15();    
	 
	 
}
void SYS_3_17_16(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_16();    
	 
	 
}
void SYS_3_17_17(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_17();    
	 
	 
}
void SYS_3_17_18(void)
{
	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
		
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,7);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,6);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
		}

	if(RefreshFlag) PAGE_3_17_18();    
	 
	 
}
void SYS_3_17_19(void)
{
	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
		
 		SYS_Base_KeyFunction();
		 if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,7);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,6);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
		}
	
	if(RefreshFlag) PAGE_3_17_19();    
	 
	 
}
void SYS_3_17_20(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_20();    
	 
	 
}
void SYS_3_17_21(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_21();    
	 
	 
}
void SYS_3_17_22(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_22();    
	 
	 
}
void SYS_3_17_23(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_23();    
	 
	 
}
void SYS_3_17_24(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_24();    
	 
	 
}
void SYS_3_17_25(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_25();    
	 
	 
}
void SYS_3_17_26(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_26();    
	 
	 
}
void SYS_3_17_27(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_27();    
	 
	 
}
void SYS_3_17_28(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_28();    
	 
	 
}
void SYS_3_17_29(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_29();    
	 
	 
}
void SYS_3_17_30(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_30();    
	 
	 
}
void SYS_3_17_31(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_31();    
	 
	 
}
void SYS_3_17_32(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_32();    
	 
	 
}
void SYS_3_17_33(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_33();    
	 
	 
}
void SYS_3_17_34(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_34();    
	 
	 
}
void SYS_3_17_35(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_35();    
	 
	 
}
void SYS_3_17_36(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_36();    
	 
	 
}
void SYS_3_17_37(void)
{
	SYS_Base_KeyFunction();
	 if(RefreshFlag) PAGE_3_17_37();   
	 
	 
}
void SYS_3_17_38(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_38();    
	 
	 
}
void SYS_3_17_39(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_39();    
	 
	 
}
void SYS_3_17_40(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_40();  
	 
	 
}
void SYS_3_17_41(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_41();    
	 
	 
}
void SYS_3_17_42(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_42();    
	 
	 
}
void SYS_3_17_43(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_43();    
	 
	 
}
void SYS_3_17_44(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_44();    
	 
	 
}
void SYS_3_17_45(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_45();    
	 
	 
}
void SYS_3_17_46(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_46();    
	 
	 
}
void SYS_3_17_47(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_47();    
	 
	 
}
void SYS_3_17_48(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_48();    
	 
	 
}
void SYS_3_17_49(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_17_49();    
	 
	 
}
void SYS_3_17_50(void)
{
	if(!Edit_flag)
	{

 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,7);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,6);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
		}
	if(RefreshFlag) PAGE_3_17_50();    
	 
	 
}
void SYS_3_17_51(void)
{
	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
		
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == DN)naviMENU = 31700;
		else if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,7);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,6);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
		}
	

 
	if(RefreshFlag) PAGE_3_17_51();    
}





void SYS_3_18_00(void)
{   
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU = 31849;
	if(RefreshFlag) PAGE_3_18_00();    
	            
}              
void SYS_3_18_01(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_01();  
	 
	 
}
void SYS_3_18_02(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_02();  
	 
	 
}
void SYS_3_18_03(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_03();  
	 
	 
}
void SYS_3_18_04(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_04();  
	 
	 
}
void SYS_3_18_05(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_05();  
	 
	 
}
void SYS_3_18_06(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_06();  
	 
	 
}
void SYS_3_18_07(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_07();  
	 
	 
}
void SYS_3_18_08(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_08();  
	 
	 
}
void SYS_3_18_09(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_09();  
	 
	 
}
void SYS_3_18_10(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_10();  
	 
	 
}
void SYS_3_18_11(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_11();  
	 
}
void SYS_3_18_12(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_12();  
	 
	 
}
void SYS_3_18_13(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_13();  
	 
	 
}
void SYS_3_18_14(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_14();  
	 
	 
}
void SYS_3_18_15(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_15();  
	 
	 
}
void SYS_3_18_16(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_16();  
	 
	 
}
void SYS_3_18_17(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_17();  
	 
	 
}
void SYS_3_18_18(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_18();  
	 
	 
}
void SYS_3_18_19(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_19();  
	 
	 
}
void SYS_3_18_20(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_20();  
	 
	 
}
void SYS_3_18_21(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_21();  
	 
	 
}
void SYS_3_18_22(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_22();  
	 
	 
}
void SYS_3_18_23(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_23();  
	 
	 
}
void SYS_3_18_24(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_24();  
	 
	 
}
void SYS_3_18_25(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_25();  
	 
	 
}
void SYS_3_18_26(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_26();  
	 
	 
}
void SYS_3_18_27(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_27();  
	 
	 
}
void SYS_3_18_28(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_28();  
	 
	 
}
void SYS_3_18_29(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_29();  
	 
	 
}
void SYS_3_18_30(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_30();  
	 
	 
}
void SYS_3_18_31(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_31();  
	 
	 
}
void SYS_3_18_32(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_32();  
	 
	 
}
void SYS_3_18_33(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_33();  
	 
	 
}
void SYS_3_18_34(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_34();  
	 
	 
}
void SYS_3_18_35(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_35();  
	 
	 
}
void SYS_3_18_36(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_36();  
	 
	 
}
void SYS_3_18_37(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_37();  
	 
	 
}
void SYS_3_18_38(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_38();  
	 
	 
}
void SYS_3_18_39(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_39();  
	 
	 
}
void SYS_3_18_40(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_40();  
	 
	 
}
void SYS_3_18_41(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_41();  
	 
	 
}
void SYS_3_18_42(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_42();  
	 
	 
}
void SYS_3_18_43(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_42();  
	 
	 
}
void SYS_3_18_44(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_44();  
	 
	 
}
void SYS_3_18_45(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_45();  
	 
	 
}
void SYS_3_18_46(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_46();  
	 
	 
}
void SYS_3_18_47(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_47();  
	 
	 
}
void SYS_3_18_48(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_18_48();  
	 
	 
}
void SYS_3_18_49(void)
{
	SYS_Base_KeyFunction();
	 
	if(KeyState.KeyValue == DN)naviMENU = 31800;
	if(RefreshFlag) PAGE_3_18_49();  
}



void SYS_3_19_00(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU = 31941;
	if(RefreshFlag) PAGE_3_19_00();  
	               
}              
void SYS_3_19_01(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_01(); 
	 
	 
}
void SYS_3_19_02(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_02(); 
	 
	 
}
void SYS_3_19_03(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_03();	 
	 
	 
}
void SYS_3_19_04(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_04(); 
	 
	 
}
void SYS_3_19_05(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_05(); 
	 
	 
}
void SYS_3_19_06(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_06(); 
	 
	 
}
void SYS_3_19_07(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_07(); 
	 
	 
}
void SYS_3_19_08(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_08(); 
	 
	 
}
void SYS_3_19_09(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_09(); 
	 
	 
}
void SYS_3_19_10(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_10(); 
	 
	 
}
void SYS_3_19_11(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_11(); 
	 
	 
}
void SYS_3_19_12(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_12(); 
	 
	 
}
void SYS_3_19_13(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_13(); 
	 
	 
}
void SYS_3_19_14(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_14(); 
	 
	 
}
void SYS_3_19_15(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_15(); 
	 
	 
}
void SYS_3_19_16(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_16(); 
	 
	 
}
void SYS_3_19_17(void)
{
	SYS_Base_KeyFunction();
	 if(RefreshFlag) PAGE_3_19_17();
	 
	 
}
void SYS_3_19_18(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_18(); 
	 
	 
}
void SYS_3_19_19(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_19(); 
	 
	 
}
void SYS_3_19_20(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_20(); 
	 
	 
}
void SYS_3_19_21(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_21(); 
	 
	 
}
void SYS_3_19_22(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_22(); 
	 
	 
}
void SYS_3_19_23(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_23(); 
	 
	 
}
void SYS_3_19_24(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_24(); 
	 
	 
}
void SYS_3_19_25(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_25(); 
	 
	 
}
void SYS_3_19_26(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_26(); 
	 
	 
}
void SYS_3_19_27(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_27(); 
	 
	 
}
void SYS_3_19_28(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_28(); 
	 
	 
}
void SYS_3_19_29(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_29(); 
	 
	 
}
void SYS_3_19_30(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_30(); 
	 
	 
}
void SYS_3_19_31(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_31(); 
	 
	 
}
void SYS_3_19_32(void)
{
	SYS_Base_KeyFunction();
	 if(RefreshFlag) PAGE_3_19_32();
	 
	 
}
void SYS_3_19_33(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_33(); 
	 
	 
}
void SYS_3_19_34(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_34(); 
	 
	 
}
void SYS_3_19_35(void)
{
	SYS_Base_KeyFunction();
	 if(RefreshFlag) PAGE_3_19_35();
	 
	 
}
void SYS_3_19_36(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_36(); 
	 
	 
}
void SYS_3_19_37(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_37(); 
	 
	 
}
void SYS_3_19_38(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_38(); 
	 
	 
}
void SYS_3_19_39(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_39(); 
	 
	 
}
void SYS_3_19_40(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_19_40(); 
	 
	 
}
void SYS_3_19_41(void)
{
	SYS_Base_KeyFunction();
	 
	if(KeyState.KeyValue == DN)naviMENU = 31900;
	if(RefreshFlag) PAGE_3_19_41(); 
}




void SYS_3_20_00(void)
{   
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU = 32041;
	if(RefreshFlag) PAGE_3_20_00(); 
	            
}              
void SYS_3_20_01(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_01();  
	 
	 
}
void SYS_3_20_02(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_02();  
	 
	 
}
void SYS_3_20_03(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_03();  
	 
	 
}
void SYS_3_20_04(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_04();  
	 
	 
}
void SYS_3_20_05(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_05();  
	 
	 
}
void SYS_3_20_06(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_06();  
	 
	 
}
void SYS_3_20_07(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_07();  
	 
	 
}
void SYS_3_20_08(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_08();  
	 
	 
}
void SYS_3_20_09(void)
{
	SYS_Base_KeyFunction();
	 if(RefreshFlag) PAGE_3_20_09(); 
	 
	 
}
void SYS_3_20_10(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_10();  
	 
	 
}
void SYS_3_20_11(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_11();  
	 
	 
}
void SYS_3_20_12(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_12();  
	 
	 
}
void SYS_3_20_13(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_13();  
	 
	 
}
void SYS_3_20_14(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_14();  
	 
	 
}
void SYS_3_20_15(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_15();  
	 
	 
}
void SYS_3_20_16(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_16();  
	 
	 
}
void SYS_3_20_17(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_17();  
	 
	 
}
void SYS_3_20_18(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_18();  
	 
	 
}
void SYS_3_20_19(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_19();  
	 
	 
}
void SYS_3_20_20(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_20();  
	 
	 
}
void SYS_3_20_21(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_21();  
	 
	 
}
void SYS_3_20_22(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_22();  
	 
	 
}
void SYS_3_20_23(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_23();  
	 
	 
}
void SYS_3_20_24(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_24();  
	 
	 
}
void SYS_3_20_25(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_25();  
	 
	 
}
void SYS_3_20_26(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_26();  
	 
	 
}
void SYS_3_20_27(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_27();  
	 
	 
}
void SYS_3_20_28(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_28();  
	 
	 
}
void SYS_3_20_29(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_29();  
	 
	 
}
void SYS_3_20_30(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_30();  
	 
	 
}
void SYS_3_20_31(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_31();  
	 
	 
}
void SYS_3_20_32(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_32();  
	 
	 
}
void SYS_3_20_33(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_33();  
	 
	 
}
void SYS_3_20_34(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_34();  
	 
	 
}
void SYS_3_20_35(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_35();  
	 
	 
}
void SYS_3_20_36(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_36();  
	 
	 
}
void SYS_3_20_37(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_37();  
	 
	 
}
void SYS_3_20_38(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_38();  
	 
	 
}
void SYS_3_20_39(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_39();  
	 
	 
}
void SYS_3_20_40(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_20_40();  
	 
	 
}
void SYS_3_20_41(void)
{
	SYS_Base_KeyFunction();
	 
	if(KeyState.KeyValue == DN)naviMENU = 32000;
	if(RefreshFlag) PAGE_3_20_41();  
}




void SYS_3_21_00(void)
{  
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU = 32115;
	if(RefreshFlag) PAGE_3_21_00();  
	             
}              
void SYS_3_21_01(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_21_01();  
	 
	 
}
void SYS_3_21_02(void)
{
	SYS_Base_KeyFunction();
	 if(RefreshFlag) PAGE_3_21_02(); 
	 
	 
}
void SYS_3_21_03(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_21_03();  
	 
	 
}
void SYS_3_21_04(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_21_04();  
	 
	 
}
void SYS_3_21_05(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_21_05();  
	 
	 
}
void SYS_3_21_06(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_21_06();  
	 
	 
}
void SYS_3_21_07(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_21_07();  
	 
	 
}
void SYS_3_21_08(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_21_08();  
	 
	 
}
void SYS_3_21_09(void)
{
	if(!Edit_flag)
	{
		
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,7);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,6);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
		}

	if(RefreshFlag) PAGE_3_21_09();  
	 
	 
}
void SYS_3_21_10(void)
{
	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
		
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,7);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,6);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
		}

	if(RefreshFlag) PAGE_3_21_10();  
	 
	 
}
void SYS_3_21_11(void)
{
	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
		
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,7);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,6);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
		}
	if(RefreshFlag) PAGE_3_21_11();  
	 
	 
}
void SYS_3_21_12(void)
{
	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
		
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,7);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,6);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
		}

	if(RefreshFlag) PAGE_3_21_12();  
	 
	 
}
void SYS_3_21_13(void)
{
	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
		
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,7);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,6);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
		}

	if(RefreshFlag) PAGE_3_21_13();  
	 
	 
}
void SYS_3_21_14(void)
{
	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
		
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,7);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,6);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
		}
	if(RefreshFlag) PAGE_3_21_14();  
	 
	 
}
void SYS_3_21_15(void)
{
	if(!Edit_flag)
	{
		if(edit_Temp != Temporary)
		{
			edit_Temp = Temporary;
			RefreshFlag = 1;
		}
		
 		SYS_Base_KeyFunction();
		if(KeyState.KeyValue == DN)naviMENU = 32100;
		if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			EnterFlag =0;
			posInpage = 0;
		}
	}
	else
	{
			if(posInpage ==0) CLCD_cursor_ON(0xC0,10);
			else if(posInpage ==1) CLCD_cursor_ON(0xC0,8);
			else if(posInpage ==2) CLCD_cursor_ON(0xC0,7);
			else if(posInpage ==3) CLCD_cursor_ON(0xC0,6);

			if(KeyState.KeyValue == ENTER)
			{
				EnterFlag = 1;
				//Send_Parameter(1,0,edit_Temp); 
				CLCD_cursor_OFF();
				//Edit_flag = 0;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				CLCD_cursor_OFF();
			}
			else if(KeyState.KeyValue == UP)
			{
				if(posInpage == 0)
				{
					if(1499 >= edit_Temp)edit_Temp = edit_Temp + 1;
				}
				else if(posInpage == 1)
				{
					if(1490 >= edit_Temp)edit_Temp = edit_Temp + 10;
				}
				else if(posInpage == 2)
				{
					if(1400 >= edit_Temp)edit_Temp = edit_Temp + 100;
				}
				else if(posInpage == 3)
				{
					if(500 >= edit_Temp)edit_Temp = edit_Temp + 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(posInpage == 0)
				{
					if(edit_Temp > 0) edit_Temp = edit_Temp - 1;
				}
				else if(posInpage == 1)
				{
					if(edit_Temp > 9)edit_Temp = edit_Temp - 10;
				}
				else if(posInpage == 2)
				{
					if(edit_Temp > 99)edit_Temp = edit_Temp - 100;
				}
				else if(posInpage == 3)
				{
					if(edit_Temp > 999)edit_Temp = edit_Temp - 1000;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 0)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(3 > posInpage)
				{
					posInpage++;
				}
			}
		}
	

	if(RefreshFlag) PAGE_3_21_15();  
}





void SYS_3_22_00(void)
{    
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU = 32208;
	if(RefreshFlag) PAGE_3_22_00();  
	           
}              
void SYS_3_22_01(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_22_01(); 
	 
	 
}
void SYS_3_22_02(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_22_02(); 
	 
	 
}
void SYS_3_22_03(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_22_03(); 
	 
	 
}
void SYS_3_22_04(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_22_04(); 
	 
	 
}
void SYS_3_22_05(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_22_05(); 
	 
	 
}
void SYS_3_22_06(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_22_06();
	 
	 
}
void SYS_3_22_07(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_22_07(); 
	 
	 
}
void SYS_3_22_08(void)
{
	SYS_Base_KeyFunction();
	 
	if(KeyState.KeyValue == DN)naviMENU = 32200;
	if(RefreshFlag) PAGE_3_22_08(); 
}




void SYS_3_23_00(void)
{   
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU =32311;
	if(RefreshFlag) PAGE_3_23_00(); 
	            
}              
void SYS_3_23_01(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_23_01();  
	 
	 
}
void SYS_3_23_02(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_23_02();  
	 
	 
}
void SYS_3_23_03(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_23_03();  
	 
	 
}
void SYS_3_23_04(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_23_04();  
	 
	 
}
void SYS_3_23_05(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_23_05();  
	 
	 
}
void SYS_3_23_06(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_23_06();  
	 
	 
}
void SYS_3_23_07(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_23_07();  
	 
	 
}
void SYS_3_23_08(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_23_08();  
	 
	 
}
void SYS_3_23_09(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_23_09();  
	 
	 
}
void SYS_3_23_10(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_3_23_10();  
	 
	 
}
void SYS_3_23_11(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == DN)naviMENU = 32300;
	if(RefreshFlag) PAGE_3_23_11();  
}


void SYS_4_00(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU /100;
	else if(KeyState.KeyValue == UP)naviMENU =402;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU +1;
	else if(KeyState.KeyValue == ENTER)
	{
		naviMENU = 4003;
		EventFlagB=1;
	}
	if(RefreshFlag) PAGE_4_00(); 
}
void SYS_4_01(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU /100;
	else if(KeyState.KeyValue == UP)naviMENU =400;
	else if(KeyState.KeyValue == DN)naviMENU = naviMENU +1;
	else if(KeyState.KeyValue == ENTER)
	{
		naviMENU = 4013;
		EventFlagB=1;
	}
	if(RefreshFlag) PAGE_4_01(); 
}
void SYS_4_02(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU /100;
	else if(KeyState.KeyValue == UP)naviMENU =401;
	else if(KeyState.KeyValue == DN)naviMENU = 400;
	else if(KeyState.KeyValue == ENTER)
	{
		naviMENU = 4023;
		EventFlagB=1;
	}
	if(RefreshFlag) PAGE_4_02(); 
}



void SYS_4_00_3(void)
{
	static unsigned char flag=0;
	if((!EventFlagB)&&(flag))// event down edge
	{
		naviMENU = 40034;
	}
	flag = EventFlagB;
	if(RefreshFlag) PAGE_4_00_3(); 
}
void SYS_4_01_3(void)
{
	static unsigned char flag=0;
	if((!EventFlagB)&&(flag))// event down edge
	{
		naviMENU = 40134;
	}
	flag = EventFlagB;
	if(RefreshFlag) PAGE_4_01_3(); 
}
void SYS_4_02_3(void)
{
	static unsigned char flag=0;
	if((!EventFlagB)&&(flag))// event down edge
	{
		naviMENU = 40234;
	}
	flag = EventFlagB;
	if(RefreshFlag) PAGE_4_02_3(); 
}

void SYS_4_00_3_4(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 400;
	if(RefreshFlag) PAGE_4_00_3_4(); 
}
void SYS_4_01_3_4(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 401;
	if(RefreshFlag) PAGE_4_01_3_4(); 
}
void SYS_4_02_3_4(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = 402;
	if(RefreshFlag) PAGE_4_02_3_4(); 
}


void SYS_5_00(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU = 508;
	if(RefreshFlag) PAGE_5_00(); 
}
void SYS_5_01(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_01();
}
void SYS_5_02(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_02();
}
void SYS_5_03(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_03();
}
void SYS_5_04(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_04();
}
void SYS_5_05(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_05();
}
void SYS_5_06(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_06();
}
void SYS_5_07(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_07();
}
void SYS_5_08(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == DN)naviMENU = 500;
	if(RefreshFlag) PAGE_5_08();
}


void SYS_5_00_00(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU = 50010;
	if(RefreshFlag) PAGE_5_00_00();
}
void SYS_5_00_01(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_00_01();
}
void SYS_5_00_02(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_00_02();
}
void SYS_5_00_03(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_00_03();
}
void SYS_5_00_04(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_00_04();
}
void SYS_5_00_05(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_00_05();
}
void SYS_5_00_06(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_00_06();
}
void SYS_5_00_07(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_00_07();
}
void SYS_5_00_08(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_00_08();
}
void SYS_5_00_09(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_00_09();
}
void SYS_5_00_10(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == DN)naviMENU = 50000;
	if(RefreshFlag) PAGE_5_00_10();
}

void SYS_5_01_00(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU = 50110;
	if(RefreshFlag) PAGE_5_01_00();
}
void SYS_5_01_01(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_01_01();
}
void SYS_5_01_02(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_01_02();
}
void SYS_5_01_03(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_01_03();
}
void SYS_5_01_04(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_01_04();
}
void SYS_5_01_05(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_01_05();
}
void SYS_5_01_06(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_01_06();
}
void SYS_5_01_07(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_01_07();
}
void SYS_5_01_08(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_01_08();
}
void SYS_5_01_09(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_01_09();
}
void SYS_5_01_10(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == DN)naviMENU = 50100;
	if(RefreshFlag) PAGE_5_01_10();
}

void SYS_5_02_00(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU = 50210;
	if(RefreshFlag) PAGE_5_02_00();
}
void SYS_5_02_01(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_02_01();
}
void SYS_5_02_02(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_02_02();
}
void SYS_5_02_03(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_02_03();
}
void SYS_5_02_04(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_02_04();
}
void SYS_5_02_05(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_02_05();
}
void SYS_5_02_06(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_02_06();
}
void SYS_5_02_07(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_02_07();
}
void SYS_5_02_08(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_02_08();
}
void SYS_5_02_09(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_02_09();
}
void SYS_5_02_10(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == DN)naviMENU = 50200;
	if(RefreshFlag) PAGE_5_02_10();
}

void SYS_5_03_00(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU = 50310;
	if(RefreshFlag) PAGE_5_03_00();
}
void SYS_5_03_01(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_03_01();
}
void SYS_5_03_02(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_03_02();
}
void SYS_5_03_03(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_03_03();
}
void SYS_5_03_04(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_03_04();
}
void SYS_5_03_05(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_03_05();
}
void SYS_5_03_06(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_03_06();
}
void SYS_5_03_07(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_03_07();
}
void SYS_5_03_08(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_03_08();
}
void SYS_5_03_09(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_03_09();
}
void SYS_5_03_10(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == DN)naviMENU = 50300;
	if(RefreshFlag) PAGE_5_03_10();
}

void SYS_5_04_00(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU = 50410;
	if(RefreshFlag) PAGE_5_04_00();
}
void SYS_5_04_01(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_04_01();
}
void SYS_5_04_02(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_04_02();
}
void SYS_5_04_03(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_04_03();
}
void SYS_5_04_04(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_04_04();
}
void SYS_5_04_05(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_04_05();
}
void SYS_5_04_06(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_04_06();
}
void SYS_5_04_07(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_04_07();
}
void SYS_5_04_08(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_04_08();
}
void SYS_5_04_09(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_04_09();
}
void SYS_5_04_10(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == DN)naviMENU = 50400;
	if(RefreshFlag) PAGE_5_04_10();
}

void SYS_5_05_00(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU = 50510;
	if(RefreshFlag) PAGE_5_05_00();
}
void SYS_5_05_01(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_05_01();
}
void SYS_5_05_02(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_05_02();
}
void SYS_5_05_03(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_05_03();
}
void SYS_5_05_04(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_05_04();
}
void SYS_5_05_05(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_05_05();
}
void SYS_5_05_06(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_05_06();
}
void SYS_5_05_07(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_05_07();
}
void SYS_5_05_08(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_05_08();
}
void SYS_5_05_09(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_05_09();
}
void SYS_5_05_10(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == DN)naviMENU = 50500;
	if(RefreshFlag) PAGE_5_05_10();
}

void SYS_5_06_00(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU = 50610;
	if(RefreshFlag) PAGE_5_06_00();
}
void SYS_5_06_01(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_06_01();
}
void SYS_5_06_02(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_06_02();
}
void SYS_5_06_03(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_06_03();
}
void SYS_5_06_04(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_06_04();
}
void SYS_5_06_05(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_06_05();
}
void SYS_5_06_06(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_06_06();
}
void SYS_5_06_07(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_06_07();
}
void SYS_5_06_08(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_06_08();
}
void SYS_5_06_09(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_06_09();
}
void SYS_5_06_10(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == DN)naviMENU = 50600;
	if(RefreshFlag) PAGE_5_06_10();
}

void SYS_5_07_00(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU = 50710;
	if(RefreshFlag) PAGE_5_07_00();
}
void SYS_5_07_01(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_07_01();
}
void SYS_5_07_02(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_07_02();
}
void SYS_5_07_03(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_07_03();
}
void SYS_5_07_04(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_07_04();
}
void SYS_5_07_05(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_07_05();
}
void SYS_5_07_06(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_07_06();
}
void SYS_5_07_07(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_07_07();
}
void SYS_5_07_08(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_07_08();
}
void SYS_5_07_09(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_07_09();
}
void SYS_5_07_10(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == DN)naviMENU = 50700;
	if(RefreshFlag) PAGE_5_07_10();
}

void SYS_5_08_00(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU = 50810;
	if(RefreshFlag) PAGE_5_08_00();
}
void SYS_5_08_01(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_08_01();
}
void SYS_5_08_02(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_08_02();
}
void SYS_5_08_03(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_08_03();
}
void SYS_5_08_04(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_08_04();
}
void SYS_5_08_05(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_08_05();
}
void SYS_5_08_06(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_08_06();
}
void SYS_5_08_07(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_08_07();
}
void SYS_5_08_08(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_08_08();
}
void SYS_5_08_09(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_5_08_09();
}
void SYS_5_08_10(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == DN)naviMENU = 50800;
	if(RefreshFlag) PAGE_5_08_10();
}


void SYS_6_00(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU = 602;
	else if(KeyState.KeyValue == ENTER)naviMENU = 60004;
	if(RefreshFlag) PAGE_6_00();
}
void SYS_6_01(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == ENTER)naviMENU = 60103;
	if(RefreshFlag) PAGE_6_01();
}
void SYS_6_02(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == DN)naviMENU = 600;
	else if(KeyState.KeyValue == ENTER)
	{
		naviMENU = 60205;
		EventFlagB=1;
	}
	if(RefreshFlag) PAGE_6_02();
}

void SYS_6_00_04(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_6_00_04();
}
void SYS_6_01_03(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == ENTER)
	{
		naviMENU = 60105;
		EventFlagB=1;
	}
	if(RefreshFlag) PAGE_6_01_03();
}
void SYS_6_01_05(void)
{
	static unsigned char flag=0;
	if((!EventFlagB)&&(flag))// event down edge
	{
		naviMENU = 1;
	}
	flag = EventFlagB;

	if(RefreshFlag) PAGE_6_01_05();
}
void SYS_6_02_05(void)
{
	static unsigned char flag=0;
	if((!EventFlagB)&&(flag))// event down edge
	{
		naviMENU = 1;
	}
	flag = EventFlagB;
	if(RefreshFlag) PAGE_6_02_05();
}


void SYS_7_00(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == UP)naviMENU = 702;
	if(RefreshFlag) PAGE_7_00();
}
void SYS_7_01(void)
{
	SYS_Base_KeyFunction();
	if(RefreshFlag) PAGE_7_01();
}
void SYS_7_02(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == DN)naviMENU = 700;
	if(RefreshFlag) PAGE_7_02();
}

void SYS_7_01_00(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == ENTER)
	{
		naviMENU = 7010005;
		SYS_password_clr();
	}
	if(RefreshFlag) PAGE_7_01_00();
}
void SYS_7_01_01(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == ENTER)
	{
		naviMENU = 7010105;
		SYS_password_clr();
	}
	if(RefreshFlag) PAGE_7_01_01();
}
void SYS_7_01_02(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == ENTER)
	{
		naviMENU = 7010205;
		SYS_password_clr();
	}
	if(RefreshFlag) PAGE_7_01_02();
}
void SYS_7_01_03(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == ENTER)
	{
		naviMENU = 7010305;
		SYS_password_clr();
	}
	if(RefreshFlag) PAGE_7_01_03();
}
void SYS_7_01_04(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == ENTER)
	{
		naviMENU = 7010405;
		SYS_password_clr();
	}
	if(RefreshFlag) PAGE_7_01_04();
}
void SYS_7_01_00_05(void)
{
	if(KeyState.KeyValue == ESC)
	{
		naviMENU = naviMENU / 100;
		SYS_password_clr();
	}
	else if(KeyState.KeyValue == RIGHT)
	{
		pass_pos++;
		if(3 <pass_pos)pass_pos=0;
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == LEFT)
	{
		if(pass_pos)pass_pos--;
		else pass_pos = 3;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == UP)
	{
		_password[pass_pos]++;
		if(9 < _password[pass_pos])_password[pass_pos]=0;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == DN)
	{
		if(_password[pass_pos])_password[pass_pos]--;
		else _password[pass_pos] = 9;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == ENTER)
	{
		if((password[0]==_password[0])&&(password[1]==_password[1])&&(password[2]==_password[2])&&(password[3]==_password[3]))
		{
			naviMENU = naviMENU+1;
		}
		else
		{
			SYS_password_clr();
		}
		
	}
	if(RefreshFlag) PAGE_7_01_00_05();
}
void SYS_7_01_00_06(void)
{
	if(KeyState.KeyValue == ESC)
	{
		naviMENU = naviMENU / 100;
		SYS_password_clr();
	}
	if(RefreshFlag) PAGE_7_01_00_06();
}
void SYS_7_01_01_05(void)
{
	if(KeyState.KeyValue == ESC)
	{
		naviMENU = naviMENU / 100;
		SYS_password_clr();
	}
	else if(KeyState.KeyValue == RIGHT)
	{
		pass_pos++;
		if(3 <pass_pos)pass_pos=0;
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == LEFT)
	{
		if(pass_pos)pass_pos--;
		else pass_pos = 3;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == UP)
	{
		_password[pass_pos]++;
		if(9 < _password[pass_pos])_password[pass_pos]=0;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == DN)
	{
		if(_password[pass_pos])_password[pass_pos]--;
		else _password[pass_pos] = 9;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == ENTER)
	{
		if((password[0]==_password[0])&&(password[1]==_password[1])&&(password[2]==_password[2])&&(password[3]==_password[3]))
		{
			naviMENU = naviMENU+1;
		}
		else
		{
			SYS_password_clr();
		}
		
	}
	if(RefreshFlag) PAGE_7_01_01_05();
}
void SYS_7_01_01_06(void)
{
	if(KeyState.KeyValue == ESC)
	{
		naviMENU = naviMENU / 100;
		SYS_password_clr();
	}
	if(RefreshFlag) PAGE_7_01_01_06();
}
void SYS_7_01_02_05(void)
{
	if(KeyState.KeyValue == ESC)
	{
		naviMENU = naviMENU / 100;
		SYS_password_clr();
	}
	else if(KeyState.KeyValue == RIGHT)
	{
		pass_pos++;
		if(3 <pass_pos)pass_pos=0;
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == LEFT)
	{
		if(pass_pos)pass_pos--;
		else pass_pos = 3;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == UP)
	{
		_password[pass_pos]++;
		if(9 < _password[pass_pos])_password[pass_pos]=0;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == DN)
	{
		if(_password[pass_pos])_password[pass_pos]--;
		else _password[pass_pos] = 9;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == ENTER)
	{
		if((password[0]==_password[0])&&(password[1]==_password[1])&&(password[2]==_password[2])&&(password[3]==_password[3]))
		{
			naviMENU = naviMENU+1;
		}
		else
		{
			SYS_password_clr();
		}
		
	}
	if(RefreshFlag) PAGE_7_01_02_05();
}
void SYS_7_01_02_06(void)
{
	if(KeyState.KeyValue == ESC)
	{
		naviMENU = naviMENU / 100;
		SYS_password_clr();
	}
	if(RefreshFlag) PAGE_7_01_02_06();
}
void SYS_7_01_03_05(void)
{
	if(KeyState.KeyValue == ESC)
	{
		naviMENU = naviMENU / 100;
		SYS_password_clr();
	}
	else if(KeyState.KeyValue == RIGHT)
	{
		pass_pos++;
		if(3 <pass_pos)pass_pos=0;
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == LEFT)
	{
		if(pass_pos)pass_pos--;
		else pass_pos = 3;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == UP)
	{
		_password[pass_pos]++;
		if(9 < _password[pass_pos])_password[pass_pos]=0;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == DN)
	{
		if(_password[pass_pos])_password[pass_pos]--;
		else _password[pass_pos] = 9;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == ENTER)
	{
		if((password[0]==_password[0])&&(password[1]==_password[1])&&(password[2]==_password[2])&&(password[3]==_password[3]))
		{
			naviMENU = naviMENU+1;
		}
		else
		{
			SYS_password_clr();
		}
		
	}
	if(RefreshFlag) PAGE_7_01_03_05();
}
void SYS_7_01_03_06(void)
{
	if(KeyState.KeyValue == ESC)
	{
		naviMENU = naviMENU / 100;
		SYS_password_clr();
	}
	if(RefreshFlag) PAGE_7_01_03_06();
}
void SYS_7_01_04_05(void)
{
	if(KeyState.KeyValue == ESC)
	{
		naviMENU = naviMENU / 100;
		SYS_password_clr();
	}
	else if(KeyState.KeyValue == RIGHT)
	{
		pass_pos++;
		if(3 <pass_pos)pass_pos=0;
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == LEFT)
	{
		if(pass_pos)pass_pos--;
		else pass_pos = 3;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == UP)
	{
		_password[pass_pos]++;
		if(9 < _password[pass_pos])_password[pass_pos]=0;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == DN)
	{
		if(_password[pass_pos])_password[pass_pos]--;
		else _password[pass_pos] = 9;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == ENTER)
	{
		if((password[0]==_password[0])&&(password[1]==_password[1])&&(password[2]==_password[2])&&(password[3]==_password[3]))
		{
			naviMENU = naviMENU+1;
		}
		else
		{
			SYS_password_clr();
		}
		
	}
	if(RefreshFlag) PAGE_7_01_04_05();
}
void SYS_7_01_04_06(void)
{
	if(KeyState.KeyValue == ESC)
	{
		naviMENU = naviMENU / 100;
		SYS_password_clr();
	}
	if(RefreshFlag) PAGE_7_01_04_06();
}



void SYS_7_02_00(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == ENTER)
	{
		naviMENU = 7020005;
		SYS_password_clr();
	}
	if(RefreshFlag) PAGE_7_02_00();
}
void SYS_7_02_01(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == ENTER)
	{
		naviMENU = 7020105;
		SYS_password_clr();
	}
	if(RefreshFlag) PAGE_7_02_01();
}
void SYS_7_02_02(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == ENTER)
	{
		naviMENU = 7020205;
		SYS_password_clr();
	}
	if(RefreshFlag) PAGE_7_02_02();
}
void SYS_7_02_03(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == ENTER)
	{
		naviMENU = 7020305;
		SYS_password_clr();
	}
	if(RefreshFlag) PAGE_7_02_03();
}
void SYS_7_02_04(void)
{
	SYS_Base_KeyFunction();
	if(KeyState.KeyValue == ENTER)
	{
		naviMENU = 7020405;
		SYS_password_clr();
	}
	if(RefreshFlag) PAGE_7_02_04();
}


void SYS_7_02_00_05(void)
{
	if(KeyState.KeyValue == ESC)
	{
		naviMENU = naviMENU / 100;
		SYS_password_clr();
	}
	else if(KeyState.KeyValue == RIGHT)
	{
		pass_pos++;
		if(3 <pass_pos)pass_pos=0;
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == LEFT)
	{
		if(pass_pos)pass_pos--;
		else pass_pos = 3;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == UP)
	{
		_password[pass_pos]++;
		if(9 < _password[pass_pos])_password[pass_pos]=0;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == DN)
	{
		if(_password[pass_pos])_password[pass_pos]--;
		else _password[pass_pos] = 9;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == ENTER)
	{
		if((password[0]==_password[0])&&(password[1]==_password[1])&&(password[2]==_password[2])&&(password[3]==_password[3]))
		{
			naviMENU = naviMENU+1;
		}
		else
		{
			SYS_password_clr();
		}
		
	}

	if(RefreshFlag) PAGE_7_02_00_05();
}
void SYS_7_02_00_06(void)
{
	if(KeyState.KeyValue == ESC)
	{
		naviMENU = naviMENU / 100;
		SYS_password_clr();
	}
	else if(KeyState.KeyValue == RIGHT)
	{
		pass_pos++;
		if(3 <pass_pos)pass_pos=0;
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == LEFT)
	{
		if(pass_pos)pass_pos--;
		else pass_pos = 3;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == UP)
	{
		_password[pass_pos]++;
		if(9 < _password[pass_pos])_password[pass_pos]=0;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == DN)
	{
		if(_password[pass_pos])_password[pass_pos]--;
		else _password[pass_pos] = 9;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == ENTER)
	{
		naviMENU = naviMENU+1;
	}
	if(RefreshFlag) PAGE_7_02_00_06();
}
void SYS_7_02_00_07(void)
{
	if(KeyState.KeyValue == ESC)
	{
		naviMENU = naviMENU / 100;
		SYS_password_clr();
	}
	if(RefreshFlag) PAGE_7_02_00_07();
}
void SYS_7_02_01_05(void)
{
	if(KeyState.KeyValue == ESC)
	{
		naviMENU = naviMENU / 100;
		SYS_password_clr();
	}
	else if(KeyState.KeyValue == RIGHT)
	{
		pass_pos++;
		if(3 <pass_pos)pass_pos=0;
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == LEFT)
	{
		if(pass_pos)pass_pos--;
		else pass_pos = 3;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == UP)
	{
		_password[pass_pos]++;
		if(9 < _password[pass_pos])_password[pass_pos]=0;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == DN)
	{
		if(_password[pass_pos])_password[pass_pos]--;
		else _password[pass_pos] = 9;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == ENTER)
	{
		if((password[0]==_password[0])&&(password[1]==_password[1])&&(password[2]==_password[2])&&(password[3]==_password[3]))
		{
			naviMENU = naviMENU+1;
		}
		else
		{
			SYS_password_clr();
		}
		
	}
	if(RefreshFlag) PAGE_7_02_01_05();
}
void SYS_7_02_01_06(void)
{
	if(KeyState.KeyValue == ESC)
	{
		naviMENU = naviMENU / 100;
		SYS_password_clr();
	}
	else if(KeyState.KeyValue == RIGHT)
	{
		pass_pos++;
		if(3 <pass_pos)pass_pos=0;
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == LEFT)
	{
		if(pass_pos)pass_pos--;
		else pass_pos = 3;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == UP)
	{
		_password[pass_pos]++;
		if(9 < _password[pass_pos])_password[pass_pos]=0;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == DN)
	{
		if(_password[pass_pos])_password[pass_pos]--;
		else _password[pass_pos] = 9;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == ENTER)
	{
		naviMENU = naviMENU+1;
	}
	if(RefreshFlag) PAGE_7_02_01_06();
}
void SYS_7_02_01_07(void)
{
	if(KeyState.KeyValue == ESC)
	{
		naviMENU = naviMENU / 100;
		SYS_password_clr();
	}
	if(RefreshFlag) PAGE_7_02_01_07();
}
void SYS_7_02_02_05(void)
{
	if(KeyState.KeyValue == ESC)
	{
		naviMENU = naviMENU / 100;
		SYS_password_clr();
	}
	else if(KeyState.KeyValue == RIGHT)
	{
		pass_pos++;
		if(3 <pass_pos)pass_pos=0;
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == LEFT)
	{
		if(pass_pos)pass_pos--;
		else pass_pos = 3;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == UP)
	{
		_password[pass_pos]++;
		if(9 < _password[pass_pos])_password[pass_pos]=0;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == DN)
	{
		if(_password[pass_pos])_password[pass_pos]--;
		else _password[pass_pos] = 9;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == ENTER)
	{
		if((password[0]==_password[0])&&(password[1]==_password[1])&&(password[2]==_password[2])&&(password[3]==_password[3]))
		{
			naviMENU = naviMENU+1;
		}
		else
		{
			SYS_password_clr();
		}
		
	}
	if(RefreshFlag) PAGE_7_02_02_05();
}
void SYS_7_02_02_06(void)
{
	if(KeyState.KeyValue == ESC)
	{
		naviMENU = naviMENU / 100;
		SYS_password_clr();
	}
	else if(KeyState.KeyValue == RIGHT)
	{
		pass_pos++;
		if(3 <pass_pos)pass_pos=0;
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == LEFT)
	{
		if(pass_pos)pass_pos--;
		else pass_pos = 3;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == UP)
	{
		_password[pass_pos]++;
		if(9 < _password[pass_pos])_password[pass_pos]=0;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == DN)
	{
		if(_password[pass_pos])_password[pass_pos]--;
		else _password[pass_pos] = 9;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == ENTER)
	{
		naviMENU = naviMENU+1;
	}
	if(RefreshFlag) PAGE_7_02_02_06();
}
void SYS_7_02_02_07(void)
{
	if(KeyState.KeyValue == ESC)
	{
		naviMENU = naviMENU / 100;
		SYS_password_clr();
	}
	if(RefreshFlag) PAGE_7_02_02_07();
}
void SYS_7_02_03_05(void)
{
	if(KeyState.KeyValue == ESC)
	{
		naviMENU = naviMENU / 100;
		SYS_password_clr();
	}
	else if(KeyState.KeyValue == RIGHT)
	{
		pass_pos++;
		if(3 <pass_pos)pass_pos=0;
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == LEFT)
	{
		if(pass_pos)pass_pos--;
		else pass_pos = 3;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == UP)
	{
		_password[pass_pos]++;
		if(9 < _password[pass_pos])_password[pass_pos]=0;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == DN)
	{
		if(_password[pass_pos])_password[pass_pos]--;
		else _password[pass_pos] = 9;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == ENTER)
	{
		if((password[0]==_password[0])&&(password[1]==_password[1])&&(password[2]==_password[2])&&(password[3]==_password[3]))
		{
			naviMENU = naviMENU+1;
		}
		else
		{
			SYS_password_clr();
		}
		
	}
	if(RefreshFlag) PAGE_7_02_03_05();
}
void SYS_7_02_03_06(void)
{
	if(KeyState.KeyValue == ESC)
	{
		naviMENU = naviMENU / 100;
		SYS_password_clr();
	}
	else if(KeyState.KeyValue == RIGHT)
	{
		pass_pos++;
		if(3 <pass_pos)pass_pos=0;
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == LEFT)
	{
		if(pass_pos)pass_pos--;
		else pass_pos = 3;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == UP)
	{
		_password[pass_pos]++;
		if(9 < _password[pass_pos])_password[pass_pos]=0;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == DN)
	{
		if(_password[pass_pos])_password[pass_pos]--;
		else _password[pass_pos] = 9;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == ENTER)
	{
		naviMENU = naviMENU+1;
	}
	if(RefreshFlag) PAGE_7_02_03_06();
}
void SYS_7_02_03_07(void)
{
	if(KeyState.KeyValue == ESC)
	{
		naviMENU = naviMENU / 100;
		SYS_password_clr();
	}
	if(RefreshFlag) PAGE_7_02_03_07();
}
void SYS_7_02_04_05(void)
{
	if(KeyState.KeyValue == ESC)
	{
		naviMENU = naviMENU / 100;
		SYS_password_clr();
	}
	else if(KeyState.KeyValue == RIGHT)
	{
		pass_pos++;
		if(3 <pass_pos)pass_pos=0;
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == LEFT)
	{
		if(pass_pos)pass_pos--;
		else pass_pos = 3;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == UP)
	{
		_password[pass_pos]++;
		if(9 < _password[pass_pos])_password[pass_pos]=0;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == DN)
	{
		if(_password[pass_pos])_password[pass_pos]--;
		else _password[pass_pos] = 9;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == ENTER)
	{
		if((password[0]==_password[0])&&(password[1]==_password[1])&&(password[2]==_password[2])&&(password[3]==_password[3]))
		{
			naviMENU = naviMENU+1;
		}
		else
		{
			SYS_password_clr();
		}
		
	}
	if(RefreshFlag) PAGE_7_02_04_05();
}
void SYS_7_02_04_06(void)
{
	if(KeyState.KeyValue == ESC)
	{
		naviMENU = naviMENU / 100;
		SYS_password_clr();
	}
	else if(KeyState.KeyValue == RIGHT)
	{
		pass_pos++;
		if(3 <pass_pos)pass_pos=0;
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == LEFT)
	{
		if(pass_pos)pass_pos--;
		else pass_pos = 3;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == UP)
	{
		_password[pass_pos]++;
		if(9 < _password[pass_pos])_password[pass_pos]=0;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == DN)
	{
		if(_password[pass_pos])_password[pass_pos]--;
		else _password[pass_pos] = 9;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == ENTER)
	{
		naviMENU = naviMENU+1;
	}
	if(RefreshFlag) PAGE_7_02_04_06();
}
void SYS_7_02_04_07(void)
{
	if(KeyState.KeyValue == ESC)
	{
		naviMENU = naviMENU / 100;
		SYS_password_clr();
	}
	if(RefreshFlag) PAGE_7_02_04_07();
}








void SYS_0xFFFFFFFF(void)
{
	if(KeyState.KeyValue ==  (ESC  & RUN & STOP & ENTER) )
	{
		naviMENU = 1;
	}
	if(RefreshFlag) PAGE_0xFFFFFFFF();
}



void SYS_password_clr(void)
{
	_password[0]=0;
	_password[1]=0;
	_password[2]=0;
	_password[3]=0;
	pass_pos = 0;
}

void SYS_Base_KeyFunction(void)
{
	if(KeyState.KeyValue == ESC)naviMENU = naviMENU /100;
	else if(KeyState.KeyValue == UP)
	{
		naviMENU = naviMENU -1;
		edit_Temp=Temporary = 0;
	}
	else if(KeyState.KeyValue == DN)
	{
		naviMENU = naviMENU +1;
		edit_Temp=Temporary = 0;
	}
	else if(KeyState.KeyValue == ENTER)naviMENU = naviMENU*100;
}


const char* _cpy_flash2memory(char const *format)
{
	char* string = text_buf;
	//memset(target, 0x00, sizeof(target)); 
	while(1)
	{
		*string =  pgm_read_byte(format);
		if(*string == 0x00)
		{
			*string = 0x00;
			break;
		}
		string++;
		format++;
	}
	string = &text_buf[0];
	return string;
}


#define TEXT_SCRATCH 12
const char* _TEXT(char const *format, ...)
{
	unsigned char scratch[TEXT_SCRATCH];
	unsigned char format_flag;
	unsigned short base;
	unsigned char *ptr;
	//double dou;
	unsigned char issigned=0;

	va_list ap;

	unsigned char markpluse=0;
	unsigned char islong=0;
	unsigned long u_val=0;
	long s_val=0;

	unsigned char fill;
	unsigned char width;

char* string = text_buf;


	va_start (ap, format);
	for (;;)
	{
		while ((format_flag = *(format++)) != '%')   // Until '%' or '\0' 
		{   
			if (!format_flag)
			{
				va_end (ap);
				*string = 0x00;
				string = &text_buf[0];
				return string;
			}
			//________________________________________________
			*string = format_flag;
			string ++;
			//________________________________________________
		}

		issigned=0; //default unsigned
		base = 10;

		format_flag = *format++; //get char after '%'

		width=0; //no formatting
		fill=0;  //no formatting
		if(format_flag=='+')
		{
			markpluse=1;
			format_flag = *format++; 
		}
		
		if(format_flag=='0' || format_flag==' ') //SPACE or ZERO padding  ?
		{
			fill=format_flag;
			format_flag = *format++; //get char after padding char
			if(format_flag>='0' && format_flag<='9')
			{
				width=format_flag-'0';
				format_flag = *format++; //get char after width char
			}
		}

		islong=0; //default int value

		if(format_flag=='l' || format_flag=='L') //Long value 
		{
			islong=1;
			format_flag = *format++; //get char after 'l' or 'L'
		}


		switch (format_flag)
		{
			case 'c':
				format_flag = va_arg(ap,int);
				// no break -> run into default
			default:
				//________________________________________________
				*string = '%';
				string ++;
				*string = format_flag;
				string ++;
				//________________________________________________

			continue;
		/*	case 'f':
				dou = va_arg(ap,double);
				gcvt(dou, 6, ptr); 
				while(*ptr) 
				{
					//________________________________________________
					*string = *ptr;
					string ++;
					ptr++;
					//________________________________________________
				}
			continue;*/

			case 's':
				ptr = (unsigned char*)va_arg(ap,char *);
				while(*ptr) 
				{
					//________________________________________________
					*string = *ptr;
					string ++;
					ptr++;
					//________________________________________________
				}
			continue;

			case 'o':
				base = 8;
				//________________________________________________
				*string = '0';
				string ++;
				//________________________________________________
			goto CONVERSION_LOOP;

			case 'd':
			issigned=1;
			// no break -> run into next case
			case 'u':


		//don't insert some case below this if USE_HEX is undefined !
		//or put       goto CONVERSION_LOOP;  before next case.

			goto CONVERSION_LOOP;
			case 'x':
				base = 16;


				CONVERSION_LOOP:

				if(issigned) //Signed types
				{
					if(islong) { s_val = va_arg(ap,long); }//
					else { s_val = va_arg(ap,int); }

					if(s_val < 0) //Value negativ ?
					{
						s_val = - s_val; //Make it positiv
						//________________________________________________
						*string = '-';
						string ++;
						//________________________________________________

					}
					else
					{
						if(markpluse)
						{
							//________________________________________________
							*string = '+';
							string ++;
							//________________________________________________
						}
					}
					u_val = (unsigned long)s_val;
				}
				else //Unsigned types
				{
					if(markpluse)
					{
						//________________________________________________
						*string = '+';
						string ++;
						//________________________________________________
					}
					if(islong) { u_val = va_arg(ap,unsigned long); }
					else { u_val = va_arg(ap,unsigned int); }
				}

				ptr = scratch + TEXT_SCRATCH;
				*--ptr = 0;

				
				do
				{
					char ch = u_val % base + '0';
					if (ch > '9')//USE_HEX
					{
					ch += 'a' - '9' - 1;

					ch-=0x20;

					}
		         
					*--ptr = ch;
					u_val /= base;

					if(width) width--; //calculate number of padding chars

				} while (u_val);

				while(width--) *--ptr = fill; //insert padding chars		      

				while(*ptr) 
				{
					//________________________________________________
					*string = *ptr;
					string ++;
					ptr++;
					//________________________________________________
				}
		}
	}
	*string = 0x00;
	string = &text_buf[0];
	return string;
}











void MainSYSTEM(void)
{
	static unsigned char _EventFlagE=0;

	RefreshFlag=0;
	if(naviMENU!=old_naveMENU)
	{
		
	}

	if(KeyState.KeyValue != 0xFFFF)
	{
		 RefreshFlag=1;
		EventFlagE = 1;
		EventTimeE = 0;
	}
	else
	{
		if(!_EventFlagE && EventFlagE)
		{
			CLCD_BackLightOnOff(1);
		}
		else if(_EventFlagE && !EventFlagE)
		{
			CLCD_BackLightOnOff(0);
		}
		_EventFlagE=EventFlagE;
	}

//	
//



#if 0

		switch(naviMENU){
			case  1			 	:				SYS_1						();     		break;
			case  2			 	:				SYS_2						();     		break;
			case  3			 	:				SYS_3						();     		break;
			case  4			 	:				SYS_4						();     		break;
			case  5			 	:				SYS_5						();     		break;
			case  6			 	:				SYS_6						();     		break;
			case  7			 	:				SYS_7						();     		break;
			
			case  10			 	:				SYS_1_0					();     		break;
			case  100		 	:				SYS_1_0_0					();     		break;
			case  11			 	:				SYS_1_1					();     		break;
			case  110		 	:				SYS_1_1_0					();     		break;
			case  12			 	:				SYS_1_2					();     		break;
			case  13			 	:				SYS_1_3					();     		break;
			case  14			 	:				SYS_1_4					();     		break;
			case  15			 	:				SYS_1_5					();     		break;
		
			case  20			 	:				SYS_2_0					();	     		break;
			case  21			 	:				SYS_2_1					();     		break;
			case  22			 	:				SYS_2_2					();     		break;
			
			case  2000		 	:				SYS_2_0_00					();	     		break;
			case  2001		 	:				SYS_2_0_01					();	     		break;
			case  2002		 	:				SYS_2_0_02					();	     		break;
			case  2003		 	:				SYS_2_0_03					();	     		break;
			case  2004		 	:				SYS_2_0_04					();	     		break;
			case  2005		 	:				SYS_2_0_05					();	     		break;
			case  2006		 	:				SYS_2_0_06					();	     		break;
			case  2007		 	:				SYS_2_0_07					();	     		break;
			case  2008		 	:				SYS_2_0_08					();	     		break;
			case  2009		 	:				SYS_2_0_09					();	     		break;
			case  2010		 	:				SYS_2_0_10					();	     		break;
			case  2011		 	:				SYS_2_0_11					();	     		break;
			case  2012		 	:				SYS_2_0_12					();	     		break;
			case  2013		 	:				SYS_2_0_13					();	     		break;

			case  2100		 	:				SYS_2_1_00					();	     		break;
			case  2101		 	:				SYS_2_1_01					();	     		break;
			case  2102		 	:				SYS_2_1_02					();	     		break;
			case  2103		 	:				SYS_2_1_03					();	     		break;
			case  2104		 	:				SYS_2_1_04					();	     		break;
			
			case  2200		 	:				SYS_2_2_00					();	     		break;
			case  2201		 	:				SYS_2_2_01					();	     		break;
			case  2202		 	:				SYS_2_2_02					();	     		break;
			case  2203		 	:				SYS_2_2_03					();	     		break;
			case  2204		 	:				SYS_2_2_04					();	     		break;
			case  2205		 	:				SYS_2_2_05					();	     		break;
			case  2206		 	:				SYS_2_2_06					();	     		break;
			case  2207		 	:				SYS_2_2_07					();	     		break;
			case  2208		 	:				SYS_2_2_08					();	     		break;

			case  300		 	:				SYS_3_00					();	     		break;
			case  301		 	:				SYS_3_01					();	     		break;
			case  302		 	:				SYS_3_02					();	     		break;
			case  303		 	:				SYS_3_03					();	     		break;
			case  304		 	:				SYS_3_04					();	     		break;
			case  305		 	:				SYS_3_05					();	     		break;
			case  306		 	:				SYS_3_06					();	     		break;
			case  307		 	:				SYS_3_07					();	     		break;
			case  308		 	:				SYS_3_08					();	     		break;
			case  309		 	:				SYS_3_09					();	     		break;
			case  310		 	:				SYS_3_10					();	     		break;
			case  311		 	:				SYS_3_11					();	     		break;
			case  312		 	:				SYS_3_12					();	     		break;
			case  313		 	:				SYS_3_13					();	     		break;
			case  314		 	:				SYS_3_14					();	     		break;
			case  315		 	:				SYS_3_15					();	     		break;
			case  316		 	:				SYS_3_16					();	     		break;
			case  317		 	:				SYS_3_17					();	     		break;
			case  318		 	:				SYS_3_18					();	     		break;
			case  319		 	:				SYS_3_19					();	     		break;
			case  320		 	:				SYS_3_20					();	     		break;
			case  321		 	:				SYS_3_21					();	     		break;
			case  322		 	:				SYS_3_22					();	     		break;
			case  323		 	:				SYS_3_23					();	     		break;
			case  324		 	:				SYS_3_24					();	     		break;

			case  30000			:				SYS_3_00_00				();					break;						
			case  30001			:				SYS_3_00_01				();					break;						
			case  30002			:				SYS_3_00_02				();					break;						
			case  30003			:				SYS_3_00_03				();					break;	 				
			case  30004			:				SYS_3_00_04				();					break;						
			case  30005			:				SYS_3_00_05				();					break;						
			case  30006			:				SYS_3_00_06				();					break;						
			case  30007			:				SYS_3_00_07				();					break;						
			case  30008			:				SYS_3_00_08				();					break;						
			case  30009			:				SYS_3_00_09				();					break;						
			case  30010			:				SYS_3_00_10				();					break;						
			case  30011			:				SYS_3_00_11				();					break;						
			case  30012			:				SYS_3_00_12				();					break;						
			case  30013			:				SYS_3_00_13				();					break;						
			case  30014			:				SYS_3_00_14				();					break;						
			case  30015			:				SYS_3_00_15				();					break;						
	                  	     		              	        		
			case  30100			:				SYS_3_01_00				();					break;						
			case  30101			:				SYS_3_01_01				();					break;						
			case  30102			:				SYS_3_01_02				();					break;						
			case  30103			:				SYS_3_01_03				();					break;						
			case  30104			:				SYS_3_01_04				();					break;						
			case  30105			:				SYS_3_01_05				();					break;						
			case  30106			:				SYS_3_01_06				();					break;						
			case  30107			:				SYS_3_01_07				();					break;						
			case  30108			:				SYS_3_01_08				();					break;						
			case  30109			:				SYS_3_01_09				();					break;			 		
	                  	     		              	        		
			case  30200			:				SYS_3_02_00				();					break;						
			case  30201			:				SYS_3_02_01				();					break;						
			case  30202			:				SYS_3_02_02				();					break;						
			case  30203			:				SYS_3_02_03				();					break;						
			case  30204			:				SYS_3_02_04				();					break;						
			case  30205			:				SYS_3_02_05				();					break;						
			case  30206			:				SYS_3_02_06				();					break;						
			case  30207			:				SYS_3_02_07				();					break;						
			case  30208			:				SYS_3_02_08				();					break;						
			case  30209			:				SYS_3_02_09				();					break;						
	                  	     		              	        		
			case  30300			:				SYS_3_03_00				();					break;						
			case  30301			:				SYS_3_03_01				();					break;						
			case  30302			:				SYS_3_03_02				();					break;					 
			case  30303			:				SYS_3_03_03				();					break;						
			case  30304			:				SYS_3_03_04				();					break;						
			case  30305			:				SYS_3_03_05				();					break;		 			
			case  30306			:				SYS_3_03_06				();					break;						
			case  30307			:				SYS_3_03_07				();					break;						
			case  30308			:				SYS_3_03_08				();					break;						
			case  30309			:				SYS_3_03_09				();					break;						
			case  30310			:				SYS_3_03_10				();					break; 					
			case  30311			:				SYS_3_03_11				();					break;					 
			case  30312			:				SYS_3_03_12				();					break;						
			case  30313			:				SYS_3_03_13				();					break;						
			case  30314			:				SYS_3_03_14				();					break;						
			case  30315			:				SYS_3_03_15				();					break;						
			case  30316			:				SYS_3_03_16				();					break;						
			case  30317			:				SYS_3_03_17				();					break;						
			case  30318			:				SYS_3_03_18				();					break;						
			case  30319			:				SYS_3_03_19				();					break;						
			case  30320			:				SYS_3_03_20				();					break;						
			case  30321			:				SYS_3_03_21				();					break;						
			case  30322			:				SYS_3_03_22				();					break;						
			case  30323			:				SYS_3_03_23				();					break;						
			case  30324			:				SYS_3_03_24				();					break;						
			case  30325			:				SYS_3_03_25				();					break;						
			case  30326			:				SYS_3_03_26				();					break;						
			case  30327			:				SYS_3_03_27				();					break;						
			case  30328			:				SYS_3_03_28				();					break;						
			case  30329			:				SYS_3_03_29				();					break;						
			case  30330			:				SYS_3_03_30				();					break;						
			case  30331			:				SYS_3_03_31				();					break;						
			case  30332			:				SYS_3_03_32				();					break;						
			case  30333			:				SYS_3_03_33				();					break;						
			case  30334			:				SYS_3_03_34				();					break;						
			case  30335			:				SYS_3_03_35				();					break;						
			case  30336			:				SYS_3_03_36				();					break;						
			case  30337			:				SYS_3_03_37				();					break;						
			case  30338			:				SYS_3_03_38				();					break;						
			case  30339			:				SYS_3_03_39				();					break;						
			case  30340			:				SYS_3_03_40				();					break;						
			case  30341			:				SYS_3_03_41				();					break;						
			case  30342			:				SYS_3_03_42				();					break;						
			case  30343			:				SYS_3_03_43				();					break;						
			case  30344			:				SYS_3_03_44				();					break;					 
			case  30345			:				SYS_3_03_45				();					break;						
			case  30346			:				SYS_3_03_46				();					break;						
			case  30347			:				SYS_3_03_47				();					break;						
			case  30348			:				SYS_3_03_48				();					break;						
			case  30349			:				SYS_3_03_49				();					break;						
			case  30350			:				SYS_3_03_50				();					break;		 			
			case  30351			:				SYS_3_03_51				();					break;						
			case  30352			:				SYS_3_03_52				();					break;						
	                  	     		              	        		
			case  30400			:				SYS_3_04_00				();					break;						
			case  30401			:				SYS_3_04_01				();					break;						
			case  30402			:				SYS_3_04_02				();					break;						
			case  30403			:				SYS_3_04_03				();					break;						
			case  30404			:				SYS_3_04_04				();					break;						
			case  30405			:				SYS_3_04_05				();					break;						
			case  30406			:				SYS_3_04_06				();					break;						
			case  30407			:				SYS_3_04_07				();					break;						
			case  30408			:				SYS_3_04_08				();					break;						
			case  30409			:				SYS_3_04_09				();					break;						
			case  30410			:				SYS_3_04_10				();					break;						
			case  30411			:				SYS_3_04_11				();					break;						
			case  30412			:				SYS_3_04_12				();					break;						
			case  30413			:				SYS_3_04_13				();					break;						
			case  30414			:				SYS_3_04_14				();					break;						
			case  30415			:				SYS_3_04_15				();					break;						
			case  30416			:				SYS_3_04_16				();					break;						
			case  30417			:				SYS_3_04_17				();					break;						
			case  30418			:				SYS_3_04_18				();					break;						
			case  30419			:				SYS_3_04_19				();					break;						
			case  30420			:				SYS_3_04_20				();					break;						
			case  30421			:				SYS_3_04_21				();					break;						
			case  30422			:				SYS_3_04_22				();					break;						
			case  30423			:				SYS_3_04_23				();					break;						
			case  30424			:				SYS_3_04_24				();					break;						
			case  30425			:				SYS_3_04_25				();					break;						
			case  30426			:				SYS_3_04_26				();					break;						
			case  30427			:				SYS_3_04_27				();					break;						
			case  30428			:				SYS_3_04_28				();					break;						
			case  30429			:				SYS_3_04_29				();					break;						
			case  30430			:				SYS_3_04_30				();					break;						
			case  30431			:				SYS_3_04_31				();					break;						
			case  30432			:				SYS_3_04_32				();					break;						
			case  30433			:				SYS_3_04_33				();					break;						
			case  30434			:				SYS_3_04_34				();					break;						
			case  30435			:				SYS_3_04_35				();					break;						
			case  30436			:				SYS_3_04_36				();					break;						
			case  30437			:				SYS_3_04_37				();					break;						
			case  30438			:				SYS_3_04_38				();					break;						
			case  30439			:				SYS_3_04_39				();					break;						
			case  30440			:				SYS_3_04_40				();					break;						
			case  30441			:				SYS_3_04_41				();					break;						
			case  30442			:				SYS_3_04_42				();					break;						
			case  30443			:				SYS_3_04_43				();					break;						
			case  30444			:				SYS_3_04_44				();					break;						
			case  30445			:				SYS_3_04_45				();					break;						
			case  30446			:				SYS_3_04_46				();					break;						
			case  30447			:				SYS_3_04_47				();					break;						
			case  30448			:				SYS_3_04_48				();					break;						
			case  30449			:				SYS_3_04_49				();					break;						
			case  30450			:				SYS_3_04_50				();					break;						
			case  30451			:				SYS_3_04_51				();					break;						
			case  30452			:				SYS_3_04_52				();					break;						
	                  	     		              	        		
			case  30500			:				SYS_3_05_00				();					break;						
			case  30501			:				SYS_3_05_01				();					break;						
			case  30502			:				SYS_3_05_02				();					break;						
			case  30503			:				SYS_3_05_03				();					break;						
			case  30504			:				SYS_3_05_04				();					break;						
			case  30505			:				SYS_3_05_05				();					break;						
			case  30506			:				SYS_3_05_06				();					break;						
			case  30507			:				SYS_3_05_07				();					break;						
			case  30508			:				SYS_3_05_08				();					break;						
			case  30509			:				SYS_3_05_09				();					break;						
			case  30510			:				SYS_3_05_10				();					break;						
			case  30511			:				SYS_3_05_11				();					break;						
			case  30512			:				SYS_3_05_12				();					break;						
			case  30513			:				SYS_3_05_13				();					break;						
			case  30514			:				SYS_3_05_14				();					break;						
			case  30515			:				SYS_3_05_15				();					break;						
			case  30516			:				SYS_3_05_16				();					break;						
			case  30517			:				SYS_3_05_17				();					break;						
			case  30518			:				SYS_3_05_18				();					break;						
			case  30519			:				SYS_3_05_19				();					break;						
			case  30520			:				SYS_3_05_20				();					break;						
			case  30521			:				SYS_3_05_21				();					break;						
			case  30522			:				SYS_3_05_22				();					break;						
			case  30523			:				SYS_3_05_23				();					break;						
			case  30524			:				SYS_3_05_24				();					break;						
			case  30525			:				SYS_3_05_25				();					break;						
			case  30526			:				SYS_3_05_26				();					break;						
			case  30527			:				SYS_3_05_27				();					break;						
			case  30528			:				SYS_3_05_28				();					break;						
			case  30529			:				SYS_3_05_29				();					break;						
			case  30530			:				SYS_3_05_30				();					break;						
			case  30531			:				SYS_3_05_31				();					break;						
			case  30532			:				SYS_3_05_32				();					break;						
			case  30533			:				SYS_3_05_33				();					break;						
			case  30534			:				SYS_3_05_34				();					break;						
			case  30535			:				SYS_3_05_35				();					break;						
			case  30536			:				SYS_3_05_36				();					break;						
			case  30537			:				SYS_3_05_37				();					break;						
			case  30538			:				SYS_3_05_38				();					break;						
			case  30539			:				SYS_3_05_39				();					break;						
			case  30540			:				SYS_3_05_40				();					break;						
			case  30541			:				SYS_3_05_41				();					break;						
			case  30542			:				SYS_3_05_42				();					break;						
			case  30543			:				SYS_3_05_43				();					break;						
			case  30544			:				SYS_3_05_44				();					break;						
	                  	     		              	        		
			case  30600			:				SYS_3_06_00				();					break;						
			case  30601			:				SYS_3_06_01				();					break;						
			case  30602			:				SYS_3_06_02				();					break;						
			case  30603			:				SYS_3_06_03				();					break;						
			case  30604			:				SYS_3_06_04				();					break;						
			case  30605			:				SYS_3_06_05				();					break;						
			case  30606			:				SYS_3_06_06				();					break;						
			case  30607			:				SYS_3_06_07				();					break;						
			case  30608			:				SYS_3_06_08				();					break;						
			case  30609			:				SYS_3_06_09				();					break;						
			case  30610			:				SYS_3_06_10				();					break;						
			case  30611			:				SYS_3_06_11				();					break;						
			case  30612			:				SYS_3_06_12				();					break;						
			case  30613			:				SYS_3_06_13				();					break;						
			case  30614			:				SYS_3_06_14				();					break;						
			case  30615			:				SYS_3_06_15				();					break;						
			case  30616			:				SYS_3_06_16				();					break;						
			case  30617			:				SYS_3_06_17				();					break;						
			case  30618			:				SYS_3_06_18				();					break;						
			case  30619			:				SYS_3_06_19				();					break;						
			case  30620			:				SYS_3_06_20				();					break;						
			case  30621			:				SYS_3_06_21				();					break;						
			case  30622			:				SYS_3_06_22				();					break;						
			case  30623			:				SYS_3_06_23				();					break;						
			case  30624			:				SYS_3_06_24				();					break;						
			case  30625			:				SYS_3_06_25				();					break;						
			case  30626			:				SYS_3_06_26				();					break;						
			case  30627			:				SYS_3_06_27				();					break;						
			case  30628			:				SYS_3_06_28				();					break;						
			case  30629			:				SYS_3_06_29				();					break;						
			case  30630			:				SYS_3_06_30				();					break;						
			case  30631			:				SYS_3_06_31				();					break;						
			case  30632			:				SYS_3_06_32				();					break;						
			case  30633			:				SYS_3_06_33				();					break;						
			case  30634			:				SYS_3_06_34				();					break;						
			case  30635			:				SYS_3_06_35				();					break;						
			case  30636			:				SYS_3_06_36				();					break;						
			case  30637			:				SYS_3_06_37				();					break;						
			case  30638			:				SYS_3_06_38				();					break;						
			case  30639			:				SYS_3_06_39				();					break;						
			case  30640			:				SYS_3_06_40				();					break;						
			case  30641			:				SYS_3_06_41				();					break;						
			case  30642			:				SYS_3_06_42				();					break;						
			case  30643			:				SYS_3_06_43				();					break;						
			case  30644			:				SYS_3_06_44				();					break;						
			case  30645			:				SYS_3_06_45				();					break;						
			case  30646			:				SYS_3_06_46				();					break;						
			case  30647			:				SYS_3_06_47				();					break;						
			case  30648			:				SYS_3_06_48				();					break;						
			case  30649			:				SYS_3_06_49				();					break;						
			case  30650			:				SYS_3_06_50				();					break;						
			case  30651			:				SYS_3_06_51				();					break;						
			case  30652			:				SYS_3_06_52				();					break;						
			case  30653			:				SYS_3_06_53				();					break;						
			case  30654			:				SYS_3_06_54				();					break;						
			case  30655			:				SYS_3_06_55				();					break;						
			case  30656			:				SYS_3_06_56				();					break;						
			case  30657			:				SYS_3_06_57				();					break;						
			case  30658			:				SYS_3_06_58				();					break;						
			case  30659			:				SYS_3_06_59				();					break;						
			case  30660			:				SYS_3_06_60				();					break;						
			case  30661			:				SYS_3_06_61				();					break;						
			case  30662			:				SYS_3_06_62				();					break;						
			case  30663			:				SYS_3_06_63				();					break;						
			case  30664			:				SYS_3_06_64				();					break;						
			case  30665			:				SYS_3_06_65				();					break;						
			case  30666			:				SYS_3_06_66				();					break;						
			case  30667			:				SYS_3_06_67				();					break;						
			case  30668			:				SYS_3_06_68				();					break;						
			case  30669			:				SYS_3_06_69				();					break;						
			case  30670			:				SYS_3_06_70				();					break;						
	                  	     		              	        		
			case  30700			:				SYS_3_07_00				();					break;						
			case  30701			:				SYS_3_07_01				();					break;						
			case  30702			:				SYS_3_07_02				();					break;						
			case  30703			:				SYS_3_07_03				();					break;						
			case  30704			:				SYS_3_07_04				();					break;						
			case  30705			:				SYS_3_07_05				();					break;						
			case  30706			:				SYS_3_07_06				();					break;						
			case  30707			:				SYS_3_07_07				();					break;						
			case  30708			:				SYS_3_07_08				();					break;						
			case  30709			:				SYS_3_07_09				();					break;						
			case  30710			:				SYS_3_07_10				();					break;						
			case  30711			:				SYS_3_07_11				();					break;						
			case  30712			:				SYS_3_07_12				();					break;						
			case  30713			:				SYS_3_07_13				();					break;						
			case  30714			:				SYS_3_07_14				();					break;						
			case  30715			:				SYS_3_07_15				();					break;						
			case  30716			:				SYS_3_07_16				();					break;						
			case  30717			:				SYS_3_07_17				();					break;						
			case  30718			:				SYS_3_07_18				();					break;						
			case  30719			:				SYS_3_07_19				();					break;						
			case  30720			:				SYS_3_07_20				();					break;						
			case  30721			:				SYS_3_07_21				();					break;						
			case  30722			:				SYS_3_07_22				();					break;						
			case  30723			:				SYS_3_07_23				();					break;						
			case  30724			:				SYS_3_07_24				();					break;						
			case  30725			:				SYS_3_07_25				();					break;						
			case  30726			:				SYS_3_07_26				();					break;						
			case  30727			:				SYS_3_07_27				();					break;						
			case  30728			:				SYS_3_07_28				();					break;						
	                  	     		              	        		
			case  30800			:				SYS_3_08_00				();					break;						
			case  30801			:				SYS_3_08_01				();					break;						
			case  30802			:				SYS_3_08_02				();					break;						
			case  30803			:				SYS_3_08_03				();					break;						
			case  30804			:				SYS_3_08_04				();					break;						
			case  30805			:				SYS_3_08_05				();					break;						
			case  30806			:				SYS_3_08_06				();					break;						
			case  30807			:				SYS_3_08_07				();					break;						
			case  30808			:				SYS_3_08_08				();					break;						
			case  30809			:				SYS_3_08_09				();					break;						
			case  30810			:				SYS_3_08_10				();					break;						
			case  30811			:				SYS_3_08_11				();					break;						
			case  30812			:				SYS_3_08_12				();					break;						
			case  30813			:				SYS_3_08_13				();					break;						
			case  30814			:				SYS_3_08_14				();					break;						
			case  30815			:				SYS_3_08_15				();					break;						
			case  30816			:				SYS_3_08_16				();					break;						
			case  30817			:				SYS_3_08_17				();					break;						
			case  30818			:				SYS_3_08_18				();					break;						
			case  30819			:				SYS_3_08_19				();					break;						
	                  	     		              	        		
			case  30900			:				SYS_3_09_00				();					break;						
			case  30901			:				SYS_3_09_01				();					break;						
			case  30902			:				SYS_3_09_02				();					break;						
			case  30903			:				SYS_3_09_03				();					break;						
			case  30904			:				SYS_3_09_04				();					break;						
			case  30905			:				SYS_3_09_05				();					break;						
			case  30906			:				SYS_3_09_06				();					break;						
			case  30907			:				SYS_3_09_07				();					break;						
			case  30908			:				SYS_3_09_08				();					break;						
			case  30909			:				SYS_3_09_09				();					break;						
			case  30910			:				SYS_3_09_10				();					break;						
			case  30911			:				SYS_3_09_11				();					break;						
			case  30912			:				SYS_3_09_12				();					break;						
			case  30913			:				SYS_3_09_13				();					break;						
			case  30914			:				SYS_3_09_14				();					break;						
			case  30915			:				SYS_3_09_15				();					break;						
			case  30916			:				SYS_3_09_16				();					break;						
	                  	     		              	        		
			case  31000			:				SYS_3_10_00				();					break;						
			case  31001			:				SYS_3_10_01				();					break;						
			case  31002			:				SYS_3_10_02				();					break;						
			case  31003			:				SYS_3_10_03				();					break;						
			case  31004			:				SYS_3_10_04				();					break;						
			case  31005			:				SYS_3_10_05				();					break;						
			case  31006			:				SYS_3_10_06				();					break;						
			case  31007			:				SYS_3_10_07				();					break;						
			case  31008			:				SYS_3_10_08				();					break;						
			case  31009			:				SYS_3_10_09				();					break;						
			case  31010			:				SYS_3_10_10				();					break;						
			case  31011			:				SYS_3_10_11				();					break;						
			case  31012			:				SYS_3_10_12				();					break;						
			case  31013			:				SYS_3_10_13				();					break;						
			case  31014			:				SYS_3_10_14				();					break;						
			case  31015			:				SYS_3_10_15				();					break;						
			case  31016			:				SYS_3_10_16				();					break;						
	                  	     		              	        		
			case  31100			:				SYS_3_11_00				();					break;						
			case  31101			:				SYS_3_11_01				();					break;						
			case  31102			:				SYS_3_11_02				();					break;						
			case  31103			:				SYS_3_11_03				();					break;						
			case  31104			:				SYS_3_11_04				();					break;						
			case  31105			:				SYS_3_11_05				();					break;						
			case  31106			:				SYS_3_11_06				();					break;						
	                 	     		              	        		
			case  31200			:				SYS_3_12_00				();					break;						
			case  31201			:				SYS_3_12_01				();					break;						
			case  31202			:				SYS_3_12_02				();					break;						
			case  31203			:				SYS_3_12_03				();					break;						
			case  31204			:				SYS_3_12_04				();					break;						
			case  31205			:				SYS_3_12_05				();					break;						
			case  31206			:				SYS_3_12_06				();					break;						
			case  31207			:				SYS_3_12_07				();					break;						
	                 	     		              	        		
			case  31300			:				SYS_3_13_00				();					break;						
			case  31301			:				SYS_3_13_01				();					break;						
			case  31302			:				SYS_3_13_02				();					break;						
			case  31303			:				SYS_3_13_03				();					break;						
			case  31304			:				SYS_3_13_04				();					break;						
			case  31305			:				SYS_3_13_05				();					break;						
			case  31306			:				SYS_3_13_06				();					break;						
			case  31307			:				SYS_3_13_07				();					break;						
			case  31308			:				SYS_3_13_08				();					break;						
			case  31309			:				SYS_3_13_09				();					break;						
			case  31310			:				SYS_3_13_10				();					break;						
			case  31311			:				SYS_3_13_11				();					break;						
	                  	     		              	        		
			case  31400			:				SYS_3_14_00				();					break;						
			case  31401			:				SYS_3_14_01				();					break;						
			case  31402			:				SYS_3_14_02				();					break;						
			case  31403			:				SYS_3_14_03				();					break;						
			case  31404			:				SYS_3_14_04				();					break;						
			case  31405			:				SYS_3_14_05				();					break;						
			case  31406			:				SYS_3_14_06				();					break;						
			case  31407			:				SYS_3_14_07				();					break;						
			case  31408			:				SYS_3_14_08				();					break;						
	                  	     		              	        		
			case  31500			:				SYS_3_15_00				();					break;						
			case  31501			:				SYS_3_15_01				();					break;						
			case  31502			:				SYS_3_15_02				();					break;						
			case  31503			:				SYS_3_15_03				();					break;						
			case  31504			:				SYS_3_15_04				();					break;						
			case  31505			:				SYS_3_15_05				();					break;						
			case  31506			:				SYS_3_15_06				();					break;						
			case  31507			:				SYS_3_15_07				();					break;						
			case  31508			:				SYS_3_15_08				();					break;						
			case  31509			:				SYS_3_15_09				();					break;						
			case  31510			:				SYS_3_15_10				();					break;						
			case  31511			:				SYS_3_15_11				();					break;						
			case  31512			:				SYS_3_15_12				();					break;						
			case  31513			:				SYS_3_15_13				();					break;						
			case  31514			:				SYS_3_15_14				();					break;						
			case  31515			:				SYS_3_15_15				();					break;						
			case  31516			:				SYS_3_15_16				();					break;						
			case  31517			:				SYS_3_15_17				();					break;						
			case  31518			:				SYS_3_15_18				();					break;						
			case  31519			:				SYS_3_15_19				();					break;						
			case  31520			:				SYS_3_15_20				();					break;						
			case  31521			:				SYS_3_15_21				();					break;						
			case  31522			:				SYS_3_15_22				();					break;						
			case  31523			:				SYS_3_15_23				();					break;						
			case  31524			:				SYS_3_15_24				();					break;						
			case  31525			:				SYS_3_15_25				();					break;						
			case  31526			:				SYS_3_15_26				();					break;						
			case  31527			:				SYS_3_15_27				();					break;						
			case  31528			:				SYS_3_15_28				();					break;						
	                  	     		              	        		
			case  31600			:				SYS_3_16_00				();					break;						
			case  31601			:				SYS_3_16_01				();					break;						
			case  31602			:				SYS_3_16_02				();					break;						
			case  31603			:				SYS_3_16_03				();					break;						
			case  31604			:				SYS_3_16_04				();					break;						
			case  31605			:				SYS_3_16_05				();					break;						
			case  31606			:				SYS_3_16_06				();					break;						
			case  31607			:				SYS_3_16_07				();					break;						
			case  31608			:				SYS_3_16_08				();					break;						
			case  31609			:				SYS_3_16_09				();					break;						
			case  31610			:				SYS_3_16_10				();					break;						
			case  31611			:				SYS_3_16_11				();					break;						
			case  31612			:				SYS_3_16_12				();					break;						
			case  31613			:				SYS_3_16_13				();					break;						
			case  31614			:				SYS_3_16_14				();					break;						
			case  31615			:				SYS_3_16_15				();					break;						
			case  31616			:				SYS_3_16_16				();					break;						
			case  31617			:				SYS_3_16_17				();					break;						
			case  31618			:				SYS_3_16_18				();					break;						
			case  31619			:				SYS_3_16_19				();					break;						
			case  31620			:				SYS_3_16_20				();					break;						
			case  31621			:				SYS_3_16_21				();					break;						
			case  31622			:				SYS_3_16_22				();					break;						
			case  31623			:				SYS_3_16_23				();					break;						
			case  31624			:				SYS_3_16_24				();					break;						
			case  31625			:				SYS_3_16_25				();					break;						
			case  31626			:				SYS_3_16_26				();					break;						
			case  31627			:				SYS_3_16_27				();					break;						
			case  31628			:				SYS_3_16_28				();					break;						
	                  	     		              	        		
			case  31700			:				SYS_3_17_00				();					break;						
			case  31701			:				SYS_3_17_01				();					break;						
			case  31702			:				SYS_3_17_02				();					break;						
			case  31703			:				SYS_3_17_03				();					break;						
			case  31704			:				SYS_3_17_04				();					break;						
			case  31705			:				SYS_3_17_05				();					break;						
			case  31706			:				SYS_3_17_06				();					break;						
			case  31707			:				SYS_3_17_07				();					break;						
			case  31708			:				SYS_3_17_08				();					break;						
			case  31709			:				SYS_3_17_09				();					break;						
			case  31710			:				SYS_3_17_10				();					break;						
			case  31711			:				SYS_3_17_11				();					break;						
			case  31712			:				SYS_3_17_12				();					break;						
			case  31713			:				SYS_3_17_13				();					break;						
			case  31714			:				SYS_3_17_14				();					break;						
			case  31715			:				SYS_3_17_15				();					break;						
			case  31716			:				SYS_3_17_16				();					break;						
			case  31717			:				SYS_3_17_17				();					break;						
			case  31718			:				SYS_3_17_18				();					break;						
			case  31719			:				SYS_3_17_19				();					break;						
			case  31720			:				SYS_3_17_20				();					break;						
			case  31721			:				SYS_3_17_21				();					break;						
			case  31722			:				SYS_3_17_22				();					break;						
			case  31723			:				SYS_3_17_23				();					break;						
			case  31724			:				SYS_3_17_24				();					break;						
			case  31725			:				SYS_3_17_25				();					break;						
			case  31726			:				SYS_3_17_26				();					break;						
			case  31727			:				SYS_3_17_27				();					break;						
			case  31728			:				SYS_3_17_28				();					break;						
			case  31729			:				SYS_3_17_29				();					break;						
			case  31730			:				SYS_3_17_30				();					break;						
			case  31731			:				SYS_3_17_31				();					break;						
			case  31732			:				SYS_3_17_32				();					break;						
			case  31733			:				SYS_3_17_33				();					break;						
			case  31734			:				SYS_3_17_34				();					break;						
			case  31735			:				SYS_3_17_35				();					break;						
			case  31736			:				SYS_3_17_36				();					break;						
			case  31737			:				SYS_3_17_37				();					break;						
			case  31738			:				SYS_3_17_38				();					break;						
			case  31739			:				SYS_3_17_39				();					break;						
			case  31740			:				SYS_3_17_40				();					break;						
			case  31741			:				SYS_3_17_41				();					break;						
			case  31742			:				SYS_3_17_42				();					break;						
			case  31743			:				SYS_3_17_43				();					break;						
			case  31744			:				SYS_3_17_44				();					break;						
			case  31745			:				SYS_3_17_45				();					break;						
			case  31746			:				SYS_3_17_46				();					break;						
			case  31747			:				SYS_3_17_47				();					break;						
			case  31748			:				SYS_3_17_48				();					break;						
			case  31749			:				SYS_3_17_49				();					break;						
			case  31750			:				SYS_3_17_50				();					break;						
			case  31751			:				SYS_3_17_51				();					break;						
	                 	     		              	        		
			case  31800			:				SYS_3_18_00				();					break;						
			case  31801			:				SYS_3_18_01				();					break;						
			case  31802			:				SYS_3_18_02				();					break;						
			case  31803			:				SYS_3_18_03				();					break;						
			case  31804			:				SYS_3_18_04				();					break;						
			case  31805			:				SYS_3_18_05				();					break;						
			case  31806			:				SYS_3_18_06				();					break;						
			case  31807			:				SYS_3_18_07				();					break;						
			case  31808			:				SYS_3_18_08				();					break;						
			case  31809			:				SYS_3_18_09				();					break;						
			case  31810			:				SYS_3_18_10				();					break;						
			case  31811			:				SYS_3_18_11				();					break;						
			case  31812			:				SYS_3_18_12				();					break;						
			case  31813			:				SYS_3_18_13				();					break;						
			case  31814			:				SYS_3_18_14				();					break;						
			case  31815			:				SYS_3_18_15				();					break;						
			case  31816			:				SYS_3_18_16				();					break;						
			case  31817			:				SYS_3_18_17				();					break;						
			case  31818			:				SYS_3_18_18				();					break;						
			case  31819			:				SYS_3_18_19				();					break;						
			case  31820			:				SYS_3_18_20				();					break;						
			case  31821			:				SYS_3_18_21				();					break;						
			case  31822			:				SYS_3_18_22				();					break;						
			case  31823			:				SYS_3_18_23				();					break;						
			case  31824			:				SYS_3_18_24				();					break;						
			case  31825			:				SYS_3_18_25				();					break;						
			case  31826			:				SYS_3_18_26				();					break;						
			case  31827			:				SYS_3_18_27				();					break;						
			case  31828			:				SYS_3_18_28				();					break;						
			case  31829			:				SYS_3_18_29				();					break;						
			case  31830			:				SYS_3_18_30				();					break;						
			case  31831			:				SYS_3_18_31				();					break;						
			case  31832			:				SYS_3_18_32				();					break;						
			case  31833			:				SYS_3_18_33				();					break;						
			case  31834			:				SYS_3_18_34				();					break;						
			case  31835			:				SYS_3_18_35				();					break;						
			case  31836			:				SYS_3_18_36				();					break;						
			case  31837			:				SYS_3_18_37				();					break;						
			case  31838			:				SYS_3_18_38				();					break;						
			case  31839			:				SYS_3_18_39				();					break;						
			case  31840			:				SYS_3_18_40				();					break;						
			case  31841			:				SYS_3_18_41				();					break;						
			case  31842			:				SYS_3_18_42				();					break;						
			case  31843			:				SYS_3_18_43				();					break;						
			case  31844			:				SYS_3_18_44				();					break;						
			case  31845			:				SYS_3_18_45				();					break;						
			case  31846			:				SYS_3_18_46				();					break;						
			case  31847			:				SYS_3_18_47				();					break;						
			case  31848			:				SYS_3_18_48				();					break;						
			case  31849			:				SYS_3_18_49				();					break;						
	                  	     		              	        		
			case  31900			:				SYS_3_19_00				();					break;						
			case  31901			:				SYS_3_19_01				();					break;						
			case  31902			:				SYS_3_19_02				();					break;						
			case  31903			:				SYS_3_19_03				();					break;						
			case  31904			:				SYS_3_19_04				();					break;						
			case  31905			:				SYS_3_19_05				();					break;						
			case  31906			:				SYS_3_19_06				();					break;						
			case  31907			:				SYS_3_19_07				();					break;						
			case  31908			:				SYS_3_19_08				();					break;						
			case  31909			:				SYS_3_19_09				();					break;						
			case  31910			:				SYS_3_19_10				();					break;						
			case  31911			:				SYS_3_19_11				();					break;						
			case  31912			:				SYS_3_19_12				();					break;						
			case  31913			:				SYS_3_19_13				();					break;						
			case  31914			:				SYS_3_19_14				();					break;						
			case  31915			:				SYS_3_19_15				();					break;						
			case  31916			:				SYS_3_19_16				();					break;						
			case  31917			:				SYS_3_19_17				();					break;						
			case  31918			:				SYS_3_19_18				();					break;						
			case  31919			:				SYS_3_19_19				();					break;						
			case  31920			:				SYS_3_19_20				();					break;						
			case  31921			:				SYS_3_19_21				();					break;						
			case  31922			:				SYS_3_19_22				();					break;						
			case  31923			:				SYS_3_19_23				();					break;						
			case  31924			:				SYS_3_19_24				();					break;						
			case  31925			:				SYS_3_19_25				();					break;						
			case  31926			:				SYS_3_19_26				();					break;						
			case  31927			:				SYS_3_19_27				();					break;						
			case  31928			:				SYS_3_19_28				();					break;						
			case  31929			:				SYS_3_19_29				();					break;						
			case  31930			:				SYS_3_19_30				();					break;						
			case  31931			:				SYS_3_19_31				();					break;						
			case  31932			:				SYS_3_19_32				();					break;						
			case  31933			:				SYS_3_19_33				();					break;						
			case  31934			:				SYS_3_19_34				();					break;						
			case  31935			:				SYS_3_19_35				();					break;						
			case  31936			:				SYS_3_19_36				();					break;						
			case  31937			:				SYS_3_19_37				();					break;						
			case  31938			:				SYS_3_19_38				();					break;						
			case  31939			:				SYS_3_19_39				();					break;						
			case  31940			:				SYS_3_19_40				();					break;						
			case  31941			:				SYS_3_19_41				();					break;						
	                  	     		              	        		
			case  32000			:				SYS_3_20_00				();					break;						
			case  32001			:				SYS_3_20_01				();					break;						
			case  32002			:				SYS_3_20_02				();					break;						
			case  32003			:				SYS_3_20_03				();					break;						
			case  32004			:				SYS_3_20_04				();					break;						
			case  32005			:				SYS_3_20_05				();					break;						
			case  32006			:				SYS_3_20_06				();					break;						
			case  32007			:				SYS_3_20_07				();					break;						
			case  32008			:				SYS_3_20_08				();					break;						
			case  32009			:				SYS_3_20_09				();					break;						
			case  32010			:				SYS_3_20_10				();					break;						
			case  32011			:				SYS_3_20_11				();					break;						
			case  32012			:				SYS_3_20_12				();					break;						
			case  32013			:				SYS_3_20_13				();					break;						
			case  32014			:				SYS_3_20_14				();					break;						
			case  32015			:				SYS_3_20_15				();					break;						
			case  32016			:				SYS_3_20_16				();					break;						
			case  32017			:				SYS_3_20_17				();					break;						
			case  32018			:				SYS_3_20_18				();					break;						
			case  32019			:				SYS_3_20_19				();					break;						
			case  32020			:				SYS_3_20_20				();					break;						
			case  32021			:				SYS_3_20_21				();					break;						
			case  32022			:				SYS_3_20_22				();					break;						
			case  32023			:				SYS_3_20_23				();					break;						
			case  32024			:				SYS_3_20_24				();					break;						
			case  32025			:				SYS_3_20_25				();					break;						
			case  32026			:				SYS_3_20_26				();					break;						
			case  32027			:				SYS_3_20_27				();					break;						
			case  32028			:				SYS_3_20_28				();					break;						
			case  32029			:				SYS_3_20_29				();					break;						
			case  32030			:				SYS_3_20_30				();					break;						
			case  32031			:				SYS_3_20_31				();					break;						
			case  32032			:				SYS_3_20_32				();					break;						
			case  32033			:				SYS_3_20_33				();					break;						
			case  32034			:				SYS_3_20_34				();					break;						
			case  32035			:				SYS_3_20_35				();					break;						
			case  32036			:				SYS_3_20_36				();					break;						
			case  32037			:				SYS_3_20_37				();					break;						
			case  32038			:				SYS_3_20_38				();					break;						
			case  32039			:				SYS_3_20_39				();					break;						
			case  32040			:				SYS_3_20_40				();					break;						
			case  32041			:				SYS_3_20_41				();					break;						
	                  	     		              	        		
			case  32100			:				SYS_3_21_00				();					break;						
			case  32101			:				SYS_3_21_01				();					break;						
			case  32102			:				SYS_3_21_02				();					break;						
			case  32103			:				SYS_3_21_03				();					break;						
			case  32104			:				SYS_3_21_04				();					break;						
			case  32105			:				SYS_3_21_05				();					break;						
			case  32106			:				SYS_3_21_06				();					break;						
			case  32107			:				SYS_3_21_07				();					break;						
			case  32108			:				SYS_3_21_08				();					break;						
			case  32109			:				SYS_3_21_09				();					break;						
			case  32110			:				SYS_3_21_10				();					break;						
			case  32111			:				SYS_3_21_11				();					break;						
			case  32112			:				SYS_3_21_12				();					break;						
			case  32113			:				SYS_3_21_13				();					break;						
			case  32114			:				SYS_3_21_14				();					break;						
			case  32115			:				SYS_3_21_15				();					break;						
	                  	     		              	        		
			case  32200			:				SYS_3_22_00				();					break;						
			case  32201			:				SYS_3_22_01				();					break;						
			case  32202			:				SYS_3_22_02				();					break;						
			case  32203			:				SYS_3_22_03				();					break;						
			case  32204			:				SYS_3_22_04				();					break;						
			case  32205			:				SYS_3_22_05				();					break;						
			case  32206			:				SYS_3_22_06				();					break;						
			case  32207			:				SYS_3_22_07				();					break;						
			case  32208			:				SYS_3_22_08				();					break;						
	                  	     		              	        		
			case  32300			:				SYS_3_23_00				();					break;						
			case  32301			:				SYS_3_23_01				();					break;						
			case  32302			:				SYS_3_23_02				();					break;						
			case  32303			:				SYS_3_23_03				();					break;						
			case  32304			:				SYS_3_23_04				();					break;						
			case  32305			:				SYS_3_23_05				();					break;						
			case  32306			:				SYS_3_23_06				();					break;						
			case  32307			:				SYS_3_23_07				();					break;						
			case  32308			:				SYS_3_23_08				();					break;						
			case  32309			:				SYS_3_23_09				();					break;						
			case  32310			:				SYS_3_23_10				();					break;						
			case  32311			:				SYS_3_23_11				();					break;			

			case  400			:				SYS_4_00					();					break;						
			case  401			:				SYS_4_01					();					break;		
			case  402			:				SYS_4_02					();					break;		

			case  4003			:				SYS_4_00_3					();					break;						
			case  4013			:				SYS_4_01_3					();					break;		
			case  4023			:				SYS_4_02_3					();					break;	

			case  40034			:				SYS_4_00_3_4				();					break;						
			case  40134			:				SYS_4_01_3_4				();					break;		
			case  40234			:				SYS_4_02_3_4				();					break;

			case  500			:				SYS_5_00					();					break;						
			case  501			:				SYS_5_01					();					break;		
			case  502			:				SYS_5_02					();					break;	
			case  503			:				SYS_5_03					();					break;						
			case  504			:				SYS_5_04					();					break;		
			case  505			:				SYS_5_05					();					break;	
			case  506			:				SYS_5_06					();					break;						
			case  507			:				SYS_5_07					();					break;	
			case  508			:				SYS_5_08					();					break;	

			case  50000			:				SYS_5_00_00				();					break;	
			case  50001			:				SYS_5_00_01				();					break;	
			case  50002			:				SYS_5_00_02				();					break;	
			case  50003			:				SYS_5_00_03				();					break;	
			case  50004			:				SYS_5_00_04				();					break;	
			case  50005			:				SYS_5_00_05				();					break;	
			case  50006			:				SYS_5_00_06				();					break;	
			case  50007			:				SYS_5_00_07				();					break;	
			case  50008			:				SYS_5_00_08				();					break;	
			case  50009			:				SYS_5_00_09				();					break;
			case  50010			:				SYS_5_00_10				();					break;
	                        
			case  50100			:				SYS_5_01_00				();					break;	
			case  50101			:				SYS_5_01_01				();					break;	
			case  50102			:				SYS_5_01_02				();					break;	
			case  50103			:				SYS_5_01_03				();					break;	
			case  50104			:				SYS_5_01_04				();					break;	
			case  50105			:				SYS_5_01_05				();					break;	
			case  50106			:				SYS_5_01_06				();					break;	
			case  50107			:				SYS_5_01_07				();					break;	
			case  50108			:				SYS_5_01_08				();					break;	
			case  50109			:				SYS_5_01_09				();					break;
			case  50110			:				SYS_5_01_10				();					break;
	                            
			case  50200			:				SYS_5_02_00				();					break;	
			case  50201			:				SYS_5_02_01				();					break;	
			case  50202			:				SYS_5_02_02				();					break;	
			case  50203			:				SYS_5_02_03				();					break;	
			case  50204			:				SYS_5_02_04				();					break;	
			case  50205			:				SYS_5_02_05				();					break;	
			case  50206			:				SYS_5_02_06				();					break;	
			case  50207			:				SYS_5_02_07				();					break;	
			case  50208			:				SYS_5_02_08				();					break;	
			case  50209			:				SYS_5_02_09				();					break;
			case  50210			:				SYS_5_02_10				();					break;
	                            
			case  50300			:				SYS_5_03_00				();					break;	
			case  50301			:				SYS_5_03_01				();					break;	
			case  50302			:				SYS_5_03_02				();					break;	
			case  50303			:				SYS_5_03_03				();					break;	
			case  50304			:				SYS_5_03_04				();					break;	
			case  50305			:				SYS_5_03_05				();					break;	
			case  50306			:				SYS_5_03_06				();					break;	
			case  50307			:				SYS_5_03_07				();					break;	
			case  50308			:				SYS_5_03_08				();					break;	
			case  50309			:				SYS_5_03_09				();					break;
			case  50310			:				SYS_5_03_10				();					break;
	                           
			case  50400			:				SYS_5_04_00				();					break;	
			case  50401			:				SYS_5_04_01				();					break;	
			case  50402			:				SYS_5_04_02				();					break;	
			case  50403			:				SYS_5_04_03				();					break;	
			case  50404			:				SYS_5_04_04				();					break;	
			case  50405			:				SYS_5_04_05				();					break;	
			case  50406			:				SYS_5_04_06				();					break;	
			case  50407			:				SYS_5_04_07				();					break;	
			case  50408			:				SYS_5_04_08				();					break;	
			case  50409			:				SYS_5_04_09				();					break;
			case  50410			:				SYS_5_04_10				();					break;
	                            
			case  50500			:				SYS_5_05_00				();					break;	
			case  50501			:				SYS_5_05_01				();					break;	
			case  50502			:				SYS_5_05_02				();					break;	
			case  50503			:				SYS_5_05_03				();					break;	
			case  50504			:				SYS_5_05_04				();					break;	
			case  50505			:				SYS_5_05_05				();					break;	
			case  50506			:				SYS_5_05_06				();					break;	
			case  50507			:				SYS_5_05_07				();					break;	
			case  50508			:				SYS_5_05_08				();					break;	
			case  50509			:				SYS_5_05_09				();					break;
			case  50510			:				SYS_5_05_10				();					break;
	                            
			case  50600			:				SYS_5_06_00				();					break;	
			case  50601			:				SYS_5_06_01				();					break;	
			case  50602			:				SYS_5_06_02				();					break;	
			case  50603			:				SYS_5_06_03				();					break;	
			case  50604			:				SYS_5_06_04				();					break;	
			case  50605			:				SYS_5_06_05				();					break;	
			case  50606			:				SYS_5_06_06				();					break;	
			case  50607			:				SYS_5_06_07				();					break;	
			case  50608			:				SYS_5_06_08				();					break;	
			case  50609			:				SYS_5_06_09				();					break;
			case  50610			:				SYS_5_06_10				();					break;
	                            
			case  50700			:				SYS_5_07_00				();					break;	
			case  50701			:				SYS_5_07_01				();					break;	
			case  50702			:				SYS_5_07_02				();					break;	
			case  50703			:				SYS_5_07_03				();					break;	
			case  50704			:				SYS_5_07_04				();					break;	
			case  50705			:				SYS_5_07_05				();					break;	
			case  50706			:				SYS_5_07_06				();					break;	
			case  50707			:				SYS_5_07_07				();					break;	
			case  50708			:				SYS_5_07_08				();					break;	
			case  50709			:				SYS_5_07_09				();					break;
			case  50710			:				SYS_5_07_10				();					break;
	                            
			case  50800			:				SYS_5_08_00				();					break;	
			case  50801			:				SYS_5_08_01				();					break;	
			case  50802			:				SYS_5_08_02				();					break;	
			case  50803			:				SYS_5_08_03				();					break;	
			case  50804			:				SYS_5_08_04				();					break;	
			case  50805			:				SYS_5_08_05				();					break;	
			case  50806			:				SYS_5_08_06				();					break;	
			case  50807			:				SYS_5_08_07				();					break;	
			case  50808			:				SYS_5_08_08				();					break;	
			case  50809			:				SYS_5_08_09				();					break;
			case  50810			:				SYS_5_08_10				();					break;

			case  600			:				SYS_6_00					();					break;	
			case  601			:				SYS_6_01					();					break;
			case  602			:				SYS_6_02					();					break;

			case  60004			:				SYS_6_00_04				();					break;	
			case  60103			:				SYS_6_01_03				();					break;
			case  60105			:				SYS_6_01_05				();					break;
			case  60205			:				SYS_6_02_05				();					break;

			case  700			:				SYS_7_00					();					break;	
			case  701			:				SYS_7_01					();					break;
			case  702			:				SYS_7_02					();					break;
			
			case  70100			:				SYS_7_01_00				();					break;	
			case  70101			:				SYS_7_01_01				();					break;	
			case  70102			:				SYS_7_01_02				();					break;	
			case  70103			:				SYS_7_01_03				();					break;	
			case  70104			:				SYS_7_01_04				();					break;	

			case  7010005		:				SYS_7_01_00_05			();					break;	
			case  7010006		:				SYS_7_01_00_06			();					break;	
			case  7010105		:				SYS_7_01_01_05			();					break;	
			case  7010106		:				SYS_7_01_01_06			();					break;	
			case  7010205		:				SYS_7_01_02_05			();					break;	
			case  7010206		:				SYS_7_01_02_06			();					break;	
			case  7010305		:				SYS_7_01_03_05			();					break;	
			case  7010306		:				SYS_7_01_03_06			();					break;	
			case  7010405		:				SYS_7_01_04_05			();					break;	
			case  7010406		:				SYS_7_01_04_06			();					break;	

			case  70200			:				SYS_7_02_00				();					break;	
			case  70201			:				SYS_7_02_01				();					break;	
			case  70202			:				SYS_7_02_02				();					break;	
			case  70203			:				SYS_7_02_03				();					break;	
			case  70204			:				SYS_7_02_04				();					break;	

			case  7020005		:				SYS_7_02_00_05			();					break;	
			case  7020006		:				SYS_7_02_00_06			();					break;	
			case  7020007		:				SYS_7_02_00_07			();					break;
			case  7020105		:				SYS_7_02_01_05			();					break;	
			case  7020106		:				SYS_7_02_01_06			();					break;	
			case  7020107		:				SYS_7_02_01_07			();					break;
			case  7020205		:				SYS_7_02_02_05			();					break;	
			case  7020206		:				SYS_7_02_02_06			();					break;	
			case  7020207		:				SYS_7_02_02_07			();					break;
			case  7020305		:				SYS_7_02_03_05			();					break;	
			case  7020306		:				SYS_7_02_03_06			();					break;	
			case  7020307		:				SYS_7_02_03_07			();					break;
			case  7020405		:				SYS_7_02_04_05			();					break;	
			case  7020406		:				SYS_7_02_04_06			();					break;	
			case  7020407		:				SYS_7_02_04_07			();					break;

			case 0xFFFFFFFF :			SYS_0xFFFFFFFF();				break;

			default:		irregularPAGE_handler();		break;
				}
#endif
		if(RefreshFlag)old_naveMENU=naviMENU;


}
