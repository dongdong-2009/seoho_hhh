//================================================================
//
// Copyright(C) 2008 HanYoung ELEC. Co., LTD.
// Designed by Paul B.H Park(idonno2000)
//
//
//  filename : display.c
//  description : glcd display
//
//  history :  - frist designed at 2008-05-28 4:02¿ÀÈÄ
//
//================================================================

#include <inavr.h>
#include <iom2560.h>

#include "key.h"
#include "util.h"

#include "16by2clcd.h"
#include "rs232_MMItoPANEL.h"
#include "display.h"
#include "lcd_text.h"
#include "display_def.h"

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

//#include <avr/pgmspace.h>
//#include <avr/eeprom.h>

#include "debug_printf.h"


unsigned int DATA_Registers[BUF_MAX];

unsigned char device_type;

unsigned char RefreshFlag = 0;
unsigned int Temporary=0;
unsigned int edit_Temp=0;

unsigned int gRequestAddr = 0;

char password[4]={0,0,0,0};

char AccessLevel = 0;

unsigned char pass_pos=0;

char Edit_flag = 0;
//char Prohibited_flag = 0;


char text_buf[256];

unsigned int eeprom_addr=0;
unsigned char FaultDetectionHistory=0;

unsigned char MenuDisplay_Handler=1;


void(*MenuDisplay)(void);


void DisplayInit(void)
{
	MenuDisplay = SYS_0;
	Edit_flag = 0;
	posInpage = 0;
	edit_Temp = 0;
	Temporary = 0;

}

#define RUN_STOP    2306
#define Local_Remote  2300
#define Direction   2301
#define Speed_ref   2302
#define Freq_ref    2303
#define Torque_ref    2304
#define PID_ref     2305


void SYS_ParameterDisplay(unsigned char mode)
{
	if(mode == MODE_5u)//_ _ _ _ _
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 5u     ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 5u     ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_1);
		}
	}
	else if(mode==MODE_1)
	{
		if(!Edit_flag)
		{
			if(Temporary == 0)     CLCD_string(0xC0,"[0] Standard I  ");
			else if(Temporary == 1)CLCD_string(0xC0,"[1] Standard II ");
			else if(Temporary == 2)CLCD_string(0xC0,"[2] Application ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d     ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			if(edit_Temp == 0)     CLCD_string(0xC0,"[0] Standard I  ");
			else if(edit_Temp == 1)CLCD_string(0xC0,"[1] Standard II ");
			else if(edit_Temp == 2)CLCD_string(0xC0,"[2] Application ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_4);
		}
	}
	else if(mode == MODE_2)
	{
		if(!Edit_flag)
		{
			if(Temporary == 0)     CLCD_string(0xC0,"[0] 200/400/500 ");
			else if(Temporary == 1)CLCD_string(0xC0,"[1]    600    ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			if(edit_Temp == 0)     CLCD_string(0xC0,"[0] 200/400/500 ");
			else if(edit_Temp == 1)CLCD_string(0xC0,"[1]    600      ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_4);
		}
	}
	else if(mode == MODE_3)
	{
		if(!Edit_flag)
		{
			if(Temporary == 0)     CLCD_string(0xC0,"[0]    50Hz     ");
			else if(Temporary == 1)CLCD_string(0xC0,"[1]    60Hz     ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d     ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			if(edit_Temp == 0)     CLCD_string(0xC0,"[0]    50Hz     ");
			else if(edit_Temp == 1)CLCD_string(0xC0,"[1]    60Hz     ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_4);
		}
	}
	else if(mode == MODE_4)
	{
		if(!Edit_flag)
		{
			if(Temporary == 0)     CLCD_string(0xC0,"[0] Thermal Rly ");
			else if(Temporary == 1)CLCD_string(0xC0,"[1] NTC Therm   ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			if(edit_Temp == 0)     CLCD_string(0xC0,"[0]  Thermal Rly");
			else if(edit_Temp == 1)CLCD_string(0xC0,"[1]  NTC Therm  ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d     ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_4);
		}
	}
	else if(mode == MODE_3d_1u_kW)//_ _ . _ Kw
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u kW   ",(int)Temporary/10,Temporary%10));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u kW  ",(int)edit_Temp/10,edit_Temp%10));
			SYS_cursor_ON_Mode(CURSOR_MODE_6);
		}
	}
	else if(mode == MODE_3d_1u_Vrms)
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u Vrms ",(int)Temporary/10,Temporary%10));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u Vrms ",(int)edit_Temp/10,edit_Temp%10));
			SYS_cursor_ON_Mode(CURSOR_MODE_6);
		}
	}
	else if(mode == MODE_3d_1u_Arms)
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u Arms ",(int)Temporary/10,Temporary%10));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u Arms ",(int)edit_Temp/10,edit_Temp%10));
			SYS_cursor_ON_Mode(CURSOR_MODE_6);
		}

	}
	else if(mode == MODE_3d_1u_Hz)
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u Hz ",(int)Temporary/10,Temporary%10));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u Hz ",(int)edit_Temp/10,edit_Temp%10));
			SYS_cursor_ON_Mode(CURSOR_MODE_6);
		}
	}
	else if(mode == MODE_5u_pole)
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 5u pole ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 5u pole ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_1);
		}
	}
	else if(mode == MODE_5u_rpm)
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 5u rpm  ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 5u rpm  ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_1);
		}
	}
	else if(mode == MODE_11)
	{
		if(!Edit_flag)
		{
			if(Temporary == 0)     CLCD_string(0xC0,"[0] V/F Freq  ");
			else if(Temporary == 1)CLCD_string(0xC0,"[1] V/F Speed   ");
			else if(Temporary == 2)CLCD_string(0xC0,"[2] S/L Vector  ");
			else if(Temporary == 3)CLCD_string(0xC0,"[3] Vector Ctrl ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			if(edit_Temp == 0)     CLCD_string(0xC0,"[0] V/F Freq  ");
			else if(edit_Temp == 1)CLCD_string(0xC0,"[1] V/F Speed   ");
			else if(edit_Temp == 2)CLCD_string(0xC0,"[2] S/L Vector  ");
			else if(edit_Temp == 3)CLCD_string(0xC0,"[3] Vector Ctrl ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_4);
		}
	}
	else if(mode == MODE_3d_1u_kHz)
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u kHz  ",(int)Temporary/10,Temporary%10));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u kHz  ",(int)edit_Temp/10,edit_Temp%10));
			SYS_cursor_ON_Mode(CURSOR_MODE_6);
		}
	}
	else if(mode == MODE_13)
	{
		if(!Edit_flag)
		{
			if(Temporary == 0)     CLCD_string(0xC0,"[0] Terminal  ");
			else if(Temporary == 1)CLCD_string(0xC0,"[1] Operator  ");
			else if(Temporary == 2)CLCD_string(0xC0,"[2] SyncCtrlBus ");
			else if(Temporary == 3)CLCD_string(0xC0,"[3] Fieldbus  ");
			else if(Temporary == 4)CLCD_string(0xC0,"[4] Free Func   ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			if(edit_Temp == 0)     CLCD_string(0xC0,"[0] Terminal  ");
			else if(edit_Temp == 1)CLCD_string(0xC0,"[1] Operator  ");
			else if(edit_Temp == 2)CLCD_string(0xC0,"[2] SyncCtrlBus ");
			else if(edit_Temp == 3)CLCD_string(0xC0,"[3] Fieldbus  ");
			else if(edit_Temp == 4)CLCD_string(0xC0,"[4] Free Func   ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_4);
		}
	}
	else if(mode == MODE_3d_1u_s)
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u s  ",(int)Temporary/10,Temporary%10));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u s  ",(int)edit_Temp/10,edit_Temp%10));
			SYS_cursor_ON_Mode(CURSOR_MODE_6);
		}
	}
	else if(mode == MODE_15)
	{
		if(!Edit_flag)
		{
			if(Temporary == 0)     CLCD_string(0xC0,"[0] Ramp STOP   ");
			else if(Temporary == 1)CLCD_string(0xC0,"[1] Free-RUN  ");
			else if(Temporary == 2)CLCD_string(0xC0,"[2] Mixed STOP  ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			if(edit_Temp == 0)     CLCD_string(0xC0,"[0] Ramp STOP   ");
			else if(edit_Temp == 1)CLCD_string(0xC0,"[1] Free-RUN  ");
			else if(edit_Temp == 2)CLCD_string(0xC0,"[2] Mixed STOP  ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_4);
		}
	}
	else if(mode == MODE_3d_1u_persent)
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u %  ",(int)Temporary/10,Temporary%10));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u %   ",(int)edit_Temp/10,edit_Temp%10));
			SYS_cursor_ON_Mode(CURSOR_MODE_6);
		}
	}
	else if(mode == MODE_En_Dis)
	{
		if(!Edit_flag)
		{
			if(Temporary == 0)     CLCD_string(0xC0,"[0] Disabled  ");
			else if(Temporary == 1)CLCD_string(0xC0,"[1] Enabled   ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			if(edit_Temp == 0)     CLCD_string(0xC0,"[0] Disabled  ");
			else if(edit_Temp == 1)CLCD_string(0xC0,"[1] Enabled   ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_4);
		}
	}
	else if(mode == MODE_18)
	{
		if(!Edit_flag)
		{
			if(Temporary == 0)     CLCD_string(0xC0,"[0] X 1 sec   ");
			else if(Temporary == 1)CLCD_string(0xC0,"[1] X 10 SEC  ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			if(edit_Temp == 0)     CLCD_string(0xC0,"[0] X 1 sec   ");
			else if(edit_Temp == 1)CLCD_string(0xC0,"[1] X 10 SEC  ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_4);
		}
	}
	else if(mode == MODE_5u_s)
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("     % 5u s   ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("     % 5u s    ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_1);
		}

	}
	else if(mode == MODE_5u_Hz)
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 5u Hz   ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 5u Hz   ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_1);
		} 
	}
	else if(mode == MODE_5u_kHz)
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 5u kHz  ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 5u kHz  ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_1);
		}

	}
	else if(mode == MODE_22)
	{
		if(!Edit_flag)
		{
			if(Temporary == 0)     CLCD_string(0xC0,"[0] STOP      ");
			else if(Temporary == 1)CLCD_string(0xC0,"[1] E_STOP    ");
			else if(Temporary == 2)CLCD_string(0xC0,"[2] Ctrl OFF  ");
			else if(Temporary == 3)CLCD_string(0xC0,"[3] IGNORE    ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			if(edit_Temp == 0)     CLCD_string(0xC0,"[0] STOP      ");
			else if(edit_Temp == 1)CLCD_string(0xC0,"[1] E_STOP    ");
			else if(edit_Temp == 2)CLCD_string(0xC0,"[2] Ctrl OFF  ");
			else if(edit_Temp == 3)CLCD_string(0xC0,"[3] IGNORE    ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_4);
		}
	}
	else if(mode == MODE_3d_1u_V)
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u V  ",(int)Temporary/10,Temporary%10));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u V  ",(int)edit_Temp/10,edit_Temp%10));
			SYS_cursor_ON_Mode(CURSOR_MODE_6);
		}
	}
	else if(mode == MODE_24)
	{
		if(!Edit_flag)
		{
			if(Temporary == 0)     CLCD_string(0xC0,"[0] Disabled    ");
			else if(Temporary == 1)CLCD_string(0xC0,"[1] Enable RUN  ");
			else if(Temporary == 2)CLCD_string(0xC0,"[2] En RUN/STOP ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			if(edit_Temp == 0)     CLCD_string(0xC0,"[0] Disabled    ");
			else if(edit_Temp == 1)CLCD_string(0xC0,"[1] Enable RUN  ");
			else if(edit_Temp == 2)CLCD_string(0xC0,"[2] En RUN/STOP ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_4);
		}
	}
	else if(mode == MODE_25)
	{
		if(!Edit_flag)
		{
			if(Temporary == 0)     CLCD_string(0xC0,"[0] STOP      ");
			else if(Temporary == 1)CLCD_string(0xC0,"[1] E_STOP    ");
			else if(Temporary == 2)CLCD_string(0xC0,"[2] CTRL OFF  ");
			else if(Temporary == 3)CLCD_string(0xC0,"[3] IGNORE    ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			if(edit_Temp == 0)     CLCD_string(0xC0,"[0] STOP      ");
			else if(edit_Temp == 1)CLCD_string(0xC0,"[1] E_STOP    ");
			else if(edit_Temp == 2)CLCD_string(0xC0,"[2] CTRL OFF  ");
			else if(edit_Temp == 3)CLCD_string(0xC0,"[3] IGNORE    ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_4);
		}
	}
	else if(mode == MODE_3d_1u_deg)
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u deg ",(int)Temporary/10,Temporary%10));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u deg  ",(int)edit_Temp/10,edit_Temp%10));
			SYS_cursor_ON_Mode(CURSOR_MODE_6);
		}
	}
	else if(mode == MODE_P6_0)
	{
		if(!Edit_flag)
		{
			if(Temporary == 0)     CLCD_string(0xC0,"[0] Disabled    ");
			else if(Temporary == 1)CLCD_string(0xC0,"[1] AI 1 -> Ref ");
			else if(Temporary == 2)CLCD_string(0xC0,"[2] AI 2 -> Ref ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			if(edit_Temp == 0)     CLCD_string(0xC0,"[0] Disabled    ");
			else if(edit_Temp == 1)CLCD_string(0xC0,"[1] AI 1 -> Ref ");
			else if(edit_Temp == 2)CLCD_string(0xC0,"[2] AI 2 -> Ref ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_4);
		}
	}
	else if(mode == MODE_P6_1)
	{
		if(!Edit_flag)
		{
			if(Temporary == 0)     CLCD_string(0xC0,"[0] Disabled    ");
			else if(Temporary == 1)CLCD_string(0xC0,"[1] Analog In   ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			if(edit_Temp == 0)     CLCD_string(0xC0,"[0] Disabled    ");
			else if(edit_Temp == 1)CLCD_string(0xC0,"[1] Analog In   ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_4);
		}
	}
	else if(mode == MODE_P6_AI_Type)
	{
		if(!Edit_flag)
		{
			if(Temporary == 0)     CLCD_string(0xC0,"[0] 0 ¡ç¡æ 10(5)V ");
			else if(Temporary == 1)CLCD_string(0xC0,"[1] -10 ¡ç¡æ 10V  ");
			else if(Temporary == 2)CLCD_string(0xC0,"[2] 4 ¡ç¡æ 20mA   ");
			else if(Temporary == 3)CLCD_string(0xC0,"[3] 0 ¡ç¡æ 20mA   ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			if(edit_Temp == 0)     CLCD_string(0xC0,"[0] 0 ¡ç¡æ 10(5)V ");
			else if(edit_Temp == 1)CLCD_string(0xC0,"[1] -10 ¡ç¡æ 10V  ");
			else if(edit_Temp == 2)CLCD_string(0xC0,"[2] 4 ¡ç¡æ 20mA   ");
			else if(edit_Temp == 3)CLCD_string(0xC0,"[3] 0 ¡ç¡æ 20mA   ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_4);
		}
	}
	else if(mode == MODE_3d_1u_ms)
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u ms ",(int)Temporary/10,Temporary%10));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u ms ",(int)edit_Temp/10,edit_Temp%10));
			SYS_cursor_ON_Mode(CURSOR_MODE_6);
		}
	}
	else if(mode == MODE_3d_1u_mA_V)
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u mA,V ",(int)Temporary/10,Temporary%10));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u mA,V ",(int)edit_Temp/10,edit_Temp%10));
			SYS_cursor_ON_Mode(CURSOR_MODE_6);
		}
	}
	else if(mode == MODE_5u_mA_V)
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 5u mA,V ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 5u mA,V ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_1);
		}

	}
	else if(mode == MODE_3d_1u_mV)
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u mV ",(int)Temporary/10,Temporary%10));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u mV ",(int)edit_Temp/10,edit_Temp%10));
			SYS_cursor_ON_Mode(CURSOR_MODE_6);
		}
	}
	else if(mode == MODE_3d_1u_mA)
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u mA ",(int)Temporary/10,Temporary%10));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u mA ",(int)edit_Temp/10,edit_Temp%10));
			SYS_cursor_ON_Mode(CURSOR_MODE_6);
		}
	}
	else if(mode == MODE_35)
	{
		if(!Edit_flag)
		{
			if(Temporary == 0)     CLCD_string(0xC0,"[0] Disabled    ");
			else if(Temporary == 1)CLCD_string(0xC0,"[1] Process PID ");
			else if(Temporary == 2)CLCD_string(0xC0,"[2] Compensator ");
			else if(Temporary == 3)CLCD_string(0xC0,"[3] FreeFnc_PID ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			if(edit_Temp == 0)     CLCD_string(0xC0,"[0] Disabled    ");
			else if(edit_Temp == 1)CLCD_string(0xC0,"[1] Process PID ");
			else if(edit_Temp == 2)CLCD_string(0xC0,"[2] Compensator ");
			else if(edit_Temp == 3)CLCD_string(0xC0,"[3] FreeFnc_PID ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_4);
		}
	}
	else if(mode == MODE_36)
	{
		if(!Edit_flag)
		{
			if(Temporary == 0)     CLCD_string(0xC0,"[0] Operator    ");
			else if(Temporary == 1)CLCD_string(0xC0,"[1] Set_Value   ");
			else if(Temporary == 2)CLCD_string(0xC0,"[2] AI 1 -> Ref ");
			else if(Temporary == 3)CLCD_string(0xC0,"[3] AI 2 -> Ref ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			if(edit_Temp == 0)     CLCD_string(0xC0,"[0] Operator    ");
			else if(edit_Temp == 1)CLCD_string(0xC0,"[1] Set_Value   ");
			else if(edit_Temp == 2)CLCD_string(0xC0,"[2] AI 1 -> Ref ");
			else if(edit_Temp == 3)CLCD_string(0xC0,"[3] AI 2 -> Ref ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_4);
		}
	}
	else if(mode == MODE_37)
	{
		if(!Edit_flag)
		{
			if(Temporary == 0)     CLCD_string(0xC0,"[0] AI 1 -> Fb  ");
			else if(Temporary == 1)CLCD_string(0xC0,"[1] AI 1 -> Fb  ");
			else if(Temporary == 2)CLCD_string(0xC0,"[2] Free Func   ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			if(edit_Temp == 0)     CLCD_string(0xC0,"[0] AI 1 -> Fb  ");
			else if(edit_Temp == 1)CLCD_string(0xC0,"[1] AI 1 -> Fb  ");
			else if(edit_Temp == 2)CLCD_string(0xC0,"[2] Free Func   ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_4);
		}
	}
	else if(mode == MODE_5u_V)
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 5u V    ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 5u V    ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_1);
		}

	}
	else if(mode == MODE_P8_0_selection)
	{
		if(!Edit_flag)
		{
			if(Temporary == 0)     CLCD_string(0xC0,"[0] 1.FWD/2.REV ");
			else if(Temporary == 1)CLCD_string(0xC0,"[1] 1.RUN/2.DIR ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			if(edit_Temp == 0)     CLCD_string(0xC0,"[0] 1.FWD/2.REV ");
			else if(edit_Temp == 1)CLCD_string(0xC0,"[1] 1.RUN/2.DIR ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_4);
		}
	}
	else if(mode == MODE_P8_1to29_selection)
	{
		if(!Edit_flag)
		{
			if(Temporary == 0)     CLCD_string(0xC0,"[0] Disabled  ");
			else if(Temporary == 1)CLCD_string(0xC0,"[1] Conv Enable ");
			else if(Temporary == 2)CLCD_string(0xC0,"[2] MultiStep 0 ");
			else if(Temporary == 3)CLCD_string(0xC0,"[3] MultiStep 1 ");
			else if(Temporary == 4)CLCD_string(0xC0,"[4] MultiStep 2 ");
			else if(Temporary == 5)CLCD_string(0xC0,"[5] MultiStep 3 ");
			else if(Temporary == 6)CLCD_string(0xC0,"[6] Fault Reset ");
			else if(Temporary == 7)CLCD_string(0xC0,"[7] JOG     ");
			else if(Temporary == 8)CLCD_string(0xC0,"[8] AI_REF_En   ");
			else if(Temporary == 9)CLCD_string(0xC0,"[9] AI Loc Remot");
			else if(Temporary ==10)CLCD_string(0xC0,"[10] Ext Fault A");
			else if(Temporary ==11)CLCD_string(0xC0,"[11] Ext Fault B");
			else if(Temporary ==12)CLCD_string(0xC0,"[12] Motor Sel  ");
			else if(Temporary ==13)CLCD_string(0xC0,"[13] Mt Brk St  ");
			else if(Temporary ==14)CLCD_string(0xC0,"[14] Accel/Decel");
			else if(Temporary ==15)CLCD_string(0xC0,"[15] Ref UP   ");
			else if(Temporary ==16)CLCD_string(0xC0,"[16] Ref DOWN   ");
			else if(Temporary ==17)CLCD_string(0xC0,"[17] Acc/Dec Byp");
			else if(Temporary ==18)CLCD_string(0xC0,"[18] PID Bypass ");
			else if(Temporary ==19)CLCD_string(0xC0,"[19] Auto PID   ");
			else if(Temporary ==20)CLCD_string(0xC0,"[20] PID Gain   ");
			else if(Temporary ==21)CLCD_string(0xC0,"[21] Rst PID INT");
			else if(Temporary ==22)CLCD_string(0xC0,"[22] Trq Opt Byp");
			else if(Temporary ==23)CLCD_string(0xC0,"[23] Trq Sign   ");
			else if(Temporary ==24)CLCD_string(0xC0,"[24] Trq Zro Out");
			else if(Temporary ==25)CLCD_string(0xC0,"[25] Inching RUN");
			else if(Temporary ==26)CLCD_string(0xC0,"[26] Slave RUN  ");
			else if(Temporary ==27)CLCD_string(0xC0,"[27] Slv Opt Byp");
			else if(Temporary ==28)CLCD_string(0xC0,"[28] FlyingStart");
			else if(Temporary ==29)CLCD_string(0xC0,"[29] Disable P/B");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			if(edit_Temp == 0)     CLCD_string(0xC0,"[0] Disabled  ");
			else if(edit_Temp == 1)CLCD_string(0xC0,"[1] Conv Enable ");
			else if(edit_Temp == 2)CLCD_string(0xC0,"[2] MultiStep 0 ");
			else if(edit_Temp == 3)CLCD_string(0xC0,"[3] MultiStep 1 ");
			else if(edit_Temp == 4)CLCD_string(0xC0,"[4] MultiStep 2 ");
			else if(edit_Temp == 5)CLCD_string(0xC0,"[5] MultiStep 3 ");
			else if(edit_Temp == 6)CLCD_string(0xC0,"[6] Fault Reset ");
			else if(edit_Temp == 7)CLCD_string(0xC0,"[7] JOG     ");
			else if(edit_Temp == 8)CLCD_string(0xC0,"[8] AI_REF_En   ");
			else if(edit_Temp == 9)CLCD_string(0xC0,"[9] AI Loc Remot");
			else if(edit_Temp ==10)CLCD_string(0xC0,"[10] Ext Fault A");
			else if(edit_Temp ==11)CLCD_string(0xC0,"[11] Ext Fault B");
			else if(edit_Temp ==12)CLCD_string(0xC0,"[12] Motor Sel  ");
			else if(edit_Temp ==13)CLCD_string(0xC0,"[13] Mt Brk St  ");
			else if(edit_Temp ==14)CLCD_string(0xC0,"[14] Accel/Decel");
			else if(edit_Temp ==15)CLCD_string(0xC0,"[15] Ref UP   ");
			else if(edit_Temp ==16)CLCD_string(0xC0,"[16] Ref DOWN   ");
			else if(edit_Temp ==17)CLCD_string(0xC0,"[17] Acc/Dec Byp");
			else if(edit_Temp ==18)CLCD_string(0xC0,"[18] PID Bypass ");
			else if(edit_Temp ==19)CLCD_string(0xC0,"[19] Auto PID   ");
			else if(edit_Temp ==20)CLCD_string(0xC0,"[20] PID Gain   ");
			else if(edit_Temp ==21)CLCD_string(0xC0,"[21] Rst PID INT");
			else if(edit_Temp ==22)CLCD_string(0xC0,"[22] Trq Opt Byp");
			else if(edit_Temp ==23)CLCD_string(0xC0,"[23] Trq Sign   ");
			else if(edit_Temp ==24)CLCD_string(0xC0,"[24] Trq Zro Out");
			else if(edit_Temp ==25)CLCD_string(0xC0,"[25] Inching RUN");
			else if(edit_Temp ==26)CLCD_string(0xC0,"[26] Slave RUN  ");
			else if(edit_Temp ==27)CLCD_string(0xC0,"[27] Slv Opt Byp");
			else if(edit_Temp ==28)CLCD_string(0xC0,"[28] FlyingStart");
			else if(edit_Temp ==29)CLCD_string(0xC0,"[29] Disable P/B");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",edit_Temp));

			SYS_cursor_ON_Mode(CURSOR_MODE_4);
		}
	}
	else if(mode == MODE_41)
	{
		if(!Edit_flag)
		{
			if(Temporary == 0)     CLCD_string(0xC0,"[0] %       ");
			else if(Temporary == 1)CLCD_string(0xC0,"[1] Hz      ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			if(edit_Temp == 0)     CLCD_string(0xC0,"[0] %       ");
			else if(edit_Temp == 1)CLCD_string(0xC0,"[1] Hz      ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_4);
		}
	}
	else if(mode == MODE_42)
	{
		if(!Edit_flag)
		{
			if(Temporary == 0)     CLCD_string(0xC0," 0  0..20mA     ");
			else if(Temporary == 1)CLCD_string(0xC0," 1  4..20mA   ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			if(edit_Temp == 0)     CLCD_string(0xC0," 0  0..20mA     ");
			else if(edit_Temp == 1)CLCD_string(0xC0," 1  4..20mA   ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_4);
		}
	}
	else if(mode == MODE_43)
	{
		if(!Edit_flag)
		{
			if(Temporary == 0)     CLCD_string(0xC0," 0 Output Freq  ");
			else if(Temporary == 1)CLCD_string(0xC0," 1 Motor Speed  ");
			else if(Temporary == 2)CLCD_string(0xC0," 2 Input Current");
			else if(Temporary == 3)CLCD_string(0xC0," 3 Output Volt  ");
			else if(Temporary == 4)CLCD_string(0xC0," 4 Actual Torque");
			else if(Temporary == 5)CLCD_string(0xC0," 5 Output Power ");
			else if(Temporary == 6)CLCD_string(0xC0," 6 DC_Link Volt ");
			else if(Temporary == 7)CLCD_string(0xC0," 7 Free_Func Out");
			else if(Temporary == 8)CLCD_string(0xC0," 8 Trim 0 mA  ");
			else if(Temporary == 9)CLCD_string(0xC0," 9 Trim 4 mA  ");
			else if(Temporary ==10)CLCD_string(0xC0,"10 Trim 20 mA   ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			if(edit_Temp == 0)     CLCD_string(0xC0," 0 Output Freq  ");
			else if(edit_Temp == 1)CLCD_string(0xC0," 1 Motor Speed  ");
			else if(edit_Temp == 2)CLCD_string(0xC0," 2 Input Current");
			else if(edit_Temp == 3)CLCD_string(0xC0," 3 Output Volt  ");
			else if(edit_Temp == 4)CLCD_string(0xC0," 4 Actual Torque");
			else if(edit_Temp == 5)CLCD_string(0xC0," 5 Output Power ");
			else if(edit_Temp == 6)CLCD_string(0xC0," 6 DC_Link Volt ");
			else if(edit_Temp == 7)CLCD_string(0xC0," 7 Free_Func Out");
			else if(edit_Temp == 8)CLCD_string(0xC0," 8 Trim 0 mA  ");
			else if(edit_Temp == 9)CLCD_string(0xC0," 9 Trim 4 mA  ");
			else if(edit_Temp ==10)CLCD_string(0xC0,"10 Trim 20 mA   ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_4);
		}
	}
	else if(mode == MODE_1d_4u_pu)
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 1d.%04u p.u  ",(int)Temporary/10000,(int)Temporary%10000));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 1d.%04u p.u  ",(int)edit_Temp/10000,(int)edit_Temp%10000));
			SYS_cursor_ON_Mode(CURSOR_MODE_8);
		}
	}
	else if(mode == MODE_45)
	{
		if(!Edit_flag)
		{
			if(Temporary == 0)     CLCD_string(0xC0," 0 Disable    ");
			else if(Temporary == 1)CLCD_string(0xC0," 1 Conv Ready   ");
			else if(Temporary == 2)CLCD_string(0xC0," 2 Fault_Out A  ");
			else if(Temporary == 3)CLCD_string(0xC0," 3 Fault_Out B  ");
			else if(Temporary == 4)CLCD_string(0xC0," 4 Motor Brake  ");
			else if(Temporary == 5)CLCD_string(0xC0," 5 RUN Status   ");
			else if(Temporary == 6)CLCD_string(0xC0," 6 WARNING Stats");
			else if(Temporary == 7)CLCD_string(0xC0," 7 DIRECTION  ");
			else if(Temporary == 8)CLCD_string(0xC0," 8 JOG Status   ");
			else if(Temporary == 9)CLCD_string(0xC0," 9 OV_OC_UV_Fn  ");

			//else if(Temporary ==10)CLCD_string(0xC0,"10 Free_Func    ");
			//else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			if(edit_Temp == 0)     CLCD_string(0xC0," 0 Disable    ");
			else if(edit_Temp == 1)CLCD_string(0xC0," 1 Conv Ready   ");
			else if(edit_Temp == 2)CLCD_string(0xC0," 2 Fault_Out A  ");
			else if(edit_Temp == 3)CLCD_string(0xC0," 3 Fault_Out B  ");
			else if(edit_Temp == 4)CLCD_string(0xC0," 4 Motor Brake  ");
			else if(edit_Temp == 5)CLCD_string(0xC0," 5 RUN Status   ");
			else if(edit_Temp == 6)CLCD_string(0xC0," 6 WARNING Stats");
			else if(edit_Temp == 7)CLCD_string(0xC0," 7 DIRECTION  ");
			else if(edit_Temp == 8)CLCD_string(0xC0," 8 JOG Status   ");
			else if(edit_Temp == 9)CLCD_string(0xC0," 9 OV_OC_UV_Fn  ");
			//else if(edit_Temp ==10)CLCD_string(0xC0,"10 Free_Func    ");

			//else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_4);
		}
	}
	else if(mode == MODE_3d_1u)
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u    ",(int)Temporary/10,Temporary%10));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u    ",(int)edit_Temp/10,edit_Temp%10));
			SYS_cursor_ON_Mode(CURSOR_MODE_6);
		}
	}
	else if(mode == MODE_47)
	{
		if(!Edit_flag)
		{
			if(Temporary == 0)     CLCD_string(0xC0,"[0] Free      ");
			else if(Temporary == 1)CLCD_string(0xC0,"[1] Locked    ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			if(edit_Temp == 0)     CLCD_string(0xC0,"[0] Free      ");
			else if(edit_Temp == 1)CLCD_string(0xC0,"[1] Locked    ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_4);
		}
	}
	else if(mode == MODE_48)
	{
		if(!Edit_flag)
		{
			if(Temporary == 0)     CLCD_string(0xC0,"[0] Manual    ");
			else if(Temporary == 1)CLCD_string(0xC0,"[1] Auto    ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			if(edit_Temp == 0)     CLCD_string(0xC0,"[0] Manual    ");
			else if(edit_Temp == 1)CLCD_string(0xC0,"[1] Auto    ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_4);
		}
	}
	else if(mode == MODE_49)
	{
		if(!Edit_flag)
		{
			if(Temporary == 0)     CLCD_string(0xC0,"[0] Linear    ");
			else if(Temporary == 1)CLCD_string(0xC0,"[1] Square    ");
			else if(Temporary == 2)CLCD_string(0xC0,"[2] User    ");
			else if(Temporary == 3)CLCD_string(0xC0,"[3] Free Func   ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			if(edit_Temp == 0)     CLCD_string(0xC0,"[0] Linear    ");
			else if(edit_Temp == 1)CLCD_string(0xC0,"[1] Square    ");
			else if(edit_Temp == 2)CLCD_string(0xC0,"[2] User    ");
			else if(edit_Temp == 3)CLCD_string(0xC0,"[3] Free Func   ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_4);
		}
	}
	else if(mode == MODE_50)
	{
		if(!Edit_flag)
		{
			if(Temporary == 0)     CLCD_string(0xC0,"[0] Default Set ");
			else if(Temporary == 1)CLCD_string(0xC0,"[1] Auto Tuning ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			if(edit_Temp == 0)     CLCD_string(0xC0,"[0] Default Set ");
			else if(edit_Temp == 1)CLCD_string(0xC0,"[1] Auto Tuning ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_4);
		}
	}
	else if(mode == MODE_5u_ms)
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 5u ms   ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 5u ms   ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_1);
		}
	}
	else if(mode == MODE_5u_persent)
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 5u %    ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 5u %   ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_1);
		}
	}
	else if(mode == MODE_5u_ppr)
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 5u ppr  ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 5u ppr  ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_1);
		}
	}
	else if(mode == MODE_5u_mOhm)
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 5u mOhm ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 5u mOhm ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_1);
		}
	}
	else if(mode == MODE_5u_mH)
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 5u mH   ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 5u mH   ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_1);
		}
	}
	else if(mode == MODE_3d_1u_A)
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u A  ",(int)Temporary/10,Temporary%10));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u A  ",(int)edit_Temp/10,edit_Temp%10));
			SYS_cursor_ON_Mode(CURSOR_MODE_6);
		}
	}
	else if(mode == MODE_5d)//_ _ _ _ _
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 5d    ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 5d    ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_1);
		}
	}
	else if(mode == MODE_5d_rms)
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 5d rpm  ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 5d rpm  ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_1);
		}
	}
	else if(mode == MODE_5u_uF)
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 5u uF   ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 5u uF   ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_1);
		}
	}
	else if(mode == MODE_5d_A)
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 5d A    ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 5d A    ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_1);
		}

	}
	else if(mode == MODE_5d_V)
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 5d V    ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 5d V    ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_1);
		}

	}
	else if(mode == MODE_3d_1u_mOhm)
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u mOhm ",(int)Temporary/10,Temporary%10));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u mOhm ",(int)edit_Temp/10,edit_Temp%10));
			SYS_cursor_ON_Mode(CURSOR_MODE_6);
		}
	}
	else if(mode == MODE_3d_1u_mH)
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u mH  ",(int)Temporary/10,Temporary%10));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u mH  ",(int)edit_Temp/10,edit_Temp%10));
			SYS_cursor_ON_Mode(CURSOR_MODE_6);
		}
	}
	else if(mode == MODE_1d_3u_pu)
	{
		if(!Edit_flag)
		{
			CLCD_string(0xC0,(char*)_TEXT("     % 1d.%03u p.u  ",(int)Temporary/1000,(int)Temporary%1000));
			CLCD_cursor_OFF();
		}
		else
		{
			CLCD_string(0xC0,(char*)_TEXT("     % 1d.%03u p.u  ",(int)edit_Temp/1000,(int)edit_Temp%1000));
			SYS_cursor_ON_Mode(CURSOR_MODE_7);
		}
	}
	else if(mode == MODE_P6_AI_D_Step)
	{
		if(!Edit_flag)
		{
			if(Temporary == 0)     CLCD_string(0xC0,"[0] Disabled    ");
			else if(Temporary == 1)CLCD_string(0xC0,"[1] 128 Steps   ");
			else if(Temporary == 2)CLCD_string(0xC0,"[2] 64 Steps    ");
			else if(Temporary == 3)CLCD_string(0xC0,"[3] 32 Steps    ");
			else if(Temporary == 4)CLCD_string(0xC0,"[4] 16 Steps    ");
			else if(Temporary == 5)CLCD_string(0xC0,"[5] 8 Steps     ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			if(edit_Temp == 0)     CLCD_string(0xC0,"[0] Disabled    ");
			else if(edit_Temp == 1)CLCD_string(0xC0,"[1] 128 Steps   ");
			else if(edit_Temp == 2)CLCD_string(0xC0,"[2] 64 Steps    ");
			else if(edit_Temp == 3)CLCD_string(0xC0,"[3] 32 Steps    ");
			else if(edit_Temp == 4)CLCD_string(0xC0,"[4] 16 Steps    ");
			else if(edit_Temp == 5)CLCD_string(0xC0,"[5] 8 Steps     ");

			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_4);
		}
	}
	else if(mode == MODE_P6_AI_U_Sel)
	{
		if(!Edit_flag)
		{
			if(Temporary == 0)     CLCD_string(0xC0,"[0] V         ");
			else if(Temporary == 1)  CLCD_string(0xC0,"[1] mA        ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			if(edit_Temp == 0)     CLCD_string(0xC0,"[0] V         ");
			else if(edit_Temp == 1)  CLCD_string(0xC0,"[1] mA        ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_4);
		}
	}
	else if(mode == MODE_P6_AI_Func)
	{
		if(!Edit_flag)
		{
			if(Temporary == 0)     CLCD_string(0xC0,"[0] Disabled  ");
			else if(Temporary == 1)  CLCD_string(0xC0,"[1] AI        ");
			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",Temporary));
			CLCD_cursor_OFF();
		}
		else
		{
			if(edit_Temp == 0)     CLCD_string(0xC0,"[0] Disabled  ");
			else if(edit_Temp == 1)  CLCD_string(0xC0,"[1] AI        ");

			else CLCD_string(0xC0,(char*)_TEXT("irValue: %d   ",edit_Temp));
			SYS_cursor_ON_Mode(CURSOR_MODE_4);
		}
	}
}



void SYS_ParameterEdt(unsigned int addr,unsigned int e_temp,  long MAX, long Min ,unsigned char mode)
{
	char value[5];

	gRequestAddr = addr;
		
	if(mode==EDIT_MODE_1)//unsigned int
	{
			if(KeyState.KeyValue == ENTER)
			{
				WriteDataMem(addr, e_temp);
				Temporary = e_temp;
				Edit_flag = 0;
				posInpage = 0;
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				posInpage = 0;
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == UP)
			{
				value[0] =  e_temp%10;
				value[1] = (e_temp/10)%10;
				value[2] = (e_temp/100)%10;
				value[3] = (e_temp/1000)%10;
				value[4] =  e_temp/10000;
				
				value[posInpage - 1]++;
				if(9 < value[posInpage - 1]) value[posInpage - 1] = 0;
				
				e_temp =  ((unsigned int)value[4]*10000) + 
								((unsigned int)value[3]*1000) + 
								((unsigned int)value[2]*100) + 
								((unsigned int)value[1]*10) + 
								 (unsigned int)value[0];    
				
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				value[0] =  e_temp%10;
				value[1] = (e_temp/10)%10;
				value[2] = (e_temp/100)%10;
				value[3] = (e_temp/1000)%10;
				value[4] =  e_temp/10000;
				
				if(value[posInpage - 1] < 1) value[posInpage - 1] = 9;
				else value[posInpage - 1]--;

				e_temp =  ((unsigned int)value[4]*10000) + 
								((unsigned int)value[3]*1000) + 
								((unsigned int)value[2]*100) + 
								((unsigned int)value[1]*10) + 
								 (unsigned int)value[0];
				
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 1)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(4 > posInpage)
				{
					posInpage++;
				}
			}
	}
	else if(mode==EDIT_MODE_2)//selset
	{
			if(KeyState.KeyValue == ENTER)
			{
				WriteDataMem(addr, e_temp);
				Temporary = e_temp;
				Edit_flag = 0;
				posInpage = 0;
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				posInpage = 0;
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == UP)
			{
				e_temp++;
				if(5 < e_temp)e_temp = 5;
				
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(0 < e_temp)e_temp--;
				else e_temp = 0;
				
				RefreshFlag=1;
			}
	}
	else if(mode==EDIT_MODE_3)//selset
	{
			if(KeyState.KeyValue == ENTER)
			{
				WriteDataMem(addr, e_temp);
				Temporary = e_temp;
				Edit_flag = 0;
				posInpage = 0;
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				posInpage = 0;
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == UP)
			{
				e_temp++;
				if(29< e_temp)e_temp = 29;
				
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if(0 < e_temp)e_temp--;
				else e_temp = 0;
				
				RefreshFlag=1;
			}
	}
	else if(mode==EDIT_MODE_4)//int
	{
		if(posInpage==1)
		{
			if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				posInpage = 0;
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == UP)
			{
				if((int)e_temp<(32767-1))
				{
					e_temp = (int)e_temp+1;
					RefreshFlag=1;
				}
			}
			else if(KeyState.KeyValue == DN)
			{
				if((1 -32768) <= (int)e_temp)
				{
					e_temp = (int)e_temp-1;
					RefreshFlag=1;
				}
			}

			else if(KeyState.KeyValue == LEFT)
			{
				if(4 > posInpage)
				{
					posInpage++;
				}
				RefreshFlag=1;
			}

			else if(KeyState.KeyValue == ENTER)
			{
				WriteDataMem(addr, e_temp);
				Temporary = e_temp;
				Edit_flag = 0;
				posInpage = 0;
				RefreshFlag=1;
			}
		}
		else if(posInpage==2)
		{
			if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				posInpage = 0;
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == UP)
			{
				if((int)e_temp<(32767-10))
				{
					e_temp = (int)e_temp+10;
					RefreshFlag=1;
				}

				if((-10 < (int)e_temp) && ( (int)e_temp < 0))
				{
					posInpage=1;
				}
				else if( (int)e_temp==0  )posInpage=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if((10 -32768) <= (int)e_temp)
				{
					e_temp = (int)e_temp-10;
					RefreshFlag=1;
				}

				if((10 > (int)e_temp)&&( (int)e_temp >0))
				{
					posInpage=1;
				}
				else if( (int)e_temp==0  )posInpage=1;
			}

			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 1)
				{
					posInpage--;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(4 > posInpage)
				{
					posInpage++;
				}
				RefreshFlag=1;
			}     
			else if(KeyState.KeyValue == ENTER)
			{
				WriteDataMem(addr, e_temp);
				Temporary = e_temp;
				Edit_flag = 0;
				posInpage = 0;
				RefreshFlag=1;

			}

		}
		else if(posInpage==3)
		{
			if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				posInpage = 0;
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == UP)
			{
				if((int)e_temp<(32767-100))
				{
					e_temp = (int)e_temp+100;
					RefreshFlag=1;
				}
				
				if((-100 < (int)e_temp) && ( (int)e_temp <0))
				{
					posInpage=2;
				}
				else if( (int)e_temp==0  )posInpage=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if((100 -32768) <= (int)e_temp)
				{
					e_temp = (int)e_temp-100;
					RefreshFlag=1;
				}

				if((100 > (int)e_temp)&&( (int)e_temp >0))
				{
					posInpage=2;
				}
				else if( (int)e_temp==0  )posInpage=1;
			}

			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 1)
				{
					posInpage--;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(4 > posInpage)
				{
					posInpage++;
				}
				RefreshFlag=1;
			}

			else if(KeyState.KeyValue == ENTER)
			{
				WriteDataMem(addr, e_temp);
				Temporary = e_temp;
				Edit_flag = 0;
				posInpage = 0;
				RefreshFlag=1;

			}
		}
		else if(posInpage==4)
		{
			if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				posInpage = 0;
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == UP)
			{
				if((int)e_temp<(32767-1000))
				{
					e_temp = (int)e_temp+1000;
					RefreshFlag=1;
				}

				
				if((-1000 < (int)e_temp) && ( (int)e_temp <0))
				{
					posInpage=3;
				}
				else if( (int)e_temp==0  )posInpage=1;
			}
			else if(KeyState.KeyValue == DN)
			{
				if((1000 -32768) <= (int)e_temp)
				{
					e_temp = (int)e_temp-1000;
					RefreshFlag=1;
				}

				if((1000 > (int)e_temp)&&( (int)e_temp >0))
				{
					posInpage=3;
				}
				else if( (int)e_temp==0  )posInpage=1;
			}

			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 1)
				{
					posInpage--;
				}
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == LEFT)
			{
				if(4 > posInpage)
				{
					posInpage++;
				}
				RefreshFlag=1;
			}

			else if(KeyState.KeyValue == ENTER)
			{
				WriteDataMem(addr, e_temp);
				Temporary = e_temp;
				Edit_flag = 0;
				posInpage = 0;
				RefreshFlag=1;

			}
		}
		else if(posInpage==5)
		{
			if(KeyState.KeyValue == ESC)
			{
				Edit_flag = 0;
				posInpage = 0;
				RefreshFlag=1;
			}
			else if(KeyState.KeyValue == UP)
			{
				if((int)e_temp<(32767-10000))
				{
					e_temp = (int)e_temp+10000;
					RefreshFlag=1;
				}
				
				if((-10000 < (int)e_temp) && ( (int)e_temp < 0))
				{
					posInpage=4;
				}
				else if( (int)e_temp==0  )posInpage=1;
				
			}
			else if(KeyState.KeyValue == DN)
			{
				if((10000 -32768) <= (int)e_temp)
				{
					e_temp = (int)e_temp-10000;
					RefreshFlag=1;
				}

				if((10000 > (int)e_temp)&&( (int)e_temp >0))
				{
					posInpage=4;
				}
				else if( (int)e_temp==0 )posInpage=1;
			}

			else if(KeyState.KeyValue == RIGHT)
			{
				if(posInpage != 1)
				{
					posInpage--;
				}
			}
			else if(KeyState.KeyValue == ENTER)
			{
				WriteDataMem(addr, e_temp);
				Temporary = e_temp;
				Edit_flag = 0;
				posInpage = 0;
				RefreshFlag=1;

			}
		}
	}
	else if(mode==EDIT_MODE_P8_123456)//selset
	{
		if(KeyState.KeyValue == ENTER)
		{
			WriteDataMem(addr, e_temp);
			Temporary = e_temp;
			Edit_flag = 0;
			posInpage = 0;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ESC)
		{
			Edit_flag = 0;
			posInpage = 0;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == UP)
		{
			if(e_temp == 0) e_temp = 1;
			else if(e_temp == 1) e_temp = 6;
			else if(e_temp == 6) e_temp = 10;
			//else if(e_temp == 9) e_temp = 10;
			else if(e_temp == 10) e_temp = 11;
			else if(e_temp == 11) e_temp = 0;
			else e_temp = 0;
		
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == DN)
		{
			if(e_temp == 0) e_temp = 11;
			else if(e_temp == 1) e_temp = 0;
			else if(e_temp == 6) e_temp = 1;
			//else if(e_temp == 9) e_temp = 6;
			else if(e_temp == 10) e_temp = 6;
			else if(e_temp == 11) e_temp = 10;
			else e_temp = 0;

			RefreshFlag=1;
		}
	}
	else if(mode==EDIT_MODE_P12)//selset
	{
		if(KeyState.KeyValue == ENTER)
		{
			WriteDataMem(addr, e_temp);
			Temporary = e_temp;
			Edit_flag = 0;
			posInpage = 0;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ESC)
		{
			Edit_flag = 0;
			posInpage = 0;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == UP)
		{
			if(e_temp == 0) e_temp = 1;
			else if(e_temp == 1) e_temp = 2;
			else if(e_temp == 2) e_temp = 3;
			else if(e_temp == 3) e_temp = 5;
			else if(e_temp == 5) e_temp = 6;
			else if(e_temp == 6) e_temp = 0;
			else e_temp = 0;
			//else if(e_temp == 9) e_temp = 10;
			//else if(e_temp == 10) e_temp = 0;
		
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == DN)
		{
			if(e_temp == 0) e_temp = 6;
			else if(e_temp == 1) e_temp = 0;
			else if(e_temp == 2) e_temp = 1;
			else if(e_temp == 3) e_temp = 2;
			else if(e_temp == 5) e_temp = 3;
			else if(e_temp == 6) e_temp = 5;
			else e_temp = 0;
			//else if(e_temp == 9) e_temp = 6;
			//else if(e_temp == 10) e_temp = 9;

			RefreshFlag=1;
		}
	}
	else if(mode==EDIT_MODE_0_1)//selset
	{
		if(KeyState.KeyValue == ENTER)
		{
			WriteDataMem(addr, e_temp);
			Temporary = e_temp;
			Edit_flag = 0;
			posInpage = 0;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ESC)
		{
			Edit_flag = 0;
			posInpage = 0;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == UP)
		{
			if(e_temp == 0) e_temp = 1;
			else if(e_temp == 1) e_temp = 0;

		
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == DN)
		{
			if(e_temp == 0) e_temp = 1;
			else if(e_temp == 1) e_temp = 0;

			RefreshFlag=1;
		}
	}
	else if(mode==EDIT_MODE_P11_0)//selset
	{
		if(KeyState.KeyValue == ENTER)
		{
			WriteDataMem(addr, e_temp);
			Temporary = e_temp;
			Edit_flag = 0;
			posInpage = 0;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == ESC)
		{
			Edit_flag = 0;
			posInpage = 0;
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == UP)
		{
			if(e_temp == 2) e_temp = 3;
			else if(e_temp == 3) e_temp = 5;
			else if(e_temp == 5) e_temp = 6;
			else if(e_temp == 6) e_temp = 8;
			else if(e_temp == 8) e_temp = 9;
			else if(e_temp == 9) e_temp = 10;
			else if(e_temp == 10) e_temp = 2;
			else e_temp = 2;
		
			RefreshFlag=1;
		}
		else if(KeyState.KeyValue == DN)
		{
			if(e_temp == 2) e_temp = 10;
			else if(e_temp == 3) e_temp = 2;
			else if(e_temp == 5) e_temp = 3;
			else if(e_temp == 6) e_temp = 5;
			else if(e_temp == 8) e_temp = 6;
			else if(e_temp == 9) e_temp = 8;
			else if(e_temp == 10) e_temp = 9;
			else e_temp = 2;

			RefreshFlag=1;
		}
	}
		e_temp = (unsigned int)BOUND(e_temp,MAX,Min);
//edit_Temp = e_temp;

	
}



void SYS_cursor_ON_Mode(unsigned char mode)
{
	if(mode==CURSOR_MODE_0)//password
	{
		if(pass_pos==0)     CLCD_cursor_ON(0xC0,9);
		else if(pass_pos==1)    CLCD_cursor_ON(0xC0,10);
		else if(pass_pos==2)    CLCD_cursor_ON(0xC0,11);
		else if(pass_pos==3)    CLCD_cursor_ON(0xC0,12);
	}
	else if(mode==CURSOR_MODE_1)// _ _ _ _
	{
		CLCD_cursor_ON(0xC0, 11 - posInpage);
	}
	else if(mode==CURSOR_MODE_2)// x _ . _ _
	{
		if(3<posInpage)posInpage=3;
		if(posInpage <= 2)CLCD_cursor_ON(0xC0, 11 - posInpage);
		else CLCD_cursor_ON(0xC0, 10 - posInpage);
	}
	else if(mode==CURSOR_MODE_3)// x x _ . _
	{
		if(2<posInpage)posInpage=2;
		if(posInpage <= 1)CLCD_cursor_ON(0xC0, 11 - posInpage);
		else CLCD_cursor_ON(0xC0, 10 - posInpage);
	}
	else if(mode==CURSOR_MODE_4)//select
	{
		CLCD_cursor_ON(0xC0, 1);
	}
	else if(mode==CURSOR_MODE_5)// x _ _ . _
	{
		if(3<posInpage)posInpage=3;
		if(posInpage <= 1)CLCD_cursor_ON(0xC0, 11 - posInpage);
		else CLCD_cursor_ON(0xC0, 10 - posInpage);
	}
	else if(mode==CURSOR_MODE_6)// _ _ _ . _
	{
		if(4<posInpage)posInpage=4;
		if(posInpage <= 1)CLCD_cursor_ON(0xC0, 11 - posInpage);
		else CLCD_cursor_ON(0xC0, 10 - posInpage);
	}
	else if(mode==CURSOR_MODE_7)// _ . _ _ _
	{
		if(4<posInpage)posInpage=4;
		if(posInpage <= 3)CLCD_cursor_ON(0xC0, 10 - posInpage);
		else CLCD_cursor_ON(0xC0, 9 - posInpage);
	}
	else if(mode==CURSOR_MODE_8)// _ . _ _ _ _
	{
		if(5<posInpage)posInpage=5;
		if(posInpage <= 4)CLCD_cursor_ON(0xC0, 10 - posInpage);
		else CLCD_cursor_ON(0xC0, 9 - posInpage);
	}
}


void SYS_AccessLevel_Mode(char access_level)
	{
		if(access_level <= AccessLevel)
		{
			Edit_flag = 1;
			posInpage = 1;
			edit_Temp = Temporary ;
		}
		else
		{
			if(access_level == LEVEL_0)CLCD_string(0xC0,"Prohibited. [L0]");
			else if(access_level == LEVEL_1)CLCD_string(0xC0,"Prohibited. [L1]");
			else if(access_level == LEVEL_2)CLCD_string(0xC0,"Prohibited. [L2]");
			else if(access_level == LEVEL_3)CLCD_string(0xC0,"Prohibited. [L3]");
			else if(access_level == LEVEL_4)CLCD_string(0xC0,"Prohibited. [L4]");
			else if(access_level == LEVEL_5)CLCD_string(0xC0,"Prohibited. [L5]");
			
			RefreshFlag = 0;
		}
	}



void SYS_0(void)
{

	if(KeyState.KeyValue == DN)MenuDisplay = SYS_6;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_1;
	else if(KeyState.KeyValue == ENTER) 
	{
		Temporary = ReadDataMem(Local_Remote);
		MenuDisplay = SYS_0_0;
	}
	else if(KeyState.KeyValue == 0xFE )MenuDisplay = SYS_0xFFFFFFFF;

	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[0][0]));
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_ROOT[1][0]));
	}

}

void SYS_1(void)
{
	if(KeyState.KeyValue == DN)MenuDisplay = SYS_0;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2;
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_1_0;
	else if(KeyState.KeyValue == 0xFE )MenuDisplay = SYS_0xFFFFFFFF;
	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[0][0]));
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_ROOT[2][0]));
	}
}
void SYS_2(void)
{
	if(KeyState.KeyValue == DN)MenuDisplay = SYS_1;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_3;
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_2_01;
	else if(KeyState.KeyValue == 0xFE )MenuDisplay = SYS_0xFFFFFFFF;

	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[0][0]));
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
	}
}
void SYS_3(void)
{
	if(KeyState.KeyValue == DN)MenuDisplay = SYS_2;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_4;
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_3_00;
	else if(KeyState.KeyValue ==0xFE )MenuDisplay = SYS_0xFFFFFFFF;

	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[0][0]));
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_ROOT[4][0]));
	}
}
void SYS_4(void)
{
	if(KeyState.KeyValue == DN)MenuDisplay = SYS_3;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_5;
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_4_00;
	else if(KeyState.KeyValue == 0xFE )MenuDisplay = SYS_0xFFFFFFFF;

	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[0][0]));
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_ROOT[5][0]));
	}
}
void SYS_5(void)
{
	if(KeyState.KeyValue == DN)MenuDisplay = SYS_4;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_6;
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_5_00;
	else if(KeyState.KeyValue == 0xFE )MenuDisplay = SYS_0xFFFFFFFF;

	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[0][0]));
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_ROOT[6][0]));
	}
}
void SYS_6(void)
{
	if(KeyState.KeyValue == DN)MenuDisplay = SYS_5;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_0;
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_6_00;
	else if(KeyState.KeyValue == 0xFE )MenuDisplay = SYS_0xFFFFFFFF;

	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[0][0]));
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_ROOT[7][0]));
	}
}


void SYS_0_0(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_0;
	else if(KeyState.KeyValue == DN)
	{
		Temporary = ReadDataMem(PID_ref);
		MenuDisplay = SYS_0_5;
	}
	else if(KeyState.KeyValue == UP)
	{
		Temporary = ReadDataMem(Direction);
		MenuDisplay = SYS_0_1;
	}
	else if(KeyState.KeyValue == ENTER)
	{
		Temporary = ReadDataMem(Local_Remote);
		if(!Temporary) WriteDataMem(Local_Remote,1);
		else WriteDataMem(Local_Remote, 0);
		RefreshFlag = 1;
	}

	
	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_0_X[0][0]));

		Temporary = ReadDataMem(Local_Remote);
		if(Temporary) CLCD_string(0xC0,"    [ LOCAL ]    ");
		else CLCD_string(0xC0,"   [ REMOTE ]   ");
	}
}

void SYS_0_1(void)
{
	if(device_type == DEV_IM)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_0;
		else if(KeyState.KeyValue == DN)
		{
			Temporary = ReadDataMem(Local_Remote);
			MenuDisplay = SYS_0_0;
		}
		else if(KeyState.KeyValue == ENTER)
		{
			Temporary = ReadDataMem(Direction);
			if(!Temporary) WriteDataMem(Direction, 1);
			else WriteDataMem(Direction, 0);
			RefreshFlag = 1;
		}
		else if(KeyState.KeyValue == UP)
		{
			Temporary = ReadDataMem(Speed_ref);
			MenuDisplay = SYS_0_2;
		}

		if(RefreshFlag)
		{
			CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_0_X[1][0]));

			Temporary = ReadDataMem(Direction);
			if(!Temporary)CLCD_string(0xC0,"   [ FORWARD ]   ");
			else CLCD_string(0xC0,"   [ REVERSE ]   ");
		}
	}
	else if(device_type == DEV_DCDC)
	{
		if(!Edit_flag)
		{
			if(KeyState.KeyValue == ESC)MenuDisplay = SYS_0;
			else if(KeyState.KeyValue == DN)MenuDisplay = SYS_0_0;
			else if(KeyState.KeyValue == UP)MenuDisplay = SYS_0_0;
			else if(KeyState.KeyValue == ENTER)
			{
				Edit_flag = 1;
				posInpage = 1;
				edit_Temp = Temporary ;
			}
		}
		else
		{
			SYS_ParameterEdt(2301, edit_Temp, 65000, 0, EDIT_MODE_4);
		}

		if(RefreshFlag)
		{
			Temporary = ReadDataMem(2301);
			CLCD_string(0x80,"[1] Vdc_Out_Set ");
			SYS_ParameterDisplay(MODE_5d_V);
		}
	}
}


void SYS_0_2(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_0;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_0_1;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_0_3;
		else if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			posInpage = 1;
			edit_Temp = Temporary ;
		}
	}
	else
	{
		SYS_ParameterEdt(Speed_ref, edit_Temp, 65000, 0, EDIT_MODE_4);
	}

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(Speed_ref);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_0_X[2][0]));
		SYS_ParameterDisplay(MODE_5d_rms);
	}
}
void SYS_0_3(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_0;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_0_2;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_0_4;
		else if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			posInpage = 1;
			edit_Temp = Temporary ;
		}
	}
	else
	{
		SYS_ParameterEdt(Freq_ref, edit_Temp, 65000, 0, EDIT_MODE_4);
	}

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(Freq_ref);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_0_X[3][0]));
		SYS_ParameterDisplay(MODE_3d_1u_Hz);
	}
}
void SYS_0_4(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_0;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_0_3;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_0_5;
		else if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			posInpage = 1;
			edit_Temp = Temporary ;
		}
	}
	else
	{
		SYS_ParameterEdt(Torque_ref, edit_Temp, 65000, 0, EDIT_MODE_4);
	}

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(Torque_ref);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_0_X[4][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}

void SYS_0_5(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_0;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_0_4;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_0_0;
		else if(KeyState.KeyValue == ENTER)
		{
			Edit_flag = 1;
			posInpage = 1;
			edit_Temp = Temporary ;
		}
	}
	else
	{
		SYS_ParameterEdt(PID_ref, edit_Temp, 65000, 0, EDIT_MODE_4);
	}

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(PID_ref);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_0_X[5][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}


void SYS_1_0(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_1;
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_1_0_00;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_1_2;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_1_1;

	if(RefreshFlag)
	{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[2][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_1_X[0][0]));
	}
}
void SYS_1_1(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_1;
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_1_1_00;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_1_0;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_1_2;

	if(RefreshFlag)
	{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[2][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_1_X[1][0]));
	}
}

void SYS_1_2(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_1;
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_1_2_00;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_1_1;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_1_0;

	if(RefreshFlag)
	{
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[2][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_1_X[2][0]));
	}

}




void SYS_1_0_00(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_1_0;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_1_0_13;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_1_0_01;

	if(TimeTic_500ms)
	{
		SCI_RequestData(2310);
		RefreshFlag = 1;
	}

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(2310);
		if(device_type == DEV_IM)
		{
			CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_0_XX[0][0]));
			SYS_ParameterDisplay(MODE_5u_rpm);
		}
		else if(device_type == DEV_DCDC)
		{
			CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_0_XX_DCDC[0][0]));
			SYS_ParameterDisplay(MODE_5u_V);
		}
	}
}
void SYS_1_0_01(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_1_0;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_1_0_00;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_1_0_02;

	if(TimeTic_500ms)
	{
		SCI_RequestData(2311);
		RefreshFlag = 1;
	}
	
	if(RefreshFlag)
	{
		Temporary = ReadDataMem(2311);
		if(device_type == DEV_IM)
		{
			CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_0_XX[1][0]));
			SYS_ParameterDisplay(MODE_3d_1u_Hz);
		}
		else if(device_type == DEV_DCDC)
		{
			CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_0_XX_DCDC[1][0]));
			SYS_ParameterDisplay(MODE_5d_A);
		}
	}
}
void SYS_1_0_02(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_1_0;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_1_0_01;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_1_0_03;

	if(TimeTic_500ms)
	{
		SCI_RequestData(2312);
		RefreshFlag = 1;
	}

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(2312);
		if(device_type == DEV_IM)
		{
			CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_0_XX[2][0]));
			SYS_ParameterDisplay(MODE_5d_V);
		}
		else if(device_type == DEV_DCDC)
		{
			CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_0_XX_DCDC[2][0]));
			SYS_ParameterDisplay(MODE_5u_V);
		}

	}
}
void SYS_1_0_03(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_1_0;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_1_0_02;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_1_0_04;

	if(TimeTic_500ms)
	{
		SCI_RequestData(2313);
		RefreshFlag = 1;
	}

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(2313);
		if(device_type == DEV_IM)
		{
			CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_0_XX[3][0]));
			SYS_ParameterDisplay(MODE_3d_1u_Arms);
		}
		else if(device_type == DEV_DCDC)
		{
			CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_0_XX_DCDC[3][0]));
			SYS_ParameterDisplay(MODE_5d_A);
		}
	}
}
void SYS_1_0_04(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_1_0;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_1_0_03;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_1_0_05;

	if(TimeTic_500ms)
	{
		SCI_RequestData(2314);
		RefreshFlag = 1;
	}
	
	if(RefreshFlag)
	{
		Temporary = ReadDataMem(2314);
		if(device_type == DEV_IM)
		{
			CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_0_XX[4][0]));
			SYS_ParameterDisplay(MODE_5u_V);
		}
		else if(device_type == DEV_DCDC)
		{
			CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_0_XX_DCDC[4][0]));
			SYS_ParameterDisplay(MODE_5d_A);
		}
	}
}
void SYS_1_0_05(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_1_0;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_1_0_04;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_1_0_06;

	if(TimeTic_500ms)
	{
		SCI_RequestData(2315);
		RefreshFlag = 1;
	}


	if(RefreshFlag)
	{
		Temporary = ReadDataMem(2315);
		if(device_type == DEV_IM)
		{
			CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_0_XX[5][0]));
			SYS_ParameterDisplay(MODE_3d_1u);
		}
		else if(device_type == DEV_DCDC)
		{
			CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_0_XX_DCDC[5][0]));
			SYS_ParameterDisplay(MODE_5d_A);
		}
	}
}
void SYS_1_0_06(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_1_0;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_1_0_05;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_1_0_07; 

	if(TimeTic_500ms)
	{
		SCI_RequestData(2316);
		RefreshFlag = 1;
	}

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(2316);
		if(device_type == DEV_IM)
		{
			CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_0_XX[6][0]));
			SYS_ParameterDisplay(MODE_3d_1u_A);
		}
		else if(device_type == DEV_DCDC)
		{
			CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_0_XX_DCDC[6][0]));
			SYS_ParameterDisplay(MODE_5d_A);
		}
	}
}
void SYS_1_0_07(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_1_0;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_1_0_06;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_1_0_08; 

	if(TimeTic_500ms)
	{
		SCI_RequestData(2317);
		RefreshFlag = 1;
	}

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(2317);
		if(device_type == DEV_IM)
		{
			CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_0_XX[7][0]));
			SYS_ParameterDisplay(MODE_3d_1u_A);
		}
		else if(device_type == DEV_DCDC)
		{
			CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_0_XX_DCDC[7][0]));
			SYS_ParameterDisplay(MODE_5d_V);
		}
	}
}
void SYS_1_0_08(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_1_0;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_1_0_07;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_1_0_09; 

	if(TimeTic_500ms)
	{
		SCI_RequestData(2318);
		RefreshFlag = 1;
	}

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(2318);
		if(device_type == DEV_IM)
		{
			CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_0_XX[8][0]));
			SYS_ParameterDisplay(MODE_3d_1u_kW);
		}
		else if(device_type == DEV_DCDC)
		{
			CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_0_XX_DCDC[8][0]));
			SYS_ParameterDisplay(MODE_3d_1u_kW);
		}
	}
}
void SYS_1_0_09(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_1_0;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_1_0_08;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_1_0_10; 

	if(TimeTic_500ms)
	{
		SCI_RequestData(2319);
		RefreshFlag = 1;
	}

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(2319);
		if(device_type == DEV_IM)
		{
			CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_0_XX[9][0]));
			SYS_ParameterDisplay(MODE_3d_1u_kW);
		}
		else if(device_type == DEV_DCDC)
		{
			CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_0_XX_DCDC[9][0]));
			SYS_ParameterDisplay(MODE_3d_1u_kW);
		}
	}
}
void SYS_1_0_10(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_1_0;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_1_0_09;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_1_0_11; 

	if(TimeTic_500ms)
	{
		SCI_RequestData(2320);
		RefreshFlag = 1;
	}

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(2320);
		if(device_type == DEV_IM)
		{
			CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_0_XX[10][0]));
			SYS_ParameterDisplay(MODE_3d_1u);
		}
		else if(device_type == DEV_DCDC)
		{
			CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_0_XX_DCDC[10][0]));
			SYS_ParameterDisplay(MODE_5d_A);
		}
	}
}
void SYS_1_0_11(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_1_0;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_1_0_10;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_1_0_12;   

	if(TimeTic_500ms)
	{
		SCI_RequestData(2321);
		RefreshFlag = 1;
	}

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(2321);
		if(device_type == DEV_IM)
		{
			CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_0_XX[11][0]));
			SYS_ParameterDisplay(MODE_3d_1u);
		}
		else if(device_type == DEV_DCDC)
		{
			CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_0_XX_DCDC[11][0]));
			SYS_ParameterDisplay(MODE_5d_A);
		}
	}
}
void SYS_1_0_12(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_1_0;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_1_0_11;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_1_0_13; 

	if(TimeTic_500ms)
	{
		SCI_RequestData(2322);
		RefreshFlag = 1;
	}

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(2322);
		if(device_type == DEV_IM)
		{
			CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_0_XX[12][0]));
			SYS_ParameterDisplay(MODE_3d_1u);
		}
		else if(device_type == DEV_DCDC)
		{
			CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_0_XX_DCDC[12][0]));
			SYS_ParameterDisplay(MODE_5d_A);
		}
	}
}
void SYS_1_0_13(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_1_0;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_1_0_12;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_1_0_00; 

	if(TimeTic_500ms)
	{
		SCI_RequestData(2323);
		RefreshFlag = 1;
	}

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(2323);
		if(device_type == DEV_IM)
		{
			CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_0_XX[13][0]));
			SYS_ParameterDisplay(MODE_3d_1u_deg);
		}
		else if(device_type == DEV_DCDC)
		{
			CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_0_XX_DCDC[13][0]));
			SYS_ParameterDisplay(MODE_3d_1u_deg);
		}
	}
}







void SYS_1_1_00(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_1_1;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_1_1_04;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_1_1_01;

	if(TimeTic_500ms)
	{
		SCI_RequestData(2340);
		RefreshFlag = 1;
	}

	if(RefreshFlag){
		Temporary = ReadDataMem(2340);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_1_XX[0][0]));
		CLCD_string(0xC0,(char*)_TEXT("     [%08b]", ReadDataMem(2340)));
	}
}
void SYS_1_1_01(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_1_1;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_1_1_00;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_1_1_04;

	if(TimeTic_500ms)
	{
		SCI_RequestData(2341);
		RefreshFlag = 1;
	}
	
	if(RefreshFlag){
		Temporary = ReadDataMem(2341);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_1_XX[1][0]));
		CLCD_string(0xC0,(char*)_TEXT("     [%03b]     ", ReadDataMem(2341)));
	}
}
void SYS_1_1_02(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_1_1;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_1_1_01;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_1_1_03; 

	if(TimeTic_500ms)
	{
		SCI_RequestData(2342);
		RefreshFlag = 1;
	}
	if(RefreshFlag){
		Temporary = ReadDataMem(2342);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_1_XX[2][0]));
		CLCD_string(0xC0,(char*)_TEXT("      % 5u V   ",DATA_Registers[2342]/10,DATA_Registers[2342]%10));
	}
}
void SYS_1_1_03(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_1_1;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_1_1_02;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_1_1_04;

	if(TimeTic_500ms)
	{
		SCI_RequestData(2343);
		RefreshFlag = 1;
	}
	if(RefreshFlag){
		Temporary = ReadDataMem(2343);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_1_XX[3][0]));
		CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u mA  ",(int)DATA_Registers[2343]/1000,DATA_Registers[2343]%1000));
	}
}
void SYS_1_1_04(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_1_1;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_1_1_01;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_1_1_00;

	if(TimeTic_500ms)
	{
		SCI_RequestData(2344);
		RefreshFlag = 1;
	}

	if(RefreshFlag){
		Temporary = ReadDataMem(2344);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_1_XX[4][0]));
		CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u mA  ",(int)DATA_Registers[2344]/1000,DATA_Registers[2344]%1000));
	}
}





void SYS_1_2_00(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_1_2;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_1_2_07;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_1_2_01;

	if(RefreshFlag){
		Temporary = ReadDataMem(2323);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_2_XX_DCDC[0][0]));
		//SYS_ParameterDisplay(MODE_5d);
		CLCD_string(0xC0," Interleaved DC ");
	}
}
void SYS_1_2_01(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_1_2;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_1_2_00;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_1_2_02;
	if(RefreshFlag){
		Temporary = ReadDataMem(2351);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_2_XX_DCDC[1][0]));
		//SYS_ParameterDisplay(MODE_5d);
		CLCD_string(0xC0," Double Loop VI ");
	}
}
void SYS_1_2_02(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_1_2;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_1_2_01;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_1_2_03;
	if(RefreshFlag){
		Temporary = ReadDataMem(2352);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_2_XX_DCDC[2][0]));
		SYS_ParameterDisplay(MODE_5d);
	}
}
void SYS_1_2_03(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_1_2;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_1_2_02;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_1_2_04;
	if(RefreshFlag){
		Temporary = ReadDataMem(2353);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_2_XX_DCDC[3][0]));
		SYS_ParameterDisplay(MODE_5d);
	}
}
void SYS_1_2_04(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_1_2;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_1_2_03;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_1_2_05;

	if(TimeTic_500ms)
	{
		SCI_RequestData(2354);
		RefreshFlag = 1;
	}
	
	if(RefreshFlag){
		Temporary = ReadDataMem(2354);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_2_XX_DCDC[4][0]));
		SYS_ParameterDisplay(MODE_3d_1u_kW);
	}
}
void SYS_1_2_05(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_1_2;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_1_2_04;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_1_2_06;

	if(TimeTic_500ms)
	{
		SCI_RequestData(2355);
		RefreshFlag = 1;
	}
	
	if(RefreshFlag){
		Temporary = ReadDataMem(2355);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_2_XX_DCDC[5][0]));
		SYS_ParameterDisplay(MODE_3d_1u_V);
	}
}
void SYS_1_2_06(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_1_2;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_1_2_05;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_1_2_07;
	if(RefreshFlag){
		Temporary = ReadDataMem(2356);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_2_XX_DCDC[6][0]));
		CLCD_string(0xC0,(char*)_TEXT("      %d      ",(int)DATA_Registers[200]));
		//SYS_ParameterDisplay(MODE_5d);
	}
}
void SYS_1_2_07(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_1_2;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_1_2_06;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_1_2_00;
	if(RefreshFlag){
		Temporary = ReadDataMem(2357);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_1_2_XX_DCDC[7][0]));
		CLCD_string(0xC0,(char*)_TEXT("   Ver %01d.%02d% 1c    ",(int)DATA_Registers[201]/100,(int)DATA_Registers[201]%100,(int)DATA_Registers[202] ));
		//SYS_ParameterDisplay(MODE_5d);
	}
}
 



void SYS_2_00(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_23;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_01;
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_2_00_00;

	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_2_XX[0][0]));
	}
}
void SYS_2_01(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_21;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03;
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_2_01_00;

	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_2_XX[1][0]));
	}
}
void SYS_2_02(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_01;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03;
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_2_02_00;

	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_2_XX[2][0]));
	}
}
void SYS_2_03(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_01;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05;
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_2_03_00;
	
	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_2_XX[3][0]));
	}
}
void SYS_2_04(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05;
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_2_04_00;
	
	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_2_XX[4][0]));
	}
}
void SYS_2_05(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06;
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_2_05_00;
	
	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_2_XX[5][0]));
	}
}
void SYS_2_06(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_08;
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_2_06_00;
	
	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_2_XX[6][0]));
	}
}
void SYS_2_07(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_08;
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_2_07_00;
	
	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_2_XX[7][0]));
	}
}
void SYS_2_08(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_09;
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_2_08_00;
	
	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_2_XX[8][0]));
	}
}
void SYS_2_09(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_08;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_11;
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_2_09_00;
	
	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_2_XX[9][0]));
	}
}
void SYS_2_10(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_09;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_11;
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_2_10_00;
	
	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_2_XX[10][0]));
	}
}
void SYS_2_11(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_09;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_12;
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_2_11_00;
	
	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_2_XX[11][0]));
	}
}
void SYS_2_12(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_11;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_13;
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_2_12_00;
	
	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_2_XX[12][0]));
	}
}
void SYS_2_13(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_12;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19;
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_2_13_00;
	
	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_2_XX[13][0]));
	}
}
void SYS_2_14(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_13;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_15;
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_2_14_00;
	
	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_2_XX[14][0]));
	}
}
void SYS_2_15(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_14;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_16;
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_2_15_00;
	
	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_2_XX[15][0]));
	}
}
void SYS_2_16(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_15;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17;
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_2_16_00;
	
	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_2_XX[16][0]));
	}
}
void SYS_2_17(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_16;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18;
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_2_17_00;
	
	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_2_XX[17][0]));
	}
}
void SYS_2_18(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19;
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_2_18_00;
	
	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_2_XX[18][0]));
	}
}
void SYS_2_19(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_13;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_21;
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_2_19_00;
	
	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_2_XX[19][0]));
	}
}
void SYS_2_20(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_21;
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_2_20_00;
	
	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_2_XX[20][0]));
	}
}
void SYS_2_21(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_00;
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_2_21_00;
	
	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_2_XX[21][0]));
	}
}
void SYS_2_22(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_21;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_23;
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_2_22_00;
	
	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_2_XX[22][0]));
	}
}
void SYS_2_23(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_12;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_29;
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_2_23_00;
	
	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_2_XX[23][0]));
	}
}
void SYS_2_29(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_12;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_08;
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_2_29_00;
	
	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[3][0]));
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_2_XX[25][0]));
	}
}



void SYS_2_00_00(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_00;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_00_15;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_00_01;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	{
		SYS_ParameterEdt(200,  edit_Temp, 65000, 0, EDIT_MODE_4);
	}

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(200);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_00_XX[0][0]));
		SYS_ParameterDisplay(MODE_5u);
		
	}
}

void SYS_2_00_01(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_00;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_00_00;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_00_02;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else    SYS_ParameterEdt(201,  edit_Temp, 65000, 0, EDIT_MODE_2);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(201);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_00_XX[1][0]));
		SYS_ParameterDisplay(MODE_1);
	}
}

void SYS_2_00_02(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_00;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_00_01;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_00_03;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(202,  edit_Temp, 65000, 0, EDIT_MODE_2);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(202);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_00_XX[2][0]));
		SYS_ParameterDisplay(MODE_1);
	}
}
void SYS_2_00_03(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_00;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_00_02;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_00_04;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(203,  edit_Temp, 65000, 0, EDIT_MODE_2);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(203);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_00_XX[3][0]));
		SYS_ParameterDisplay(MODE_1);
	}
}
void SYS_2_00_04(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_00;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_00_03;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_00_05;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(204,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(204);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_00_XX[4][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_00_05(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_00;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_00_04;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_00_06;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(205,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(205);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_00_XX[5][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_00_06(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_00;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_00_05;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_00_07;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(206,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(206);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_00_XX[5][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_00_07(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_00;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_00_06;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_00_08;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(207,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(207);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_00_XX[7][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_00_08(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_00;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_00_07;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_00_09;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(208,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(208);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_00_XX[8][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_00_09(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_00;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_00_08;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_00_10;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(209,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(209);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_00_XX[9][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_00_10(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_00;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_00_09;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_00_11;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(210,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(210);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_00_XX[2][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_00_11(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_00;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_00_10;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_00_12;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(211,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(211);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_00_XX[11][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_00_12(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_00;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_00_11;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_00_13;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(212,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(212);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_00_XX[12][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_00_13(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_00;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_00_12;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_00_14;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(213,  edit_Temp, 65000, 0, EDIT_MODE_2);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(213);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_00_XX[13][0]));
		SYS_ParameterDisplay(MODE_2);
	}
}
void SYS_2_00_14(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_00;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_00_13;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_00_15;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(214,  edit_Temp, 65000, 0, EDIT_MODE_2);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(214);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_00_XX[14][0]));
		SYS_ParameterDisplay(MODE_3);
	}
}
void SYS_2_00_15(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_00;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_00_14;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_00_00;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(215,  edit_Temp, 65000, 0, EDIT_MODE_2);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(215);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_00_XX[15][0]));
		SYS_ParameterDisplay(MODE_3);
	}
}
 
void SYS_2_01_00(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_01;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_01_09;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_01_01;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(240,  edit_Temp, 1000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(240);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_01_XX[0][0]));
		SYS_ParameterDisplay(MODE_3d_1u_kW);
	}
}
void SYS_2_01_01(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_01;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_01_00;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_01_02;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(241,  edit_Temp, 1500, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(241);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_01_XX[1][0]));
		SYS_ParameterDisplay(MODE_3d_1u_Vrms);

	}
}
void SYS_2_01_02(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_01;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_01_01;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_01_03;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(242,  edit_Temp, 2000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(242);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_01_XX[2][0]));
		SYS_ParameterDisplay(MODE_3d_1u_Arms);
	}
}
void SYS_2_01_03(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_01;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_01_02;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_01_04;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(243,  edit_Temp, 300, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(243);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_01_XX[3][0]));
		SYS_ParameterDisplay(MODE_3d_1u_Hz);
	}
}
void SYS_2_01_04(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_01;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_01_03;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_01_05;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(244,  edit_Temp, 24, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(244);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_01_XX[4][0]));
		SYS_ParameterDisplay(MODE_5u_pole);
	}
}
void SYS_2_01_05(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_01;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_01_04;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_01_06;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(245,  edit_Temp, 18000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(245);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_01_XX[5][0]));
		SYS_ParameterDisplay(MODE_5u_rpm);
	}
}
void SYS_2_01_06(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_01;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_01_05;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_01_07;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(246,  edit_Temp, 3, 3, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(246);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_01_XX[6][0]));
		SYS_ParameterDisplay(MODE_11);
	}
}
void SYS_2_01_07(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_01;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_01_06;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_01_09;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(247,  edit_Temp, 100, 8, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(247);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_01_XX[7][0]));
		SYS_ParameterDisplay(MODE_3d_1u_kHz);
	}
}
void SYS_2_01_08(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_01;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_01_07;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_01_09;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(248,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(248);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_01_XX[8][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_01_09(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_01;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_01_07;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_01_00;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(249,  edit_Temp, 1500, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(249);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_01_XX[9][0]));
		SYS_ParameterDisplay(MODE_3d_1u_Vrms);
	}
}
 



void SYS_2_02_00(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_02;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_02_09;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_02_01;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(260,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(260);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_02_XX[0][0]));
		SYS_ParameterDisplay(MODE_3d_1u_kW);
	}
}
void SYS_2_02_01(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_02;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_02_00;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_02_02;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(261,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(261);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_02_XX[1][0]));
		SYS_ParameterDisplay(MODE_3d_1u_Vrms);
	}
}
void SYS_2_02_02(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_02;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_02_01;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_02_03;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(262,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(262);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_02_XX[2][0]));
		SYS_ParameterDisplay(MODE_3d_1u_Arms);
	}
}
void SYS_2_02_03(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_02;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_02_02;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_02_04;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(263,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(263);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_02_XX[3][0]));
		SYS_ParameterDisplay(MODE_3d_1u_Hz);
	}
}
void SYS_2_02_04(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_02;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_02_03;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_02_05;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(264,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(264);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_02_XX[4][0]));
		SYS_ParameterDisplay(MODE_5u_pole);
	}
}
void SYS_2_02_05(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_02;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_02_04;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_02_06;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(265,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(265);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_02_XX[5][0]));
		SYS_ParameterDisplay(MODE_5u_rpm);
	}
}
void SYS_2_02_06(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_02;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_02_05;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_02_07;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(266,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(266);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_02_XX[6][0]));
		SYS_ParameterDisplay(MODE_11);
	}
}
void SYS_2_02_07(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_02;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_02_06;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_02_08;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(267,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(267);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_02_XX[7][0]));
		SYS_ParameterDisplay(MODE_3d_1u_kHz);
	}
}
void SYS_2_02_08(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_02;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_02_07;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_02_09;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(268,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(268);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_02_XX[8][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_02_09(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_02;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_02_08;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_02_00;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(269,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(269);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_02_XX[9][0]));
		SYS_ParameterDisplay(MODE_3d_1u_Vrms);
	}
}
 



void SYS_2_03_00(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_34;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_01;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(280,  edit_Temp, 1, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(280);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[0][0]));
		SYS_ParameterDisplay(MODE_13);
	}
}
void SYS_2_03_01(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_00;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_09;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(281,  edit_Temp, 1, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(281);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[1][0]));
		SYS_ParameterDisplay(MODE_13);
	}
}
void SYS_2_03_02(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_01;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_03;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(282,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(282);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[2][0]));
		SYS_ParameterDisplay(MODE_3d_1u_s);
	}
}
void SYS_2_03_03(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_02;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_04;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(283,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(283);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[3][0]));
		SYS_ParameterDisplay(MODE_15);
	}
}
void SYS_2_03_04(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_03;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_05;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(284,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(284);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[4][0]));
		SYS_ParameterDisplay(MODE_3d_1u_s);
	}
}
void SYS_2_03_05(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_04;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_06;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(285,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(285);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[5][0]));
		SYS_ParameterDisplay(MODE_3d_1u_s);
	}
}
void SYS_2_03_06(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_05;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_07;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(286,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(286);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[6][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_03_07(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_06;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_08;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(287,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(287);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[7][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_03_08(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_07;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_09;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(288,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(288);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[8][0]));
		SYS_ParameterDisplay(MODE_18);
	}
}
void SYS_2_03_09(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_01;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_16;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(289,  edit_Temp, 3000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(289);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[9][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
 
void SYS_2_03_10(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_09;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_11;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(290,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(290);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[10][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_03_11(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_10;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_12;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(291,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(291);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[11][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_03_12(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_11;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_13;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(292,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(292);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[12][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_03_13(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_12;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_14;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(293,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(293);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[13][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_03_14(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_13;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_15;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(294,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(294);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[14][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_03_15(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_14;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_16;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(295,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(295);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[15][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_03_16(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_09;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_17;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(296,  edit_Temp, 3000, 1, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(296);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[16][0]));
		SYS_ParameterDisplay(MODE_3d_1u_s);
	}
}
void SYS_2_03_17(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_16;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_26;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(297,  edit_Temp, 3000, 1, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(297);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[17][0]));
		SYS_ParameterDisplay(MODE_3d_1u_s);
	}
}
void SYS_2_03_18(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_17;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_19;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(298,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(298);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[18][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_03_19(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_18;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_20;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(299,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(299);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[19][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
 
 
void SYS_2_03_20(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC){MenuDisplay = SYS_2_03;}
		else if(KeyState.KeyValue == DN){MenuDisplay = SYS_2_03_19;}
		else if(KeyState.KeyValue == UP){MenuDisplay = SYS_2_03_21;}
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(300,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(300);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[20][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_03_21(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_20;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_22;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(301,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(301);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[21][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_03_22(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_21;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_23;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(302,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(302);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[22][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_03_23(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_22;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_24;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(303,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(303);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[23][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_03_24(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_23;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_25;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(304,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(304);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[24][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_03_25(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_24;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_26;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(305,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(305);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[25][0]));
		SYS_ParameterDisplay(MODE_18);
	}
}
void SYS_2_03_26(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_17;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_33;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(306,  edit_Temp, 300, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(306);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[26][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_03_27(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_26;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_28;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(307,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(307);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[27][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_03_28(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_27;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_29;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(308,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(308);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[28][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_03_29(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_28;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_30;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(309,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(309);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[29][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}

 
void SYS_2_03_30(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_29;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_31;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(310,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(310);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[30][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_03_31(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_30;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_32;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(311,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(311);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[31][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_03_32(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_31;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_33;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(312,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(312);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[32][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_03_33(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_26;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_34;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(313,  edit_Temp, 3000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(313);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[33][0]));
		SYS_ParameterDisplay(MODE_3d_1u_s);
	}
}
void SYS_2_03_34(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_33;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_00;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(314,  edit_Temp, 3000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(314);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[34][0]));
		SYS_ParameterDisplay(MODE_3d_1u_s);
	}
}
void SYS_2_03_35(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_34;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_36;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(315,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(315);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[35][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_03_36(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_35;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_37;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(316,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(316);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[36][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_03_37(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_36;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_38;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(317,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(317);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[37][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_03_38(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_37;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_39;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(318,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(318);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[38][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_03_39(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_38;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_40;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(319,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(319);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[39][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_03_40(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_39;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_41;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(320,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(320);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[40][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_03_41(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_40;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_42;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(321,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(321);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[41][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_03_42(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_41;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_43;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(322,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(322);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[42][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_03_43(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_42;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_44;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(323,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(323);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[43][0]));
		SYS_ParameterDisplay(MODE_3d_1u_s);
	}
}
void SYS_2_03_44(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_43;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_45;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(324,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(324);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[44][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_03_45(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_44;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_46;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(325,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(325);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[45][0]));
		SYS_ParameterDisplay(MODE_3d_1u_s);
	}
}
void SYS_2_03_46(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_45;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_47;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(326,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(326);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[46][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_03_47(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_46;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_48;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(327,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(327);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[47][0]));
		SYS_ParameterDisplay(MODE_3d_1u_s);
	}
}
void SYS_2_03_48(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_47;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_49;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(328,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(328);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[48][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_03_49(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_48;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_40;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(329,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(329);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[49][0]));
		SYS_ParameterDisplay(MODE_15);
	}
}
void SYS_2_03_50(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_49;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_51;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(330,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(330);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[50][0]));
		SYS_ParameterDisplay(MODE_3d_1u);
	}
}
void SYS_2_03_51(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_50;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_52;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(331,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(331);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[51][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_03_52(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_03_51;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_03_00;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(332,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(332);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_03_XX[52][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}

 

void SYS_2_04_00(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_52;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_01;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(390,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(390);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[0][0]));
		SYS_ParameterDisplay(MODE_13);
	}
}
void SYS_2_04_01(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_00;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_02;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(391,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(391);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[1][0]));
		SYS_ParameterDisplay(MODE_13);
	}
}
void SYS_2_04_02(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_01;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_03;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(392,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(392);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[2][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_04_03(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_02;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_04;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(393,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(393);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[3][0]));
		SYS_ParameterDisplay(MODE_15);
	}
}
void SYS_2_04_04(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_03;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_05;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(394,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(394);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[4][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_04_05(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_04;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_06;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(395,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(395);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[5][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_04_06(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_05;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_07;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(396,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(396);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[6][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_04_07(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_06;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_08;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(397,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(397);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[7][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_04_08(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_07;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_09;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(398,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(398);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[8][0]));
		SYS_ParameterDisplay(MODE_18);
	}
}
void SYS_2_04_09(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_08;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_10;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(399, edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(399);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[9][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
 
void SYS_2_04_10(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_09;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_11;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(400,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(400);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[10][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_04_11(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_10;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_12;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(401,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(401);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[11][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_04_12(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_11;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_13;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(402,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(402);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[12][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_04_13(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_12;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_14;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(403,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(403);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[13][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_04_14(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_13;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_15;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(404,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(404);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[14][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_04_15(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_14;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_16;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(405,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(405);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[15][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_04_16(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_15;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_17;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(406,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(406);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[16][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_04_17(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_16;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_18;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(407,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(407);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[17][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_04_18(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_17;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_19;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(408,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(408);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[18][0]));
		SYS_ParameterDisplay(MODE_3d_1u_s);
	}
}
void SYS_2_04_19(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_18;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_20;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(409,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(409);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[19][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
 
 
void SYS_2_04_20(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_19;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_21;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(410,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(410);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[20][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_04_21(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_20;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_22;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(411,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(411);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[21][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_04_22(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_21;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_23;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(412,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(412);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[22][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_04_23(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_22;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_24;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(413,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(413);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[23][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_04_24(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_23;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_25;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(414,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(414);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[24][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_04_25(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_24;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_26;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(415,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(415);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[25][0]));
		SYS_ParameterDisplay(MODE_18);
	}
}
void SYS_2_04_26(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_25;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_27;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(416,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(416);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[26][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_04_27(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_26;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_28;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(417,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(417);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[27][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_04_28(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_27;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_29;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(418,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(418);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[28][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_04_29(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_28;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_30;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(419,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(419);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[29][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}

 
void SYS_2_04_30(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_29;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_31;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(420,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(420);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[30][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_04_31(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_30;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_32;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(421,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(421);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[31][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_04_32(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_31;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_33;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(422,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(422);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[32][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_04_33(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_32;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_34;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(423,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(423);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[33][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_04_34(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_33;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_35;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(424,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(424);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[34][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_04_35(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_34;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_36;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(425,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(425);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[35][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_04_36(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_35;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_37;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(426,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(426);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[36][0]));
		SYS_ParameterDisplay(MODE_5u_s);
				}
}
void SYS_2_04_37(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_36;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_38;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(427,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(427);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[37][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_04_38(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_37;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_39;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(428,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(428);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[38][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_04_39(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_38;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_40;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(429,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(429);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[39][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_04_40(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_39;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_41;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(430,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(430);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[40][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_04_41(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_40;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_42;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(431,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(431);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[41][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_04_42(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_41;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_43;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(432,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(432);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[42][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_04_43(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_42;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_44;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(433,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(433);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[43][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_04_44(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_43;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_45;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(434,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(434);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[44][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_04_45(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_44;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_46;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(435,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(435);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[45][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_04_46(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_45;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_47;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(436,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(436);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[46][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_04_47(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_46;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_48;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(437,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(437);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[47][0]));
		SYS_ParameterDisplay(MODE_5u);
				}
}
void SYS_2_04_48(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_47;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_49;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(438,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(438);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[48][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_04_49(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_48;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_40;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(439,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(439);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[49][0]));
		SYS_ParameterDisplay(MODE_15);
	}
}
void SYS_2_04_50(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_49;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_51;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(440,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(440);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[50][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_04_51(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_50;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_52;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(441,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(441);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[51][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}


void SYS_2_04_52(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_04;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_04_51;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_04_00;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(442,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(442);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_04_XX[52][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}

 

void SYS_2_05_00(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_40;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_07;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(500,  edit_Temp, 2000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(500);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[0][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_05_01(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_00;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_02;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(501,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(501);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[1][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_05_02(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_01;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_03;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(502,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(502);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[2][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_05_03(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_02;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_04;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(503,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(503);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[3][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_05_04(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_03;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_05;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(504,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(504);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[4][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_05_05(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_04;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_06;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(505,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(505);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[5][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_05_06(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_05;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_07;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(506,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(506);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[6][0]));
		SYS_ParameterDisplay(MODE_3d_1u_s);
	}
}
void SYS_2_05_07(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_00;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_08;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(507,  edit_Temp, 2500, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(507);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[7][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_05_08(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_07;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_09;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(508,  edit_Temp, 2500, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(508);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[8][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_05_09(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_08;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_11;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(509,  edit_Temp, 3000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(509);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[9][0]));
		SYS_ParameterDisplay(MODE_3d_1u_s);
	}
}
 
void SYS_2_05_10(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_09;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_11;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(510,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(510);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[10][0]));
		SYS_ParameterDisplay(MODE_22);
	}
}
void SYS_2_05_11(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_09;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_12;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(511,  edit_Temp, 3500, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(511);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[11][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_05_12(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_11;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_15;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(512,  edit_Temp, 1000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(512);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[12][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_05_13(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_12;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_14;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(513,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(513);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[13][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_05_14(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_13;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_15;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(514,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(514);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[14][0]));
		SYS_ParameterDisplay(MODE_5u_V);
	}
}
void SYS_2_05_15(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_12;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_18;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(515,  edit_Temp, 9000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(515);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[15][0]));
		SYS_ParameterDisplay(MODE_5u_V);
	}
}
void SYS_2_05_16(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_15;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_17;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(516,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(516);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[16][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_05_17(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_16;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_18;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(517,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(517);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[17][0]));
		SYS_ParameterDisplay(MODE_5u_V);
	}
}
void SYS_2_05_18(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_15;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_23;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(518,  edit_Temp, 10000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(518);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[18][0]));
		SYS_ParameterDisplay(MODE_5u_V);
	}
}
void SYS_2_05_19(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_18;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_20;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(519,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(519);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[19][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
 
 
void SYS_2_05_20(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_19;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_21;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(520,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(520);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[20][0]));
		SYS_ParameterDisplay(MODE_5u_Hz);
	}
}
void SYS_2_05_21(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_20;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_22;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(521,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(521);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[21][0]));
		SYS_ParameterDisplay(MODE_24);
	}
}
void SYS_2_05_22(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_21;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_23;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(522,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(522);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[22][0]));
		SYS_ParameterDisplay(MODE_5u_kHz);
	}
}
void SYS_2_05_23(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_18;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_24;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(523,  edit_Temp, 850, 300, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(523);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[23][0]));
		SYS_ParameterDisplay(MODE_5u_V);
	}
}
void SYS_2_05_24(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_23;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_40;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(524,  edit_Temp, 850, 300, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(524);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[24][0]));
		SYS_ParameterDisplay(MODE_5u_V);
	}
}
void SYS_2_05_25(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_24;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_26;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(525,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(525);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[25][0]));
		SYS_ParameterDisplay(MODE_25);
	}
}
void SYS_2_05_26(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_25;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_27;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(526,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(526);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[26][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_05_27(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_26;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_28;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(527,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(527);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[27][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_05_28(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_27;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_29;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(528,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(528);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[28][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_05_29(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_28;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_30;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(529,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(529);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[29][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}

 
void SYS_2_05_30(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_29;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_31;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(530,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(530);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[30][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_05_31(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_30;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_32;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(531,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(531);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[31][0]));
		SYS_ParameterDisplay(MODE_3d_1u_s);
	}
}
void SYS_2_05_32(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_31;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_33;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(532,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(532);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[32][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_05_33(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_32;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_34;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(533,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(533);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[33][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_05_34(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_33;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_35;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(534,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(534);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[34][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_05_35(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_34;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_36;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(535,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(535);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[35][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_05_36(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_35;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_37;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(536,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(536);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[36][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_05_37(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_36;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_38;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(537,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(537);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[37][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_05_38(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_37;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_39;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(538,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(538);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[38][0]));
		SYS_ParameterDisplay(MODE_3d_1u_s);
	}
}
void SYS_2_05_39(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_38;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_40;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(539,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(539);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[39][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_05_40(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_24;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_00;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(540,  edit_Temp, 200, 950, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(540);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[40][0]));
		SYS_ParameterDisplay(MODE_3d_1u_deg);
	}
}
void SYS_2_05_41(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_40;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_42;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(541,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(541);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[41][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_05_42(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_41;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_43;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(542,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(542);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[42][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_05_43(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_42;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_44;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(543,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(543);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[43][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_05_44(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_05;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_05_43;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_05_00;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(544,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(544);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_05_XX[44][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}




void SYS_2_06_00(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_22;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_01;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(600,  edit_Temp, 2, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(600);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[0][0]));
		SYS_ParameterDisplay(MODE_P6_0);
	}
}
void SYS_2_06_01(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_00;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_02;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(601,  edit_Temp, 1, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(601);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[1][0]));
		SYS_ParameterDisplay(MODE_P6_1);
	}
}
void SYS_2_06_02(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_01;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_03;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(602,  edit_Temp, 3, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(602);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[2][0]));
		SYS_ParameterDisplay(MODE_P6_AI_Type);
	}
}
void SYS_2_06_03(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_02;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_04;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(603,  edit_Temp, 2000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(603);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[3][0]));
		SYS_ParameterDisplay(MODE_5u_ms);
	}
}
void SYS_2_06_04(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_03;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_05;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(604,  edit_Temp, 100, -100, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(604);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[4][0]));
		SYS_ParameterDisplay(MODE_5u_mA_V);
	}
}
void SYS_2_06_05(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_04;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_06;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(605,  edit_Temp, 1000, -1000, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(605);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[5][0]));
		SYS_ParameterDisplay(MODE_5u_mA_V);
	}
}
void SYS_2_06_06(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_05;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_07;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(606,  edit_Temp, 100, -100, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(606);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[6][0]));
		SYS_ParameterDisplay(MODE_5u_mA_V);
	}
}
void SYS_2_06_07(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_06;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_08;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(607,  edit_Temp, 5000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(607);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[7][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_06_08(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_07;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_09;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(608,  edit_Temp, 5000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(608);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[8][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_06_09(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_08;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_10;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(609,  edit_Temp, 1, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(609);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[9][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
 
void SYS_2_06_10(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_09;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_11;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(610,  edit_Temp, 5, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(610);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[10][0]));
		SYS_ParameterDisplay(MODE_P6_AI_D_Step);
	}
}
void SYS_2_06_11(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_10;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_12;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(611,  edit_Temp, 1, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(611);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[11][0]));
		SYS_ParameterDisplay(MODE_P6_AI_U_Sel);
	}
}
void SYS_2_06_12(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_11;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_13;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(612,  edit_Temp, 1, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(612);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[12][0]));
		SYS_ParameterDisplay(MODE_P6_AI_Func);
	}
}
void SYS_2_06_13(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_12;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_14;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(613,  edit_Temp, 3, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(613);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[13][0]));
		SYS_ParameterDisplay(MODE_P6_AI_Type);
	}
}
void SYS_2_06_14(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_13;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_15;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(614,  edit_Temp, 2000, 1, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(614);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[14][0]));
		SYS_ParameterDisplay(MODE_5u_ms);
	}
}
void SYS_2_06_15(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_14;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_16;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(615,  edit_Temp, 100, -100, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(615);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[15][0]));
		SYS_ParameterDisplay(MODE_5u_mA_V);
	}
}
void SYS_2_06_16(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_15;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_17;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(616,  edit_Temp, 100, -100, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(616);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[16][0]));
		SYS_ParameterDisplay(MODE_5u_mA_V);
	}
}
void SYS_2_06_17(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_16;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_18;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(617,  edit_Temp, 100, -100, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(617);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[17][0]));
		SYS_ParameterDisplay(MODE_5u_mA_V);
	}
}
void SYS_2_06_18(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_17;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_19;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(618,  edit_Temp, 5000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(618);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[18][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_06_19(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_18;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_20;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(619,  edit_Temp, 5000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(619);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[19][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
 
 
void SYS_2_06_20(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_19;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_21;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(620,  edit_Temp, 1, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(620);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[20][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_06_21(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_20;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_22;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(621,  edit_Temp, 5, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(621);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[21][0]));
		SYS_ParameterDisplay(MODE_P6_AI_D_Step);
	}
}
void SYS_2_06_22(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_21;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_00;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(622,  edit_Temp, 1, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(622);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[22][0]));
		SYS_ParameterDisplay(MODE_P6_AI_U_Sel);
	}
}
void SYS_2_06_23(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_22;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_24;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(623,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(623);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[23][0]));
		SYS_ParameterDisplay(MODE_3d_1u_V);
	}
}
void SYS_2_06_24(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_23;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_25;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(624,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(624);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[24][0]));
		SYS_ParameterDisplay(MODE_3d_1u_mV);
	}
}
void SYS_2_06_25(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_24;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_26;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(625,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(625);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[25][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_06_26(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_25;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_27;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(626,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(626);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[26][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_06_27(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_26;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_28;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(627,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(627);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[27][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_06_28(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_27;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_29;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(628,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(628);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[28][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_06_29(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_28;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_30;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(629,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(629);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[29][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}

 
void SYS_2_06_30(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_29;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_31;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(630,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(630);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[30][0]));
		SYS_ParameterDisplay(MODE_P6_AI_Type);
	}
}
void SYS_2_06_31(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_30;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_32;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(631,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(631);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[31][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_06_32(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_31;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_33;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(632,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(632);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[32][0]));
		SYS_ParameterDisplay(MODE_3d_1u_ms);
	}
}
void SYS_2_06_33(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_03;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_32;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_34;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(633,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(633);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[33][0]));
		SYS_ParameterDisplay(MODE_5u_mA_V);
	}
}
void SYS_2_06_34(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_33;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_35;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(634,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(634);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[34][0]));
		SYS_ParameterDisplay(MODE_3d_1u_V);
	}
}
void SYS_2_06_35(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_34;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_36;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(635,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(635);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[35][0]));
		SYS_ParameterDisplay(MODE_3d_1u_mV);
	}
}
void SYS_2_06_36(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_35;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_37;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(636,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(636);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[36][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_06_37(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_36;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_38;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(637,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(637);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[37][0]));
		SYS_ParameterDisplay(MODE_3d_1u_V);
	}
}
void SYS_2_06_38(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_37;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_39;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(638,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(638);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[38][0]));
		SYS_ParameterDisplay(MODE_3d_1u_mV);
	}
}
void SYS_2_06_39(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_38;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_40;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(639,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(639);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[39][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_06_40(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_39;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_41;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(640,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(640);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[40][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
 
void SYS_2_06_41(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_40;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_42;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(641,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(641);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[41][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_06_42(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_41;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_43;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(642,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(642);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[42][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_06_43(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_42;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_44;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(643,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(643);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[43][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_06_44(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_43;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_45;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(644,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(644);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[44][0]));
		SYS_ParameterDisplay(MODE_P6_AI_Type);
	}
}
void SYS_2_06_45(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_44;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_46;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(645,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(645);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[45][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_06_46(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_45;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_47;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(646,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(646);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[46][0]));
		SYS_ParameterDisplay(MODE_3d_1u_ms);
	}
}
void SYS_2_06_47(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_46;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_48;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(647,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(647);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[47][0]));
		SYS_ParameterDisplay(MODE_5u_mA_V);
	}
}
void SYS_2_06_48(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_47;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_49;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(648,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(648);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[48][0]));
		SYS_ParameterDisplay(MODE_3d_1u_V);
	}
}
void SYS_2_06_49(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_48;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_50;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(649,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(649);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[49][0]));
		SYS_ParameterDisplay(MODE_3d_1u_mA);
	}
}
 
void SYS_2_06_50(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_49;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_51;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(650,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(650);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[50][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_06_51(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_50;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_52;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(651,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(651);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[51][0]));
		SYS_ParameterDisplay(MODE_3d_1u_V);
	}
}
void SYS_2_06_52(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_51;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_53;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(652,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(652);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[52][0]));
		SYS_ParameterDisplay(MODE_3d_1u_mA);
	}
}
void SYS_2_06_53(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_52;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_54;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(653,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(653);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[53][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_06_54(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_53;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_55;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(654,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(654);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[54][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_06_55(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_54;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_56;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(655,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(655);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[55][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_06_56(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_55;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_57;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(656,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(656);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[56][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_06_57(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_56;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_58;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(657,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(657);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[57][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_06_58(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_57;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_59;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(658,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(658);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[58][0]));
		SYS_ParameterDisplay(MODE_P6_AI_Type);
	}
}
void SYS_2_06_59(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_58;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_60;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(659,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(659);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[59][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
 
 
void SYS_2_06_60(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_59;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_61;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(660,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(660);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[60][0]));
		SYS_ParameterDisplay(MODE_3d_1u_ms);
	}
}
void SYS_2_06_61(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_60;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_62;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(661,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(661);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[61][0]));
		SYS_ParameterDisplay(MODE_5u_mA_V);
	}
}
void SYS_2_06_62(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_61;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_63;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(662,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(662);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[62][0]));
		SYS_ParameterDisplay(MODE_3d_1u_V);
	}
}
void SYS_2_06_63(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_62;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_64;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(663,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(663);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[63][0]));
		SYS_ParameterDisplay(MODE_3d_1u_mA);
	}
}
void SYS_2_06_64(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_63;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_65;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(664,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(664);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[64][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_06_65(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_64;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_66;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(665,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(665);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[65][0]));
		SYS_ParameterDisplay(MODE_3d_1u_V);
	}
}
void SYS_2_06_66(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_65;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_67;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(666,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(666);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[66][0]));
		SYS_ParameterDisplay(MODE_3d_1u_mV);
	}
}
void SYS_2_06_67(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_66;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_68;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(667,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(667);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[67][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_06_68(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_67;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_69;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(668,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(668);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[68][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_06_69(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_68;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_70;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(669,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(669);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[69][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}

 
void SYS_2_06_70(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_06;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_06_69;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_06_00;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(670,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(670);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_06_XX[70][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}





void SYS_2_07_00(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_07;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_07_28;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_07_01;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(750,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(750);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_07_XX[0][0]));
		SYS_ParameterDisplay(MODE_35);
	}
}
void SYS_2_07_01(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_07;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_07_00;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_07_02;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(751,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(751);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_07_XX[1][0]));
		SYS_ParameterDisplay(MODE_36);
	}
}
void SYS_2_07_02(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_07;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_07_01;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_07_03;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(752,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(752);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_07_XX[2][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_07_03(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_07;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_07_02;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_07_04;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(753,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(753);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_07_XX[3][0]));
		SYS_ParameterDisplay(MODE_37);
	}
}
void SYS_2_07_04(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_07;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_07_03;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_07_05;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(754,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(754);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_07_XX[4][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_07_05(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_07;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_07_04;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_07_06;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(755,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(755);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_07_XX[5][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_07_06(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_07;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_07_05;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_07_07;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(756,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(756);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_07_XX[6][0]));
		SYS_ParameterDisplay(MODE_5u_ms);
	}
}
void SYS_2_07_07(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_07;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_07_06;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_07_08;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(757,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(757);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_07_XX[7][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_07_08(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_07;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_07_07;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_07_09;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(758,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(758);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_07_XX[8][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_07_09(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_07;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_07_08;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_07_10;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(759,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(759);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_07_XX[9][0]));
		SYS_ParameterDisplay(MODE_5u_ms);
	}
}
 
void SYS_2_07_10(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_07;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_07_09;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_07_11;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(760,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(760);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_07_XX[10][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_07_11(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_07;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_07_10;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_07_12;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(761,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(761);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_07_XX[11][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_07_12(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_07;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_07_11;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_07_13;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(762,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(762);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_07_XX[12][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_07_13(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_07;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_07_12;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_07_14;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(763,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(763);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_07_XX[13][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_07_14(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_07;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_07_13;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_07_15;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(764,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(764);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_07_XX[14][0]));
		SYS_ParameterDisplay(MODE_5u_ms);
	}
}
void SYS_2_07_15(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_07;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_07_14;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_07_16;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(765,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(765);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_07_XX[15][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_07_16(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_07;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_07_15;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_07_17;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(766,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(766);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_07_XX[16][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_07_17(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_07;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_07_16;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_07_18;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(767,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(767);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_07_XX[17][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_07_18(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_07;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_07_17;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_07_19;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(768,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(768);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_07_XX[18][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_07_19(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_07;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_07_18;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_07_20;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(769,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(769);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_07_XX[19][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
 
 
void SYS_2_07_20(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_07;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_07_19;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_07_21;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(770,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(770);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_07_XX[20][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_07_21(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_07;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_07_20;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_07_22;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(771,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(771);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_07_XX[21][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_07_22(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_07;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_07_21;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_07_23;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(772,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(772);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_07_XX[22][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_07_23(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_07;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_07_22;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_07_24;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(773,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(773);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_07_XX[23][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_07_24(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_07;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_07_23;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_07_25;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(774,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(774);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_07_XX[24][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_07_25(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_07;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_07_24;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_07_26;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(775,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(775);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_07_XX[25][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_07_26(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_07;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_07_25;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_07_27;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(776,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(776);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_07_XX[26][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_07_27(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_07;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_07_26;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_07_28;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(777,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(777);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_07_XX[27][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_07_28(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_07;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_07_27;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_07_00;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(778,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(778);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_07_XX[28][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}


void SYS_2_08_00(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_08;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_08_06;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_08_01;
		//else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(810,  edit_Temp, 8, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(810);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_08_XX[0][0]));
		SYS_ParameterDisplay(MODE_P8_0_selection);
	}
}
void SYS_2_08_01(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_08;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_08_00;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_08_02;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(811,  edit_Temp, 29, 0, EDIT_MODE_P8_123456);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(811);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_08_XX[1][0]));
		SYS_ParameterDisplay(MODE_P8_1to29_selection);
	}
}
void SYS_2_08_02(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_08;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_08_01;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_08_03;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(812,  edit_Temp, 29, 0, EDIT_MODE_P8_123456);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(812);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_08_XX[2][0]));
		SYS_ParameterDisplay(MODE_P8_1to29_selection);
	}
}
void SYS_2_08_03(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_08;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_08_02;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_08_04;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(813,  edit_Temp, 29, 0, EDIT_MODE_P8_123456);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(813);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_08_XX[3][0]));
		SYS_ParameterDisplay(MODE_P8_1to29_selection);
	}
}
void SYS_2_08_04(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_08;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_08_03;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_08_05;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(814,  edit_Temp, 29, 0, EDIT_MODE_P8_123456);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(814);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_08_XX[4][0]));
		SYS_ParameterDisplay(MODE_P8_1to29_selection);
	}
}
void SYS_2_08_05(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_08;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_08_04;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_08_06;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(815,  edit_Temp, 29, 0, EDIT_MODE_P8_123456);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(815);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_08_XX[5][0]));
		SYS_ParameterDisplay(MODE_P8_1to29_selection);
	}
}
void SYS_2_08_06(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_08;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_08_05;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_08_00;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(816,  edit_Temp, 29, 0, EDIT_MODE_P8_123456);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(816);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_08_XX[6][0]));
		SYS_ParameterDisplay(MODE_P8_1to29_selection);
	}
}
void SYS_2_08_07(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_08;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_08_06;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_08_08;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(817,  edit_Temp, 65000, 0, EDIT_MODE_P8_123456);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(817);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_08_XX[7][0]));
		SYS_ParameterDisplay(MODE_P8_1to29_selection);
	}
}
void SYS_2_08_08(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_08;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_08_07;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_08_09;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(818,  edit_Temp, 65000, 0, EDIT_MODE_P8_123456);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(818);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_08_XX[8][0]));
		SYS_ParameterDisplay(MODE_P8_1to29_selection);
	}
}
void SYS_2_08_09(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_08;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_08_08;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_08_10;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(819,  edit_Temp, 65000, 0, EDIT_MODE_P8_123456);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(819);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_08_XX[9][0]));
		SYS_ParameterDisplay(MODE_P8_1to29_selection);
	}
}
 
void SYS_2_08_10(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_08;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_08_09;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_08_11;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(820,  edit_Temp, 65000, 0, EDIT_MODE_P8_123456);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(820);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_08_XX[10][0]));
		SYS_ParameterDisplay(MODE_P8_1to29_selection);
	}
}
void SYS_2_08_11(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_08;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_08_10;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_08_12;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(821,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(821);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_08_XX[11][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_08_12(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_08;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_08_11;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_08_13;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(822,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(822);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_08_XX[12][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_08_13(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_08;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_08_12;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_08_14;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(823,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(823);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_08_XX[13][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_08_14(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_08;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_08_13;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_08_15;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(824,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(824);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_08_XX[14][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_08_15(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_08;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_08_14;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_08_16;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(825,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(825);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_08_XX[15][0]));
		SYS_ParameterDisplay(MODE_3d_1u_s);
	}
}
void SYS_2_08_16(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_08;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_08_06;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_08_18;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(826,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(826);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_08_XX[16][0]));
		SYS_ParameterDisplay(MODE_3d_1u_s);
	}
}
void SYS_2_08_17(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_08;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_08_16;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_08_18;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(827,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(827);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_08_XX[17][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_08_18(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_08;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_08_16;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_08_00;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(828,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(828);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_08_XX[18][0]));
		SYS_ParameterDisplay(MODE_3d_1u_s);
	}
}
void SYS_2_08_19(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_08;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_08_18;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_08_00;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(829,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(829);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_08_XX[19][0]));
		SYS_ParameterDisplay(MODE_3d_1u_s);
	}
}






void SYS_2_09_00(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_09;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_09_16;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_09_01;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(860,  edit_Temp, 3000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(860);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_09_XX[0][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_09_01(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_09;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_09_00;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_09_02;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(861,  edit_Temp, 3000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(861);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_09_XX[1][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_09_02(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_09;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_09_01;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_09_03;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(862,  edit_Temp, 3000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(862);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_09_XX[2][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_09_03(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_09;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_09_02;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_09_04;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(863,  edit_Temp, 3000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(863);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_09_XX[3][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_09_04(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_09;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_09_03;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_09_05;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(864,  edit_Temp, 3000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(864);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_09_XX[4][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_09_05(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_09;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_09_04;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_09_06;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(865,  edit_Temp, 3000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(865);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_09_XX[5][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_09_06(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_09;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_09_05;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_09_07;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(866,  edit_Temp, 3000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(866);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_09_XX[6][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_09_07(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_09;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_09_06;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_09_08;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(867,  edit_Temp, 3000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(867);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_09_XX[7][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_09_08(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_09;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_09_07;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_09_09;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(868,  edit_Temp, 3000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(868);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_09_XX[8][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_09_09(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_09;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_09_08;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_09_10;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(869,  edit_Temp, 3000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(869);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_09_XX[9][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
 
void SYS_2_09_10(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_09;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_09_09;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_09_11;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(870,  edit_Temp, 3000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(870);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_09_XX[10][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_09_11(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_09;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_09_10;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_09_12;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(871,  edit_Temp, 3000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(871);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_09_XX[11][0]));
		if(!Edit_flag)
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_09_12(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_09;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_09_11;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_09_13;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(872,  edit_Temp, 3000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(872);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_09_XX[12][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_09_13(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_09;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_09_12;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_09_14;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(873,  edit_Temp, 3000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(873);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_09_XX[13][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_09_14(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_09;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_09_13;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_09_15;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(874,  edit_Temp, 3000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(874);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_09_XX[14][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_09_15(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_09;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_09_14;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_09_16;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(875,  edit_Temp, 3000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(875);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_09_XX[15][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_09_16(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_09;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_09_15;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_09_00;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(876,  edit_Temp, 1, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(876);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_09_XX[16][0]));
		SYS_ParameterDisplay(MODE_41);
	}
}





void SYS_2_10_00(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_10;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_10_16;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_10_01;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(900,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(900);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_10_XX[0][0]));

		if(DATA_Registers[916])SYS_ParameterDisplay(MODE_5u_Hz);
		else  SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_10_01(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_10;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_10_00;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_10_02;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(901,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(901);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_10_XX[1][0]));
		if(DATA_Registers[916])SYS_ParameterDisplay(MODE_5u_Hz);
		else  SYS_ParameterDisplay(MODE_5u_persent);

	}
}
void SYS_2_10_02(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_10;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_10_01;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_10_03;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(902,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(902);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_10_XX[2][0]));
		if(DATA_Registers[916])SYS_ParameterDisplay(MODE_5u_Hz);
		else  SYS_ParameterDisplay(MODE_5u_persent);

	}
}
void SYS_2_10_03(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_10;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_10_02;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_10_04;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(903,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(903);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_10_XX[3][0]));
		if(DATA_Registers[916])SYS_ParameterDisplay(MODE_5u_Hz);
		else  SYS_ParameterDisplay(MODE_5u_persent);

	}
}
void SYS_2_10_04(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_10;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_10_03;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_10_05;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(904,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(904);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_10_XX[4][0]));
		if(DATA_Registers[916])SYS_ParameterDisplay(MODE_5u_Hz);
		else  SYS_ParameterDisplay(MODE_5u_persent);

	}
}
void SYS_2_10_05(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_10;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_10_04;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_10_06;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(905,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(905);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_10_XX[5][0]));
		if(DATA_Registers[916])SYS_ParameterDisplay(MODE_5u_Hz);
		else  SYS_ParameterDisplay(MODE_5u_persent);

	}
}
void SYS_2_10_06(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_10;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_10_05;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_10_07;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(906,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(906);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_10_XX[6][0]));
		if(DATA_Registers[916])SYS_ParameterDisplay(MODE_5u_Hz);
		else  SYS_ParameterDisplay(MODE_5u_persent);

	}
}
void SYS_2_10_07(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_10;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_10_06;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_10_08;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(907,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(907);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_10_XX[7][0]));
		if(DATA_Registers[916])SYS_ParameterDisplay(MODE_5u_Hz);
		else  SYS_ParameterDisplay(MODE_5u_persent);

	}
}
void SYS_2_10_08(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_10;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_10_07;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_10_09;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(908,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(908);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_10_XX[8][0]));
		if(DATA_Registers[916])SYS_ParameterDisplay(MODE_5u_Hz);
		else  SYS_ParameterDisplay(MODE_5u_persent);

	}
}
void SYS_2_10_09(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_10;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_10_08;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_10_10;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(909,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(909);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_10_XX[9][0]));
		if(DATA_Registers[916])SYS_ParameterDisplay(MODE_5u_Hz);
		else  SYS_ParameterDisplay(MODE_5u_persent);

	}
}
 
void SYS_2_10_10(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_10;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_10_09;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_10_11;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(910,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(910);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_10_XX[10][0]));
		if(DATA_Registers[916])SYS_ParameterDisplay(MODE_5u_Hz);
		else  SYS_ParameterDisplay(MODE_5u_persent);

	}
}
void SYS_2_10_11(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_10;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_10_10;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_10_12;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(911,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(911);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_10_XX[11][0]));
		if(DATA_Registers[916])SYS_ParameterDisplay(MODE_5u_Hz);
		else  SYS_ParameterDisplay(MODE_5u_persent);

	}
}
void SYS_2_10_12(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_10;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_10_11;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_10_13;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(912,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(912);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_10_XX[12][0]));
		if(DATA_Registers[916])SYS_ParameterDisplay(MODE_5u_Hz);
		else  SYS_ParameterDisplay(MODE_5u_persent);

	}
}
void SYS_2_10_13(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_10;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_10_12;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_10_14;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(913,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(913);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_10_XX[13][0]));
		if(DATA_Registers[916])SYS_ParameterDisplay(MODE_5u_Hz);
		else  SYS_ParameterDisplay(MODE_5u_persent);

	}
}
void SYS_2_10_14(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_10;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_10_13;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_10_15;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(914,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(914);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_10_XX[14][0]));
		if(DATA_Registers[916])SYS_ParameterDisplay(MODE_5u_Hz);
		else  SYS_ParameterDisplay(MODE_5u_persent);

	}
}
void SYS_2_10_15(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_10;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_10_14;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_10_16;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(915,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(915);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_10_XX[15][0]));
		if(DATA_Registers[916])SYS_ParameterDisplay(MODE_5u_Hz);
		else  SYS_ParameterDisplay(MODE_5u_persent);

	}
}
void SYS_2_10_16(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_10;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_10_15;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_10_00;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(916,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(916);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_10_XX[16][0]));
		SYS_ParameterDisplay(MODE_41);
	}
}





void SYS_2_11_00(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_11;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_11_05;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_11_01;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(940,  edit_Temp, 10, 0, EDIT_MODE_P11_0);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(940);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_11_XX[0][0]));
		SYS_ParameterDisplay(MODE_43);
	}
}
void SYS_2_11_01(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_11;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_11_00;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_11_05;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(941,  edit_Temp, 1, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(941);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_11_XX[1][0]));
		SYS_ParameterDisplay(MODE_42);
	}
}
void SYS_2_11_02(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_11;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_11_01;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_11_03;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(942,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(942);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_11_XX[2][0]));
		SYS_ParameterDisplay(MODE_1d_4u_pu);
	}
}
void SYS_2_11_03(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_11;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_11_02;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_11_04;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(943,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(943);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_11_XX[3][0]));
		SYS_ParameterDisplay(MODE_1d_4u_pu);
	}
}
void SYS_2_11_04(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_11;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_11_03;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_11_05;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(944,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(944);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_11_XX[4][0]));
		SYS_ParameterDisplay(MODE_1d_3u_pu);
	}
}
void SYS_2_11_05(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_11;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_11_01;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_11_00;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(945,  edit_Temp, 300, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(945);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_11_XX[5][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_11_06(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_11;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_11_05;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_11_00;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(946,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(946);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_11_XX[6][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}






void SYS_2_12_00(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_12;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_12_02;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_12_01;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(960,  edit_Temp, 4, 0, EDIT_MODE_P12);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(960);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_12_XX[0][0]));
		SYS_ParameterDisplay(MODE_45);
	}
}
void SYS_2_12_01(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_12;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_12_00;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_12_02;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(961,  edit_Temp, 4, 0, EDIT_MODE_P12);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(961);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_12_XX[1][0]));
		SYS_ParameterDisplay(MODE_45);
	}
}
void SYS_2_12_02(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_12;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_12_01;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_12_00;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(962,  edit_Temp, 4, 0, EDIT_MODE_P12);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(962);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_12_XX[2][0]));
		SYS_ParameterDisplay(MODE_45);
	}
}
void SYS_2_12_03(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_12;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_12_02;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_12_04;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(963,  edit_Temp, 65000, 0, EDIT_MODE_P12);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(963);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_12_XX[3][0]));
		SYS_ParameterDisplay(MODE_45);
	}
}
void SYS_2_12_04(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_12;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_12_03;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_12_05;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(964,  edit_Temp, 65000, 0, EDIT_MODE_P12);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(964);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_12_XX[4][0]));
		SYS_ParameterDisplay(MODE_45);
	}
}
void SYS_2_12_05(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_12;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_12_04;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_12_06;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(965,  edit_Temp, 65000, 0, EDIT_MODE_P12);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(965);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_12_XX[5][0]));
		SYS_ParameterDisplay(MODE_45);
	}
}
void SYS_2_12_06(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_12;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_12_05;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_12_07;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(966,  edit_Temp, 65000, 0, EDIT_MODE_P12);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(966);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_12_XX[6][0]));
		SYS_ParameterDisplay(MODE_45);
	}
}
void SYS_2_12_07(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_12;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_12_06;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_12_00;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(967,  edit_Temp, 65000, 0, EDIT_MODE_P12);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(967);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_12_XX[7][0]));
		SYS_ParameterDisplay(MODE_45);
	}
}






void SYS_2_13_00(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_13;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_13_05;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_13_01;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(980,  edit_Temp, 100, -100, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(980);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_13_XX[0][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_13_01(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_13;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_13_00;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_13_02;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(981,  edit_Temp, 10, -10, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(981);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_13_XX[1][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_13_02(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_13;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_13_01;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_13_03;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(982,  edit_Temp, 150, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(982);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_13_XX[2][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_13_03(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_13;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_13_02;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_13_04;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(983,  edit_Temp, 5, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(983);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_13_XX[3][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_13_04(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_13;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_13_03;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_13_05;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(984,  edit_Temp, 100, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(984);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_13_XX[4][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_13_05(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_13;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_13_04;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_13_00;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(985,  edit_Temp, 10, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(985);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_13_XX[5][0]));
		SYS_ParameterDisplay(MODE_3d_1u);
	}
}
void SYS_2_13_06(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_13;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_13_05;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_13_07;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(986,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(986);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_13_XX[6][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_13_07(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_13;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_13_06;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_13_08;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(987,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(987);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_13_XX[7][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_13_08(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_13;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_13_07;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_13_09;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(988,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(988);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_13_XX[8][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_13_09(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_13;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_13_08;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_13_10;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(989,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(989);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_13_XX[9][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
 
void SYS_2_13_10(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_13;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_13_09;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_13_11;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(990,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(990);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_13_XX[10][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_13_11(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_13;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_13_10;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_13_00;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(991,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(991);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_13_XX[11][0]));
		SYS_ParameterDisplay(MODE_3d_1u_s);
	}
}







void SYS_2_14_00(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_14;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_14_08;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_14_01;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1010, edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1010);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_14_XX[0][0]));
		SYS_ParameterDisplay(MODE_47);
	}
}
void SYS_2_14_01(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_14;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_14_00;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_14_02;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1011,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1011);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_14_XX[1][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_14_02(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_14;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_14_01;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_14_03;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1012,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1012);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_14_XX[2][0]));
		SYS_ParameterDisplay(MODE_5u_rpm);
	}
}
void SYS_2_14_03(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_14;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_14_02;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_14_04;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1013,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1013);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_14_XX[3][0]));
		SYS_ParameterDisplay(MODE_5u_rpm);
	}
}
void SYS_2_14_04(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_14;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_14_03;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_14_05;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1014,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1014);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_14_XX[4][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_14_05(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_14;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_14_04;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_14_06;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1015,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1015);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_14_XX[5][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_14_06(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_14;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_14_05;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_14_07;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1016,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1016);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_14_XX[6][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_14_07(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_14;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_14_06;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_14_08;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1017,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1017);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_14_XX[7][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_14_08(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_14;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_14_07;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_14_00;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1018,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1018);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_14_XX[8][0]));
		SYS_ParameterDisplay(MODE_5u_Hz);
	}
}






void SYS_2_15_00(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_15;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_15_28;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_15_01;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1030, edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1030);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_15_XX[0][0]));
		SYS_ParameterDisplay(MODE_48);
	}
}
void SYS_2_15_01(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_15;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_15_00;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_15_02;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1031,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1031);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_15_XX[1][0]));
		SYS_ParameterDisplay(MODE_5u_Hz);
	}
}
void SYS_2_15_02(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_15;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_15_01;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_15_03;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1032,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1032);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_15_XX[2][0]));
		SYS_ParameterDisplay(MODE_5u_Hz);
	}
}
void SYS_2_15_03(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_15;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_15_02;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_15_04;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1033,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1033);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_15_XX[3][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_15_04(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_15;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_15_03;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_15_05;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1034,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1034);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_15_XX[4][0]));
		SYS_ParameterDisplay(MODE_5u_ms);
	}
}
void SYS_2_15_05(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_15;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_15_04;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_15_06;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1035,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1035);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_15_XX[5][0]));
		SYS_ParameterDisplay(MODE_5u_ms);
	}
}
void SYS_2_15_06(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_15;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_15_05;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_15_07;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1036,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1036);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_15_XX[6][0]));
		SYS_ParameterDisplay(MODE_49);
	}
}
void SYS_2_15_07(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_15;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_15_06;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_15_08;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1037,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1037);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_15_XX[7][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_15_08(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_15;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_15_07;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_15_09;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1038,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1038);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_15_XX[8][0]));
		SYS_ParameterDisplay(MODE_5u_Hz);
	}
}
void SYS_2_15_09(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_15;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_15_08;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_15_10;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1039,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1039);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_15_XX[9][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
 
void SYS_2_15_10(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_15;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_15_09;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_15_11;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1040,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1040);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_15_XX[10][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_15_11(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_15;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_15_10;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_15_12;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1041,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1041);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_15_XX[11][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_15_12(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_15;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_15_11;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_15_13;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1042,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1042);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_15_XX[12][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_15_13(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_15;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_15_12;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_15_14;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1043,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1043);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_15_XX[13][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_15_14(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_15;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_15_13;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_15_15;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1044,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1044);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_15_XX[14][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_15_15(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_15;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_15_14;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_15_16;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1045,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1045);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_15_XX[15][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_15_16(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_15;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_15_15;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_15_17;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1046,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1046);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_15_XX[16][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_15_17(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_15;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_15_16;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_15_18;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1047,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1047);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_15_XX[17][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_15_18(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_15;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_15_17;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_15_19;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1048,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1048);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_15_XX[18][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_15_19(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_15;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_15_18;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_15_20;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1049,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1049);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_15_XX[19][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
 
 
void SYS_2_15_20(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_15;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_15_19;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_15_21;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1050,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1050);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_15_XX[20][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_15_21(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_15;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_15_20;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_15_22;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1051,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1051);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_15_XX[21][0]));
		SYS_ParameterDisplay(MODE_5u_Hz);
	}
}
void SYS_2_15_22(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_15;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_15_21;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_15_23;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1052,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1052);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_15_XX[22][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_15_23(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_15;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_15_22;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_15_24;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1053,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1053);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_15_XX[23][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_15_24(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_15;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_15_23;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_15_25;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1054,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1054);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_15_XX[24][0]));
		SYS_ParameterDisplay(MODE_5u_ms);
	}
}
void SYS_2_15_25(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_15;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_15_24;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_15_26;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1055,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1055);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_15_XX[25][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_15_26(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_15;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_15_25;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_15_27;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1056,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1056);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_15_XX[26][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_15_27(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_15;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_15_26;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_15_28;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1057,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1057);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_15_XX[27][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_15_28(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_15;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_15_27;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_15_00;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1058,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1058);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_15_XX[28][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}






void SYS_2_16_00(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_16;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_16_28;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_16_01;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1090,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1090);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_16_XX[0][0]));
		SYS_ParameterDisplay(MODE_48);
	}
}
void SYS_2_16_01(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_16;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_16_00;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_16_02;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1091,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1091);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_16_XX[1][0]));
		SYS_ParameterDisplay(MODE_5u_Hz);
	}
}
void SYS_2_16_02(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_16;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_16_01;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_16_03;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1092,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1092);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_16_XX[2][0]));
		SYS_ParameterDisplay(MODE_5u_Hz);
	}
}
void SYS_2_16_03(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_16;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_16_02;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_16_04;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1093,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1093);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_16_XX[3][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_16_04(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_16;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_16_03;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_16_05;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1094,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1094);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_16_XX[4][0]));
		SYS_ParameterDisplay(MODE_5u_ms);
	}
}
void SYS_2_16_05(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_16;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_16_04;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_16_06;
	}
	else
	SYS_ParameterEdt(1095,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1095);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_16_XX[5][0]));
		SYS_ParameterDisplay(MODE_5u_ms);
	}
}
void SYS_2_16_06(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_16;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_16_05;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_16_07;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1096,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1096);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_16_XX[6][0]));
		SYS_ParameterDisplay(MODE_49);
	}
}
void SYS_2_16_07(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_16;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_16_06;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_16_08;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1097,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1097);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_16_XX[7][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_16_08(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_16;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_16_07;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_16_09;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1098,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1098);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_16_XX[8][0]));
		SYS_ParameterDisplay(MODE_5u_Hz);
	}
}
void SYS_2_16_09(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_16;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_16_08;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_16_10;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1099,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1099);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_16_XX[9][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
 
void SYS_2_16_10(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_16;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_16_09;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_16_11;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1100,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1100);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_16_XX[10][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_16_11(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_16;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_16_10;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_16_12;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1101,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1101);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_16_XX[11][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_16_12(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_16;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_16_11;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_16_13;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1102,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1102);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_16_XX[12][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_16_13(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_16;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_16_12;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_16_14;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1103,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1103);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_16_XX[13][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_16_14(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_16;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_16_13;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_16_15;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1104,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1104);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_16_XX[14][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_16_15(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_16;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_16_14;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_16_16;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1105,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1105);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_16_XX[15][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_16_16(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_16;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_16_15;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_16_17;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1106,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1106);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_16_XX[16][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_16_17(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_16;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_16_16;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_16_18;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1107,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1107);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_16_XX[17][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_16_18(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_16;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_16_17;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_16_19;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1108,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1108);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_16_XX[18][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_16_19(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_16;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_16_18;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_16_20;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1109,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1109);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_16_XX[19][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
 
 
void SYS_2_16_20(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_16;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_16_19;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_16_21;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1110,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1110);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_16_XX[20][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_16_21(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_16;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_16_20;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_16_22;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1111,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1111);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_16_XX[21][0]));
		SYS_ParameterDisplay(MODE_5u_Hz);
	}
}
void SYS_2_16_22(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_16;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_16_21;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_16_23;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1112,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1112);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_16_XX[22][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_16_23(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_16;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_16_22;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_16_24;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1113,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1113);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_16_XX[23][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_16_24(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_16;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_16_23;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_16_25;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1114,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1114);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_16_XX[24][0]));
		SYS_ParameterDisplay(MODE_5u_ms);
	}
}
void SYS_2_16_25(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_16;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_16_24;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_16_26;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1115,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1115);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_16_XX[25][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_16_26(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_16;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_16_25;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_16_27;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1116,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1116);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_16_XX[26][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_16_27(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_16;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_16_26;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_16_28;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1117,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1117);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_16_XX[27][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_16_28(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_16;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_16_27;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_16_00;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1118,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1118);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_16_XX[28][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}


 

void SYS_2_17_00(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_51;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_01;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1150,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1150);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[0][0]));
		SYS_ParameterDisplay(MODE_5u_ms);
	}
}
void SYS_2_17_01(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_00;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_02;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1151,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1151);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[1][0]));
		SYS_ParameterDisplay(MODE_5u_rpm);
	}
}
void SYS_2_17_02(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_01;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_03;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1152,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1152);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[2][0]));
		SYS_ParameterDisplay(MODE_5u_rpm);
	}
}
void SYS_2_17_03(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_02;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_04;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1153,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1153);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[3][0]));
		SYS_ParameterDisplay(MODE_5u_rpm);
	}
}
void SYS_2_17_04(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_03;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_05;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1154,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1154);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[4][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_17_05(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_04;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_06;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1155,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1155);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[5][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_17_06(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_05;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_07;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1156,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1156);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[6][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_17_07(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_06;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_08;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1157,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1157);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[7][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_17_08(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_07;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_09;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1158,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1158);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[8][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_17_09(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_08;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_10;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1159,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1159);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[9][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
 
void SYS_2_17_10(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_09;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_11;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1160,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1160);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[10][0]));
		SYS_ParameterDisplay(MODE_5u_ms);
	}
}
void SYS_2_17_11(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_10;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_12;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1161,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1161);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[11][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_17_12(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_11;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_13;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1162,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1162);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[12][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_17_13(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_12;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_14;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1163,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1163);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[13][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_17_14(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_13;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_15;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1164,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1164);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[14][0]));
		SYS_ParameterDisplay(MODE_50);
	}
}
void SYS_2_17_15(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_14;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_16;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1165,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1165);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[15][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_17_16(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_15;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_17;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1166,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1166);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[16][0]));
		SYS_ParameterDisplay(MODE_5u_ms);
	}
}
void SYS_2_17_17(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_16;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_18;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1167,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1167);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[17][0]));
		SYS_ParameterDisplay(MODE_5u_Hz);
	}
}
void SYS_2_17_18(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_17;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_19;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1168,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1168);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[18][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_17_19(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_18;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_20;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1169,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1169);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[19][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
 
 
void SYS_2_17_20(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_19;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_21;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1170,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1170);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[20][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_17_21(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_20;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_22;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1171,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1171);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[21][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_17_22(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_21;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_23;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1172,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1172);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[22][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_17_23(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_22;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_24;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1173,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1173);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[23][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_17_24(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_23;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_25;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1174,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1174);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[24][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_17_25(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_24;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_26;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1175,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1175);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[25][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_17_26(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_25;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_27;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1176,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1176);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[26][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_17_27(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_26;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_28;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1177,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1177);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[27][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_17_28(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_27;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_29;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1178,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1178);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[28][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_17_29(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_28;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_30;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1179,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1179);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[29][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}

 
void SYS_2_17_30(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_29;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_31;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1180,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1180);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[30][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_17_31(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_30;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_32;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1181,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1181);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[31][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_17_32(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_31;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_33;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1182,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1182);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[32][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_17_33(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_32;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_34;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1183,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1183);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[33][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_17_34(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_33;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_35;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1184,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1184);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[34][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_17_35(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_34;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_36;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1185,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1185);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[35][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_17_36(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_35;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_37;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1186,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1186);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[36][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_17_37(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_36;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_38;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1187,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1187);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[37][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_17_38(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_37;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_39;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1188,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1188);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[38][0]));
		SYS_ParameterDisplay(MODE_5u_rpm);
	}
}
void SYS_2_17_39(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_38;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_40;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1189,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1189);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[39][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_17_40(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_39;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_41;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1190,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1190);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[40][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}
void SYS_2_17_41(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_40;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_42;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1191,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1191);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[41][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_17_42(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_41;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_43;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1192,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1192);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[42][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_17_43(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_42;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_44;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1193,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1193);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[43][0]));
		SYS_ParameterDisplay(MODE_5u_ms);
	}
}
void SYS_2_17_44(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_43;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_45;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1194,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1194);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[44][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_17_45(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_44;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_46;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1195,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1195);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[45][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_17_46(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_45;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_47;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1196,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1196);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[46][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_17_47(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_46;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_48;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1197,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1197);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[47][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_17_48(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_47;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_49;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1198,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1198);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[48][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_17_49(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_48;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_40;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1199,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1199);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[49][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_17_50(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_49;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_51;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1200,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1200);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[50][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_17_51(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_17;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_17_50;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_17_00;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1201,  edit_Temp, 65000, 0, EDIT_MODE_0_1);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1201);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_17_XX[51][0]));
		SYS_ParameterDisplay(MODE_En_Dis);
	}
}


void SYS_2_18_00(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_49;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_01;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1260,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1260);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[0][0]));
		SYS_ParameterDisplay(MODE_5u_ms);
	}
}
void SYS_2_18_01(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_00;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_02;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1261,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1261);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[1][0]));
		SYS_ParameterDisplay(MODE_5u_rpm);
	}
}
void SYS_2_18_02(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_01;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_03;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1262,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1262);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[2][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_18_03(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_02;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_04;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1263,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1263);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[3][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_18_04(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_03;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_05;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1264,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1264);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[4][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_18_05(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_04;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_06;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1265,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1265);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[5][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_18_06(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_05;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_07;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1266,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1266);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[6][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_18_07(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_06;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_08;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1267,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1267);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[7][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_18_08(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_07;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_09;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1268,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1268);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[8][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_18_09(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_08;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_10;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1269,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1269);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[9][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
 
void SYS_2_18_10(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_09;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_11;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1270,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1270);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[10][0]));
		SYS_ParameterDisplay(MODE_5u_ms);
	}
}
void SYS_2_18_11(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_10;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_12;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1271,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1271);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[11][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_18_12(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_11;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_13;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1272,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1272);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[12][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_18_13(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_12;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_14;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1273,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1273);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[13][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_18_14(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_13;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_15;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1274,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1274);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[14][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_18_15(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_14;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_16;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1275,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1275);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[15][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_18_16(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_15;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_17;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1276,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1276);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[16][0]));
		SYS_ParameterDisplay(MODE_5u_ms);
	}
}
void SYS_2_18_17(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_16;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_18;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1277,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1277);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[17][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_18_18(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_17;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_19;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1278,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1278);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[18][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_18_19(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_18;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_20;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1279,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1279);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[19][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
 
 
void SYS_2_18_20(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_19;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_21;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1280,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1280);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[20][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_18_21(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_20;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_22;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1281,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1281);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[21][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_18_22(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_21;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_23;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1282,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1282);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[22][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_18_23(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_22;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_24;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1283,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1283);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[23][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_18_24(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_23;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_25;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1284,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1284);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[24][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_18_25(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_24;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_26;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1285,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1285);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[25][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_18_26(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_25;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_27;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1286,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1286);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[26][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_18_27(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_26;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_28;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1287,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1287);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[27][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_18_28(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_27;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_29;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1288,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1288);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[28][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_18_29(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_28;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_30;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1289,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1289);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[29][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}

 
void SYS_2_18_30(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_29;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_31;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1290,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1290);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[30][0]));
		SYS_ParameterDisplay(MODE_5u_Hz);
	}
}
void SYS_2_18_31(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_30;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_32;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1291,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1291);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[31][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_18_32(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_31;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_33;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1292,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1292);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[32][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_18_33(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_32;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_34;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1293,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1293);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[33][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_18_34(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_33;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_35;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1294,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1294);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[34][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_18_35(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_34;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_36;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1295,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1295);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[35][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_18_36(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_35;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_37;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1296,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1296);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[36][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_18_37(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_36;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_38;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1297,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1297);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[37][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_18_38(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_37;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_39;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1298,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1298);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[38][0]));
		SYS_ParameterDisplay(MODE_5u_rpm);
	}
}
void SYS_2_18_39(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_38;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_40;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1299,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1299);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[39][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_18_40(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_39;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_41;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1300,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1300);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[40][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_18_41(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_40;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_42;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1301,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1301);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[41][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_18_42(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_41;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_43;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1302,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1302);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[42][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_18_43(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_42;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_44;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1303,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1303);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[43][0]));
		SYS_ParameterDisplay(MODE_5u_ms);
	}
}
void SYS_2_18_44(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_43;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_45;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1304,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1304);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[44][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_18_45(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_44;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_46;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1305,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1305);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[45][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_18_46(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_45;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_47;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1306,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1306);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[46][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_18_47(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_46;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_48;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1307,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1307);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[47][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_18_48(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_47;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_49;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1308,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1308);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[48][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_18_49(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_18;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_18_48;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_18_00;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1309,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1309);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_18_XX[49][0]));
		SYS_ParameterDisplay(MODE_5u_ms);
	}
}



 

void SYS_2_19_00(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_05;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_01;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1370,  edit_Temp, 8192, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1370);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[0][0]));
		SYS_ParameterDisplay(MODE_5u_ppr);
	}
}
void SYS_2_19_01(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_00;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_03;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1371,  edit_Temp, 1, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1371);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[1][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_19_02(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_01;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_03;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1372,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1372);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[2][0]));
		SYS_ParameterDisplay(MODE_5u_ms);
	}
}
void SYS_2_19_03(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_01;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_04;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1373,  edit_Temp, 7200, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1373);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[3][0]));
		SYS_ParameterDisplay(MODE_5u_rpm);
	}
}
void SYS_2_19_04(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_03;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_05;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1374,  edit_Temp, 300, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1374);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[4][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_19_05(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_04;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_00;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1375,  edit_Temp, 320, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1375);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[5][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_19_06(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_05;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_07;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1376,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1376);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[6][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_19_07(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_06;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_08;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1377,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1377);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[7][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_19_08(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_07;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_09;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1378,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1378);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[8][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_19_09(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_08;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_10;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1379,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1379);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[9][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
 
void SYS_2_19_10(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_09;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_11;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1380,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1380);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[10][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_19_11(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_10;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_12;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1381,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1381);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[11][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_19_12(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_11;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_13;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1382,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1382);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[12][0]));
		SYS_ParameterDisplay(MODE_5u_ms);
	}
}
void SYS_2_19_13(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_12;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_14;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1383,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1383);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[13][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_19_14(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_13;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_15;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1384,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1384);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[14][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_19_15(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_14;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_16;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1385,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1385);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[15][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_19_16(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_15;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_17;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1386,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1386);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[16][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_19_17(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_16;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_18;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1387,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1387);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[17][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_19_18(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_17;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_19;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1388,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1388);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[18][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_19_19(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_18;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_20;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1389,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1389);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[19][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
 
 
void SYS_2_19_20(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_19;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_21;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1390,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1390);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[20][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_19_21(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_20;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_22;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1391,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1391);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[21][0]));
		SYS_ParameterDisplay(MODE_5u_ms);
	}
}
void SYS_2_19_22(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_21;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_23;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1392,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1392);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[22][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_19_23(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_22;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_24;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1393,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1393);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[23][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_19_24(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_23;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_25;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1394,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1394);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[24][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_19_25(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_24;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_26;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1395,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1395);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[25][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_19_26(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_25;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_27;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1396,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1396);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[26][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_19_27(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_26;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_28;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1397,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1397);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[27][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_19_28(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_27;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_29;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1398,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1398);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[28][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_19_29(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_28;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_30;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1399,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1399);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[29][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}

 
void SYS_2_19_30(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_29;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_31;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1400,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1400);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[30][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_19_31(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_30;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_32;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1401,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1401);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[31][0]));
		SYS_ParameterDisplay(MODE_5u_rpm);
	}
}
void SYS_2_19_32(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_31;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_33;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1402,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1402);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[32][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_19_33(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_32;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_34;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1403,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1403);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[33][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_19_34(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_33;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_35;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1404,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1404);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[34][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_19_35(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_34;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_36;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1405,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1405);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[35][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_19_36(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_35;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_37;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1406,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1406);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[36][0]));
		SYS_ParameterDisplay(MODE_5u_ms);
	}
}
void SYS_2_19_37(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_36;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_38;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1407,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1407);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[37][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_19_38(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_37;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_39;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1408,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1408);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[38][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_19_39(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_38;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_40;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1409,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1409);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[39][0]));
		SYS_ParameterDisplay(MODE_5u_ms);
	}
}
void SYS_2_19_40(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_39;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_41;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1410,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1410);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[40][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_19_41(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_19;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_19_40;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_19_00;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1411,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1411);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_19_XX[41][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}

 

void SYS_2_20_00(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_41;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_01;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1460,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1460);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[0][0]));
		SYS_ParameterDisplay(MODE_5u_ppr);
	}
}
void SYS_2_20_01(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_00;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_02;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1461,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1461);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[1][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_20_02(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_01;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_03;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1462,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1462);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[2][0]));
		SYS_ParameterDisplay(MODE_5u_ms);
	}
}
void SYS_2_20_03(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_02;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_04;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1463,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1463);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[3][0]));
		SYS_ParameterDisplay(MODE_5u_rpm);
	}
}
void SYS_2_20_04(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_03;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_05;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1464,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1464);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[4][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_20_05(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_04;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_06;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1465,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1465);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[5][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_20_06(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_05;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_07;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1466,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1466);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[6][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_20_07(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_06;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_08;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1467,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1467);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[7][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_20_08(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_07;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_09;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1468,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1468);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[8][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_20_09(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_08;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_10;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1469,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1469);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[9][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
 
void SYS_2_20_10(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_09;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_11;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1470,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1470);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[10][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_20_11(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_10;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_12;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1471,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1471);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[11][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_20_12(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_11;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_13;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1472,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1472);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[12][0]));
		SYS_ParameterDisplay(MODE_5u_ms);
	}
}
void SYS_2_20_13(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_12;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_14;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1473,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1473);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[13][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_20_14(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_13;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_15;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1474,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1474);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[14][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_20_15(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_14;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_16;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1475,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1475);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[15][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_20_16(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_15;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_17;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1476,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1476);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[16][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_20_17(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_16;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_18;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1477,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1477);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[17][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_20_18(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_17;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_19;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1478,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1478);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[18][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_20_19(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_18;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_20;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1479,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1479);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[19][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
 
 
void SYS_2_20_20(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_19;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_21;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1480,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1480);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[20][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_20_21(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_20;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_22;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1481,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1481);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[21][0]));
		SYS_ParameterDisplay(MODE_5u_ms);
	}
}
void SYS_2_20_22(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_21;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_23;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1482,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1482);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[22][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_20_23(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_22;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_24;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1483,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1483);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[23][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_20_24(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_23;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_25;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1484,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1484);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[24][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_20_25(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_24;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_26;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1485,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1485);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[25][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_20_26(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_25;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_27;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1486,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1486);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[26][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_20_27(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_26;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_28;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1487,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1487);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[27][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_20_28(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_27;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_29;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1488,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1488);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[28][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_20_29(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_28;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_30;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1489,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1489);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[29][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}

 
void SYS_2_20_30(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_29;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_31;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);

	}
	else
	SYS_ParameterEdt(1490,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1490);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[30][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_20_31(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_30;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_32;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1491,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1491);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[31][0]));
		SYS_ParameterDisplay(MODE_5u_rpm);
	}
}
void SYS_2_20_32(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_31;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_33;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1492,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1492);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[32][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_20_33(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_32;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_34;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1493,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1493);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[33][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_20_34(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_33;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_35;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1494,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1494);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[34][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_20_35(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_34;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_36;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1495,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1495);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[35][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_20_36(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_35;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_37;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1496,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1496);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[36][0]));
		SYS_ParameterDisplay(MODE_5u_ms);
	}
}
void SYS_2_20_37(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_36;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_38;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1497,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1497);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[37][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_20_38(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_37;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_39;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1498,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1498);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[38][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_20_39(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_38;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_40;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1499,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1499);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[39][0]));
		SYS_ParameterDisplay(MODE_5u_ms);
	}
}
void SYS_2_20_40(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_39;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_41;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1500,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1500);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[40][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_20_41(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_20;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_20_40;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_20_00;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1501,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1501);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_20_XX[41][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}


 

void SYS_2_21_00(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_21;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_21_06;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_21_02;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1550,  edit_Temp, 30000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1550);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_21_XX[0][0]));
		SYS_ParameterDisplay(MODE_5u_mOhm);
	}
}
void SYS_2_21_01(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_21;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_21_00;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_21_02;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1551,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1551);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_21_XX[1][0]));
		SYS_ParameterDisplay(MODE_5u_mOhm);
	}
}
void SYS_2_21_02(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_21;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_21_01;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_21_03;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1552,  edit_Temp, 30000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1552);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_21_XX[2][0]));
		SYS_ParameterDisplay(MODE_5u_mOhm);
	}
}
void SYS_2_21_03(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_21;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_21_02;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_21_04;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1553,  edit_Temp, 30000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1553);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_21_XX[3][0]));
		SYS_ParameterDisplay(MODE_5u_mH);
	}
}
void SYS_2_21_04(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_21;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_21_03;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_21_05;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1554,  edit_Temp, 30000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1554);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_21_XX[4][0]));
		SYS_ParameterDisplay(MODE_5u_mH);
	}
}
void SYS_2_21_05(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_21;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_21_04;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_21_06;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1555,  edit_Temp, 30000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1555);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_21_XX[5][0]));
		SYS_ParameterDisplay(MODE_5u_mH);
	}
}
void SYS_2_21_06(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_21;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_21_05;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_21_00;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1556,  edit_Temp, 30000, 1, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1556);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_21_XX[6][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_21_07(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_21;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_21_06;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_21_08;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1557,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1557);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_21_XX[7][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_21_08(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_21;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_21_07;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_21_09;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1558,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1558);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_21_XX[8][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_21_09(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_21;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_21_08;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_21_10;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1559,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1559);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_21_XX[9][0]));
		SYS_ParameterDisplay(MODE_5u_mOhm);
	}
}
 
void SYS_2_21_10(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_21;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_21_09;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_21_11;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1560,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1560);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_21_XX[10][0]));
		SYS_ParameterDisplay(MODE_5u_mH);
	}
}
void SYS_2_21_11(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_21;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_21_10;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_21_12;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1561,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1561);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_21_XX[11][0]));
		SYS_ParameterDisplay(MODE_5u_mH);
	}
}
void SYS_2_21_12(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_21;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_21_11;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_21_13;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1562,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1562);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_21_XX[12][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_21_13(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_21;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_21_12;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_21_14;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1563,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1563);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_21_XX[13][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_21_14(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_21;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_21_13;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_21_15;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1564,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1564);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_21_XX[14][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_21_15(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_21;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_21_14;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_21_00;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1565,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1565);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_21_XX[15][0]));
		SYS_ParameterDisplay(MODE_5u_rpm);
	}
}







 

void SYS_2_22_00(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_22;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_22_08;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_22_01;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1570,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1570);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_22_XX[0][0]));
		SYS_ParameterDisplay(MODE_5u_mOhm);
	}
}
void SYS_2_22_01(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_22;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_22_00;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_22_02;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1571,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1571);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_22_XX[1][0]));
		SYS_ParameterDisplay(MODE_5u_mOhm);
	}
}
void SYS_2_22_02(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_22;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_22_01;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_22_03;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1572,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1572);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_22_XX[2][0]));
		SYS_ParameterDisplay(MODE_5u_mOhm);
	}
}
void SYS_2_22_03(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_22;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_22_02;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_22_04;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1573,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1573);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_22_XX[3][0]));
		SYS_ParameterDisplay(MODE_5u_mH);
	}
}
void SYS_2_22_04(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_22;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_22_03;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_22_05;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1574,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1574);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_22_XX[4][0]));
		SYS_ParameterDisplay(MODE_5u_mH);
	}
}
void SYS_2_22_05(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_22;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_22_04;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_22_06;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1575,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1575);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_22_XX[5][0]));
		SYS_ParameterDisplay(MODE_5u_mH);
	}
}
void SYS_2_22_06(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_22;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_22_05;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_22_07;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1576,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1576);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_22_XX[6][0]));
		SYS_ParameterDisplay(MODE_5u_s);
	}
}
void SYS_2_22_07(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_22;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_22_06;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_22_08;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1577,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1577);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_22_XX[7][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}
void SYS_2_22_08(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_22;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_22_07;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_22_00;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1578,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1578);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_22_XX[8][0]));
		SYS_ParameterDisplay(MODE_5u_persent);
	}
}






 

void SYS_2_23_00(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_23;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_23_11;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_23_01;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1590,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1590);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_23_XX[0][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_23_01(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_23;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_23_00;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_23_02;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1591,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1591);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_23_XX[1][0]));
		SYS_ParameterDisplay(MODE_1d_4u_pu);
	}
}
void SYS_2_23_02(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_23;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_23_01;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_23_03;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1592,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1592);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_23_XX[2][0]));
		SYS_ParameterDisplay(MODE_1d_4u_pu);
	}
}
void SYS_2_23_03(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_23;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_23_02;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_23_04;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1593,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1593);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_23_XX[3][0]));
		SYS_ParameterDisplay(MODE_1d_4u_pu);
	}
}
void SYS_2_23_04(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_23;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_23_03;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_23_05;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1594,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1594);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_23_XX[4][0]));
		SYS_ParameterDisplay(MODE_1d_4u_pu);
	}
}
void SYS_2_23_05(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_23;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_23_04;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_23_06;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1595,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1595);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_23_XX[5][0]));
		SYS_ParameterDisplay(MODE_1d_4u_pu);
	}
}
void SYS_2_23_06(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_23;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_23_05;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_23_07;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1596,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1596);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_23_XX[6][0]));
		SYS_ParameterDisplay(MODE_3d_1u_A);
	}
}
void SYS_2_23_07(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_23;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_23_06;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_23_08;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1597,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1597);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_23_XX[7][0]));
		SYS_ParameterDisplay(MODE_3d_1u_A);
	}
}
void SYS_2_23_08(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_23;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_23_07;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_23_09;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1598,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1598);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_23_XX[8][0]));
		SYS_ParameterDisplay(MODE_3d_1u_A);
	}
}
void SYS_2_23_09(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_23;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_23_08;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_23_10;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1599,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1599);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_23_XX[9][0]));
		SYS_ParameterDisplay(MODE_3d_1u_A);
	}
}
 
void SYS_2_23_10(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_23;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_23_09;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_23_11;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1600,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1600);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_23_XX[10][0]));
		SYS_ParameterDisplay(MODE_3d_1u_A);
	}
}
void SYS_2_23_11(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_23;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_23_10;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_23_00;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1601,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1601);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_23_XX[11][0]));
		SYS_ParameterDisplay(MODE_3d_1u_A);
	}
}


void SYS_2_29_00(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_29;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_29_26;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_29_01;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1870,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1870);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_29_XX[0][0]));
		SYS_ParameterDisplay(MODE_3d_1u_kHz);
	}
}
void SYS_2_29_01(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_29;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_29_00;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_29_02;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1871,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1871);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_29_XX[1][0]));
		SYS_ParameterDisplay(MODE_5d_V);
	}
}
void SYS_2_29_02(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_29;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_29_01;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_29_03;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1872,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1872);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_29_XX[2][0]));
		SYS_ParameterDisplay(MODE_5d_V);
	}
}
void SYS_2_29_03(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_29;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_29_02;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_29_04;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1873,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1873);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_29_XX[3][0]));
		SYS_ParameterDisplay(MODE_3d_1u_kW);
	}
}

void SYS_2_29_04(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_29;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_29_03;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_29_05;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1874,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1874);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_29_XX[4][0]));
		SYS_ParameterDisplay(MODE_3d_1u_mOhm);
	}
}
void SYS_2_29_05(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_29;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_29_04;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_29_06;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1875,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1875);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_29_XX[5][0]));
		SYS_ParameterDisplay(MODE_3d_1u_mH);
	}
}
void SYS_2_29_06(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_29;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_29_05;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_29_07;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1876,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1876);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_29_XX[6][0]));
		SYS_ParameterDisplay(MODE_3d_1u_ms);
	}
}
void SYS_2_29_07(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_29;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_29_06;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_29_08;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1877,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1877);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_29_XX[7][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_29_08(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_29;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_29_07;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_29_09;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1878,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1878);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_29_XX[8][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_29_09(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_29;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_29_08;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_29_10;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1879,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1879);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_29_XX[9][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
 
void SYS_2_29_10(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_29;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_29_09;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_29_11;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1880,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1880);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_29_XX[10][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_29_11(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_29;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_29_10;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_29_12;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1881,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1881);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_29_XX[11][0]));
		SYS_ParameterDisplay(MODE_5u);
	}
}
void SYS_2_29_12(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_29;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_29_11;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_29_13;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1882,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1882);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_29_XX[12][0]));
		SYS_ParameterDisplay(MODE_5u_uF);
	}
}
void SYS_2_29_13(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_29;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_29_12;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_29_14;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1883,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1883);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_29_XX[13][0]));
		SYS_ParameterDisplay(MODE_5u_uF);
	}
}
void SYS_2_29_14(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_29;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_29_13;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_29_15;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1884,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1884);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_29_XX[14][0]));
		SYS_ParameterDisplay(MODE_3d_1u_Hz);
	}
}
void SYS_2_29_15(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_29;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_29_14;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_29_16;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1885,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1885);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_29_XX[15][0]));
		SYS_ParameterDisplay(MODE_3d_1u_Hz);
	}
}
void SYS_2_29_16(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_29;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_29_15;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_29_17;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1886,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1886);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_29_XX[16][0]));
		SYS_ParameterDisplay(MODE_5u_V);
	}
}
void SYS_2_29_17(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_29;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_29_16;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_29_18;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1887,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1887);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_29_XX[17][0]));
		SYS_ParameterDisplay(MODE_5u_V);
	}
}
void SYS_2_29_18(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_29;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_29_17;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_29_19;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1888,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1888);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_29_XX[18][0]));
		SYS_ParameterDisplay(MODE_5u_V);
	}
}

 void SYS_2_29_19(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_29;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_29_18;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_29_20;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1889,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1889);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_29_XX[19][0]));
		SYS_ParameterDisplay(MODE_5u_V);
	}
}
 
void SYS_2_29_20(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_29;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_29_19;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_29_21;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1890,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1890);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_29_XX[20][0]));
		SYS_ParameterDisplay(MODE_5d_A);
	}
}
void SYS_2_29_21(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_29;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_29_20;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_29_22;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1891,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1891);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_29_XX[21][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_29_22(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_29;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_29_21;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_29_23;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1892,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1892);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_29_XX[22][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_29_23(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_29;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_29_22;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_29_24;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1893,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1893);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_29_XX[23][0]));
		SYS_ParameterDisplay(MODE_3d_1u_persent);
	}
}
void SYS_2_29_24(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_29;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_29_23;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_29_25;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1894,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1894);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_29_XX[24][0]));
		SYS_ParameterDisplay(MODE_3d_1u_s);
	}
}
void SYS_2_29_25(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_29;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_29_24;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_29_26;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1895,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1895);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_29_XX[25][0]));
		SYS_ParameterDisplay(MODE_3d_1u_s);
	}
}
void SYS_2_29_26(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_2_29;
		else if(KeyState.KeyValue == DN)MenuDisplay = SYS_2_29_25;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_2_29_00;
		else if(KeyState.KeyValue == ENTER)SYS_AccessLevel_Mode( LEVEL_0);
	}
	else
	SYS_ParameterEdt(1896,  edit_Temp, 65000, 0, EDIT_MODE_4);

	if(RefreshFlag)
	{
		Temporary = ReadDataMem(1896);
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_2_29_XX[26][0]));
		SYS_ParameterDisplay(MODE_3d_1u_deg);
	}
}
 


void SYS_3_00(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_3;
		else if(KeyState.KeyValue == DN)MenuDisplay =SYS_3_02;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_3_01;
		else if(KeyState.KeyValue == ENTER)
		{ Edit_flag = 1;RefreshFlag=1;}

		if(RefreshFlag){
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[4][0]));
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX_3_4[0][0]));
		}
	}
	else
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_3;
		else if(KeyState.KeyValue == ENTER)
		{
			MenuDisplay = SYS_3_00_3;
			Edit_flag=0;
			WriteDataMem(2370,1);
			EventFlagB=1;
		}

		if(RefreshFlag){
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX_3_4[0][0]));
		CLCD_string(0xC0,"   ENTER / ESC  ");
		}
		
	}

}
void SYS_3_01(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_3;
		else if(KeyState.KeyValue == DN)MenuDisplay =SYS_3_00;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_3_02;
		else if(KeyState.KeyValue == ENTER)
		{ Edit_flag = 1;RefreshFlag=1;}

		if(RefreshFlag){
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[4][0]));
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX_3_4[1][0]));
		}
	}
	else
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_3;
		else if(KeyState.KeyValue == ENTER)
		{
			MenuDisplay = SYS_3_01_3;
			Edit_flag=0;
			WriteDataMem(2371,1);
			EventFlagB=1;
		}

		if(RefreshFlag){
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX_3_4[1][0]));
		CLCD_string(0xC0,"   ENTER / ESC  ");
		}
		
	}

}

void SYS_3_02(void)
{
	if(!Edit_flag)
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_3;
		else if(KeyState.KeyValue == DN)MenuDisplay =SYS_3_01;
		else if(KeyState.KeyValue == UP)MenuDisplay = SYS_3_00;
		else if(KeyState.KeyValue == ENTER)
		{ Edit_flag = 1;RefreshFlag=1;}

		if(RefreshFlag){
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[4][0]));
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX_3_4[2][0]));
		}
	}
	else
	{
		if(KeyState.KeyValue == ESC)MenuDisplay = SYS_3;
		else if(KeyState.KeyValue == ENTER)
		{
			MenuDisplay = SYS_3_02_3;
			Edit_flag=0;
			WriteDataMem(2372,1);
			EventFlagB=1;
		}

		if(RefreshFlag){
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX_3_4[2][0]));
		CLCD_string(0xC0,"   ENTER / ESC  ");
		}
		
	}

}




void SYS_3_00_3(void)
{
	static unsigned char flag=0;
	if((!EventFlagB)&&(flag))// event down edge
	{
		MenuDisplay = SYS_3_00_3_4;
	}
	flag = EventFlagB;
	if(RefreshFlag){
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX_3_4[0][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX_3_4[3][0]));
	}
}
void SYS_3_01_3(void)
{
	static unsigned char flag=0;
	if((!EventFlagB)&&(flag))// event down edge
	{
		MenuDisplay = SYS_3_01_3_4;
	}
	flag = EventFlagB;
	if(RefreshFlag) {
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX_3_4[1][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX_3_4[3][0]));
	}
}
void SYS_3_02_3(void)
{
	static unsigned char flag=0;
	if((!EventFlagB)&&(flag))// event down edge
	{
		MenuDisplay = SYS_3_02_3_4;
	}
	flag = EventFlagB;
	if(RefreshFlag){
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX_3_4[2][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX_3_4[3][0]));
	}
}

void SYS_3_00_3_4(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_3_00;
	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX_3_4[0][0]));
		
		if(DATA_Registers[2370])
		{
			CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX_3_4[3][0]));
		}
		else
		{
			CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX_3_4[4][0]));
		}
	}
}
void SYS_3_01_3_4(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_3_01;
	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX_3_4[1][0]));
		if(DATA_Registers[2371])
		{
			CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX_3_4[3][0]));
		}
		else
		{
			CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX_3_4[4][0]));
		}
	}
}
void SYS_3_02_3_4(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_3_02;
	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX_3_4[2][0]));
		if(DATA_Registers[2372])
		{
			CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX_3_4[3][0]));
		}
		else
		{
			CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_3_XX_3_4[4][0]));
		}
	}
}


void SYS_4_00(void)
{
	if(TimeTic_500ms)
	{
		SCI_RequestData(2381);
		RefreshFlag = 1;
	}
	
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_4;
	if(DATA_Registers[2381])
	{
		if(KeyState.KeyValue == ENTER)
		{
			MenuDisplay = SYS_4_00_00;
			SCI_SendData(2374, 1);
			Temporary = 0;
		}
		else if(KeyState.KeyValue == DN)
		{
			if(DATA_Registers[2382] <= 1)DATA_Registers[2382] = DATA_Registers[2381];
			else DATA_Registers[2382]--;
			RefreshFlag = 1;
		}
		else if(KeyState.KeyValue == UP)
		{
			DATA_Registers[2382]++;
			
			if(DATA_Registers[2381] < DATA_Registers[2382]) DATA_Registers[2382] = 1;
			RefreshFlag = 1;
		}
	}
	
	if(RefreshFlag)
	{
		CLCD_string(0x80,(char*)_TEXT("Total Fault = %d",DATA_Registers[2379]));
		if(DATA_Registers[2381])
		{
			CLCD_string(0xC0,(char*)_TEXT(" % 2d Record(%02d)  ",DATA_Registers[2382],DATA_Registers[2382]));
		}
		else
		{
			CLCD_string(0xC0,"No Value Record ");
		}
	}

	
}

void SYS_4_00_00(void)
{
	if(TimeTic_1s)
	{
		SCI_RequestData(2390);
		SCI_RequestData(2391);
		SCI_RequestData(2392);
		SCI_RequestData(2393);
		SCI_RequestData(2394);
		SCI_RequestData(2395);
		SCI_RequestData(2396);
		SCI_RequestData(2397);
		SCI_RequestData(2398);
		SCI_RequestData(2399);
		RefreshFlag = 1;
	}


	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_4_00;
	else if(KeyState.KeyValue == DN)
	{
		if(Temporary == 0)Temporary = 6;
		else Temporary--;
		RefreshFlag = 1;
	}
	else if(KeyState.KeyValue == UP)
	{
		Temporary++;
		if(6 < Temporary) Temporary = 0;
		RefreshFlag = 1;
	}

 
	if(RefreshFlag){
		if(Temporary==0)
		{
			//CLCD_string(0x80,((char*)_TEXT("20%02x/%02x/%02x %02x:%02x",DATA_Registers[2390]>>8,DATA_Registers[2390]&0x00FF,DATA_Registers[2391]>>8,DATA_Registers[2391]&0x00FF,DATA_Registers[2392]>>8,DATA_Registers[2392]&0x00FF))) ;
			CLCD_string(0x80,((char*)_TEXT("%02x%02x%02x %02x:%02x:%02x ",DATA_Registers[2390]>>8,DATA_Registers[2390]&0x00FF,DATA_Registers[2391]>>8,DATA_Registers[2391]&0x00FF,DATA_Registers[2392]>>8,DATA_Registers[2392]&0x00FF))) ;

			if(DATA_Registers[2393])CLCD_string(0xC0,(char*)_cpy_flash2memory(&FAULT_CODE[DATA_Registers[2393]-1][0]));
			else CLCD_string(0xC0,(char*)_TEXT(" irValue:% 3d",(int)DATA_Registers[2393]));
		}
		else if(Temporary==1)
		{
			CLCD_string(0x80,"*Input Current* ");
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u A    ",(int)DATA_Registers[2394]/10,DATA_Registers[2394]%10));
		}
		else if(Temporary==2)
		{
			CLCD_string(0x80,"*Input Voltage* ");
			CLCD_string(0xC0,(char*)_TEXT("      % 5u V    ",(int)DATA_Registers[2395]));
		}
		else if(Temporary==3)
		{
			CLCD_string(0x80,"*Output Voltage*");
			CLCD_string(0xC0,(char*)_TEXT("      % 5u V    ",(int)DATA_Registers[2396]));
		}
		else if(Temporary==4)
		{
			CLCD_string(0x80,"*Temperature*   ");
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u deg  ",(int)DATA_Registers[2397]/10,DATA_Registers[2397]%10));
		}
		else if(Temporary==5)
		{
			CLCD_string(0x80,"*Input Power*   ");
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u kW   ",(int)DATA_Registers[2398]/10,DATA_Registers[2398]%10));
		}
		/*else if(Temporary==6)
		{
			CLCD_string(0x80,"Phase Current   ");
			CLCD_string(0xC0,(char*)_TEXT("      % 3d.% 1u A    ",(int)DATA_Registers[2399]/10,DATA_Registers[2399]%10));
		}*/
		else if(Temporary==6)
		{
			CLCD_string(0x80,"*Fault State*   ");
			if((int)DATA_Registers[2399]==0)CLCD_string(0xC0,"Converter Init  ");
			else if((int)DATA_Registers[2399]==1)CLCD_string(0xC0,"Converter Stop  ");
			else if((int)DATA_Registers[2399]==2)CLCD_string(0xC0,"Converter Run   ");
			else if((int)DATA_Registers[2399]==3)CLCD_string(0xC0,"Converter Fault ");
			else if((int)DATA_Registers[2399]==4)CLCD_string(0xC0,"Converter Re-Try");
			else if((int)DATA_Registers[2399]==5)CLCD_string(0xC0,"Fault Record Rd ");      
			else CLCD_string(0xC0,(char*)_TEXT(" irValue:% 3d",(int)DATA_Registers[2399]));
		}

	}
}




void SYS_5_00(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_5;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_5_02;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_5_01;
 
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_5_00_04;
	if(RefreshFlag) {
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[6][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[0][0]));
}
}
void SYS_5_01(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_5;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_5_00;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_5_02; 
	else if(KeyState.KeyValue == ENTER)MenuDisplay = SYS_5_01_03;
	if(RefreshFlag){
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[6][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[1][0]));
}
}
void SYS_5_02(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_5;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_5_01;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_5_00;   
	else if(KeyState.KeyValue == ENTER)
	{
		MenuDisplay = SYS_5_02_05;
		EventFlagB=1;
	}
	if(RefreshFlag){
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[6][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[2][0]));
}
}

void SYS_5_00_04(void)
{
	static unsigned char flag=0;
	if((!EventFlagB)&&(flag))// event down edge
	{
		MenuDisplay = SYS_0;
	}
	flag = EventFlagB;
	
	if(RefreshFlag){
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[0][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[4][0]));
}
}
void SYS_5_01_03(void)
{
	
	if(KeyState.KeyValue == ENTER)
	{
		MenuDisplay = SYS_5_01_05;
		EventFlagB=1;
	}
	if(RefreshFlag){
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[1][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[3][0]));
}
}
void SYS_5_01_05(void)
{
	static unsigned char flag=0;
	if((!EventFlagB)&&(flag))// event down edge
	{
		MenuDisplay = SYS_0;
	}
	flag = EventFlagB;

	if(RefreshFlag){
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[1][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[5][0]));
}
}
void SYS_5_02_05(void)
{
	static unsigned char flag=0;
	if((!EventFlagB)&&(flag))// event down edge
	{
		MenuDisplay = SYS_0;
	}
	flag = EventFlagB;
	if(RefreshFlag){
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[2][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_5_XX_XX[5][0]));
}
}


void SYS_6_00(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_6;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_6_02;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_6_01;
	if(RefreshFlag) {
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[7][0]));
	CLCD_string(0xC0,(char*)_TEXT(  (char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[0][0]), AccessLevel)  );
	}
}
void SYS_6_01(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_6;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_6_00;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_6_02; 
	if(RefreshFlag){
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[7][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[1][0]));
}
}
void SYS_6_02(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_6;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_6_01;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_6_00;   

	if(RefreshFlag){
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_ROOT[7][0]));
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[2][0]));
}
}

void SYS_6_01_00(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_6_01;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_6_01_04;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_6_01_01;  
	else if(KeyState.KeyValue == ENTER)
	{
		MenuDisplay = SYS_6_01_00_05;
		SYS_password_clr();
		
	}
	if(RefreshFlag){
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[1][0]));
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[5][0]), 1))) ;
}
}
void SYS_6_01_01(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_6_01;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_6_01_00;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_6_01_02;    
	if(KeyState.KeyValue == ENTER)
	{
		MenuDisplay = SYS_6_01_01_05;
		SYS_password_clr();
	}
	if(RefreshFlag){
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[1][0]));
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[5][0]), 2))) ;
}
}
void SYS_6_01_02(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_6_01;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_6_01_01;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_6_01_03;    
	if(KeyState.KeyValue == ENTER)
	{
		MenuDisplay = SYS_6_01_02_05;
		SYS_password_clr();
	}
	if(RefreshFlag){
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[1][0]));
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[5][0]), 3))) ;
}
}
void SYS_6_01_03(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_6_01;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_6_01_02;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_6_01_04;    
	if(KeyState.KeyValue == ENTER)
	{
		MenuDisplay = SYS_6_01_03_05;
		SYS_password_clr();
	}
	if(RefreshFlag){
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[1][0]));
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[5][0]), 4))) ;
}
}
void SYS_6_01_04(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_6_01;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_6_01_03;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_6_01_00;    
	else if(KeyState.KeyValue == ENTER)
	{
		MenuDisplay = SYS_6_01_04_05;
		SYS_password_clr();
	}
	if(RefreshFlag){
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[1][0]));
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[5][0]), 5))) ;
}
}
void SYS_6_01_00_05(void)
{
	if(KeyState.KeyValue == ESC)
	{
		MenuDisplay = SYS_6_01_00;
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
		password[pass_pos]++;
		if(9 < password[pass_pos])password[pass_pos]=0;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == DN)
	{
		if(password[pass_pos])password[pass_pos]--;
		else password[pass_pos] = 9;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == ENTER)
	{
		Temporary = ((unsigned int)password[3]*1000) + ((unsigned int)password[2]*100) + ((unsigned int)password[1]*10) + (unsigned int)password[0];    
		if(DATA_Registers[2491] == Temporary )
		{
			MenuDisplay = SYS_6_01_00_06;
			AccessLevel = 1;
		}
		else
		{
			SYS_password_clr();
		}
	}
	if(RefreshFlag) {
		CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[5][0]), 1))) ;
		CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[3][0]),password[0],password[1],password[2],password[3]))) ;

		SYS_cursor_ON_Mode(CURSOR_MODE_0);
	}
}
void SYS_6_01_00_06(void)
{
	if(KeyState.KeyValue == ESC)
	{
		MenuDisplay = SYS_6_01_00;
		SYS_password_clr();
	}
	if(RefreshFlag){
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[5][0]), 1))) ;
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[6][0]), 1))) ;
	CLCD_cursor_OFF();
}
}
void SYS_6_01_01_05(void)
{
	if(KeyState.KeyValue == ESC)
	{
		MenuDisplay = SYS_6_01_01;
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
		password[pass_pos]++;
		if(9 < password[pass_pos])password[pass_pos]=0;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == DN)
	{
		if(password[pass_pos])password[pass_pos]--;
		else password[pass_pos] = 9;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == ENTER)
	{
		Temporary = ((unsigned int)password[3]*1000) + ((unsigned int)password[2]*100) + ((unsigned int)password[1]*10) + (unsigned int)password[0];    
		if(DATA_Registers[2492] == Temporary )
		{
			MenuDisplay = SYS_6_01_01_06;
			AccessLevel = 2;
		}
		else
		{
			SYS_password_clr();
		}
		
	}
	if(RefreshFlag){
		CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[5][0]), 1))) ;
		CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[3][0]),password[0],password[1],password[2],password[3]))) ;

		SYS_cursor_ON_Mode(CURSOR_MODE_0);
	}
}
void SYS_6_01_01_06(void)
{
	if(KeyState.KeyValue == ESC)
	{
		MenuDisplay = SYS_6_01_01;
		SYS_password_clr();
	}
	if(RefreshFlag){
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[5][0]), 2))) ;
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[6][0]), 2))) ;
	CLCD_cursor_OFF();
}
}
void SYS_6_01_02_05(void)
{
	if(KeyState.KeyValue == ESC)
	{
		MenuDisplay = SYS_6_01_02;
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
		password[pass_pos]++;
		if(9 < password[pass_pos])password[pass_pos]=0;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == DN)
	{
		if(password[pass_pos])password[pass_pos]--;
		else password[pass_pos] = 9;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == ENTER)
	{
		Temporary = ((unsigned int)password[3]*1000) + ((unsigned int)password[2]*100) + ((unsigned int)password[1]*10) + (unsigned int)password[0];    
		if(DATA_Registers[2493] == Temporary )
		{
			MenuDisplay =SYS_6_01_02_06;
			AccessLevel = 3;
		}
		else
		{
			SYS_password_clr();
		}
		
	}
	if(RefreshFlag){
		CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[5][0]), 3))) ;
		CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[3][0]),password[0],password[1],password[2],password[3]))) ;

		SYS_cursor_ON_Mode(CURSOR_MODE_0);
	}
}
void SYS_6_01_02_06(void)
{
	if(KeyState.KeyValue == ESC)
	{
		MenuDisplay = SYS_6_01_02;
		SYS_password_clr();
	}
	if(RefreshFlag) {
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[5][0]), 3))) ;
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[6][0]), 3))) ;
	CLCD_cursor_OFF();
}
}
void SYS_6_01_03_05(void)
{
	if(KeyState.KeyValue == ESC)
	{
		MenuDisplay =SYS_6_01_03;
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
		password[pass_pos]++;
		if(9 < password[pass_pos])password[pass_pos]=0;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == DN)
	{
		if(password[pass_pos])password[pass_pos]--;
		else password[pass_pos] = 9;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == ENTER)
	{
		Temporary = ((unsigned int)password[3]*1000) + ((unsigned int)password[2]*100) + ((unsigned int)password[1]*10) + (unsigned int)password[0];    
		if(DATA_Registers[2494] == Temporary )
		{
			MenuDisplay = SYS_6_01_03_06;
			AccessLevel = 4;
		}
		else
		{
			SYS_password_clr();
		}
	} 
	if(RefreshFlag){
		CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[5][0]), 4))) ;
		CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[3][0]),password[0],password[1],password[2],password[3]))) ;

		SYS_cursor_ON_Mode(CURSOR_MODE_0);
	}
}
void SYS_6_01_03_06(void)
{
	if(KeyState.KeyValue == ESC)
	{
		MenuDisplay = SYS_6_01_03;
		SYS_password_clr();
	}
	if(RefreshFlag) {
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[5][0]), 4))) ;
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[6][0]), 4))) ;
	CLCD_cursor_OFF();
}
}
void SYS_6_01_04_05(void)
{
	if(KeyState.KeyValue == ESC)
	{
		MenuDisplay = SYS_6_01_04;
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
		password[pass_pos]++;
		if(9 < password[pass_pos])password[pass_pos]=0;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == DN)
	{
		if(password[pass_pos])password[pass_pos]--;
		else password[pass_pos] = 9;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == ENTER)
	{
		Temporary = ((unsigned int)password[3]*1000) + ((unsigned int)password[2]*100) + ((unsigned int)password[1]*10) + (unsigned int)password[0];    
		if(DATA_Registers[2495] == Temporary )
		{
			MenuDisplay = SYS_6_01_04_06;
			AccessLevel = 5;
		}
		else
		{
			SYS_password_clr();
		}
		
	}
	if(RefreshFlag){
		CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[5][0]), 5))) ;
		CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[3][0]),password[0],password[1],password[2],password[3]))) ;

		SYS_cursor_ON_Mode(CURSOR_MODE_0);
	}
}
void SYS_6_01_04_06(void)
{
	if(KeyState.KeyValue == ESC)
	{
		MenuDisplay = SYS_6_01_04_06;
		SYS_password_clr();
	}
	if(RefreshFlag){
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[5][0]), 5))) ;
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[6][0]), 5))) ;
	CLCD_cursor_OFF();
}
}



void SYS_6_02_00(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_6_02;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_6_02_04;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_6_02_01;    
	if(KeyState.KeyValue == ENTER)
	{
		MenuDisplay = SYS_6_02_00_05;
		SYS_password_clr();
	}
	if(RefreshFlag){
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[2][0]));
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[5][0]), 1))) ;
}
}
void SYS_6_02_01(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_6_02;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_6_02_00;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_6_02_02;    
	if(KeyState.KeyValue == ENTER)
	{
		MenuDisplay = SYS_6_02_01_05;
		SYS_password_clr();
	}
	if(RefreshFlag){
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[2][0]));
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[5][0]), 2))) ;
}
}
void SYS_6_02_02(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_6_02;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_6_02_01;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_6_02_03;    
	if(KeyState.KeyValue == ENTER)
	{
		MenuDisplay = SYS_6_02_02_05;
		SYS_password_clr();
	}
	if(RefreshFlag){
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[2][0]));
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[5][0]), 3))) ;
}
}
void SYS_6_02_03(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_6_02;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_6_02_02;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_6_02_04;    
	if(KeyState.KeyValue == ENTER)
	{
		MenuDisplay = SYS_6_02_03_05;
		SYS_password_clr();
	}
	if(RefreshFlag) {
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[2][0]));
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[5][0]), 4))) ;
}
}
void SYS_6_02_04(void)
{
	if(KeyState.KeyValue == ESC)MenuDisplay = SYS_6_02;
	else if(KeyState.KeyValue == UP)MenuDisplay = SYS_6_02_03;
	else if(KeyState.KeyValue == DN)MenuDisplay = SYS_6_02_00;    
	if(KeyState.KeyValue == ENTER)
	{
		MenuDisplay = SYS_6_02_04_05;
		SYS_password_clr();
	}
	if(RefreshFlag){
	CLCD_string(0x80,(char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[2][0]));
	CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[5][0]), 5))) ;
}
}


void SYS_6_02_00_05(void)
{
	if(KeyState.KeyValue == ESC)
	{
		MenuDisplay = SYS_6_02_00;
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
		password[pass_pos]++;
		if(9 < password[pass_pos])password[pass_pos]=0;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == DN)
	{
		if(password[pass_pos])password[pass_pos]--;
		else password[pass_pos] = 9;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == ENTER)
	{
		Temporary = ((unsigned int)password[3]*1000) + ((unsigned int)password[2]*100) + ((unsigned int)password[1]*10) + (unsigned int)password[0];    
		if(DATA_Registers[2491] == Temporary )
		{
			MenuDisplay = SYS_6_02_00_06;
		}
		else
		{
			SYS_password_clr();
		}
		
	}

	if(RefreshFlag) {
		CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[5][0]), 1))) ;
		CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[3][0]),password[0],password[1],password[2],password[3]))) ;

		SYS_cursor_ON_Mode(CURSOR_MODE_0);
	}
}
void SYS_6_02_00_06(void)
{
	if(KeyState.KeyValue == ESC)
	{
		MenuDisplay = SYS_6_02_00;
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
		password[pass_pos]++;
		if(9 < password[pass_pos])password[pass_pos]=0;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == DN)
	{
		if(password[pass_pos])password[pass_pos]--;
		else password[pass_pos] = 9;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == ENTER)
	{
		MenuDisplay = SYS_6_02_00_07;
		DATA_Registers[2491] = ((unsigned int)password[3]*1000) + ((unsigned int)password[2]*100) + ((unsigned int)password[1]*10) + (unsigned int)password[0]; 
	}
	if(RefreshFlag){
		CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[7][0]), 1))) ;
		CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[3][0]),password[0],password[1],password[2],password[3]))) ;

		SYS_cursor_ON_Mode(CURSOR_MODE_0);
	}
}
void SYS_6_02_00_07(void)
{
	if(KeyState.KeyValue == ESC)
	{
		MenuDisplay = SYS_6_02_00;
		SYS_password_clr();
	}
	if(RefreshFlag){
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[7][0]), 1))) ;
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[4][0]));
	CLCD_cursor_OFF();
}
}
void SYS_6_02_01_05(void)
{
	if(KeyState.KeyValue == ESC)
	{
		MenuDisplay = SYS_6_02_01;
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
		password[pass_pos]++;
		if(9 < password[pass_pos])password[pass_pos]=0;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == DN)
	{
		if(password[pass_pos])password[pass_pos]--;
		else password[pass_pos] = 9;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == ENTER)
	{
		Temporary = ((unsigned int)password[3]*1000) + ((unsigned int)password[2]*100) + ((unsigned int)password[1]*10) + (unsigned int)password[0];    
		if(DATA_Registers[2492] == Temporary )
		{
			MenuDisplay =SYS_6_02_01_06;
		}
		else
		{
			SYS_password_clr();
		}
		
	}
	if(RefreshFlag){
		CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[5][0]), 2))) ;
		CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[3][0]),password[0],password[1],password[2],password[3]))) ;

		SYS_cursor_ON_Mode(CURSOR_MODE_0);
	}
}
void SYS_6_02_01_06(void)
{
	if(KeyState.KeyValue == ESC)
	{
		MenuDisplay = SYS_6_02_01;
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
		password[pass_pos]++;
		if(9 < password[pass_pos])password[pass_pos]=0;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == DN)
	{
		if(password[pass_pos])password[pass_pos]--;
		else password[pass_pos] = 9;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == ENTER)
	{
		MenuDisplay = SYS_6_02_01_07;
		DATA_Registers[2492] = ((unsigned int)password[3]*1000) + ((unsigned int)password[2]*100) + ((unsigned int)password[1]*10) + (unsigned int)password[0]; 
	}
	if(RefreshFlag){
		CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[7][0]), 2))) ;
		CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[3][0]),password[0],password[1],password[2],password[3]))) ;

		SYS_cursor_ON_Mode(CURSOR_MODE_0);
	}
}
void SYS_6_02_01_07(void)
{
	if(KeyState.KeyValue == ESC)
	{
		MenuDisplay = SYS_6_02_01;
		SYS_password_clr();
	}
	if(RefreshFlag){
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[7][0]), 2))) ;
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[4][0]));
	CLCD_cursor_OFF();
}
}
void SYS_6_02_02_05(void)
{
	if(KeyState.KeyValue == ESC)
	{
		MenuDisplay = SYS_6_02_02;
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
		password[pass_pos]++;
		if(9 < password[pass_pos])password[pass_pos]=0;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == DN)
	{
		if(password[pass_pos])password[pass_pos]--;
		else password[pass_pos] = 9;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == ENTER)
	{
		Temporary = ((unsigned int)password[3]*1000) + ((unsigned int)password[2]*100) + ((unsigned int)password[1]*10) + (unsigned int)password[0];    
		if(DATA_Registers[2493] == Temporary )
		{
			MenuDisplay =SYS_6_02_02_06;
		}
		else
		{
			SYS_password_clr();
		}
		
	}
	if(RefreshFlag){
		CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[5][0]), 3))) ;
		CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[3][0]),password[0],password[1],password[2],password[3]))) ;

		SYS_cursor_ON_Mode(CURSOR_MODE_0);
	}
}
void SYS_6_02_02_06(void)
{
	if(KeyState.KeyValue == ESC)
	{
		MenuDisplay = SYS_6_02_02;
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
		password[pass_pos]++;
		if(9 < password[pass_pos])password[pass_pos]=0;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == DN)
	{
		if(password[pass_pos])password[pass_pos]--;
		else password[pass_pos] = 9;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == ENTER)
	{
		MenuDisplay = SYS_6_02_02_07;
		DATA_Registers[2493] = ((unsigned int)password[3]*1000) + ((unsigned int)password[2]*100) + ((unsigned int)password[1]*10) + (unsigned int)password[0]; 
	}
	if(RefreshFlag){
		CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[7][0]), 3))) ;
		CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[3][0]),password[0],password[1],password[2],password[3]))) ;

		SYS_cursor_ON_Mode(CURSOR_MODE_0);
	}
}
void SYS_6_02_02_07(void)
{
	if(KeyState.KeyValue == ESC)
	{
		MenuDisplay = SYS_6_02_02;
		SYS_password_clr();
	}
	if(RefreshFlag){
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[7][0]), 3))) ;
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[4][0]));
	CLCD_cursor_OFF();
}
}
void SYS_6_02_03_05(void)
{
	if(KeyState.KeyValue == ESC)
	{
		MenuDisplay = SYS_6_02_03;
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
		password[pass_pos]++;
		if(9 < password[pass_pos])password[pass_pos]=0;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == DN)
	{
		if(password[pass_pos])password[pass_pos]--;
		else password[pass_pos] = 9;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == ENTER)
	{
		Temporary = ((unsigned int)password[3]*1000) + ((unsigned int)password[2]*100) + ((unsigned int)password[1]*10) + (unsigned int)password[0];    
		if(DATA_Registers[2494] == Temporary )
		{
			MenuDisplay = SYS_6_02_03_06;
		}
		else
		{
			SYS_password_clr();
		}
		
	}
	if(RefreshFlag){
		CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[5][0]), 4))) ;
		CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[3][0]),password[0],password[1],password[2],password[3]))) ;

		SYS_cursor_ON_Mode(CURSOR_MODE_0);
	}
}
void SYS_6_02_03_06(void)
{
	if(KeyState.KeyValue == ESC)
	{
		MenuDisplay = SYS_6_02_03;
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
		password[pass_pos]++;
		if(9 < password[pass_pos])password[pass_pos]=0;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == DN)
	{
		if(password[pass_pos])password[pass_pos]--;
		else password[pass_pos] = 9;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == ENTER)
	{
		MenuDisplay = SYS_6_02_03_07;
		DATA_Registers[2494] = ((unsigned int)password[3]*1000) + ((unsigned int)password[2]*100) + ((unsigned int)password[1]*10) + (unsigned int)password[0]; 
	}
	if(RefreshFlag){
		CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[7][0]), 4))) ;
		CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[3][0]),password[0],password[1],password[2],password[3]))) ;

		SYS_cursor_ON_Mode(CURSOR_MODE_0);
	}
}
void SYS_6_02_03_07(void)
{
	if(KeyState.KeyValue == ESC)
	{
		MenuDisplay = SYS_6_02_03_07;
		SYS_password_clr();
	}
	if(RefreshFlag){
	CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[7][0]), 4))) ;
	CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[4][0]));
	CLCD_cursor_OFF();
}
}
void SYS_6_02_04_05(void)
{
	if(KeyState.KeyValue == ESC)
	{
		MenuDisplay = SYS_6_02_04;
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
		password[pass_pos]++;
		if(9 < password[pass_pos])password[pass_pos]=0;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == DN)
	{
		if(password[pass_pos])password[pass_pos]--;
		else password[pass_pos] = 9;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == ENTER)
	{
		Temporary = ((unsigned int)password[3]*1000) + ((unsigned int)password[2]*100) + ((unsigned int)password[1]*10) + (unsigned int)password[0];    
		if(DATA_Registers[2495] == Temporary )
		{
			MenuDisplay = SYS_6_02_04_06;
		}
		else
		{
			SYS_password_clr();
		}
		
	}
	if(RefreshFlag){
		CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[5][0]), 5))) ;
		CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[3][0]),password[0],password[1],password[2],password[3]))) ;

		SYS_cursor_ON_Mode(CURSOR_MODE_0);
	}
}
void SYS_6_02_04_06(void)
{
	if(KeyState.KeyValue == ESC)
	{
		MenuDisplay = SYS_6_02_04;
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
		password[pass_pos]++;
		if(9 < password[pass_pos])password[pass_pos]=0;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == DN)
	{
		if(password[pass_pos])password[pass_pos]--;
		else password[pass_pos] = 9;
	
		RefreshFlag=1;
	}
	else if(KeyState.KeyValue == ENTER)
	{
		MenuDisplay = SYS_6_02_04_07;
		DATA_Registers[2495] = ((unsigned int)password[3]*1000) + ((unsigned int)password[2]*100) + ((unsigned int)password[1]*10) + (unsigned int)password[0]; 
	}
	if(RefreshFlag){
		CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[7][0]), 5))) ;
		CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[3][0]),password[0],password[1],password[2],password[3]))) ;

		SYS_cursor_ON_Mode(CURSOR_MODE_0);
	}
}
void SYS_6_02_04_07(void)
{
	if(KeyState.KeyValue == ESC)
	{
		MenuDisplay = SYS_6_02_04;
		SYS_password_clr();
	}
	if(RefreshFlag){
		CLCD_string(0x80,((char*)_TEXT((char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[7][0]), 5))) ;
		CLCD_string(0xC0,(char*)_cpy_flash2memory(&PAGE_DIR_6_XX_XX[4][0]));
		CLCD_cursor_OFF();
	}
}







void SYS_0xFFFFFFFF(void)
{
	if(KeyState.KeyValue ==  ESC )  MenuDisplay = SYS_0;
	if(RefreshFlag){
	//GLCD_BufClear();  
	//GLCD_print0508(2, 1,"for The GOD");
	//GLCD_print0508(2, 2,"for The JESUS");
	//GLCD_print0508(2, 3,"for The PEOPLE");

	CLCD_string(0x80,"        Designed");
	CLCD_string(0xC0," by Paul BH Park");

}
}


void SYS_password_clr(void)
{
	password[0]=0;
	password[1]=0;
	password[2]=0;
	password[3]=0;
	pass_pos = 0;
}
 



//const char* _cpy_flash2memory(char const *format)
const char* _cpy_flash2memory(char __flash *format)
{
	char* string = text_buf;
	//memset(target, 0x00, sizeof(target)); 
	while(1)
	{
		//*string =  pgm_read_byte(format);
		*string =  *format;
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
	unsigned char markminus=0;
	unsigned char islong=0;
	unsigned long u_val=0;
	long s_val=0;

	unsigned char fill;
	unsigned char width;
	unsigned char text_cnt;
	unsigned char char_flag=0;

char* string = text_buf;


	va_start (ap, format);
	for (;;)
	{
		while ((format_flag = *(format++)) != '%')   // Until '%' or '\0' 
		{   
			if (!format_flag)
			{
				va_end (ap);
				//*string = 0x00;
				//string = &text_buf[0];
				//return string;
															 goto END_FUNC;
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
		
		if(format_flag=='0' || format_flag==' ' || format_flag=='.'|| format_flag=='_') //SPACE or ZERO padding  ?
		{
			fill=format_flag;
			format_flag = *format++; //get char after padding char
			if(format_flag>='0' && format_flag<='9')
			{
				width=format_flag-'0';
				format_flag = *format++; //get char after width char
				if(format_flag>='0' && format_flag<='9')
				{
					width = width * 10 + (format_flag-'0');
					format_flag = *format++; //get char after width char
				}
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
				*string = format_flag;
				string ++;
				char_flag = 1;
				// no break -> run into default
			default:
				//________________________________________________
				if(!char_flag)
				{
				 *string = '%';
				 string ++;
				}

				//________________________________________________

			continue;
		/*  case 'f':
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
						markminus = 1;
						s_val = - s_val; //Make it positiv
						//________________________________________________
						//string--;

						//*string = '-';
						//string ++;
						//________________________________________________

					}
					else
					{
						if(markpluse)
						{
							//________________________________________________
							string--;
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

				if(markminus)
				{
					 *--ptr = '-';
					 ptr++;
				}
				

				while(*ptr) 
				{
					//________________________________________________
					*string = *ptr;
					string ++;
					ptr++;
					//________________________________________________
				}
				break;
				
			case 'b':
				if(islong) { u_val = va_arg(ap,unsigned long); }
				else { u_val = va_arg(ap,unsigned int); }

				ptr = scratch + TEXT_SCRATCH;
				*--ptr = 0;
				
				text_cnt = 0;
				while(width--)
				{
					if((char)(u_val >> text_cnt)& 0x01)*--ptr = '1';
					else *--ptr = fill;
					text_cnt++;
				}

				while(*ptr) 
				{
					//________________________________________________
					*string = *ptr;
					string ++;
					ptr++;
					//________________________________________________
				}

				break;
				
		}
	}
	END_FUNC:      
	*string = 0x00;
	string = &text_buf[0];
	return string;
}

void MainSYSTEM(void)
{
	static unsigned char _EventFlagE=0;

	RefreshFlag=0;
	if(KeyState.KeyValue != 0xFF)
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

	if(KeyState.KeyValue == RUN)
	{
		DATA_Registers[2306] =  0x0001;
	}
	else if(KeyState.KeyValue == STOP)
	{
		DATA_Registers[2306] =  0x0000;
	}


	//local/remote
	if(DATA_Registers[2300])PORTL_Bit0 = 1;
	else PORTL_Bit0 = 0;

	if(DATA_Registers[2270])//run status
	{
		PORTL_Bit1 = 1;
		PORTL_Bit2 = 0;
	}
	else
	{
		PORTL_Bit1 = 0;
		PORTL_Bit2 = 1;
	}
//communication fault
#if 1
if(Communication_Fault_Flag)
{
	PORTL_Bit3 = 1;

	CLCD_string(0x80," * Comm Fault * ");
	CLCD_string(0xC0,"  Tx/Rx Failed  ");
	
	SCI_RegisterRefresh();
	MenuDisplay_Handler = MENU_STOP;
}
else
{
	PORTL_Bit3 = 0;
	MenuDisplay_Handler = MENU_RUN;
}
#endif
#if 1
//fault detection
	if(DATA_Registers[2379] != FaultDetectionHistory)
	{
		PORTL_Bit3 = 1;
		MenuDisplay_Handler = MENU_STOP;
		CLCD_string(0x80," * Drive Fault * ");
				 if(DATA_Registers[2376] & 0x8000) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[0][0])))) ;
		else if(DATA_Registers[2376] & 0x4000) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[1][0]))));
		else if(DATA_Registers[2376] & 0x2000) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[2][0]))));
		else if(DATA_Registers[2376] & 0x1000) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[3][0]))));
		else if(DATA_Registers[2376] & 0x0800) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[4][0]))));
		else if(DATA_Registers[2376] & 0x0400) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[5][0]))));
		else if(DATA_Registers[2376] & 0x0200) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[6][0]))));
		else if(DATA_Registers[2376] & 0x0100) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[7][0]))));
		else if(DATA_Registers[2376] & 0x0080) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[8][0]))));
		else if(DATA_Registers[2376] & 0x0040) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[9][0]))));
		else if(DATA_Registers[2376] & 0x0020) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[10][0]))));
		else if(DATA_Registers[2376] & 0x0010) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[11][0]))));
		else if(DATA_Registers[2376] & 0x0008) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[12][0]))));
		else if(DATA_Registers[2376] & 0x0004) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[13][0]))));
		else if(DATA_Registers[2376] & 0x0002) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[14][0]))));
		else if(DATA_Registers[2376] & 0x0001) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[15][0]))));
		else if(DATA_Registers[2377] & 0x8000) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[16][0]))));
		else if(DATA_Registers[2377] & 0x4000) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[17][0]))));
		else if(DATA_Registers[2377] & 0x2000) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[18][0]))));
		else if(DATA_Registers[2377] & 0x1000) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[19][0]))));
		else if(DATA_Registers[2377] & 0x0800) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[20][0]))));
		else if(DATA_Registers[2377] & 0x0400) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[21][0]))));
		else if(DATA_Registers[2377] & 0x0200) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[22][0]))));
		else if(DATA_Registers[2377] & 0x0100) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[23][0]))));
		else if(DATA_Registers[2377] & 0x0080) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[24][0]))));
		else if(DATA_Registers[2377] & 0x0040) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[25][0]))));
		else if(DATA_Registers[2377] & 0x0020) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[26][0]))));
		else if(DATA_Registers[2377] & 0x0010) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[27][0]))));
		else if(DATA_Registers[2377] & 0x0008) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[28][0]))));
		else if(DATA_Registers[2377] & 0x0004) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[29][0]))));
		else if(DATA_Registers[2377] & 0x0002) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[30][0]))));
		else if(DATA_Registers[2377] & 0x0001) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[31][0]))));
		else if(DATA_Registers[2378] & 0x8000) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[32][0]))));
		else if(DATA_Registers[2378] & 0x4000) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[33][0]))));
		else if(DATA_Registers[2378] & 0x2000) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[34][0]))));
		else if(DATA_Registers[2378] & 0x1000) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[35][0]))));
		else if(DATA_Registers[2378] & 0x0800) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[36][0]))));
		else if(DATA_Registers[2378] & 0x0400) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[37][0]))));
		else if(DATA_Registers[2378] & 0x0200) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[38][0]))));
		else if(DATA_Registers[2378] & 0x0100) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[39][0]))));
		else if(DATA_Registers[2378] & 0x0080) CLCD_string(0xC0,((char*)_TEXT((char*)_cpy_flash2memory(&FAULT_CODE[40][0]))));
		else
		{
			FaultDetectionHistory = (unsigned char)DATA_Registers[2379]; 
			MenuDisplay_Handler = MENU_RUN;
		}
		
		if(KeyState.KeyValue == ESC)
		{
			FaultDetectionHistory = (unsigned char)DATA_Registers[2379]; 
			MenuDisplay_Handler = MENU_RUN;
			DATA_Registers[2484] = 1;
		}

	}
	else
	{
		PORTL_Bit3 = 0;
	}
	
#endif

}

