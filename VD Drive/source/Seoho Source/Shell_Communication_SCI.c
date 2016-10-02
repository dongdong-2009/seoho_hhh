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

#include	<All_Header.h>
#include	<All_Extern_Variables.h>
#include 	"Shell_SCI_BC.h"

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
	SCIB_TX_START;
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
unsigned int Communication_Fault_Cnt = 3;
unsigned char Communication_Fault_Flag = 0;


unsigned int RxType=0;
unsigned int RxAddr=0;
unsigned int RxData=0;
unsigned int RxCRC=0;
unsigned char RxBuf[9];

//unsigned char Temp_A;
//unsigned char Temp_B;
//unsigned int Temp_C;
interrupt void scic_rx_isr(void)
{
	scic_rxd = ScicRegs.SCIRXBUF.all;
	if(!SciC_RxFlag)
	{
		if(SciC_RxStep == 0)//sync1
		{
			if(scic_rxd == 0xAB)
			{
				
				RxBuf[0] = 0xAB;
				SciC_RxStep++;
			}
			else SciC_RxStep=0;
		}
		else if(SciC_RxStep == 1)//sync2
		{
			if(scic_rxd == 0xCD)
			{
				RxBuf[1] = 0xCD;
				SciC_RxStep++;
			}
			else SciC_RxStep=0;
		}
		else if(SciC_RxStep == 2)//type
		{
			RxBuf[2] = scic_rxd;
			SciC_RxStep++;
		}
		else if(SciC_RxStep == 3)//addr_h
		{
			RxBuf[3] = scic_rxd;
			SciC_RxStep++;
		}
		else if(SciC_RxStep == 4)//addr_l
		{
			RxBuf[4] = scic_rxd;
			SciC_RxStep++;
		}
		else if(SciC_RxStep == 5)//data_h
		{
			RxBuf[5] = scic_rxd;
			SciC_RxStep++;
		}
		else if(SciC_RxStep == 6)//data_l
		{
			RxBuf[6] = scic_rxd;
			SciC_RxStep++;
		}
		else if(SciC_RxStep == 7)//crc_H
		{
			RxBuf[7] = scic_rxd;
			SciC_RxStep++;
		}
		else//crc_L
		{
			RxBuf[8] = scic_rxd;

			CRC.Word = 0;
			CRC_16(RxBuf[0]);
			CRC_16(RxBuf[1]);
			CRC_16(RxBuf[2]);
			CRC_16(RxBuf[3]);
			CRC_16(RxBuf[4]);
			CRC_16(RxBuf[5]);
			CRC_16(RxBuf[6]);

			RxType = RxBuf[2];
			RxAddr = ((unsigned int)RxBuf[3]<<8) | RxBuf[4] ;
			RxData = ((unsigned int)RxBuf[5]<<8) | RxBuf[6] ;
			RxCRC   = ((unsigned int)RxBuf[7]<<8) | RxBuf[8] ;

			if((RxBuf[7] == CRC.Byte.b1) && (RxBuf[8] == CRC.Byte.b0))
			{
				SciC_RxFlag=1;
				SCI_Registers[RxAddr] = RxData;

				if(RxType == SEND)
				{
					CRC.Word = 0;
					scic_putc(RxBuf[0]);		CRC_16(RxBuf[0]);
					scic_putc(RxBuf[1]);		CRC_16(RxBuf[1]);
					scic_putc(RESPONSE);		CRC_16(RESPONSE);
					scic_putc(RxBuf[3]);		CRC_16(RxBuf[3]);
					scic_putc(RxBuf[4]);		CRC_16(RxBuf[4]);
					scic_putc(RxBuf[5]);		CRC_16(RxBuf[5]);
					scic_putc(RxBuf[6]);		CRC_16(RxBuf[6]);
					scic_putc(CRC.Byte.b1);
					scic_putc(CRC.Byte.b0);

					SCIC_TX_START;
					Data_Registers[RxAddr] = RxData;

					// (110107 by HHH)
					Rx_index= RxAddr;
					Read_Data_Registers(Rx_index);
					Flag.Monitoring.bit.EEPROM_WRITE_ENABLE_Rx= 1;
				}
				else if(RxType == REQUEST)
				{
					CRC.Word = 0;
					scic_putc(RxBuf[0]);								CRC_16(RxBuf[0]);
					scic_putc(RxBuf[1]);								CRC_16(RxBuf[1]);
					scic_putc(SEND);									CRC_16(SEND);
					scic_putc(RxBuf[3]);								CRC_16(RxBuf[3]);
					scic_putc(RxBuf[4]);								CRC_16(RxBuf[4]);
					scic_putc((char)(Data_Registers[RxAddr]>>8)); CRC_16((char)(Data_Registers[RxAddr]>>8));
					scic_putc((char)Data_Registers[RxAddr]);		CRC_16((char)Data_Registers[RxAddr]);

					scic_putc(CRC.Byte.b1);
					scic_putc(CRC.Byte.b0);

					SCIC_TX_START;
				}
				else if(RxType == QUERY)
				{
					Communication_Fault_Cnt = 3;
				}
				
			}
			SciC_RxStep=0;
		}
	}
	else SciC_RxStep=0;


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

	tmp_CRC=((CRC.Word >> 8) ^ input) << 8 ;
	for (i = 0 ; i < 8 ; i++)
	{
		if (tmp_CRC & 0x8000) tmp_CRC = (tmp_CRC << 1) ^ GEN_POLYNOMAL ;
		else tmp_CRC <<= 1 ;
	}
	CRC.Word = (CRC.Word << 8) ^ tmp_CRC ;
}



// WORD SCI_Registers[Buf_MAX];
WORD SCI_TxOffset=0;
void SCIC_Process(void)
{
	unsigned int i;
	static int Tx_complete= 1;
//Rx========================================
    if(SciC_RxFlag)	SciC_RxFlag = 0;

	if(Data_Registers[3195])
	{
		for(i=0;i<Buf_MAX;i++)	SCI_Registers[i]=0x0000;
		Data_Registers[3195] = 0;
		SCI_Registers[3195] = 1;
	}

//Tx========================================
	// (110120 by HHH)
	// data 가 변경될 경우 Tx 발생
	// Tx_count 시간에 도달 하지 못한 경우 자동 대기 함
//	SCI_TxOffset= 2310;
	if(Data_Registers[SCI_TxOffset] != SCI_Registers[SCI_TxOffset])
	{
		if(Tx_count>= (int)(1.e-2/Tsamp))
		{
			//SCI_Registers[SCI_TxOffset] = Data_Registers[SCI_TxOffset];

			CRC.Word = 0;

			scic_putc(0xAB);										CRC_16(0xAB);
			scic_putc(0xCD);										CRC_16(0xCD);

			scic_putc(SEND);										CRC_16(SEND);

			scic_putc((char)(SCI_TxOffset>>8));					CRC_16((char)(SCI_TxOffset>>8));
			scic_putc((char)SCI_TxOffset);							CRC_16((char)SCI_TxOffset);

			scic_putc((char)(Data_Registers[SCI_TxOffset]>>8));	CRC_16((char)(Data_Registers[SCI_TxOffset]>>8));
			scic_putc((char)Data_Registers[SCI_TxOffset]);			CRC_16((char)Data_Registers[SCI_TxOffset]);

			scic_putc(CRC.Byte.b1);
			scic_putc(CRC.Byte.b0);

			SCIC_TX_START;

			// (110107 by HHH)
			Tx_index= (int)SCI_TxOffset;
			Tx_count= 0;
			Tx_complete= 1;
			Flag.Monitoring.bit.EEPROM_WRITE_ENABLE_Tx= 1;
		}
		else Tx_complete= 0;
	}
	else
	{
		if (Tx_complete) SCI_TxOffset ++;	
			
		if (Tx_test>=(Uint16)(1.0/Tsamp))
		{
			CRC.Word = 0;
			scic_putc(0xAB);			CRC_16(0xAB);
			scic_putc(0xCD);			CRC_16(0xCD);

			scic_putc(QUERY);			CRC_16(QUERY);

			scic_putc(0);				CRC_16(0);
			scic_putc(0);				CRC_16(0);

			scic_putc(0);				CRC_16(0);
			scic_putc(0);				CRC_16(0);

			scic_putc(CRC.Byte.b1);
			scic_putc(CRC.Byte.b0);

			SCIC_TX_START;

			if(!Communication_Fault_Cnt)Communication_Fault_Flag=1;
			else 
			{
				Communication_Fault_Cnt--;
				Communication_Fault_Flag=0;
			}
		
			Tx_test= 0;
		}
		if(Buf_MAX <= SCI_TxOffset) 
		{
			SCI_TxOffset = 0;
/*
			CRC.Word = 0;
			scic_putc(0xAB);			CRC_16(0xAB);
			scic_putc(0xCD);			CRC_16(0xCD);

			scic_putc(QUERY);			CRC_16(QUERY);

			scic_putc(0);				CRC_16(0);
			scic_putc(0);				CRC_16(0);

			scic_putc(0);				CRC_16(0);
			scic_putc(0);				CRC_16(0);

			scic_putc(CRC.Byte.b1);
			scic_putc(CRC.Byte.b0);

			SCIC_TX_START;

			if(!Communication_Fault_Cnt)Communication_Fault_Flag=1;
			else 
			{
				Communication_Fault_Cnt--;
				Communication_Fault_Flag=0;
			}
*/		}
	}

}








