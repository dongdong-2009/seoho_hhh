#include	<All_Header.h>
#include	<All_Extern_Variables.h>

// 메 PWM에서 확인한다. 
void Fault_Recording(int Fault_code, double Fault_data,char * st)
{
//	Fault_Recording( ERR_OVER_CURRENT_U_PHASE, (double)(Ias),"OC U_phase");
	

	FaultInfo.CODE		= Fault_code;
	FaultInfo.DATA		= Fault_data;
	FaultInfo.CURRENT	= Is_mag;
	FaultInfo.VDC 		= Vdc;
	FaultInfo.RPM 		= Wrpm_det;
	strncpy( FaultInfo.MSG,st,20) ;
}	
#pragma CODE_SECTION(Fault_Processing, "ramfuncs");
void Fault_Processing( )
{
	double Temp;
	static int Vdc_check_Timer= 0, Fault_index= 0;
	static int Gate_driver_reset_counter= 0;


	Init_reference= Final_reference= 0.;
	Wrpm_ref= 0.;
	PWM_ON_OFF(0);	
	Driver_ON=0;
	OP.Run_stop.bit.AUTO_TUNING= 0;
	Auto_tuning_index= 0;
	
	// OP.Run_stop.bit.FAULT_RESET= Data_Registers[2484]; 요렇게 하면 안됨
	// 우리 모두 원인을 찾아보도록 하자
	// 아래 명령대로 하면 쓰기가 됨
	OP.Run_stop.bit.FAULT_RESET= Dummy_comm= Data_Registers[2484];
	switch (Fault_index)
	{
		case 0:
			Flag.Monitoring.bit.FAULT_RESET_COMPLETE= 0;
			DC_CONTACT_OFF; // Main Charge Relay Off
			Fault_count++;
			Fault_index++;

			break;
		case 1:
			if (OP.Run_stop.bit.FAULT_RESET== 1)
			{
				Auto_tuning_index= 0;
				
				Temp= (double)(P.G01.P02_Rated_current_x10)*0.1;
				if ( fabs(Ias) < Temp )	Flag.Fault1.bit.OC_A= 0;
				if ( fabs(Ibs) < Temp )	Flag.Fault1.bit.OC_B= 0;
				if ( fabs(Ics) < Temp )	Flag.Fault1.bit.OC_C= 0;
				if ( Is_mag    < Temp )	Flag.Fault1.bit.OC_MAG= 0;
				
				Flag.Fault1.bit.OL= 0; 

				Temp= P.G01.P08_Supply_voltage * SQRT2; // 입력전압 Set 값을 가져와야함 
				if ((Vdc>(Temp*0.85))&&(Vdc<(Temp*1.1)))	Vdc_check_Timer++;
				else if (Vdc_check_Timer>0) Vdc_check_Timer--;
				if (Vdc_check_Timer>10)	
				{
					Flag.Fault1.bit.INIT_CHARGE= 0;
					Flag.Fault1.bit.OV= 0;
					Flag.Fault1.bit.UV= 0;
				}
				if (fabs(Wrpm_det)<=Wrpm_max)	Flag.Fault1.bit.OVER_SPEED= 0;
				if (GpioDataRegs.GPADAT.bit.GPIO12!=0) 	Flag.Fault1.bit.DEVICE_SHORT= 0;
				if (GpioDataRegs.GPADAT.bit.GPIO13!=0)	
				{	EPwm4Regs.AQCSFRC.bit.CSFA= 0;	Flag.Fault1.bit.DB= 0;	}

				if (Flag.DI.bit.EXT_FAULT_A== 0)	Flag.Fault1.bit.EXT_FAULT_A= 0;
				if (Flag.DI.bit.EXT_FAULT_B== 0)	Flag.Fault1.bit.EXT_FAULT_B= 0;

				// Trip zone clear 가 완료 되면 clear flag 해제
				if ( (!EPwm1Regs.TZFLG.bit.OST)&&(!EPwm2Regs.TZFLG.bit.OST)&&(!EPwm3Regs.TZFLG.bit.OST) )
				{
					EPwm1Regs.TZCLR.bit.OST= 0;
					EPwm2Regs.TZCLR.bit.OST= 0;	
					EPwm3Regs.TZCLR.bit.OST= 0;
					Flag.Fault1.bit.HARDWARE_FAULT= 0;
				}
				else 
				{
					EPwm1Regs.TZCLR.bit.OST= 1;
					EPwm2Regs.TZCLR.bit.OST= 1;	
					EPwm3Regs.TZCLR.bit.OST= 1;
				}

				Fault_index++;
			}
			
			break;
		case 2:
			if ( 	((Flag.Fault1.all & (~Flag.Fault_Neglect1.all)) == 0x0000) 
				&&	((Flag.Fault2.all & (~Flag.Fault_Neglect2.all)) == 0x0000)    ) 
			{
				Gate_driver_reset_counter++;
/*			
				EPwm1Regs.TZCLR.bit.OST= 1;
				EPwm2Regs.TZCLR.bit.OST= 1;	
				EPwm3Regs.TZCLR.bit.OST= 1;
				OP.Run_stop.bit.FAULT_RESET= 0;
				Flag.Monitoring.bit.FAULT_RESET_COMPLETE= 1;
				Fault_index= 0;
				DC_CONTACT_ON; // 메인 전원 on
*/
				// Trip zone clear 가 완료 되면 clear flag 해제
				if ( (!EPwm1Regs.TZFLG.bit.OST)&&(!EPwm2Regs.TZFLG.bit.OST)&&(!EPwm3Regs.TZFLG.bit.OST) )
				{
					EPwm1Regs.TZCLR.bit.OST= 0;
					EPwm2Regs.TZCLR.bit.OST= 0;	
					EPwm3Regs.TZCLR.bit.OST= 0;
				}				
				
				if (Gate_driver_reset_counter>= ((int)(0.010/Tsamp)))
				{
					Gate_driver_reset_counter= 0;

					OP.Run_stop.bit.FAULT_RESET= 0;
					Flag.Monitoring.bit.FAULT_RESET_COMPLETE= 1;
					Fault_index= 0;
					DC_CONTACT_ON; // 메인 전원 on
				}
				else if (Gate_driver_reset_counter== ((int)(0.005/Tsamp)))
					RESET_DRIVER_SET; //316J PWM on
				else if (Gate_driver_reset_counter<= 1) 
					RESET_DRIVER_CLEAR; //316J Reset

	
			}
			else Fault_index--;

			break;
	}


}

#pragma CODE_SECTION(Fault_Check_1Sampling, "ramfuncs");
void Fault_Check_1Sampling()
{
	double Temp;
	static int OV_Timer= 0, UV_Timer= 0;

	Temp= OC_trip_value*(double)(P.G05.P11_Over_current_trip_x1000)*1.e-3;
	if (fabs(Ias) > Temp ) Flag.Fault1.bit.OC_A= 1;
	if (fabs(Ibs) > Temp ) Flag.Fault1.bit.OC_B= 1;
	if (fabs(Ics) > Temp ) Flag.Fault1.bit.OC_C= 1;

	if (Is_mag > Temp )	
	{
		Flag.Fault1.bit.OC_MAG= 1;
		Fault_Recording( 1, (double)(Is_mag),"  OC Mag  ");
//		test4 = 1.;
//		test5 = Temp;
		test6 = Is_mag;
	}
	
//	test4= Temp;
//	test5= Is_mag;
	
	

	Temp= 0.002/Tsamp;
	if (Vdc>(P.G05.P15_Over_voltage_trip))	OV_Timer++;
	else if (OV_Timer>0)	OV_Timer--;
	if (OV_Timer>=Temp)	Flag.Fault1.bit.OV= 1;
	if (Vdc<(P.G05.P18_Under_voltage_trip))	UV_Timer++;
	else if (UV_Timer>0)	UV_Timer--;
	if (UV_Timer>=Temp)	Flag.Fault1.bit.UV= 1;	
 
	if ( EPwm1Regs.TZFLG.bit.OST || EPwm2Regs.TZFLG.bit.OST || EPwm3Regs.TZFLG.bit.OST)
		Flag.Fault1.bit.HARDWARE_FAULT= 1;

	if (GpioDataRegs.GPADAT.bit.GPIO12==0) 	Flag.Fault1.bit.DEVICE_SHORT= 1;
}


#pragma CODE_SECTION(Fault_Check_4Sampling, "ramfuncs");
void Fault_Check_4Sampling()
{
	static int OT_Timer= 0;
	static double OL_new= 0.;


    if (OL_new>=0)
    	OL_new+= (Is_mag*Is_mag - Continuous_OL_current*Continuous_OL_current)*4.*Tsamp;
    if (OL_new>OL_limit)
	{
		Flag.Fault1.bit.OL= 1;
		OL_new= 0;
	}
	
	 
	if ( Temperature_x10> ((double)P.G05.P40_Over_temperature_trip_x10) )	OT_Timer++;
	else if (OT_Timer>0)	OT_Timer--;
	if ( OT_Timer>= (0.002/Tsamp) )	Flag.Fault1.bit.OT= 1;


	//	if (GpioDataRegs.GPADAT.bit.GPIO16==0) 	FaultZCCount++;
//	else if( FaultZCCount > 0)			FaultZCCount--;
//	if (FaultZCCount > 10 )	Flag.Fault.bit.ZC= 1;

}





