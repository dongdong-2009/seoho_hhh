//###############################################################################################################
// ���ϸ�   : SCI_B.c
// ȸ���   : RealSYS
// �ۼ���¥ : 2008.10.20
// �ۼ���   : R.N.K
// ����     : F28335 ������ ŰƮ�� DSP28335 DSK�� SCI-B ���� ���� �ڵ� ����
// 
// [ ����� ���� ȯ�� ]
// 1) TI CCS3.3�� Service Pack v11.0(CCS_v3.3_SR11_81.6.2.exe ����) - 2008.10.20
// 2) CCS3.3 Setup�� Target Driver Package(setupCCSPlatinum_v30329.exe ����) - 2008.10.20
// 3) C2000 Code Generation Tools v5.1.1 Release(C2000CodeGenerationTools5.1.1.exe ����) - 2008.10.20
//
// [ ������ Software Developer Kit(SDK) ]
// 1) TI�� SDK�� C/C++ Header Files and Peripheral Examples v1.20 ��� - 2008.10.20
//
// [ Revision History ]
//
//###############################################################################################################

#include "Core_DSP2833x_Device.h"     // Headerfile Include File
#include "Core_DSP2833x_Examples.h"   // Examples Include File

#include "fd.h"

// ������ ���������� ���ͷ�Ʈ ���� ��ƾ�� "ramfuncs" �̶�� ���ǿ� �Ҵ��ϰ�
// "ramfuncs" ������ ���� ���� ó���� ���� ���� RAM���� �����ϵ��� ��

// SCI-B Interrupt Service Function ����
#pragma CODE_SECTION(scib_tx_isr, "ramfuncs");
#pragma CODE_SECTION(scib_rx_isr, "ramfuncs");

interrupt void scib_tx_isr(void);
interrupt void scib_rx_isr(void);

#define	CPUCLK			150000000L							// CPU Main Clock
#define	SCIB_LSPCLK		(CPUCLK/4)							// Peripheral Low Speed Clock for SCI-B
#define	SCIB_BAUDRATE	57600L								// SCI-B Baudrate
#define	SCIB_BRR_VAL	(SCIB_LSPCLK/(8*SCIB_BAUDRATE)-1)	// SCI-B BaudRate ���� Register ��


/* BPS ������ *****************************************************************
*    BPS	   CPUCLK	  LSPCLK	         BRR_VAL	     BPS	error
*   4800	150000000	37500000	975.5625	976 	4797.851 	-0.045 
*   9600	150000000	37500000	487.28125	487 	9605.533 	0.058 
*  19200	150000000	37500000	243.140625	243 	19211.066 	0.058 
*  38400	150000000	37500000	121.0703125	121 	38422.131 	0.058 
*  57600	150000000	37500000	80.38020833	80 		57870.370 	0.469 
* 115200	150000000	37500000	39.69010417	40 		114329.268 	-0.756 
******************************************************************************/

#define	SCIB_BUF_SIZE	200

#define	SCIB_TX_START	{	if(ScibRegs.SCICTL2.bit.TXRDY){						\
								ScibRegs.SCICTL2.bit.TXINTENA=1;				\
								ScibRegs.SCITXBUF = scib_tx_buf[scib_tx_pos++];	\
								if(scib_tx_pos >= SCIB_BUF_SIZE) scib_tx_pos=0;	\
							}													\
							else ScibRegs.SCICTL2.bit.TXINTENA=1;				\
						}

#define	SCIB_TX_STOP	ScibRegs.SCICTL2.bit.TXINTENA=0

#define SD_RX_BUFFER_SIZE     100  ////PBHP_110901 ������ ������� ������Ŷ���� Ŀ����
//


/* Variables for Serial Communication  */
char scib_tx_buf[SCIB_BUF_SIZE+1];
char scib_tx_pos=0, scib_tx_end=0;
char scib_rx_buf[SCIB_BUF_SIZE+1];
char scib_rx_pos=0, scib_rx_end=0;
char scib_rxd=' ';

UINT8 abRxBuffer[ SD_RX_BUFFER_SIZE ];
typedef struct sd_DataType
{

   BOOL fEnabled;
   UINT16 iRxGet;
   UINT16 iRxPut;
   UINT16 iRxSize;
   
   UINT16 iTxGet;
   UINT16 iTxSize;

}sd_DataType;

sd_DataType sd_s;





/*******************************************************************************
**
** Private Globals
**
********************************************************************************
*/

sd_DataType sd_s;

void scib_tx_start(void)
{
	SCIB_TX_START;
}


void scib_TxChar(char c)
{
    while(!ScibRegs.SCICTL2.bit.TXRDY);
    ScibRegs.SCITXBUF=c;
}    


/*---------------------------------------------*/
/*      Transmmit Character                    */
/*---------------------------------------------*/
void scib_putc(char d)
{
	scib_tx_buf[scib_tx_end++] = d;
	if(scib_tx_end >= SCIB_BUF_SIZE) scib_tx_end = 0;
	SCIB_TX_START;
}


unsigned int cccc=0;
unsigned int kkkk=0;
BOOL SD_CharReceived( void )
{
   if( sd_s.iRxSize == 0 ) return FALSE;
   else return TRUE;
} /* end of SD_CharReceived() */

UCHAR SD_GetChar( void )
{

   UCHAR bReturnChar;

   if( sd_s.iRxSize == 0 )return 0;

   bReturnChar = abRxBuffer[ sd_s.iRxGet ];

   sd_s.iRxGet++;
   sd_s.iRxSize--;

   if( sd_s.iRxGet == SD_RX_BUFFER_SIZE ) sd_s.iRxGet = 0;

   return bReturnChar;

}/* end SD_GetChar */





unsigned int tttt=0;
unsigned int rrrr=0;
interrupt void scib_tx_isr(void){
	if(scib_tx_pos != scib_tx_end){
		ScibRegs.SCITXBUF = scib_tx_buf[scib_tx_pos++];
		if(scib_tx_pos >= SCIB_BUF_SIZE) scib_tx_pos = 0;
		tttt++;
	}
	else{                              
		SCIB_TX_STOP;
	}

	// Acknowledge this interrupt to recieve more interrupts from group 9
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}


unsigned int datascib = 0;
interrupt void scib_rx_isr(void){

   abRxBuffer[ sd_s.iRxPut ] = ScibRegs.SCIRXBUF.all;

   sd_s.iRxSize++;
   sd_s.iRxPut++;

   if( sd_s.iRxPut == SD_RX_BUFFER_SIZE ) sd_s.iRxPut = 0;

	// Acknowledge this interrupt to recieve more interrupts from group 9
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}


// SCI-B �ʱ�ȭ �Լ�
void scib_init(void)
{
	ScibRegs.SCIFFTX.all = 0x8000;			// FIFO reset
 	ScibRegs.SCIFFCT.all = 0x4000;			// Clear ABD(Auto baud bit)
 	
 	ScibRegs.SCICCR.all = 0x0007;  			// 1 stop bit,  No loopback 
                                   			// No parity,8 char bits,
                                   			// async mode, idle-line protocol
	ScibRegs.SCICTL1.all = 0x0003; 			// enable TX, RX, internal SCICLK, 
                                   			// Disable RX ERR, SLEEP, TXWAKE

	ScibRegs.SCICTL2.bit.RXBKINTENA = 1;	// RX/BK INT ENA=1,
	ScibRegs.SCICTL2.bit.TXINTENA = 1;		// TX INT ENA=1,

  	ScibRegs.SCIHBAUD = SCIB_BRR_VAL >> 8;
  	ScibRegs.SCILBAUD = SCIB_BRR_VAL & 0xff;

	ScibRegs.SCICTL1.all = 0x0023;			// Relinquish SCI from Reset  
    
	// Initialize SCI-B RX interrupt
  	EALLOW;
	PieVectTable.SCIRXINTB = &scib_rx_isr;
  	PieVectTable.SCITXINTB = &scib_tx_isr;

    /* Enable internal pull-up for the selected pins */
	GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0; // Enable pull-up for GPIO19 (SCIRXDB)
	GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;  // Enable pull-up for GPIO18  (SCITXDB)

	/* Set qualification for selected pins to asynch only */
	GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3;  // Asynch input GPIO19 (SCIRXDB)

	/* Configure SCI-B pins using GPIO regs*/
	GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 2;   // Configure GPIO19 for SCIRXDB operation
	GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 2;    // Configure GPIO18 for SCITXDB operation

	
	EDIS;
//---------------------------
// �Լ� ���� ���� �ٸ������� �ű� �� ryu
	sd_s.iRxGet = 0;
	sd_s.iRxPut = 0;
	sd_s.iRxSize = 0;
	sd_s.iTxGet = 0;
	sd_s.iTxSize = 0;
//----------------------------

  // Enable CPU INT9 for SCI-B
	IER |= M_INT9;
	
  // Enable SCI-B RX INT in the PIE: Group 9 interrupt 3
	PieCtrlRegs.PIEIER9.bit.INTx3 = 1;

  // Enable SCI-B TX INT in the PIE: Group 9 interrupt 4
	PieCtrlRegs.PIEIER9.bit.INTx4 = 1;
}

