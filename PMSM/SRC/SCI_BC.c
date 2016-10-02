/*****************************************************************************
*
* File Name: SCI_BC.C
*
* Description: SCI_BC driver
*
* Modules Included:
*
*****************************************************************************/

/************************************************************************/
/* INCLUDE                                                              */
/************************************************************************/
#include "sci_bc.h"
//#include <header.h>
//#include <extern.h>


/*---------------------------------------------*/
/*      Initialize SCI                         */
/*---------------------------------------------*/

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
	GpioCtrlRegs.GPAPUD.bit.GPIO23 = 0; // Enable pull-up for GPIO11 (SCIRXDB)
	GpioCtrlRegs.GPAPUD.bit.GPIO22 = 0;  // Enable pull-up for GPIO9  (SCITXDB)

	/* Set qualification for selected pins to asynch only */
	GpioCtrlRegs.GPAQSEL2.bit.GPIO23 = 3;  // Asynch input GPIO11 (SCIRXDB)

	/* Configure SCI-B pins using GPIO regs*/
	GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 3;   // Configure GPIO11 for SCIRXDB operation
	GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 3;    // Configure GPIO9 for SCITXDB operation
	EDIS;

  // Enable CPU INT9 for SCI-B
	IER |= M_INT9;
	
  // Enable SCI-B RX INT in the PIE: Group 9 interrupt 3
	PieCtrlRegs.PIEIER9.bit.INTx3 = 1;

  // Enable SCI-B TX INT in the PIE: Group 9 interrupt 4
	PieCtrlRegs.PIEIER9.bit.INTx4 = 1;
}

void scic_init(){
	ScicRegs.SCIFFTX.all = 0x8000;			// FIFO reset
 	ScicRegs.SCIFFCT.all = 0x4000;			// Clear ABD(Auto baud bit)
 	
 	ScicRegs.SCICCR.all = 0x0007;  			// 1 stop bit,  No loopback 
                                   			// No parity,8 char bits,
                                   			// async mode, idle-line protocol
	ScicRegs.SCICTL1.all = 0x0003; 			// enable TX, RX, internal SCICLK, 
                                   			// Disable RX ERR, SLEEP, TXWAKE

	ScicRegs.SCICTL2.bit.RXBKINTENA = 1;	// RX/BK INT ENA=1,
	ScicRegs.SCICTL2.bit.TXINTENA = 1;		// TX INT ENA=1,

    ScicRegs.SCIHBAUD = SCIC_BRR_VAL >> 8;
    ScicRegs.SCILBAUD = SCIC_BRR_VAL & 0xff;

	ScicRegs.SCICTL1.all = 0x0023;			// Relinquish SCI from Reset  
    
	// Initialize SCI-C RX interrupt
    EALLOW;
	PieVectTable.SCIRXINTC = &scic_rx_isr;
    PieVectTable.SCITXINTC = &scic_tx_isr;
   
    /* Enable internal pull-up for the selected pins */
	GpioCtrlRegs.GPBPUD.bit.GPIO62 = 0; // Enable pull-up for GPIO62 (SCIRXDC)
	GpioCtrlRegs.GPBPUD.bit.GPIO63 = 0;  // Enable pull-up for GPI63  (SCITXDC)

	/* Set qualification for selected pins to asynch only */
	GpioCtrlRegs.GPBQSEL2.bit.GPIO62 = 3;  // Asynch input GPIO11 (SCIRXDC)

	/* Configure SCI-C pins using GPIO regs*/
	GpioCtrlRegs.GPBMUX2.bit.GPIO62 = 1;   // Configure GPIO62 for SCIRXDC operation
	GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 1;    // Configure GPI63 for SCITXDC operation
	EDIS;

    // Enable CPU INT8 for SCI-C
	IER |= M_INT8;
	
    // Enable SCI-C RX INT in the PIE: Group 8 interrupt 5
	PieCtrlRegs.PIEIER8.bit.INTx5 = 1;

    // Enable SCI-C TX INT in the PIE: Group 8 interrupt 6
	PieCtrlRegs.PIEIER8.bit.INTx6 = 1;


}

/************************************************************************/
/*      Transmmit data by polling                                       */
/************************************************************************/
/*---------------------------------------------*/
/*      Transmmit Character                    */
/*---------------------------------------------*/
void scib_TxChar(char c)
{
    while(!ScibRegs.SCICTL2.bit.TXRDY);
    ScibRegs.SCITXBUF=c;
}    

/*---------------------------------------------*/
/*      Transmmit String                       */
/*---------------------------------------------*/
void scib_TxString(char *p)
{
    char	rd;
    while(rd = *p++) scib_TxChar(rd);
}

void scic_TxChar(char c)
{
    while(!ScicRegs.SCICTL2.bit.TXRDY);
    ScicRegs.SCITXBUF=c;
}    

/*---------------------------------------------*/
/*      Transmmit String                       */
/*---------------------------------------------*/
void scic_TxString(char *p)
{
    char	rd;
    while(rd = *p++) scic_TxChar(rd);
}

/************************************************************************/
/*      Transmmit character by interrupt                                */
/************************************************************************/
/*---------------------------------------------*/
/*      SCI TX Start                           */
/*---------------------------------------------*/
void scib_tx_start(void)
{
	SCIB_TX_START;
}

void scic_tx_start(void)
{
	SCIC_TX_START;
}

/*---------------------------------------------*/
/*      Transmmit Character                    */
/*---------------------------------------------*/
void scib_putc(char d)
{
	scib_tx_buf[scib_tx_end++] = d;
	if(scib_tx_end >= SCIB_BUF_SIZE) scib_tx_end = 0;
}

void scic_putc(char d)
{
	scic_tx_buf[scic_tx_end++] = d;
	if(scic_tx_end >= SCIC_BUF_SIZE) scic_tx_end = 0;
}

/*---------------------------------------------*/
/*      Transmmit String                       */
/*---------------------------------------------*/
void scib_puts(char *p)
{
  char rd;

	while(rd = *p++){             
		scib_tx_buf[scib_tx_end++] = rd;
		if(scib_tx_end >= SCIB_BUF_SIZE) scib_tx_end = 0;
	}
}

void scic_puts(char *p)
{
  char rd;

	while(rd = *p++){             
		scic_tx_buf[scic_tx_end++] = rd;
		if(scic_tx_end >= SCIC_BUF_SIZE) scic_tx_end = 0;
	}
}

//-----
int tx_cnt=0, rx_cnt=0;
interrupt void scib_tx_isr(void)
{
	if(scib_tx_pos != scib_tx_end){
		ScibRegs.SCITXBUF = scib_tx_buf[scib_tx_pos++];
		if(scib_tx_pos >= SCIB_BUF_SIZE) scib_tx_pos = 0;
	}
	else{                              
		SCIB_TX_STOP;
	}

	// Acknowledge this interrupt to recieve more interrupts from group 9
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

//-----
interrupt void scib_rx_isr(void)
{
	//------------------------------
	// 이 사이에서 rx 처리 한다. 
	scib_rxd = ScibRegs.SCIRXBUF.all;
	
	scib_puts("SCI-B: rxd = ");
	scib_putc(scib_rxd);
	scib_puts("\r\n");
	
	/*
	// 버퍼에 저장하기만 하는 경우
    scib_rx_buf[scib_rx_end++] = scib_rxd;
    if (scib_rx_end >= SCIC_BUF_SIZE) scib_rx_end = 0;
	*/
	//------------------------------
	SCIB_TX_START;
	
	// Acknowledge this interrupt to recieve more interrupts from group 9
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

//-----
interrupt void scic_tx_isr(void)
{
    tx_cnt++;
	if(scic_tx_pos != scic_tx_end)
	{
		//if(ScicRegs.SCICTL2.bit.TXRDY)
		{	
			ScicRegs.SCITXBUF = scic_tx_buf[scic_tx_pos++];
			if(scic_tx_pos >= SCIC_BUF_SIZE) scic_tx_pos = 0;
		}
	}
	else
	{                              
		SCIC_TX_STOP;
	}

	// Acknowledge this interrupt to recieve more interrupts from group 8
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;
}


//-----------------------------
// 데이타 수신
//-----------------------------
unsigned char SciC_RxStep=0;
unsigned char SciC_RxFlag=0;

unsigned int RxAddr=0;
unsigned int RxData=0;
unsigned int RxCRC=0;
unsigned char RxBuf[8];

//unsigned char Temp_A;
//unsigned char Temp_B;
//unsigned int Temp_C;
interrupt void scic_rx_isr(void)
{
	scic_rxd = ScicRegs.SCIRXBUF.all;			//데이타 수신 버퍼

	if(!SciC_RxFlag)
	{
		if(SciC_RxStep == 0)//sync1
		{
			if(scic_rxd == 0xAB)
			{
				
				RxBuf[0] = scic_rxd;
				SciC_RxStep++;
			}
			else SciC_RxStep=0;
		}
		else if(SciC_RxStep == 1)//sync2
		{
			if(scic_rxd == 0xCD)
			{
				RxBuf[1] = scic_rxd;
				SciC_RxStep++;
			}
			else SciC_RxStep=0;
		}
		else if(SciC_RxStep == 2)//addr_h
		{
			RxBuf[2] = scic_rxd;
			SciC_RxStep++;
		}
		else if(SciC_RxStep == 3)//addr_l
		{
			RxBuf[3] = scic_rxd;
			SciC_RxStep++;
		}
		else if(SciC_RxStep == 4)//data_h
		{
			RxBuf[4] = scic_rxd;
			SciC_RxStep++;
		}
		else if(SciC_RxStep == 5)//data_l
		{
			RxBuf[5] = scic_rxd;
			SciC_RxStep++;
		}
		else if(SciC_RxStep == 6)//crc_H
		{
			RxBuf[6] = scic_rxd;
			SciC_RxStep++;
		}
		else//crc_L
		{
			RxBuf[7] = scic_rxd;

			//scic_putc(RxBuf[0]);
			//scic_putc(RxBuf[1]);
			//scic_putc(RxBuf[2]);
			//scic_putc(RxBuf[3]);
			//scic_putc(RxBuf[4]);
			//scic_putc(RxBuf[5]);
			//scic_putc(RxBuf[6]);
			//scic_putc(RxBuf[7]);

			CRC.word = 0;
			CRC_16(RxBuf[0]);
			CRC_16(RxBuf[1]);
			CRC_16(RxBuf[2]);
			CRC_16(RxBuf[3]);
			CRC_16(RxBuf[4]);
			CRC_16(RxBuf[5]);

			//Temp_A = CRC.byte.b0;
			//Temp_B = CRC.byte.b1;
			//Temp_C = CRC.word;

			RxAddr = ((unsigned int)RxBuf[2]<<8) | RxBuf[3] ;
			RxData = ((unsigned int)RxBuf[4]<<8) | RxBuf[5] ;
			RxCRC   = ((unsigned int)RxBuf[6]<<8) | RxBuf[7] ;

			if((RxBuf[6] == CRC.byte.b1) && (RxBuf[7] == CRC.byte.b0)) SciC_RxFlag=1;
		
			SciC_RxStep=0;
		}
	}
//	else SciC_RxStep=0;

	// Acknowledge this interrupt to recieve more interrupts from group 8
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;
}

//-----
void scic_test(void)
{
    scic_puts("SCI-C Test\r\n");
	SCIC_TX_START;
}




//===============================================================================================
void CRC_16(unsigned char input)
{
	unsigned char 	i ;
	unsigned int 	tmp_CRC ;

	tmp_CRC=((CRC.word >> 8) ^ input) << 8 ;
	for (i = 0 ; i < 8 ; i++)
	{
		if (tmp_CRC & 0x8000) tmp_CRC = (tmp_CRC << 1) ^ GEN_POLYNOMAL ;
		else tmp_CRC <<= 1 ;
	}
	CRC.word = (CRC.word << 8) ^ tmp_CRC ;
}


WORD SCI_Registers[Buf_MAX];
WORD SCI_TxOffset=0;
void SCIC_Tx_process(void)
{
	if(Data_Registers[SCI_TxOffset] != SCI_Registers[SCI_TxOffset])
	{
		//SCI_Registers[SCI_TxOffset] = Data_Registers[SCI_TxOffset];

		CRC.word = 0;

		scic_putc(0xAB);										CRC_16(0xAB);
		scic_putc(0xCD);										CRC_16(0xCD);

		scic_putc((char)(SCI_TxOffset>>8));					CRC_16((char)(SCI_TxOffset>>8));
		scic_putc((char)SCI_TxOffset);							CRC_16((char)SCI_TxOffset);

		scic_putc((char)(Data_Registers[SCI_TxOffset]>>8));	CRC_16((char)(Data_Registers[SCI_TxOffset]>>8));
		scic_putc((char)Data_Registers[SCI_TxOffset]);			CRC_16((char)Data_Registers[SCI_TxOffset]);

		scic_putc(CRC.byte.b1);
		scic_putc(CRC.byte.b0);

		SCIC_TX_START;
	}
	SCI_TxOffset ++;	
	if(Buf_MAX <= SCI_TxOffset) SCI_TxOffset = 0;
}



void SCIC_Rx_process(void)
{
      if(SciC_RxFlag)
      	{
		SCI_Registers[RxAddr] = RxData;
		if(SCI_Registers[RxAddr] != Data_Registers[RxAddr] )
		{
				scic_putc(RxBuf[0]);
				scic_putc(RxBuf[1]);
				scic_putc(RxBuf[2]);
				scic_putc(RxBuf[3]);
				scic_putc(RxBuf[4]);
				scic_putc(RxBuf[5]);
				scic_putc(RxBuf[6]);
				scic_putc(RxBuf[7]);

				SCIC_TX_START;
				Data_Registers[RxAddr] = RxData;
		}
				
		SciC_RxFlag = 0;
      	}
}

