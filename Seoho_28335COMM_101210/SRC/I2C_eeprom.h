/*****************************************************************************
*
* File Name: I2C_EEPROM.H
*
* Description: Header file for 24LC512 I2C eeprom driver
*
* Modules Included:
*
*****************************************************************************/


#ifndef __I2C_EEPROM_H
#define __I2C_EEPROM_H

/* micro-seconds delay function */
// TI SDK 1.10의 소스 DSP2833x_usDelay.asm에서 제공하는 DELAY_US 함수를 사용
#define delay_us(us)		DELAY_US(us)	 

/* milli-seconds delay function */
// TI SDK 1.10의 소스 DSP2833x_usDelay.asm에서 제공하는 DELAY_US 함수를 사용
#define delay_ms(ms)		DELAY_US(ms*1000)

#define	nWP_ENABLE			(GpioDataRegs.GPBSET.bit.GPIO39 = 1)		// WP = 1, Write protected
#define nWP_DISABLE         (GpioDataRegs.GPBCLEAR.bit.GPIO39 = 1)  // WP = 0, Write unprotected


void Init_I2C_eeprom(void);
void Write_EEPROM_Data(Uint16 uCSBit, Uint16 uAddr, Uint16 uData);
void Read_EEPROM_Data(Uint16 uCSBit, Uint16 uAddr, Uint16 *pData);

#endif



