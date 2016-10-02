/*****************************************************************************
*
* File Name: I2C_RTC.H
*
* Description: Header file for DS1307 I2C RTC driver
*
* Modules Included:
*
*****************************************************************************/


#ifndef __I2C_RTC_H
#define __I2C_RTC_H

#define RTC_REGS_CONTROL		0x07
#define RTC_REGS_YEAR			0x06
#define RTC_REGS_MONTH			0x05
#define RTC_REGS_DATE			0x04
#define RTC_REGS_WEEK			0x03
#define RTC_REGS_HOUR			0x02
#define RTC_REGS_MINUTE			0x01
#define RTC_REGS_SECOND			0x00

typedef struct {
    Uint16 year;
    Uint16 month;
    Uint16 date;
	Uint16 week;
    Uint16 hour;
    Uint16 minute;
    Uint16 second;
} TIME;

void Change_I2C_RTC(void);
void Recover_I2C_EEPROM(void);
void Write_RTC_Data(Uint16 uAddr, Uint16 uData);
void Read_RTC_Data(Uint16 uAddr, Uint16 *pData);
void Set_InitialTime(void);
void Write_Time(TIME time);
void Read_Time(TIME *time);

extern TIME time;

#endif



