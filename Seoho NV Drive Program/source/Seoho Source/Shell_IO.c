#include	<All_Header.h>
#include	<All_Extern_Variables.h>

#pragma CODE_SECTION(Get_Command, "ramfuncs"); 
void Get_Command( int * Command, double * Init_reference )
{
	int Local_command, Terminal_command, Fieldbus_command;
	double Temp_ref;
	double Local_reference, Analog_reference, Digital_reference, Terminal_reference,  Fieldbus_reference;

	Local_Processing( & Local_command, & Local_reference );
	AI_Processing( & Analog_reference);
	DI_Processing( & Terminal_command, & Digital_reference );
	if ( (Digital_reference== 0.)&&(P.G03.P53_S_curve_enable== 0) )
	{
		Terminal_reference= Analog_reference; 
		Flag.Monitoring.bit.AI_REF_ENABLE= 1;
	}
	else 
	{
		Terminal_reference= Digital_reference;
		Flag.Monitoring.bit.AI_REF_ENABLE= 0;
	} 
	Fieldbus_Processing( & Fieldbus_command, & Fieldbus_reference  );
	
	if (OP.Run_stop.bit.Local== 1)
	{
		
		* Command= Local_command;
		Temp_ref= Local_reference;
		if (OP.Run_stop.bit.Local_DIR)		
			Temp_ref= -(Temp_ref);
		
	}
	else 
	{
		switch ( P.G03.P00_Run_stop_source )
		{
			case 0:		* Command= Terminal_command;	break;	// Terminal
			case 1: 	* Command= Local_command;		break;	// Local
			case 2: 	* Command= Fieldbus_command;	break;	// Fieldbus
			default:	* Command= CMD_STOP;			break; 
		}

		switch ( P.G03.P01_Reference_source )
		{
			case 0: 	Temp_ref= Terminal_reference;	break;	// Terminal
			case 1:		Temp_ref= Local_reference;		break;	// Local
			case 2: 	Temp_ref= Fieldbus_reference; 	break;	// Fieldbus
			default:	Temp_ref= 0.0;					break;
		}
		
		switch ( P.G03.P00_Run_stop_source )
		{
			case 0:	if (Flag.DI.bit.INVERTER_DIR)		Temp_ref= -(Temp_ref);	break;	// Terminal
			case 1:	if (OP.Run_stop.bit.Local_DIR)		Temp_ref= -(Temp_ref);	break;	// Local
			case 2:	if (OP.Run_stop.bit.Fieldbus_DIR)	Temp_ref= -(Temp_ref);	break;	// Fieldbus
		}
	}

	// 초기 필드 형성시간 대기
	#if (VF_MODE==0)
		if (!Flux_build_up) Temp_ref= 0.;
	#endif
	if	( 	((Flag.Fault1.all & (~Flag.Fault_Neglect1.all)) != 0x0000) 
		||	((Flag.Fault2.all & (~Flag.Fault_Neglect2.all)) != 0x0000)	
		||	((Flag.Fault3.all & (~Flag.Fault_Neglect3.all)) != 0x0000)	)
		* Command= CMD_STOP;
	
	if (OP.Run_stop.bit.Emergency_STOP== 1)
		* Command= CMD_STOP;
	
	if ( (Flag.Monitoring.bit.DRIVE_ENABLE_CONTROL== 1) && (Flag.DI.bit.DRIVE_ENABLE== 0)  )
		* Command= CMD_STOP;

	if (* Command== CMD_STOP) Temp_ref= 0.;
	
	if (Temp_ref>= 0)	Temp_ref= BOUND(fabs(Temp_ref),(Wrpm_max/Wrpm_rated),(Wrpm_min/Wrpm_rated));
	else				Temp_ref= -BOUND(fabs(Temp_ref),(Wrpm_max/Wrpm_rated),(Wrpm_min/Wrpm_rated));	

	* Init_reference= Temp_ref;

}



#pragma CODE_SECTION(DI_Function, "ramfuncs"); 
void DI_Function()
{
	int DI_bit[9], Temp, n, DI_function[7], a;
		
	Flag.DI.all= 0;
	DI_bit_all= Temp= (int)((65535-ZONE0_BUF[0x0050]) & 0xFF);
	// DI 0~1 Run/dir
	DI_bit[0]= Temp & 0x1;
	DI_bit[1]= (Temp>>1) & 0x1;

	if (!P.G08.P00_Run_stop_control)
	{			
		// FWD,REV 모드 
		if ((DI_bit[0]^DI_bit[1])==1)
		{
			Flag.DI.bit.INVERTER_RUN= 1;
			if (DI_bit[1])	Flag.DI.bit.INVERTER_DIR= 1;
			else 			Flag.DI.bit.INVERTER_DIR= 0; 
		}
		else	Flag.DI.bit.INVERTER_RUN= 0;
			
	}
 	else
 	{ 
		Flag.DI.bit.INVERTER_RUN = DI_bit[0];
		Flag.DI.bit.INVERTER_DIR = DI_bit[1];  // reverse run
		
	}
	
	
	// DI 2~7 Flag on by DI function selection 	
	DI_function[0]= P.G08.P01_DI03_function;
	DI_function[1]=	P.G08.P02_DI04_function;
	DI_function[2]=	P.G08.P03_DI05_function;
	DI_function[3]=	P.G08.P04_DI06_function;
	DI_function[4]=	P.G08.P05_DI07_function;
	DI_function[5]=	P.G08.P06_DI08_function;
	DI_function[6]=	0;
		
	a= 0;

	for	(n=1;n<8;n++)
	{
		DI_bit[n]= (Temp>>n) & 0x1;
		if (DI_function[n-2]== 1) a= 1;
		
		switch ( DI_function[n-2] )
		{
			case 0:														break;
			case 1:		Flag.DI.bit.DRIVE_ENABLE=		DI_bit[n];		break;
			case 2:		Flag.DI.bit.MULTI_STEP_BIT0=	DI_bit[n];		break;	
			case 3:		Flag.DI.bit.MULTI_STEP_BIT1=	DI_bit[n];		break; 
			case 4:		Flag.DI.bit.MULTI_STEP_BIT2=	DI_bit[n];		break;				
			case 5:		Flag.DI.bit.MULTI_STEP_BIT3=	DI_bit[n];		break;		
			case 6:		Flag.DI.bit.FAULT_RESET= 		DI_bit[n];		break;		
			case 7:		Flag.DI.bit.JOG= 				DI_bit[n];		break;	
//			case 8:		Flag.DI.bit.AI_REF_ACTIVE=		DI_bit[n];		break;	
//			case 9:		Flag.DI.bit.AI_LOCAL_REMOTE=	DI_bit[n];		break;	
			case 10:	Flag.DI.bit.EXT_FAULT_A= 		DI_bit[n];		break;	
			case 11:	Flag.DI.bit.EXT_FAULT_B= 		DI_bit[n];		break;	
//			case 12:	Flag.DI.bit.MOTOR_SELECT= 		DI_bit[n];		break;	
//			case 13:	Flag.DI.bit.MB_BRAKE_STATE= 	DI_bit[n];		break;	
//			case 14:	Flag.DI.bit.ACCEL_DECEL_SWITCH= DI_bit[n];		break;	
//			case 15:	Flag.DI.bit.REF_TUNING_INC= 	DI_bit[n];		break;	
//			case 16:	Flag.DI.bit.REF_TUNING_DEC= 	DI_bit[n];		break;	
//			case 17:	Flag.DI.bit.ACC_DEC_BYP= 		DI_bit[n];		break;	
//			case 18:	Flag.DI.bit.PID_CNTL_ENABLE= 	DI_bit[n];		break;	
//			case 19:	Flag.DI.bit.AUTO_PID_MODE= 		DI_bit[n];		break;	
//			case 20:	Flag.DI.bit.PID_GAIN_SEL= 		DI_bit[n];		break;	
//			case 21:	Flag.DI.bit.PID_I_RESET= 		DI_bit[n];		break;	
//			case 22:	Flag.DI.bit.TRQ_REF_OPT_BYP= 	DI_bit[n];		break;	
//			case 23:	Flag.DI.bit.TRQ_SIGN= 			DI_bit[n];		break;	
//			case 24:	Flag.DI.bit.TRQ_OUT_ZERO= 		DI_bit[n];		break;	
//			case 25:	Flag.DI.bit.TIMER_RUN_ENABLE= 	DI_bit[n];		break;	
//			case 26:	Flag.DI.bit.SLAVE_RUN_STATUS= 	DI_bit[n];		break;	
//			case 27:	Flag.DI.bit.SYNC_CTRL_OPT_BPY= 	DI_bit[n];		break;	
//			case 28:	Flag.DI.bit.FLYING_START= 		DI_bit[n];		break;	
//			case 29:	Flag.DI.bit.DISABLE_PROFIBUS= 	DI_bit[n];		break;	
			default:	Flag.DI.all= 0;									break;
		}
	}
	if (a== 1)
		Flag.Monitoring.bit.DRIVE_ENABLE_CONTROL= 1;
	else Flag.Monitoring.bit.DRIVE_ENABLE_CONTROL= 0;
}

#pragma CODE_SECTION(Multi_Step_Processing, "ramfuncs"); 
#define USE_PER	0
#define USE_Hz	1
#define USE_RPM	2  
double Multi_Step_Processing( )
{
	int Temp = 0;

	double di_ref;

	if (Flag.DI.bit.MULTI_STEP_BIT0)	Temp+= 1;
	if (Flag.DI.bit.MULTI_STEP_BIT1)	Temp+= 2;
	if (Flag.DI.bit.MULTI_STEP_BIT2)	Temp+= 4;
	if (Flag.DI.bit.MULTI_STEP_BIT3)	Temp+= 8;

	switch (Temp)
	{
		case 0:  di_ref= P.G19.P03_Minimum_speed;		break;
		case 1:  di_ref= P.G09.P01_Step01_set_x10;		break;
		case 2:  di_ref= P.G09.P02_Step02_set_x10;		break;
		case 3:  di_ref= P.G09.P03_Step03_set_x10;		break;
		case 4:  di_ref= P.G09.P04_Step04_set_x10;		break;
		case 5:  di_ref= P.G09.P05_Step05_set_x10;		break;
		case 6:  di_ref= P.G09.P06_Step06_set_x10;		break;
		case 7:  di_ref= P.G09.P07_Step07_set_x10;		break;
		case 8:  di_ref= P.G09.P08_Step08_set_x10;		break;
		case 9:  di_ref= P.G09.P09_Step09_set_x10;		break;
		case 10: di_ref= P.G09.P10_Step10_set_x10;		break;
		case 11: di_ref= P.G09.P11_Step11_set_x10;		break;
		case 12: di_ref= P.G09.P12_Step12_set_x10;		break;
		case 13: di_ref= P.G09.P13_Step13_set_x10;		break;
		case 14: di_ref= P.G09.P14_Step14_set_x10;		break;
		case 15: di_ref= P.G09.P15_Step15_set_x10;		break;
		default: di_ref= 0.0;   						break;
	}
	
	switch (P.G09.P16_Unit_selection)
	{
		case 0:		di_ref/= 1000.;								break;	// USE_PER
		case 1: 	di_ref/= P.G01.P03_Rated_frequency * 10;	break;	// USE_Hz
		case 2: 	di_ref/= P.G01.P05_Rated_speed * 10;		break;	// USE_RPM
		default:	di_ref= 0.;									break;
	}
	
	return di_ref; 
}


void driver_enable_proc()
{
}


void DI_Processing(int * cmd, double * ref )
{

	DI_Function();

	* ref = Multi_Step_Processing( );

	if (Flag.DI.bit.INVERTER_RUN)	* cmd= CMD_RUN;
	else 							* cmd= CMD_STOP; 
	
	if( Flag.DI.bit.JOG == 1 )		* ref = (double)P.G09.P00_JOG_set_x1000*1.e-3;

	if( Flag.DI.bit.DRIVE_ENABLE )	driver_enable_proc();

	if( Flag.DI.bit.EXT_FAULT_A )	
	{
		Flag.DO.bit.FAULT_OUT_A = 1;
		Flag.Fault2.bit.EXT_FAULT= 1;
	}
	
	if( Flag.DI.bit.EXT_FAULT_B )
	{
		Flag.DO.bit.FAULT_OUT_B = 1;
		Flag.Fault2.bit.EXT_FAULT= 1;
	}
	
	if (Flag.DI.bit.FAULT_RESET)
	{
		OP.Run_stop.bit.FAULT_RESET= 1;
	}
}


int DO_Port_Check( int out_function )
{
	int i;

	i = 0;

	if (out_function== 4)
		Flag_brake_control= 1;

	switch (out_function)
	{
		case 1:		i= Flag.DO.bit.DRIVE_READY;				break;
		case 2:		i= Flag.DO.bit.FAULT_OUT_A;				break;
		case 3:		i= Flag.DO.bit.FAULT_OUT_B;				break;
		case 4:		i= Flag.DO.bit.MOTOR_BRAKE;				break;
		default:	i= 0;									break;
	}
	return i;
}  	

void DO_Processing()
{
	int i, a;
	
	// Test 용 DO toggle 기능 만들것 

	if ( (State_index==STATE_POWER_ON)||(State_index==STATE_INITIAL_DRIVE_CALIBRATION)||(State_index==STATE_FAULT) )
		Flag.DO.bit.DRIVE_READY= 0;
	else Flag.DO.bit.DRIVE_READY= 1;

	if ( (Flag.Fault2.bit.EXT_FAULT== 1)&&(Flag.DI.bit.EXT_FAULT_A== 1) )		Flag.DO.bit.FAULT_OUT_A= 1;
	else 																		Flag.DO.bit.FAULT_OUT_A= 0;

	if ( (Flag.Fault2.bit.EXT_FAULT== 1)&&(Flag.DI.bit.EXT_FAULT_B== 1) )		Flag.DO.bit.FAULT_OUT_B= 1;
	else 																		Flag.DO.bit.FAULT_OUT_B= 0;	
	
	a= 0;
	Flag_brake_control= 0;

	i = DO_Port_Check( P.G12.P00_DO1_function );
	if( i ) {	DO1_ON();	a+= 1;	}
	else 		DO1_OFF();
	

	i = DO_Port_Check( P.G12.P00_DO2_function );
	if( i ) {	DO2_ON();	a+= 2;	}
	else 		DO2_OFF();
	 	
	i = DO_Port_Check( P.G12.P00_DO3_function );
	if( i ) {	DO3_ON();	a+= 4;	}
	else 		DO3_OFF();

	DO_bit_all= a;
	if (Flag_brake_control== 1)	Flag.Monitoring.bit.BREAK_CONTROL= 1;
	else						Flag.Monitoring.bit.BREAK_CONTROL= 0;

}


/*
#define Count20mA	1925
#define Count4mA	3250
#define Count0mA    14250
*/
//========================================================
//	start 2010-0830
//========================================================
#pragma CODE_SECTION(AO_Processing, "ramfuncs")
void AO_Processing()
{

	double Output,RateValue;
	double Temp;
	unsigned int count;
	int Temp1, Count0mA, Count4mA, Count20mA;

	Count0mA= (int)(750.*(50./(double)P.G01.P07_PWM_frequency_x10_kHz));
	Count20mA= EPwmPeriodCount- (int)(1500.*(50./(double)P.G01.P07_PWM_frequency_x10_kHz));
	Count4mA= (Count20mA - Count0mA)*0.2 + Count0mA;

//	Temp= (double)P.G11.AO1.P02_Adjustment_0mA *(Count20mA-Count0mA);
	Temp= 0.;
	switch ( P.G11.AO1.P00_Output_selection ) //     
	{
		case 0:		Output= (Wrpm_det_flt/Wrpm_rated)*(double)P.G01.P03_Rated_frequency;
					RateValue= (double)P.G01.P03_Rated_frequency;											break;
		case 1:		Output= Wrpm_det_flt;			RateValue= Wrpm_rated;									break;
		case 2:		Output= Is_mag_rms;				RateValue= Is_rated;									break;
		case 3:		Output= (sqrt(Vdss_ref*Vdss_ref+Vqss_ref*Vqss_ref)*SQRT2);
					RateValue= (double)P.G01.P01_Rated_voltage;												break;
		case 4:		Output= Te;						RateValue= Te_rate;										break;
		case 5:		Output= Output_power_x10_kW;	RateValue= (double)P.G01.P00_Rated_power_x10_kW*100;	break;
		case 6:		Output= Vdc;					RateValue= (double)P.G01.P09_Supply_voltage*SQRT2;		break;
		// Free Function Output 
//		case 7:	 	Output= 0.0;					RateValue= 100.0;										break;
		case 8:		count= (unsigned int)Count0mA+(unsigned int)(Temp);										break;
		case 9:		count= (unsigned int)Count4mA+(unsigned int)(Temp);										break;
		case 10:	count= (unsigned int)Count20mA+(unsigned int)(Temp);									break;
		default:	Output= 0.0; 	count= 0;	RateValue= 100.0;											break;
	}

	if ( P.G11.AO1.P00_Output_selection<= 7 )
	{
		Temp= fabs(Output)/(RateValue*(double)P.G11.AO1.P05_Max_output_x1000*1.e-3);
/*		
		if( P.G11.AO1.P01_Type == 1 ) // 4~20mA 출력 
			Temp1 = P.G11.AO1.P03_Adjustment_4mA; // 4mA offset count
		else Temp1 = P.G11.AO1.P02_Adjustment_0mA; // 0~ 20mA 출력
*/
		if( P.G11.AO1.P01_Type == 1 ) // 4~20mA 출력 
			Temp1 = Count4mA; // 4mA offset count
		else Temp1 = Count0mA; // 0~ 20mA 출력
				
		
		Temp= (Temp*(double)(Count20mA - Temp1)+Temp1);
//		if(P.G11.AO1.P06_Inversion== 1) // 0일 때 20mA
//			Temp= Count20mA+Temp;
//		else	Temp= Temp1-Temp;
//		Temp= Temp1-Temp;

		Temp= BOUND(Temp, Count20mA, Temp1);
			
		count = (Uint16) Temp;
	}


	EPwm5Regs.CMPA.half.CMPA 	= count;
	EPwm5Regs.CMPB  			= count;

// channel 2 
/*
	Temp= (double)P.G11.AO2.P02_Adjustment_0mA *(Count20mA-Count0mA);
	switch ( P.G11.AO2.P00_Output_selection ) //     
	{
		case 0:		Output= (Wrpm_det_flt/Wrpm_rated)*(double)P.G01.P03_Rated_frequency;
					RateValue= (double)P.G01.P03_Rated_frequency;											break;
		case 1:		Output= Wrpm_det_flt;				RateValue= Wrpm_rated;				break;
		case 2:		Output= Is_mag_rms;				RateValue= Is_rated;		break;
		case 3:		Output= (sqrt(Vdss_ref*Vdss_ref+Vqss_ref*Vqss_ref)*SQRT2);
					RateValue= (double)P.G01.P01_Rated_voltage;												break;
		case 4:		Output= Te;						RateValue= Te_rate;										break;
		case 5:		Output= Output_power_x10_kW;	RateValue= (double)P.G01.P00_Rated_power_x10_kW*100;	break;
		case 6:		Output= Vdc;					RateValue= (double)P.G01.P09_Supply_voltage*SQRT2;		break;
		// Free Function Output 
//		case 7:	 	Output= 0.0;					RateValue= 100.0;										break;
		case 8:		count= (unsigned int)Count0mA-(unsigned int)(Temp);										break;
		case 9:		count= (unsigned int)Count4mA-(unsigned int)(Temp);										break;
		case 10:	count= (unsigned int)Count20mA-(unsigned int)(Temp);									break;
		default:	Output= 0.0; 	count= 0;	RateValue= 100.0;											break;
	}
	
	
	if ( P.G11.AO2.P00_Output_selection<= 7 )
	{
		x= Output;
		x1= 0.0;
		x2= RateValue*(double)P.G11.AO2.P05_Max_output_x1000*1.e-3;
		y2= (double)P.G11.AO2.P04_Adjustment_20mA;
		if( P.G11.AO2.P01_Type == 1 ) // 4~20mA 출력 
			y1 = (double)P.G11.AO2.P03_Adjustment_4mA; // 4mA offset count
		else y1 = (double)P.G11.AO2.P02_Adjustment_0mA; // 0~ 20mA 출력

		if(P.G11.AO2.P06_Inversion== 0) // 0일 때 20mA
			Temp = linear_eq( x1, x2, y1, y2, x );
		else Temp = linear_eq( x2, x1, y1, y2, x );

		Temp = (1- Temp) * EPwmPeriodCount;
		count = (Uint16) Temp;
	}

//	* count1 = count; 
*/
}


//====================================
// 2010-0831 아날로그 입력 지령 처리 
//====================================


#define AI_COUNT_0A		(0.)		// default 4mA input AD count
#define AI_COUNT_4mA	(800.0)		// default 4mA input AD count
#define AI_COUNT_20mA	(4000.0)		// default 4mA input AD count
#define AI_COUNT_n10V	(250.0)
#define AI_COUNT_0V		(2190.0)
#define AI_COUNT_p10V	(4095.0)		//	

// ADC의 범위는 -1.0 ~ 1.0 까지임 
#pragma CODE_SECTION(GetAnalogRef, "ramfuncs"); 	
void GetAnalogRef( double * analog_ref )
{

	double sign,a,y, Temp;
	double Offset, Min_value, Max_value, Min_scale, Max_scale; 
	static double Temp_LPF= 0.;

	switch( P.G06.P00_Analog_reference_source)	// channel select
	{
		case 1:
			Offset= (double)P.G06.P04_AI1_Offset_x10*0.1;
			Min_value= (double)P.G06.P05_AI1_Min_value_x10*0.1;
			Max_value= (double)P.G06.P06_AI1_Max_value_x10*0.1;
//			Unit= P.G06.P11_AI1_Unit_selection;
			Min_scale= (double)P.G06.P07_AI1_Min_scale_x1000*0.001;
			Max_scale= (double)P.G06.P08_AI1_Max_scale_x1000*0.001;
			
			switch( P.G06.P02_AI1_Type ) // input type
			{
				case 0:				// 0~10V
					y= 	LINEAR_EQ(Adc_AIN_V1, AI_COUNT_0V, AI_COUNT_p10V, 0., 10.) + Offset;

					if( y < Min_value ) 		y= Min_scale;
					else if( y > Max_value )	y= Max_scale;
					else						y= LINEAR_EQ(y, Min_value, Max_value, Min_scale, Max_scale);				
														
					break;
				
				case 1 :	// -10 ~ 10V
					y= 	LINEAR_EQ(Adc_AIN_V1, AI_COUNT_n10V, AI_COUNT_p10V, -10., 10.) + Offset;

					if( y < 0.0 ) sign = -1.0;
					else		  sign =  1.0;

					a= y * sign;
					if( a < Min_value )			y = Min_scale * sign;
					else if( a > Max_value )	y = Max_scale * sign ;
					else						y = LINEAR_EQ(a, Min_value, Max_value, Min_scale, Max_scale) * sign;

					break;
				
				case 2:		// 4 ~ 20 mA
					y = LINEAR_EQ(Adc_AIN_I1, AI_COUNT_4mA, AI_COUNT_20mA, 4., 20.) + Offset; 
						
					if (Min_value< 4.) Min_value= 4.;
					 
					if( y < Min_value )			y = Min_scale;
					else if( y > Max_value )	y = Max_scale;
					else						y = LINEAR_EQ(y, Min_value, Max_value, Min_scale, Max_scale); 

					break;

				case 3: // 0~ 20mA
					y = LINEAR_EQ(Adc_AIN_I1, AI_COUNT_0A, AI_COUNT_20mA, 0., 20.) + Offset ; 
					
					if( y < Min_value ) 		y = Min_scale;
					else if( y > Max_value ) 	y = Max_scale;
					else						y = LINEAR_EQ(y, Min_value, Max_value, Min_scale, Max_scale); 

					break;
				}
			
			a= (double)P.G06.P10_AI1_Discretness;
			if( P.G06.P09_AI1_Inversion == 1) y = 1.0 - y ;		// y = -y
			if (( a > 7 )&&(a < 129)) y = (((int)( y * a )) / a); 
			a= (double)P.G06.P03_AI1_Filter_time_constant_ms *1.e-3; 
			Temp_LPF+= Tsamp/(a*10.)*(y-Temp_LPF);
			Temp_LPF= (double)((int)(Temp_LPF*1.e+4)) * 1.e-4;
			Temp= Temp_LPF;
			break;

		// ----------- ch2 select

		case 2:  
			Offset= (double)P.G06.P15_AI2_Offset_x10*0.1;
			Min_value= (double)P.G06.P16_AI2_Min_value_x10*0.1;
			Max_value= (double)P.G06.P17_AI2_Max_value_x10*0.1;
//			Unit= P.G06.P22_AI2_Unit_selection;
			Min_scale= (double)P.G06.P18_AI2_Min_scale_x1000*0.001;
			Max_scale= (double)P.G06.P19_AI2_Max_scale_x1000*0.001;
			
			switch( P.G06.P13_AI2_Type ) // input type
			{
				case 0:				// 0~10V
					y= LINEAR_EQ(Adc_AIN_V1, AI_COUNT_0V, AI_COUNT_p10V, 0., 10.) + Offset ;	// 입력 볼트 
					
					if( y < Min_value ) 		y= Min_scale;
					else if( y > Max_value )	y= Max_scale;
					else						y= LINEAR_EQ(y, Min_value, Max_value, Min_scale, Max_scale);				

					break;
				
				case 1 :	// -10 ~ 10V
					y = LINEAR_EQ(Adc_AIN_V1, AI_COUNT_n10V, AI_COUNT_p10V, -10., 10.) + Offset;	// 입력 볼트 

					if( y < 0.0 ) sign = -1.0;
					else		  sign =  1.0;

					a= y * sign;
					if( a < Min_value )			y = Min_scale * sign;
					else if( a > Max_value )	y = Max_scale * sign ;
					else						y = LINEAR_EQ(a, Min_value, Max_value, Min_scale, Max_scale) * sign;

					break;
				
				case 2:		// 4 ~ 20 mA
					y = LINEAR_EQ(Adc_AIN_I1, AI_COUNT_4mA, AI_COUNT_20mA, 4., 20.) + Offset; 

					if (Min_value< 4.) Min_value= 4.;
					 
					if( y < Min_value )			y = Min_scale;
					else if( y > Max_value )	y = Max_scale;
					else						y = LINEAR_EQ(y, Min_value, Max_value, Min_scale, Max_scale); 

					break;

				case 3: // 0~ 20mA
					y = LINEAR_EQ(Adc_AIN_I1, AI_COUNT_0A, AI_COUNT_20mA, 0., 20.) + Offset ; 

					if( y < Min_value ) 		y = Min_scale;
					else if( y > Max_value ) 	y = Max_scale;
					else						y = LINEAR_EQ(y, Min_value, Max_value, Min_scale, Max_scale); 

					break;
				}
			
			a= (double)P.G06.P21_AI2_Discretness;
			if( P.G06.P20_AI2_Inversion == 1) y = 1.0 - y ;		// y = -y
			if (( a > 7 )&&(a < 129)) y = (((int)( y * a )) / a); 
			a= (double)P.G06.P14_AI2_Filter_time_constant_ms *1.e-3;
			Temp_LPF+= Tsamp/(a*10.)*(y-Temp_LPF);
			Temp_LPF= (double)((int)(Temp_LPF*1.e+4)) * 1.e-4;
			Temp= Temp_LPF;
			break; 

		default:
			y = 0.0;
			break;
	}
	* analog_ref = Temp;
}


void AI_Processing(double * ref )
{

	double adc_in0;

	
	GetAnalogRef( & adc_in0);
	
	* ref = adc_in0;
//	* ref = (double)((int)(adc_in0*1.e+3)*1.e-3); 

	if (P.G06.P00_Analog_reference_source == 0)
		* ref = 0.0;

}


void Local_Processing(int * cmd, double * ref )
{
	static double Temp_speed= 0., Temp_frequency= 0.;
	static int Flag_ref= 0;
	double Temp1, Temp2;
	
	Temp1= (double)OP.Reference.Local.Speed;
	Temp2= (double)OP.Reference.Local.Frequency_x10*1.e-1;

	// Flag_ref: 0-speed, 1-freq, 2-torque, 3-PID
	if (Temp1!= Temp_speed)
		Flag_ref= 0;
	else if (Temp2!= Temp_frequency)
		Flag_ref= 1;
	
	switch (Flag_ref)
	{
		case 0:	* ref= Temp1/Wrpm_rated;	break;
		case 1:	* ref= Temp2/(double)P.G01.P03_Rated_frequency; break;
//		case 2:
//		case 3:
		default: * ref= 0.;
	}
	Temp_speed= Temp1;
	Temp_frequency= Temp2;


	if (OP.Run_stop.bit.Local_RUN) * cmd= CMD_RUN; 
	else * cmd= CMD_STOP;
}

void Fieldbus_Processing( int * cmd, double * ref )
{
	* ref= (double)OP.Reference.Fieldbus.Speed/Wrpm_rated; 
	
	if (OP.Run_stop.bit.Fieldbus_RUN)	* cmd= CMD_RUN; 
	else { * cmd= CMD_STOP;}
}





//----------------------------------
// end of file
//----------------------------------
