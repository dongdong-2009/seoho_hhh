//==============================================================================================
//
// Copyright(C) 2008 HanYoung ELEC. Co., LTD.
// Designed by Paul B.H Park(idonno2000)
//
//
//	filename : serial.c
//	description : 시리얼통신 함수 정의
//
//	history : 2008-05-09 4:03오후
//            2008-06-26 - 링퍼버 수신모듈 구성
//                       - 송신부분은 테스트중
//                       - 관련문서 ( ringbuffer.doc )
//                                  ( 링버퍼 패킷송수신.vsd) 
//           2008-08-20 - printf함수 포함
//                              - 링버퍼 수신모듈 리턴인자 수정 (0or1)
//           1010-03-03 - MINV MMI to CTRL_PANEL간 통신 전용
//                              - 기존의 링버퍼를 활용하여 자기자신의 데이터가 변할蒻�
//===============================================================================================

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#include <string.h>
#include "type.h"
#include "util.h"
#include "rs232_MMItoPANEL.h"

#include "display.h"


// 통신 프로토콜

//-- RS232C 통신 전송제어 문자
#define	SOH		0x01		// 헤딩 개시(Start of Heading)
#define	STX		0x02		// 텍스트 개시 (Start of Text)
#define	ETX		0x03		// 텍스트 종료 (End of Text)
#define	EOT		0x04		// 전송 종료 (End of Transmission)
#define	ENQ		0x05		// 상태간 문의 (Enquiry)
#define	ACK		0x06		// 긍정 응답 (Acknowledge)
#define	DLE		0x10		// 각종 전송제어 (Data Link Escape)
#define	NAK		0x15		// 부정 응답 (Negative Acknowledge)
#define	SYN		0x16		// 동기 유지 (Synchronous Idle)
#define	ETB		0x17		// 블록 종료 (End of Transmission Block)		
#define	CR		0x0D		// Carriage Return	

#define	GEN_POLYNOMAL	0x8821		// CRC 젯수

//-- OP : operator
#define	OP_WRITE	0x40		// 쓰기	
#define	OP_READ		0x80		// 읽기
#define	OP_ANSWER	0xC0		// 응답

//-- <OBJ> : object 
#define	OBJ_PARAMETER_ACCESS		0x24		// 파라미터 읽기/쓰기 (R/W)
#define	OBJ_PARAMETER_BLOCK_ACCESS	0x25		// 파라미터 블록 단위로 읽기/쓰기 (R/W)
#define Comm_STATUS_Ok			0x20

//-- 데이타 타입
#define	DATA_TYPE_BYTE		0x30		// ->	바이트 
#define	DATA_TYPE_WORD		0x31		// ->	2 바이트 정수
#define	DATA_TYPE_DWORD		0x32 		// ->	4 바이트 정수
#define	DATA_TYPE_STRING	0x33		// ->	스트링	

//-- 패널 데이타 그룹 코드
#define	GROUP_CODE_PANEL_DATA	107

//-- 패널 파라미터 코드 : 콘트롤보드 -> 패널
// 0 	~ 	15		->	라인 1 데이타				
// 16 	~	31		->	라인 2 데이타
#define	PANEL_INDEX_LCD_CONTRAST_DUTY	32		// 글자 선명도 조정
#define	PANEL_INDEX_LIGHT_TIME		33		// 백라이드 지속시간	
#define	PANEL_INDEX_KEY_REPEAT_TIME	34		// 키 연속 입력 반응 시간
#define	PANEL_INDEX_LED_DATA		35
#define	PANEL_INDEX_LCD_CURSOR		36
#define	PANEL_INDEX_LCD_CMD		37

//-- 패널 -> 콘트롤보드
#define	PANEL_INDEX_KEY			100
#define	PANEL_INDEX_CONNECTION_REFLESH	101
#define PANEL_INDEX_PARAMETER		102

//-----------------------------------------------------------------------------------------


//-- 패널 연결 
#define	PANEL_CONNECTION_REFLESH_TIME	0.5





//-- 백라이트가 켜져 있는 시간
#define	DEFAULT_LIGHT_HOLD_TIME_minute	30	// 30분 후 LCD 자동 꺼짐


//-- 송수신 스택 길이
#define	RXD_STACK_LENGTH	55		// 수신단 스택의 버퍼 길이
#define	TXD_STACK_LENGTH	20		// 송신단 스택의 버퍼 길이

#define	__CRC16_LOOK_UP_TABLE_	1			// CRC Table 사용

#if __CRC16_LOOK_UP_TABLE_
	extern unsigned CRC16_Table[256];
#endif




unsigned char Auto_BR_Detection_Mode;
unsigned char Auto_BR_Detection_Cnt;

//-- Serial Data Stack  
unsigned char 	RRXD_Stack[RXD_STACK_LENGTH];
unsigned char 	RXD;				// 수신 데이타
unsigned char 	RXD_StackWritePtr;		// Serial 데이타의 수신스택 포인터
unsigned char 	RXD_StackReadPtr;		// 수신스택으로 부터 현재 읽혀져야 할 포인터
unsigned char 	RXD_Stack[RXD_STACK_LENGTH];	// 시리얼 데이타를 저장하는 수신 스택 메모리 공간

unsigned char 	TXD_StackWritePtr;		// Serial 데이타의 송신스택 포인터
unsigned char 	TXD_StackReadPtr;		// 송신 스택으로 부터 현재 읽혀져야 할 포인터
unsigned char 	TXD_Stack[TXD_STACK_LENGTH];	// 시리얼 데이타를 저장하는 송신 스택 메모리 공간
	
unsigned char 	NewFrame_StackPtr;		// 스택공간상에서 새로 검출된 프레임의 주소
unsigned char 	Frame_StackPtr;			// 스택공간상에서 전에 검출된 프레임의 주소
unsigned char 	Packet_StackPtr;			// 스택 공간에서 해당 패킷의 주소
	
unsigned char 	NewFrame_Detect;		// 새로운 프레임 검출
	
unsigned char 	NewFrame_Packet_State;		// Packet의 수신 단계 
unsigned char 	NewFrame_ByteCnt;		// 수신된 바이트 수를 나타내는 인덱스
unsigned char 	Frame_ByteCnt;
unsigned char 	Packet_ByteCnt;

unsigned int	Para_count ;

unsigned int	ms_cnt ;
unsigned int	s_reg ;

//-- 통신 수신시간 초과에 해당하는 카운트 값
unsigned int 	TimeOut_Cnt;
unsigned int 	COMM_Watchdog_Timer = 0;	// 통신 감시 타이머


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
        while(s_reg != ms_cnt) ;                //s_reg = ms_cnt일때까지 기다린다.
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

	//timer 3 인터럽트
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

	RXD = 0;			// 수신 데이타
	RXD_StackWritePtr = 0;		// Serial 데이타의 수신 스택 포인터
	RXD_StackReadPtr = 0;		// 수신 스택으로 부터 현재 읽혀져야 할 포인터

	TXD_StackWritePtr = 0 ;		// Serial 데이타의 송신 스택 포인터
	TXD_StackReadPtr = 0 ;		// 송신 스택으로 부터 현재 읽혀져야 할 포인터

	NewFrame_StackPtr = 0 ;		// 스택공간상에서 새로 검출된 프레임의 주소
	Frame_StackPtr = 0 ;		// 스택공간상에서 전에 검출된 프레임의 주소
	Packet_StackPtr = 0 ;		// 스택 공간에서 해당 패킷의 주소

	NewFrame_Detect = 0 ;		// 새로운 프레임 검출

	NewFrame_Packet_State = 0 ;	// Packet의 수신 단계
	NewFrame_ByteCnt = 0 ;		// 수신된 바이트 수를 나타내는 인덱스
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
	ms_cnt++ ;				// 범용 타이머		
	// 스택에 쓰여진 데이타를 전송한다.
	// TXD_StackReadPtr는 데이타 1개 전송때마다 1씩 증가한다.(아래 참조)
	// -> U1TXREG = TXD_Stack[TXD_StackReadPtr++]
	// TXD_StackReadPtr가 20을 초과하면 0으로 된다.

	// Transmitte_Serial_Stack에 데이타를 쓸때 마다 TXD_StackWritePtr에 1을 더한다.
	// TXD_Stack[TXD_StackWritePtr++] = TXD

	if (TXD_StackReadPtr != TXD_StackWritePtr)
	{
		if((UCSR1A & 0x20) == 0x00);// 송신버퍼가 비어 있음
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
//	시리얼 포트 인터럽터
//	송신 및 수신완료 인터럽터
//----------------------------------------
/* UART0 interrupt function */

ISR(USART1_RX_vect)	
{
	//int iU1STA_Value = 0 ;

	//iU1STA_Value = U1STA ;
	//IFS0bits.U1RXIF = 0 ;                   	//Clear interrupt flag

	// 수신 완료 => 수신 프로토콜
	// 새로운 데이타 수신

	if(PAR_UP_DN_flg == 1) 		//파라미터 UP/Down 로딩때에만 사용 한다.
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

		// 받은 데이타(RXD)를 검사한다.
		//Serial_Comm_Protocol() ;
	}
	else 				//정상모드에서 사용 한다.
	{
		RXD = UDR1 ;

		//실험 후 지울 것
		if(Para_count < RXD_STACK_LENGTH)
		{
			RRXD_Stack[Para_count++] = RXD ;
		}
		else 
		{
			return ;
		}
/*
		if (!AUTO_BR._bit.DETECTION_MODE)	//B/R 체크 모드가 아니면...
		{
			// 받은 데이타(RXD)를 검사한다.
			Serial_Comm_Protocol() ;
		}
*/
	}
	return ;
}

//------------------------------------------
void Initialize_SCI_Stack(void)
{
	// 송신 링-스택 초기화
	TXD_StackWritePtr = 0 ;		// Serial 데이타의 송신 스택 포인터
	TXD_StackReadPtr = 0 ;		// 송신 스택으로 부터 현재 읽혀져야 할 포인터

	// 수신 링-스택 초기화
	RXD_StackWritePtr = 0 ;		// Serial 데이타의 수신 스택 포인터
	RXD_StackReadPtr = 0 ;		// 수신스택으로 부터 현재 읽혀져야 할 포인터

	NewFrame_Packet_State = 0 ;	// Packet의 수신 단계
	NewFrame_ByteCnt = 0 ;		// 수신된 바이트 수
	Frame_ByteCnt = 0 ;
	Packet_ByteCnt = 0 ;

	NewFrame_Detect = 0 ;		// 새로운 프레임 검출

	NewFrame_StackPtr = 0 ;
	Frame_StackPtr = 0 ;
	Packet_StackPtr = 0 ;

	COMM_Watchdog_Timer = 0 ;	// 통신감시 타이머

	RXD_Stack[0] = UDR1 ;
}

//--------------------------------------------
#if __CRC16_LOOK_UP_TABLE_
	// 16비트 CRC (Cyclic Redundancy Check) 계산
	// CRC 업데이트
	// x : CRC
	// y : 입력 데이타

	#define	CRC16_Update(x)	CRC.Word = (CRC.Word << 8) ^ CRC16_Table[((CRC.Word >> 8) ^ x) & 0x0ff]

#else
	// CRC (Cyclic Redundancy Check) : 오류 검사
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

// Transmit Serial Stack에 데이타를 쓴다.
void Write_TransmitSerialStack(unsigned char TXD)
{
	TXD_Stack[TXD_StackWritePtr++] = TXD ;
	if (TXD_StackWritePtr >= TXD_STACK_LENGTH)	// TXD_STACK_LENGTH = 20 
	{
		TXD_StackWritePtr = 0 ;
	}
}

// Transmit Serial Stack에 데이타를 쓰고, CRC를 업데이트 한다.
void Write_TransmitSerialStack_CRC_Update(unsigned char TXD) 
{
	TXD_Stack[TXD_StackWritePtr++] = TXD ;
	if (TXD_StackWritePtr >= TXD_STACK_LENGTH)	// TXD_STACK_LENGTH = 20	
	{
		TXD_StackWritePtr = 0 ;
	}

	// 데이타가 <DLE>인 경우 중복 쓰기
	if (TXD == DLE)
	{
		TXD_Stack[TXD_StackWritePtr++] = TXD ;
		if (TXD_StackWritePtr >= TXD_STACK_LENGTH)
		{
			TXD_StackWritePtr = 0 ;
		}
	}

	// CRC-16 업데이트
	#if __CRC16_LOOK_UP_TABLE_
		CRC16_Update(TXD);
	#else
		CRC_16(TXD);
	#endif

}

// Receive Serial Stack으로 부터 현재 포인터의 데이타를 읽는다.
// RXD_StackOffset는 수신된 데이타의 숫자(?)를 가리킨다.
unsigned char Read_ReceiveSerialStack(unsigned char RXD_StackOffset)
{

	// 스택포인터는 새로운 프레임의 스택 시작포인터에 더해진다.
	// if(RXD==OP), Packet_StackPtr = Frame_StackPtr = NewFrame_StackPtr = RXD_StackWritePtr
	RXD_StackReadPtr = Packet_StackPtr + RXD_StackOffset ;

	if (RXD_StackReadPtr >= RXD_STACK_LENGTH)		// 55
	{
		RXD_StackReadPtr -= RXD_STACK_LENGTH ;		// 링 스택
	}

	return	RXD_Stack[RXD_StackReadPtr];
}


// 통신 프로토콜
// <DLE><STX><OP><OBJ><DATA><DLE><ETX><CRC>
// <OP><OBJ>는 제어문자를 포함하지 않는다. 따라서 항상 0x20 이상의 값을 갖는다.
// <CRC>는 16비트 값을 가지며, <OP><OBJ><DATA>를 계산에 포함하며, 하위 바이트가 앞선다.
// <DATA>에 <DLE>가 포함될 경우 <DLE>를 중첩시키고 하나의 <DLE>로 인식한다.
// <OP><OBJ><DATA><CRC>만 스택에 저장되고, ByteIndex에 포함된다.


// State	Byte Index
//----------------------------------------------------------------------
//	0	DLE 		<----	Data Line Escape
//	1	STX		<----	Start of Text
//	2	OP	0	<----	operator		: Stack Ptr=0
//	3	OBJ	1	<----	object
//		~
//	4	DATA		<----	max. 256 bytes
//		~
//	5      	DLE		<---	Data Line Escape	: Byte Index에서 제외
//	6	ETX		<----	End of Text		: Byte Index에서 제외
//	7	CRC[L]		<----	CRC-16 하위 바이트
//	8	CRC[H]		<----	CRC-16 상위 바이트
//----------------------------------------------------------------------

// 외부에서 데이타 수신되고 B/R체크 모드가 아니면 실행된다.
// 외부에서 수신된 데이타(=RXD)는 RXD_STACK[]에 저장 된다.

void Serial_Comm_Protocol()
{
	switch (NewFrame_Packet_State)
	{
		// <DLE>
		case 0 : if (RXD == DLE)
			 {
				// if (RXD == DLE) 이면 NewFrame_Packet_State++하여 다음에 void Serial_Comm_Protocol() 함수가
				// 실행 시 case 1: 을 실행 하도록 한다.
				// 이와 같이 조건이 맞으면 NewFrame_Packet_State++하여 프로토콜은 이어서
				// 체크하도록 한다.

				NewFrame_Packet_State++ ;
				Auto_BR_Detection_Cnt = 0 ;	// BR 자동 검출 완료
			 }
			 else if (RXD == CR)
			 {
				Write_TransmitSerialStack(ENQ) ;
			 }
			 else if (RXD != CR)
			 {
				Auto_BR_Detection_Cnt++ ;		// 카운터를 증가하고 5가 넘으면 B/R을 다시 체크 한다. 
			 }
			break;
		// <STX>
		case 1 : if (RXD == STX)
			 {
			 	NewFrame_Packet_State++ ;
			 	Auto_BR_Detection_Cnt = 0 ;	// BR 자동 검출 완료
			 }
			 else
			 {
				NewFrame_Packet_State = 0 ;		// <DLE><STX>가 아니면 처음으로 되돌아감
			 }
			 break ;
		
		// <OP> : operator
		// operator는 0x20 이상의 값을 갖도록 설계한다.
		// 데이타를 어떻게 처리할 지 결정 한다.

		//    OP Code  	  OP		    설    명
		//------------------------------------------------------
		//	40h	WRITE		데이터 쓰기
		//	80h	READ		데이터 읽기
		//	C0h	ANSWER		요청 데이터에 대한 응답

		case 2 : if (RXD >= 0x20)
			 {
			 	NewFrame_Packet_State++ ;

			 	// 새로운 프레임의 시작이 검출되면 스택의 시작 포인터를 저장한다.
			 	// <OP> <OBJ> <DATA> <CRC>

			 	NewFrame_StackPtr = RXD_StackWritePtr ;
			 	RXD_Stack[RXD_StackWritePtr++] = RXD ;	//수신된 데이타를 수신 스택에 저장한다.
			 }
			 else
			 {
				NewFrame_Packet_State = 0 ;		// 처음으로 되돌아감
			 }
			 break;

		// <OBJ> : object
		// object는 0x20이상의 값을 갖도록 설계한다.

		//   OBJ Code	     OBJ			설명
		//-----------------------------------------------------------------
		//	20h	Flash Program		단일/복수 단위로 프로그램
		//	21h	Flash Block Program	블록 단위로 프로그램
		//	22h	Flash Erase		플래쉬 메모리 지우기	
		//	24h	Parameter		단일/복수 파라미터/변수
		//	25h	Parameter Block Access	블록 단위의 파라미터/변수
		//	A0h	STATUS RETURN		수신 상태 
							
		case 3 : if (RXD>=0x20)
			 {
			 	NewFrame_Packet_State++;
			 	RXD_Stack[RXD_StackWritePtr++] = RXD;	//수신된 데이타를 수신 스택에 저장한다.
			 }
			 else
			 {
			 	NewFrame_Packet_State = 0;		// 처음으로 되돌아감
			 }
			 break;

		// <DATA> : 데이타
		// <DLE> : 제어문자 인식 (다음 문자는 제어문자 또는 데이타로 사용될 <DLE>가 온다.)

		// 아래 부터는 데이타를 저장하게 되는데 <DLE>가 올때까지 저장하고 <DLE>가 오면
		// case5로 바꾸어 다음에 수신된 제어문자가 무엇인지 체크한다.
 
		case 4 : if (RXD==DLE)
			 {
				NewFrame_Packet_State = 5 ;		// 제어문자 인식 State로 이동
			 }
			 else
			 {
				RXD_Stack[RXD_StackWritePtr++] = RXD ;	// State는 변하지 않는다.
			 }
				
			 break;

		// 제어문자 인식
		// <DLE><STX>	:	수신 오류가 발생했거나, 송신단으로 데이타가 끊겨진 후 다시 송신되는 경우
		// <DLE><ETX>	:	데이타의 종료
		// <DLE><DLE>	:	<DLE>중첩은 하나의 <DLE>를 데이타로 인식
		// <DLE><etc.>	:	오류 (정의되지 않는 데이타 수신)
		case 5 : if (RXD == STX)
			 {
			 	NewFrame_Packet_State = 2 ;		// <DLE><STX> State로 이동
			 }
			 else if (RXD == ETX)
			 {
				NewFrame_Packet_State = 6 ;		// <DLE><ETX> State로 이동			
			 }
			 else if (RXD == DLE)
			 {
				NewFrame_Packet_State = 4 ;		// <DATA> State로 이동
				RXD_Stack[RXD_StackWritePtr++] = RXD ;	// <DLE>중첩은 하나의 <DLE>를 데이타로 인식	
			 }
			 else
			 {
				NewFrame_Packet_State = 0;		// 데이타 오류
			 }
			 break;

		// <CRC[MSB]>		
		case 6 : NewFrame_Packet_State = 7 ;
			 RXD_Stack[RXD_StackWritePtr++] = RXD ;
			 break;

		// <CRC[LSB]>								
		case 7 : NewFrame_Packet_State = 0 ;			// 처음으로 돌아감

			//위 case 2:에서 if(RXD==OP)이면 NewFrame_StackPtr = RXD_StackWritePtr한다.
			 NewFrame_ByteCnt = RXD_StackWritePtr - NewFrame_StackPtr + 1 ;	// <OP> <OBJ> <DATA> <CRC>
			 if (NewFrame_ByteCnt <= 0)
			 {
				NewFrame_ByteCnt += RXD_STACK_LENGTH;	//RXD_STACK_LENGTH = 55
			 }

			 RXD_Stack[RXD_StackWritePtr++]= RXD ;	

			 NewFrame_Detect = 1;				// => 프레임의 끝 : 새로운 프레임 검출

			//위 case 2:에서 if(RXD==OP)이면 NewFrame_StackPtr = RXD_StackWritePtr한다.
			 Frame_StackPtr = NewFrame_StackPtr;		// 스택 주소 저장

			//NewFrame_ByteCnt = RXD_StackWritePtr - NewFrame_StackPtr + 1 ;
			 Frame_ByteCnt = NewFrame_ByteCnt;		// 스택 길이 저장

			 break;
	}	//end of switch()

	// 링 스택 오버플로우 방지
	if (RXD_StackWritePtr >= RXD_STACK_LENGTH)			//RXD_STACK_LENGTH = 55
	{
		RXD_StackWritePtr = 0 ;
	}
}

//----------------------------------------
// main()루프에서 call한다.
//----------------------------------------
void Serial_Comm_Service() 
{	
	unsigned char  Err_Status;
	int 	Stack_Index;
	int 	RXD_StackReadPtr;

	// "ENQ" 전송 -> "CR" 수신 
	// 초기 시 AUTO_BR._bit.DETECTION_MODE = 1임
	// 정상 연결되면 AUTO_BR._bit.DETECTION_MODE = 0이 되어 아래 부분은 실행하지 않는다.

	// B/R 체크 .....	

	// 자동 B/R 검출 모드인 경우

	if (Auto_BR_Detection_Mode)
	{
		Xmitt_Delay_Cnt++ ;

		//아래 루프는 확인해본 결과 실행되지 않는다. 즉, Xmitt_Delay_Cnt가 10을 넘지 않는다.
		if (Xmitt_Delay_Cnt > 10)
		{
			Xmitt_Delay_Cnt = 0 ;
			//asm("reset") ;				//자동 reset을 한다.
		}

		RXD = NULL ;
		Write_TransmitSerialStack(ENQ) ;		// ENQ : 0x05, 상태 문의(Enquiry)

		s_reg = ms_cnt + 1 ;				// 슬래이브 응답 대기 시간
		wait() ;

		if (RXD == CR)					// CR :0x0D, Carriage Return
		{

			Auto_BR_Detection_Cnt++ ;
		}
		else if(Auto_BR_Detection_Cnt> 0)
		{
			Auto_BR_Detection_Cnt-- ;
		}

		// 5 회이상 "ENQ" -> "CR"면 통신이 연결된 것으로 판단
		if (Auto_BR_Detection_Cnt > 5)
		{
			Auto_BR_Detection_Mode= 0 ;
			Auto_BR_Detection_Cnt = 0 ;
			Initialize_SCI_Stack() ;
		}

		NewFrame_Detect = 0;		// 자동 B/R 검출 모드에서는 전 단계의 모든 패킷은 무시된다.
						// 0이면 정상 통신 모드를 실행하지 못한다.
		COMM_Watchdog_Timer = 0;	// 통신감시 타이머
						// Timer4에서 1씩 증가 한다.
	}

	// 자동 B/R 검출 모드가 아닌 경우 = 정상 모드이면
	else 
	{
		// 연속하여 5번 이상의 잘못된 데이타가 수신되는 경우
		// 통신감시 시간초과 : 특정시간 (시도 횟수 : 5) 이상 동안 아무런 응답이 없을 없을 경우 
		// -> 자동 B/R 검출 모드

		if (Auto_BR_Detection_Cnt > 5)
		{

			Auto_BR_Detection_Mode = 1 ;
			Auto_BR_Detection_Cnt = 0 ;
			NewFrame_Packet_State = 0 ;
		}

		// 통신 오류 : 시간 초과 -> 통신 오류 -> 보오-레이트 재설정
		// TimeOut_Cnt : 5000
		// COMM_Watchdog_Timer는 200us마다 증가 -> 2000us x 5000 = 10sec
		// 즉 10sec가 넘어가면 AUTO_BR._bit.DETECTION_MODE를 1로 하여 Baud rate를 다시 체크 한다.
		// 따라서 10초가 넘어가지 전에 0으로 해주어야 한다.
	
		if (COMM_Watchdog_Timer > TimeOut_Cnt)	
		{
			Auto_BR_Detection_Mode= 1 ;
			Auto_BR_Detection_Cnt = 0 ;
			NewFrame_Packet_State = 0 ;
		}
	}

	//-------------------------------------
	// 정상 통신 모드

	// 데이타 프레임을 갖는 패킷만 처리
	// CRC까지 데이타를 모두 수신되면 NewFrame_Detect=1이 된다.
	if (NewFrame_Detect)
	{
		Err_Status = 0 ;

		// 동시에 들어오는 여러 프레임의 스택 데이타가 중복되는 방지 => 스택 데이타 저장 
		// 데이타 프레임의 스택포인터

		// if(RXD==OP)이면 Packet_StackPtr = Frame_StackPtr = NewFrame_StackPtr = RXD_StackWritePtr한다.
		Packet_StackPtr = Frame_StackPtr ;
		
		// CRC까지 데이타를 모두 수신 하면, 
		// Frame_ByteCnt = NewFrame_ByteCnt = RXD_StackWritePtr - NewFrame_StackPtr + 1 ;
		// 즉, Packet_ByteCnt는 수신된 데이타의 숫자(?)를 가리킨다.
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
		//	5      	DLE		<---	Data Line Escape	: Byte Index에서 제외
		//	6	ETX		<----	End of Text		: Byte Index에서 제외
		//	7	CRC[L]		<----	CRC-16 하위 바이트
		//	8	CRC[H]		<----	CRC-16 상위 바이트
		//----------------------------------------------------------------------

		// 오류 검사 : CRC16
		CRC.Word=0;								// CRC 초기화
		for (Stack_Index=0 ; Stack_Index<(Packet_ByteCnt-2) ; Stack_Index++)	// -2 : <CRC> 2바이트는 제외
		{
			RXD_StackReadPtr = Packet_StackPtr + Stack_Index;
			if (RXD_StackReadPtr >= RXD_STACK_LENGTH){			// 55
				RXD_StackReadPtr -= RXD_STACK_LENGTH ;			// 링 스택
			}			
			// CRC-16 업데이트	
			#if __CRC16_LOOK_UP_TABLE_					// 1
				CRC16_Update(RXD_Stack[RXD_StackReadPtr]);

			#else	
				CRC_16(RXD_Stack[RXD_StackReadPtr]);
			#endif
		}

		// Cyclic Redundancy Check (CRC[H:L])
		// MSB가 먼저 수신됨
		// Packet_ByteCnt는 수신된 데이타의 숫자(?)를 가리킨다.

		// 현재 포인터의 데이타를 읽는다.
		if ((CRC.Byte.b1 == Read_ReceiveSerialStack(Packet_ByteCnt - 2))
			&& (CRC.Byte.b0 == Read_ReceiveSerialStack(Packet_ByteCnt - 1)))
		{
			// 코멘드 읽기
			//	Bit[7:6]
			// 	0x40	:	WRITE
			//	0x80	:	READ
			//	0xC0	:	ANSWER
			Packet_Head.OP=Read_ReceiveSerialStack(0);	// operator
			Packet_Head.OBJ=Read_ReceiveSerialStack(1);	// object

			if (Packet_Head.OP==OP_WRITE)
			{
				// OP_ANSWER의 패킷이 수신 되면 => 감시 타이머 ->  0
				// Packet_Head.OBJ의 값은 OBJ_PARAMETER_BLOCK_ACCESS(0x25)임
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

		// 수신된 프레임이 처리됨.
		// "NewFrame_Detect"이 0이 아닌 경우 다음 프레임을 연속적으로 처리한다. 
		NewFrame_Detect = 0 ;
		COMM_Watchdog_Timer = 0 ;
	}					//end of if(NewFrame_Detect)	
}	

#if 0
//---------------------------------------------
// 파라미터 업데이트
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
				// LCD 글자 선명도 조정 
				// PANEL_INDEX_LCD_CONTRAST_DUTY = 32

				case PANEL_INDEX_LCD_CONTRAST_DUTY :
						if (_data <= 10) {
							Contrast_Cnt_Duty = _data ;
						}
						else {
							Contrast_Cnt_Duty = 10 ;
						}
				     		break ;	

				// 백라이트 콘트롤 : 백라이트가 연속으로 켜져 있는 시간
				// PANEL_INDEX_LIGHT_TIME = 33

				case PANEL_INDEX_LIGHT_TIME :
						LIGHT_HOLD_TIME_minute = _data ;
						break;

				// 키 연속 입력 반응 시간 (x100) : 0.25 ~ 2.0초 -> 25 ~ 200
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
				// LED 출력
				// PANEL_INDEX_LED_DATA = 35
				case PANEL_INDEX_LED_DATA :
						LED.byte = _data;
						break;
				// 커서위치
				// PANEL_INDEX_LCD_CURSOR = 36
				case PANEL_INDEX_LCD_CURSOR :
						// 커서의 위치 설정
						if (_data < 16) {
							Cursor(0, _data);
						}
						else if (_data < 32) {
							Cursor(1, (_data - 16));
						}		
						break;
				// LCD 출력 코멘드
				// PANEL_INDEX_LCD_CMD = 37
				case PANEL_INDEX_LCD_CMD :
						LCD_Cmd(_data);
						break;
			}
		}
	}
}
#endif

// 파라미터 쓰고/읽기
//----------------------------------------------------------------------------------------
// OBJ : "PARAMETER_ACCESS" : 1 바이트형 정수
// 총 바이트 수	= Packet Head (2) : 	<OP> <OBJ>
//		  Data Head (2)   : 	<No. of Data> <Data Type>
//		  Datum (3 x N)	  :	<ADDR.1[MSB]> ~ <DATA.N>
//		  CRC (2)	  :	<CRC[MSB]> <CRC[LSB]>
//		  =>	(3 x N) + 6
//				
// Byte Index	데이타
//	0	<OP>			<----	WRITE
//	1	<OBJ>			<----	PARAMETER ACCESS
//	2	<No. of Data :N>	<---- 	데이타의 총 갯수
//	3	<DATA TYPE>		<----	데이타 타입	
//	4	ADDR.1[MSB]		<----	첫번째 데이타 주소의 파라미터 그룹 코드
//	5	ADDR.1[LSB]
//	6	DATA.1			<----	첫번째 데이타
//	~					~
//	~					~
//(3 x N) + 1	ADDR.N[MSB]		<----	N 번째 데이타 주소의 파라미터 그룹 코드
//(3 x N) + 2	ADDR.N[LSB]
//(3 x N) + 3	DATA.N			<----	N 번째 데이타
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
	} _code;					// 코드[H:L]

	// 데이타 타입은 바이트 형
	// 데이타의 수가 일치하는지 확인
	if (Packet_ByteCnt == (3*Read_ReceiveSerialStack(2)+6))
	{
		// 파라미터 쓰기	: 2 바이트형 정수	
		for (StackIndex=4;StackIndex<(Packet_ByteCnt-2);)	// <ADDR.1[MSB]> ~ <DATA.N>
		{
			// 어드레스
			_code.b.b1 = Read_ReceiveSerialStack(StackIndex++);	// 파라미터 그룹
			_code.b.b0 = Read_ReceiveSerialStack(StackIndex++);	// 파라미터 인덱스		
			_data = Read_ReceiveSerialStack(StackIndex++);		// 파라미터 값

			// 파라미터 업데이트
			// _code.b.b0 < 32일 경우 _data을 string[]에 저장한다.
			// _code.b.b1 -> GROUP, _code.b.b0 -> INDEX, _data -> DATA
			//Parameter_Update(_code.b.b1,_code.b.b0,_data) ;
		}
	}
}

//--------------------------------------------------------------------------------------
// OBJ : "PARAMETER_BLOCK_ACCESS" : 1 바이트형 정수
// 총 바이트 수 = Packet Head (2) : 	<OP> <OBJ>
//		  Data Head (2)   : 	<No. of Data> <DATA TYPE>
//		  Datum (N+2)	  :	<ADDR[LSB]><ADDR[MSB]> ~ <DATA.N>
//		  CRC (2)	  :	<CRC[MSB]> <CRC[LSB]>
//					=>	N + 8
//				
// Byte Index		데이타
//	0		<OP>			<----	WRITE
//	1		<OBJ>			<----	PARAMETER BLOCK ACCESS
//	2		<No. of Data :N>	<---- 	데이타의 총 갯수
//	3		<DATA TYPE>
//	4		ADDR[MSB]		<----	첫번째 데이타 주소의 파라미터 그룹 코드
//	5		ADDR[LSB]
//	6		DATA.1			<----	첫번째 데이타
//	~		~
//	~		~
//	N + 5		DATA.N			<----	N 번째 데이타
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
									// 코드[H:L]
	// 데이타 타입은 바이트 형
	// 데이타의 수가 일치하는지 확인

	if (Packet_ByteCnt == (Read_ReceiveSerialStack(2)+8))
	{
		_code.b.b1 = Read_ReceiveSerialStack(4);		// 파라미터 코드
		_code.b.b0 = Read_ReceiveSerialStack(5);

		// 파라미터 쓰기	: 2 바이트형 정수	
		for (StackIndex = 6 ; StackIndex < (Packet_ByteCnt-2) ; )			
		{	
			_data = Read_ReceiveSerialStack(StackIndex++);	// 파라미터 값
			
			// 파라미터 업데이트
			// _code.b.b0 < 32일 경우 _data을 string[]에 저장한다.
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

	Write_TransmitSerialStack_CRC_Update(OP);		// 데이타 쓰기
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

// 데이타 업로드 : 1개씩 전송
// State	TXD
//----------------------------------------------------------------------
//	0	DLE 			<----	Data Line Escape
//	1	STX			<----	Start of Text 
//	2	OP_WRITE		<----	operator
//	3	OBJ_PARAMETER_ACCESS	<----	object
//	4	1			<----	No. of Data
//	4	DATA TYPE		<----	Data Type : byte, word, double word
//	4	GROUP			<----	파라미터 그룹		
//	4	INDEX			<----	그룹내 인덱스
//	4	DATA[MSB]		<----	데이타 : MSB						
//	4	DATA[LSB]		<----	데이타 : LSB						
//	5      	DLE			<---	Data Line Escape	: Byte Index에서 제외
//	6	ETX			<----	End of Text		: Byte Index에서 제외
//	7	CRC[MSB]		<----	CRC-16 : MSB
//	8	CRC[LSB]		<----	CRC-16 : LSB
//----------------------------------------------------------------------

// 파라미터 쓰기 : 마스터(패널) -> 슬래이브 (콘트롤 보드)

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
	//Write_TransmitSerialStack_CRC_Update(1);		// 데이타 갯수 : 1

	Write_TransmitSerialStack_CRC_Update(DATA_TYPE_WORD);	// 데이타 타입: word

	Write_TransmitSerialStack_CRC_Update(GROUP);		// 그룹번호
	Write_TransmitSerialStack_CRC_Update(INDEX);		// 인덱스

	_data.Word=DATA ;					// 데이타
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
		// Control Board에 Write가 되지 않을때 반복한다
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
//	Write_TransmitSerialStack_CRC_Update(1);		// 데이타 갯수 : 1

	Write_TransmitSerialStack_CRC_Update(Xmitt_Key_Num);

	Write_TransmitSerialStack_CRC_Update(DATA_TYPE_WORD);	// 데이타 타입: word

	Write_TransmitSerialStack_CRC_Update(GROUP);		// 그룹번호
	Write_TransmitSerialStack_CRC_Update(INDEX);		// 인덱스
	
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

	if(GROUP != 16) 		// Group 16이 아니면
	{
		if((RRXD_Stack[2] == OP_ANSWER) && (RRXD_Stack[3] == OBJ_PARAMETER_ACCESS) && (RRXD_Stack[6] == GROUP) && (RRXD_Stack[7] == INDEX))
		{
			err_cnt = 0 ;					//에러 카운트 reset

			if(i != 16) {
				_data.Byte.b0 = RRXD_Stack[9] ;	//하위 바이트
				_data.Byte.b1 = RRXD_Stack[8] ;	//상위바이트
			}
			else {
				_data.Byte.b0 = RRXD_Stack[10] ;	//하위 바이트
				_data.Byte.b1 = RRXD_Stack[9] ;	//상위바이트
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
	else 				// Group 16이면
	{
		if((RRXD_Stack[2] == OP_ANSWER) && (RRXD_Stack[3] == OBJ_PARAMETER_ACCESS) && (RRXD_Stack[6] == GROUP) && (RRXD_Stack[7] == INDEX))
		{
			err_cnt = 0 ;					//에러 카운트 reset
			if(i != 16) {
				_data.Byte.b0 = RRXD_Stack[10] ;	//하위 바이트
				_data.Byte.b1 = RRXD_Stack[9] ;	//상위바이트
			}
			else {
				_data.Byte.b0 = RRXD_Stack[11] ;	//하위 바이트
				_data.Byte.b1 = RRXD_Stack[10] ;	//상위바이트
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



