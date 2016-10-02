#include	<All_Header.h>
#include	<All_Extern_Variables.h>

void Parameter_Initialization_by_Power()
{
	int Drive_power= 0;

	if (GpioDataRegs.GPCDAT.bit.GPIO68) Drive_power+= 1;
	if (GpioDataRegs.GPCDAT.bit.GPIO67) Drive_power+= 2;
	if (GpioDataRegs.GPCDAT.bit.GPIO66) Drive_power+= 4;
	if (GpioDataRegs.GPCDAT.bit.GPIO65) Drive_power+= 8;
	if (GpioDataRegs.GPCDAT.bit.GPIO64) Drive_power+= 16;	
	
	switch ( Drive_power )
	{
		case 0:
			P.G00.Drive_power_x10_kW = 55;
			I_scale= 100.0/1.8;
			P.G00.IGBT_current= 35.;
			P.G01.P00_Rated_power_x10_kW= 55;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 1:
			P.G00.Drive_power_x10_kW = 75;
			I_scale= 100.0/1.43;
			P.G00.IGBT_current= 35.;
			P.G01.P00_Rated_power_x10_kW= 75;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break; 	
		case 2:
		 	P.G00.Drive_power_x10_kW = 110; 
			P.G00.IGBT_current= 35.;
			P.G01.P00_Rated_power_x10_kW= 110;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 3: 
			P.G00.Drive_power_x10_kW = 150; 
			P.G00.IGBT_current= 35.;
			P.G01.P00_Rated_power_x10_kW= 150;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 4: 
			P.G00.Drive_power_x10_kW = 185; 
			I_scale= 200.0/0.85;
			P.G00.IGBT_current= 63.;
			P.G01.P00_Rated_power_x10_kW= 185;
			P.G01.P01_Rated_voltage= 415;
			P.G01.P02_Rated_current_x10= 350;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1465;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 380;
			break;
		case 5: // (110104 by HHH) 현재 실험중
			P.G00.Drive_power_x10_kW = 220;
			I_scale= 100.0/0.85;
			P.G00.IGBT_current= 63;
			P.G01.P00_Rated_power_x10_kW= 110;
			P.G01.P01_Rated_voltage= 380;
			P.G01.P02_Rated_current_x10= 232;
			P.G01.P03_Rated_frequency= 60;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1770;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 380;
			break;
		case 6: 
			P.G00.Drive_power_x10_kW = 300;
			P.G00.IGBT_current= 35.;
			P.G01.P00_Rated_power_x10_kW= 300;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 7: 
			P.G00.Drive_power_x10_kW = 370; 
			P.G00.IGBT_current= 35.;
			P.G01.P00_Rated_power_x10_kW= 370;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 8: 
			P.G00.Drive_power_x10_kW = 450;
			P.G00.IGBT_current= 35.;
			P.G01.P00_Rated_power_x10_kW= 450;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 9: 
			P.G00.Drive_power_x10_kW = 550;
			P.G00.IGBT_current= 35.;
			P.G01.P00_Rated_power_x10_kW= 550;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 10: 
			P.G00.Drive_power_x10_kW = 750;
			P.G00.IGBT_current= 35.; 	
			P.G01.P00_Rated_power_x10_kW= 750;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 11: 
			P.G00.Drive_power_x10_kW = 900; 
			P.G00.IGBT_current= 35.;
			P.G01.P00_Rated_power_x10_kW= 900;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 12: 
			P.G00.Drive_power_x10_kW = 1100;
			P.G00.IGBT_current= 35.;
			P.G01.P00_Rated_power_x10_kW= 1100;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 13: 
			P.G00.Drive_power_x10_kW = 1320; 
			P.G00.IGBT_current= 35.;
			P.G01.P00_Rated_power_x10_kW= 1320;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 14: 
			P.G00.Drive_power_x10_kW = 1600; 
			P.G00.IGBT_current= 35.;	
			P.G01.P00_Rated_power_x10_kW= 1600;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 15: 
			P.G00.Drive_power_x10_kW = 2000;
			P.G00.IGBT_current= 35.;
			P.G01.P00_Rated_power_x10_kW= 2000;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 16: 
			P.G00.Drive_power_x10_kW = 2500;
			P.G00.IGBT_current= 35.;
			P.G01.P00_Rated_power_x10_kW= 2500;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 17: 
			P.G00.Drive_power_x10_kW = 3150;
			P.G00.IGBT_current= 35.; 
			P.G01.P00_Rated_power_x10_kW= 3150;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 18: 
			P.G00.Drive_power_x10_kW = 4000;
			P.G00.IGBT_current= 35.;
			P.G01.P00_Rated_power_x10_kW= 4000;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 19:
		 	P.G00.Drive_power_x10_kW = 5000; 
		 	P.G00.IGBT_current= 35.;	
			P.G01.P00_Rated_power_x10_kW= 5000;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 20: 
			P.G00.Drive_power_x10_kW = 5600; 
			P.G00.IGBT_current= 35.;
			P.G01.P00_Rated_power_x10_kW= 5600;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 21: 
			P.G00.Drive_power_x10_kW = 6300; 
			P.G00.IGBT_current= 35.;
			P.G01.P00_Rated_power_x10_kW= 6300;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 22: 
			P.G00.Drive_power_x10_kW = 7150; 
			P.G00.IGBT_current= 35.;
			P.G01.P00_Rated_power_x10_kW= 7150;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 23: 
			P.G00.Drive_power_x10_kW = 7800; 
			P.G00.IGBT_current= 35.;
			P.G01.P00_Rated_power_x10_kW= 7800;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 30: 
			P.G00.Drive_power_x10_kW = 22; 
			P.G00.IGBT_current= 35.;
			P.G01.P00_Rated_power_x10_kW= 22;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 31: 
			P.G00.Drive_power_x10_kW = 37; 
			P.G00.IGBT_current= 35.;
			P.G01.P00_Rated_power_x10_kW= 37;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
	} 
}

void Parameter_Initialization()
{
	OP.Run_stop.bit.Emergency_STOP= 0;
	OP.Run_stop.bit.Local_RUN= 0;					
	OP.Run_stop.bit.Local_DIR= 0;					
	OP.Reference.Local.Speed= 0;					
	OP.Reference.Local.Frequency_x10= 0;			
	OP.Reference.Local.Torque_x1000= 0;		
	OP.Reference.Local.PID_x1000= 0;				
	OP.Run_stop.bit.Fieldbus_RUN= 0;				
	OP.Run_stop.bit.Fieldbus_DIR= 0;				
	OP.Reference.Fieldbus.Speed= 0;				
	OP.Reference.Fieldbus.Frequency_x10= 0;		
	OP.Reference.Fieldbus.Torque_x1000=	0;		 
	OP.Reference.Fieldbus.PID_x1000= 0;				
	OP.Run_stop.bit.Emergency_STOP= 0;
					
	P.G00.Password= 0;
												// default //
	P.G03.P00_Run_stop_source= 0; 				// terminal
	P.G03.P01_Reference_source= 0;				// terminal
	P.G03.P03_Stop_mode= 0;						// ramp stop
	P.G03.P04_Stop_hold_time= 0;				// 0s
	P.G03.P05_Output_off_hold_time= 1; 			// 1s
	P.G03.P09_Accel_switching_ref1_x1000= 1000;		// 100%
	P.G03.P10_Accel_switching_ref2_x1000= 1500;		// 150%
	P.G03.P16_Accel_time1_x10= 20;					// 5s
	P.G03.P17_Accel_time2_x10= 20;					// 5s
	P.G03.P26_Decel_switching_ref1_x1000= 1000;			// 100%	
	P.G03.P27_Decel_switching_ref2_x1000= 1500;		// 150%	
	P.G03.P33_Decel_time1_x10= 20;					// 5s	 
	P.G03.P34_Decel_time2_x10= 20;					// 5s
	P.G03.P42_Conunter_deceleration_ramp= 0;	// disabled
	P.G03.P43_Conunter_deceleration_time_x10= 50;	// 5s
	P.G03.P49_Emergency_stop_mode= 0;			// ramp stop
	P.G03.P50_Emergency_stop_decel_time_x10= 10;	// 1s

	P.G05.P00_Current_limit_x1000= 1300;				// 130%
	P.G05.P07_Max_continuous_current_x1000= 950;		// 95%
	P.G05.P08_Overload_current_x1000= 1350;			// 135%
	P.G05.P09_Over_load_time_x10= 600;				// 60s
	P.G05.P10_Over_load_fault_action= 0;		// normal stop
	P.G05.P11_Over_current_trip_x1000= 1300;			// 200% -> 120%
	P.G05.P12_Zero_sequence_current_trip_x1000= 0.15;	// 15%
	P.G05.P13_Over_voltage_limiting_function= 0;// disabled
	P.G05.P14_Over_voltage_limit= 690;			// 350V 임의값 
	P.G05.P15_Over_voltage_trip= 710;			// 380V 임의값 
	P.G05.P16_Under_voltage_compensation= 0;	// disabled
	P.G05.P17_UV_compensation_voltage= 450;		// 270V 임의값 
	P.G05.P18_Under_voltage_trip= 400; 			// 250V 임의값
	P.G05.P19_Open_phase_protection= 0;			// disabled
	P.G05.P20_Supply_frequency= 60;				// 60 Hz
	P.G05.P21_Built_in_dynamic_brake= 0;		// disabled
	P.G05.P22_DB_switching_frequency= 5.0;		// 5.0 임의값 
	P.G05.P23_DB_start_voltage= 600;			// 350V 임의값
	P.G05.P24_DB_full_voltage= 630;				// 370V 임의값
	P.G05.P25_Over_temperature_trip_action= 0;	// Control off
	P.G05.P30_Auto_restart_count= 0;			// 0
	P.G05.P31_Retry_delay_time_x10= 15;			// 1.5s
	P.G05.P32_OC_auto_reset= 0;					// disabled
	P.G05.P33_OV_auto_reset= 0;					// disabled
	P.G05.P34_UV_auto_reset= 0;					// disabled
	P.G05.P37_Out_of_control_auto_reset= 0;		// disabled
	P.G05.P38_Out_of_control_time_x10= 50;			// 5.0s
	P.G05.P39_Out_of_control_current_x1000= 900;		// 90%
	P.G05.P40_Over_temperature_trip_x10= 850;		// 85 deg
	P.G05.P42_Line_under_voltage_trip_x1000= 900;		// 90%
	P.G05.P43_Line_under_voltage_auto_reset= 0;	// disabled
	P.G05.P44_Line_unbalance_auto_reset= 0;		// disabled

	P.G06.AI0_Analog_reference_source= 1;		// AI 1
	P.G06.AI1.P01_Function= 1;					// AI
	P.G06.AI1.P02_Type= 0;						// 0~10V
	P.G06.AI1.P03_Filter_time_constant_x10_mA= 5000;	// 0.5s
	P.G06.AI1.P04_Offset_x10= 0;					// 0
	P.G06.AI1.P05_Minimum_x10= 50;			// 5%
	P.G06.AI1.P06_Maximum_x10= 1000;			// 100%
	P.G06.AI1.P07_Inversion= 0;					// disabled
	P.G06.AI1.P08_Discretness= 0;				// disabled
	P.G06.AI1.P09_Unit_selection= 1;			// %
	P.G06.AI2.P01_Function= 1;					// AI
	P.G06.AI2.P02_Type= 0;						// 0~10V
	P.G06.AI2.P03_Filter_time_constant_x10_mA= 5000;	// 0.5s
	P.G06.AI2.P04_Offset_x10= 0;					// 0
	P.G06.AI2.P05_Minimum_x10= 50;			// 5%
	P.G06.AI2.P06_Maximum_x10= 1000;			// 100%
	P.G06.AI2.P07_Inversion= 0;					// disabled
	P.G06.AI2.P08_Discretness= 0;				// disabled
	P.G06.AI2.P09_Unit_selection= 1;			// %
/*
	P.G06.AI3.P01_Function= 1;					// AI
	P.G06.AI3.P02_Type= 0;						// 0~10V
	P.G06.AI3.P03_Filter_time_constant_x10_mA= 5000;	// 0.5s
	P.G06.AI3.P04_Offset_x10= 0;					// 0
	P.G06.AI3.P05_Minimum_x10= 50;			// 5%
	P.G06.AI3.P06_Maximum_x10= 1000;			// 100%
	P.G06.AI3.P07_Inversion= 0;					// disabled
	P.G06.AI3.P08_Discretness= 0;				// disabled
	P.G06.AI3.P09_Unit_selection= 1;			// %
	P.G06.AI4.P01_Function= 1;					// AI
	P.G06.AI4.P02_Type= 0;						// 0~10V
	P.G06.AI4.P03_Filter_time_constant_x10_mA= 5000;	// 0.5s
	P.G06.AI4.P04_Offset_x10= 0;					// 0
	P.G06.AI4.P05_Minimum_x10= 50;			// 5%
	P.G06.AI4.P06_Maximum_x10= 1000;			// 100%
	P.G06.AI4.P07_Inversion= 0;					// disabled
	P.G06.AI4.P08_Discretness= 0;				// disabled
	P.G06.AI4.P09_Unit_selection= 1;			// % 
	P.G06.AI5.P01_Function= 1;					// AI
	P.G06.AI5.P02_Type= 0;						// 0~10V
	P.G06.AI5.P03_Filter_time_constant_x10_mA= 5000;	// 0.5s
	P.G06.AI5.P04_Offset_x10= 0;					// 0
	P.G06.AI5.P05_Minimum_x10= 50;			// 5%
	P.G06.AI5.P06_Maximum_x10= 1000;			// 100%
	P.G06.AI5.P07_Inversion= 0;					// disabled
	P.G06.AI5.P08_Discretness= 0;				// disabled
	P.G06.AI5.P09_Unit_selection= 1;			// % 
*/

	P.G07.P00_Control_mode= 0;					// disabled

	P.G08.P00_Run_stop_control= 0;				// FWD/REV
	P.G08.P01_DI03_function= 2;					// none	- > MULTI_STEP_BIT0
	P.G08.P02_DI04_function= 3;					// none	- > MULTI_STEP_BIT1	
	P.G08.P03_DI05_function= 4;					// none	- > MULTI_STEP_BIT2			
	P.G08.P04_DI06_function= 5;				// none	- > MULTI_STEP_BIT3	
	P.G08.P05_DI07_function= 0;					// none			
	P.G08.P06_DI08_function= 0;					// none	
	P.G08.P07_DI09_function= 0;					// none			
	P.G08.P08_DI10_function= 0;					// none	
	P.G08.P09_DI11_function= 0;					// none			
	P.G08.P10_DI12_function= 0;					// none	
	P.G08.P11_Run_delay_time= 0;				// 0s


	P.G09.P00_JOG_set_x1000= 20;						// 20%
	P.G09.P01_Step01_set_x10= 100;					// 15%	
	P.G09.P02_Step02_set_x10= 200;					// 30%
	P.G09.P03_Step03_set_x10= 300;					// 50%
	P.G09.P04_Step04_set_x10= 400;					// 100%
	P.G09.P05_Step05_set_x10= 500;					// 100%		
	P.G09.P06_Step06_set_x10= 600;					// 100%
	P.G09.P07_Step07_set_x10= 700;					// 100%		
	P.G09.P08_Step08_set_x10= 800;					// 100%
	P.G09.P09_Step09_set_x10= 900;					// 100%		
	P.G09.P10_Step10_set_x10= 1000;					// 100%
	P.G09.P11_Step11_set_x10= 1000;					// 100%		
	P.G09.P12_Step12_set_x10= 1000;					// 100%
	P.G09.P13_Step13_set_x10= 1000;					// 100%		
	P.G09.P14_Step14_set_x10= 1000;					// 100%
	P.G09.P15_Step15_set_x10= 1000;					// 100%	
	P.G09.P16_Unit_selection= 0;				// percent[%]

	P.G11.AO1.P00_Output_selection= 1;			// motor speed
	P.G11.AO1.P01_Type= 0;
	P.G11.AO1.P02_Adjustment_0mA= 0;
	P.G11.AO1.P03_Adjustment_4mA= 0;
	P.G11.AO1.P04_Adjustment_20mA= 0;
	P.G11.AO1.P05_Max_output_x1000= 1000;				// 100%
	P.G11.AO1.P06_Inversion= 0;					// disabled
	P.G11.AO2.P00_Output_selection= 0;			// output frequency
	P.G11.AO2.P01_Type= 1;
	P.G11.AO2.P02_Adjustment_0mA= 0;
	P.G11.AO2.P03_Adjustment_4mA= 0;
	P.G11.AO2.P04_Adjustment_20mA= 0;
	P.G11.AO2.P05_Max_output_x1000= 1000;
	P.G11.AO2.P06_Inversion= 0;	
		
	P.G12.P00_DO1_function= 0;					// disabled
	P.G12.P00_DO2_function= 0;	
	P.G12.P00_DO3_function= 0;

	P.G13.P00_M1_locked_state_up_speed_up_x1000= 4000;// 4%

	P.G14.P09_Dead_time_compansation_x100_us= 99;

	P.G19.P00_Number_of_encoder_pulses= 1024;	//1024
	P.G19.P01_Inversion_of_PG_direction= 0;		//disabled
	P.G19.P03_Minimum_speed= 0;					// 0 rpm
	P.G19.P04_Maximum_speed_x1000= 1000;					// 100%
	P.G19.P05_Over_speed_limit_x1000= 1250;			// 125%
/*
	P.G21.P00_Stator_resistance_x1000= 282;
	P.G21.P01_Rotor_resistance_x1000= 99;
	P.G21.P02_Stator_inductance_x1000= 51;
	P.G21.P03_Rotor_inductance_x1000= 51;
	P.G21.P04_Stator_transient_inductance_x1000= 4;
	P.G21.P05_Inertia_x1000= 10;
*/
	// PS 부하용 11kW Motor
	P.G21.P00_Stator_resistance_x1000= 450;
	P.G21.P01_Rotor_resistance_x1000= 151;
	P.G21.P02_Stator_inductance_x1000= 77;
	P.G21.P03_Rotor_inductance_x1000= 76;
	P.G21.P04_Stator_transient_inductance_x1000= 7;
	P.G21.P05_Inertia_x1000= 100;
}

void System_Variable_Initialization()
{
	double OL_current;
	
//	Flag.DI.all= 0;
	Flag.DO.all= 0;
	Flag.Fault1.all= 0;
	Flag.Fault2.all= 0;
	Flag.Fault_Neglect1.all= 0;
	Flag.Fault_Neglect2.all= 0;

	Flag.Monitoring.all= 0;
	Flag.Monitoring.bit.FAULT_RESET_COMPLETE= 1; 
	
//	OP.Run_stop.all= 0;
	FaultInfo.DATA= 0.;
	Voltage_class= 0;
	Command= CMD_STOP;
	Init_reference= 0.;

	



#if (DUAL_PWM_INTERRUPT)
	Tsamp= Tsamp_VF= Tsamp_CC= 1./((double)P.G01.P07_PWM_frequency_x10_kHz*1e+2)/2.;
#else
	Tsamp= Tsamp_VF= Tsamp_CC= 1./((double)P.G01.P07_PWM_frequency_x10_kHz*1e+2);
#endif 
 
	Tsamp_SC= Tsamp_SD= Tsamp_FC= Tsamp*4.;
	Period_SD= (Uint32)(CPU_CLOCK*Tsamp*4.); 		// EQEP Timer Period
	 

	Vdc_max=0;
	I_DT= (double)P.G00.IGBT_current*0.01;
	T_dead_Tuning= (double)P.G14.P09_Dead_time_compansation_x100_us * 1.e-2;

//--------Motor Parameters--------//
//	Rs= 0.28175;
//	Rr= 0.09948;
// 	Ls= 0.05135;
//	Lr= 0.05084;
//	Lsigma= 4.09e-3;


	Rs= (double)(P.G21.P00_Stator_resistance_x1000)*1.e-3;
	Rr= (double)(P.G21.P01_Rotor_resistance_x1000)*1.e-3;
 	Ls= (double)(P.G21.P02_Stator_inductance_x1000)*1.e-3;
	Lr= (double)(P.G21.P03_Rotor_inductance_x1000)*1.e-3;
	Lsigma= (double)(P.G21.P04_Stator_transient_inductance_x1000)*1.e-3;
	Lm= sqrt((Ls-Lsigma)*Lr);
	Jm= (double)(P.G21.P05_Inertia_x1000)*1.e-3;
	inv_Jm=1./Jm;
	Pole_pair=(double)(P.G01.P04_Number_of_poles)/2.; Bm= 0.; 
	LAMdre_rate = 0.45*(double)(P.G01.P08_Supply_voltage)/220.;
	LAMdre_ref = 0.45*(double)(P.G01.P08_Supply_voltage)/220.; 
//	Is_rate=(20.6*SQRT2), Wrpm_rate=1740., 
	// Wrpm_max=3500.;
	PPR= P.G19.P00_Number_of_encoder_pulses;

	// 정격출력(W)/정격속도(rad/sec) -> 정격토크
	// 정격토크 = 정격출력/( (2xPI/60) x rpm)
	//         =  정격출력 x (30/PI) / rpm 
	// Te_rated_f = (1.0e3*(1.0e-1*P.G01.P00_Rated_power_x10_kW)*30)/(PI * P.G01.P05_Rated_speed) 
	// 				->	1.0e2 x 30 / PI x (P.G01.P00_Rated_power_x10_kW / P.G01.P05_Rated_speed)
	//				->	954.9305 x (P.G01.P00_Rated_power_x10_kW / P.G01.P05_Rated_speed) 
	Te_rate =  (954.9305*(double)P.G01.P00_Rated_power_x10_kW)/(double)P.G01.P05_Rated_speed;
 
//------Motor Parameters End------//  
	
	Is_max= ((double)P.G01.P02_Rated_current_x10)*0.1*SQRT2;
	if (Is_max>((double)P.G00.IGBT_current*0.85))	OC_trip_value= (double)P.G00.IGBT_current*0.85;
	else OC_trip_value= Is_max;

	OL_current= Is_max*(double)P.G05.P08_Overload_current_x1000*1.e-3;
	Continuous_OL_current= Is_max*(double)P.G05.P07_Max_continuous_current_x1000*1.e-3;
	OL_limit= (OL_current*OL_current - Continuous_OL_current*Continuous_OL_current)*(double)P.G05.P09_Over_load_time_x10*1.e-1;


	// Motor Parmeter
 //	Ls = Lr = (Lsigma + sqrt(Lsigma*Lsigma + 4.*Lm*Lm) ) / 2.;

	//encoder
	EQep1Regs.QPOSLAT= 0;
	

	Init_reference=0.;
	Final_reference=0.; 
	Wrpm_scale=(double)P.G01.P05_Rated_speed*(double)P.G01.P08_Supply_voltage/(double)P.G01.P01_Rated_voltage;
	Accel_time=(double)P.G03.P16_Accel_time1_x10 * 0.1;	
	Decel_time=(double)P.G03.P33_Decel_time1_x10 * 0.1;
	 
	Theta=0.0;

	// 전류
	Is_mag=0.0;
	Is_mag_rms=0.0;


	// Flux Estimation  
	K_CM1= (1.-(Tsamp_CC/2.)*(Rr/Lr))/(1.+(Tsamp_CC/2.)*(Rr/Lr));
	K_CM2= (Tsamp_CC/2.)*(Rr/Lr)*Lm/(1.+(Tsamp_CC/2.)*(Rr/Lr));

	Freq_Fst= 10.;
	Wc_Fst= (2.*PI)*Freq_Fst;
	Kp_Fst= (Lm/Lr)*SQRT2*Wc_Fst;
	Ki_Fst= (Lm/Lr)*Wc_Fst*Wc_Fst;

	// Current Controller
	Wc_cc= 1250.*(double)P.G01.P07_PWM_frequency_x10_kHz/50.; //1250
	Kp_cc= Lsigma*Wc_cc; 
	Ki_cc= Rs*2.*Wc_cc;  
	Ka_cc= 3./Kp_cc;

	// Flux Controller
	Wc_fc= 50.*(double)P.G01.P07_PWM_frequency_x10_kHz/50.;
	Kp_fc= (Lr/Lm)*Wc_fc/Rr;
	Ki_fc= Wc_fc/Lm;
	Ka_fc= 2./Kp_fc;
	Idse_ref_max = Is_max/SQRT2; 		
	

	// Speed Controller 
	Wc_sc= 250.*(double)P.G01.P07_PWM_frequency_x10_kHz/50.;
//	Kp_sc= Jm*Wc_sc/Kt;
//	Ki_sc= Kp_sc*(Wc_sc/7.);
//	Ka_sc= 2./Kp_sc;

	// Speed_Detection
	Kob_sd=	1000.*200.e-6/(Tsamp_SD);	// Speed Observer 
	L1_sp_est= 3.*Kob_sd;
	L2_sp_est= 3.*Kob_sd*Kob_sd;
	L3_sp_tmp= -(Kob_sd*Kob_sd*Kob_sd);
//	L3_sp_est= -(SPEED_OB_GAIN*SPEED_OB_GAIN*SPEED_OB_GAIN)*Jm;

	//Field Weakening
	K_fw1= (LAMdre_rate*LAMdre_rate)*(Ls*Ls-Lsigma*Lsigma)/(Lm*Lm)
	           +(Lsigma*Lsigma)*(Is_max*Is_max);
	K_fw2= sqrt( (Ls*Ls+Lsigma*Lsigma)/(2.*Ls*Ls*Lsigma*Lsigma) );

	cishu =cs;
//  M = (Td + Ton - Toff) * ISR_FREQUENCY;//[(Td + Ton - Toff)/1000]/T=(d + Ton - Toff)*ISR_FREQUENCY;(pu).used by voltage estimation
	M = (Td + Ton - Toff) * 2.5;//[(Td + Ton - Toff)/1000]/T=(d + Ton - Toff)*ISR_FREQUENCY;(pu).used by voltage estimation/
	M1 = M * (Vd-Vs);//used by voltage estimation.
	C1 = 1.0/(6.2831853 * Hui * T+1.0);//RC/( T + RC);//
	C2 = 1.0 - C1;
 	B1 = (1 + b) * 0.5;
	B2 = (1 - b) * 0.5; 
//	cc1 = 1.0/(6.2831853 * Hui1 * T+1.0);//RC/( T + RC);//
//	cc2 = 1.0 - cc1;
    A2 = Wc * T * a2;
	A1 = Wc * Wc * T * a1;
	A0 = Wc * Wc * Wc * a0 * T;



}

void System_Variable_Update()
{
	double OL_current;

#if (DUAL_PWM_INTERRUPT)
	Tsamp= Tsamp_VF= Tsamp_CC= 1./((double)P.G01.P07_PWM_frequency_x10_kHz*1e+2)/2.;
#else
	Tsamp= Tsamp_VF= Tsamp_CC= 1./((double)P.G01.P07_PWM_frequency_x10_kHz*1e+2);
#endif 
 
	Tsamp_SC= Tsamp_SD= Tsamp_FC= Tsamp*4.;
	Period_SD= (Uint32)(CPU_CLOCK*Tsamp*4.); 		// EQEP Timer Period

	I_DT= (double)P.G00.IGBT_current*0.01;
	// autotuning 완료 이후 교체 할것
	Rs= (double)(P.G21.P00_Stator_resistance_x1000)*1.e-3;
	Rr= (double)(P.G21.P01_Rotor_resistance_x1000)*1.e-3;
 	Ls= (double)(P.G21.P02_Stator_inductance_x1000)*1.e-3;
	Lr= (double)(P.G21.P03_Rotor_inductance_x1000)*1.e-3;
	Lsigma= (double)(P.G21.P04_Stator_transient_inductance_x1000)*1.e-3;
	Lm= sqrt((Ls-Lsigma)*Lr);
	Jm= (double)(P.G21.P05_Inertia_x1000)*1.e-3;
	inv_Jm=1./Jm;
	Pole_pair=(double)(P.G01.P04_Number_of_poles)/2.; Bm= 0.; 
	LAMdre_rate = 0.45*(double)(P.G01.P08_Supply_voltage)/220.;
	LAMdre_ref = 0.45*(double)(P.G01.P08_Supply_voltage)/220.;  
	PPR= P.G19.P00_Number_of_encoder_pulses;
 	
	Te_rate =  (954.9305*(double)P.G01.P00_Rated_power_x10_kW)/(double)P.G01.P05_Rated_speed;
//------Motor Parameters End------//  
	
	Is_max= ((double)P.G01.P02_Rated_current_x10)*0.1*SQRT2;
	if (Is_max>((double)P.G00.IGBT_current*0.85))	OC_trip_value= (double)P.G00.IGBT_current*0.85;
	else OC_trip_value= Is_max;
	
	OL_current= Is_max*(double)P.G05.P08_Overload_current_x1000*1.e-3;
	Continuous_OL_current= Is_max*(double)P.G05.P07_Max_continuous_current_x1000*1.e-3;
	OL_limit= (OL_current*OL_current - Continuous_OL_current*Continuous_OL_current)*(double)P.G05.P09_Over_load_time_x10*1.e-1;

	Wrpm_scale=(double)P.G01.P05_Rated_speed*(double)P.G01.P08_Supply_voltage/(double)P.G01.P01_Rated_voltage;
	Accel_time=(double)P.G03.P16_Accel_time1_x10 * 0.1;	
	Decel_time=(double)P.G03.P33_Decel_time1_x10 * 0.1;
	 
	// Flux Estimation  
	K_CM1= (1.-(Tsamp_CC/2.)*(Rr/Lr))/(1.+(Tsamp_CC/2.)*(Rr/Lr));
	K_CM2= (Tsamp_CC/2.)*(Rr/Lr)*Lm/(1.+(Tsamp_CC/2.)*(Rr/Lr));


//	Freq_Fst= 10.;
//	Wc_Fst= (2.*PI)*Freq_Fst;
	Kp_Fst= (Lm/Lr)*SQRT2*Wc_Fst;
	Ki_Fst= (Lm/Lr)*Wc_Fst*Wc_Fst;

	// Current Controller
	Wc_cc= 1250.*(double)P.G01.P07_PWM_frequency_x10_kHz/50.; //1250
	Kp_cc= Lsigma*Wc_cc; 
	Ki_cc= Rs*2.*Wc_cc;  
	Ka_cc= 3./Kp_cc;

	// Flux Controller
	Wc_fc= 50.*(double)P.G01.P07_PWM_frequency_x10_kHz/50.;
	Kp_fc= (Lr/Lm)*Wc_fc/Rr;
	Ki_fc= Wc_fc/Lm;
	Ka_fc= 2./Kp_fc;
	Idse_ref_max = Is_max/SQRT2; 		
	

	// Speed Controller 
	Wc_sc= 250.*(double)P.G01.P07_PWM_frequency_x10_kHz/50.;

	//Field Weakening
	K_fw1= (LAMdre_rate*LAMdre_rate)*(Ls*Ls-Lsigma*Lsigma)/(Lm*Lm)
	           +(Lsigma*Lsigma)*(Is_max*Is_max);
	K_fw2= sqrt( (Ls*Ls+Lsigma*Lsigma)/(2.*Ls*Ls*Lsigma*Lsigma) );

}




//---------------------------------
// end of file
//----------------------------------

