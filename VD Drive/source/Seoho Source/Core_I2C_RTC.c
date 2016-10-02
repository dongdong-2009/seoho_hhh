
//#include "Core_DSP2833x_Device.h"     // DSP2833x Headerfile Include File
//#include "Core_DSP2833x_Examples.h"   // Examples Include File
// #include "Core_DSK2833x_Define.h"

#include	<All_Header.h>
#include	<All_Extern_Variables.h>
#include "Core_I2C_RTC.h"


TIME time;

// Initialize I2C for serial RTC(DS1307) access
void Change_I2C_RTC(void)
{
	// Set Slave Address according to DS1307 device
	I2caRegs.I2CSAR = 0x0068;

	// Set I2C module clock input
//	I2caRegs.I2CPSC.all = 14; // need 7-12 Mhz on module clk (150/15 = 10MHz)
	
	// 100KHz clock speed in SCL for master mode(10us period)
	// Clock Pulse Width Low  : 4.9us
	// Clock Pulse Width High : 4.1us
	// F28335의 I2C User Guide에서 Master Clock 주기를 구하는 공식을 참조바람.
	I2caRegs.I2CCLKL = 49;
	I2caRegs.I2CCLKH = 41;
}

void Recover_I2C_EEPROM(void)
{
	// Set Slave Address according to 24LC512 device
	I2caRegs.I2CSAR = 0x0050;

	// Set I2C module clock input
//	I2caRegs.I2CPSC.all = 14; // need 7-12 Mhz on module clk (150/15 = 10MHz)
	
	// 400KHz clock speed in SCL for master mode(2.5us period)
	// Clock Pulse Width Low  : 1.5us
	// Clock Pulse Width High : 1us
	// F28335의 I2C User Guide에서 Master Clock 주기를 구하는 공식을 참조바람.
	I2caRegs.I2CCLKL = 15;
	I2caRegs.I2CCLKH = 10;
}

// refer to single byte write mode of DS1307
void Write_RTC_Data(Uint16 uAddr, Uint16 uData)
{
    Uint16 uSlaveAddr;

	uSlaveAddr = 0x68;

	// Wait until the STP bit is cleared from any previous master communication.
	while(I2caRegs.I2CMDR.bit.STP == 1);
	
	// Wait until bus-free status
	while(I2caRegs.I2CSTR.bit.BB == 1);
	
	// Set Device(Slave) Address
	I2caRegs.I2CSAR = uSlaveAddr;
	
	// Setup number of bytes to send
	I2caRegs.I2CCNT = 2;

	// Setup data to send
	I2caRegs.I2CDXR = uAddr & 0x0ff;        // Address
	I2caRegs.I2CDXR = uData;		        // Write Data
	
	// Send start as master transmitter with STT(=1), STP(=1), XA(=0), RM(=0) 
	I2caRegs.I2CMDR.all = 0x6E20;
	
	// Wait until STOP condition is detected and clear STOP condition bit
	while(I2caRegs.I2CSTR.bit.SCD == 0);
	I2caRegs.I2CSTR.bit.SCD = 1;

	// Wait the Write-Cycle Time for DS1307
	// refer to DS1307 datasheet
	delay_ms(1);
}

// refer to single byte random read mode of DS1307
void Read_RTC_Data(Uint16 uAddr, Uint16 *pData)
{
    Uint16 uSlaveAddr;
	
	uSlaveAddr = 0x68;

	// Wait until the STP bit is cleared from any previous master communication.
	while(I2caRegs.I2CMDR.bit.STP == 1);
	
	// Wait until bus-free status
	while(I2caRegs.I2CSTR.bit.BB == 1);
	
	// Set Device(Slave) Address
	I2caRegs.I2CSAR = uSlaveAddr;
	
	// Setup number of bytes to send
	I2caRegs.I2CCNT = 1;

	// Setup data to send
	I2caRegs.I2CDXR = uAddr & 0x0ff;        // Address

	// Send start as master transmitter with FREE(=1), STT(=1), STP(=0), XA(=0), RM(=0) 
	I2caRegs.I2CMDR.all = 0x6620;
	
	// Wait until ARDY status bit is set
	while(I2caRegs.I2CSTR.bit.ARDY == 0);
	
	// Wait until the STP bit is cleared
	while(I2caRegs.I2CMDR.bit.STP == 1);
	
	// Set Device(Slave) Address
	I2caRegs.I2CSAR = uSlaveAddr;
	
	// Setup number of bytes to read
	I2caRegs.I2CCNT = 1;
	
	// Send start as master receiver with FREE(=1), STT(=1), STP(=1), XA(=0), RM(=0) 
	I2caRegs.I2CMDR.all = 0x6C20;
	
	// Wait until STOP condition is detected and clear STOP condition bit
	while(I2caRegs.I2CSTR.bit.SCD == 0);
	I2caRegs.I2CSTR.bit.SCD = 1;
	
	*pData = (BYTE)(I2caRegs.I2CDRR & 0xff);  
}

void Set_InitialTime(void)
{
    Change_I2C_RTC();                   // Change I2C master clock frequency for DS1307

	Write_RTC_Data(RTC_REGS_SECOND, 0x00);			// CH bit=0(enable oscillator), second
	Write_RTC_Data(RTC_REGS_MINUTE, 0x46);			// minute
	Write_RTC_Data(RTC_REGS_HOUR, 0x08);           // hour
	Write_RTC_Data(RTC_REGS_WEEK, 0x04);           // week
	Write_RTC_Data(RTC_REGS_DATE, 0x09);           // date
	Write_RTC_Data(RTC_REGS_MONTH, 0x02);           // month
	Write_RTC_Data(RTC_REGS_YEAR, 0x11);           // year
	Write_RTC_Data(RTC_REGS_CONTROL, 0x00);           // control

	Recover_I2C_EEPROM();              // Recover I2C master clock frequency for 24LC512
} 

void Write_Time(TIME time)
{
    Change_I2C_RTC();                   // Change I2C master clock frequency for DS1307

	Write_RTC_Data(RTC_REGS_SECOND, time.second);
	Write_RTC_Data(RTC_REGS_MINUTE, time.minute);
    Write_RTC_Data(RTC_REGS_HOUR, time.hour);
	Write_RTC_Data(RTC_REGS_WEEK, time.week);
	Write_RTC_Data(RTC_REGS_DATE, time.date);
	Write_RTC_Data(RTC_REGS_MONTH, time.month);
	Write_RTC_Data(RTC_REGS_YEAR, time.year);

	Recover_I2C_EEPROM();              // Recover I2C master clock frequency for 24LC512
}

void Read_Time(TIME *time)
{
    Change_I2C_RTC();                   // Change I2C master clock frequency for DS1307

    Read_RTC_Data(RTC_REGS_YEAR, &time->year);
	Read_RTC_Data(RTC_REGS_MONTH, &time->month);
	Read_RTC_Data(RTC_REGS_DATE, &time->date);
	Read_RTC_Data(RTC_REGS_WEEK, &time->week);
	Read_RTC_Data(RTC_REGS_HOUR, &time->hour);
	Read_RTC_Data(RTC_REGS_MINUTE, &time->minute);
	Read_RTC_Data(RTC_REGS_SECOND, &time->second);

	Recover_I2C_EEPROM();              // Recover I2C master clock frequency for 24LC512
}












