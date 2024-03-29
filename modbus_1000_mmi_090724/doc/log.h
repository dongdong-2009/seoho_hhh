/**
    @mainpage  Project : MODUS-1000_MMI
    @section intro 소개
        - 소개 : datfile_t는 텍스트 파일을 이용하여 문자열, 정수, 실수, Boolean 값을
                 구분자로 구별하면서 읽거나 쓸 수 있도로 처리한다.
        - 설명 : main() 함수에는 datfile_t를 이용하여 dat 파일 내용을 출력하는 예제를 포함
 
 //-----------------------------------------------------------------------------------------------------
@section HISTORY 히스토리
090223
	- 폴더(modbusw)의 소스를 EPU-100보드에 포팅함
	- 소스분석은 아직 못했고 동작에 이상은 없어보임
	- 베이스소스의 출처는 구글에서 검색함 폴더안에 PDF정보가 있음


090317
  - PCB입고 : 4대 조립

090320
  기본동작 테스트:
  - 기존 MODUS프로토콜 소스를 포팅하여  LED점등 테스트

  - 드라이버 충돌이 발생하여  AVR-JTAG를 인식하지 못하였음
   - 프로그램을 새로 설치 하여 해결

090324
	- UART동작 시험
		- PC와 통신하여 정상동작 하는지 확인함
		- modbus프로토콜 소스가 정상동작하는지 확인함
   		- 통신채널 4채널을 차례로 점검
     	-	통신쪽은 이상없음
		- LCD장착
		  - 개발용으로 만든보드는 뒷면으로 모든 부품을 조립하여
		   LCD핀이 거꾸로 되어버림
		  - 핀을 납땜으로 다시연결
		 	- 딥핑후 패드가 떨어져나가는 현상이 있음
		   - 주의해야함

090331
	- MMI전용 SMPS연결 시험
  	- 별문제없이 동작함
	- LCD를 연결신호선들이 끊어짐
  - 프로그램 개발도 불편하고 보드검증도 쉽지않아
   새로 보드를 조립함
   	- MCU를 다시 붙이고 기본동작 테스트
    :이상없음
	- LCD쪽 타이밍 관련한 부분의 프로그램을 수정, 
   불필요한 요소를 제거하고 최적화함

090401
	 - LCD SHIFT 스크롤 함수 구현
	  - 메뉴화면 프로그램 작성할 때 아랫줄을 스크롤 시킴 
	 - GPIO테스트 LED와 부저가 연결된 포트의 동작을 확인함
	 - 진행사항
	  - uart동작 확인 :  이상없음
	 	- modbus 소스포팅 : 동작함 컨트롤 보드와 연동하여 튜닝하여야 함
	  - LCD 기능 : 이상없음
	  - GPIO : 동작확인함
	 - 예정사항
	 	- adc키 점검
	  - menu 프로그램
	    -dsp보드 modbus 프로토콜 포팅
	
090402
	 - ADC키 동작 점검
	  - ADC초기화하는 과정에서 레지스터 설정에 다서 
	   난해했던 부분이 있었음
	   - xadc5와 xadc4~0이 나누어져있음
	
	 - 현재 10비트 중 하위 2비트를 사용하지 않고
	   키동작확인함
	   - default 0xE0, 0x80, 0xC0, 0xAB, 0xC0, 0xD6, 0xDC에서
	   키가 동작함
	
	 - LCD에 한글 띄우기 진행중
	  : 한글폰트 삽입
	 
	 - UART 프로그램 최적화
	
	
	 - MENU 프로그램 진행
	  ; 소스 포팅중
	   
	090403
	- KEY동작을 이용하여
	  MENU 네비게이션 동작을 구현함
		 - 좌 : 변수/10
	   - 우 : 변수*10
	   - 위 : 변수 +1
	   - 아래 : 변수 -1
	
	연산된 변수는 메뉴핸들링을 위한 핸들이 됨  
	
	
	090414
	 - 데이터 검증은 진행 중 
	  - DSP 수신쪽에 패킷 마지막 데이터가 수신되지 않는 현상
	  - MMI쪽 485이네이블 신호가 데이터보다 일찍 떨어짐
	    - 인터럽트로 처리후 dsp쪽에서 데이터가 잘 전달받음을 확인함
	  - MODUS서버프로그램에서 패킷fault시 오작동하는 현상 소스수정
	  - MMI쪽 485 TX 이네이블시 RXD단자가 LOW로 떨어지는 현상
	   - AVR은 이것을 0x00으로 인식함
	  - 485 이네이블 신호시 수신포트를 disable해줌으로 오작동을 막음
	
	  - DSP에서 데이터수신후 응답 송신할 때 10us 딜레이를 줌
	   - 송수신데이터의 간격을 벌림 
	
	090415(modbus_1000_mmi_090415)
	 - MODBUS프로토콜 데이터 검증
	  - 실제 사용되는 영역을 확장하고 소스 정리함
	
	 - MENU화면 구성 
	  - LCD제어 코드를 좀더 명확하게 수정함
	  - 스크롤 기능이 정상작동 함
	
	 - MENU핸들러(변수이름)를 통하여 메뉴화면 이동 
	  - adc키 대신 UART키로 대체
	
	- MENU화면 구성 
	  - 1. 운전상태
	  - 2. 수동절체
	  - 3. 이력보기
	  - 4. 파라메타 설정
	\n;운전상태보기 전압전류값을 통신으로부터 읽어와
	 LCD에 출력하는 프로그램
	
	090415(modbus_1000_mmi_090416)
	 - 프로젝트 백업
	
	
	090429(modbus_1000_mmi_090423)
	- 메모리맵(main_data_array): 메모리정리
	- 3번 메뉴와 4번 메뉴 위치 바꿈
	
	
	090506(modbus_1000_mmi_090506)
	- 히스토리, 패스워드, 날짜 변경
	- 이벤트 입력받아 출력

% release	
 //-----------------------------------------------------------------------------------------------------	
\n090527(modbus_1000_mmi_090527)
	 - MODBUS관련된 프로그램 수정
	   DSP
	   - modbus -> data_array -> 제어변수 -> data_array ->modbus
	    와 같이 데이터가 전달되어야 하는데 제어변수에서 data_array로
	    복사가 먼저 이루어져 수신데이터가 지워지는현상
	   	 - void modbusStoreData(QUERY* query ) 함수에 
	   read_com_buf();를 삽입함으로 해결
	   - Bound_min_max(tmp, -32767, 32767)에서 tmp값이
	    음수일 때 0값이 되어버림
	@code
	/*
	  // Vin_offset
	  tmp = main_data_array[70];
	  Vin_offset = (Bound_min_max(tmp, -32767, 32767));
	  
	  // Vin_gain
	  tmp = main_data_array[71];
	  Vin_gain = (Bound_min_max(tmp, -32767, 32767));
	  
	  // Ain_offset
	  tmp = main_data_array[72];
	  Ain_offset = (Bound_min_max(tmp, -32767, 32767));
	  
	  // Ain_gain
	  tmp = main_data_array[73];
	  Ain_gain = (Bound_min_max(tmp, -32767, 32767));
	  
	  // Vout_offset
	  tmp = main_data_array[74];
	  Vout_offset = (Bound_min_max(tmp, -32767, 32767));
	  
	  // Vout_gain
	  tmp = main_data_array[75];
	  Vout_gain = (Bound_min_max(tmp, -32767, 32767));
	  
	  // Aout_offset
	  tmp = main_data_array[76];
	  Aout_offset = (Bound_min_max(tmp, -32767, 32767));
	  
	  // Aout_gain
	  tmp = main_data_array[77];
	  Aout_gain = (Bound_min_max(tmp, -32767, 32767));
	  
	  // Vbyp_offset
	  tmp = main_data_array[78];
	  Vbyp_offset = (Bound_min_max(tmp, -32767, 32767));
	  
	  // Vbyp_gain
	  tmp = main_data_array[79];
	  Vbyp_gain = (Bound_min_max(tmp, -32767, 32767));
	  
	  // Vbat_offset
	  tmp = main_data_array[80];
	  Vbat_offset = (Bound_min_max(tmp, -32767, 32767));
	  
	  // Vbat_gain
	  tmp = main_data_array[81];
	  Vbat_gain = (Bound_min_max(tmp, -32767, 32767));
	  
	  // Abat_offset
	  tmp = main_data_array[82];
	  Abat_offset = (Bound_min_max(tmp, -32767, 32767));
	  
	  // Abat_gain
	  tmp = main_data_array[83];
	  Abat_gain = (Bound_min_max(tmp, -32767, 32767));
	  
	  // Vdc_offset
	  tmp = main_data_array[84];
	  Vdc_offset = (Bound_min_max(tmp, -32767, 32767));
	  
	  // Vdc_gain
	  tmp = main_data_array[85];
	  Vdc_gain = (Bound_min_max(tmp, -32767, 32767));
	  */
	
	  // Vin_offset
	  Vin_offset = main_data_array[70];
	  // Vin_gain
	  Vin_gain = main_data_array[71];
	  // Ain_offset
	  Ain_offset = main_data_array[72];
	  // Ain_gain
	  Ain_gain = main_data_array[73];
	  // Vout_offset
	  Vout_offset = main_data_array[74];
	  // Vout_gain
	  Vout_gain = main_data_array[75];
	  // Aout_offset
	  Aout_offset = main_data_array[76];
	  // Aout_gain
	  Aout_gain = main_data_array[77];
	  // Vbyp_offset
	  Vbyp_offset = main_data_array[78];
	  // Vbyp_gain
	  Vbyp_gain = main_data_array[79];
	  // Vbat_offset
	  Vbat_offset = main_data_array[80];
	  // Vbat_gain
	  Vbat_gain = main_data_array[81];
	  // Abat_offset
	  Abat_offset = main_data_array[82];
	  // Abat_gain
	  Abat_gain = main_data_array[83];
	  // Vdc_offset
	  Vdc_offset = main_data_array[84];
	  // Vdc_gain
	  Vdc_gain = main_data_array[85];
	
	@endcode
	
	   MMI
	   - makeQuery1함수에 버그발견/수정
	    인수 전달부분이 상수로 고정되어있어 특정번지에 데이터가
	    잘못전달되는 현상
	
	   - 초기 메뉴에서 10초 뒤에 계통도 화면으로 전환
	@code
	void SYS_1(void)
	{
		static unsigned char flag=0;
	
		if(EventFlagC)
		{
			if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
			else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
			else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
			else if(KeyState.KeyValue == ENTER)naviMENU = naviMENU * 10;
			else if(KeyState.KeyValue == (ESC  & UP & DN & ENTER))
			{
				naviMENU = 0xFFFFFFFF;
			}
			
			if(KeyState.KeyValue != 0xFF)EventTimeC=0;
		}
		else 
		{
			if(KeyState.KeyValue != 0xFF)EventFlagC=1;
			if(naviMENU!=old_naveMENU)EventFlagC=1;
		}
	
		if((!EventFlagC)&&(flag))RefreshFlag=1;// event edge
		else if((EventFlagC)&&(!flag))RefreshFlag=1;// event edge
		flag = EventFlagC;
	}
	
	void PAGE_1(void)
	{
		if(EventFlagC)
		{
			GLCD_BufClear();	
			GLCD_print0508(5, 0,"--MODUS-1000 UPS--");
			GLCD_print0508(2, 2, "1.SYSTEM MEASURE");
			GLCD_print0508(2, 3, "2.SYSTEM OPERATION");
			GLCD_print0508(2, 4, "3.SYSTEM HISTORY");
			GLCD_print0508(2, 5, "4.PARAMETER SETTING");
			
			GLCD_inversion(12,2,112);
		}
		else
		{
			GLCD_loaddiagram();
		}
	}
	@endcode
	
	   - 오프셋/게인 변경메뉴(4400..) 시작위치(posInpage)를
	     1의 자리로 변경함
	@code
			else if(KeyState.KeyValue == RIGHT){	posInpage=5;RefreshFlag=1;}
			else if(KeyState.KeyValue == LIFT)	{	posInpage=0;RefreshFlag=1;}
			else if(KeyState.KeyValue == ENTER){	posInpage=5;RefreshFlag=1;}
	@endcode
	
	
	090527(modbus_1000_mmi_090610)
	- 메모리맵과 아래 변수가 일치하지 않았음
	   Bat_Float_Volt
	   Bat_Equal_volt
	   Bat_Equal_Curr
	   메모리 맵을 확인하고 수정하여 적용함
	
	
	- 오프셋/게인의 값을 조정할 때  통신끊김 화면이 뜨는 현상
	@code
	else if(rc_poll==(BYTE)CMPLT_NO_HOPE)
	{
	start_time=0;//get_milliseconds();
	mseconds = 0;
	communication_fault = 0;
			communication_fault_cnt = 0;
	modbus_state=0;
	clr_rx() ;
	Poll_flag=0;
	}
	@endcode
	경우의 수를 두어 해결함
	3가지 경우의 수가 있음
	cmplt : 정상상태
	cmplt_hope : 대기상태 (통신이 이루어지지 않으면 타임아웃발생)
	CMPLT_NO_HOPE : 전송실패(통신이 끊긴상태는 아님)
	
	
	- 알람/LED 동작 수정
	- 히스토리 발생순서 조정
 @code
	//===================================================
	//Vin OUT
			if((main_data_array[42] & 0x0008) && (!(event_array[2]&0x0008))){history_update(33);BuzzerFlag=1;AlarmLED=1;}				//
			else if(!(main_data_array[42] & 0x0008) && ((event_array[2]&0x0008))){AlarmLED=0;GLCD_BuzzerOnOff(0);}	
	
			if((main_data_array[42] & 0x0080) && (!(event_array[2]&0x0080))){history_update(29);BuzzerFlag=1;AlarmLED=1;}				//
			else if(!(main_data_array[42] & 0x0080) && ((event_array[2]&0x0080))){AlarmLED=0;GLCD_BuzzerOnOff(0);}	
	
			if((main_data_array[42] & 0x0020) && (!(event_array[2]&0x0020))){history_update(31);}				//
			else if(!(main_data_array[42] & 0x0020) && ((event_array[2]&0x0020))){}	
	//====================================================
	
	//====================================================
	//Vin IN
			if((main_data_array[42] & 0x0004) && (!(event_array[2]&0x0004))){history_update(34);}				//
			else if(!(main_data_array[42] & 0x0004) && ((event_array[2]&0x0004))){}	
	
			if((main_data_array[42] & 0x0040) && (!(event_array[2]&0x0040))){history_update(30);}				//
			else if(!(main_data_array[42] & 0x0040) && ((event_array[2]&0x0040))){}	
	
			if((main_data_array[42] & 0x0010) && (!(event_array[2]&0x0010))){history_update(32);}				//
			else if(!(main_data_array[42] & 0x0010) && ((event_array[2]&0x0010))){}	
	//====================================================
@endcode	
	 - 통신 끊김 현상 개선
	  ; 통신이 끊어졌는데 타임 아웃후 패킷을 2회 재전송해 본 후 통신끊김 이벤트 발생 
@code
		rc_poll=modbusPacketResponseComplete();
	
		if ( rc_poll==(BYTE)CMPLT_COMPLETE )
		{
			modbus_state++;
			 start_time=0;//get_milliseconds();
	 		 mseconds = 0;
			communication_fault = 0;
			communication_fault_cnt = 0;
			break;
		}
	
		/******************************************************************/
		/*       IF NO RESPONSE RECEIVED TIME OUT AND SEND NEXT POLL      */
		/******************************************************************/
		if ((get_milliseconds()-start_time)>=(UINT32)MB_POLL_TIMEOUT)
		{
			modbus_state=0;
			clr_rx() ;
	
			communication_fault_cnt++;
			if(2<communication_fault_cnt)
			{
				communication_fault = 1;
			}		
			Poll_flag=0;
		}
	         break;
@endcode
	
	
-	090728(modbus_1000_mmi_090724)
	- page44XXX SAVE/CANCEL 관련한 동작 수정
	- 그 외 소소한 버그 수정 
	  - 글자 오타 , 변수 맵핑 버그
	
	
	
	
	*/