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

#define SD_RX_BUFFER_SIZE     10


typedef struct sd_DataType
{

   BOOL fEnabled;
   UINT8 abRxBuffer[ SD_RX_BUFFER_SIZE ];
   UINT16 iRxGet;
   UINT16 iRxPut;
   UINT16 iRxSize;
   UINT16 iTxGet;

   UINT16 iTxSize;

}

sd_DataType;

sd_DataType sd_s;



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
// 함수 새로 생성 다른곳으로 옮길 것 ryu
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
	PieVectTable.SCIRXINTC = &scic_rx_isr; // HHH
	PieVectTable.SCITXINTC = &scic_tx_isr; // HHH
   
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



   datascib = sd_s.abRxBuffer[ sd_s.iRxPut ] = ScibRegs.SCIRXBUF.all;

	rrrr++;


   /*
   ** go to the next possition in the rx buffer
   */

   sd_s.iRxSize++;
   sd_s.iRxPut++;

   /*
   ** Handle the rotation of our ring buffer
   */

   if( sd_s.iRxPut == SD_RX_BUFFER_SIZE )
   {

      sd_s.iRxPut = 0;

   }/* end if end of buffer */

	
	// Acknowledge this interrupt to recieve more interrupts from group 9
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

//-----
#pragma CODE_SECTION(scic_tx_isr, "ramfuncs");
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
unsigned int SciC_RxStep=0;
unsigned int SciC_RxFlag=0;
unsigned int SciC_TxFlag=0;
unsigned int Communication_Fault_Cnt = 3;
unsigned int Communication_Fault_Flag = 0;

unsigned int Device_type=0;


unsigned int RxType=0;
unsigned int RxAddr=0;
unsigned int RxData=0;
unsigned int RxCRC=0;
unsigned char RxBuf[9];

//unsigned char Temp_A;
//unsigned char Temp_B;
//unsigned int Temp_C;
#pragma CODE_SECTION(scic_rx_isr, "ramfuncs");
interrupt void scic_rx_isr(void)
{
	double Temp;

	Temp= (double)CpuTimer0Regs.TIM.all;
	
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

					SciC_TxFlag = 1;
					//SCIC_TX_START;
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

					SCI_Registers[RxAddr] = 0;

					SciC_TxFlag = 1;
					//SCIC_TX_START;

					// (110107 by HHH)
					Rx_index= RxAddr;
					Read_Data_Registers(Rx_index);
					Flag.Monitoring.bit.EEPROM_WRITE_ENABLE_Rx= 1;
				}
				else if(RxType == QUERY)
				{
					Communication_Fault_Cnt = 3;
					if(RxData==1)
					{
						Device_type= 1;
					}
					else Device_type= 0;

				}
				
			}
			SciC_RxStep=0;
		}
	}
	else SciC_RxStep=0;

	Temp= (Temp-(double)CpuTimer0Regs.TIM.all)/150.;  // us Time
	if (Interrupt_time_max_op<Temp)	Interrupt_time_max_op= Temp; 
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
//#pragma CODE_SECTION(CRC_16, "ramfuncs");
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

	if (ScicRegs.SCIRXST.bit.RXERROR) scic_init(); // Detection of RXERROR
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
#if 1

if (Device_type== 0)
{
	if(Tx_count_25ms>= (Uint16)(25.e-3/Tsamp))
	{
		if (Tx_count_1s>=(Uint16)(1.0/Tsamp))
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
		
			Tx_count_1s= 0;
		}
		else if(SciC_TxFlag)
		{
			SCIC_TX_START;
			SciC_TxFlag=0;
		}
		else 
		{
			//SCI_Registers[SCI_TxOffset] = Data_Registers[SCI_TxOffset];

			CRC.Word = 0;

			scic_putc(0xAB);									CRC_16(0xAB);
			scic_putc(0xCD);									CRC_16(0xCD);

			scic_putc(SEND);									CRC_16(SEND);

			scic_putc((char)(SCI_TxOffset>>8));					CRC_16((char)(SCI_TxOffset>>8));
			scic_putc((char)SCI_TxOffset);						CRC_16((char)SCI_TxOffset);

			scic_putc((char)(Data_Registers[SCI_TxOffset]>>8));	CRC_16((char)(Data_Registers[SCI_TxOffset]>>8));
			scic_putc((char)Data_Registers[SCI_TxOffset]);		CRC_16((char)Data_Registers[SCI_TxOffset]);

			scic_putc(CRC.Byte.b1);
			scic_putc(CRC.Byte.b0);

			SCIC_TX_START;

			// (110107 by HHH)
			Tx_index= (int)SCI_TxOffset;
			Flag.Monitoring.bit.EEPROM_WRITE_ENABLE_Tx= 1;
			SCI_TxOffset ++;
			Tx_complete= 1;
		}

		Tx_count_25ms= 0;
	}

	if ( (Data_Registers[SCI_TxOffset] == SCI_Registers[SCI_TxOffset])&&(Tx_complete== 1) )
		SCI_TxOffset ++;
	else Tx_complete= 0;
	if(Buf_MAX <= SCI_TxOffset)	SCI_TxOffset = 0;
}
else
{
//	if(Tx_count_15ms>= (Uint16)(25.e-3/Tsamp))
//	{
		if (Tx_count_1s>=(Uint16)(1.0/Tsamp))
		{
		/*	CRC.Word = 0;
			scic_putc(0xAB);			CRC_16(0xAB);
			scic_putc(0xCD);			CRC_16(0xCD);

			scic_putc(QUERY);			CRC_16(QUERY);

			scic_putc(0);				CRC_16(0);
			scic_putc(0);				CRC_16(0);

			scic_putc(0);				CRC_16(0);
			scic_putc(0);				CRC_16(0);

			scic_putc(CRC.Byte.b1);
			scic_putc(CRC.Byte.b0);

			SCIC_TX_START;*/

			if(!Communication_Fault_Cnt)Communication_Fault_Flag=1;
			else 
			{
				Communication_Fault_Cnt--;
				Communication_Fault_Flag=0;
			}
		
			Tx_count_1s= 0;
		}
		else if(SciC_TxFlag)
		{
			SCIC_TX_START;
			SciC_TxFlag=0;

			// (110107 by HHH)
			Tx_index= (int)SCI_TxOffset;
			Flag.Monitoring.bit.EEPROM_WRITE_ENABLE_Tx= 1;
		}

//	}
}
	#endif


}


/*------------------------------------------------------------------------------
** SD_CharReceived()
**------------------------------------------------------------------------------
*/


unsigned int cccc=0;
unsigned int kkkk=0;
BOOL SD_CharReceived( void )
{

   /*
   ** Check if there are any characters in the receive buffer.
   */

   if( sd_s.iRxSize == 0 )
   {
	cccc++;
      return FALSE;

   }
   else
   {
	kkkk++;
      return TRUE;

   }/* end if */

} /* end of SD_CharReceived() */

UCHAR SD_GetChar( void )
{

   UCHAR bReturnChar;

   if( sd_s.iRxSize == 0 )
   {

      /*
      ** There is no characters in the buffer.
      */

      return 0;

   }/* end if */

   bReturnChar = sd_s.abRxBuffer[ sd_s.iRxGet ];


   /*
   ** Take a step so we point to the next character in the rx buffer, then
   ** decrement the usage of the buffer indicating that we have seen the
   ** character.
   */

   sd_s.iRxGet++;
   sd_s.iRxSize--;


   /*
   ** Handle the rotation of our ring buffer
   */

   if( sd_s.iRxGet == SD_RX_BUFFER_SIZE )
   {

      sd_s.iRxGet = 0;

   }/* end if */

   return bReturnChar;


}/* end SD_GetChar */






