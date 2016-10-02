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

//void Write_Answer(unsigned char, unsigned char, unsigned int) ;
//void get_cmd(unsigned char,unsigned char);
void Serial_Comm_Service(void) ;
void Serial_Comm_Protocol(void) ;
void Xmitt_Packet_Head(unsigned char OP,unsigned char OBJ);
void Xmitt_Packet_End();
void Write_TransmitSerialStack(unsigned char TXD);
void Write_TransmitSerialStack_CRC_Update(unsigned char TXD);
unsigned char Read_ReceiveSerialStack(unsigned char RXD_StackOffset); 


/************************************************************************/
/*      Initialize SCI                                                  */
/************************************************************************/
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

	//==========================================================================
	// �۽� ��-���� �ʱ�ȭ
	TXD_StackWritePtr = 0;		// Serial ����Ÿ�� �۽� ���� ������
	TXD_StackReadPtr = 0;		// �۽� �������� ���� ���� �������� �� ������

	// ���� ��-���� �ʱ�ȭ
	RXD_StackWritePtr = 0;		// Serial ����Ÿ�� ���� ���� ������
	RXD_StackReadPtr = 0;		// ���Ž������� ���� ���� �������� �� ������

	NewFrame_Packet_State = 0;	// Packet�� ���� �ܰ�
	NewFrame_ByteCnt = 0;		// ���ŵ� ����Ʈ ��
	Frame_ByteCnt = 0 ;
	Packet_ByteCnt = 0 ;

	NewFrame_Detect = 0 ;		// ���ο� ������ ����

	NewFrame_StackPtr = 0 ;
	Frame_StackPtr = 0 ;
	Packet_StackPtr = 0 ;

	//COMM_Watchdog_Timer = 0 ;	// ��Ű��� Ÿ�̸�
	//==========================================================================

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
	// �� ���̿��� rx ó�� �Ѵ�. 
	scib_rxd = ScibRegs.SCIRXBUF.all;
	
	scib_puts("SCI-B: rxd = ");
	scib_putc(scib_rxd);
	scib_puts("\r\n");
	
	/*
	// ���ۿ� �����ϱ⸸ �ϴ� ���
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
/*
	if(scic_tx_pos != scic_tx_end){
		ScicRegs.SCITXBUF = scic_tx_ibuf[scic_tx_pos++];
		if(scic_tx_pos >= SCIC_BUF_SIZE) scic_tx_pos = 0;
	}
	else{                              
		SCIC_TX_STOP;
	}
*/
	if (TXD_StackReadPtr != TXD_StackWritePtr) {
		if(ScicRegs.SCICTL2.bit.TXRDY) {			// �۽Ź��۰� ��� ����
			ScicRegs.SCITXBUF = TXD_Stack[TXD_StackReadPtr++] ;
			if (TXD_StackReadPtr >= TXD_STACK_LENGTH) {
				TXD_StackReadPtr = 0 ;
			}
		}
	}
	else {
	    SCIC_TX_STOP;
	}


	// Acknowledge this interrupt to recieve more interrupts from group 8
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;
}

//-----------------------------
// ����Ÿ ����
//-----------------------------
interrupt void scic_rx_isr(void)
{
	scic_rxd = ScicRegs.SCIRXBUF.all;			//����Ÿ ���� ����
	RXD = 	scic_rxd;

	//=============================
	Serial_Comm_Protocol() ;
	//=============================

	// Acknowledge this interrupt to recieve more interrupts from group 8
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;
}

//-----
void scic_test(void)
{
    scic_puts("SCI-C Test\r\n");
	SCIC_TX_START;
}

//-----
void scic_puti(int rece_)
{
	scic_tx_ibuf[scic_tx_end++] = rece_;
	if(scic_tx_end >= SCIC_BUF_SIZE) scic_tx_end = 0;
}




//===============================================================================================
#if __CRC16_LOOK_UP_TABLE_

	#define	CRC16_Update(x)	CRC.word = (CRC.word << 8) ^ CRC16_Table[((CRC.word >> 8) ^ x) & 0x0ff]

#else
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
#endif

//-

void Write_TransmitSerialStack(unsigned char TXD)
{
	TXD_Stack[TXD_StackWritePtr++] = TXD ;
	if (TXD_StackWritePtr >= TXD_STACK_LENGTH)
	{
		TXD_StackWritePtr = 0 ;
	}
}

//----

void Write_TransmitSerialStack_CRC_Update(unsigned char TXD) 
{
	TXD_Stack[TXD_StackWritePtr++] = TXD ;
	if (TXD_StackWritePtr >= TXD_STACK_LENGTH)	
	{
		TXD_StackWritePtr = 0 ;
	}

	// ����Ÿ�� <DLE>�� ��� �ߺ� ����
	if (TXD == DLE)
	{
		TXD_Stack[TXD_StackWritePtr++] = TXD ;
		if (TXD_StackWritePtr >= TXD_STACK_LENGTH)
		{
			TXD_StackWritePtr = 0 ;
		}
	}

	// CRC-16 ������Ʈ
	#if __CRC16_LOOK_UP_TABLE_
		CRC16_Update(TXD);
	#else
		CRC_16(TXD);
	#endif

}

//-------

unsigned char Read_ReceiveSerialStack(unsigned char RXD_StackOffset)
{
	RXD_StackReadPtr = Packet_StackPtr + RXD_StackOffset ;

	if (RXD_StackReadPtr >= RXD_STACK_LENGTH)		// 55
	{
		RXD_StackReadPtr -= RXD_STACK_LENGTH ;		// �� ����
	}

	return	RXD_Stack[RXD_StackReadPtr];
}

//---------

// <DLE><STX><OP><OBJ>
void Xmitt_Packet_Head(unsigned char OP,unsigned char OBJ) 
{
	CRC.word=0;
	Write_TransmitSerialStack(DLE);
	Write_TransmitSerialStack(STX);

	Write_TransmitSerialStack_CRC_Update(OP);		// ����Ÿ ����
	Write_TransmitSerialStack_CRC_Update(OBJ);		
}

//---------

// <DLE><ETX><CRC[MSB><CRC[LSB]>
void Xmitt_Packet_End()
{
	Write_TransmitSerialStack(DLE);
	Write_TransmitSerialStack(ETX);
	Write_TransmitSerialStack(CRC.byte.b1);			// b0 // -> CRC MSB
	Write_TransmitSerialStack(CRC.byte.b0);			// b1 // -> CRC LSB
}

//------------

// �Ķ���� ���� : ������(Keypad) -> �����̺� (Control Board)
void Send_Parameter(unsigned char GROUP,unsigned char INDEX,unsigned DATA) 
{
	union
	{
		unsigned word;
		struct
		{
			unsigned b0	:8;
			unsigned b1	:8;
		} byte;
	} _data;

	// <DLE><STX><OP><OBJ>
	Xmitt_Packet_Head(OP_WRITE,OBJ_PARAMETER_ACCESS) ;
	
	// <DATA>
	Write_TransmitSerialStack_CRC_Update(1);
	Write_TransmitSerialStack_CRC_Update(DATA_TYPE_WORD);

	Write_TransmitSerialStack_CRC_Update(GROUP);			// �׷��ȣ
	Write_TransmitSerialStack_CRC_Update(INDEX);			// �ε���

	_data.word=DATA ;
	Write_TransmitSerialStack_CRC_Update(_data.byte.b1) ;	// -> MSB
	Write_TransmitSerialStack_CRC_Update(_data.byte.b0) ;	// -> LSB

	// <DLE><ETX><CRC[MSB><CRC[LSB]>
	Xmitt_Packet_End() ;

}

//------------------

void Read_Parameter(unsigned char GROUP,unsigned char INDEX)
{
	// <DLE><STX><OP><OBJ>
	Xmitt_Packet_Head(OP_READ,OBJ_PARAMETER_ACCESS) ;
	
	// <DATA>
	Write_TransmitSerialStack_CRC_Update(1);
	Write_TransmitSerialStack_CRC_Update(DATA_TYPE_WORD);	// ����Ÿ Ÿ��: word

	Write_TransmitSerialStack_CRC_Update(GROUP);			// �׷��ȣ
	Write_TransmitSerialStack_CRC_Update(INDEX);			// �ε���
	
	// <DLE><ETX><CRC[MSB><CRC[LSB]>
	Xmitt_Packet_End() ;
}

//------------------------------

//Keypad���� Read ���� �� ����
void Read_Answer(unsigned char GROUP,unsigned char INDEX,unsigned DATA) 
{
	union
	{
		unsigned word;
		struct
		{
			unsigned b0	:8;
			unsigned b1	:8;
		} byte;
	} _data;

	// <DLE><STX><OP><OBJ>
	Xmitt_Packet_Head(OP_ANSWER,OBJ_PARAMETER_ACCESS) ;
	
	// <DATA>
	Write_TransmitSerialStack_CRC_Update(1);
	Write_TransmitSerialStack_CRC_Update(DATA_TYPE_WORD);

	Write_TransmitSerialStack_CRC_Update(GROUP);			// �׷��ȣ
	Write_TransmitSerialStack_CRC_Update(INDEX);			// �ε���

	_data.word=DATA ;
	Write_TransmitSerialStack_CRC_Update(_data.byte.b1) ;	// -> MSB
	Write_TransmitSerialStack_CRC_Update(_data.byte.b0) ;	// -> LSB

	Xmitt_Packet_End() ;
    //SCIC_TX_iSTART;
}

//-------------------------------

// KeyPAD���� Write ���� �� ����
void Write_Answer(unsigned char GROUP,unsigned char INDEX, unsigned int DATA) 
{
	Xmitt_Packet_Head(OP_ANSWER,OBJ_Recv_Status) ;

	Write_TransmitSerialStack_CRC_Update(0x20);				// STATUS

	Write_TransmitSerialStack_CRC_Update(GROUP);			// �׷��ȣ
	Write_TransmitSerialStack_CRC_Update(INDEX);			// �ε���

	Xmitt_Packet_End() ;
	//SCIC_TX_iSTART;
}

//--------------------
// ��� ��������
//--------------------
void Serial_Comm_Protocol(void)
{
	switch (NewFrame_Packet_State)
	{
		// <DLE>
		case 0 : if (RXD == DLE)
		 	{
				NewFrame_Packet_State++ ;
		 	}
		 	else if (RXD == CR)
		 	{
				Write_TransmitSerialStack(ENQ) ;
				//SCIC_TX_iSTART;
			}
		 	else if (RXD != CR)
		 	{
		 	}
			break;
		// <STX>
		case 1 : if (RXD == STX)
		 	{
		 		NewFrame_Packet_State++ ;
		 	}
		 	else
		 	{
				NewFrame_Packet_State = 0 ;
		 	}
		 	break ;

		// <OP> : operator
		case 2 : if (RXD >= 0x20)
		 	{
		 		NewFrame_Packet_State++ ;

		 		NewFrame_StackPtr = RXD_StackWritePtr ;
		 		RXD_Stack[RXD_StackWritePtr++] = RXD ;
		 	}
		 	else
		 	{
				NewFrame_Packet_State = 0 ;
		 	}
		 	break;

		// <OBJ> : object
		case 3 : if (RXD>=0x20)
		 	{
		 		NewFrame_Packet_State++;
		 		RXD_Stack[RXD_StackWritePtr++] = RXD;
			}
		 	else
		 	{
		 		NewFrame_Packet_State = 0;
		 	}
		 	break;

		// <DATA> : ����Ÿ

		case 4 : if (RXD==DLE)
		 	{
				NewFrame_Packet_State = 5 ;
		 	}
		 	else
		 	{
				RXD_Stack[RXD_StackWritePtr++] = RXD ;
		 	}
				
		 	break;

		// ����� �ν�
		case 5 : if (RXD == STX)
		 	{
		 		NewFrame_Packet_State = 2 ;
		 	}
		 	else if (RXD == ETX)
		 	{
				NewFrame_Packet_State = 6 ;	
		 	}
		 	else if (RXD == DLE)
		 	{
				NewFrame_Packet_State = 4 ;
				RXD_Stack[RXD_StackWritePtr++] = RXD ;
		 	}
		 	else
		 	{
				NewFrame_Packet_State = 0;
		 	}
		 	break;

		// <CRC[MSB]>		
		case 6 : NewFrame_Packet_State = 7 ;
		 		 RXD_Stack[RXD_StackWritePtr++] = RXD ;
			break;

		// <CRC[LSB]>								
		case 7 : NewFrame_Packet_State = 0 ;

		 	NewFrame_ByteCnt = RXD_StackWritePtr - NewFrame_StackPtr + 1 ;
		 	if (NewFrame_ByteCnt <= 0)
		 	{
				NewFrame_ByteCnt += RXD_STACK_LENGTH;
		 	}

		 	RXD_Stack[RXD_StackWritePtr++]= RXD ;	

		 	NewFrame_Detect = 1;

		 	Frame_StackPtr = NewFrame_StackPtr;

		 	Frame_ByteCnt = NewFrame_ByteCnt;

			//-------------------------------------------------------------------------
		 	Data_receive_flg = 1;					//Data�� ���������� ���ŵ��� �˸�.
			//-------------------------------------------------------------------------
				
			break;
	}	//end of switch()

	if (RXD_StackWritePtr >= RXD_STACK_LENGTH)
	{
		RXD_StackWritePtr = 0 ;
	}

}

//----------------------------------------
// main()�������� call�Ѵ�.
//----------------------------------------
void Serial_Comm_Service(void)
{
	int 		  Stack_Index;
	int 		  RXD_StackReadPtr;
	unsigned char Comm_Key_Num;
	unsigned int  Comm_DATA;

	union
	{
		unsigned word;
		struct
		{
			unsigned b0	:8;				// LSB
			unsigned b1	:8;				// MSB
		} b;
	} _code;
	
	if (NewFrame_Detect)
	{
		Packet_StackPtr = Frame_StackPtr ;
		Packet_ByteCnt = Frame_ByteCnt ;

		CRC.word=0;
		for (Stack_Index=0 ; Stack_Index<(Packet_ByteCnt-2) ; Stack_Index++)
		{
			RXD_StackReadPtr = Packet_StackPtr + Stack_Index;
			if (RXD_StackReadPtr >= RXD_STACK_LENGTH)
			{
				RXD_StackReadPtr -= RXD_STACK_LENGTH ;
			}			

			#if __CRC16_LOOK_UP_TABLE_					// 1
				CRC16_Update(RXD_Stack[RXD_StackReadPtr]);

			#else	
				CRC_16(RXD_Stack[RXD_StackReadPtr]);
			#endif
		}

		if ((CRC.byte.b1 == Read_ReceiveSerialStack(Packet_ByteCnt - 2))&& (CRC.byte.b0 == Read_ReceiveSerialStack(Packet_ByteCnt - 1)))
		{
			Packet_Head.OP = Read_ReceiveSerialStack(0);	// operator
			Packet_Head.OBJ = Read_ReceiveSerialStack(1);	// object
			Comm_Key_Num = Read_ReceiveSerialStack(2);		//Key Status
			Comm_GROUP = Read_ReceiveSerialStack(4);
			Comm_INDEX = Read_ReceiveSerialStack(5);
			_code.b.b1 = Read_ReceiveSerialStack(6);		// DATA MSB
			_code.b.b0 = Read_ReceiveSerialStack(7);		// DATA LSB
			Comm_DATA = _code.word;
				
			Comm_array[2106] = (unsigned int)Comm_Key_Num;				// Key ���¸� ���� �Ѵ�

			if (Packet_Head.OP==OP_READ)
			{
				switch (Packet_Head.OBJ)
				{
					case OBJ_PARAMETER_ACCESS :
						 switch(Comm_GROUP)
						 {
							// Mode 2 
							case 0 : Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[Comm_INDEX]);									 break ;
							case 1 : Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[40+Comm_INDEX]);									 break ;
							case 2 : Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[60+Comm_INDEX]);									 break ;
							case 3 : Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[80+Comm_INDEX]);									 break ;
							case 4 : Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[190+Comm_INDEX]);									 break ;
							case 5 : Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[300+Comm_INDEX]);									 break ;
							case 6 : Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[400+Comm_INDEX]);									 break ;
							case 7 : Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[550+Comm_INDEX]);									 break ;
							case 8 : Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[610+Comm_INDEX]);									 break ;
							case 9 : Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[660+Comm_INDEX]);									 break ;
							case 10: Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[700+Comm_INDEX]);									 break ;
							case 11: Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[740+Comm_INDEX]);									 break ;
							case 12: Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[760+Comm_INDEX]);									 break ;
							case 13: Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[780+Comm_INDEX]);									 break ;
							case 14: Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[810+Comm_INDEX]);									 break ;
							case 15: Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[830+Comm_INDEX]);									 break ;
							case 16: Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[890+Comm_INDEX]);									 break ;
							case 17: Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[950+Comm_INDEX]);									 break ;
							case 18: Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[1060+Comm_INDEX]);									 break ;
							case 19: Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[1170+Comm_INDEX]);									 break ;
							case 20: Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[1260+Comm_INDEX]);									 break ;
							case 21: Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[1350+Comm_INDEX]);									 break ;
							case 22: Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[1370+Comm_INDEX]);									 break ;
							case 23: Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[1390+Comm_INDEX]);									 break ;
							case 24: Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[1420+Comm_INDEX]);									 break ;
							case 25: Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[1460+Comm_INDEX]);									 break ;
							case 26: Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[1490+Comm_INDEX]);									 break ;
							case 27: Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[1550+Comm_INDEX]);									 break ;
							case 28: Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[1640+Comm_INDEX]);									 break ;
							case 29: Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[1670+Comm_INDEX]);									 break ;
							case 30: Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[1740+Comm_INDEX]);									 break ;
							case 31: Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[1950+Comm_INDEX]);									 break ;
							case 32: Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[2000+Comm_INDEX]);									 break ;
							// Mode 0
							case 50: Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[2100+Comm_INDEX]);									 break ;
							// Mode 1
							case 51: Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[2110+Comm_INDEX]);									 break ;
							case 52: Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[2140+Comm_INDEX]);									 break ;
							case 53: Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[2150+Comm_INDEX]);									 break ;
							// Mode 3
							case 60: Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[2170+Comm_INDEX]);									 break ;
							// Mode 4
							case 70: Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[2180+Comm_INDEX]);									 break ;
							// Mode 5
							case 80: Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[2280+Comm_INDEX]);									 break ;
							// Mode 6
							case 90: Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[2290+Comm_INDEX]);									 break ;
							case 91: Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[2291+Comm_INDEX]);									 break ;
							case 92: Read_Answer(Comm_GROUP, Comm_INDEX, Comm_array[2296+Comm_INDEX]);									 break ;
						 }
						 //SCIC_TX_iSTART;
					     break;
					case OBJ_PARAMETER_BLOCK_ACCESS	:	// 0x25
					     break;	
				}
			}
			else if (Packet_Head.OP==OP_WRITE)
			{
//				test_led1_toggle; 
				switch (Packet_Head.OBJ)
				{
					case OBJ_PARAMETER_ACCESS :			// 0x24
						 switch(Comm_GROUP)
						 {
							// Mode 2
							case 0 : Comm_array[Comm_INDEX] = Comm_DATA;									 break ;
							case 1 : Comm_array[40+Comm_INDEX]= Comm_DATA;									 break ;
							case 2 : Comm_array[60+Comm_INDEX] = Comm_DATA;									 break ;
							case 3 : Comm_array[80+Comm_INDEX] = Comm_DATA;									 break ;
							case 4 : Comm_array[190+Comm_INDEX] = Comm_DATA;									 break ;
							case 5 : Comm_array[300+Comm_INDEX] = Comm_DATA;									 break ;
							case 6 : Comm_array[400+Comm_INDEX] = Comm_DATA;									 break ;
							case 7 : Comm_array[550+Comm_INDEX] = Comm_DATA;									 break ;
							case 8 : Comm_array[610+Comm_INDEX] = Comm_DATA;									 break ;
							case 9 : Comm_array[660+Comm_INDEX] = Comm_DATA;									 break ;
							case 10: Comm_array[700+Comm_INDEX] = Comm_DATA;									 break ;
							case 11: Comm_array[740+Comm_INDEX] = Comm_DATA;									 break ;
							case 12: Comm_array[760+Comm_INDEX] = Comm_DATA;									 break ;
							case 13: Comm_array[780+Comm_INDEX] = Comm_DATA;									 break ;
							case 14: Comm_array[810+Comm_INDEX] = Comm_DATA;									 break ;
							case 15: Comm_array[830+Comm_INDEX] = Comm_DATA;									 break ;
							case 16: Comm_array[890+Comm_INDEX] = Comm_DATA;									 break ;
							case 17: Comm_array[950+Comm_INDEX] = Comm_DATA;									 break ;
							case 18: Comm_array[1060+Comm_INDEX] = Comm_DATA;									 break ;
							case 19: Comm_array[1170+Comm_INDEX] = Comm_DATA;									 break ;
							case 20: Comm_array[1260+Comm_INDEX] = Comm_DATA;									 break ;
							case 21: Comm_array[1350+Comm_INDEX] = Comm_DATA;									 break ;
							case 22: Comm_array[1370+Comm_INDEX] = Comm_DATA;									 break ;
							case 23: Comm_array[1390+Comm_INDEX] = Comm_DATA;									 break ;
							case 24: Comm_array[1420+Comm_INDEX] = Comm_DATA;									 break ;
							case 25: Comm_array[1460+Comm_INDEX] = Comm_DATA;									 break ;
							case 26: Comm_array[1490+Comm_INDEX] = Comm_DATA;									 break ;
							case 27: Comm_array[1550+Comm_INDEX] = Comm_DATA;									 break ;
							case 28: Comm_array[1640+Comm_INDEX] = Comm_DATA;									 break ;
							case 29: Comm_array[1670+Comm_INDEX] = Comm_DATA;									 break ;
							case 30: Comm_array[1740+Comm_INDEX] = Comm_DATA;									 break ;
							case 31: Comm_array[1950+Comm_INDEX] = Comm_DATA;									 break ;
							case 32: Comm_array[2000+Comm_INDEX] = Comm_DATA;									 break ;
							// Mode 0
							case 50: Comm_array[2100+Comm_INDEX] = Comm_DATA;									 break ;
							// Mode 3
							case 60: Comm_array[2170+Comm_INDEX] = Comm_DATA;									 break ;
							// Mode 5
							case 80: Comm_array[2280+Comm_INDEX] = Comm_DATA;									 break ;
							// Mode 6
							case 90: Comm_array[2290+Comm_INDEX] = Comm_DATA;									 break ;
							case 91:Comm_array[2291+Comm_INDEX] = Comm_DATA;									 break ;
							case 92: Comm_array[2296+Comm_INDEX] = Comm_DATA;									 break ;
						 }	

						 Write_Answer(Comm_GROUP,Comm_INDEX,Comm_DATA);
						 //SCIC_TX_iSTART;
						 Key_pad2eeprom(Comm_GROUP,Comm_INDEX,Comm_DATA);
					     break;
					case OBJ_PARAMETER_BLOCK_ACCESS	:	// 0x25
					     break;	
				}
			} 
		}
		NewFrame_Detect = 0 ;
	}					//end of if(NewFrame_Detect)	
}
//============================================================

