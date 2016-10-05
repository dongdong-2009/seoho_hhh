#include	<All_Header.h>
#include	<All_Extern_Variables.h> 


// #pragma CODE_SECTION(Read_Data_Registers, "ramfuncs"); 
void Read_Data_Registers(int Parameter_index)
{
	switch(Parameter_index)
	{
		case 2306:	OP.Run_stop.bit.Local_RUN= 					Data_Registers[Parameter_index];	break; 
		case 2300:	OP.Run_stop.bit.Local=						Data_Registers[Parameter_index];	break;
		case 2301:	OP.Run_stop.bit.Local_DIR= 					Data_Registers[Parameter_index];	break; 
		case 2302:	OP.Reference.Local.Speed= 					Data_Registers[Parameter_index];	break; 
		case 2303:	OP.Reference.Local.Frequency_x10=			Data_Registers[Parameter_index];	break; 
		case 2304:	OP.Reference.Local.Torque_x1000=			Data_Registers[Parameter_index];	break; 
		case 2305:	OP.Reference.Local.PID_x1000= 				Data_Registers[Parameter_index];	break;
		case 2290:	OP.Run_stop.bit.Fieldbus_RUN= 				Data_Registers[Parameter_index];	break;
		case 2291:	OP.Run_stop.bit.Fieldbus_DIR= 				Data_Registers[Parameter_index];	break;
		case 2292:	OP.Reference.Fieldbus.Speed= 				Data_Registers[Parameter_index];	break; 
		case 2293:	OP.Reference.Fieldbus.Frequency_x10=		Data_Registers[Parameter_index];	break; 
		case 2294:	OP.Reference.Fieldbus.Torque_x1000=			Data_Registers[Parameter_index];	break; 
		case 2295:	OP.Reference.Fieldbus.PID_x1000= 			Data_Registers[Parameter_index];	break;
		case 2307:	OP.Run_stop.bit.Emergency_STOP= 			Data_Registers[Parameter_index];	break;
		case 2371:	OP.Run_stop.bit.AUTO_TUNING=				Data_Registers[Parameter_index];	break;
		// 원인불명 국적불명의 원인으로 인해 Write가 안됨 (이 뭥미??? - 나도 모르겠음) 함수로 직접이동
//		case 2484:	OP.Run_stop.bit.FAULT_RESET= 				Data_Registers[Parameter_index];	break;

		case 200:	P.G00.P00_Device_number=	 				Data_Registers[Parameter_index];	break;
		case 201:	P.G00.P01_Software_version1=				Data_Registers[Parameter_index];	break;
		case 202:	P.G00.P02_Software_version2=				Data_Registers[Parameter_index];	break;
		case 203:	P.G00.P03_Software_version3=				Data_Registers[Parameter_index];	break;
		case 208:	P.G00.P08_Password= 						Data_Registers[Parameter_index];	break;
		case 209:	P.G00.P09_IGBT_current= 					Data_Registers[Parameter_index];	break;
		case 210:	P.G00.P10_Drive_power_x10_kW= 				Data_Registers[Parameter_index];	break; 

		case 240:	P.G01.P00_Rated_power_x10_kW= 				Data_Registers[Parameter_index];	break;
		case 241:	P.G01.P01_Rated_voltage= 					Data_Registers[Parameter_index];	break;
		case 242:	P.G01.P02_Rated_current_x10= 				Data_Registers[Parameter_index];	break;
		case 243:	P.G01.P03_Rated_frequency= 					Data_Registers[Parameter_index];	break;
		case 244:	P.G01.P04_Number_of_poles= 					Data_Registers[Parameter_index];	break;
		case 245:	P.G01.P05_Rated_speed= 						Data_Registers[Parameter_index];	break;
		case 246:	P.G01.P06_Control_method= 					Data_Registers[Parameter_index];	break;
		case 247:	P.G01.P07_PWM_frequency_x10_kHz=			Data_Registers[Parameter_index];	break;
		case 249:	P.G01.P08_Supply_voltage=					Data_Registers[Parameter_index];	break; 
														
		case 280:	P.G03.P00_Run_stop_source= 					Data_Registers[Parameter_index];	break;
		case 281:	P.G03.P01_Reference_source= 				Data_Registers[Parameter_index];	break; 			
		case 283:	P.G03.P03_Stop_mode= 						Data_Registers[Parameter_index];	break;				
		case 284:	P.G03.P04_Stop_hold_time= 					Data_Registers[Parameter_index];	break;		
		case 285:	P.G03.P05_Output_off_hold_time= 			Data_Registers[Parameter_index];	break;		
		case 289:	P.G03.P09_Accel_switching_ref1_x1000= 		Data_Registers[Parameter_index];	break;		
		case 290:	P.G03.P10_Accel_switching_ref2_x1000= 		Data_Registers[Parameter_index];	break;		
		case 296:	P.G03.P16_Accel_time1_x10= 					Data_Registers[Parameter_index];	break;				
		case 297:	P.G03.P17_Accel_time2_x10= 					Data_Registers[Parameter_index];	break;				
		case 306:	P.G03.P26_Decel_switching_ref1_x1000= 		Data_Registers[Parameter_index];	break;		
		case 307:	P.G03.P27_Decel_switching_ref2_x1000= 		Data_Registers[Parameter_index];	break;		
		case 313:	P.G03.P33_Decel_time1_x10= 					Data_Registers[Parameter_index];	break;					 
		case 314:	P.G03.P34_Decel_time2_x10= 					Data_Registers[Parameter_index];	break;				
		case 322:	P.G03.P42_Conunter_deceleration_ramp= 		Data_Registers[Parameter_index];	break;
		case 323:	P.G03.P43_Conunter_deceleration_time_x10= 	Data_Registers[Parameter_index];	break;
		case 329:	P.G03.P49_Emergency_stop_mode= 				Data_Registers[Parameter_index];	break;		
		case 330:	P.G03.P50_Emergency_stop_decel_time_x10= 	Data_Registers[Parameter_index];	break;
		case 331:	P.G03.P51_S_curve_enable= 					Data_Registers[Parameter_index];	break;		
		case 332:	P.G03.P52_S_curve_weight_x1000=			 	Data_Registers[Parameter_index];	break;		

		case 500:	P.G05.P00_Current_limit_x1000= 				Data_Registers[Parameter_index];	break;				
		case 507:	P.G05.P07_Max_continuous_current_x1000= 	Data_Registers[Parameter_index];	break;	
		case 508:	P.G05.P08_Overload_current_x1000= 			Data_Registers[Parameter_index];	break;			
		case 509:	P.G05.P09_Over_load_time_x10= 				Data_Registers[Parameter_index];	break;			
		case 510:	P.G05.P10_Over_load_fault_action= 			Data_Registers[Parameter_index];	break;		
		case 511:	P.G05.P11_Over_current_trip_x1000= 			Data_Registers[Parameter_index];	break;	
		case 512:	P.G05.P12_Zero_sequence_current_trip_x1000= Data_Registers[Parameter_index];	break;	
		case 513:	P.G05.P13_Over_voltage_limiting_function= 	Data_Registers[Parameter_index];	break;
		case 514:	P.G05.P14_Over_voltage_limit= 				Data_Registers[Parameter_index];	break;			
		case 515:	P.G05.P15_Over_voltage_trip= 				Data_Registers[Parameter_index];	break;		 
		case 516:	P.G05.P16_Under_voltage_compensation= 		Data_Registers[Parameter_index];	break;	
		case 517:	P.G05.P17_UV_compensation_voltage= 			Data_Registers[Parameter_index];	break;		
		case 518:	P.G05.P18_Under_voltage_trip= 				Data_Registers[Parameter_index];	break; 		
		case 519:	P.G05.P19_Open_phase_protection= 			Data_Registers[Parameter_index];	break;		
		case 520:	P.G05.P20_Supply_frequency= 				Data_Registers[Parameter_index];	break;				
		case 521:	P.G05.P21_Built_in_dynamic_brake= 			Data_Registers[Parameter_index];	break;		
		case 522:	P.G05.P22_DB_switching_frequency= 			Data_Registers[Parameter_index];	break;		
		case 523:	P.G05.P23_DB_start_voltage= 				Data_Registers[Parameter_index];	break;			
		case 524:	P.G05.P24_DB_full_voltage= 					Data_Registers[Parameter_index];	break;			
		case 525:	P.G05.P25_Over_temperature_trip_action= 	Data_Registers[Parameter_index];	break;
		case 530:	P.G05.P30_Auto_restart_count= 				Data_Registers[Parameter_index];	break;		
		case 531:	P.G05.P31_Retry_delay_time_x10= 			Data_Registers[Parameter_index];	break;	
		case 532:	P.G05.P32_OC_auto_reset= 					Data_Registers[Parameter_index];	break;			
		case 533:	P.G05.P33_OV_auto_reset= 					Data_Registers[Parameter_index];	break;			
		case 534:	P.G05.P34_UV_auto_reset= 					Data_Registers[Parameter_index];	break;					
		case 537:	P.G05.P37_Out_of_control_auto_reset= 		Data_Registers[Parameter_index];	break;		
		case 538:	P.G05.P38_Out_of_control_time_x10= 			Data_Registers[Parameter_index];	break;		
		case 539:	P.G05.P39_Out_of_control_current_x1000= 	Data_Registers[Parameter_index];	break;
		case 540:	P.G05.P40_Over_temperature_trip_x10= 		Data_Registers[Parameter_index];	break;		
		case 542:	P.G05.P42_Line_under_voltage_trip_x1000= 	Data_Registers[Parameter_index];	break;		
		case 543:	P.G05.P43_Line_under_voltage_auto_reset= 	Data_Registers[Parameter_index];	break;	
		case 544:	P.G05.P44_Line_unbalance_auto_reset= 		Data_Registers[Parameter_index];	break;	

		case 600:	P.G06.AI0_Analog_reference_source= 			Data_Registers[Parameter_index];	break;	
		case 601:	P.G06.AI1.P01_Function= 					Data_Registers[Parameter_index];	break;			
		case 602:	P.G06.AI1.P02_Type= 						Data_Registers[Parameter_index];	break;					
		case 603:	P.G06.AI1.P03_Filter_time_constant_x10_mA= 	Data_Registers[Parameter_index];	break;
		case 604:	P.G06.AI1.P04_Offset_x10= 					Data_Registers[Parameter_index];	break;				
		case 605:	P.G06.AI1.P05_Minimum_x10= 					Data_Registers[Parameter_index];	break;				
		case 606:	P.G06.AI1.P06_Maximum_x10= 					Data_Registers[Parameter_index];	break;				
		case 607:	P.G06.AI1.P07_Inversion= 					Data_Registers[Parameter_index];	break;				
		case 608:	P.G06.AI1.P08_Discretness= 					Data_Registers[Parameter_index];	break;			
		case 609:	P.G06.AI1.P09_Unit_selection= 				Data_Registers[Parameter_index];	break;				
		case 610:	P.G06.AI2.P01_Function= 					Data_Registers[Parameter_index];	break;		
		case 611:	P.G06.AI2.P02_Type= 						Data_Registers[Parameter_index];	break;				
		case 612:	P.G06.AI2.P03_Filter_time_constant_x10_mA= 	Data_Registers[Parameter_index];	break;
		case 613:	P.G06.AI2.P04_Offset_x10= 					Data_Registers[Parameter_index];	break;			
		case 614:	P.G06.AI2.P05_Minimum_x10= 					Data_Registers[Parameter_index];	break;				
		case 615:	P.G06.AI2.P06_Maximum_x10= 					Data_Registers[Parameter_index];	break;			
		case 616:	P.G06.AI2.P07_Inversion= 					Data_Registers[Parameter_index];	break;				
		case 617:	P.G06.AI2.P08_Discretness= 					Data_Registers[Parameter_index];	break;			
		case 618:	P.G06.AI2.P09_Unit_selection=				Data_Registers[Parameter_index];	break;		
/*
		case 619:	P.G06.AI3.P01_Function= 					Data_Registers[Parameter_index];	break;		
		case 620:	P.G06.AI3.P02_Type= 						Data_Registers[Parameter_index];	break;					
		case 621:	P.G06.AI3.P03_Filter_time_constant_x10_mA= 	Data_Registers[Parameter_index];	break;	
		case 622:	P.G06.AI3.P04_Offset_x10= 					Data_Registers[Parameter_index];	break;			
		case 623:	P.G06.AI3.P05_Minimum_x10= 					Data_Registers[Parameter_index];	break;
		case 624:	P.G06.AI3.P06_Maximum_x10= 					Data_Registers[Parameter_index];	break;		
		case 625:	P.G06.AI3.P07_Inversion= 					Data_Registers[Parameter_index];	break;				
		case 626:	P.G06.AI3.P08_Discretness= 					Data_Registers[Parameter_index];	break;		
		case 627:	P.G06.AI3.P09_Unit_selection= 				Data_Registers[Parameter_index];	break;		
		case 628:	P.G06.AI4.P01_Function= 					Data_Registers[Parameter_index];	break;		
		case 629:	P.G06.AI4.P02_Type= 						Data_Registers[Parameter_index];	break;					
		case 630:	P.G06.AI4.P03_Filter_time_constant_x10_mA= 	Data_Registers[Parameter_index];	break;
		case 631:	P.G06.AI4.P04_Offset_x10= 					Data_Registers[Parameter_index];	break;		
		case 632:	P.G06.AI4.P05_Minimum_x10= 					Data_Registers[Parameter_index];	break;		
		case 633:	P.G06.AI4.P06_Maximum_x10= 					Data_Registers[Parameter_index];	break;		
		case 634:	P.G06.AI4.P07_Inversion= 					Data_Registers[Parameter_index];	break;	
		case 635:	P.G06.AI4.P08_Discretness= 					Data_Registers[Parameter_index];	break;		
		case 636:	P.G06.AI4.P09_Unit_selection= 				Data_Registers[Parameter_index];	break;		
		case 637:	P.G06.AI5.P01_Function= 					Data_Registers[Parameter_index];	break;			
		case 638:	P.G06.AI5.P02_Type= 						Data_Registers[Parameter_index];	break;				
		case 639:	P.G06.AI5.P03_Filter_time_constant_x10_mA= 	Data_Registers[Parameter_index];	break;	
		case 640:	P.G06.AI5.P04_Offset_x10= 					Data_Registers[Parameter_index];	break;				
		case 641:	P.G06.AI5.P05_Minimum_x10= 					Data_Registers[Parameter_index];	break;			
		case 642:	P.G06.AI5.P06_Maximum_x10= 					Data_Registers[Parameter_index];	break;			
		case 643:	P.G06.AI5.P07_Inversion= 					Data_Registers[Parameter_index];	break;				
		case 644:	P.G06.AI5.P08_Discretness= 					Data_Registers[Parameter_index];	break;	
		case 645:	P.G06.AI5.P09_Unit_selection= 				Data_Registers[Parameter_index];	break;			
*/
		case 750:	P.G07.P00_Control_mode= 					Data_Registers[Parameter_index];	break;			

		case 810:	P.G08.P00_Run_stop_control= 				Data_Registers[Parameter_index];	break;			
		case 811:	P.G08.P01_DI03_function= 					Data_Registers[Parameter_index];	break;	
		case 812:	P.G08.P02_DI04_function= 					Data_Registers[Parameter_index];	break;				
		case 813:	P.G08.P03_DI05_function= 					Data_Registers[Parameter_index];	break;						
		case 814:	P.G08.P04_DI06_function= 					Data_Registers[Parameter_index];	break;				
		case 815:	P.G08.P05_DI07_function= 					Data_Registers[Parameter_index];	break;					
		case 816:	P.G08.P06_DI08_function= 					Data_Registers[Parameter_index];	break;					
		case 817:	P.G08.P07_DI09_function= 					Data_Registers[Parameter_index];	break;						
		case 818:	P.G08.P08_DI10_function= 					Data_Registers[Parameter_index];	break;				
		case 819:	P.G08.P09_DI11_function= 					Data_Registers[Parameter_index];	break;						
		case 820:	P.G08.P10_DI12_function= 					Data_Registers[Parameter_index];	break;				
		case 828:	P.G08.P11_Run_delay_time= 					Data_Registers[Parameter_index];	break;			

		case 860:	P.G09.P00_JOG_set_x1000=					Data_Registers[Parameter_index];	break;					
		case 861:	P.G09.P01_Step01_set_x10= 					Data_Registers[Parameter_index];	break;					
		case 862:	P.G09.P02_Step02_set_x10= 					Data_Registers[Parameter_index];	break;				
		case 863:	P.G09.P03_Step03_set_x10= 					Data_Registers[Parameter_index];	break;				
		case 864:	P.G09.P04_Step04_set_x10= 					Data_Registers[Parameter_index];	break;					
		case 865:	P.G09.P05_Step05_set_x10= 					Data_Registers[Parameter_index];	break;					
		case 866:	P.G09.P06_Step06_set_x10= 					Data_Registers[Parameter_index];	break;				
		case 867:	P.G09.P07_Step07_set_x10= 					Data_Registers[Parameter_index];	break;						
		case 868:	P.G09.P08_Step08_set_x10= 					Data_Registers[Parameter_index];	break;				
		case 869:	P.G09.P09_Step09_set_x10=					Data_Registers[Parameter_index];	break;						
		case 870:	P.G09.P10_Step10_set_x10= 					Data_Registers[Parameter_index];	break;					
		case 871:	P.G09.P11_Step11_set_x10= 					Data_Registers[Parameter_index];	break;							
		case 872:	P.G09.P12_Step12_set_x10= 					Data_Registers[Parameter_index];	break;				
		case 873:	P.G09.P13_Step13_set_x10= 					Data_Registers[Parameter_index];	break;					
		case 874:	P.G09.P14_Step14_set_x10= 					Data_Registers[Parameter_index];	break;				
		case 875:	P.G09.P15_Step15_set_x10= 					Data_Registers[Parameter_index];	break;				
		case 876:	P.G09.P16_Unit_selection= 					Data_Registers[Parameter_index];	break;			

		case 940:	P.G11.AO1.P00_Output_selection= 			Data_Registers[Parameter_index];	break;			
		case 941:	P.G11.AO1.P01_Type= 						Data_Registers[Parameter_index];	break;
		case 942:	P.G11.AO1.P02_Adjustment_0mA= 				Data_Registers[Parameter_index];	break;
		case 943:	P.G11.AO1.P03_Adjustment_4mA= 				Data_Registers[Parameter_index];	break;
		case 944:	P.G11.AO1.P04_Adjustment_20mA= 				Data_Registers[Parameter_index];	break;
		case 945:	P.G11.AO1.P05_Max_output_x1000= 			Data_Registers[Parameter_index];	break;			
		case 946:	P.G11.AO1.P06_Inversion= 					Data_Registers[Parameter_index];	break;				
		case 947:	P.G11.AO2.P00_Output_selection= 			Data_Registers[Parameter_index];	break;			
		case 948:	P.G11.AO2.P01_Type= 						Data_Registers[Parameter_index];	break;
		case 949:	P.G11.AO2.P02_Adjustment_0mA= 				Data_Registers[Parameter_index];	break;
		case 950:	P.G11.AO2.P03_Adjustment_4mA= 				Data_Registers[Parameter_index];	break;
		case 951:	P.G11.AO2.P04_Adjustment_20mA=				Data_Registers[Parameter_index];	break;
		case 952:	P.G11.AO2.P05_Max_output_x1000= 			Data_Registers[Parameter_index];	break;
		case 953:	P.G11.AO2.P06_Inversion= 					Data_Registers[Parameter_index];	break;	
				
		case 960:	P.G12.P00_DO1_function= 					Data_Registers[Parameter_index];	break;					
		case 961:	P.G12.P00_DO2_function= 					Data_Registers[Parameter_index];	break;	
		case 962:	P.G12.P00_DO3_function= 					Data_Registers[Parameter_index];	break;

		case 980:	P.G13.P00_M1_locked_state_up_speed_up_x1000=Data_Registers[Parameter_index];	break;

		case 1370:	P.G19.P00_Number_of_encoder_pulses= 		Data_Registers[Parameter_index];	break;	
		case 1371:	P.G19.P01_Inversion_of_PG_direction= 		Data_Registers[Parameter_index];	break;	
		case 1373:	P.G19.P03_Minimum_speed= 					Data_Registers[Parameter_index];	break;			
		case 1374:	P.G19.P04_Maximum_speed_x1000= 				Data_Registers[Parameter_index];	break;			
		case 1375:	P.G19.P05_Over_speed_limit_x1000= 			Data_Registers[Parameter_index];	break;		

		case 1550:	P.G21.P00_Stator_resistance_x1000= 			Data_Registers[Parameter_index];	break;
		case 1552:	P.G21.P01_Rotor_resistance_x1000= 			Data_Registers[Parameter_index];	break;
		case 1553:	P.G21.P02_Stator_inductance_x1000= 			Data_Registers[Parameter_index];	break;
		case 1554:	P.G21.P03_Rotor_inductance_x1000= 			Data_Registers[Parameter_index];	break;
		case 1555:	P.G21.P04_Stator_transient_inductance_x1000=Data_Registers[Parameter_index];	break;
		case 1556:	P.G21.P05_Inertia_x1000= 					Data_Registers[Parameter_index];	break;
	}

}

// #pragma CODE_SECTION(Write_Data_Registers_Online, "ramfuncs"); 
void Write_Data_Registers_Online()
{
	static int Write_counter= 0;
	
	Write_counter++;
	if (Write_counter & 0x1)
	{
 		Data_Registers[2310]= 	(int)Wrpm_det;				
		Data_Registers[2311]= 	(int)((Wrpm_det/(double)P.G01.P05_Rated_speed)*(double)P.G01.P03_Rated_frequency*10.); 						
		Data_Registers[2312]= 	(int)(Vdc);
		Data_Registers[2313]= 	(int)(Is_mag_rms*10.);
		Data_Registers[2314]= 	(int)(sqrt(Vdss_ref*Vdss_ref+Vqss_ref*Vqss_ref)*SQRT2);
		Data_Registers[2315]= 	(int)(Te/Te_rate*1000.);
		Data_Registers[2316]= 	(int)(Iqse*10.);
		Data_Registers[2317]= 	(int)(Idse*10.);
		Data_Registers[2318]= 	(int)(Input_power_x10_kW);			
		Data_Registers[2319]= 	(int)(Output_power_x10_kW);	
		Data_Registers[2323]= 	Temperature_x10; 

		Data_Registers[2330]=	State_Index;

		Data_Registers[2270]=	Flag.Monitoring.bit.RUN_STOP_STATUS;	
		Data_Registers[2271]=	Flag.Monitoring.bit.DIR_STATUS;

		Data_Registers[2371]=	OP.Run_stop.bit.AUTO_TUNING;

		Data_Registers[2376]=	Flag.Fault1.all;
		Data_Registers[2377]=	Flag.Fault2.all;
		Data_Registers[2379]= 	Fault_count;

		Data_Registers[2485]=	Flag.Monitoring.bit.FAULT_RESET_COMPLETE;

		Data_Registers[2484]=	OP.Run_stop.bit.FAULT_RESET;
	}
	else 
	{
		if (Write_counter & 0x2)
		{
			Data_Registers[1550]= 	P.G21.P00_Stator_resistance_x1000; 			
			Data_Registers[1552]= 	P.G21.P01_Rotor_resistance_x1000; 			
			Data_Registers[1553]= 	P.G21.P02_Stator_inductance_x1000; 			
			Data_Registers[1554]= 	P.G21.P03_Rotor_inductance_x1000; 			
			Data_Registers[1555]= 	P.G21.P04_Stator_transient_inductance_x1000;
			Data_Registers[1556]=	P.G21.P05_Inertia_x1000;
		}
	}

	if (Write_counter>= 30000)	Write_counter= 0;
	
	
}

void Write_Data_Registers_Offline(int Parameter_index)
{
	switch(Parameter_index)
	{
		case 2306:	Data_Registers[Parameter_index]=	OP.Run_stop.bit.Local_RUN;					break; 
		case 2300:	Data_Registers[Parameter_index]=	OP.Run_stop.bit.Local;						break;
		case 2301:	Data_Registers[Parameter_index]=	OP.Run_stop.bit.Local_DIR;					break; 
		case 2302:	Data_Registers[Parameter_index]=	OP.Reference.Local.Speed;					break; 
		case 2303:	Data_Registers[Parameter_index]=	OP.Reference.Local.Frequency_x10;			break; 
		case 2304:	Data_Registers[Parameter_index]=	OP.Reference.Local.Torque_x1000;			break; 
		case 2305:	Data_Registers[Parameter_index]=	OP.Reference.Local.PID_x1000;				break;
		case 2290:	Data_Registers[Parameter_index]=	OP.Run_stop.bit.Fieldbus_RUN;				break;
		case 2291:	Data_Registers[Parameter_index]=	OP.Run_stop.bit.Fieldbus_DIR;				break;
		case 2292:	Data_Registers[Parameter_index]=	OP.Reference.Fieldbus.Speed;				break; 
		case 2293:	Data_Registers[Parameter_index]=	OP.Reference.Fieldbus.Frequency_x10;		break; 
		case 2294:	Data_Registers[Parameter_index]=	OP.Reference.Fieldbus.Torque_x1000;			break; 
		case 2295:	Data_Registers[Parameter_index]=	OP.Reference.Fieldbus.PID_x1000;			break;
		case 2307:	Data_Registers[Parameter_index]=	OP.Run_stop.bit.Emergency_STOP;				break;
		case 2371:	Data_Registers[Parameter_index]=	OP.Run_stop.bit.AUTO_TUNING;				break;
		case 2484:	Data_Registers[Parameter_index]=	OP.Run_stop.bit.FAULT_RESET;				break;
		case 2376:	Data_Registers[Parameter_index]=	Flag.Fault1.all;							break;
		case 2377:	Data_Registers[Parameter_index]=	Flag.Fault2.all;							break;
		case 2379:	Data_Registers[Parameter_index]=	Fault_count;								break;

		case 200:	Data_Registers[Parameter_index]=	P.G00.P00_Device_number;					break;
		case 201:	Data_Registers[Parameter_index]=	P.G00.P01_Software_version1;				break;
		case 202:	Data_Registers[Parameter_index]=	P.G00.P02_Software_version2;				break;
		case 203:	Data_Registers[Parameter_index]=	P.G00.P03_Software_version3;				break;
		case 208:	Data_Registers[Parameter_index]= 	P.G00.P08_Password; 						break;
		case 209:	Data_Registers[Parameter_index]= 	P.G00.P09_IGBT_current; 					break;
		case 210:	Data_Registers[Parameter_index]= 	P.G00.P10_Drive_power_x10_kW;				break;

		case 240:	Data_Registers[Parameter_index]= 	P.G01.P00_Rated_power_x10_kW; 				break;
		case 241:	Data_Registers[Parameter_index]= 	P.G01.P01_Rated_voltage; 					break;
		case 242:	Data_Registers[Parameter_index]= 	P.G01.P02_Rated_current_x10; 				break;
		case 243:	Data_Registers[Parameter_index]= 	P.G01.P03_Rated_frequency; 					break;
		case 244:	Data_Registers[Parameter_index]= 	P.G01.P04_Number_of_poles; 					break;
		case 245:	Data_Registers[Parameter_index]= 	P.G01.P05_Rated_speed; 						break;
		case 246:	Data_Registers[Parameter_index]= 	P.G01.P06_Control_method; 					break;
		case 247:	Data_Registers[Parameter_index]= 	P.G01.P07_PWM_frequency_x10_kHz;			break;
		case 249:	Data_Registers[Parameter_index]= 	P.G01.P08_Supply_voltage;					break; 
														
		case 280:	Data_Registers[Parameter_index]= 	P.G03.P00_Run_stop_source; 					break; 	
		case 281:	Data_Registers[Parameter_index]= 	P.G03.P01_Reference_source; 				break;		
		case 283:	Data_Registers[Parameter_index]= 	P.G03.P03_Stop_mode; 						break;				
		case 284:	Data_Registers[Parameter_index]= 	P.G03.P04_Stop_hold_time; 					break;		
		case 285:	Data_Registers[Parameter_index]= 	P.G03.P05_Output_off_hold_time; 			break;		
		case 289:	Data_Registers[Parameter_index]= 	P.G03.P09_Accel_switching_ref1_x1000; 		break;		
		case 290:	Data_Registers[Parameter_index]= 	P.G03.P10_Accel_switching_ref2_x1000; 		break;		
		case 296:	Data_Registers[Parameter_index]= 	P.G03.P16_Accel_time1_x10; 					break;				
		case 297:	Data_Registers[Parameter_index]= 	P.G03.P17_Accel_time2_x10; 					break;				
		case 306:	Data_Registers[Parameter_index]= 	P.G03.P26_Decel_switching_ref1_x1000; 		break;		
		case 307:	Data_Registers[Parameter_index]= 	P.G03.P27_Decel_switching_ref2_x1000; 		break;		
		case 313:	Data_Registers[Parameter_index]= 	P.G03.P33_Decel_time1_x10; 					break;					 
		case 314:	Data_Registers[Parameter_index]= 	P.G03.P34_Decel_time2_x10; 					break;				
		case 322:	Data_Registers[Parameter_index]= 	P.G03.P42_Conunter_deceleration_ramp; 		break;
		case 323:	Data_Registers[Parameter_index]= 	P.G03.P43_Conunter_deceleration_time_x10; 	break;
		case 329:	Data_Registers[Parameter_index]= 	P.G03.P49_Emergency_stop_mode; 				break;		
		case 330:	Data_Registers[Parameter_index]= 	P.G03.P50_Emergency_stop_decel_time_x10; 	break;
		case 331:	Data_Registers[Parameter_index]= 	P.G03.P51_S_curve_enable;				 	break;
		case 332:	Data_Registers[Parameter_index]= 	P.G03.P52_S_curve_weight_x1000;			 	break;
			
		case 500:	Data_Registers[Parameter_index]= 	P.G05.P00_Current_limit_x1000; 				break;				
		case 507:	Data_Registers[Parameter_index]= 	P.G05.P07_Max_continuous_current_x1000; 	break;	
		case 508:	Data_Registers[Parameter_index]= 	P.G05.P08_Overload_current_x1000; 			break;			
		case 509:	Data_Registers[Parameter_index]= 	P.G05.P09_Over_load_time_x10; 				break;			
		case 510:	Data_Registers[Parameter_index]= 	P.G05.P10_Over_load_fault_action; 			break;		
		case 511:	Data_Registers[Parameter_index]= 	P.G05.P11_Over_current_trip_x1000; 			break;	
		case 512:	Data_Registers[Parameter_index]= 	P.G05.P12_Zero_sequence_current_trip_x1000; break;	
		case 513:	Data_Registers[Parameter_index]= 	P.G05.P13_Over_voltage_limiting_function; 	break;
		case 514:	Data_Registers[Parameter_index]= 	P.G05.P14_Over_voltage_limit; 				break;			
		case 515:	Data_Registers[Parameter_index]= 	P.G05.P15_Over_voltage_trip; 				break;		 
		case 516:	Data_Registers[Parameter_index]= 	P.G05.P16_Under_voltage_compensation; 		break;	
		case 517:	Data_Registers[Parameter_index]= 	P.G05.P17_UV_compensation_voltage; 			break;		
		case 518:	Data_Registers[Parameter_index]= 	P.G05.P18_Under_voltage_trip; 				break; 		
		case 519:	Data_Registers[Parameter_index]= 	P.G05.P19_Open_phase_protection; 			break;		
		case 520:	Data_Registers[Parameter_index]= 	P.G05.P20_Supply_frequency; 				break;				
		case 521:	Data_Registers[Parameter_index]= 	P.G05.P21_Built_in_dynamic_brake; 			break;		
		case 522:	Data_Registers[Parameter_index]= 	P.G05.P22_DB_switching_frequency;			break;		
		case 523:	Data_Registers[Parameter_index]= 	P.G05.P23_DB_start_voltage; 				break;			
		case 524:	Data_Registers[Parameter_index]= 	P.G05.P24_DB_full_voltage; 					break;			
		case 525:	Data_Registers[Parameter_index]= 	P.G05.P25_Over_temperature_trip_action; 	break;
		case 530:	Data_Registers[Parameter_index]= 	P.G05.P30_Auto_restart_count; 				break;		
		case 531:	Data_Registers[Parameter_index]= 	P.G05.P31_Retry_delay_time_x10; 			break;	
		case 532:	Data_Registers[Parameter_index]= 	P.G05.P32_OC_auto_reset; 					break;			
		case 533:	Data_Registers[Parameter_index]= 	P.G05.P33_OV_auto_reset; 					break;			
		case 534:	Data_Registers[Parameter_index]= 	P.G05.P34_UV_auto_reset; 					break;					
		case 537:	Data_Registers[Parameter_index]= 	P.G05.P37_Out_of_control_auto_reset; 		break;		
		case 538:	Data_Registers[Parameter_index]= 	P.G05.P38_Out_of_control_time_x10; 			break;		
		case 539:	Data_Registers[Parameter_index]= 	P.G05.P39_Out_of_control_current_x1000; 	break;
		case 540:	Data_Registers[Parameter_index]= 	P.G05.P40_Over_temperature_trip_x10; 		break;		
		case 542:	Data_Registers[Parameter_index]= 	P.G05.P42_Line_under_voltage_trip_x1000; 	break;		
		case 543:	Data_Registers[Parameter_index]= 	P.G05.P43_Line_under_voltage_auto_reset; 	break;	
		case 544:	Data_Registers[Parameter_index]= 	P.G05.P44_Line_unbalance_auto_reset; 		break;	

		case 600:	Data_Registers[Parameter_index]= 	P.G06.AI0_Analog_reference_source; 			break;	
		case 601:	Data_Registers[Parameter_index]= 	P.G06.AI1.P01_Function; 					break;			
		case 602:	Data_Registers[Parameter_index]= 	P.G06.AI1.P02_Type; 						break;					
		case 603:	Data_Registers[Parameter_index]= 	P.G06.AI1.P03_Filter_time_constant_x10_mA; 	break;
		case 604:	Data_Registers[Parameter_index]= 	P.G06.AI1.P04_Offset_x10; 					break;				
		case 605:	Data_Registers[Parameter_index]= 	P.G06.AI1.P05_Minimum_x10; 					break;				
		case 606:	Data_Registers[Parameter_index]= 	P.G06.AI1.P06_Maximum_x10; 					break;				
		case 607:	Data_Registers[Parameter_index]= 	P.G06.AI1.P07_Inversion; 					break;				
		case 608:	Data_Registers[Parameter_index]= 	P.G06.AI1.P08_Discretness; 					break;			
		case 609:	Data_Registers[Parameter_index]= 	P.G06.AI1.P09_Unit_selection; 				break;				
		case 610:	Data_Registers[Parameter_index]= 	P.G06.AI2.P01_Function; 					break;		
		case 611:	Data_Registers[Parameter_index]= 	P.G06.AI2.P02_Type; 						break;				
		case 612:	Data_Registers[Parameter_index]= 	P.G06.AI2.P03_Filter_time_constant_x10_mA; 	break;
		case 613:	Data_Registers[Parameter_index]= 	P.G06.AI2.P04_Offset_x10; 					break;			
		case 614:	Data_Registers[Parameter_index]= 	P.G06.AI2.P05_Minimum_x10; 					break;				
		case 615:	Data_Registers[Parameter_index]= 	P.G06.AI2.P06_Maximum_x10; 					break;			
		case 616:	Data_Registers[Parameter_index]= 	P.G06.AI2.P07_Inversion; 					break;				
		case 617:	Data_Registers[Parameter_index]= 	P.G06.AI2.P08_Discretness; 					break;			
		case 618:	Data_Registers[Parameter_index]= 	P.G06.AI2.P09_Unit_selection;				break;		
/*
		case 619:	Data_Registers[Parameter_index]= 	P.G06.AI3.P01_Function; 					break;		
		case 620:	Data_Registers[Parameter_index]= 	P.G06.AI3.P02_Type; 						break;					
		case 621:	Data_Registers[Parameter_index]= 	P.G06.AI3.P03_Filter_time_constant_x10_mA; 	break;	
		case 622:	Data_Registers[Parameter_index]= 	P.G06.AI3.P04_Offset_x10; 					break;			
		case 623:	Data_Registers[Parameter_index]= 	P.G06.AI3.P05_Minimum_x10; 					break;
		case 624:	Data_Registers[Parameter_index]= 	P.G06.AI3.P06_Maximum_x10; 					break;		
		case 625:	Data_Registers[Parameter_index]=	P.G06.AI3.P07_Inversion; 					break;				
		case 626:	Data_Registers[Parameter_index]= 	P.G06.AI3.P08_Discretness; 					break;		
		case 627:	Data_Registers[Parameter_index]= 	P.G06.AI3.P09_Unit_selection; 				break;		
		case 628:	Data_Registers[Parameter_index]= 	P.G06.AI4.P01_Function; 					break;		
		case 629:	Data_Registers[Parameter_index]= 	P.G06.AI4.P02_Type; 						break;					
		case 630:	Data_Registers[Parameter_index]= 	P.G06.AI4.P03_Filter_time_constant_x10_mA; 	break;
		case 631:	Data_Registers[Parameter_index]= 	P.G06.AI4.P04_Offset_x10; 					break;		
		case 632:	Data_Registers[Parameter_index]= 	P.G06.AI4.P05_Minimum_x10; 					break;		
		case 633:	Data_Registers[Parameter_index]= 	P.G06.AI4.P06_Maximum_x10; 					break;		
		case 634:	Data_Registers[Parameter_index]= 	P.G06.AI4.P07_Inversion; 					break;	
		case 635:	Data_Registers[Parameter_index]= 	P.G06.AI4.P08_Discretness; 					break;		
		case 636:	Data_Registers[Parameter_index]= 	P.G06.AI4.P09_Unit_selection; 				break;		
		case 637:	Data_Registers[Parameter_index]= 	P.G06.AI5.P01_Function; 					break;			
		case 638:	Data_Registers[Parameter_index]= 	P.G06.AI5.P02_Type; 						break;				
		case 639:	Data_Registers[Parameter_index]= 	P.G06.AI5.P03_Filter_time_constant_x10_mA; 	break;	
		case 640:	Data_Registers[Parameter_index]= 	P.G06.AI5.P04_Offset_x10; 					break;				
		case 641:	Data_Registers[Parameter_index]= 	P.G06.AI5.P05_Minimum_x10; 					break;			
		case 642:	Data_Registers[Parameter_index]= 	P.G06.AI5.P06_Maximum_x10; 					break;			
		case 643:	Data_Registers[Parameter_index]= 	P.G06.AI5.P07_Inversion; 					break;				
		case 644:	Data_Registers[Parameter_index]= 	P.G06.AI5.P08_Discretness; 					break;	
		case 645:	Data_Registers[Parameter_index]= 	P.G06.AI5.P09_Unit_selection; 				break;			
*/
		case 750:	Data_Registers[Parameter_index]= 	P.G07.P00_Control_mode; 					break;			

		case 810:	Data_Registers[Parameter_index]= 	P.G08.P00_Run_stop_control; 				break;			
		case 811:	Data_Registers[Parameter_index]= 	P.G08.P01_DI03_function; 					break;	
		case 812:	Data_Registers[Parameter_index]= 	P.G08.P02_DI04_function; 					break;				
		case 813:	Data_Registers[Parameter_index]= 	P.G08.P03_DI05_function; 					break;						
		case 814:	Data_Registers[Parameter_index]=	P.G08.P04_DI06_function; 					break;				
		case 815:	Data_Registers[Parameter_index]= 	P.G08.P05_DI07_function; 					break;					
		case 816:	Data_Registers[Parameter_index]= 	P.G08.P06_DI08_function; 					break;					
		case 817:	Data_Registers[Parameter_index]= 	P.G08.P07_DI09_function; 					break;						
		case 818:	Data_Registers[Parameter_index]= 	P.G08.P08_DI10_function; 					break;				
		case 819:	Data_Registers[Parameter_index]= 	P.G08.P09_DI11_function; 					break;						
		case 820:	Data_Registers[Parameter_index]= 	P.G08.P10_DI12_function; 					break;				
		case 828:	Data_Registers[Parameter_index]= 	P.G08.P11_Run_delay_time; 					break;			

		case 860:	Data_Registers[Parameter_index]= 	P.G09.P00_JOG_set_x1000;					break;					
		case 861:	Data_Registers[Parameter_index]= 	P.G09.P01_Step01_set_x10; 					break;					
		case 862:	Data_Registers[Parameter_index]= 	P.G09.P02_Step02_set_x10; 					break;				
		case 863:	Data_Registers[Parameter_index]= 	P.G09.P03_Step03_set_x10; 					break;				
		case 864:	Data_Registers[Parameter_index]= 	P.G09.P04_Step04_set_x10; 					break;					
		case 865:	Data_Registers[Parameter_index]=	P.G09.P05_Step05_set_x10; 					break;					
		case 866:	Data_Registers[Parameter_index]= 	P.G09.P06_Step06_set_x10; 					break;				
		case 867:	Data_Registers[Parameter_index]= 	P.G09.P07_Step07_set_x10; 					break;						
		case 868:	Data_Registers[Parameter_index]= 	P.G09.P08_Step08_set_x10; 					break;				
		case 869:	Data_Registers[Parameter_index]= 	P.G09.P09_Step09_set_x10;					break;						
		case 870:	Data_Registers[Parameter_index]= 	P.G09.P10_Step10_set_x10; 					break;					
		case 871:	Data_Registers[Parameter_index]= 	P.G09.P11_Step11_set_x10; 					break;							
		case 872:	Data_Registers[Parameter_index]= 	P.G09.P12_Step12_set_x10; 					break;				
		case 873:	Data_Registers[Parameter_index]= 	P.G09.P13_Step13_set_x10; 					break;					
		case 874:	Data_Registers[Parameter_index]= 	P.G09.P14_Step14_set_x10; 					break;				
		case 875:	Data_Registers[Parameter_index]= 	P.G09.P15_Step15_set_x10; 					break;				
		case 876:	Data_Registers[Parameter_index]= 	P.G09.P16_Unit_selection; 					break;			

		case 940:	Data_Registers[Parameter_index]= 	P.G11.AO1.P00_Output_selection; 			break;			
		case 941:	Data_Registers[Parameter_index]= 	P.G11.AO1.P01_Type; 						break;
		case 942:	Data_Registers[Parameter_index]= 	P.G11.AO1.P02_Adjustment_0mA; 				break;
		case 943:	Data_Registers[Parameter_index]= 	P.G11.AO1.P03_Adjustment_4mA; 				break;
		case 944:	Data_Registers[Parameter_index]= 	P.G11.AO1.P04_Adjustment_20mA; 				break;
		case 945:	Data_Registers[Parameter_index]= 	P.G11.AO1.P05_Max_output_x1000; 			break;			
		case 946:	Data_Registers[Parameter_index]= 	P.G11.AO1.P06_Inversion; 					break;				
		case 947:	Data_Registers[Parameter_index]= 	P.G11.AO2.P00_Output_selection; 			break;			
		case 948:	Data_Registers[Parameter_index]= 	P.G11.AO2.P01_Type; 						break;
		case 949:	Data_Registers[Parameter_index]= 	P.G11.AO2.P02_Adjustment_0mA; 				break;
		case 950:	Data_Registers[Parameter_index]= 	P.G11.AO2.P03_Adjustment_4mA; 				break;
		case 951:	Data_Registers[Parameter_index]= 	P.G11.AO2.P04_Adjustment_20mA;				break;
		case 952:	Data_Registers[Parameter_index]= 	P.G11.AO2.P05_Max_output_x1000; 			break;
		case 953:	Data_Registers[Parameter_index]= 	P.G11.AO2.P06_Inversion; 					break;	
				
		case 960:	Data_Registers[Parameter_index]=	P.G12.P00_DO1_function; 					break;					
		case 961:	Data_Registers[Parameter_index]=	P.G12.P00_DO2_function; 					break;	
		case 962:	Data_Registers[Parameter_index]= 	P.G12.P00_DO3_function; 					break;
	
		case 980:	Data_Registers[Parameter_index]= 	P.G13.P00_M1_locked_state_up_speed_up_x1000;break;

		case 1019:	Data_Registers[Parameter_index]= 	P.G14.P09_Dead_time_compansation_x100_us;	break;
		
		case 1370:	Data_Registers[Parameter_index]= 	P.G19.P00_Number_of_encoder_pulses; 		break;	
		case 1371:	Data_Registers[Parameter_index]= 	P.G19.P01_Inversion_of_PG_direction; 		break;	
		case 1373:	Data_Registers[Parameter_index]= 	P.G19.P03_Minimum_speed; 					break;			
		case 1374:	Data_Registers[Parameter_index]= 	P.G19.P04_Maximum_speed_x1000; 				break;			
		case 1375:	Data_Registers[Parameter_index]= 	P.G19.P05_Over_speed_limit_x1000; 			break;		

		case 1550:	Data_Registers[Parameter_index]= 	P.G21.P00_Stator_resistance_x1000; 			break;
		case 1552:	Data_Registers[Parameter_index]= 	P.G21.P01_Rotor_resistance_x1000; 			break;
		case 1553:	Data_Registers[Parameter_index]= 	P.G21.P02_Stator_inductance_x1000; 			break;
		case 1554:	Data_Registers[Parameter_index]= 	P.G21.P03_Rotor_inductance_x1000; 			break;
		case 1555:	Data_Registers[Parameter_index]= 	P.G21.P04_Stator_transient_inductance_x1000;break;
		case 1556:	Data_Registers[Parameter_index]=	P.G21.P05_Inertia_x1000; 					break;
		default:	Data_Registers[Parameter_index]=	0;
	}

}

// ryu
//-------------------
// This function is related with CANopen
void Decomposite_CAN_Data(void)
{
    if (CANopenParameters.ControlWord.all == 0x000F)	OP.Run_stop.bit.Fieldbus_RUN = 1;  // run
	if (CANopenParameters.ControlWord.all == 0x010F)    OP.Run_stop.bit.Fieldbus_RUN = 0;  // stop
//    Theta_ref = CANopenParameters.TargetPosition.all;			// Theta_ref
}

void Composite_CAN_Data(void)
{
    if (OP.Run_stop.bit.Fieldbus_RUN) CANopenParameters.StatusWord.bit.Operation_enabled = 1;
	else                              CANopenParameters.StatusWord.bit.Operation_enabled = 0;
}

/*
void Read_CAN_Registers(int Parameter_index)
{
	switch(Parameter_index)
	{
		case 10:	CANopenParameters.ControlWord.byte.LowByte=				canInData[Parameter_index];	break; 
		case 11:	CANopenParameters.ControlWord.byte.HighByte=			canInData[Parameter_index];	break;
		case 12:	CANopenParameters.StatusWord.byte.LowByte= 				canInData[Parameter_index];	break; 
		case 13:	CANopenParameters.StatusWord.byte.HighByte=				canInData[Parameter_index];	break; 
		case 16:    CANopenParameters.TargetPosition.byte.LowByte=          canInData[Parameter_index]; break;
		case 17:    CANopenParameters.TargetPosition.byte.HighByte=         canInData[Parameter_index]; break;
	}

	Decomposite_CAN_Data();		// composite data for control
}
*/
void Read_CAN_Registers(void)
{
	CANopenParameters.ControlWord.byte.LowByte=				canInData[10]; 
	CANopenParameters.ControlWord.byte.HighByte=			canInData[11];
	CANopenParameters.StatusWord.byte.LowByte= 				canInData[12]; 
	CANopenParameters.StatusWord.byte.HighByte=				canInData[13]; 
    CANopenParameters.TargetPosition.byte.LowByte=          canInData[16];
    CANopenParameters.TargetPosition.byte.HighByte=         canInData[17];

	Decomposite_CAN_Data();		// composite data for control
}

void Write_CAN_Registers(void)
{
    Composite_CAN_Data();		// composite data for CANopen

	canOutData[12]= 	CANopenParameters.StatusWord.byte.LowByte;				
	canOutData[13]= 	CANopenParameters.StatusWord.byte.HighByte; 
	canOutData[14]=     CANopenParameters.Position.byte.LowByte;
	canOutData[15]=     CANopenParameters.Position.byte.HighByte;						
}




