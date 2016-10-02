#include "math.h"
#include "main_def.h"

void set_digit_input_funtion( int input_state, int function_option );

int input_state(int port)
{
	unsigned int xbus_in;
	int i,j;

	i = 1;

	j = port;

	while( j ){	i *= 2; j--; }
	asm ("      nop");

	xbus_in = ( ZONE0_BUF[0x0050] & i ); 
	if( xbus_in ) 	return 0;
	else 			return 1;  
}


void set_digit_input_funtion( int input_state, int function_option )
{
	switch( function_option ){

	case 0:	// None
		break;
	case 1:	// drive enable 
		if( input_state ) func_flag.bit.DRIVE_ENABLE = 1;
		else  func_flag.bit.DRIVE_ENABLE = 0;
		break;
	case 2:	// multi step bit 0 
		if( input_state ) func_flag.bit.MULTI_STEP_BIT0 = 1;
		else func_flag.bit.MULTI_STEP_BIT0 = 0;
		break;
	case 3:	// multi step bit 1 
		if( input_state ) func_flag.bit.MULTI_STEP_BIT1 = 1;
		else func_flag.bit.MULTI_STEP_BIT1 = 0;
		break;
	case 4:	// multi step bit 2 
		if( input_state ) func_flag.bit.MULTI_STEP_BIT2 = 1;
		else func_flag.bit.MULTI_STEP_BIT2 = 0;
		break;
	case 5:	// multi step bit 3 
		if( input_state ) func_flag.bit.MULTI_STEP_BIT3 = 1;
		else func_flag.bit.MULTI_STEP_BIT3 = 0;
		break;
	case 6:	// fault_reset 
		if( input_state ) func_flag.bit.FAULT_RESET = 1;
		else func_flag.bit.FAULT_RESET = 0;
		break;
	case 7:	// jog 
		if( input_state ) func_flag.bit.JOG = 1;
		else func_flag.bit.JOG = 0;
		break;
/*
	case 8:	// 
		if( input_state ) func_flag.bit.AI_REF_ACTIVE = 1;
		break;
	case 9:	// 
		if( input_state ) func_flag.bit.AI_LOCAL_REMOTE = 1;
		break;
	case 10:	// 
		if( input_state ) func_flag.bit.EXT_FAULT_A = 1;
		break;
	case 11:	// 
		if( input_state ) func_flag.bit.EXT_FAULT_B = 1;
		break;
	case 12:	// 
		if( input_state ) func_flag.bit.MOTOR_SELECT = 1;
		break;
	case 13:	// 
		if( input_state ) func_flag.bit.MB_BRAKE_STATE = 1;
		break;
	case 14:	// 
		if( input_state ) func_flag.bit.ACCEL_DECEL_SWITCH = 1;
		break;
	case 15:	// 
		if( input_state ) func_flag.bit.REF_TUNING_INC = 1;
		break;
	case 16:	// 
		if( input_state ) func_flag.bit.REF_TUNING_DEC = 1;
		break;
	case 17:	// 
		if( input_state ) func_flag.bit.ACC_DEC_BYP = 1;
		break;
	case 18:	// 
		if( input_state ) func_flag.bit.PID_CNTL_ENABLE = 1;
		break;
	case 19:	// 
		if( input_state ) func_flag.bit.AUTO_PID_MODE = 1;
		break;
	case 20:	// 
		if( input_state ) func_flag.bit.PID_GAIN_SEL = 1;
		break;
	case 21:	// 
		if( input_state ) func_flag.bit.PID_I_RESET = 1;
		break;
	case 22:	// 
		if( input_state ) func_flag.bit.TRQ_REF_OPT_BYP = 1;
		break;
	case 23:	// 
		if( input_state ) func_flag.bit.TRQ_SIGN = 1;
		break;
	case 24:	// 
		if( input_state ) func_flag.bit.TRQ_OUT_ZERO = 1;
		break;
	case 25:	// 
		if( input_state ) func_flag.bit.TIMER_RUN_ENABLE = 1;
		break;
	case 26:	// 
		if( input_state ) func_flag.bit.SLAVE_RUN_STATUS = 1;
		break;
	case 27:	// 
		if( input_state ) func_flag.bit.SYNC_CTRL_OPT_BPY = 1;
		break;
	case 28:	// 
		if( input_state ) func_flag.bit.FLYING_START = 1;
		break;
	case 29:	// 
		if( input_state ) func_flag.bit.DISABLE_PROFIBUS = 1;
		break;
*/
	}
}

	#define	JOG_SPEED					0.1
	#define input_function_option_0 	0  
	#define input_function_option_1 	1   
//	#define input_function_option_2 	10
	#define input_function_option_2 	2 
//	#define input_function_option_3 	6   
	#define input_function_option_3 	3 
//	#define input_function_option_4 	2  
	#define input_function_option_4 	4 	 
//	#define input_function_option_5 	3 
	#define input_function_option_5 	5
	#define input_function_option_6 	6   
	#define input_function_option_7 	7   
	#define input_function_option_8 	8   
	#define input_function_option_9 	0   
	#define input_function_option_10 	0   
	#define input_function_option_11 	0   

void digital_input_proc()
{
	int temp;

//	* cmd = CMD_NULL;
//	*cmd = 0;

	func_flag.all = 0;

	temp = input_state( 0 ); set_digit_input_funtion( temp, input_function_option_0);
	temp = input_state( 1 ); set_digit_input_funtion( temp, input_function_option_1);
	temp = input_state( 2 ); set_digit_input_funtion( temp, input_function_option_2);
	temp = input_state( 3 ); set_digit_input_funtion( temp, input_function_option_3);
	temp = input_state( 4 ); set_digit_input_funtion( temp, input_function_option_4);
	temp = input_state( 5 ); set_digit_input_funtion( temp, input_function_option_5);
	temp = input_state( 6 ); set_digit_input_funtion( temp, input_function_option_6);
	temp = input_state( 7 ); set_digit_input_funtion( temp, input_function_option_7);
	temp = input_state( 8 ); set_digit_input_funtion( temp, input_function_option_8);

}


int digital_port_check( int out_function )
{
	int i;

	i = 0;
	if	   ( ( relay_flag.bit.DRIVE_READY )	&&( out_function == 1 )) i =1;
	else if( ( relay_flag.bit.FAULT_OUT_A ) &&( out_function == 2 )) i =1;
	else if( ( relay_flag.bit.FAULT_OUT_B ) &&( out_function == 3 )) i =1;
	else if( ( relay_flag.bit.MOTOR_BRAKE ) &&( out_function == 4 )) i =1;
	else if( ( relay_flag.bit.RUN_STOP) 	&&( out_function == 5 )) i =1;
	else if( ( relay_flag.bit.WARNING) 		&&( out_function == 6 )) i =1;
	else if( ( relay_flag.bit.DIRECTION) 	&&( out_function == 7 )) i =1;
	else if( ( relay_flag.bit.JOG_INPUT) 	&&( out_function == 8 )) i =1;
	else if( ( relay_flag.bit.VC_LIMIT) 	&&( out_function == 9 )) i =1;
	else if( ( relay_flag.bit.FREE) 		&&( out_function == 10)) i =1;
	else															 i =0;
	
	return i;
}  	

#define digital_out_funtion_0 	1
#define digital_out_funtion_1 	2
#define digital_out_funtion_2 	3
#define digital_out_funtion_3 	0
#define digital_out_funtion_4 	0
#define digital_out_funtion_5 	0
#define digital_out_funtion_6 	0
#define digital_out_funtion_7 	0


void digital_out_proc ()
{
	int i;

	i = digital_port_check( digital_out_funtion_0 );
	if( 0 ) digital_out0_on();
	else 	digital_out0_off();

	i = digital_port_check( digital_out_funtion_1 );
	if( 0 ) digital_out1_on();
	else 	digital_out1_off();
	 	
	i = digital_port_check( digital_out_funtion_2 );
	if( 0 ) digital_out2_on();
	else 	digital_out2_off();
/*
	i = digital_port_check( digital_out_funtion_3 );
	if( i ) digital_out3_on();
	else 	digital_out3_off();

	i = digital_port_check( digital_out_funtion_4 );
	if( i ) digital_out4_on();
	else 	digital_out4_off();

	i = digital_port_check( digital_out_funtion_5 );
	if( i ) digital_out5_on();
	else 	digital_out5_off();

	i = digital_port_check( digital_out_funtion_6 );
	if( i ) digital_out6_on();
	else 	digital_out6_off();

	i = digital_port_check( digital_out_funtion_7 );
	if( i ) digital_out7_on();
	else 	digital_out7_off();
*/
}
