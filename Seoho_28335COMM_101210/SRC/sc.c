#define _SC_
#include "main_def.h"

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
//			Base_spd = 5000.;
				if(Wrpm_ref > Base_spd)	
				{	
					Wrpm_ref = Base_spd;	
					sensorless_mode = 4;
	//				Idse_ref = Idse_ref_sensorless;
//				if(Wrpm_hat < 7000)
		//		{
		//			Idse_ref = Idse_ref_max;
		//		}
		//		else Idse_ref - 0.1;
		//		if (Idse_ref < 5) Idse_ref = 5.;
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


