#include	<All_Header.h>
#include	<All_Extern_Variables.h>


#pragma CODE_SECTION(Fault_Check_1Sampling, "ramfuncs");
void Fault_Check_1Sampling()
{
//	if (fabs(Ias) > OC_trip_value ) Flag.Fault1.bit.OC_A= 1;
//	if (fabs(Ibs) > OC_trip_value ) Flag.Fault1.bit.OC_B= 1;
//	if (fabs(Ics) > OC_trip_value ) Flag.Fault1.bit.OC_C= 1;
//	if (Is_mag > OC_trip_value )	Flag.Fault1.bit.OC_MAG= 1;
	
	if (fabs(Ias) > OC_trip_value ) Flag.Fault1.bit.OC= 1;
	if (fabs(Ibs) > OC_trip_value ) Flag.Fault1.bit.OC= 1;
	if (fabs(Ics) > OC_trip_value ) Flag.Fault1.bit.OC= 1;
	if (Is_mag > OC_trip_value )	Flag.Fault1.bit.OC= 1;

 
	if ( EPwm1Regs.TZFLG.bit.OST || EPwm2Regs.TZFLG.bit.OST || EPwm3Regs.TZFLG.bit.OST 
		||(GpioDataRegs.GPADAT.bit.GPIO12==0) ) 
		Flag.Fault2.bit.DEVICE_SHORT= 1;
}


#pragma CODE_SECTION(Fault_Check_4Sampling, "ramfuncs");
void Fault_Check_4Sampling()
{
	int Temp;
	static int OT_Timer= 0, OV_Timer= 0, UV_Timer= 0, ZSC_Timer= 0;
	static double OL_new= 0.;


	Temp= (int)(0.002/(Tsamp*4.));
	if (Vdc>(double)(P.G05.P15_Over_voltage_trip))	OV_Timer++;
	else if (OV_Timer>0)	OV_Timer--;
	if (OV_Timer>=Temp)	Flag.Fault1.bit.OV= 1;

	if (Vdc<(double)(P.G05.P18_Under_voltage_trip))	UV_Timer++;
	else if (UV_Timer>0)	UV_Timer--;
	if (UV_Timer>=Temp)	Flag.Fault1.bit.UV= 1;

	else if (OT_Timer>0)	OT_Timer--;
	if (OT_Timer>= Temp)	Flag.Fault2.bit.OT= 1; 

	Temp= (int)(0.01/(Tsamp*4.));	
	if ( (fabs(Ias+Ibs+Ics))>ZSC_limit)	ZSC_Timer++;
	else if (ZSC_Timer>0)	ZSC_Timer--;
	if (ZSC_Timer>=Temp)	Flag.Fault1.bit.ZSC= 1; 

    if (OL_new>=0)
    	OL_new+= (Is_mag*Is_mag - Continuous_OL_current*Continuous_OL_current)*4.*Tsamp;
    if (OL_new>OL_limit)
	{
		Flag.Fault1.bit.OL= 1;
		OL_new= 0;
	}

	if ( Temperature_x10> ((double)P.G05.P40_Over_temperature_trip_x10) )	OT_Timer++;

}
 
#define Fault_Record_Start_Addr	3500
#define Fault_Read_Start_Addr	2390

void RW_Fault_Record()
{
	static int  Write_index= 0, Write_counter= 0, Write_counter2= 0;
	int a;

	switch (Write_index)
	{
		case 0:
			if (Flag.Monitoring.bit.WRITE_FAULT_RECORD== 1)
			{
				if (Fault_count>=50)
				{
					OP.Fault_recording.TOTAL= 50;
					OP.Fault_recording.NOW= Fault_count % 50;
				}
				else	OP.Fault_recording.TOTAL= OP.Fault_recording.NOW= Fault_count;
				Write_index++;
			}
			else if (OP.Fault_recording.READ== 1)
			{
				OP.Fault_recording.NOW= Data_Registers[2382];
				Write_index= 4;
			}
			else if (OP.Fault_recording.CLEAR== 1)
				Write_index= 5;


			break;

		case 1:
			Word_Write_data(2381, OP.Fault_recording.TOTAL);
			Write_index++;
			break;

		case 2:
			Word_Write_data(2382, OP.Fault_recording.NOW);
			Write_index++;
			break;

		case 3:
			if (Write_counter<16)
			{
				// 20개 단위 저장 공간 : 16개 사용중 
				Word_Write_data((Fault_Record_Start_Addr+(OP.Fault_recording.NOW-1)*20 + Write_counter), Data_Registers[2390+Write_counter]);
				Write_counter++;
			}
			else
			{
				Flag.Monitoring.bit.WRITE_FAULT_RECORD= 0;
				Write_counter= 0;
				Write_index= 0;
			}
			break;

		case 4:
			if (Write_counter<16)
			{
				Word_Read_data((Fault_Record_Start_Addr+(OP.Fault_recording.NOW-1)*20 + Write_counter), &Data_Registers[Fault_Read_Start_Addr+Write_counter]);
				Write_counter++;
			}
			else
			{
				OP.Fault_recording.READ= 0;
				Write_counter= 0;
				Write_index= 0;
			}
			break;

		case 5:
			if (Write_counter2<50)
			{
				if (Write_counter<16)
				{
					Word_Write_data((Fault_Record_Start_Addr+Write_counter2*20 + Write_counter), 0);
					Write_counter++;
				}
				else
				{
					Fault_count= 0;

					Word_Write_data(2381, 0);
					Word_Write_data(2382, 0);
					for (a= -1; a<15; a++)
						Data_Registers[2390+a]= 0;

					Write_counter= 0;
					Write_counter2++;
				}

			}
			else
			{
				OP.Fault_recording.CLEAR= 0;
				Write_counter2= 0;
				Write_index= 0;
			}
			break; 
	}

}



// #pragma CODE_SECTION(Fault_Recording, "ramfuncs");
void Fault_Recording()
{
	int i, Temp; 
	static volatile int Fault_number, Fault_bits1_comparison= 0, Fault_bits2_comparison= 0, Fault_bits3_comparison= 0;
	static volatile int Fault_bits1_old= 0, Fault_bits2_old= 0, Fault_bits3_old= 0;

	if (Flag.Monitoring.bit.WRITE_FAULT_RECORD== 0)
	{
		Fault_bits1_comparison= Fault_bits1 & (Fault_bits1 ^ Fault_bits1_old);
		Fault_bits2_comparison= Fault_bits2 & (Fault_bits2 ^ Fault_bits2_old);
		Fault_bits3_comparison= Fault_bits3 & (Fault_bits3 ^ Fault_bits3_old);

		if ( (Fault_bits1_comparison!= 0)||(Fault_bits2_comparison!= 0)||(Fault_bits3_comparison!= 0) )
		{
			Fault_count++;
			if (Fault_count>200) Fault_count-= 200;
			
			if (Fault_bits1_comparison!= 0)
			{
				for (i= 0;i<16;i++)
				{
					Temp= (Fault_bits1_comparison>>(16-i)) & 0x1;
					if (Temp== 1) Fault_number= i;
				}
			}
			else if (Fault_bits2_comparison!= 0)
			{	
				for (i= 0;i<16;i++)
				{
					Temp= (Fault_bits2_comparison>>(16-i)) & 0x1;
					if (Temp== 1) Fault_number= i+16;
				}
			}
			else if (Fault_bits3_comparison!= 0)
			{	
				for (i= 0;i<16;i++)
				{
					Temp= (Fault_bits3_comparison>>(16-i)) & 0x1;
					if (Temp== 1) Fault_number= i+32;
				}
			}

			Data_Registers[2390]=	(int)((time.year<<8)|time.month);
			Data_Registers[2391]=	(int)((time.date<<8)|time.hour);
			Data_Registers[2392]=	(int)((time.minute<<8)|time.second);
			Data_Registers[2393]=	Fault_number;
			Data_Registers[2394]=	State_index_old; 
			Data_Registers[2395]=	(int)Is_mag_rms*10.;
			Data_Registers[2396]=	(int)Vdc;
			Data_Registers[2397]=	(int)Wrpm_det_flt;
			Data_Registers[2398]=	Temperature_x10;
			Data_Registers[2399]=	(int)(Te/Te_rate*1000.);
			Data_Registers[2400]=	(int)(Iqse*10.);
			Data_Registers[2401]=	(int)(Idse*10.);
			Data_Registers[2402]=	(int)(Output_power_x10_kW);
			Data_Registers[2403]=	(int)(Ias*10.);
			Data_Registers[2404]=	(int)(Ibs*10.);
			Data_Registers[2405]=	(int)(Ics*10.);

			Fault_bits1_old= Fault_bits1;
			Fault_bits2_old= Fault_bits2;
			Fault_bits3_old= Fault_bits3;
			Flag.Monitoring.bit.WRITE_FAULT_RECORD= 1;
		}
		else 
		{
			Fault_bits1_old= Fault_bits1;
			Fault_bits2_old= Fault_bits2;
			Fault_bits3_old= Fault_bits3;
//			Fault_number= 0;
		}
	}

}	


#pragma CODE_SECTION(Fault_Processing, "ramfuncs");
void Fault_Processing( )
{
	static int Vdc_check_Timer= 0, Izsc_check_Timer= 0, Fault_index= 0;
	static int Gate_driver_reset_counter= 0;

	Init_reference= Final_reference= 0.;
	Wrpm_ref= 0.;
	PWM_ON_OFF(0);	
	Driver_ON=0;
	OP.Run_stop.bit.MOTOR_TUNING= 0;
	OP.Run_stop.bit.Local_RUN= 0;
	OP.Run_stop.bit.Fieldbus_RUN= 0;
	Flag.DO.bit.MOTOR_BRAKE= 0;
	Auto_tuning_index= INITIALIZATION;
	Fault_bits1= (int)(Flag.Fault1.all & (~Flag.Fault_Neglect1.all));
	Fault_bits2= (int)(Flag.Fault2.all & (~Flag.Fault_Neglect2.all));
	Fault_bits3= (int)(Flag.Fault3.all & (~Flag.Fault_Neglect3.all));
	
	// OP.Run_stop.bit.FAULT_RESET= Data_Registers[2484]; 요렇게 하면 안됨
	// 우리 모두 원인을 찾아보도록 하자
	// 아래 명령대로 하면 쓰기가 됨
	OP.Run_stop.bit.FAULT_RESET= Dummy_comm= Data_Registers[2484];
	switch (Fault_index)
	{
		case 0:

			Fault_Recording();
			Flag.Monitoring.bit.FAULT_RESET_COMPLETE= 0;
			DC_CONTACT_OFF; // Main Charge Relay Off
			
			if ( (OP.Run_stop.bit.FAULT_RESET== 1)||( (Fault_bits1== 0)&&(Fault_bits2== 0)&&(Fault_bits3== 0) ) )
				Fault_index++;

			
			break;
		case 1:

			Auto_tuning_index= INITIALIZATION;
			
//				if ( fabs(Ias) < OC_trip_value )	Flag.Fault1.bit.OC_A= 0;
//				if ( fabs(Ibs) < OC_trip_value )	Flag.Fault1.bit.OC_B= 0;
//				if ( fabs(Ics) < OC_trip_value )	Flag.Fault1.bit.OC_C= 0;
//				if ( Is_mag    < OC_trip_value )	Flag.Fault1.bit.OC_MAG= 0;

			if ( (fabs(Ias)< OC_trip_value)&&(fabs(Ibs)< OC_trip_value)
				&&(fabs(Ics)< OC_trip_value)&&(Is_mag< OC_trip_value) )
				Flag.Fault1.bit.OC= 0;
			
			Flag.Fault1.bit.OL= 0; 

			if ((Vdc>(double)P.G05.P18_Under_voltage_trip)
				&&(Vdc<(double)P.G05.P15_Over_voltage_trip))	
				Vdc_check_Timer++;
			else if (Vdc_check_Timer>0) Vdc_check_Timer--;

			if (Vdc_check_Timer>(int)(0.002/Tsamp))	
			{
				Flag.Fault2.bit.CHAGING_ERROR= 0;
				Flag.Fault1.bit.OV= 0;
				Flag.Fault1.bit.UV= 0;
			}

			if (fabs(Wrpm_det_flt)<=Wrpm_limit)	Flag.Fault1.bit.OVER_SPEED= 0;

			if (GpioDataRegs.GPADAT.bit.GPIO13!=0)	
			{	EPwm4Regs.AQCSFRC.bit.CSFA= 0;	Flag.Fault2.bit.GATE_DRIVER_FAIL= 0;	}

			if ( (Flag.DI.bit.EXT_FAULT_A== 0)&&(Flag.DI.bit.EXT_FAULT_B== 0) )	Flag.Fault2.bit.EXT_FAULT= 0;

			// Trip zone clear 가 완료 되면 clear flag 해제
			if ( (!EPwm1Regs.TZFLG.bit.OST)&&(!EPwm2Regs.TZFLG.bit.OST)&&(!EPwm3Regs.TZFLG.bit.OST) )
			{
				if (GpioDataRegs.GPADAT.bit.GPIO12!=0)
				{
					EPwm1Regs.TZCLR.bit.OST= 0;
					EPwm2Regs.TZCLR.bit.OST= 0;	
					EPwm3Regs.TZCLR.bit.OST= 0;
					Flag.Fault2.bit.DEVICE_SHORT= 0;
				}
			}
			else 
			{
				EPwm1Regs.TZCLR.bit.OST= 1;
				EPwm2Regs.TZCLR.bit.OST= 1;	
				EPwm3Regs.TZCLR.bit.OST= 1;
			}

			if (Rs0> 0.)		Flag.Fault3.bit.Rs_FAIL= 0;
			if (Ls0> 0.)	{	Flag.Fault3.bit.Ls_FAIL= 0; Flag.Fault3.bit.Lr_FAIL= 0; 	}
			if ((Ls0/Tr)> 0.)	Flag.Fault3.bit.Rr_FAIL= 0;

			if ( (fabs(Ias+Ibs+Ics))<ZSC_limit)	Izsc_check_Timer++;
			else if (Izsc_check_Timer>0)	Izsc_check_Timer--;
			if ( Izsc_check_Timer>= (int)(0.01/Tsamp) )	Flag.Fault1.bit.ZSC= 0; 

			Fault_index++;

			
			break;
		case 2:
			if ( (Fault_bits1== 0)&&(Fault_bits2== 0)&&(Fault_bits3== 0) )
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
			else Fault_index= 0;

			break;
	}


}







