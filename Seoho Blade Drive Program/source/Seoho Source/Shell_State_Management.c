
#include	<All_Header.h>
#include	<All_Extern_Variables.h>

#pragma CODE_SECTION(State_Management, "ramfuncs");
void State_Management()
{

	Get_Command( & Command, & Init_reference );

	if ( (Flag.Fault1.all & (~Flag.Fault_Neglect1.all)) != 0x0000 )	State_Index= STATE_FAULT;
	if ( (Flag.Fault2.all & (~Flag.Fault_Neglect2.all)) != 0x0000 )	State_Index= STATE_FAULT;

	switch( State_Index )
	{
		case STATE_POWER_ON:	State_Index= STATE_CALIBRATION; Flag.Monitoring.bit.RUN_STOP_STATUS= 0; break;
		case STATE_CALIBRATION:	if (Flag.Monitoring.bit.DRIVE_CAL)	State_Index= STATE_STOP;	break;
		case STATE_STOP:			
			// (Break On,Off)  의 상태 검사 Code 추가할것 
			// 실행은 Interrupt Routine 에서 처리 
			if ((Command == CMD_RUN)&&(Init_reference!=0))
			{
				// if ( (Flag.ETC.bit.PWM_ON) && (BREAK_ON) )
				if (Flag.Monitoring.bit.PWM_ON) State_Index= STATE_ACCELERATING;
				Flag.Monitoring.bit.RUN_STOP_STATUS= 1;
			}
			else if (Command == CMD_STOP)
			{
				// if ( (!Flag.ETC.bit.PWM_ON) && (BREAK_OFF) )
				if (!Flag.Monitoring.bit.PWM_ON)
				{
					State_Index= STATE_READY;
					Flag.Monitoring.bit.RUN_STOP_STATUS= 0;
				}
			}
			break;

		case STATE_READY:
			if (Command == CMD_RUN)
			{
				State_Index= STATE_STOP;
				Flag.Monitoring.bit.RUN_STOP_STATUS= 1;
			}
			else if (OP.Run_stop.bit.AUTO_TUNING)
			{
			 	State_Index= STATE_TUNING;
				Flag.Monitoring.bit.RUN_STOP_STATUS= 1;
			}
			break; 

		// STATE_RUNING 의 판단은 입력 속도의 정상상태로 결정한다
		// 실제 속도로 판단할 경우 수정이 필요하다.  
		case STATE_ACCELERATING: 
			if (Final_reference == Init_reference) State_Index= STATE_RUNNING;
			break; 
		
		case STATE_DECELERATING:
			if (Final_reference == Init_reference)
			{
				if (Init_reference == 0) State_Index= STATE_STOP;
				else if (Command == CMD_RUN) State_Index= STATE_RUNNING;
			}
			else if (Flag.Monitoring.bit.ACC) State_Index= STATE_ACCELERATING;
			break; 
		
		case STATE_RUNNING:
			if ( (Final_reference == Init_reference)&&(Init_reference == 0.) )
				State_Index= STATE_STOP;
			else
			{
				if (Flag.Monitoring.bit.ACC) State_Index= STATE_ACCELERATING;
				else if (Flag.Monitoring.bit.DEC) State_Index= STATE_DECELERATING;
			}
			break;

		case STATE_TUNING:
			if ( (!OP.Run_stop.bit.AUTO_TUNING)&&(Auto_tuning_index==5) ) State_Index= STATE_STOP;
			break;  

		case STATE_FAULT:
			Flag.Monitoring.bit.RUN_STOP_STATUS= 0; 
			if  (Flag.Monitoring.bit.FAULT_RESET_COMPLETE)
				State_Index= STATE_STOP;
			break;  

		default:
			break;
	}


}		

//===========================================================================
// End of file.
//===========================================================================

