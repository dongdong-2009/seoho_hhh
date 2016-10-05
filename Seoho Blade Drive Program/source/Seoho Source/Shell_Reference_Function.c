#include	<All_Header.h>
#include	<All_Extern_Variables.h>
	
#pragma CODE_SECTION(Reference_Function, "ramfuncs"); 
void Reference_Function()
{

	double Set_ref, Out_ref, fabs_Set_ref, fabs_Out_ref, Temp1, Temp2; 
	static double Ref_old=0., Accel_value= 0., Decel_value= 0., Accel_delta= 0., Decel_delta= 0.;
	static double Curve_weight= 0., Accel_value_curve= 0., Decel_value_curve= 0.;
	static long Ref_counter= 0;
	static int Flag_inversion_run= 0, Flag_curve_complete= 0;


		
	Set_ref= Init_reference;
	if ((!Driver_ON)||(Command==CMD_STOP)) Set_ref= 0.;
	Out_ref= Final_reference;
	fabs_Set_ref= fabs(Set_ref);
	fabs_Out_ref= fabs(Out_ref);
		
	if (Set_ref!=Ref_old)
	{
		if ((Set_ref*Out_ref)<0)
		{	Accel_delta= Set_ref;			Decel_delta= Out_ref;	Flag_inversion_run= 1;			}
		else if (fabs_Set_ref>fabs_Out_ref)
		{	Accel_delta= Set_ref-Out_ref;	Decel_delta= 0.;					}
		else if (fabs_Set_ref<fabs_Out_ref)		
		{	Accel_delta= 0.;				Decel_delta= Out_ref-Set_ref;		}
		else
		{	Accel_delta= 0.;				Decel_delta= 0.;					}
		
//		Accel_value= Accel_delta/Accel_time*Tsamp;
//		Decel_value= Decel_delta/Decel_time*Tsamp;
		Ref_counter= 0;

		if (P.G03.P51_S_curve_enable== 1)
		{
			Curve_weight= (double)P.G03.P52_S_curve_weight_x1000*1.e-3;
			Accel_value_curve= (Accel_delta/(Accel_time*(1.- Curve_weight/2.)))*Tsamp;
			Decel_value_curve= (Decel_delta/(Decel_time*(1.- Curve_weight/2.)))*Tsamp;
			Accel_value= Decel_value= 0.;
		}
		else
		{
			Accel_value= Accel_delta/Accel_time*Tsamp;
			Decel_value= Decel_delta/Decel_time*Tsamp;
		}

		Ref_counter= 0;
	}
	Ref_old= Set_ref;
	Ref_counter++;
	if (Ref_counter>=300000)	Ref_counter= 400000;
	
	// S-Curve
	if (P.G03.P51_S_curve_enable== 1)
	{
		Temp1= (double)Ref_counter*Tsamp;
					
		if (Decel_delta!= 0.)
		{
			Temp2= Decel_time*Curve_weight/2.;
			
			if (Temp1>Decel_time)	
			{
				Decel_value= 0.;
//				Flag_curve_complete= 1;
				if (Flag_inversion_run== 1)
				{
					Decel_delta= 0.;
					Ref_counter= 0;
				}
				else Flag_curve_complete= 1;
			}
			else if ( Temp1<Temp2 )
					Decel_value+= (Decel_value_curve/Temp2)*Tsamp;
			else if ( Temp1>(Decel_time-Temp2) )
					Decel_value-= (Decel_value_curve/Temp2)*Tsamp;
		}
		else if (Accel_delta!= 0.)
		{
			Temp2= Accel_time*Curve_weight/2.;
			
			if (Temp1>Accel_time)
			{
				Accel_value= 0.;
				Flag_curve_complete= 1;
			}
			else if ( Temp1<Temp2 )
					Accel_value+= (Accel_value_curve/Temp2)*Tsamp;
			else if ( (Temp1)>(Accel_time-Temp2) )
					Accel_value-= (Accel_value_curve/Temp2)*Tsamp;
		}
			

	}

	if ( (fabs(Set_ref-Out_ref)<=fabs(Decel_value))||(fabs(Set_ref-Out_ref)<=fabs(Accel_value))
		||(Flag_curve_complete== 1) )
	{	
		Out_ref= Set_ref;
		Flag_curve_complete= 0;
//		Accel_value= Decel_value= 0.;
		Flag_inversion_run= 0;
		Flag.Monitoring.bit.ACC= 0.;
		Flag.Monitoring.bit.DEC= 0.;
	}	
	else if ((Set_ref*Out_ref)<0)
	{	
		Out_ref-= Decel_value;
		Flag.Monitoring.bit.ACC= 0.;
		Flag.Monitoring.bit.DEC= 1.;
	}
	else if (fabs_Set_ref>fabs_Out_ref)
	{
		Out_ref+= Accel_value;
		Flag.Monitoring.bit.ACC= 1.;
		Flag.Monitoring.bit.DEC= 0.;
	} 
	else if (fabs_Set_ref<fabs_Out_ref)
	{
		Out_ref-= Decel_value;
		Flag.Monitoring.bit.ACC= 0.;
		Flag.Monitoring.bit.DEC= 1.;
	}
	else 
	{
		Out_ref= Set_ref;
		Accel_value= Decel_value= 0.;
		Flag_inversion_run= 0;
		Flag.Monitoring.bit.ACC= 0.;
		Flag.Monitoring.bit.DEC= 0.;
	}


	if ( (fabs_Out_ref<=0.0005)&&(Command==CMD_STOP) ) 
	{ 
		Accel_value= Decel_value= Out_ref= 0.; 
		Flag_inversion_run= 0;
		Flag_curve_complete= 0;
	}
	Final_reference= BOUND(Out_ref,1,(-1));
	if (Final_reference< 0.)	Flag.Monitoring.bit.DIR_STATUS= 1;
	else	Flag.Monitoring.bit.DIR_STATUS= 0;
}	







//----------------------------------
// End of File
//----------------------------------

