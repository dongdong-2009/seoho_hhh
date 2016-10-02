//--------------------------------------------------------
// This source is modified by J.S. RYU for RealDSP-UT
//--------------------------------------------------------

#pragma DATA_SECTION(ZONE0_BUF,"ZONE0DATA");
volatile unsigned int ZONE0_BUF[2048];  // by RYU 

#include	<All_Header.h>
#include	<All_Global_Variables.h>
#include	<All_Extern_Variables.h>


#pragma CODE_SECTION(cpu_timer1_isr, "ramfuncs");
interrupt void cpu_timer1_isr(void);
WORD t_cnt0 = 0;

//unsigned char abInData[2]={0,};
//unsigned char abOutData[2]={0,};
unsigned char abInData[32]={0,};
unsigned char abOutData[32]={0,};
unsigned int InOutAddr=0;
int16 test11=0, test12=0;
//UCHAR canInData[128]={0,};
//UCHAR canOutData[128]={0,};


void Nop()
{
  asm ("      nop");
}


//--- debug2010-0831 end
unsigned int check_baud=0;

void main( void )
{
    Uint16 i;

	F28335PowerOnSet();
	delay_ms(20);

	ABIC_AutoBaud();

    check_baud = ABIC_AutoBaud();
    delay_ms(5);

    ABIC_NormalMode();
    delay_ms(1);

    abInData[0] = 0x00;
    abInData[1] = 0x00;

    abOutData[0]=0x01;
    abOutData[1]=0x02;
    canOutData[0] = 0x01;
    canOutData[1] = 0x02;

	// 시간 초기화 
	Set_InitialTime();   // by RYU


	// 시간 읽기 - BCD CODE 이므로 주의 바람 
	Read_Time(&time);    // by RYU
    
	if (P.G00.P10_Drive_power_x10_kW==55)
    {
       //5.5kW inverter
       Td = 4.0/1000;// dead-time (ms)
       Ton = 0.085/1000;//1.0/1000; Switching Times of IGBT (ms)
       Toff = 0.42/1000;//2.0/1000;Switching Times of IGBT (ms)
       Vs = 1.8;//1.8; Collector-Emitter Saturation Voltage (v)
       Vd = 1.75;//1.75; FWD Forward Voltage (v)	
       Wc = 20;// the cutoff frequency of third-order butterworth digital filter (Hz)
       exciting_step = 0.005;	
	   if (P.G01.P00_Rated_power_x10_kW<50)
	   {
	      cs = P.G01.P07_PWM_frequency_x10_kHz * 7 - 150;//change exciting signal after cs sampling periods
	      I_rated = (float)P.G01.P02_Rated_current_x10*0.1;//80% rated current value of motor nameplate.
	   }
	   else
	   {
	      cs = P.G01.P07_PWM_frequency_x10_kHz * 4;
	      I_rated = (float)P.G01.P02_Rated_current_x10*0.08;//80% rated current value of motor nameplate.
	   }
/*
		K_Vs_slope0=(1.17-0.5)/10.0;
		K_Vs_slope1=(1.8-1.17)/(40.0-10.0);
		K_Vd_slope0=(1.17-0.7)/10.0;
		K_Vd_slope1=(1.75-1.17)/(40.0-10.0);
*/
	}
	if (P.G00.P10_Drive_power_x10_kW==110)
    {

       Td = 4.0/1000;// dead-time (ms)
       Ton = 0.065/1000;//0.16/1000;// Switching Times of IGBT (ms)
       Toff = 0.380/1000;//0.4/1000;// Switching Times of IGBT (ms)
       Vs = 2.2;// Collector-Emitter Saturation Voltage (v)
       Vd = 1.75;// FWD Forward Voltage (v)
	   cs = P.G01.P07_PWM_frequency_x10_kHz * 4;//change exciting signal after cs sampling periods
//	   cs = P.G01.P07_PWM_frequency_x10_kHz * 7 - 150;//change exciting signal after cs sampling periods
	   Wc=10;// the cutoff frequency of third-order butterworth digital filter (Hz)
	   exciting_step = 0.005;
	   I_rated = (float)P.G01.P02_Rated_current_x10*0.08;//80% rated current value of motor nameplate.
	}

	if (P.G00.P10_Drive_power_x10_kW==220)
    {

       //55kW inverter
       Td = 4.0/1000;// dead-time (ms)
       Ton = 0.16/1000;//0.16/1000;// Switching Times of IGBT (ms)
       Toff = 0.33/1000;//0.4/1000;// Switching Times of IGBT (ms)
       Vs = 1.75;// Collector-Emitter Saturation Voltage (v)
       Vd = 1.7;// FWD Forward Voltage (v)
	   cs = P.G01.P07_PWM_frequency_x10_kHz * 4;//change exciting signal after cs sampling periods
//	   cs = P.G01.P07_PWM_frequency_x10_kHz * 7 - 150;//change exciting signal after cs sampling periods
	   Wc=10;// the cutoff frequency of third-order butterworth digital filter (Hz)
	   exciting_step = 0.005;
	   I_rated = (float)P.G01.P02_Rated_current_x10*0.08;//80% rated current value of motor nameplate.
	}

	if (P.G00.P10_Drive_power_x10_kW==550)
    {

       //55kW inverter
       Td = 4.0/1000;// dead-time (ms)
       Ton = 0.2/1000;//1.0/1000; Switching Times of IGBT (ms)
       Toff = 0.5/1000;//2.0/1000;Switching Times of IGBT (ms)
       Vs = 1.75;//1.75; Collector-Emitter Saturation Voltage (v)
       Vd = 1.65;//1.65; FWD Forward Voltage (v)
	   cs = P.G01.P07_PWM_frequency_x10_kHz * 4;//change exciting signal after cs sampling periods
	   Wc=10;// the cutoff frequency of third-order butterworth digital filter (Hz)
	   exciting_step = 0.005;
	   I_rated = (float)P.G01.P02_Rated_current_x10*0.08;//80% rated current value of motor nameplate.

/*	   K_Vs_slope0=(1.0-0.4)/50.0;
       K_Vs_slope1=(1.75-1.0)/(300.0-50.0);
	   K_Vd_slope0=(1.0-0.7)/50.0;
	   K_Vd_slope1=(1.65-1.0)/(300.0-50.0);
*/
    }
	if (P.G00.P10_Drive_power_x10_kW==1600)
    {

       //55kW inverter
       Td = 4.0/1000;// dead-time (ms)
       Ton = 0.28/1000;//0.16/1000;// Switching Times of IGBT (ms)
       Toff = 0.80/1000;//0.4/1000;// Switching Times of IGBT (ms)
       Vs = 2;// Collector-Emitter Saturation Voltage (v)
       Vd = 1.8;// FWD Forward Voltage (v)
	   cs =P.G01.P07_PWM_frequency_x10_kHz * 4;//change exciting signal after cs sampling periods
//	   cs = P.G01.P07_PWM_frequency_x10_kHz * 7 - 150;//change exciting signal after cs sampling periods
	   Wc=10;// the cutoff frequency of third-order butterworth digital filter (Hz)
	   exciting_step = 0.005;
	   I_rated = (float)P.G01.P02_Rated_current_x10*0.08;//80% rated current value of motor nameplate.
	}

	if (P.G00.P10_Drive_power_x10_kW==2000)
    {

       //55kW inverter
       Td = 4.0/1000;// dead-time (ms)
       Ton = 0.20/1000;//0.16/1000;// Switching Times of IGBT (ms)
       Toff = 0.50/1000;//0.4/1000;// Switching Times of IGBT (ms)
       Vs = 1.75;// Collector-Emitter Saturation Voltage (v)
       Vd = 1.7;// FWD Forward Voltage (v)
	   cs = 200;//P.G01.P07_PWM_frequency_x10_kHz * 4;//change exciting signal after cs sampling periods
//	   cs = P.G01.P07_PWM_frequency_x10_kHz * 7 - 150;//change exciting signal after cs sampling periods
	   Wc=5;// the cutoff frequency of third-order butterworth digital filter (Hz)
	   exciting_step = 0.005;
	   I_rated = (float)P.G01.P02_Rated_current_x10*0.08;//80% rated current value of motor nameplate.
	}

	if (P.G00.P10_Drive_power_x10_kW==4000)
    {

       //55kW inverter
       Td = 4.0/1000;// dead-time (ms)
       Ton = 1.3/1000;//0.16/1000;// Switching Times of IGBT (ms)
       Toff =3.0/1000;//0.4/1000;// Switching Times of IGBT (ms)
       Vs = 1.5;// Collector-Emitter Saturation Voltage (v)
       Vd = 1.0;// FWD Forward Voltage (v)
	   cs = 300;//P.G01.P07_PWM_frequency_x10_kHz * 4;//change exciting signal after cs sampling periods
//	   cs = P.G01.P07_PWM_frequency_x10_kHz * 7 - 150;//change exciting signal after cs sampling periods
	   Wc=15;// the cutoff frequency of third-order butterworth digital filter (Hz)
	   exciting_step = 0.005;
	   I_rated = (float)P.G01.P02_Rated_current_x10*0.08;//80% rated current value of motor nameplate.
	}

    a0=1.0;//the coefficient of third-order butterworth digital filter transfer function
    a1=2.0;//the coefficient of third-order butterworth digital filter transfer function
    a2=2.0;//the coefficient of third-order butterworth digital filter transfer function
    Hui = 1000.0;//cut off frequency of digital filter
    T = Tsamp;// Samping period (sec), SWITCH_FREQ=2500.
	NN = (long)1200 * P.G01.P07_PWM_frequency_x10_kHz;//identification times
	predict_weight=0.001;//prediction weight
	cs_count = cs;	
	//  M = (Td + Ton - Toff) * ISR_FREQUENCY;//[(Td + Ton - Toff)/1000]/T=(d + Ton - Toff)*ISR_FREQUENCY;(pu).used by voltage estimation
	M = (float)P.G01.P07_PWM_frequency_x10_kHz * 0.1 * (Td + Ton - Toff) ;//[(Td + Ton - Toff)/1000]/T=(d + Ton - Toff)*ISR_FREQUENCY;(pu).used by voltage estimation/
	M1 = M * (Vd-Vs);//used by voltage estimation.
	C1 = 1.0/(6.2831853 * Hui * T+1.0);//RC/( T + RC);//
	C2 = 1.0 - C1;
	A2 = Wc * T * a2;
	A1 = Wc * Wc * T * a1;
	A0 = Wc * Wc * Wc * a0 * T;
	B1 = (1 + predict_weight) * 0.5;
	B2 = (1 - predict_weight) * 0.5; 

	 // Infinity Loop
	while(1)
	{
		State_Management();

//******** canopen **********//
/*
        if (check_baud != 1) {   // ryu
            check_baud = ABIC_AutoBaud();

            ABIC_NormalMode();
	    }
*/
		ABIC_ReadOutData( InOutAddr, 4, abInData );   // canopen

        //ABIC_ReadOutData( InOutAddr, 2, abInData );

#if 0
		if (ReadDoneFlag) {
		    //for (i=0; i<16; i++) 
		    //    canInData[(16*InOutAddr)+i] = abInData[i];

			canInData[(4*InOutAddr)+0] = abInData[0];
			canInData[(4*InOutAddr)+1] = abInData[1];
			canInData[(4*InOutAddr)+2] = abInData[2];
			canInData[(4*InOutAddr)+3] = abInData[3];
            //ABIC_WriteInData(InOutAddr, 1, abOutData);
			ReadDoneFlag = 0;
			InOutAddr++;
			if(32<= InOutAddr)InOutAddr = 0;
		}
#endif
#if 1
		if(ReadDoneFlag)
    	{
		    /*
			if (InOutAddr == 0)
			{
						test1 = abInData[0];
						test2 = abInData[1];

						test11 = abInData[6];
						test12 = abInData[7];

				if (test1==1) test3 = 65;
				else          test3 = 66;
//			abOutData[0] = (unsigned int) Theta_ref;
//			Theta_ref = (double)test2;

			ABIC_WriteInData(InOutAddr, 1, abOutData);
			}
            */

		    for (i=0; i<2; i++) 
		        canInData[(2*InOutAddr)+i] = abInData[i];

            if (InOutAddr == 0) {
			    test1 = canInData[0];
				test2 = canInData[1];
			}

            //abOutData[0] = canOutData[(2*InOutAddr)+0];
			//abOutData[1] = canOutData[(2*InOutAddr)+1];
            //ABIC_WriteInData(InOutAddr, 1, abOutData);
			for (i=0; i<2; i++)
		        abOutData[i] = canOutData[(4*InOutAddr)+i];

            //ABIC_WriteInData(InOutAddr, 8, abOutData);

			ReadDoneFlag=0;

			InOutAddr++;
			//if(64<= InOutAddr)InOutAddr = 0;
			if(64<= InOutAddr)InOutAddr = 0;
		}

#endif

		Read_CAN_Registers();		// ryu
		Write_CAN_Registers();      // ryu
        //***** canopen end ******//

		if (Flag.Monitoring.bit.EEPROM_WRITE_ENABLE_Rx)
		{
			Word_Write_data(Rx_index, Data_Registers[Rx_index]);
			Flag.Monitoring.bit.EEPROM_WRITE_ENABLE_Rx= 0;
		}

		if (Flag.Monitoring.bit.PWM_FREQUENCY_CHANGE)
		{
			DINT;
			PWM_Frequency_Update();
			EPWM_Initialization();
			EQEP_Initialization();
			Flag.Monitoring.bit.PWM_FREQUENCY_CHANGE= 0;
			
			EALLOW;  // This is needed to write to EALLOW protected registers
			PieVectTable.EPWM1_INT 	= &Driver_Calibration;
  			EDIS;    // This is needed to disable write to EALLOW protected registers



			// Enable global Interrupts and higher priority real-time debug events:
			EINT;   // Enable Global interrupt INTM
			ERTM;	// Enable Global realtime interrupt DBGM			
			
		}
		
		Write_Data_Registers_Online();
		
		// 통신 Tx 속도 25ms 로 고정
		SCIC_Process();
		if (Flag.Monitoring.bit.EEPROM_WRITE_ENABLE_Tx)
		{
			Word_Write_data(Tx_index, Data_Registers[Tx_index]);
			Flag.Monitoring.bit.EEPROM_WRITE_ENABLE_Tx= 0;
		}

		// 활성화 시킬 경우 Keypad 먹통됨 
//		Read_Time(&time);    // by RYU	
        
		A2 = Wc * T * a2;
	    A1 = Wc * Wc * T * a1;
    	A0 = Wc * Wc * Wc * a0 * T;

		NOP;

	}

}

void F28335PowerOnSet()
{
	int i;
	Uint16 EEPROM_WRITE_CHECK = 0;

	State_Index = STATE_POWER_ON;
	InitSysCtrl();
	InitXintf();
	InitGpio();
	dev_DacSetup ();                  // chy 2010.11.17   DAC 
    dev_InitDACVariable();            // chy 2010.11.17   DAC
	InitMcbspa();
	// FLASH Code Copy and 초기화 루틴 --------------------------------------------
	// Copy time critical code and Flash setup code to RAM
	// The  RamfuncsLoadStart, RamfuncsLoadEnd, and RamfuncsRunStart
	// symbols are created by the linker. Refer to the FLASH.cmd file.
	MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);  // by RYU
	
	// Call Flash Initialization to setup flash waitstates
	// This function must reside in RAM
 	InitFlash();                                                       // by RYU

	delay_ms(50);  // ryu 외부 하드웨어 안정화를 기다리는 시간.. 

	RESET_DRIVER_SET; //316J PWM on : Reset중 일때는 입력전원 차단이 되지 않는다 

	PWM_HARDWARE_OFF;		// pwm 출력 방지 
	BOOT_MODE_OFF;			// pwm 출력 방지 
	DC_CONTACT_OFF;

/*----------------Interrupt Enable-------------------*/
	DINT;
	IER = 0x0000;
	IFR = 0x0000;
	
	
	Parameter_Initialization_by_Power();
	Drive_Power_kW= ((double)P.G00.P10_Drive_power_x10_kW) * 1.e-1;
	EPWM_Initialization();

	InitPieCtrl();
	InitCpuTimers();   // For this example, only initialize the Cpu Timers

	ConfigCpuTimer(&CpuTimer0, 150, 1000);	// debug 2008.07.16
	ConfigCpuTimer(&CpuTimer1, 150, 1000);	// debug 2008.07.16
	StartCpuTimer0();
//	StartCpuTimer1();
// Initialize SCI-A for data monitoring 
	sci_debug_init(); // by RYU
	// Initialize SCI-B, SCI-C

	for(i=0;i<Buf_MAX;i++)//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11111
	{
		Data_Registers[i]=0x0000;
		CAN_Registers[i]=0x0000;
		SCI_Registers[i]=0x0000;
	}
  
	scib_init();

	scic_init();

    I2C_EEPROM_Initialization();

	abs_encoder1__on(); // for test 나중에 위치 제대로 잡을것.
    delay_ms(1);

	// EEPROM 초기화 용 Code, 활성화시 초기화됨
	Word_Write_data(81, 0);

	// data_register[81]: EEPROM_WRITE_CHECK
	Word_Read_data(81, &EEPROM_WRITE_CHECK);
	Word_Read_data(81, &Data_Registers[81]);
	Word_Write_data(2306, 0);	// Run/stop -> 0
	Word_Write_data(2371, 0);	// Auto tuning -> 0
	// EEPROM_WRITED: 1
	if(EEPROM_WRITE_CHECK == 0) 
	{
//		Parameter_Initialization_by_Power();
		Parameter_Initialization();
		
		for(i=0;i<Buf_MAX;i++)
		{
			Write_Data_Registers_Offline(i);
//			SCI_Registers[i]= Data_Registers[i];
//			Word_Write_data(i, Data_Registers[i]);
		}
//		Data_Registers[81]= SCI_Registers[81]= 1;
		Data_Registers[81]= 1;
		Word_Write_data(81, 1);	// 첫기동시 1회 사용됨
	}
	else 
	{
		for(i=0;i<Buf_MAX;i++)
		{
			Word_Read_data(i, &Data_Registers[i]);
//			SCI_Registers[i]= Data_Registers[i];
			Read_Data_Registers(i);
		}
	}
	
	System_Variable_Initialization();


	InitAdc();
	EQEP_Initialization();

	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.EPWM1_INT 	= &Driver_Calibration;
  	EDIS;    // This is needed to disable write to EALLOW protected registers

// Enable PIE group 3 interrupt 1 for EPWM1_INT

//	IER |= M_INT1;		// Enable CPU INT1 which is connected to CPU-Timer 0:
	IER |= M_INT3;


// chy added 2011.3.17

#if 0
	EALLOW;
	PieVectTable.XINT13= &cpu_timer1_isr; // save interrupt function;
	EDIS;
	IER |= M_INT13;		// Enable CPU INT1 which is connected to CPU-Timer 1:
#endif


  EALLOW;	// Allow access to EALLOW protected registers
   PieVectTable.MRINTA= &Mcbsp_RxINTA_ISR;
   PieVectTable.MXINTA= &Mcbsp_TxINTA_ISR;
   EDIS;   // Disable access to EALLOW protected registers

   PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
   PieCtrlRegs.PIEIER6.bit.INTx5=1;     // Enable PIE Group 6, INT 5
   PieCtrlRegs.PIEIER6.bit.INTx6=1;     // Enable PIE Group 6, INT 6
   IER |=0x20;                            // Enable CPU INT6
 //  EINT;                 

/*
	ZONE0_BUF[0x0060] = 0x0006; //hhh DAC_reset on -> by RYU
	NOP;
	ZONE0_BUF[0x0060] = 0x0007; // DAC_reset off -> by RYU
	NOP;
*/

	DC_CONTACT_ON;		// 메인 전원 on
	delay_ms(10); 
	test_led1_on();
	test_led2_off();
	RESET_DRIVER_CLEAR;	//316J Reset
	delay_ms(1);			
	RESET_DRIVER_SET;		//316J PWM on
	


// Enable global Interrupts and higher priority real-time debug events:
	EINT;   // Enable Global interrupt INTM
	ERTM;	// Enable Global realtime interrupt DBGM

/*----------------Interrupt Enable End-------------------*/	 

//	StopCpuTimer0();
	
}

unsigned int zzzz = 0;
unsigned int kk = 0;
interrupt void cpu_timer1_isr(void){
	CpuTimer1.InterruptCount++;
	t_cnt0++;
	test6++;
	// 10ms period
	if((t_cnt0 % 10) == 0) 
	{
//		MB_iTimeOutTime++;
//		MB_iTimeOutTime_b++;
		kk++;
	}
	// 10ms period
//	if((t_cnt0 % 50) == 0) 	
	
	// Acknowledge this interrupt to receive more interrupts from group 1
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
