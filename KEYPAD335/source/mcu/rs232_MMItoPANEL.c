//==============================================================================================
//
// Copyright(C) 2008 HanYoung ELEC. Co., LTD.
// Designed by Paul B.H Park(idonno2000)
//
//
//	filename : serial.c
//	description : �ø������ �Լ� ����
//
//	history : 2008-05-09 4:03����
//            2008-06-26 - ���۹� ���Ÿ�� ����
//                       - �۽źκ��� �׽�Ʈ��
//                       - ���ù��� ( ringbuffer.doc )
//                                  ( ������ ��Ŷ�ۼ���.vsd) 
//           2008-08-20 - printf�Լ� ����
//                              - ������ ���Ÿ�� �������� ���� (0or1)
//           1010-03-03 - MINV MMI to CTRL_PANEL�� ��� ����
//                              - ������ �����۸� Ȱ���Ͽ� �ڱ��ڽ��� �����Ͱ� ����嶧
//===============================================================================================

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#include <string.h>
#include "type.h"
#include "util.h"
#include "rs232_MMItoPANEL.h"

#include "display.h"


// ��� ��������

//-- RS232C ��� �������� ����
#define	SOH		0x01		// ��� ����(Start of Heading)
#define	STX		0x02		// �ؽ�Ʈ ���� (Start of Text)
#define	ETX		0x03		// �ؽ�Ʈ ���� (End of Text)
#define	EOT		0x04		// ���� ���� (End of Transmission)
#define	ENQ		0x05		// ���°� ���� (Enquiry)
#define	ACK		0x06		// ���� ���� (Acknowledge)
#define	DLE		0x10		// ���� �������� (Data Link Escape)
#define	NAK		0x15		// ���� ���� (Negative Acknowledge)
#define	SYN		0x16		// ���� ���� (Synchronous Idle)
#define	ETB		0x17		// ��� ���� (End of Transmission Block)		
#define	CR		0x0D		// Carriage Return	

#define	GEN_POLYNOMAL	0x8821		// CRC ����

//-- OP : operator
#define	OP_WRITE	0x40		// ����	
#define	OP_READ		0x80		// �б�
#define	OP_ANSWER	0xC0		// ����

//-- <OBJ> : object 
#define	OBJ_PARAMETER_ACCESS		0x24		// �Ķ���� �б�/���� (R/W)
#define	OBJ_PARAMETER_BLOCK_ACCESS	0x25		// �Ķ���� ��� ������ �б�/���� (R/W)
#define Comm_STATUS_Ok			0x20

//-- ����Ÿ Ÿ��
#define	DATA_TYPE_BYTE		0x30		// ->	����Ʈ 
#define	DATA_TYPE_WORD		0x31		// ->	2 ����Ʈ ����
#define	DATA_TYPE_DWORD		0x32 		// ->	4 ����Ʈ ����
#define	DATA_TYPE_STRING	0x33		// ->	��Ʈ��	

//-- �г� ����Ÿ �׷� �ڵ�
#define	GROUP_CODE_PANEL_DATA	107

//-- �г� �Ķ���� �ڵ� : ��Ʈ�Ѻ��� -> �г�
// 0 	~ 	15		->	���� 1 ����Ÿ				
// 16 	~	31		->	���� 2 ����Ÿ
#define	PANEL_INDEX_LCD_CONTRAST_DUTY	32		// ���� ���� ����
#define	PANEL_INDEX_LIGHT_TIME		33		// ����̵� ���ӽð�	
#define	PANEL_INDEX_KEY_REPEAT_TIME	34		// Ű ���� �Է� ���� �ð�
#define	PANEL_INDEX_LED_DATA		35
#define	PANEL_INDEX_LCD_CURSOR		36
#define	PANEL_INDEX_LCD_CMD		37

//-- �г� -> ��Ʈ�Ѻ���
#define	PANEL_INDEX_KEY			100
#define	PANEL_INDEX_CONNECTION_REFLESH	101
#define PANEL_INDEX_PARAMETER		102

//-----------------------------------------------------------------------------------------


//-- �г� ���� 
#define	PANEL_CONNECTION_REFLESH_TIME	0.5





//-- �����Ʈ�� ���� �ִ� �ð�
#define	DEFAULT_LIGHT_HOLD_TIME_minute	30	// 30�� �� LCD �ڵ� ����


//-- �ۼ��� ���� ����
#define	RXD_STACK_LENGTH	55		// ���Ŵ� ������ ���� ����
#define	TXD_STACK_LENGTH	20		// �۽Ŵ� ������ ���� ����

#define	__CRC16_LOOK_UP_TABLE_	1			// CRC Table ���

#if __CRC16_LOOK_UP_TABLE_
	extern unsigned CRC16_Table[256];
#endif




unsigned char Auto_BR_Detection_Mode;
unsigned char Auto_BR_Detection_Cnt;

//-- Serial Data Stack  
unsigned char 	RRXD_Stack[RXD_STACK_LENGTH];
unsigned char 	RXD;				// ���� ����Ÿ
unsigned char 	RXD_StackWritePtr;		// Serial ����Ÿ�� ���Ž��� ������
unsigned char 	RXD_StackReadPtr;		// ���Ž������� ���� ���� �������� �� ������
unsigned char 	RXD_Stack[RXD_STACK_LENGTH];	// �ø��� ����Ÿ�� �����ϴ� ���� ���� �޸� ����

unsigned char 	TXD_StackWritePtr;		// Serial ����Ÿ�� �۽Ž��� ������
unsigned char 	TXD_StackReadPtr;		// �۽� �������� ���� ���� �������� �� ������
unsigned char 	TXD_Stack[TXD_STACK_LENGTH];	// �ø��� ����Ÿ�� �����ϴ� �۽� ���� �޸� ����
	
unsigned char 	NewFrame_StackPtr;		// ���ð����󿡼� ���� ����� �������� �ּ�
unsigned char 	Frame_StackPtr;			// ���ð����󿡼� ���� ����� �������� �ּ�
unsigned char 	Packet_StackPtr;			// ���� �������� �ش� ��Ŷ�� �ּ�
	
unsigned char 	NewFrame_Detect;		// ���ο� ������ ����
	
unsigned char 	NewFrame_Packet_State;		// Packet�� ���� �ܰ� 
unsigned char 	NewFrame_ByteCnt;		// ���ŵ� ����Ʈ ���� ��Ÿ���� �ε���
unsigned char 	Frame_ByteCnt;
unsigned char 	Packet_ByteCnt;

unsigned int	Para_count ;

unsigned int	ms_cnt ;
unsigned int	s_reg ;

//-- ��� ���Žð� �ʰ��� �ش��ϴ� ī��Ʈ ��
unsigned int 	TimeOut_Cnt;
unsigned int 	COMM_Watchdog_Timer = 0;	// ��� ���� Ÿ�̸�


unsigned char DATA_Sending_flg;
unsigned char Run_Signal_flg;
unsigned char PAR_UP_DN_flg;

unsigned int     Comm_arry_ptr;

unsigned int 	err_cnt ;

#if __CRC16_LOOK_UP_TABLE_

unsigned CRC16_Table[256] ={0,34849,39011,4162,47335,12486,8324,43173,63983,29134,
			24972,59821,16648,51497,55659,20810,31743,62430,58268,27581,
			49944,19257,23419,54106,33296,2609,6771,37458,15095,45782,
			41620,10933,63486,32735,28573,59324,20249,51000,55162,24411,
			3601,34352,38514,7763,46838,16087,11925,42676,35841,1056,
			5218,40003,13542,48327,44165,9380,30190,64975,60813,26028,
			52489,17704,21866,56651,26589,61436,65470,30623,57146,22299,
			18265,53112,40498,5651,1617,36464,9941,44788,48822,13975,
			7202,37891,33857,3168,42181,11492,15526,46215,58829,28140,
			32174,62863,23850,54539,50505,19816,36899,6146,2112,32865,
			10436,41189,45223,14470,27084,57837,61871,31118,53547,22794,
			18760,49513,60380,25597,29631,64414,21307,56090,52056,17273,
			4659,39442,35408,625,43732,8949,12983,47766,53178,18331,
			22489,57336,30557,65404,61246,26399,13909,48756,44598,9751,
			36530,1683,5841,40688,46149,15460,11302,41991,3234,33923,
			38081,7392,19882,50571,54729,24040,62797,32108,27950,58639,
			14404,45157,40999,10246,32931,2178,6336,37089,49579,18826,
			22984,53737,31052,61805,57647,26894,17339,52122,56280,21497,
			64348,29565,25407,60190,47700,12917,8759,43542,691,35474,
			39632,4849,43111,8262,12292,47141,4224,39073,35043,194,
			20872,55721,51691,16842,59759,24910,28940,63789,54168,23481,
			19451,50138,27519,58206,62236,31549,10871,41558,45588,14901,
			37520,6833,2803,33490,24473,55224,51194,20443,59262,28511,
			32541,63292,42614,11863,15893,46644,7825,38576,34546,3795,
			9318,44103,48133,13348,40065,5280,1250,36035,56713,21928,
			17898,52683,25966,60751,64781,29996};

#endif


void wait(void) 
{
        while(s_reg != ms_cnt) ;                //s_reg = ms_cnt�϶����� ��ٸ���.
}


//==============================================================
// UART  initialize
void UART_init(void)
{
	UBRR1H = 0; 								// 19200 baud for 16MHz OSC
	UBRR1L = 51;
	UCSR1A = 0x00;								// asynchronous normal mode
	UCSR1B = 0xD8;								// Rx/Tx enbale, Rx Complete interrupt enable
	UCSR1C = 0x06;								// 8 data, 1 stop, no parity

	//timer 3 ���ͷ�Ʈ
	TCCR3A = 0x10;
	TCCR3B = 0x0C;
	TCCR3C = 0x00;
	OCR3A = 62;//1ms
	TCNT3 = 0x0000;

	TIMSK3 = 0x02;
	TIFR3 = 0x2F;

	ms_cnt = 0 ;
	s_reg = 0 ;

	COMM_Watchdog_Timer = 0 ;

	Auto_BR_Detection_Cnt = 0 ;
	Auto_BR_Detection_Mode = 0 ;

	RXD = 0;			// ���� ����Ÿ
	RXD_StackWritePtr = 0;		// Serial ����Ÿ�� ���� ���� ������
	RXD_StackReadPtr = 0;		// ���� �������� ���� ���� �������� �� ������

	TXD_StackWritePtr = 0 ;		// Serial ����Ÿ�� �۽� ���� ������
	TXD_StackReadPtr = 0 ;		// �۽� �������� ���� ���� �������� �� ������

	NewFrame_StackPtr = 0 ;		// ���ð����󿡼� ���� ����� �������� �ּ�
	Frame_StackPtr = 0 ;		// ���ð����󿡼� ���� ����� �������� �ּ�
	Packet_StackPtr = 0 ;		// ���� �������� �ش� ��Ŷ�� �ּ�

	NewFrame_Detect = 0 ;		// ���ο� ������ ����

	NewFrame_Packet_State = 0 ;	// Packet�� ���� �ܰ�
	NewFrame_ByteCnt = 0 ;		// ���ŵ� ����Ʈ ���� ��Ÿ���� �ε���
	Frame_ByteCnt = 0 ;
	Packet_ByteCnt = 0 ;

	CRC.Word = 0 ;

	Comm_arry_ptr = 0;
}



static unsigned char Xmitt_Delay_Cnt=0;
static unsigned char Xmitt_Key_Num;



//----------------------------------------
//	Timer 3 Interrupt every 1ms
//----------------------------------------
ISR(TIMER3_COMPA_vect)
{
	ms_cnt++ ;				// ���� Ÿ�̸�		
	// ���ÿ� ������ ����Ÿ�� �����Ѵ�.
	// TXD_StackReadPtr�� ����Ÿ 1�� ���۶����� 1�� �����Ѵ�.(�Ʒ� ����)
	// -> U1TXREG = TXD_Stack[TXD_StackReadPtr++]
	// TXD_StackReadPtr�� 20�� �ʰ��ϸ� 0���� �ȴ�.

	// Transmitte_Serial_Stack�� ����Ÿ�� ���� ���� TXD_StackWritePtr�� 1�� ���Ѵ�.
	// TXD_Stack[TXD_StackWritePtr++] = TXD

	if (TXD_StackReadPtr != TXD_StackWritePtr)
	{
		if((UCSR1A & 0x20) == 0x00);// �۽Ź��۰� ��� ����
		{
			if (Xmitt_Delay_Cnt > 1)
			{
				//while((UCSR1A & 0x20) == 0x00);		// data register empty ?
				UDR1 =  TXD_Stack[TXD_StackReadPtr++];

				if (TXD_StackReadPtr >= TXD_STACK_LENGTH)	// > 20	
				{
					TXD_StackReadPtr = 0 ;
				}
				Xmitt_Delay_Cnt = 0 ;
			}
			else
			{
				Xmitt_Delay_Cnt++ ;
			}
		}
	}
}

//----------------------------------------
//	�ø��� ��Ʈ ���ͷ���
//	�۽� �� ���ſϷ� ���ͷ���
//----------------------------------------
/* UART0 interrupt function */

ISR(USART1_RX_vect)	
{
	//int iU1STA_Value = 0 ;

	//iU1STA_Value = U1STA ;
	//IFS0bits.U1RXIF = 0 ;                   	//Clear interrupt flag

	// ���� �Ϸ� => ���� ��������
	// ���ο� ����Ÿ ����

	if(PAR_UP_DN_flg == 1) 		//�Ķ���� UP/Down �ε������� ��� �Ѵ�.
	{
		RXD = UDR1 ;
                
		if(Para_count < RXD_STACK_LENGTH)
		{
			RRXD_Stack[Para_count++] = RXD ;
		}
		else 
		{
			return ;
		}

		// ���� ����Ÿ(RXD)�� �˻��Ѵ�.
		//Serial_Comm_Protocol() ;
	}
	else 				//�����忡�� ��� �Ѵ�.
	{
		RXD = UDR1 ;

		//���� �� ���� ��
		if(Para_count < RXD_STACK_LENGTH)
		{
			RRXD_Stack[Para_count++] = RXD ;
		}
		else 
		{
			return ;
		}
/*
		if (!AUTO_BR._bit.DETECTION_MODE)	//B/R üũ ��尡 �ƴϸ�...
		{
			// ���� ����Ÿ(RXD)�� �˻��Ѵ�.
			Serial_Comm_Protocol() ;
		}
*/
	}
	return ;
}

//------------------------------------------
void Initialize_SCI_Stack(void)
{
	// �۽� ��-���� �ʱ�ȭ
	TXD_StackWritePtr = 0 ;		// Serial ����Ÿ�� �۽� ���� ������
	TXD_StackReadPtr = 0 ;		// �۽� �������� ���� ���� �������� �� ������

	// ���� ��-���� �ʱ�ȭ
	RXD_StackWritePtr = 0 ;		// Serial ����Ÿ�� ���� ���� ������
	RXD_StackReadPtr = 0 ;		// ���Ž������� ���� ���� �������� �� ������

	NewFrame_Packet_State = 0 ;	// Packet�� ���� �ܰ�
	NewFrame_ByteCnt = 0 ;		// ���ŵ� ����Ʈ ��
	Frame_ByteCnt = 0 ;
	Packet_ByteCnt = 0 ;

	NewFrame_Detect = 0 ;		// ���ο� ������ ����

	NewFrame_StackPtr = 0 ;
	Frame_StackPtr = 0 ;
	Packet_StackPtr = 0 ;

	COMM_Watchdog_Timer = 0 ;	// ��Ű��� Ÿ�̸�

	RXD_Stack[0] = UDR1 ;
}

//--------------------------------------------
#if __CRC16_LOOK_UP_TABLE_
	// 16��Ʈ CRC (Cyclic Redundancy Check) ���
	// CRC ������Ʈ
	// x : CRC
	// y : �Է� ����Ÿ

	#define	CRC16_Update(x)	CRC.Word = (CRC.Word << 8) ^ CRC16_Table[((CRC.Word >> 8) ^ x) & 0x0ff]

#else
	// CRC (Cyclic Redundancy Check) : ���� �˻�
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
#endif
//--------------------------------------------

// Transmit Serial Stack�� ����Ÿ�� ����.
void Write_TransmitSerialStack(unsigned char TXD)
{
	TXD_Stack[TXD_StackWritePtr++] = TXD ;
	if (TXD_StackWritePtr >= TXD_STACK_LENGTH)	// TXD_STACK_LENGTH = 20 
	{
		TXD_StackWritePtr = 0 ;
	}
}

// Transmit Serial Stack�� ����Ÿ�� ����, CRC�� ������Ʈ �Ѵ�.
void Write_TransmitSerialStack_CRC_Update(unsigned char TXD) 
{
	TXD_Stack[TXD_StackWritePtr++] = TXD ;
	if (TXD_StackWritePtr >= TXD_STACK_LENGTH)	// TXD_STACK_LENGTH = 20	
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

// Receive Serial Stack���� ���� ���� �������� ����Ÿ�� �д´�.
// RXD_StackOffset�� ���ŵ� ����Ÿ�� ����(?)�� ����Ų��.
unsigned char Read_ReceiveSerialStack(unsigned char RXD_StackOffset)
{

	// ���������ʹ� ���ο� �������� ���� ���������Ϳ� ��������.
	// if(RXD==OP), Packet_StackPtr = Frame_StackPtr = NewFrame_StackPtr = RXD_StackWritePtr
	RXD_StackReadPtr = Packet_StackPtr + RXD_StackOffset ;

	if (RXD_StackReadPtr >= RXD_STACK_LENGTH)		// 55
	{
		RXD_StackReadPtr -= RXD_STACK_LENGTH ;		// �� ����
	}

	return	RXD_Stack[RXD_StackReadPtr];
}


// ��� ��������
// <DLE><STX><OP><OBJ><DATA><DLE><ETX><CRC>
// <OP><OBJ>�� ����ڸ� �������� �ʴ´�. ���� �׻� 0x20 �̻��� ���� ���´�.
// <CRC>�� 16��Ʈ ���� ������, <OP><OBJ><DATA>�� ��꿡 �����ϸ�, ���� ����Ʈ�� �ռ���.
// <DATA>�� <DLE>�� ���Ե� ��� <DLE>�� ��ø��Ű�� �ϳ��� <DLE>�� �ν��Ѵ�.
// <OP><OBJ><DATA><CRC>�� ���ÿ� ����ǰ�, ByteIndex�� ���Եȴ�.


// State	Byte Index
//----------------------------------------------------------------------
//	0	DLE 		<----	Data Line Escape
//	1	STX		<----	Start of Text
//	2	OP	0	<----	operator		: Stack Ptr=0
//	3	OBJ	1	<----	object
//		~
//	4	DATA		<----	max. 256 bytes
//		~
//	5      	DLE		<---	Data Line Escape	: Byte Index���� ����
//	6	ETX		<----	End of Text		: Byte Index���� ����
//	7	CRC[L]		<----	CRC-16 ���� ����Ʈ
//	8	CRC[H]		<----	CRC-16 ���� ����Ʈ
//----------------------------------------------------------------------

// �ܺο��� ����Ÿ ���ŵǰ� B/Rüũ ��尡 �ƴϸ� ����ȴ�.
// �ܺο��� ���ŵ� ����Ÿ(=RXD)�� RXD_STACK[]�� ���� �ȴ�.

void Serial_Comm_Protocol()
{
	switch (NewFrame_Packet_State)
	{
		// <DLE>
		case 0 : if (RXD == DLE)
			 {
				// if (RXD == DLE) �̸� NewFrame_Packet_State++�Ͽ� ������ void Serial_Comm_Protocol() �Լ���
				// ���� �� case 1: �� ���� �ϵ��� �Ѵ�.
				// �̿� ���� ������ ������ NewFrame_Packet_State++�Ͽ� ���������� �̾
				// üũ�ϵ��� �Ѵ�.

				NewFrame_Packet_State++ ;
				Auto_BR_Detection_Cnt = 0 ;	// BR �ڵ� ���� �Ϸ�
			 }
			 else if (RXD == CR)
			 {
				Write_TransmitSerialStack(ENQ) ;
			 }
			 else if (RXD != CR)
			 {
				Auto_BR_Detection_Cnt++ ;		// ī���͸� �����ϰ� 5�� ������ B/R�� �ٽ� üũ �Ѵ�. 
			 }
			break;
		// <STX>
		case 1 : if (RXD == STX)
			 {
			 	NewFrame_Packet_State++ ;
			 	Auto_BR_Detection_Cnt = 0 ;	// BR �ڵ� ���� �Ϸ�
			 }
			 else
			 {
				NewFrame_Packet_State = 0 ;		// <DLE><STX>�� �ƴϸ� ó������ �ǵ��ư�
			 }
			 break ;
		
		// <OP> : operator
		// operator�� 0x20 �̻��� ���� ������ �����Ѵ�.
		// ����Ÿ�� ��� ó���� �� ���� �Ѵ�.

		//    OP Code  	  OP		    ��    ��
		//------------------------------------------------------
		//	40h	WRITE		������ ����
		//	80h	READ		������ �б�
		//	C0h	ANSWER		��û �����Ϳ� ���� ����

		case 2 : if (RXD >= 0x20)
			 {
			 	NewFrame_Packet_State++ ;

			 	// ���ο� �������� ������ ����Ǹ� ������ ���� �����͸� �����Ѵ�.
			 	// <OP> <OBJ> <DATA> <CRC>

			 	NewFrame_StackPtr = RXD_StackWritePtr ;
			 	RXD_Stack[RXD_StackWritePtr++] = RXD ;	//���ŵ� ����Ÿ�� ���� ���ÿ� �����Ѵ�.
			 }
			 else
			 {
				NewFrame_Packet_State = 0 ;		// ó������ �ǵ��ư�
			 }
			 break;

		// <OBJ> : object
		// object�� 0x20�̻��� ���� ������ �����Ѵ�.

		//   OBJ Code	     OBJ			����
		//-----------------------------------------------------------------
		//	20h	Flash Program		����/���� ������ ���α׷�
		//	21h	Flash Block Program	��� ������ ���α׷�
		//	22h	Flash Erase		�÷��� �޸� �����	
		//	24h	Parameter		����/���� �Ķ����/����
		//	25h	Parameter Block Access	��� ������ �Ķ����/����
		//	A0h	STATUS RETURN		���� ���� 
							
		case 3 : if (RXD>=0x20)
			 {
			 	NewFrame_Packet_State++;
			 	RXD_Stack[RXD_StackWritePtr++] = RXD;	//���ŵ� ����Ÿ�� ���� ���ÿ� �����Ѵ�.
			 }
			 else
			 {
			 	NewFrame_Packet_State = 0;		// ó������ �ǵ��ư�
			 }
			 break;

		// <DATA> : ����Ÿ
		// <DLE> : ����� �ν� (���� ���ڴ� ����� �Ǵ� ����Ÿ�� ���� <DLE>�� �´�.)

		// �Ʒ� ���ʹ� ����Ÿ�� �����ϰ� �Ǵµ� <DLE>�� �ö����� �����ϰ� <DLE>�� ����
		// case5�� �ٲپ� ������ ���ŵ� ����ڰ� �������� üũ�Ѵ�.
 
		case 4 : if (RXD==DLE)
			 {
				NewFrame_Packet_State = 5 ;		// ����� �ν� State�� �̵�
			 }
			 else
			 {
				RXD_Stack[RXD_StackWritePtr++] = RXD ;	// State�� ������ �ʴ´�.
			 }
				
			 break;

		// ����� �ν�
		// <DLE><STX>	:	���� ������ �߻��߰ų�, �۽Ŵ����� ����Ÿ�� ������ �� �ٽ� �۽ŵǴ� ���
		// <DLE><ETX>	:	����Ÿ�� ����
		// <DLE><DLE>	:	<DLE>��ø�� �ϳ��� <DLE>�� ����Ÿ�� �ν�
		// <DLE><etc.>	:	���� (���ǵ��� �ʴ� ����Ÿ ����)
		case 5 : if (RXD == STX)
			 {
			 	NewFrame_Packet_State = 2 ;		// <DLE><STX> State�� �̵�
			 }
			 else if (RXD == ETX)
			 {
				NewFrame_Packet_State = 6 ;		// <DLE><ETX> State�� �̵�			
			 }
			 else if (RXD == DLE)
			 {
				NewFrame_Packet_State = 4 ;		// <DATA> State�� �̵�
				RXD_Stack[RXD_StackWritePtr++] = RXD ;	// <DLE>��ø�� �ϳ��� <DLE>�� ����Ÿ�� �ν�	
			 }
			 else
			 {
				NewFrame_Packet_State = 0;		// ����Ÿ ����
			 }
			 break;

		// <CRC[MSB]>		
		case 6 : NewFrame_Packet_State = 7 ;
			 RXD_Stack[RXD_StackWritePtr++] = RXD ;
			 break;

		// <CRC[LSB]>								
		case 7 : NewFrame_Packet_State = 0 ;			// ó������ ���ư�

			//�� case 2:���� if(RXD==OP)�̸� NewFrame_StackPtr = RXD_StackWritePtr�Ѵ�.
			 NewFrame_ByteCnt = RXD_StackWritePtr - NewFrame_StackPtr + 1 ;	// <OP> <OBJ> <DATA> <CRC>
			 if (NewFrame_ByteCnt <= 0)
			 {
				NewFrame_ByteCnt += RXD_STACK_LENGTH;	//RXD_STACK_LENGTH = 55
			 }

			 RXD_Stack[RXD_StackWritePtr++]= RXD ;	

			 NewFrame_Detect = 1;				// => �������� �� : ���ο� ������ ����

			//�� case 2:���� if(RXD==OP)�̸� NewFrame_StackPtr = RXD_StackWritePtr�Ѵ�.
			 Frame_StackPtr = NewFrame_StackPtr;		// ���� �ּ� ����

			//NewFrame_ByteCnt = RXD_StackWritePtr - NewFrame_StackPtr + 1 ;
			 Frame_ByteCnt = NewFrame_ByteCnt;		// ���� ���� ����

			 break;
	}	//end of switch()

	// �� ���� �����÷ο� ����
	if (RXD_StackWritePtr >= RXD_STACK_LENGTH)			//RXD_STACK_LENGTH = 55
	{
		RXD_StackWritePtr = 0 ;
	}
}

//----------------------------------------
// main()�������� call�Ѵ�.
//----------------------------------------
void Serial_Comm_Service() 
{	
	unsigned char  Err_Status;
	int 	Stack_Index;
	int 	RXD_StackReadPtr;

	// "ENQ" ���� -> "CR" ���� 
	// �ʱ� �� AUTO_BR._bit.DETECTION_MODE = 1��
	// ���� ����Ǹ� AUTO_BR._bit.DETECTION_MODE = 0�� �Ǿ� �Ʒ� �κ��� �������� �ʴ´�.

	// B/R üũ .....	

	// �ڵ� B/R ���� ����� ���

	if (Auto_BR_Detection_Mode)
	{
		Xmitt_Delay_Cnt++ ;

		//�Ʒ� ������ Ȯ���غ� ��� ������� �ʴ´�. ��, Xmitt_Delay_Cnt�� 10�� ���� �ʴ´�.
		if (Xmitt_Delay_Cnt > 10)
		{
			Xmitt_Delay_Cnt = 0 ;
			//asm("reset") ;				//�ڵ� reset�� �Ѵ�.
		}

		RXD = NULL ;
		Write_TransmitSerialStack(ENQ) ;		// ENQ : 0x05, ���� ����(Enquiry)

		s_reg = ms_cnt + 1 ;				// �����̺� ���� ��� �ð�
		wait() ;

		if (RXD == CR)					// CR :0x0D, Carriage Return
		{

			Auto_BR_Detection_Cnt++ ;
		}
		else if(Auto_BR_Detection_Cnt> 0)
		{
			Auto_BR_Detection_Cnt-- ;
		}

		// 5 ȸ�̻� "ENQ" -> "CR"�� ����� ����� ������ �Ǵ�
		if (Auto_BR_Detection_Cnt > 5)
		{
			Auto_BR_Detection_Mode= 0 ;
			Auto_BR_Detection_Cnt = 0 ;
			Initialize_SCI_Stack() ;
		}

		NewFrame_Detect = 0;		// �ڵ� B/R ���� ��忡���� �� �ܰ��� ��� ��Ŷ�� ���õȴ�.
						// 0�̸� ���� ��� ��带 �������� ���Ѵ�.
		COMM_Watchdog_Timer = 0;	// ��Ű��� Ÿ�̸�
						// Timer4���� 1�� ���� �Ѵ�.
	}

	// �ڵ� B/R ���� ��尡 �ƴ� ��� = ���� ����̸�
	else 
	{
		// �����Ͽ� 5�� �̻��� �߸��� ����Ÿ�� ���ŵǴ� ���
		// ��Ű��� �ð��ʰ� : Ư���ð� (�õ� Ƚ�� : 5) �̻� ���� �ƹ��� ������ ���� ���� ��� 
		// -> �ڵ� B/R ���� ���

		if (Auto_BR_Detection_Cnt > 5)
		{

			Auto_BR_Detection_Mode = 1 ;
			Auto_BR_Detection_Cnt = 0 ;
			NewFrame_Packet_State = 0 ;
		}

		// ��� ���� : �ð� �ʰ� -> ��� ���� -> ����-����Ʈ �缳��
		// TimeOut_Cnt : 5000
		// COMM_Watchdog_Timer�� 200us���� ���� -> 2000us x 5000 = 10sec
		// �� 10sec�� �Ѿ�� AUTO_BR._bit.DETECTION_MODE�� 1�� �Ͽ� Baud rate�� �ٽ� üũ �Ѵ�.
		// ���� 10�ʰ� �Ѿ�� ���� 0���� ���־�� �Ѵ�.
	
		if (COMM_Watchdog_Timer > TimeOut_Cnt)	
		{
			Auto_BR_Detection_Mode= 1 ;
			Auto_BR_Detection_Cnt = 0 ;
			NewFrame_Packet_State = 0 ;
		}
	}

	//-------------------------------------
	// ���� ��� ���

	// ����Ÿ �������� ���� ��Ŷ�� ó��
	// CRC���� ����Ÿ�� ��� ���ŵǸ� NewFrame_Detect=1�� �ȴ�.
	if (NewFrame_Detect)
	{
		Err_Status = 0 ;

		// ���ÿ� ������ ���� �������� ���� ����Ÿ�� �ߺ��Ǵ� ���� => ���� ����Ÿ ���� 
		// ����Ÿ �������� ����������

		// if(RXD==OP)�̸� Packet_StackPtr = Frame_StackPtr = NewFrame_StackPtr = RXD_StackWritePtr�Ѵ�.
		Packet_StackPtr = Frame_StackPtr ;
		
		// CRC���� ����Ÿ�� ��� ���� �ϸ�, 
		// Frame_ByteCnt = NewFrame_ByteCnt = RXD_StackWritePtr - NewFrame_StackPtr + 1 ;
		// ��, Packet_ByteCnt�� ���ŵ� ����Ÿ�� ����(?)�� ����Ų��.
		Packet_ByteCnt = Frame_ByteCnt ;

		// State	Byte Index
		//----------------------------------------------------------------------
		//	0	DLE 		<----	Data Line Escape
		//	1	STX		<----	Start of Text 
		//	2	OP	0	<----	operator		: Stack Ptr=0
		//	3	OBJ	1	<----	object
		//		~
		//	4	DATA		<----	max. 256 bytes		
		//		~
		//	5      	DLE		<---	Data Line Escape	: Byte Index���� ����
		//	6	ETX		<----	End of Text		: Byte Index���� ����
		//	7	CRC[L]		<----	CRC-16 ���� ����Ʈ
		//	8	CRC[H]		<----	CRC-16 ���� ����Ʈ
		//----------------------------------------------------------------------

		// ���� �˻� : CRC16
		CRC.Word=0;								// CRC �ʱ�ȭ
		for (Stack_Index=0 ; Stack_Index<(Packet_ByteCnt-2) ; Stack_Index++)	// -2 : <CRC> 2����Ʈ�� ����
		{
			RXD_StackReadPtr = Packet_StackPtr + Stack_Index;
			if (RXD_StackReadPtr >= RXD_STACK_LENGTH){			// 55
				RXD_StackReadPtr -= RXD_STACK_LENGTH ;			// �� ����
			}			
			// CRC-16 ������Ʈ	
			#if __CRC16_LOOK_UP_TABLE_					// 1
				CRC16_Update(RXD_Stack[RXD_StackReadPtr]);

			#else	
				CRC_16(RXD_Stack[RXD_StackReadPtr]);
			#endif
		}

		// Cyclic Redundancy Check (CRC[H:L])
		// MSB�� ���� ���ŵ�
		// Packet_ByteCnt�� ���ŵ� ����Ÿ�� ����(?)�� ����Ų��.

		// ���� �������� ����Ÿ�� �д´�.
		if ((CRC.Byte.b1 == Read_ReceiveSerialStack(Packet_ByteCnt - 2))
			&& (CRC.Byte.b0 == Read_ReceiveSerialStack(Packet_ByteCnt - 1)))
		{
			// �ڸ�� �б�
			//	Bit[7:6]
			// 	0x40	:	WRITE
			//	0x80	:	READ
			//	0xC0	:	ANSWER
			Packet_Head.OP=Read_ReceiveSerialStack(0);	// operator
			Packet_Head.OBJ=Read_ReceiveSerialStack(1);	// object

			if (Packet_Head.OP==OP_WRITE)
			{
				// OP_ANSWER�� ��Ŷ�� ���� �Ǹ� => ���� Ÿ�̸� ->  0
				// Packet_Head.OBJ�� ���� OBJ_PARAMETER_BLOCK_ACCESS(0x25)��
				COMM_Watchdog_Timer=0;
				switch (Packet_Head.OBJ)
				{
					case OBJ_PARAMETER_ACCESS :		// 0x24
						WRITE_Packet_PARAMETER_ACCESS();
					     break;
					case OBJ_PARAMETER_BLOCK_ACCESS	:	// 0x25
						WRITE_Packet_PARAMETER_BLOCK_ACCESS();
					     break;	
				}
			} 
		}

		// ���ŵ� �������� ó����.
		// "NewFrame_Detect"�� 0�� �ƴ� ��� ���� �������� ���������� ó���Ѵ�. 
		NewFrame_Detect = 0 ;
		COMM_Watchdog_Timer = 0 ;
	}					//end of if(NewFrame_Detect)	
}	

#if 0
//---------------------------------------------
// �Ķ���� ������Ʈ
// GROUP <- _code.b.b1
// INDEX <- _code.b.b0
// _data <- _data
//---------------------------------------------
void Parameter_Update(unsigned char GROUP,unsigned char INDEX,unsigned char _data) 
{
	if (GROUP == GROUP_CODE_PANEL_DATA)	// GROUP_CODE_PANEL_DATA = 107
	{
		if (INDEX < 32) 
		{
			string[INDEX] = _data;
		}
		else 
		{
			switch (INDEX)
			{
				// LCD ���� ���� ���� 
				// PANEL_INDEX_LCD_CONTRAST_DUTY = 32

				case PANEL_INDEX_LCD_CONTRAST_DUTY :
						if (_data <= 10) {
							Contrast_Cnt_Duty = _data ;
						}
						else {
							Contrast_Cnt_Duty = 10 ;
						}
				     		break ;	

				// �����Ʈ ��Ʈ�� : �����Ʈ�� �������� ���� �ִ� �ð�
				// PANEL_INDEX_LIGHT_TIME = 33

				case PANEL_INDEX_LIGHT_TIME :
						LIGHT_HOLD_TIME_minute = _data ;
						break;

				// Ű ���� �Է� ���� �ð� (x100) : 0.25 ~ 2.0�� -> 25 ~ 200
				// PANEL_INDEX_KEY_REPEAT_TIME = 34

				case PANEL_INDEX_KEY_REPEAT_TIME :
						if (_data < 25) {
							_data = 25 ;
						}
						else if (_data > 200) {
							_data = 200 ;
						}
						New_Key_Enable_Delay_Cnt_Period=(unsigned)(_data*inv_Ts_f_x100);
						break;
				// LED ���
				// PANEL_INDEX_LED_DATA = 35
				case PANEL_INDEX_LED_DATA :
						LED.byte = _data;
						break;
				// Ŀ����ġ
				// PANEL_INDEX_LCD_CURSOR = 36
				case PANEL_INDEX_LCD_CURSOR :
						// Ŀ���� ��ġ ����
						if (_data < 16) {
							Cursor(0, _data);
						}
						else if (_data < 32) {
							Cursor(1, (_data - 16));
						}		
						break;
				// LCD ��� �ڸ��
				// PANEL_INDEX_LCD_CMD = 37
				case PANEL_INDEX_LCD_CMD :
						LCD_Cmd(_data);
						break;
			}
		}
	}
}
#endif

// �Ķ���� ����/�б�
//----------------------------------------------------------------------------------------
// OBJ : "PARAMETER_ACCESS" : 1 ����Ʈ�� ����
// �� ����Ʈ ��	= Packet Head (2) : 	<OP> <OBJ>
//		  Data Head (2)   : 	<No. of Data> <Data Type>
//		  Datum (3 x N)	  :	<ADDR.1[MSB]> ~ <DATA.N>
//		  CRC (2)	  :	<CRC[MSB]> <CRC[LSB]>
//		  =>	(3 x N) + 6
//				
// Byte Index	����Ÿ
//	0	<OP>			<----	WRITE
//	1	<OBJ>			<----	PARAMETER ACCESS
//	2	<No. of Data :N>	<---- 	����Ÿ�� �� ����
//	3	<DATA TYPE>		<----	����Ÿ Ÿ��	
//	4	ADDR.1[MSB]		<----	ù��° ����Ÿ �ּ��� �Ķ���� �׷� �ڵ�
//	5	ADDR.1[LSB]
//	6	DATA.1			<----	ù��° ����Ÿ
//	~					~
//	~					~
//(3 x N) + 1	ADDR.N[MSB]		<----	N ��° ����Ÿ �ּ��� �Ķ���� �׷� �ڵ�
//(3 x N) + 2	ADDR.N[LSB]
//(3 x N) + 3	DATA.N			<----	N ��° ����Ÿ
//(3 x N) + 4	CRC[MSB]		<----	CRC
//(3 x N) + 5 	CRC[LSB]	
//----------------------------------------------------------------------------------------
void WRITE_Packet_PARAMETER_ACCESS()
{
	unsigned char StackIndex;
	unsigned char _data;

	union
	{
		unsigned int w;
		struct
		{
			unsigned char b0;
			unsigned char b1;
		} b;
	} _code;					// �ڵ�[H:L]

	// ����Ÿ Ÿ���� ����Ʈ ��
	// ����Ÿ�� ���� ��ġ�ϴ��� Ȯ��
	if (Packet_ByteCnt == (3*Read_ReceiveSerialStack(2)+6))
	{
		// �Ķ���� ����	: 2 ����Ʈ�� ����	
		for (StackIndex=4;StackIndex<(Packet_ByteCnt-2);)	// <ADDR.1[MSB]> ~ <DATA.N>
		{
			// ��巹��
			_code.b.b1 = Read_ReceiveSerialStack(StackIndex++);	// �Ķ���� �׷�
			_code.b.b0 = Read_ReceiveSerialStack(StackIndex++);	// �Ķ���� �ε���		
			_data = Read_ReceiveSerialStack(StackIndex++);		// �Ķ���� ��

			// �Ķ���� ������Ʈ
			// _code.b.b0 < 32�� ��� _data�� string[]�� �����Ѵ�.
			// _code.b.b1 -> GROUP, _code.b.b0 -> INDEX, _data -> DATA
			//Parameter_Update(_code.b.b1,_code.b.b0,_data) ;
		}
	}
}

//--------------------------------------------------------------------------------------
// OBJ : "PARAMETER_BLOCK_ACCESS" : 1 ����Ʈ�� ����
// �� ����Ʈ �� = Packet Head (2) : 	<OP> <OBJ>
//		  Data Head (2)   : 	<No. of Data> <DATA TYPE>
//		  Datum (N+2)	  :	<ADDR[LSB]><ADDR[MSB]> ~ <DATA.N>
//		  CRC (2)	  :	<CRC[MSB]> <CRC[LSB]>
//					=>	N + 8
//				
// Byte Index		����Ÿ
//	0		<OP>			<----	WRITE
//	1		<OBJ>			<----	PARAMETER BLOCK ACCESS
//	2		<No. of Data :N>	<---- 	����Ÿ�� �� ����
//	3		<DATA TYPE>
//	4		ADDR[MSB]		<----	ù��° ����Ÿ �ּ��� �Ķ���� �׷� �ڵ�
//	5		ADDR[LSB]
//	6		DATA.1			<----	ù��° ����Ÿ
//	~		~
//	~		~
//	N + 5		DATA.N			<----	N ��° ����Ÿ
//	N + 6		CRC[MSB]		<----	CRC
//	N + 7 		CRC[LSB]	
//--------------------------------------------------------------------------------------
void WRITE_Packet_PARAMETER_BLOCK_ACCESS() 
{
	unsigned char StackIndex;
	unsigned char _data;
	union
	{
		unsigned int w;
		struct
		{
			unsigned char b0;				// LSB
			unsigned char b1;				// MSB
		} b;
	} _code;
									// �ڵ�[H:L]
	// ����Ÿ Ÿ���� ����Ʈ ��
	// ����Ÿ�� ���� ��ġ�ϴ��� Ȯ��

	if (Packet_ByteCnt == (Read_ReceiveSerialStack(2)+8))
	{
		_code.b.b1 = Read_ReceiveSerialStack(4);		// �Ķ���� �ڵ�
		_code.b.b0 = Read_ReceiveSerialStack(5);

		// �Ķ���� ����	: 2 ����Ʈ�� ����	
		for (StackIndex = 6 ; StackIndex < (Packet_ByteCnt-2) ; )			
		{	
			_data = Read_ReceiveSerialStack(StackIndex++);	// �Ķ���� ��
			
			// �Ķ���� ������Ʈ
			// _code.b.b0 < 32�� ��� _data�� string[]�� �����Ѵ�.
			// _code.b.b1 -> GROUP, _code.b.b0 -> INDEX, _data -> DATA
			//Parameter_Update(_code.b.b1,_code.b.b0,_data);
			_code.b.b0++;
		}		
	}
}

// <DLE><STX><OP><OBJ>
void Xmitt_Packet_Head(unsigned char OP,unsigned char OBJ) 
{
	CRC.Word=0;
	Write_TransmitSerialStack(DLE);
	Write_TransmitSerialStack(STX);

	Write_TransmitSerialStack_CRC_Update(OP);		// ����Ÿ ����
	Write_TransmitSerialStack_CRC_Update(OBJ);		
}

// <DLE><ETX><CRC[MSB><CRC[LSB]>
void Xmitt_Packet_End()
{
	Write_TransmitSerialStack(DLE);
	Write_TransmitSerialStack(ETX);
	Write_TransmitSerialStack(CRC.Byte.b1);			// b0 // -> CRC MSB
	Write_TransmitSerialStack(CRC.Byte.b0);			// b1 // -> CRC LSB
}

// ����Ÿ ���ε� : 1���� ����
// State	TXD
//----------------------------------------------------------------------
//	0	DLE 			<----	Data Line Escape
//	1	STX			<----	Start of Text 
//	2	OP_WRITE		<----	operator
//	3	OBJ_PARAMETER_ACCESS	<----	object
//	4	1			<----	No. of Data
//	4	DATA TYPE		<----	Data Type : byte, word, double word
//	4	GROUP			<----	�Ķ���� �׷�		
//	4	INDEX			<----	�׷쳻 �ε���
//	4	DATA[MSB]		<----	����Ÿ : MSB						
//	4	DATA[LSB]		<----	����Ÿ : LSB						
//	5      	DLE			<---	Data Line Escape	: Byte Index���� ����
//	6	ETX			<----	End of Text		: Byte Index���� ����
//	7	CRC[MSB]		<----	CRC-16 : MSB
//	8	CRC[LSB]		<----	CRC-16 : LSB
//----------------------------------------------------------------------

// �Ķ���� ���� : ������(�г�) -> �����̺� (��Ʈ�� ����)

void Send_Parameter(unsigned char GROUP,unsigned char INDEX,unsigned DATA) 
{
	//DATA_Sending_flg = 1;

	unsigned char tmp_Group;
	unsigned char tmp_Index;
	unsigned int  tmp_Data;

	union
	{
		unsigned int Word;
		struct
		{
			unsigned char b0;
			unsigned char b1;
		} Byte;
	} _data;

	tmp_Group = GROUP;
	tmp_Index = INDEX;
	tmp_Data = DATA;

	// <DLE><STX><OP><OBJ>
        // OP_WRITE = 0x40
        // OBJ_PARAMETER_ACCESS = 0x24
	Xmitt_Packet_Head(OP_WRITE,OBJ_PARAMETER_ACCESS) ;
	
	// <DATA>
	if(Run_Signal_flg == 1) Write_TransmitSerialStack_CRC_Update(0xff);
	else Write_TransmitSerialStack_CRC_Update(0xf0);
	//Write_TransmitSerialStack_CRC_Update(1);		// ����Ÿ ���� : 1

	Write_TransmitSerialStack_CRC_Update(DATA_TYPE_WORD);	// ����Ÿ Ÿ��: word

	Write_TransmitSerialStack_CRC_Update(GROUP);		// �׷��ȣ
	Write_TransmitSerialStack_CRC_Update(INDEX);		// �ε���

	_data.Word=DATA ;					// ����Ÿ
	Write_TransmitSerialStack_CRC_Update(_data.Byte.b1) ;	// -> MSB
	Write_TransmitSerialStack_CRC_Update(_data.Byte.b0) ;	// -> LSB

	// <DLE><ETX><CRC[MSB><CRC[LSB]>
	Xmitt_Packet_End() ;

	s_reg = ms_cnt + 100 ;
	wait() ;

	if((RRXD_Stack[4] == Comm_STATUS_Ok) && (RRXD_Stack[5] == tmp_Group) && (RRXD_Stack[6] == tmp_Index))
	{

	}
	else
	{
		// Control Board�� Write�� ���� ������ �ݺ��Ѵ�
		Send_Parameter(tmp_Group,tmp_Index,tmp_Data);
	}

	//DATA_Sending_flg = 0;
}

void Read_Parameter(unsigned char GROUP,unsigned char INDEX)
{
	// <DLE><STX><OP><OBJ>
        // OP_READ = 0x80
        // OBJ_PARAMETER_ACCESS = 0x24
	Xmitt_Packet_Head(OP_READ,OBJ_PARAMETER_ACCESS) ;
	
	// <DATA>
//	Write_TransmitSerialStack_CRC_Update(1);		// ����Ÿ ���� : 1

	Write_TransmitSerialStack_CRC_Update(Xmitt_Key_Num);

	Write_TransmitSerialStack_CRC_Update(DATA_TYPE_WORD);	// ����Ÿ Ÿ��: word

	Write_TransmitSerialStack_CRC_Update(GROUP);		// �׷��ȣ
	Write_TransmitSerialStack_CRC_Update(INDEX);		// �ε���
	
	// <DLE><ETX><CRC[MSB><CRC[LSB]>
	Xmitt_Packet_End() ;
}

//----------
void Read_DATA_from_ControlBoard(unsigned char GROUP, unsigned char INDEX)
{
	unsigned char i=0 ;
	unsigned char tmp_Group ;
	unsigned char tmp_Index ;

	union
	{
		unsigned int Word;
		struct
		{
			unsigned char b0;
			unsigned char b1;
		}Byte;
	} _data;

	tmp_Group = GROUP;
	tmp_Index = INDEX;
	i = INDEX ;

	Read_Parameter(GROUP, INDEX) ;
	Para_count = 0 ;
	s_reg = ms_cnt + 100 ;
	wait() ;

	if(GROUP != 16) 		// Group 16�� �ƴϸ�
	{
		if((RRXD_Stack[2] == OP_ANSWER) && (RRXD_Stack[3] == OBJ_PARAMETER_ACCESS) && (RRXD_Stack[6] == GROUP) && (RRXD_Stack[7] == INDEX))
		{
			err_cnt = 0 ;					//���� ī��Ʈ reset

			if(i != 16) {
				_data.Byte.b0 = RRXD_Stack[9] ;	//���� ����Ʈ
				_data.Byte.b1 = RRXD_Stack[8] ;	//��������Ʈ
			}
			else {
				_data.Byte.b0 = RRXD_Stack[10] ;	//���� ����Ʈ
				_data.Byte.b1 = RRXD_Stack[9] ;	//��������Ʈ
			}

			//Para_Addr_Mem_[i*2] = _PAR_data.b.b0 ;
			//Para_Addr_Mem_[(i*2)+1] = _PAR_data.b.b1 ;
                     //   Comm_array[Comm_arry_ptr] = _data.word;
			*(volatile int *)((Comm_arry_ptr<<1)) = _data.Byte.b0;
			*(volatile int *)((Comm_arry_ptr<<1)+1) = _data.Byte.b1;
			//LED_FAULT = 0 ;
		}
		else
		{
			//LED_FAULT = 1 ;
			Read_DATA_from_ControlBoard(tmp_Group, tmp_Index) ;
		}
	}
	else 				// Group 16�̸�
	{
		if((RRXD_Stack[2] == OP_ANSWER) && (RRXD_Stack[3] == OBJ_PARAMETER_ACCESS) && (RRXD_Stack[6] == GROUP) && (RRXD_Stack[7] == INDEX))
		{
			err_cnt = 0 ;					//���� ī��Ʈ reset
			if(i != 16) {
				_data.Byte.b0 = RRXD_Stack[10] ;	//���� ����Ʈ
				_data.Byte.b1 = RRXD_Stack[9] ;	//��������Ʈ
			}
			else {
				_data.Byte.b0 = RRXD_Stack[11] ;	//���� ����Ʈ
				_data.Byte.b1 = RRXD_Stack[10] ;	//��������Ʈ
			}

			//Para_Addr_Mem_[i*2] = _PAR_data.b.b0 ;
			//Para_Addr_Mem_[(i*2)+1] = _PAR_data.b.b1 ;
			*(volatile int *)((Comm_arry_ptr<<1)) = _data.Byte.b0;
			*(volatile int *)((Comm_arry_ptr<<1)+1) = _data.Byte.b1;
			//LED_FAULT = 0 ;
		}
		else
		{
			//LED_FAULT = 1 ;
			Read_DATA_from_ControlBoard(tmp_Group, tmp_Index) ;
		}
	}
	Para_count = 0 ;
}

//==============================================================
// base function

void TX1_char( char data)		/* transmit a character by USART0 */
{
	while((UCSR1A & 0x20) == 0x00);		// data register empty ?
	UDR1 = data;
}



ISR(USART1_TX_vect)             // USART2 Tx Complete interrupt
{
	UCSR0B = UCSR0B | 0x10;
}



