//--------------------------------------------------------
// This source is modified by J.S. RYU for RealDSP-UT
//--------------------------------------------------------

#pragma DATA_SECTION(ZONE0_BUF,"ZONE0DATA");
volatile unsigned int ZONE0_BUF[2048];  // by RYU 

#include	<All_Header.h>
#include	<All_Global_Variables.h>
#include	<All_Extern_Variables.h>


//--- debug2010-0831 end
void main( void )
{
	int i;
	F28335PowerOnSet();



	// 시간 읽기 - BCD CODE 이므로 주의 바람 
	Read_Time(&time);    // by RYU

	 // Infinity Loop
	while(1)
	{
		State_Management(); //100us max
		
		if (Flag.Monitoring.bit.EEPROM_WRITE_ENABLE_Rx)
		{
			Word_Write_data(Rx_index, Data_Registers[Rx_index]);
			Flag.Monitoring.bit.EEPROM_WRITE_ENABLE_Rx= 0;
		}

		if (Flag.Monitoring.bit.PWM_FREQUENCY_CHANGE)
		{
			DINT;
			EALLOW;
			EPWM_Initialization();
			Main_counter= 0;
			pwm1.init(&pwm1);
			EQEP_Initialization();
			PWM_Frequency_Update();
			Flag.Monitoring.bit.PWM_FREQUENCY_CHANGE= 0;
/*			
			  // This is needed to write to EALLOW protected registers
			PieVectTable.EPWM1_INT 	= &Initial_Driver_Calibration;
  			    // This is needed to disable write to EALLOW protected registers

*/
			EDIS;
			// Enable global Interrupts and higher priority real-time debug events:
			EINT;   // Enable Global interrupt INTM
			ERTM;	// Enable Global realtime interrupt DBGM			
			
		}
		
//		Write_Data_Registers_Online();
				
		// 통신 Tx 속도 25ms 로 고정
//		SCIC_Process();

	if(Data_Registers[3195])
	{
		for(i=0;i<Buf_MAX;i++)	SCI_Registers[i]=0x0000;
		Data_Registers[3195] = 0;
		SCI_Registers[3195] = 1;
	}
		
		if (Flag.Monitoring.bit.EEPROM_WRITE_ENABLE_Tx)
		{
			Word_Write_data(Tx_index, Data_Registers[Tx_index]);
			Flag.Monitoring.bit.EEPROM_WRITE_ENABLE_Tx= 0;
		}

		RW_Fault_Record();
		
		// by RYU, exe time: 2.8ms~2.9ms
		if (Time_counter_500ms*Tsamp>= 0.5)
		{	Read_Time(&time);	 Time_counter_500ms= 0;	}
		
//		DO1_ON();
//		DO2_ON();
//		DO3_ON();
		NOP;

	}

}

void F28335PowerOnSet()
{
	int i;
	Uint16 EEPROM_WRITE_CHECK = 0;

	State_index = STATE_POWER_ON;
	InitSysCtrl();
	InitXintf();
	InitGpio();
	dev_DacSetup ();                  // chy 2010.11.17   DAC 
    dev_InitDACVariable();            // chy 2010.11.17   DAC

	// FLASH Code Copy and 초기화 루틴 --------------------------------------------
	// Copy time critical code and Flash setup code to RAM
	// The  RamfuncsLoadStart, RamfuncsLoadEnd, and RamfuncsRunStart
	// symbols are created by the linker. Refer to the FLASH.cmd file.
	MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);  // by RYU
	
	// Call Flash Initialization to setup flash waitstates
	// This function must reside in RAM
 	InitFlash();                                                       // by RYU
	
	delay_ms(50);

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
	StartCpuTimer0();

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
    delay_ms(1);

	// EEPROM 초기화 용 Code, 활성화시 초기화됨
	Word_Write_data(81, 0);

	// data_register[81]: EEPROM_WRITE_CHECK
	Word_Read_data(81, &EEPROM_WRITE_CHECK);
	Word_Read_data(81, &Data_Registers[81]);
	Word_Write_data(2300, 0);	// Local -> 0
	Word_Write_data(2306, 0);	// Run/stop -> 0
	Word_Write_data(2370, 0);	// Drive Calibration -> 0
	Word_Write_data(2371, 0);	// Auto tuning -> 0

	// EEPROM_WRITED: 1
	if(EEPROM_WRITE_CHECK != 1) 
	{
//		Parameter_Initialization_by_Power();
		Parameter_Initialization();
		
		for(i=0;i<Buf_MAX;i++)
		{
			Write_Data_Registers_Offline(i);
//			SCI_Registers[i]= Data_Registers[i];
//			Word_Write_data(i, Data_Registers[i]);
		}

		// 시간 초기화 
		Set_InitialTime();   // by RYU
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
			Data_Registers[209]= P.G00.P09_IGBT_current_x10;
			Data_Registers[210]= P.G00.P10_Drive_power_x10_kW;
			Read_Data_Registers(i);
		}
	}
	delay_ms(50);
	System_Variable_Initialization();


	InitAdc();
	EQEP_Initialization();

	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.EPWM1_INT 	= &Initial_Driver_Calibration;
  	EDIS;    // This is needed to disable write to EALLOW protected registers

// Enable PIE group 3 interrupt 1 for EPWM1_INT

//	IER |= M_INT1;		// Enable CPU INT1 which is connected to CPU-Timer 0:
	IER |= M_INT3;

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






