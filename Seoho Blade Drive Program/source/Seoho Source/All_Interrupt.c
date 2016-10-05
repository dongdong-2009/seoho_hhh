#include	<All_Header.h>
#include	<All_Extern_Variables.h>

interrupt void Driver_Calibration(void)
{
	static long	counter=0;
	static double Ias_sum=0., Ibs_sum=0.;
	static int INIT_CHARGE_Timer= 0;
	double Temp, j;
	int COUNT_START, COUNT_END;

	Temp= (double)P.G01.P07_PWM_frequency_x10_kHz*1.e+2;
	COUNT_START=		((int)(0.01/((1./Temp)/2.)+0.5));		// 초기 ADC 안정화를 기다리는 시간 
	COUNT_END=			((int)(1.0/((1./Temp)/2.)+0.5)); 


	// Wait ADC Sensing
	// S/H 할 수 있는 시간을 주자
	for(j=0;j<200;j++)	NOP;

	Main_counter++;
	Tx_count_25ms++;
	Tx_count_1s++;
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
	double Temp, j;
	static int Flag_DB= 0, Flag_DS= 0, Flag_HF= 0, Flag_OT= 0; 
	static int Flag_IC= 0, Flag_OM= 0, Flag_OS= 0, Flag_UV= 0;
	
	Uint16 AO_Count;	// debug2010-0830


	Temp= (double)CpuTimer0Regs.TIM.all;

	Main_counter++;
	Tx_count_25ms++;
	Tx_count_1s++;


	
	// Wait ADC Sensing
	// S/H 할 수 있는 시간을 주자
	for(j=0;j<100;j++)	NOP;

	GetSensorValue();
			
	Fault_Check_1Sampling();

	if (Auto_tuning_index!= 3)
	{
		#if (VF_MODE==1)
			VF_Controller(); 
		#else
			Flux_Estimation();			// 3465	->	23.1us
			Current_Controller();		// 679	->	4.527us
		#endif
		SVPWM();					// 1090	->	7.267us
	}

		#if ( (ENCODER_TYPE== 1)&&(DUAL_PWM_INTERRUPT==0) )
			mcbsp_xmit(0x00,0x00);
		#endif


	// Call Scheduled Routines 
	if( Main_counter & 0x1 )			// 2*CC Odd Routines : 1 
	{	


		#if (DUAL_PWM_INTERRUPT)
			EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_PRD;
			EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_PRD;
			EPwm1Regs.ETSEL.bit.SOCBSEL= ET_CTR_PRD;
//			EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_PRD;
//			EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_PRD;
//			EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_PRD;
		#endif
/*
		#if (DUAL_PWM_INTERRUPT)
			mcbsp_xmit(0x00,0x00);
		#endif
*/	

		if( Main_counter & 0x2 ) 	// 4*CC Routines : 10  
		{		

			Fault_Check_4Sampling();	
		}
//		else if( Main_counter & 0x4 ) 	// 8*CC Routines : 100 
//		{	
//			x8_routine();
//		}
		else if( Main_counter & 0x8 ) 	// 16*CC Routines : 1000 
		{	
			#if (POSITION_CONTROL==1)
//				Theta_ref= Final_reference*1018.;
				Position_Controller();
			#endif
		}	
	}
	else
	{
		#if (DUAL_PWM_INTERRUPT)
			EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;
			EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;
			EPwm1Regs.ETSEL.bit.SOCBSEL= ET_CTR_ZERO;
//			EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
//			EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
//			EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
		#endif
/*
			if (Auto_tuning_index!= 3)
			{
				#if (ENCODER_TYPE== 0)
					Speed_Detection();         // by chy
				#else
					Speed_Detection_ABS();     // by chy
					rx_complete = 0;           // by chy
					rx_int = 0;                // by chy
				#endif
			}

*/
		mcbsp_xmit(0x00,0x00);

//		x2_routine();				// 2*CC Even Routines : 0 						

		if( Main_counter & 0x2 ) 	// 4*CC Routines : 10  
		{		

			if (Auto_tuning_index!= 3)
			{
				#if (ENCODER_TYPE== 0)
					Speed_Detection();         // by chy
				#else
					Speed_Detection_ABS();     // by chy
					rx_complete = 0;           // by chy
					rx_int = 0;                // by chy
				#endif

				#if (VF_MODE==0)&&(POSITION_CONTROL==0)
					if(!OP.Run_stop.bit.AUTO_TUNING) Wrpm_ref = Final_reference*Wrpm_scale;
				#endif
				
//				Wrpm_ref= 100.;
				Speed_Controller();
			}
//			x4_routine();
		}
		else if( Main_counter & 0x4 ) 	// 8*CC Routines : 100 
		{	
			if (Auto_tuning_index!= 3)
			{
				Field_Weakening(); 
				Flux_Controller();
			}
//			x8_routine();
		}
		else if( Main_counter & 0x8 ) 	// 16*CC Routines : 1000 
		{	
			if (Auto_tuning_index!= 3)
			{
				Field_Weakening(); 
				Flux_Controller();
			}

	    	MB_iTimeOutTime++;  // 반드시 인터럽트 루틴 안에 위치
			                    // anybus-ic 용 MODBUS 타임아웃 카운터 ryu

//			x16_routine();
		}		
	}
	
	DB_Controller();
	
	if ( (Flag.Fault1.all & (~Flag.Fault_Neglect1.all)) != 0x0000 )	State_Index= STATE_FAULT;
	if ( (Flag.Fault2.all & (~Flag.Fault_Neglect2.all)) != 0x0000 )	State_Index= STATE_FAULT;
	

	Flag_DB= Flag.Fault1.bit.DB;
	Flag_DS= Flag.Fault1.bit.DEVICE_SHORT;
	Flag_HF= Flag.Fault1.bit.HARDWARE_FAULT;
	Flag_IC= Flag.Fault1.bit.INIT_CHARGE;
	Flag_OM= Flag.Fault1.bit.OC_MAG;
	Flag_OT= Flag.Fault1.bit.OT;
	Flag_OS= Flag.Fault1.bit.OVER_SPEED;
	Flag_UV= Flag.Fault1.bit.UV;

	

	
	switch(State_Index)
	{
		case STATE_READY: 			System_Variable_Update();		break;
		case STATE_STOP:
			
			if ( (Command==CMD_RUN)&&(!OP.Run_stop.bit.Emergency_STOP) )
			{	PWM_ON_OFF(1);	Driver_ON=1;	}
			else	
			{	PWM_ON_OFF(0);	Driver_ON=0;	}

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

//====================================
// F28335 
// DAC Parameter
//==================================== 

  
//	DA1= Idss;		DA1_rng= 25.;		DA1_offset= 0.;
//	DA2= Iqss;		DA2_rng= 25.;		DA2_offset= 0.;
/*
	DA1= Temp_HHH1;		DA1_rng= 5.;		DA1_offset= 0.;
	DA2= Theta_ref;		DA2_rng= 1000.;		DA2_offset= 0.;

	DA3= Final_reference;			DA3_rng= 1.;		DA3_offset= 0.;
	DA4= Ics;			DA4_rng= 25.;		DA4_offset= 0.;

*/

	DA3= etest4;		DA3_rng= 100.;		DA3_offset= 0.;       // torque
	DA4= etest6;			DA4_rng= 600.;		DA4_offset= 0.;  // encoder pulse

/*
	DA1= Theta_ref;		DA1_rng= 3000.;		DA1_offset= 0.;   // 90 degree Theta_ref 2446.83516
	DA2= etest5;		DA2_rng= 3000.;		DA2_offset= 0.;
	DA3= etest4;			DA3_rng= 50.;		DA3_offset= 0.;
	DA4= etest6;			DA4_rng= 25.;		DA4_offset= 0.;


	DA1=  Wrpm_ref;		DA1_rng= 5000.;		DA1_offset= 0.;
	DA2= Wrpm_det;		DA2_rng= 5000.;		DA2_offset= 0.;
	DA3= etest4;			DA3_rng= 50.;		DA3_offset= 0.;
	DA4= Iqse;			DA4_rng= 50.;		DA4_offset= 0.;



	DA1= Wrpm_ref;		DA1_rng= 5000.;		DA1_offset= 0.;
//	DA1= etest4;		DA1_rng= 100.;		DA1_offset= 0.;
	DA2= Wrpm_det;		DA2_rng= 5000.;		DA2_offset= 0.;

	DA3= Iqse_ref;			DA3_rng= 50.;		DA3_offset= 0.;
	DA4= Iqse;			DA4_rng= 50.;		DA4_offset= 0.;

*/

	dev_BackgroundDAC ();

	Temp= (Temp-(double)CpuTimer0Regs.TIM.all)/150.;  // us Time
	if (Interrupt_time_max<Temp)	Interrupt_time_max= Temp; 


	EPwm1Regs.ETCLR.bit.INT = 1;	
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;	
}




void DB_Controller()
{
	double Temp, Temp1;

	if (GpioDataRegs.GPADAT.bit.GPIO13==0) 	Flag.Fault1.bit.DB= 1;

	// PWM duty 최소 20% ~ 최대 90%
	// High 일때 전압 차단 , Low일때 전압 인가 
	Temp1= (double)EPwmPeriodCount;
	Temp= (double)(P.G05.P24_DB_full_voltage - P.G05.P23_DB_start_voltage);
	Temp= Temp1*(0.7*((Vdc-(double)P.G05.P23_DB_start_voltage)/Temp)+0.2);
	if ( Temp<(Temp1*0.2) ){ Temp= 0.;}
	else if ( Temp>(Temp1*0.9) ) Temp= Temp1*0.90; // 최대 90% Open
	if (Flag.Fault1.bit.DB == 0)
		EPwm4Regs.CMPA.half.CMPA= EPwmPeriodCount - (Uint16)Temp;
	else EPwm4Regs.AQCSFRC.bit.CSFA= 2;
/*	
	if (Flag.Fault.bit.DB == 0)
	{
		if ( Vdc >  (double)P.G05.P24_DB_full_voltage)	EPwm4Regs.AQCSFRC.bit.CSFA= 1;
		else if ( Vdc <  (double)P.G05.P23_DB_start_voltage)	EPwm4Regs.AQCSFRC.bit.CSFA= 2;
	}
*/
}




// chy added 2010.10.25

interrupt void Mcbsp_TxINTA_ISR(void)
{
//    McbspaRegs.DXR1.all= sdata;
//   sdata = (sdata+1)& 0x00FF ;
    // To receive more interrupts from this PIE group, acknowledge this interrupt
	tx_complete = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;
}


interrupt void Mcbsp_RxINTA_ISR(void)
{
	static Uint16 adata1=0, cdata1=0;
	static Uint16 adata2=0,  cdata2=0;

	static int i=0;
	double Temp;
	Temp= (double)CpuTimer0Regs.TIM.all;  
//    rdata=McbspaRegs.DRR1.all;
//    if (rdata != ( (rdata_point) & 0x00FF) ) error();
//    rdata_point = (rdata_point+1) & 0x00FF;
    // To receive more interrupts from this PIE group, acknowledge this interrupt
//    PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;

   if( McbspaRegs.SPCR1.bit.RRDY == 1 && tx_complete ==1 )
   {
//		rdata2 = McbspaRegs.DRR2.all;		                             // Read DRR2 first.
//	   	rdata1 = McbspaRegs.DRR1.all; 
		adata2 = McbspaRegs.DRR2.all;		                             // Read DRR2 first.
	   	adata1 = McbspaRegs.DRR1.all; 
		tx_complete= 0;	
		rx_complete = 1;

	}


//			cdata2 = adata2;
//			cdata1 = adata1;

			com_rdata2[rx_int]= adata2;
  			com_rdata1[rx_int++]= adata1;

		if (i < 1)
		{
		//	edata2 = adata2;
		  //	edata1 = adata2;
			i = 100;
		}	  	
	
			
//		rdata2 = com_rdata2[1];
//		rdata1 = com_rdata1[1];
	
//edata2 = rdata2;
//edata1 = rdata2;

	Temp= (Temp-(double)CpuTimer0Regs.TIM.all)/150.;  // us Time
	if (Interrupt_time_max_enc<Temp)	Interrupt_time_max_enc= Temp; 
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;
	
}

//===========================================================================
// No more.
//===========================================================================





