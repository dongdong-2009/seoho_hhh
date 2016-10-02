#define _SC_
#include "main_def.h"

void op_master()
{
flagcontrol = flag.control;
flagFOC = flag.FOC;
flagGT1_ENA = flag.GT1_ENA;

	switch(flag.control)
	{
		case 1 :
			flag.GT1_ENA = 1;
			if(op_cnt == 100)
			{
				op_cnt = 0;
			}
			else
			{
				op_cnt++;
				flag.control = 0.;
			}
			break;
		case 2 :		
			break;
		case 0 :
		default :
			flag.GT1_ENA = 0;
			flag.PWM_CTR = 0;
			break;
	}
}



/* 1msec routine */
void speed_controller()
{
	if(run_Sensorless)
	{
		switch(sensorless_mode)
		{
			case 0 :
				Wrpm_ref =30;
				break;
			case 1 :
				Wrpm_ref = 0;
				break;
			case 2 : 
				if(Wrpm_ref > Wrpm_base_speed)	
				{	
					Wrpm_ref = Wrpm_base_speed;	
//					DRIVE_ENABLE = 0;	// for test Kemf
					sensorless_mode = 4;
					Idse_ref = Idse_ref_sensorless;
				}										
				break;
			case 3 :
				break;
			case 4 :
				Wrm_err = (Wrpm_ref * 2.0 * PI/60.) - Wrm_hat;
				Wrm_err_integ += Wrm_err * Ki_scT;
				Wrm_err_integ = (Wrm_err_integ > Is_max) ? Is_max :
								(Wrm_err_integ < -Is_max) ? -Is_max : Wrm_err_integ;
				Iqse_ref = (Kp_sc*Wrm_err + Wrm_err_integ);	
				
				Iqse_ref = (Iqse_ref > Is_max) ? Is_max :
								(Iqse_ref < -Is_max) ? -Is_max : Iqse_ref;
						
				break;					
			case 99 :
			default :
				break;																
		}
	}		
	else
	{
		Idse = 0;
		Iqse = 0;
	}
}


