
#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // Examples Include File
#include "DSK2833x_Define.h"
#include "I2C_eeprom.h"

// Initialize I2C for serial eeprom(24LC512) access
void Init_I2C_eeprom(void)
{
	// Setting GPIO for I2C
	EALLOW;
	/* Enable internal pull-up for the selected pins */
	GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;    // Enable pull-up for GPIO32 (SDAA)
	GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;	   // Enable pull-up for GPIO33 (SCLA)
	
	/* Set qualification for selected pins to asynch only */
	GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3;  // Asynch input GPIO32 (SDAA)
	GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3;  // Asynch input GPIO33 (SCLA)
	
	/* Configure I2C pins using GPIO regs*/
	// This specifies which of the possible GPIO pins will be I2C functional pins.
	GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 1;   // Configure GPIO32 for SDAA operation
	GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 1;   // Configure GPIO33 for SCLA operation
	EDIS;
	
	// Initialize I2C interface
	// Put I2C module in the reset state
	I2caRegs.I2CMDR.bit.IRS = 0;
	
	// Set Slave Address according to AT24C16 device
	I2caRegs.I2CSAR = 0x0050;

	// Set I2C module clock input
	I2caRegs.I2CPSC.all = 14; // need 7-12 Mhz on module clk (150/15 = 10MHz)
	
	// 400KHz clock speed in SCL for master mode(2.5us period)
	// Clock Pulse Width Low  : 1.5us
	// Clock Pulse Width High : 1.0us
	// F28335�� I2C User Guide���� Master Clock �ֱ⸦ ���ϴ� ������ �����ٶ�.
	I2caRegs.I2CCLKL = 10;
	I2caRegs.I2CCLKH = 5;
	
	// Disable I2C interrupt
	I2caRegs.I2CIER.all = 0x0;
	
	// Enable TX and RX FIFO in I2C module
	I2caRegs.I2CFFTX.all = 0x6000; // Enable FIFO mode and TXFIFO
	I2caRegs.I2CFFRX.all = 0x2040; // Enable RXFIFO, clear RXFFINT

	// Enable I2C module
	I2caRegs.I2CMDR.all = 0x20; 
}

// refer to single byte write mode of 24LC512
void Write_EEPROM_Data(Uint16 uCSBit, Uint16 uAddr, Uint16 uData)
{
    Uint16 uSlaveAddr;

	nWP_DISABLE;							// WP = 0; 

	uSlaveAddr = 0x50 | (uCSBit & 0x07);

	// Wait until the STP bit is cleared from any previous master communication.
	while(I2caRegs.I2CMDR.bit.STP == 1);
	
	// Wait until bus-free status
	while(I2caRegs.I2CSTR.bit.BB == 1);
	
	// Set Device(Slave) Address
	I2caRegs.I2CSAR = uSlaveAddr;
	
	// Setup number of bytes to send
	I2caRegs.I2CCNT = 3;

	// Setup data to send
	I2caRegs.I2CDXR = (uAddr >> 8) & 0x0ff; // Address High Byte
	I2caRegs.I2CDXR = uAddr & 0x0ff;        // Address Low Byte
	I2caRegs.I2CDXR = uData;		        // Write Data
	
	// Send start as master transmitter with STT(=1), STP(=1), XA(=0), RM(=0) 
	I2caRegs.I2CMDR.all = 0x6E20;
	
	// Wait until STOP condition is detected and clear STOP condition bit
	while(I2caRegs.I2CSTR.bit.SCD == 0);
	I2caRegs.I2CSTR.bit.SCD = 1;

	// Wait the Write-Cycle Time for EEPROM
	// refer to 24LC512 datasheet
	delay_ms(5);

	nWP_ENABLE;
}

// refer to single byte random read mode of 24LC512
void Read_EEPROM_Data(Uint16 uCSBit, Uint16 uAddr, Uint16 *pData)
{
    Uint16 uSlaveAddr;
	
	uSlaveAddr = 0x50 | (uCSBit & 0x07);

	// Wait until the STP bit is cleared from any previous master communication.
	while(I2caRegs.I2CMDR.bit.STP == 1);
	
	// Wait until bus-free status
	while(I2caRegs.I2CSTR.bit.BB == 1);
	
	// Set Device(Slave) Address
	I2caRegs.I2CSAR = uSlaveAddr;
	
	// Setup number of bytes to send
	I2caRegs.I2CCNT = 2;

	// Setup data to send
	I2caRegs.I2CDXR = (uAddr >> 8) & 0x0ff; // Address High Byte
	I2caRegs.I2CDXR = uAddr & 0x0ff;        // Address Low Byte

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

void Word_Write_data(Uint16 uAddr, Uint16 uData)
{
	Uint16 lo, hi;

    lo = uData & 0xff;
	hi = (uData >> 8);
	Write_EEPROM_Data(0, 2 * uAddr, lo);
	Write_EEPROM_Data(0, 2 * uAddr+1, hi);
} 

void Word_Read_data(Uint16 uAddr, Uint16 *pData)
{
	Uint16 lo, hi;

	Read_EEPROM_Data(0, 2 * uAddr, &lo);
	Read_EEPROM_Data(0, 2 * uAddr+1, &hi);
    *pData = lo + (hi << 8);
}
