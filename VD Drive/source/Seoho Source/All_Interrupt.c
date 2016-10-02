#include	<All_Header.h>
#include	<All_Extern_Variables.h>

interrupt void Driver_Calibration(void)
{
	static long	counter=0;
	static double Ias_sum=0., Ibs_sum=0.;
	static int INIT_CHARGE_Timer= 0;
	double Temp;
	int j= 0, COUNT_START, COUNT_END;

	Temp= (double)P.G01.P07_PWM_frequency_x10_kHz*1e+2;
	COUNT_START=		((int)(0.01/((1./Temp)/2.)+0.5));		// 초기 ADC 안정화를 기다리는 시간 
	COUNT_END=			((int)(0.2/((1./Temp)/2.)+0.5)); 


	// Wait ADC Sensing
	// S/H 할 수 있는 시간을 주자
	// 5.5kW 7.5kW CT 지연시간 10us
	for (j=0; j<600; j++)
	 NOP; 

	Main_counter++;
	Tx_count++;
	if( Main_counter & 0x1 )			
	{		
		#if (DUAL_PWM_INTERRUPT)
			EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_PRD;
			EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_PRD;
		#endif
	}
	else
	{
		#if (DUAL_PWM_INTERRUPT)
			EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;
			EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;
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
		Temp= P.G01.P08_Supply_voltage * SQRT2; // 입력전압 Set 값을 가져와야함 	
		if ((Vdc<(Temp*0.5))||(Vdc>(Temp*1.5)))	INIT_CHARGE_Timer++;
		else if (INIT_CHARGE_Timer>0) INIT_CHARGE_Timer--;
		if (INIT_CHARGE_Timer>10)	Flag.Fault1.bit.INIT_CHARGE= 1;
		Ias_sum += Ias;		
		Ibs_sum += Ibs;
		counter++;
	}
	else
	{
		Ias_offset = Ias_sum/(COUNT_END-COUNT_START);
		Ibs_offset = Ibs_sum/(COUNT_END-COUNT_START);
		Flag.Monitoring.bit.DRIVE_CAL= 1.;

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
	static int FaultDriverCount= 0.; 
//	static int FaultZCCount= 0.;
	double Temp;

	int j;

	Uint16 AO_Count;	// debug2010-0830


	Temp= (double)CpuTimer0Regs.TIM.all;




	Main_counter++;
	Tx_count++;
	Tx_test++;

	
	// Wait ADC Sensing
	// S/H 할 수 있는 시간을 주자
	// 5.5kW 7.5kW CT 지연시간 10us
	for (j=0; j<350; j++)
	 NOP;


	GetSensorValue();			
	Fault_Check_1Sampling();
	#if (VF_MODE==1)
		VF_Controller(); 
	#else
		Flux_Estimation();			// 3465	->	23.1us
		Current_Controller();		// 679	->	4.527us
	#endif
	SVPWM();					// 1090	->	7.267us

	// Call Scheduled Routines 
	if( Main_counter & 0x1 )			// 2*CC Odd Routines : 1 
	{	
		#if (DUAL_PWM_INTERRUPT)
			EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_PRD;
			EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_PRD;
//			EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_PRD;
//			EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_PRD;
//			EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_PRD;
		#endif
	

		if( Main_counter & 0x2 ) 	// 4*CC Routines : 10  
		{		
			Fault_Check_4Sampling();		
		}
//		else if( Main_counter & 0x4 ) 	// 8*CC Routines : 100 
//		{	
//			x8_routine();
//		}
//		else if( Main_counter & 0x8 ) 	// 16*CC Routines : 1000 
//		{	
//			x16_routine();
//		}	
	}
	else
	{
		#if (DUAL_PWM_INTERRUPT)
			EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;
			EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;
//			EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
//			EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
//			EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
		#endif

//		x2_routine();				// 2*CC Even Routines : 0 						

		if( Main_counter & 0x2 ) 	// 4*CC Routines : 10  
		{		
//			if(!OP.Run_stop.bit.AUTO_TUNING){ 
			Speed_Detection();
			#if (VF_MODE==0)
				if(!OP.Run_stop.bit.AUTO_TUNING) Wrpm_ref = Final_reference*Wrpm_scale;
			#endif
			Speed_Controller();
//			}
//			x4_routine();
		}
		else if( Main_counter & 0x4 ) 	// 8*CC Routines : 100 
		{	
//			if(!OP.Run_stop.bit.AUTO_TUNING){
			Field_Weakening();
			Flux_Controller();
//			}
//			x8_routine();
		}
		else if( Main_counter & 0x8 ) 	// 16*CC Routines : 1000 
		{	
//			if(!OP.Run_stop.bit.AUTO_TUNING){
			Field_Weakening();
			Flux_Controller();
//			}
//			x16_routine();
		}		
	}
	
	DB_Controller();
	
	if (GpioDataRegs.GPADAT.bit.GPIO12==0) 	FaultDriverCount++;
	else if( FaultDriverCount > 0)			FaultDriverCount--;
	if (FaultDriverCount > 10 )	Flag.Fault1.bit.DRIVER= 1;


//	if (GpioDataRegs.GPADAT.bit.GPIO16==0) 	FaultZCCount++;
//	else if( FaultZCCount > 0)			FaultZCCount--;
//	if (FaultZCCount > 10 )	Flag.Fault.bit.ZC= 1; 

	
	
	if ( (Flag.Fault1.all & (~Flag.Fault_Neglect1.all)) != 0x0000 )	State_Index= STATE_FAULT;
	if ( (Flag.Fault2.all & (~Flag.Fault_Neglect2.all)) != 0x0000 )	State_Index= STATE_FAULT;

	switch(State_Index)
	{
		case STATE_READY: 			System_Variable_Update();		break;
		case STATE_STOP:
			
			if ( (Command==CMD_RUN)&&(!OP.Run_stop.bit.Emergency_STOP) )
			{	PWM_ON_OFF(1);	Driver_ON=1;	}
			else	
			{	PWM_ON_OFF(0);	Driver_ON=0;
				// Trip zone clear 가 완료 되면 clear flag 해제
				if ( (!EPwm1Regs.TZFLG.bit.OST)&&(!EPwm2Regs.TZFLG.bit.OST)&&(!EPwm3Regs.TZFLG.bit.OST) )
				{
					EPwm1Regs.TZCLR.bit.OST= 0;
					EPwm2Regs.TZCLR.bit.OST= 0;	
					EPwm3Regs.TZCLR.bit.OST= 0;
				}
			}

			break;

		case STATE_ACCELERATING:	Reference_Function();			break;		
		case STATE_DECELERATING:	Reference_Function();			break;
		case STATE_RUNNING:			Reference_Function();			break; 
		case STATE_TUNING:			Auto_Tuning();					break;			 
		case STATE_FAULT:			Fault_Processing();				break; 			 
		default: 					PWM_ON_OFF(0);	Driver_ON=0;	break;
	}



//========================================================
//	start 2010-0830
//========================================================

	AO_Processing( & AO_Count );

	EPwm5Regs.CMPA.half.CMPA 	= AO_Count;
	EPwm5Regs.CMPB  			= AO_Count;
	
//========================================================
//	end of 2010-0830 
//========================================================

	dev_BackgroundDAC ();


	Temp= (Temp-(double)CpuTimer0Regs.TIM.all)/150.;  // us Time
	if (Interrupt_time_max<Temp)	Interrupt_time_max= Temp; 


	EPwm1Regs.ETCLR.bit.INT = 1;	
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;	


	 
}




void DB_Controller()
{
	static int FaultDBCount= 0.;
	double Temp, Temp1;

	if (GpioDataRegs.GPADAT.bit.GPIO13==0) 	FaultDBCount++;
	else if( FaultDBCount > 0)			FaultDBCount--;
	if (FaultDBCount > (0.002/Tsamp) )	Flag.Fault1.bit.DB= 1; 
	
	Temp1= (double)EPwmPeriodCount;
	Temp= (double)(P.G05.P24_DB_full_voltage - P.G05.P23_DB_start_voltage);
	Temp= Temp1*((Vdc-(double)P.G05.P23_DB_start_voltage)/Temp);
	if (Temp<0) Temp= 0.;
	Temp= Temp1-Temp;
	if ( Temp<(Temp1*0.1) ) Temp= Temp1*0.1; // 최대 90% Open

	if (Flag.Fault1.bit.DB == 0)
		EPwm4Regs.CMPA.half.CMPA= (Uint16)Temp;
	else EPwm4Regs.AQCSFRC.bit.CSFA= 2;
	
/*	
	if (Flag.Fault.bit.DB == 0)
	{
		if ( Vdc >  (double)P.G05.P24_DB_full_voltage)	EPwm4Regs.AQCSFRC.bit.CSFA= 1;
		else if ( Vdc <  (double)P.G05.P23_DB_start_voltage)	EPwm4Regs.AQCSFRC.bit.CSFA= 2;
	}
*/
}








