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
	 

#define	nWP_ENABLE			(GpioDataRegs.GPBSET.bit.GPIO39 = 1)		// WP = 1, Write protected
#define nWP_DISABLE         (GpioDataRegs.GPBCLEAR.bit.GPIO39 = 1)  // WP = 0, Write unprotected


void Init_I2C_eeprom(void);
void Write_EEPROM_Data(Uint16 uCSBit, Uint16 uAddr, Uint16 uData);
void Read_EEPROM_Data(Uint16 uCSBit, Uint16 uAddr, Uint16 *pData);

#endif



