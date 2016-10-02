#ifndef		__STRUCT_VARIABLES_
#define		__STRUCT_VARIABLES_



//--------------------------------------//
//-------State_index, Is_mag, Vdc, Wrpm_det_flt, Temperature, Torque_I,	Flux_I, OutPower, Ias, Ibs, Ics-----------//
// F1	Over Load:				
// F2	Over Current:			
// F4	Zero Sequence Current:	
// F7	Over Voltage:				
// F9	Under Votage:		
// F10	Over Speed:			 	
// F21	Over Temperature:			
// F22	Device Short:			
// F23	Chaging Error:			
// F24	Gate Driver Fail:		
// F25	External Fault:	
// F44	Rs Fail:
// F45	Rr Fail:
// F46	Ls Fail:
// F47	Lr Fail:			

// Fault 는 상위 bit->하위 bit 순으로 배치함 
// 외부인력 요청 (박병훈)
// Fault 1 Group
struct FAULT1_BITS	// bits   description 
{          
   Uint16	rsvd1:				6;		// bit . 1~6	F16~F11
   Uint16	OVER_SPEED:			1;		// bit . 7		F10
   Uint16	UV:					1;		// bit . 8		F9
   Uint16	rsvd2:				1;		// bit . 9		F8
   Uint16	OV:					1;		// bit . 10		F7
   Uint16	rsvd3:				2;		// bit . 11~12	F6~F5 
   Uint16	ZSC:				1;		// bit . 13		F4
   Uint16	rsvd4:				1;		// bit . 14		F3
   Uint16	OC:					1;		// bit . 15		F2
   Uint16	OL:					1;		// bit . 16		F1 
};

union FAULT1_REG 
{
   Uint16					all;
   struct	FAULT1_BITS		bit;
}; 

// Fault 2 Group
struct FAULT2_BITS	// bits   description 
{          
   Uint16	rsvd1:				7;		// bit . 1~7	F32~F26
   Uint16	EXT_FAULT:			1;		// bit . 8		F25
   Uint16	GATE_DRIVER_FAIL:	1;		// bit . 9		F24
   Uint16	CHAGING_ERROR:		1;		// bit . 10 	F23
   Uint16	DEVICE_SHORT:		1;		// bit . 11		F22
   Uint16	OT:					1;		// bit . 12		F21
   Uint16	rsvd2:				4;		// bit . 13~16	F20~F17
};

union FAULT2_REG 
{
   Uint16					all;
   struct	FAULT2_BITS		bit;
};

// Fault 3 Group
struct FAULT3_BITS	// bits   description 
{          
   Uint16	rsvd1:				1;		// bit . 1		F48
   Uint16	Lr_FAIL:			1;		// bit . 2		F47
   Uint16	Ls_FAIL:			1;		// bit . 3		F46
   Uint16	Rr_FAIL:			1;		// bit . 4		F45
   Uint16	Rs_FAIL:			1;		// bit . 5		F44
   Uint16	rsvd2:				11;		// bit . 6~16	F43~F33   
};

union FAULT3_REG 
{
   Uint16					all;
   struct	FAULT3_BITS		bit;
};

// Fault_Neglect 1 Group
struct FAULT_NEGLECT1_BITS	// bits   description 
{          
   Uint16	rsvd1:				6;		// bit . 1~6	F16~F11
   Uint16	OVER_SPEED:			1;		// bit . 7		F10
   Uint16	UV:					1;		// bit . 8		F9
   Uint16	rsvd2:				1;		// bit . 9		F8
   Uint16	OV:					1;		// bit . 10		F7
   Uint16	rsvd3:				2;		// bit . 11~12	F6~F5 
   Uint16	ZC:					1;		// bit . 13		F4
   Uint16	rsvd4:				1;		// bit . 14		F3
   Uint16	OC:					1;		// bit . 15		F2
   Uint16	OL:					1;		// bit . 16		F1
};

union FAULT_NEGLECT1_REG 
{
   Uint16							all;
   struct	FAULT_NEGLECT1_BITS		bit;
};  

// Fault_Neglect 2 Group
struct FAULT_NEGLECT2_BITS	// bits   description 
{          
   Uint16	rsvd1:				7;		// bit . 1~6	F32~F26
   Uint16	EXT_FAULT:			1;		// bit . 8		F25
   Uint16	GATE_DRIVER_FAIL:	1;		// bit . 9		F24
   Uint16	CHAGING_ERROR:		1;		// bit . 10 	F23
   Uint16	DEVICE_SHORT:		1;		// bit . 11		F22
   Uint16	OT:					1;		// bit . 12		F21
   Uint16	rsvd2:				4;		// bit . 13~16	F20~F17       
};

union FAULT_NEGLECT2_REG 
{
   Uint16							all;
   struct	FAULT_NEGLECT2_BITS		bit;
};

// Fault_Neglect 3 Group
struct FAULT_NEGLECT3_BITS	// bits   description 
{          
   Uint16	rsvd1:				1;		// bit . 1		F48
   Uint16	Lr_FAIL:			1;		// bit . 2		F47
   Uint16	Ls_FAIL:			1;		// bit . 3		F46
   Uint16	Rr_FAIL:			1;		// bit . 4		F45
   Uint16	Rs_FAIL:			1;		// bit . 5		F44
   Uint16	rsvd2:				11;		// bit . 6~16	F43~F33     
};

union FAULT_NEGLECT3_REG 
{
   Uint16							all;
   struct	FAULT_NEGLECT3_BITS		bit;
};

//--------------------------------------//
struct DIGITAL_IN_BITS 
{      
	Uint16 INVERTER_RUN:1;		//
	Uint16 INVERTER_DIR:1;		//	

	Uint16 DRIVE_ENABLE:1;      // 
	Uint16 MULTI_STEP_BIT0:1;	// 
	Uint16 MULTI_STEP_BIT1:1;	// 
	Uint16 MULTI_STEP_BIT2:1;	// 
	Uint16 MULTI_STEP_BIT3:1;	// 
	Uint16 FAULT_RESET:1;		// 
	Uint16 JOG:1;				//
//	Uint16 AI_REF_ACTIVE:1;		//
//	Uint16 AI_LOCAL_REMOTE:1;	//
	Uint16 EXT_FAULT_A:1;		//
	Uint16 EXT_FAULT_B:1;		//
//	Uint16 MOTOR_SELECT:1;		//
//	Uint16 MB_BRAKE_STATE:1;	//
//	Uint16 ACCEL_DECEL_SWITCH:1;	//
//	Uint16 REF_TUNING_INC:1;	//
//	Uint16 REF_TUNING_DEC:1;	//
//	Uint16 ACC_DEC_BYP:1;		//
//	Uint16 PID_CNTL_ENABLE:1;	//
//	Uint16 AUTO_PID_MODE:1;		//
//	Uint16 PID_GAIN_SEL:1;		//
//	Uint16 PID_I_RESET:1;		//
//	Uint16 TRQ_REF_OPT_BYP:1;	//
//	Uint16 TRQ_SIGN:1;			//
//	Uint16 TRQ_OUT_ZERO:1;		//
//	Uint16 TIMER_RUN_ENABLE:1;	//
//	Uint16 SLAVE_RUN_STATUS:1;	//
//	Uint16 SYNC_CTRL_OPT_BPY:1;	//
//	Uint16 FLYING_START:1;		//
//	Uint16 DISABLE_PROFIBUS:1;	//

};

struct DIGITAL_OUT_BITS
{
	Uint16 DRIVE_READY:1;	// driver ready
	Uint16 FAULT_OUT_A:1;
	Uint16 FAULT_OUT_B:1;
	Uint16 MOTOR_BRAKE:1;
//	Uint16 RUN_STOP:1;
//	Uint16 WARNING:1;
//	Uint16 DIRECTION:1;
//	Uint16 JOG_INPUT:1;
//	Uint16 VC_LIMIT:1;
//	Uint16 FREE:1;
};

union DIGITAL_OUT_REG
{
	Uint16					all;
	struct DIGITAL_OUT_BITS bit;
};

union DIGITAL_IN_REG
{
   Uint16					all;
   struct DIGITAL_IN_BITS	bit;
} ; 

struct MONITORING_BITS
{
	Uint16 INIT_DRIVE_CAL:1;
	Uint16 FAULT_RESET_COMPLETE:1;
	Uint16 MOTOR_TUNING_COMPLETE:1;
	Uint16 ACC:1;
	Uint16 DEC:1;
	Uint16 PWM_ON:1;
	Uint16 FW_MODE:2;
	Uint16 EEPROM_WRITE_ENABLE_Rx:1;
	Uint16 EEPROM_WRITE_ENABLE_Tx:1;
	Uint16 RUN_STOP_STATUS:1;
	Uint16 DIR_STATUS:1;
	Uint16 PWM_FREQUENCY_CHANGE:1;
	Uint16 DRIVE_CAL:1;
	Uint16 BREAK_CONTROL:1;
	Uint16 DRIVE_ENABLE_CONTROL:1;
	Uint16 WRITE_FAULT_RECORD:1;
	Uint16 AI_REF_ENABLE: 1;
};

union MONITORING_REG
{
	Uint16			all;
	struct MONITORING_BITS	bit;
};



typedef volatile struct FLAG_DEFINE 
{
	union	FAULT1_REG				Fault1;
	union	FAULT2_REG				Fault2;
	union	FAULT3_REG				Fault3;
	union	FAULT_NEGLECT1_REG		Fault_Neglect1;
	union	FAULT_NEGLECT2_REG		Fault_Neglect2;
	union	FAULT_NEGLECT3_REG		Fault_Neglect3;
	union	DIGITAL_IN_REG			DI;
	union	DIGITAL_OUT_REG			DO;
	union	MONITORING_REG			Monitoring;
	//	union	STATE_REG	State;
}FLAG;

struct PROGRAM_CONTROL
{
	int	P00_Device_number;
	int P01_Software_version1;
	int P02_Software_version2;
	int P03_Software_version3;
	int P04_Ias_offset1;
	int P05_Ias_offset2;
	int P06_Ibs_offset1;
	int P07_Ibs_offset2;
	int	P08_Password;
	int P09_IGBT_current_x10;
	int P10_Drive_power_x10_kW;					
};
struct BASIC_CONTROL_SETUP1
{
	int		P00_Rated_power_x10_kW;	
	int		P01_Rated_voltage;
	int		P02_Rated_current_x10;
	int		P03_Rated_frequency;
	int		P04_Number_of_poles;
	int		P05_Rated_speed;
	int		P06_Control_method;
	int		P07_PWM_frequency_x10_kHz;
	int		P09_Supply_voltage;	 
};

struct RAMP_CONTROL_CONFIGURATION1
{
	int		P00_Run_stop_source;
	int		P01_Reference_source;
//	int		P03_Stop_mode;
//	int		P04_Stop_hold_time;
//	int		P05_Output_off_hold_time;
	int		P09_Accel_switching_ref1_x1000;
//	int		P10_Accel_switching_ref2_x1000;
	int		P16_Accel_time1_x10;
	int		P17_Accel_time2_x10;
	int		P26_Decel_switching_ref1_x1000;
//	int		P27_Decel_switching_ref2_x1000;
	int		P33_Decel_time1_x10;	 
	int		P34_Decel_time2_x10;
//	int 	P42_Conunter_deceleration_ramp;
//	int 	P43_Conunter_deceleration_time_x10;
//	int		P49_Emergency_stop_mode;
//	int		P50_Emergency_stop_decel_time_x10;
	int		P53_S_curve_enable;
	int		P54_S_curve_weight_x1000;
};

struct PROTECTION_SETUP
{
	int		P00_Current_limit_x1000;
	int		P07_Max_continuous_current_x1000;
	int		P08_Overload_current_x1000;
	int		P09_Over_load_time_x10;
//	int		P10_Over_load_fault_action;
	int		P11_Over_current_trip_x1000;
	int		P12_Zero_sequence_current_trip_x1000;
//	int		P13_Over_voltage_limiting_function;
//	int		P14_Over_voltage_limit;
	int		P15_Over_voltage_trip;
//	int		P16_Under_voltage_compensation;
//	int		P17_UV_compensation_voltage;
	int		P18_Under_voltage_trip; 
//	int		P19_Open_phase_protection;
//	int		P20_Supply_frequency;
//	int		P21_Built_in_dynamic_brake;
//	int		P22_DB_switching_frequency_x10;
	int		P23_DB_start_voltage;
	int		P24_DB_full_voltage;
//	int		P25_Over_temperature_trip_action;
//	int		P30_Auto_restart_count;
//	int		P31_Retry_delay_time_x10;
//	int		P32_OC_auto_reset;
//	int		P33_OV_auto_reset;
//	int		P34_UV_auto_reset;
//	int		P37_Out_of_control_auto_reset;
//	int		P38_Out_of_control_time_x10;
//	int		P39_Out_of_control_current_x1000;
	int		P40_Over_temperature_trip_x10;
//	int		P42_Line_under_voltage_trip_x1000;
//	int		P43_Line_under_voltage_auto_reset;
//	int		P44_Line_unbalance_auto_reset;
};

struct ANALOG_INPUT
{
	int		P00_Analog_reference_source;
	int		P01_AI1_Function;
	int		P02_AI1_Type;
	int		P03_AI1_Filter_time_constant_ms;
	int		P04_AI1_Offset_x10;
	int		P05_AI1_Min_value_x10;
	int		P06_AI1_Max_value_x10;
	int		P07_AI1_Min_scale_x1000;
	int		P08_AI1_Max_scale_x1000;
	int		P09_AI1_Inversion;
	int		P10_AI1_Discretness;
	int		P11_AI1_Unit_selection;
	int		P12_AI2_Function;
	int		P13_AI2_Type;
	int		P14_AI2_Filter_time_constant_ms;
	int		P15_AI2_Offset_x10;
	int		P16_AI2_Min_value_x10;
	int		P17_AI2_Max_value_x10;
	int		P18_AI2_Min_scale_x1000;
	int		P19_AI2_Max_scale_x1000;
	int		P20_AI2_Inversion;
	int		P21_AI2_Discretness;
	int		P22_AI2_Unit_selection;

};
/*
struct	FREE_FUNCTION
{
	int		P00_Control_mode;
};
*/						
struct	DIGITAL_INPUT						
{
	int		P00_Run_stop_control;
	int		P01_DI03_function;		int		P02_DI04_function;
	int		P03_DI05_function;		int		P04_DI06_function;
	int		P05_DI07_function;		int		P06_DI08_function;
//	int		P07_DI09_function;		int		P08_DI10_function;
//	int		P09_DI11_function;		int		P10_DI12_function;
//	int		P11_Run_delay_time;
};

struct	MULTI_STEP_SET_POINT1				
{
	int		P00_JOG_set_x1000;
	int		P01_Step01_set_x10;		int		P02_Step02_set_x10;
	int		P03_Step03_set_x10;		int		P04_Step04_set_x10;
	int		P05_Step05_set_x10;		int		P06_Step06_set_x10;
	int		P07_Step07_set_x10;		int		P08_Step08_set_x10;
	int		P09_Step09_set_x10;		int		P10_Step10_set_x10;
	int		P11_Step11_set_x10;		int		P12_Step12_set_x10;
	int		P13_Step13_set_x10;		int		P14_Step14_set_x10;
	int		P15_Step15_set_x10;	
	int		P16_Unit_selection;
};

struct	AO_SETUP						
{
	int		P00_Output_selection;
	int		P01_Type;
//	int		P02_Adjustment_0mA;
//	int		P03_Adjustment_4mA;
//	int		P04_Adjustment_20mA;
	int		P05_Max_output_x1000;
//	int		P06_Inversion;	
};

struct	ANALOG_OUTPUT						
{
	struct	AO_SETUP					AO1;
//	struct	AO_SETUP					AO2;
};

struct	DIGITAL_OUTPUT						
{
	int		P00_DO1_function;
	int		P00_DO2_function;
	int		P00_DO3_function;
};

struct	MOTOR_BRAKE_CONTROL					
{
	int		P00_M1_locked_state_up_speed_set_x1000;
	int		P01_M1_locked_state_down_speed_set_x1000;
	int		P02_M1_break_open_current_x1000;
	int 	P03_M1_start_delay_time_x10;
	int		P04_M1_break_close_speed_set_x1000;
	int		P05_M1_break_open_torque_build_time_x10;
};

struct	AUTO_TUNING
{
	int		P09_Dead_time_compansation_x100_us;
};

struct	VECTOR_CONTROL1
{
	int		P00_Number_of_encoder_pulses;
	int		P01_Inversion_of_PG_direction;
	int		P03_Minimum_speed;
	int		P04_Maximum_speed_x1000;
	int		P05_Over_speed_limit_x1000;
};

struct	MOTOR_CONSTANT1						
{
	int		P00_Stator_resistance_x10_mOhm;
	int		P01_Rotor_resistance_x10_mOhm;
	int		P02_Stator_inductance_x10_mH;
	int		P03_Rotor_inductance_x10_mH;
	int		P04_Stator_transient_inductance_x10_mH;
	int		P05_Inertia_x1000;
};


typedef volatile struct PARAMETER_DEFINE 
{
	struct	PROGRAM_CONTROL						G00;
	struct	BASIC_CONTROL_SETUP1				G01;
	struct	RAMP_CONTROL_CONFIGURATION1			G03;
	struct	PROTECTION_SETUP					G05;
	struct	ANALOG_INPUT						G06;
//	struct	FREE_FUNCTION						G07;
	struct	DIGITAL_INPUT						G08;
	struct	MULTI_STEP_SET_POINT1				G09;
	struct	ANALOG_OUTPUT						G11;
	struct	DIGITAL_OUTPUT						G12;
	struct	MOTOR_BRAKE_CONTROL					G13;
	struct	AUTO_TUNING							G14;
	struct	VECTOR_CONTROL1						G19;
	struct	MOTOR_CONSTANT1						G21;
}PARAMETER;

struct RUN_STOP_CONTROL_BITS
{
	Uint16 Local_RUN:1;
	Uint16 Local_DIR:1;
	Uint16 Fieldbus_RUN:1;
	Uint16 Fieldbus_DIR:1;
	Uint16 Local:1;
	Uint16 Emergency_STOP:1;	// All mode
	Uint16 MOTOR_TUNING:1;	// Only Local Mode
	Uint16 FAULT_RESET:1;
	Uint16 DRIVE_CALIBRATION:1;
	Uint16 rsvd:7;
};

union RUN_STOP_CONTROL
{
	Uint16							all;
	struct RUN_STOP_CONTROL_BITS	bit;
};

struct REFERENCE
{
	int Speed;
	int Frequency_x10;
	int Torque_x1000;
	int PID_x1000;
};


union REFERENCE_CONTROL
{
	struct REFERENCE		Local;
	struct REFERENCE		Fieldbus;
};

struct FAULT_RECORDING_DATA
{
	Uint16	READ:		1;
	Uint16	CLEAR:		1;
	int		TOTAL;
	int		NOW;
};

typedef volatile struct OPERATION_DEFINE 
{
	union	RUN_STOP_CONTROL		Run_stop;
	union	REFERENCE_CONTROL		Reference;
	struct	FAULT_RECORDING_DATA	Fault_recording;
}OPERATION;


//--------------------------------------//







#endif

