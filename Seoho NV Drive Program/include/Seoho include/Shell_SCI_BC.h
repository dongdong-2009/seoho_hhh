/*****************************************************************************
*
* File Name: SCI_BC.H
*
* Description: Header file for SCI_B, SCI_C driver
*
* Modules Included:
*
*****************************************************************************/


#ifndef __SCI_BC_H
#define __SCI_BC_H

#include "Core_DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "Core_DSP2833x_Examples.h"   // DSP2833x Examples Include File
#include "Shell_Bit_Define.h"


typedef	union
{
	unsigned Word;
	struct
	{
		unsigned b0	:8;	// CRC16 하위 바이트
		unsigned b1	:8;	// CRC16 상위 바이트
	} Byte;
} CRC_flg ;

// SCI-B, SCI-C Interrupt Service Function 선언
#pragma CODE_SECTION(scib_tx_isr, "ramfuncs");
#pragma CODE_SECTION(scib_rx_isr, "ramfuncs");
#pragma CODE_SECTION(scic_tx_isr, "ramfuncs");
#pragma CODE_SECTION(scic_rx_isr, "ramfuncs");

#define	CPUCLK			150000000L							// CPU Main Clock
#define	SCIB_LSPCLK		(CPUCLK/4)							// Peripheral Low Speed Clock for SCI-B
#define	SCIB_BAUDRATE	115200L								// SCI-B Baudrate
#define	SCIB_BRR_VAL	(SCIB_LSPCLK/(8*SCIB_BAUDRATE)-1)	// SCI-B BaudRate 설정 Register 값

#define	SCIC_LSPCLK		(CPUCLK/4)							// Peripheral Low Speed Clock for SCI-C
#define	SCIC_BAUDRATE	115200L								// SCI-C Baudrate
//#define	SCIC_BAUDRATE	9600L								// SCI-C Baudrate
#define	SCIC_BRR_VAL	(SCIC_LSPCLK/(8*SCIC_BAUDRATE)-1)	// SCI-C BaudRate 설정 Register 값

/* BPS 에러율 *****************************************************************
*    BPS	   CPUCLK	  LSPCLK	         BRR_VAL	     BPS	error
*   4800	150000000	37500000	975.5625	976 	4797.851 	-0.045 
*   9600	150000000	37500000	487.28125	487 	9605.533 	0.058 
*  19200	150000000	37500000	243.140625	243 	19211.066 	0.058 
*  38400	150000000	37500000	121.0703125	121 	38422.131 	0.058 
*  57600	150000000	37500000	80.38020833	80 		57870.370 	0.469 
* 115200	150000000	37500000	39.69010417	40 		114329.268 	-0.756 
******************************************************************************/

#define	SCIB_BUF_SIZE	50


#define	SCIB_TX_START	{	if(ScibRegs.SCICTL2.bit.TXRDY){						\
								ScibRegs.SCICTL2.bit.TXINTENA=1;				\
								ScibRegs.SCITXBUF = scib_tx_buf[scib_tx_pos++];	\
								if(scib_tx_pos >= SCIB_BUF_SIZE) scib_tx_pos=0;	\
							}													\
							else ScibRegs.SCICTL2.bit.TXINTENA=1;				\
						}

#define	SCIB_TX_STOP	ScibRegs.SCICTL2.bit.TXINTENA=0

#define	SCIC_BUF_SIZE	50

#define	SCIC_TX_START	{	if(ScicRegs.SCICTL2.bit.TXRDY){						\
								ScicRegs.SCICTL2.bit.TXINTENA=1;				\
								ScicRegs.SCITXBUF = scic_tx_buf[scic_tx_pos++];	\
								if(scic_tx_pos >= SCIC_BUF_SIZE) scic_tx_pos=0;	\
							}													\
							else ScicRegs.SCICTL2.bit.TXINTENA=1;				\
					}

#define	SCIC_TX_STOP	ScicRegs.SCICTL2.bit.TXINTENA=0

//---------------------------------------------------------
#define	GEN_POLYNOMAL	0x8821		// CRC 젯수

#define QUERY		0x01
#define SEND		0x02
#define RESPONSE	0x03
#define REQUEST		0x04


void scib_init(void);
void scic_init(void);
void scib_TxChar(char c);
void scib_TxString(char *p);
void scic_TxChar(char c);
void scic_TxString(char *p);
void scib_tx_start(void);
void scic_tx_start(void);
void scib_putc(char d);
//void scic_putc(char d);
void scib_puts(char *p);
void scic_puts(char *p);
void sci_hex2(BYTE d);
char hex_to_asc(char hex);

interrupt void scib_tx_isr(void);
interrupt void scib_rx_isr(void);
interrupt void scic_tx_isr(void);
interrupt void scic_rx_isr(void);

void scic_test(void);
void CRC_16(unsigned char input);
#endif



