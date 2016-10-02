
#include	<All_Header.h>
#include	<All_Extern_Variables.h>

#pragma CODE_SECTION(State_Management, "ramfuncs");
void State_Management()
{
	double Temp;

	Get_Command( & Command, & Init_reference );

	if ( (Flag.Fault1.all & (~Flag.Fault_Neglect1.all)) != 0x0000 )	State_index= STATE_FAULT;
	if ( (Flag.Fault2.all & (~Flag.Fault_Neglect2.all)) != 0x0000 )	State_index= STATE_FAULT;
	if ( (Flag.Fault3.all & (~Flag.Fault_Neglect3.all)) != 0x0000 )	State_index= STATE_FAULT;

	switch( State_index )
	{
		case STATE_POWER_ON:	State_index= STATE_INITIAL_DRIVE_CALIBRATION; Flag.Monitoring.bit.RUN_STOP_STATUS= 0; break;
		case STATE_INITIAL_DRIVE_CALIBRATION:	if (Flag.Monitoring.bit.INIT_DRIVE_CAL)	State_index= STATE_STOP;	break;
		case STATE_DRIVE_CALIBRATION:	
			if ( (!OP.Run_stop.bit.DRIVE_CALIBRATION)&&(Flag.Monitoring.bit.DRIVE_CAL) )
				State_index= STATE_STOP;	
			break;
		case STATE_STOP:			
			// (Break On,Off)  의 상태 검사 Code 추가할것 
			// 실행은 Interrupt Routine 에서 처리 
			if ((Command == CMD_RUN)&&(Init_reference!=0))
			{
				// if ( (Flag.ETC.bit.PWM_ON) && (BREAK_ON) )
				if (Flag.Monitoring.bit.PWM_ON)
				{
					if (Flag.Monitoring.bit.BREAK_CONTROL)
							State_index= STATE_BREAKING;
					else	State_index= STATE_ACCELERATING;
				}
				Flag.Monitoring.bit.RUN_STOP_STATUS= 1;
			}
			else if (Command == CMD_STOP)
			{
				// if ( (!Flag.ETC.bit.PWM_ON) && (BREAK_OFF) )
				if (!Flag.Monitoring.bit.PWM_ON)
				{
					State_index= STATE_READY;
					Flag.Monitoring.bit.RUN_STOP_STATUS= 0;
				}
			}
			break;

		case STATE_READY:

			if (Command == CMD_RUN)
			{
				State_index= STATE_STOP;
				Flag.Monitoring.bit.RUN_STOP_STATUS= 1;
			}
			else if (OP.Run_stop.bit.DRIVE_CALIBRATION)
			{
				State_index= STATE_DRIVE_CALIBRATION;
				Flag.Monitoring.bit.RUN_STOP_STATUS= 1;
			}
			else if (OP.Run_stop.bit.MOTOR_TUNING)
			{
			 	State_index= STATE_TUNING;
				Auto_tuning_index= INITIALIZATION;
				Flag.Monitoring.bit.RUN_STOP_STATUS= 1;
			}
			break; 
			

		case STATE_BREAKING:
			if ( (Command == CMD_RUN)&&(Break_index>=3) )
				State_index= STATE_ACCELERATING;
			else if (Command == CMD_STOP)
			{
				Ref_old= Final_reference;
				State_index= STATE_DECELERATING;
			}
			break;

		// STATE_RUNING 의 판단은 입력 속도의 정상상태로 결정한다
		// 실제 속도로 판단할 경우 수정이 필요하다.  
		case STATE_ACCELERATING:
			Temp= fabs(Init_reference - Final_reference);
			if (Flag.Monitoring.bit.ACC== 0)
			{
				if (Temp<= 0.001) 
					State_index= STATE_RUNNING;
				else if (Flag.Monitoring.bit.DEC)
					State_index= STATE_DECELERATING;
			}
			break; 
		
		case STATE_DECELERATING:
			if (Final_reference == Init_reference)
			{
				if (Init_reference == 0)  State_index= STATE_STOP;
				else if (Command == CMD_RUN) State_index= STATE_RUNNING;
			}
			else if (Flag.Monitoring.bit.ACC) State_index= STATE_ACCELERATING;
			break; 
		
		case STATE_RUNNING:
			if ( (Final_reference == Init_reference)&&(Init_reference == 0.) )
				State_index= STATE_STOP;
			else
			{
				if (Flag.Monitoring.bit.ACC) State_index= STATE_ACCELERATING;
				else if (Flag.Monitoring.bit.DEC) State_index= STATE_DECELERATING;
			}
			break;

		case STATE_TUNING:
			if ( (!OP.Run_stop.bit.MOTOR_TUNING)&&(Auto_tuning_index== COMPLETE) ) 
				State_index= STATE_STOP;
			break;  

		case STATE_FAULT:
			Flag.Monitoring.bit.RUN_STOP_STATUS= 0; 
			if  (Flag.Monitoring.bit.FAULT_RESET_COMPLETE)
				State_index= STATE_STOP;
			break;  

		default:
			break;
	}

	DO_Processing();


}		

//===========================================================================
// End of file.
//===========================================================================

