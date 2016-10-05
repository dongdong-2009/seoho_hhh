#include	<All_Header.h>
#include	<All_Extern_Variables.h>

interrupt void Initial_Driver_Calibration(void)
{
	static long	counter=0;
	static double Ias_sum=0., Ibs_sum=0.;
	static int INIT_CHARGE_Timer= 0;
	double Temp, j;
	int COUNT_START, COUNT_END;

	Temp= (double)P.G01.P07_PWM_frequency_x10_kHz*1.e+2;
	COUNT_START=		((int)(0.01/((1./Temp)/2.)+0.5));		// 초기 ADC 안정화를 기다리는 시간 
	COUNT_END=			((int)(0.5/((1./Temp)/2.)+0.5)); 


	// Wait ADC Sensing
	// S/H 할 수 있는 시간을 주자
	for(j=0;j<200;j++)	NOP;

	Main_counter++;
	TxIntervalCnt++;
	TxInterval_1s++;

	Test_count_Rx++;
	Test_count_Tx++;

	if( Main_counter & 0x1 )			
	{		
		#if (DUAL_PWM_INTERRUPT)
			EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_PRD;
			EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_PRD;
			EPwm1Regs.ETSEL.bit.SOCBSEL= ET_CTR_PRD;
		#endif
	}
	else
	{
		#if (DUAL_PWM_INTERRUPT)
			EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;
			EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;
			EPwm1Regs.ETSEL.bit.SOCBSEL= ET_CTR_ZERO;
		#endif 
	}

	if (counter<=(long)COUNT_START)
	{
		counter++;
		Ias_sum=0.;
		Ibs_sum=0.;
		INIT_CHARGE_Timer= 0;
	}
	else if (counter<=(long)COUNT_END)
	{
		Ias_offset = 0.;
		Ibs_offset = 0.;
		GetSensorValue();
		if (Adc_Vclass<1366)	Voltage_class= 0;
		else Voltage_class= 1;
		Temp= P.G01.P09_Supply_voltage * SQRT2; // 입력전압 Set 값을 가져와야함 	
		if ((Vdc<(Temp*0.5))||(Vdc>(Temp*1.5)))	INIT_CHARGE_Timer++;
		else if (INIT_CHARGE_Timer>0) INIT_CHARGE_Timer--;
		if (INIT_CHARGE_Timer>10)	Flag.Fault2.bit.CHAGING_ERROR= 1;
		Ias_sum += Ias;		
		Ibs_sum += Ibs;
		counter++;
	}
	else
	{
		Ias_offset = Ias_sum/(COUNT_END-COUNT_START);
		Ibs_offset = Ibs_sum/(COUNT_END-COUNT_START);
		Flag.Monitoring.bit.INIT_DRIVE_CAL= 1;

		EALLOW;
		PieVectTable.EPWM1_INT 	= &MainPWM;
		EDIS;
	}



	EPwm1Regs.ETCLR.bit.INT = 1;	
	// Acknowledge interrupt to recieve more interrupts from PIE group 3
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

}


	/*---------------------------------------------**
	**    Seoho VD Drive Program             	   **
	** 		1. Shell Program(Wait ADC S/H)         **
	** 		2. ADC                                 **
	** 		3. Flux Estimation                     **
	** 		4. Current Controller                  **
	** 		5. SVPWM                               **
	** 		6-1. Speed Calculation                 **
	**      6-2. Speed Controller				   **
	**      6-3. Field Weakening				   **
	**      6-4. Flux Controller				   **
	**      7. DB Controller					   **
	**      8. Fault Check						   **
	**      9. State Management					   **
	**---------------------------------------------*/

#pragma CODE_SECTION(MainPWM, "ramfuncs");
interrupt void MainPWM(void)
{
	double Interrupt_time; 
//	static int Flag_OL= 0, Flag_OC= 0, Flag_ZSC= 0;

	// Wait ADC Sensing
	Interrupt_time= (double)CpuTimer0Regs.TIM.all;

	Main_counter++;
	TxIntervalCnt++;
	TxInterval_1s++;
	Time_counter_500ms++;

	Test_count_Rx++;
	Test_count_Tx++; 

	Write_Data_Registers_Online();

	// 통신 Tx 속도 25ms 로 고정
	SCIC_Process(); 

//	for (a= 0; a< 50; a++) {  NOP; }

	// Wait ADC Sensing End

	GetSensorValue();
			
	Fault_Check_1Sampling();

	if (Auto_tuning_index!= IDENTIFICATION)
	{
		#if (VF_MODE==1)
			VF_Controller(); 
		#else
			Flux_Estimation();			// 3465	->	23.1us
			Current_Controller();		// 679	->	4.527us
		#endif
		SVPWM();					// 1090	->	7.267us
		if (P.G01.P06_Control_method==4)
			Speed_Detection_Est();
		
	}


	// Call Scheduled Routines 
	if( Main_counter & 0x1 )	
	{	
		#if (DUAL_PWM_INTERRUPT)
			EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_PRD;
			EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_PRD;
			EPwm1Regs.ETSEL.bit.SOCBSEL= ET_CTR_PRD;
		#endif

		if( Main_counter & 0x2 ) 	
			Fault_Check_4Sampling();	
	}
	else
	{
		#if (DUAL_PWM_INTERRUPT)
			EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;
			EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;
			EPwm1Regs.ETSEL.bit.SOCBSEL= ET_CTR_ZERO;
		#endif


		if (Auto_tuning_index!= IDENTIFICATION)
		{
			if (P.G01.P06_Control_method==3)
				Speed_Detection_Enc();

			if( Main_counter & 0x2 )		
				Speed_Controller();
			else if( Main_counter & 0x4 )
			{	Field_Weakening();	Flux_Controller();	}
			else if( Main_counter & 0x8 )
			{	Field_Weakening();	Flux_Controller();	}
		}		
	}
	
	DB_Controller();
	
	if ( (Flag.Fault1.all & (~Flag.Fault_Neglect1.all)) != 0x0000 )	State_index= STATE_FAULT;
	if ( (Flag.Fault2.all & (~Flag.Fault_Neglect2.all)) != 0x0000 )	State_index= STATE_FAULT;
	if ( (Flag.Fault3.all & (~Flag.Fault_Neglect3.all)) != 0x0000 )	State_index= STATE_FAULT;

//	Flag_OL= Flag.Fault1.bit.OL;	Flag_OC= Flag.Fault1.bit.OC;	Flag_ZSC= Flag.Fault1.bit.ZSC;


	switch(State_index)
	{
		case STATE_READY: 						System_Variable_Update();		break;
		case STATE_STOP:
			
			if ( (Command==CMD_RUN)&&(!OP.Run_stop.bit.Emergency_STOP) )
			{	PWM_ON_OFF(1);	Driver_ON=1;	}
			else if ( fabs(Wrpm_det_flt)< 5. ) // || ( Free_run_mode_enable== 1)	
			{	PWM_ON_OFF(0);	Driver_ON=0;	}

			break;
		
		case STATE_DRIVE_CALIBRATION:		 	Drive_Calibration();			break;
		case STATE_BREAKING:					Break_Speed_Up_Controller();	break;
		case STATE_ACCELERATING:				Reference_Function();			break;		
		case STATE_DECELERATING:				
			
			Reference_Function();
			if (Flag.Monitoring.bit.BREAK_CONTROL== 1)		
				Break_Speed_Down_Controller();
			break;

		case STATE_RUNNING:						Reference_Function();			break; 
		case STATE_TUNING:						Auto_Tuning();					break;			 
		case STATE_FAULT:						Fault_Processing();				break; 			 
		default: 								PWM_ON_OFF(0);	Driver_ON=0;	break;
	}
	
	if (State_index!= STATE_FAULT) State_index_old= State_index;

	AO_Processing();

//====================================
// F28335 
// DAC Parameter
// DAx 			: 변수
// DAx_rng		: 범위 -2.5V~0V, 0V ~2.5V
// DAx_offset	: offset, 영점 이동
// DAx_sign		: 0-> +-, 1-> +(positive value only)
//==================================== 
	
/*
	DA1= 0;		DA1_range= 50.;		DA1_offset= 1435.;		DA1_sign= 0.;
	DA2= 0;		DA2_range= 50.;		DA2_offset= 1435.;		DA2_sign= 0.;
	DA3= 0;		DA3_range= 50.;		DA3_offset= 1435.;		DA3_sign= 0.;
	DA4= 0;		DA4_range= 50.;		DA4_offset= 0.;		DA4_sign= 0.;
	dev_BackgroundDAC ();
*/
	

	Interrupt_time= (Interrupt_time-(double)CpuTimer0Regs.TIM.all)/150.;  // us Time
	if (Interrupt_time_max<Interrupt_time)	Interrupt_time_max= Interrupt_time; 


	EPwm1Regs.ETCLR.bit.INT = 1;	
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;	
	 
}










