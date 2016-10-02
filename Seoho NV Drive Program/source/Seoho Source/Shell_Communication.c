#include	<All_Header.h>
#include	<All_Extern_Variables.h> 


// #pragma CODE_SECTION(Read_Data_Registers, "ramfuncs"); 
void Read_Data_Registers(int Parameter_index)
{
	switch(Parameter_index)
	{
		case 2306:	OP.Run_stop.bit.Local_RUN= 						Data_Registers[Parameter_index];	break; 
		case 2300:	OP.Run_stop.bit.Local=							Data_Registers[Parameter_index];	break;
		case 2301:	OP.Run_stop.bit.Local_DIR= 						Data_Registers[Parameter_index];	break; 
		case 2302:	OP.Reference.Local.Speed= 						Data_Registers[Parameter_index];	break; 
		case 2303:	OP.Reference.Local.Frequency_x10=				Data_Registers[Parameter_index];	break; 
		case 2304:	OP.Reference.Local.Torque_x1000=				Data_Registers[Parameter_index];	break; 
		case 2305:	OP.Reference.Local.PID_x1000= 					Data_Registers[Parameter_index];	break;
		case 2272:	Flag.Monitoring.bit.DRIVE_CAL=					Data_Registers[Parameter_index];	break;
		case 2290:	OP.Run_stop.bit.Fieldbus_RUN= 					Data_Registers[Parameter_index];	break;
		case 2291:	OP.Run_stop.bit.Fieldbus_DIR= 					Data_Registers[Parameter_index];	break;
		case 2292:	OP.Reference.Fieldbus.Speed= 					Data_Registers[Parameter_index];	break; 
		case 2293:	OP.Reference.Fieldbus.Frequency_x10=			Data_Registers[Parameter_index];	break; 
		case 2294:	OP.Reference.Fieldbus.Torque_x1000=				Data_Registers[Parameter_index];	break; 
		case 2295:	OP.Reference.Fieldbus.PID_x1000= 				Data_Registers[Parameter_index];	break;
		case 2307:	OP.Run_stop.bit.Emergency_STOP= 				Data_Registers[Parameter_index];	break;
		case 2370:	OP.Run_stop.bit.DRIVE_CALIBRATION=				Data_Registers[Parameter_index];	break;
		case 2371:	OP.Run_stop.bit.MOTOR_TUNING=					Data_Registers[Parameter_index];	break;
		case 2374:	OP.Fault_recording.READ=						Data_Registers[Parameter_index];	break;
		case 2375:	OP.Fault_recording.CLEAR=						Data_Registers[Parameter_index];	break;
		case 2382:	OP.Fault_recording.NOW=							Data_Registers[Parameter_index];	break;

		// 원인불명 국적불명의 원인으로 인해 Write가 안됨 (이 뭥미??? - 나도 모르겠음) 함수로 직접이동
//		case 2484:	OP.Run_stop.bit.FAULT_RESET= 					Data_Registers[Parameter_index];	break;

		case 200:	P.G00.P00_Device_number=	 					Data_Registers[Parameter_index];	break;
		case 201:	P.G00.P01_Software_version1=					Data_Registers[Parameter_index];	break;
		case 202:	P.G00.P02_Software_version2=					Data_Registers[Parameter_index];	break;
		case 203:	P.G00.P03_Software_version3=					Data_Registers[Parameter_index];	break;
		case 204:	P.G00.P04_Ias_offset1=							Data_Registers[Parameter_index];	break;
		case 205:	P.G00.P05_Ias_offset2=							Data_Registers[Parameter_index];	break;
		case 206:	P.G00.P06_Ibs_offset1=							Data_Registers[Parameter_index];	break;
		case 207:	P.G00.P07_Ibs_offset2=							Data_Registers[Parameter_index];	break;
		case 208:	P.G00.P08_Password= 							Data_Registers[Parameter_index];	break;
		case 209:	P.G00.P09_IGBT_current_x10= 					Data_Registers[Parameter_index];	break;
		case 210:	P.G00.P10_Drive_power_x10_kW= 					Data_Registers[Parameter_index];	break;	

		case 240:	P.G01.P00_Rated_power_x10_kW= 					Data_Registers[Parameter_index];	break;
		case 241:	P.G01.P01_Rated_voltage= 						Data_Registers[Parameter_index];	break;
		case 242:	P.G01.P02_Rated_current_x10= 					Data_Registers[Parameter_index];	break;
		case 243:	P.G01.P03_Rated_frequency= 						Data_Registers[Parameter_index];	break;
		case 244:	P.G01.P04_Number_of_poles= 						Data_Registers[Parameter_index];	break;	
		case 245:	P.G01.P05_Rated_speed= 							Data_Registers[Parameter_index];	break;	
		case 246:	P.G01.P06_Control_method= 						Data_Registers[Parameter_index];	break;
		case 247:	P.G01.P07_PWM_frequency_x10_kHz=				Data_Registers[Parameter_index];	break;
		case 249:	P.G01.P09_Supply_voltage=						Data_Registers[Parameter_index];	break;	
														
		case 280:	P.G03.P00_Run_stop_source= 						Data_Registers[Parameter_index];	break;
		case 281:	P.G03.P01_Reference_source= 					Data_Registers[Parameter_index];	break;		
//		case 283:	P.G03.P03_Stop_mode= 							Data_Registers[Parameter_index];	break;				
//		case 284:	P.G03.P04_Stop_hold_time= 						Data_Registers[Parameter_index];	break;		
//		case 285:	P.G03.P05_Output_off_hold_time= 				Data_Registers[Parameter_index];	break;
		case 289:	P.G03.P09_Accel_switching_ref1_x1000= 			Data_Registers[Parameter_index];	break;		
//		case 290:	P.G03.P10_Accel_switching_ref2_x1000= 			Data_Registers[Parameter_index];	break;			
		case 296:	P.G03.P16_Accel_time1_x10= 						Data_Registers[Parameter_index];	break;			
		case 297:	P.G03.P17_Accel_time2_x10= 						Data_Registers[Parameter_index];	break;					
		case 306:	P.G03.P26_Decel_switching_ref1_x1000= 			Data_Registers[Parameter_index];	break;	
//		case 307:	P.G03.P27_Decel_switching_ref2_x1000= 			Data_Registers[Parameter_index];	break;		
		case 313:	P.G03.P33_Decel_time1_x10= 						Data_Registers[Parameter_index];	break;						 
		case 314:	P.G03.P34_Decel_time2_x10= 						Data_Registers[Parameter_index];	break;				
//		case 322:	P.G03.P42_Conunter_deceleration_ramp= 			Data_Registers[Parameter_index];	break;
//		case 323:	P.G03.P43_Conunter_deceleration_time_x10= 		Data_Registers[Parameter_index];	break;	
//		case 329:	P.G03.P49_Emergency_stop_mode= 					Data_Registers[Parameter_index];	break;		
//		case 330:	P.G03.P50_Emergency_stop_decel_time_x10= 		Data_Registers[Parameter_index];	break;	
		case 331:	P.G03.P53_S_curve_enable= 						Data_Registers[Parameter_index];	break;			
		case 332:	P.G03.P54_S_curve_weight_x1000=			 		Data_Registers[Parameter_index];	break;

		case 500:	P.G05.P00_Current_limit_x1000= 					Data_Registers[Parameter_index];	break;					
		case 507:	P.G05.P07_Max_continuous_current_x1000= 		Data_Registers[Parameter_index];	break;
		case 508:	P.G05.P08_Overload_current_x1000= 				Data_Registers[Parameter_index];	break;			
		case 509:	P.G05.P09_Over_load_time_x10= 					Data_Registers[Parameter_index];	break;			
//		case 510:	P.G05.P10_Over_load_fault_action= 				Data_Registers[Parameter_index];	break;	
		case 511:	P.G05.P11_Over_current_trip_x1000= 				Data_Registers[Parameter_index];	break;		
		case 512:	P.G05.P12_Zero_sequence_current_trip_x1000=		Data_Registers[Parameter_index];	break;
//		case 513:	P.G05.P13_Over_voltage_limiting_function= 		Data_Registers[Parameter_index];	break;	
//		case 514:	P.G05.P14_Over_voltage_limit= 					Data_Registers[Parameter_index];	break;			
		case 515:	P.G05.P15_Over_voltage_trip= 					Data_Registers[Parameter_index];	break;			 
//		case 516:	P.G05.P16_Under_voltage_compensation= 			Data_Registers[Parameter_index];	break;	
//		case 517:	P.G05.P17_UV_compensation_voltage= 				Data_Registers[Parameter_index];	break;		
		case 518:	P.G05.P18_Under_voltage_trip= 					Data_Registers[Parameter_index];	break;
//		case 519:	P.G05.P19_Open_phase_protection= 				Data_Registers[Parameter_index];	break;	
//		case 520:	P.G05.P20_Supply_frequency= 					Data_Registers[Parameter_index];	break;		
//		case 521:	P.G05.P21_Built_in_dynamic_brake= 				Data_Registers[Parameter_index];	break;		
//		case 522:	P.G05.P22_DB_switching_frequency_x10=			Data_Registers[Parameter_index];	break;	
		case 523:	P.G05.P23_DB_start_voltage= 					Data_Registers[Parameter_index];	break;				
		case 524:	P.G05.P24_DB_full_voltage= 						Data_Registers[Parameter_index];	break;
//		case 525:	P.G05.P25_Over_temperature_trip_action=	 		Data_Registers[Parameter_index];	break;
//		case 530:	P.G05.P30_Auto_restart_count= 					Data_Registers[Parameter_index];	break;		
//		case 531:	P.G05.P31_Retry_delay_time_x10= 				Data_Registers[Parameter_index];	break;
//		case 532:	P.G05.P32_OC_auto_reset= 						Data_Registers[Parameter_index];	break;			
//		case 533:	P.G05.P33_OV_auto_reset= 						Data_Registers[Parameter_index];	break;	
//		case 534:	P.G05.P34_UV_auto_reset= 						Data_Registers[Parameter_index];	break;			
//		case 537:	P.G05.P37_Out_of_control_auto_reset= 			Data_Registers[Parameter_index];	break;	
//		case 538:	P.G05.P38_Out_of_control_time_x10= 				Data_Registers[Parameter_index];	break;		
//		case 539:	P.G05.P39_Out_of_control_current_x1000= 		Data_Registers[Parameter_index];	break;	
		case 540:	P.G05.P40_Over_temperature_trip_x10= 			Data_Registers[Parameter_index];	break;	
//		case 542:	P.G05.P42_Line_under_voltage_trip_x1000= 		Data_Registers[Parameter_index];	break;
//		case 543:	P.G05.P43_Line_under_voltage_auto_reset= 		Data_Registers[Parameter_index];	break;
//		case 544:	P.G05.P44_Line_unbalance_auto_reset= 			Data_Registers[Parameter_index];	break;	

		case 600:	P.G06.P00_Analog_reference_source= 				Data_Registers[Parameter_index];	break;
		case 601:	P.G06.P01_AI1_Function= 						Data_Registers[Parameter_index];	break;				
		case 602:	P.G06.P02_AI1_Type= 							Data_Registers[Parameter_index];	break;				
		case 603:	P.G06.P03_AI1_Filter_time_constant_ms=	 		Data_Registers[Parameter_index];	break;	
		case 604:	P.G06.P04_AI1_Offset_x10= 						Data_Registers[Parameter_index];	break;	
		case 605:	P.G06.P05_AI1_Min_value_x10= 					Data_Registers[Parameter_index];	break;
		case 606:	P.G06.P06_AI1_Max_value_x10= 					Data_Registers[Parameter_index];	break;	
		case 607:	P.G06.P07_AI1_Min_scale_x1000= 					Data_Registers[Parameter_index];	break;	
		case 608:	P.G06.P08_AI1_Max_scale_x1000= 					Data_Registers[Parameter_index];	break;
		case 609:	P.G06.P09_AI1_Inversion= 						Data_Registers[Parameter_index];	break;				
		case 610:	P.G06.P10_AI1_Discretness= 						Data_Registers[Parameter_index];	break;	
		case 611:	P.G06.P11_AI1_Unit_selection= 					Data_Registers[Parameter_index];	break;			
		case 612:	P.G06.P12_AI2_Function= 						Data_Registers[Parameter_index];	break;				
		case 613:	P.G06.P13_AI2_Type= 							Data_Registers[Parameter_index];	break;					
		case 614:	P.G06.P14_AI2_Filter_time_constant_ms= 			Data_Registers[Parameter_index];	break;	
		case 615:	P.G06.P15_AI2_Offset_x10= 						Data_Registers[Parameter_index];	break;			
		case 616:	P.G06.P16_AI2_Min_value_x10= 					Data_Registers[Parameter_index];	break;				
		case 617:	P.G06.P17_AI2_Max_value_x10= 					Data_Registers[Parameter_index];	break;	
		case 618:	P.G06.P18_AI2_Min_scale_x1000= 					Data_Registers[Parameter_index];	break;			
		case 619:	P.G06.P19_AI2_Max_scale_x1000= 					Data_Registers[Parameter_index];	break;		
		case 620:	P.G06.P20_AI2_Inversion= 						Data_Registers[Parameter_index];	break;			
		case 621:	P.G06.P21_AI2_Discretness= 						Data_Registers[Parameter_index];	break;	
		case 622:	P.G06.P22_AI2_Unit_selection= 					Data_Registers[Parameter_index];	break;	

//		case 750:	P.G07.P00_Control_mode= 						Data_Registers[Parameter_index];	break;	

		case 810:	P.G08.P00_Run_stop_control= 					Data_Registers[Parameter_index];	break;			
		case 811:	P.G08.P01_DI03_function= 						Data_Registers[Parameter_index];	break;
		case 812:	P.G08.P02_DI04_function= 						Data_Registers[Parameter_index];	break;				
		case 813:	P.G08.P03_DI05_function= 						Data_Registers[Parameter_index];	break;						
		case 814:	P.G08.P04_DI06_function= 						Data_Registers[Parameter_index];	break;			
		case 815:	P.G08.P05_DI07_function= 						Data_Registers[Parameter_index];	break;					
		case 816:	P.G08.P06_DI08_function= 						Data_Registers[Parameter_index];	break;					
//		case 817:	P.G08.P07_DI09_function= 						Data_Registers[Parameter_index];	break;				
//		case 818:	P.G08.P08_DI10_function= 						Data_Registers[Parameter_index];	break;			
//		case 819:	P.G08.P09_DI11_function= 						Data_Registers[Parameter_index];	break;					
//		case 820:	P.G08.P10_DI12_function= 						Data_Registers[Parameter_index];	break;				
//		case 828:	P.G08.P11_Run_delay_time= 						Data_Registers[Parameter_index];	break;

		case 860:	P.G09.P00_JOG_set_x1000=						Data_Registers[Parameter_index];	break;				
		case 861:	P.G09.P01_Step01_set_x10= 						Data_Registers[Parameter_index];	break;				
		case 862:	P.G09.P02_Step02_set_x10= 						Data_Registers[Parameter_index];	break;			
		case 863:	P.G09.P03_Step03_set_x10= 						Data_Registers[Parameter_index];	break;		
		case 864:	P.G09.P04_Step04_set_x10= 						Data_Registers[Parameter_index];	break;				
		case 865:	P.G09.P05_Step05_set_x10= 						Data_Registers[Parameter_index];	break;					
		case 866:	P.G09.P06_Step06_set_x10= 						Data_Registers[Parameter_index];	break;			
		case 867:	P.G09.P07_Step07_set_x10= 						Data_Registers[Parameter_index];	break;						
		case 868:	P.G09.P08_Step08_set_x10= 						Data_Registers[Parameter_index];	break;				
		case 869:	P.G09.P09_Step09_set_x10=						Data_Registers[Parameter_index];	break;				
		case 870:	P.G09.P10_Step10_set_x10= 						Data_Registers[Parameter_index];	break;				
		case 871:	P.G09.P11_Step11_set_x10= 						Data_Registers[Parameter_index];	break;					
		case 872:	P.G09.P12_Step12_set_x10= 						Data_Registers[Parameter_index];	break;			
		case 873:	P.G09.P13_Step13_set_x10= 						Data_Registers[Parameter_index];	break;				
		case 874:	P.G09.P14_Step14_set_x10= 						Data_Registers[Parameter_index];	break;			
		case 875:	P.G09.P15_Step15_set_x10= 						Data_Registers[Parameter_index];	break;				
		case 876:	P.G09.P16_Unit_selection= 						Data_Registers[Parameter_index];	break;		

		case 940:	P.G11.AO1.P00_Output_selection= 				Data_Registers[Parameter_index];	break;		
		case 941:	P.G11.AO1.P01_Type= 							Data_Registers[Parameter_index];	break;
//		case 942:	P.G11.AO1.P02_Adjustment_0mA= 					Data_Registers[Parameter_index];	break;
//		case 943:	P.G11.AO1.P03_Adjustment_4mA= 					Data_Registers[Parameter_index];	break;
//		case 944:	P.G11.AO1.P04_Adjustment_20mA= 					Data_Registers[Parameter_index];	break;
		case 945:	P.G11.AO1.P05_Max_output_x1000= 				Data_Registers[Parameter_index];	break;		
//		case 946:	P.G11.AO1.P06_Inversion= 						Data_Registers[Parameter_index];	break;			
//		case 947:	P.G11.AO2.P00_Output_selection= 				Data_Registers[Parameter_index];	break;			
//		case 948:	P.G11.AO2.P01_Type= 							Data_Registers[Parameter_index];	break;
//		case 949:	P.G11.AO2.P02_Adjustment_0mA= 					Data_Registers[Parameter_index];	break;
//		case 950:	P.G11.AO2.P03_Adjustment_4mA= 					Data_Registers[Parameter_index];	break;
//		case 951:	P.G11.AO2.P04_Adjustment_20mA=					Data_Registers[Parameter_index];	break;
//		case 952:	P.G11.AO2.P05_Max_output_x1000= 				Data_Registers[Parameter_index];	break;
//		case 953:	P.G11.AO2.P06_Inversion= 						Data_Registers[Parameter_index];	break;
				
		case 960:	P.G12.P00_DO1_function= 						Data_Registers[Parameter_index];	break;						
		case 961:	P.G12.P00_DO2_function= 						Data_Registers[Parameter_index];	break;	
		case 962:	P.G12.P00_DO3_function= 						Data_Registers[Parameter_index];	break;	

		case 980:	P.G13.P00_M1_locked_state_up_speed_set_x1000=	Data_Registers[Parameter_index];	break;
		case 981:	P.G13.P01_M1_locked_state_down_speed_set_x1000=	Data_Registers[Parameter_index];	break;	
		case 982:	P.G13.P02_M1_break_open_current_x1000=			Data_Registers[Parameter_index];	break;	
		case 983:	P.G13.P03_M1_start_delay_time_x10=				Data_Registers[Parameter_index];	break;
		case 984:	P.G13.P04_M1_break_close_speed_set_x1000=		Data_Registers[Parameter_index];	break;
		case 985:	P.G13.P05_M1_break_open_torque_build_time_x10=	Data_Registers[Parameter_index];	break;	

		case 1019:	P.G14.P09_Dead_time_compansation_x100_us=		Data_Registers[Parameter_index];	break;

		case 1370:	P.G19.P00_Number_of_encoder_pulses= 			Data_Registers[Parameter_index];	break;		
		case 1371:	P.G19.P01_Inversion_of_PG_direction= 			Data_Registers[Parameter_index];	break;	
		case 1373:	P.G19.P03_Minimum_speed= 						Data_Registers[Parameter_index];	break;			
		case 1374:	P.G19.P04_Maximum_speed_x1000= 					Data_Registers[Parameter_index];	break;			
		case 1375:	P.G19.P05_Over_speed_limit_x1000= 				Data_Registers[Parameter_index];	break;			

		case 1550:	P.G21.P00_Stator_resistance_x1000= 				Data_Registers[Parameter_index];	break;
		case 1552:	P.G21.P01_Rotor_resistance_x1000= 				Data_Registers[Parameter_index];	break;
		case 1553:	P.G21.P02_Stator_inductance_x1000= 				Data_Registers[Parameter_index];	break;
		case 1554:	P.G21.P03_Rotor_inductance_x1000= 				Data_Registers[Parameter_index];	break;
		case 1555:	P.G21.P04_Stator_transient_inductance_x1000=	Data_Registers[Parameter_index];	break;	
		case 1556:	P.G21.P05_Inertia_x1000= 						Data_Registers[Parameter_index];	break;

	}

}

#pragma CODE_SECTION(Write_Data_Registers_Online, "ramfuncs"); 
void Write_Data_Registers_Online()
{
	static int Write_counter= 0;
	
	Write_counter++;
	 		Data_Registers[2310]= 	(int)Wrpm_det_flt;				
			Data_Registers[2311]= 	(int)((Wrpm_det_flt/Wrpm_rated)*(double)P.G01.P03_Rated_frequency*10.); 						

			Data_Registers[2314]= 	(int)(sqrt(Vdss_ref*Vdss_ref+Vqss_ref*Vqss_ref)*SQRT2); 
			Data_Registers[2312]= 	(int)(Vdc);
			Data_Registers[2313]= 	(int)(Is_mag_rms*10.); 

			Data_Registers[2315]= 	(int)(Te/Te_rate*1000.);
			Data_Registers[2316]= 	(int)(Iqse*10.);
			Data_Registers[2317]= 	(int)(Idse*10.);

	if (Write_counter & 0x1)
	{
 		if (Write_counter & 0x2)
		{
//	 		Data_Registers[2310]= 	(int)Wrpm_det_flt;				
//			Data_Registers[2311]= 	(int)((Wrpm_det_flt/Wrpm_rated)*(double)P.G01.P03_Rated_frequency*10.); 						

//			Data_Registers[2314]= 	(int)(sqrt(Vdss_ref*Vdss_ref+Vqss_ref*Vqss_ref)*SQRT2);


		}
		else
		{
//			Data_Registers[2312]= 	(int)(Vdc);
//			Data_Registers[2313]= 	(int)(Is_mag_rms*10.); 

//			Data_Registers[2315]= 	(int)(Te/Te_rate*1000.);
//			Data_Registers[2316]= 	(int)(Iqse*10.);
//			Data_Registers[2317]= 	(int)(Idse*10.);
			Data_Registers[2318]= 	(int)(Input_power_x10_kW);			
			Data_Registers[2319]= 	(int)(Output_power_x10_kW);	
			Data_Registers[2323]= 	Temperature_x10; 		
		}

	}
	else 
	{
		if (Write_counter & 0x2)
		{
			Data_Registers[204]=	P.G00.P04_Ias_offset1;
			Data_Registers[205]=	P.G00.P05_Ias_offset2;
			Data_Registers[206]=	P.G00.P06_Ibs_offset1;
			Data_Registers[207]=	P.G00.P07_Ibs_offset2;
			
			Data_Registers[1550]= 	P.G21.P00_Stator_resistance_x1000; 			
			Data_Registers[1552]= 	P.G21.P01_Rotor_resistance_x1000; 			
			Data_Registers[1553]= 	P.G21.P02_Stator_inductance_x1000; 			
			Data_Registers[1554]= 	P.G21.P03_Rotor_inductance_x1000; 			
			Data_Registers[1555]= 	P.G21.P04_Stator_transient_inductance_x1000;
			Data_Registers[1556]=	P.G21.P05_Inertia_x1000;

			Data_Registers[2270]=	Flag.Monitoring.bit.RUN_STOP_STATUS;	
			Data_Registers[2271]=	Flag.Monitoring.bit.DIR_STATUS;
			Data_Registers[2272]=	Flag.Monitoring.bit.DRIVE_CAL;


		}
		else
		{
			Data_Registers[2291]=	OP.Run_stop.bit.Fieldbus_RUN;
			Data_Registers[2306]=	OP.Run_stop.bit.Local_RUN;
			Data_Registers[2330]=	State_index;

			Data_Registers[2340]=	DI_bit_all;
			Data_Registers[2341]=	DO_bit_all;

			Data_Registers[2370]=	OP.Run_stop.bit.DRIVE_CALIBRATION;
			Data_Registers[2371]=	OP.Run_stop.bit.MOTOR_TUNING;

			Data_Registers[2374]=	OP.Fault_recording.READ;
			Data_Registers[2375]=	OP.Fault_recording.CLEAR;
			Data_Registers[2376]=	Flag.Fault1.all;
			Data_Registers[2377]=	Flag.Fault2.all;
			Data_Registers[2378]=	Flag.Fault3.all;
			Data_Registers[2379]= 	Fault_count;
			Data_Registers[2381]= 	OP.Fault_recording.TOTAL;
			Data_Registers[2382]= 	OP.Fault_recording.NOW;

			Data_Registers[2484]=	OP.Run_stop.bit.FAULT_RESET;
			Data_Registers[2485]=	Flag.Monitoring.bit.FAULT_RESET_COMPLETE; 
		}
	}

	
	
	
}

void Write_Data_Registers_Offline(int Parameter_index)
{
	int Temp;
	
	switch(Parameter_index)
	{
		case 2306:	Temp=	OP.Run_stop.bit.Local_RUN;						break; 
		case 2300:	Temp=	OP.Run_stop.bit.Local;							break;
		case 2301:	Temp=	OP.Run_stop.bit.Local_DIR;						break; 
		case 2302:	Temp=	OP.Reference.Local.Speed;						break; 
		case 2303:	Temp=	OP.Reference.Local.Frequency_x10;				break; 
		case 2304:	Temp=	OP.Reference.Local.Torque_x1000;				break; 
		case 2305:	Temp=	OP.Reference.Local.PID_x1000;					break;
		case 2272:	Temp=	Flag.Monitoring.bit.DRIVE_CAL;					break;
		case 2290:	Temp=	OP.Run_stop.bit.Fieldbus_RUN;					break;
		case 2291:	Temp=	OP.Run_stop.bit.Fieldbus_DIR;					break;
		case 2292:	Temp=	OP.Reference.Fieldbus.Speed;					break; 
		case 2293:	Temp=	OP.Reference.Fieldbus.Frequency_x10;			break; 
		case 2294:	Temp=	OP.Reference.Fieldbus.Torque_x1000;				break; 
		case 2295:	Temp=	OP.Reference.Fieldbus.PID_x1000;				break;
		case 2307:	Temp=	OP.Run_stop.bit.Emergency_STOP;					break;
		case 2370:	Temp=	OP.Run_stop.bit.DRIVE_CALIBRATION;				break;
		case 2371:	Temp=	OP.Run_stop.bit.MOTOR_TUNING;					break;
		case 2374:	Temp=	OP.Fault_recording.READ;						break;
		case 2375:	Temp=	OP.Fault_recording.CLEAR;						break;
		case 2484:	Temp=	OP.Run_stop.bit.FAULT_RESET;					break;
		case 2376:	Temp=	Flag.Fault1.all;								break;
		case 2377:	Temp=	Flag.Fault2.all;								break;
		case 2378:	Temp=	Flag.Fault3.all;								break;
		case 2379:	Temp=	Fault_count;									break;
		case 2381:	Temp=	OP.Fault_recording.TOTAL;						break;
		case 2382:	Temp=	OP.Fault_recording.NOW;							break;

		case 200:	Temp=	P.G00.P00_Device_number;						break;
		case 201:	Temp=	P.G00.P01_Software_version1;					break;
		case 202:	Temp=	P.G00.P02_Software_version2;					break;
		case 203:	Temp=	P.G00.P03_Software_version3;					break;
		case 204:	Temp=	P.G00.P04_Ias_offset1;							break;
		case 205:	Temp=	P.G00.P05_Ias_offset2;							break;
		case 206:	Temp=	P.G00.P06_Ibs_offset1;							break;
		case 207:	Temp=	P.G00.P07_Ibs_offset2;							break;
		case 208:	Temp= 	P.G00.P08_Password; 							break;
		case 209:	Temp= 	P.G00.P09_IGBT_current_x10; 					break;
		case 210:	Temp= 	P.G00.P10_Drive_power_x10_kW;					break;

		case 240:	Temp= 	P.G01.P00_Rated_power_x10_kW; 					break;
		case 241:	Temp= 	P.G01.P01_Rated_voltage; 						break;
		case 242:	Temp= 	P.G01.P02_Rated_current_x10; 					break;
		case 243:	Temp= 	P.G01.P03_Rated_frequency; 						break;
		case 244:	Temp= 	P.G01.P04_Number_of_poles; 						break;
		case 245:	Temp= 	P.G01.P05_Rated_speed; 							break;
		case 246:	Temp= 	P.G01.P06_Control_method; 						break;
		case 247:	Temp= 	P.G01.P07_PWM_frequency_x10_kHz;				break;
		case 249:	Temp= 	P.G01.P09_Supply_voltage;						break; 
															
		case 280:	Temp= 	P.G03.P00_Run_stop_source; 						break; 	
		case 281:	Temp= 	P.G03.P01_Reference_source; 					break;		
//		case 283:	Temp= 	P.G03.P03_Stop_mode; 							break;				
//		case 284:	Temp= 	P.G03.P04_Stop_hold_time; 						break;		
//		case 285:	Temp= 	P.G03.P05_Output_off_hold_time; 				break;		
		case 289:	Temp= 	P.G03.P09_Accel_switching_ref1_x1000; 			break;		
//		case 290:	Temp= 	P.G03.P10_Accel_switching_ref2_x1000; 			break;		
		case 296:	Temp= 	P.G03.P16_Accel_time1_x10; 						break;				
		case 297:	Temp= 	P.G03.P17_Accel_time2_x10; 						break;				
		case 306:	Temp= 	P.G03.P26_Decel_switching_ref1_x1000; 			break;		
//		case 307:	Temp= 	P.G03.P27_Decel_switching_ref2_x1000; 			break;		
		case 313:	Temp= 	P.G03.P33_Decel_time1_x10; 						break;					 
		case 314:	Temp= 	P.G03.P34_Decel_time2_x10; 						break;				
//		case 322:	Temp= 	P.G03.P42_Conunter_deceleration_ramp; 			break;
//		case 323:	Temp= 	P.G03.P43_Conunter_deceleration_time_x10;	 	break;
//		case 329:	Temp= 	P.G03.P49_Emergency_stop_mode; 					break;		
//		case 330:	Temp= 	P.G03.P50_Emergency_stop_decel_time_x10; 		break;
		case 331:	Temp= 	P.G03.P53_S_curve_enable;				 		break;
		case 332:	Temp= 	P.G03.P54_S_curve_weight_x1000;			 		break;
			
		case 500:	Temp= 	P.G05.P00_Current_limit_x1000; 					break;				
		case 507:	Temp= 	P.G05.P07_Max_continuous_current_x1000; 		break;	
		case 508:	Temp= 	P.G05.P08_Overload_current_x1000; 				break;			
		case 509:	Temp= 	P.G05.P09_Over_load_time_x10; 					break;			
//		case 510:	Temp= 	P.G05.P10_Over_load_fault_action; 				break;		
		case 511:	Temp= 	P.G05.P11_Over_current_trip_x1000; 				break;	
		case 512:	Temp= 	P.G05.P12_Zero_sequence_current_trip_x1000; 	break;	
//		case 513:	Temp= 	P.G05.P13_Over_voltage_limiting_function; 		break;
//		case 514:	Temp= 	P.G05.P14_Over_voltage_limit; 					break;			
		case 515:	Temp= 	P.G05.P15_Over_voltage_trip; 					break;		 
//		case 516:	Temp= 	P.G05.P16_Under_voltage_compensation; 			break;	
//		case 517:	Temp= 	P.G05.P17_UV_compensation_voltage; 				break;		
		case 518:	Temp= 	P.G05.P18_Under_voltage_trip; 					break; 		
//		case 519:	Temp= 	P.G05.P19_Open_phase_protection; 				break;		
//		case 520:	Temp= 	P.G05.P20_Supply_frequency; 					break;				
//		case 521:	Temp= 	P.G05.P21_Built_in_dynamic_brake; 				break;		
//		case 522:	Temp= 	P.G05.P22_DB_switching_frequency_x10;			break;		
		case 523:	Temp= 	P.G05.P23_DB_start_voltage; 					break;			
		case 524:	Temp= 	P.G05.P24_DB_full_voltage; 						break;			
//		case 525:	Temp= 	P.G05.P25_Over_temperature_trip_action; 		break;
//		case 530:	Temp= 	P.G05.P30_Auto_restart_count; 					break;		
//		case 531:	Temp= 	P.G05.P31_Retry_delay_time_x10; 				break;	
//		case 532:	Temp= 	P.G05.P32_OC_auto_reset; 						break;			
//		case 533:	Temp= 	P.G05.P33_OV_auto_reset; 						break;			
//		case 534:	Temp= 	P.G05.P34_UV_auto_reset; 						break;					
//		case 537:	Temp= 	P.G05.P37_Out_of_control_auto_reset; 			break;		
//		case 538:	Temp= 	P.G05.P38_Out_of_control_time_x10; 				break;		
//		case 539:	Temp= 	P.G05.P39_Out_of_control_current_x1000; 		break;
		case 540:	Temp= 	P.G05.P40_Over_temperature_trip_x10; 			break;		
//		case 542:	Temp= 	P.G05.P42_Line_under_voltage_trip_x1000; 		break;		
//		case 543:	Temp= 	P.G05.P43_Line_under_voltage_auto_reset; 		break;	
//		case 544:	Temp= 	P.G05.P44_Line_unbalance_auto_reset; 			break;	

		case 600:	Temp= 	P.G06.P00_Analog_reference_source; 				break;	
		case 601:	Temp= 	P.G06.P01_AI1_Function; 						break;			
		case 602:	Temp= 	P.G06.P02_AI1_Type; 							break;					
		case 603:	Temp= 	P.G06.P03_AI1_Filter_time_constant_ms; 			break;
		case 604:	Temp= 	P.G06.P04_AI1_Offset_x10; 						break;				
		case 605:	Temp= 	P.G06.P05_AI1_Min_value_x10; 					break;				
		case 606:	Temp= 	P.G06.P06_AI1_Max_value_x10; 					break;	
		case 607:	Temp= 	P.G06.P07_AI1_Min_scale_x1000; 					break;				
		case 608:	Temp= 	P.G06.P08_AI1_Max_scale_x1000; 					break;				
		case 609:	Temp= 	P.G06.P09_AI1_Inversion; 						break;				
		case 610:	Temp= 	P.G06.P10_AI1_Discretness; 						break;			
		case 611:	Temp= 	P.G06.P11_AI1_Unit_selection; 					break;	
		case 612:	Temp= 	P.G06.P12_AI2_Function; 						break;			
		case 613:	Temp= 	P.G06.P13_AI2_Type; 							break;					
		case 614:	Temp= 	P.G06.P14_AI2_Filter_time_constant_ms;	 		break;
		case 615:	Temp= 	P.G06.P15_AI2_Offset_x10; 						break;				
		case 616:	Temp= 	P.G06.P16_AI2_Min_value_x10; 					break;				
		case 617:	Temp= 	P.G06.P17_AI2_Max_value_x10; 					break;	
		case 618:	Temp= 	P.G06.P18_AI2_Min_scale_x1000; 					break;				
		case 619:	Temp= 	P.G06.P19_AI2_Max_scale_x1000; 					break;				
		case 620:	Temp= 	P.G06.P20_AI2_Inversion; 						break;				
		case 621:	Temp= 	P.G06.P21_AI2_Discretness; 						break;			
		case 622:	Temp= 	P.G06.P22_AI2_Unit_selection; 					break;				
		

//		case 750:	Temp= 	P.G07.P00_Control_mode; 						break;			

		case 810:	Temp= 	P.G08.P00_Run_stop_control; 					break;			
		case 811:	Temp= 	P.G08.P01_DI03_function; 						break;	
		case 812:	Temp= 	P.G08.P02_DI04_function; 						break;				
		case 813:	Temp= 	P.G08.P03_DI05_function; 						break;						
		case 814:	Temp=	P.G08.P04_DI06_function; 						break;				
		case 815:	Temp= 	P.G08.P05_DI07_function; 						break;					
		case 816:	Temp= 	P.G08.P06_DI08_function; 						break;					
//		case 817:	Temp= 	P.G08.P07_DI09_function; 						break;						
//		case 818:	Temp= 	P.G08.P08_DI10_function; 						break;				
//		case 819:	Temp= 	P.G08.P09_DI11_function; 						break;						
//		case 820:	Temp= 	P.G08.P10_DI12_function; 						break;				
//		case 828:	Temp= 	P.G08.P11_Run_delay_time; 						break;			

		case 860:	Temp= 	P.G09.P00_JOG_set_x1000;						break;					
		case 861:	Temp= 	P.G09.P01_Step01_set_x10; 						break;					
		case 862:	Temp= 	P.G09.P02_Step02_set_x10; 						break;				
		case 863:	Temp= 	P.G09.P03_Step03_set_x10; 						break;				
		case 864:	Temp= 	P.G09.P04_Step04_set_x10; 						break;					
		case 865:	Temp=	P.G09.P05_Step05_set_x10; 						break;					
		case 866:	Temp= 	P.G09.P06_Step06_set_x10; 						break;				
		case 867:	Temp= 	P.G09.P07_Step07_set_x10; 						break;						
		case 868:	Temp= 	P.G09.P08_Step08_set_x10; 						break;				
		case 869:	Temp= 	P.G09.P09_Step09_set_x10;						break;						
		case 870:	Temp= 	P.G09.P10_Step10_set_x10; 						break;					
		case 871:	Temp= 	P.G09.P11_Step11_set_x10; 						break;							
		case 872:	Temp= 	P.G09.P12_Step12_set_x10; 						break;				
		case 873:	Temp= 	P.G09.P13_Step13_set_x10; 						break;					
		case 874:	Temp= 	P.G09.P14_Step14_set_x10; 						break;				
		case 875:	Temp= 	P.G09.P15_Step15_set_x10; 						break;				
		case 876:	Temp= 	P.G09.P16_Unit_selection; 						break;			

		case 940:	Temp= 	P.G11.AO1.P00_Output_selection; 				break;			
		case 941:	Temp= 	P.G11.AO1.P01_Type; 							break;
//		case 942:	Temp= 	P.G11.AO1.P02_Adjustment_0mA; 					break;
//		case 943:	Temp= 	P.G11.AO1.P03_Adjustment_4mA; 					break;
//		case 944:	Temp= 	P.G11.AO1.P04_Adjustment_20mA; 					break;
		case 945:	Temp= 	P.G11.AO1.P05_Max_output_x1000; 				break;			
//		case 946:	Temp= 	P.G11.AO1.P06_Inversion; 						break;				
//		case 947:	Temp= 	P.G11.AO2.P00_Output_selection; 				break;			
//		case 948:	Temp= 	P.G11.AO2.P01_Type; 							break;
//		case 949:	Temp= 	P.G11.AO2.P02_Adjustment_0mA; 					break;
//		case 950:	Temp= 	P.G11.AO2.P03_Adjustment_4mA; 					break;
//		case 951:	Temp= 	P.G11.AO2.P04_Adjustment_20mA;					break;
//		case 952:	Temp= 	P.G11.AO2.P05_Max_output_x1000; 				break;
//		case 953:	Temp= 	P.G11.AO2.P06_Inversion; 						break;	
				
		case 960:	Temp=	P.G12.P00_DO1_function; 						break;					
		case 961:	Temp=	P.G12.P00_DO2_function; 						break;	
		case 962:	Temp= 	P.G12.P00_DO3_function; 						break;
	

		case 980:	Temp= 	P.G13.P00_M1_locked_state_up_speed_set_x1000;	break;
		case 981:	Temp= 	P.G13.P01_M1_locked_state_down_speed_set_x1000;	break;
		case 982:	Temp= 	P.G13.P02_M1_break_open_current_x1000;			break;
		case 983:	Temp= 	P.G13.P03_M1_start_delay_time_x10;				break;
		case 984:	Temp= 	P.G13.P04_M1_break_close_speed_set_x1000;		break;
		case 985:	Temp= 	P.G13.P05_M1_break_open_torque_build_time_x10;	break;

		case 1019:	Temp= 	P.G14.P09_Dead_time_compansation_x100_us;		break;
		
		case 1370:	Temp= 	P.G19.P00_Number_of_encoder_pulses; 			break;	
		case 1371:	Temp= 	P.G19.P01_Inversion_of_PG_direction; 			break;	
		case 1373:	Temp= 	P.G19.P03_Minimum_speed; 						break;			
		case 1374:	Temp= 	P.G19.P04_Maximum_speed_x1000; 					break;			
		case 1375:	Temp= 	P.G19.P05_Over_speed_limit_x1000; 				break;		

		case 1550:	Temp= 	P.G21.P00_Stator_resistance_x1000; 				break;
		case 1552:	Temp= 	P.G21.P01_Rotor_resistance_x1000; 				break;
		case 1553:	Temp= 	P.G21.P02_Stator_inductance_x1000; 				break;
		case 1554:	Temp= 	P.G21.P03_Rotor_inductance_x1000; 				break;
		case 1555:	Temp= 	P.G21.P04_Stator_transient_inductance_x1000;	break;
		case 1556:	Temp=	P.G21.P05_Inertia_x1000; 						break;
		default:	Temp=	0;
	}

	Data_Registers[Parameter_index]= Temp;

}








