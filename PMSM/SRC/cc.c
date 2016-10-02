#define _CC_


#include "main_def.h"
#include "math.h"

float Ilim = 5.;
unsigned int delay_cnt = 0;
void current_controller()
{
	cc_smp = !cc_smp;		// cc flag

	if(sc_cnt == 10)	{	sc_cnt = 0; speed_controller();	}
	sc_cnt++;

	adc.conv_val[0] = (int)(AdcRegs.ADCRESULT0 >> 4) & 0x0FFF;	// Ias
	adc.conv_val[1] = (int)(AdcRegs.ADCRESULT1 >> 4) & 0x0FFF;	// Ibs
	adc.conv_val[3] = (int)(AdcRegs.ADCRESULT3 >> 4) & 0x0FFF;	// Vdc

	adc.conv_val[4] = (int)(AdcRegs.ADCRESULT4 >> 4); // Analog Input I1
	adc.conv_val[5] = (int)(AdcRegs.ADCRESULT5 >> 4); // Analog Input V1
	adc.conv_val[6] = (int)(AdcRegs.ADCRESULT6 >> 4); // Analog Input I2
	adc.conv_val[7] = (int)(AdcRegs.ADCRESULT7 >> 4); // Analog Input V2
	adc.conv_val[15]= (int)(AdcRegs.ADCRESULT15>> 4); // Voltage Range

	adc.tmp[0] = (float)adc.conv_val[0] - (float)Ias_offset;
	adc.tmp[1] = (float)adc.conv_val[1] - (float)Ibs_offset;
	adc.tmp[3] = (float)adc.conv_val[3];						// Vdc
//	adc.tmp[4] = (float)adc.conv_val[4];						// AIN.I1
	adc.tmp[5] = (float)adc.conv_val[5] - 2262.;				// AIN.V1
//	adc.tmp[5] = (float)adc.conv_val[5] - (float)AI_offset;		// AICMD
//	adc.tmp[6] = (float)adc.conv_val[6];						// AIN.I2
//	adc.tmp[7] = (float)adc.conv_val[7] - 2262.;				// AIN.V2

//	AINI1 = adc.scale[4] * adc.tmp[4];
	AICMD = adc.scale[5] * adc.tmp[5];
//	AINI2 = adc.scale[6] * adc.tmp[6];
//	AINV2 = adc.scale[7] * adc.tmp[7];
//	AICMD = AINV1;		// bayasaa 2010.05.19
	adc.tmp[15] = (float)adc.conv_val[15];

	Ias = (adc.scale[0] * adc.tmp[0]);
	Ibs = (adc.scale[1] * adc.tmp[1]);
	Ics = -(Ias + Ibs);

	Vdc_measured = (adc.scale[3]*adc.tmp[3]);
//	Vdc_measured = 527.;
	Voltage_Range = adc.scale[15] * adc.tmp[15];

	Idss = Ias;
	Iqss = (Ibs - Ics)*INV_SQRT3;

	if (flag.control == 1) 
	{
		if(cc_cnt == 10) { cc_cnt = 0; flag.FOC = 1;}
		cc_cnt++;
	}

	if (run_Sensorless==1)
	{

		// Configurate slew_rate of speed estimation
		ref_slew_up = 1000. * Tsamp;	// 속도 증가 기울기 = 1000 * Tsamp * Fsw = 1000rpm/sec
		ref_slew_dn = 1000. * Tsamp;

		gain_tr = 0.0001, gain_wr = 0.08;
		Kwr = gain_wr;
		Ktr = gain_tr;

		if(ref_control_mode)
		{
			if		(Wrpm_ref_user > (Wrpm_ref + ref_slew_up))	{	Wrpm_ref = Wrpm_ref + ref_slew_up; }
			else if	(Wrpm_ref_user < (Wrpm_ref - ref_slew_dn))	{	Wrpm_ref = Wrpm_ref - ref_slew_dn; }
			else 	 Wrpm_ref = Wrpm_ref_user;
		}
		Wrm_ref = Wrpm_ref * 0.1047197;	//2*PI/60;
		Wr_ref = Wrm_ref;	// * PP;	2극 모터에서 회전기와 전기자의 주파수가 같다. dbsgln
		
		switch(sensorless_mode)
		{
			case 0 :
				Idse_ref = Idse_ref_max;
				if(thetar_openloop > 6.283185 )		//(2. * PI))
				{
					sensorless_mode = 1;
				}
				else
				{
					thetar_openloop = thetar_openloop + Wr_ref*Tsamp;
				}
				Wrm_hat = Wr_hat;
				Wrpm_hat = Wrm_hat * 9.549298;
				cos_theta = cos(thetar_openloop);
				sin_theta = sin(thetar_openloop);
				thetar_hat1 = BOUND_PI(thetar_openloop + 1.5 * Wr_ref*Tsamp);
				cos_theta1 = cos(thetar_hat1);
				sin_theta1 = sin(thetar_hat1);
				break;
			case 1 :
				
				Idse_ref = Idse_ref_max;
				thetar_openloop = 0;
				cos_theta = 1.0	;			//cos(thetar_openloop);
				sin_theta = 0.0 ;			//sin(thetar_openloop);
				thetar_hat1 = BOUND_PI(thetar_openloop + 1.5 * Wr_ref*Tsamp);
				cos_theta1 = cos(thetar_hat1);
				sin_theta1 = sin(thetar_hat1);
				tmp_cnt++;
				if(tmp_cnt >= 30000)
				{
					tmp_cnt = 0;
					sensorless_mode = 2;
				}
				Wrm_hat = Wr_hat;
				Wrpm_hat = Wrm_hat * 9.549298;
		  		
				break;
			case 2 :
				
				if 		(Wrpm_ref < 7000.) Idse_ref = Idse_ref_max;
				else 
				{
					Idse_ref_max = Idse_ref_max - Tsamp * 8.;	// 10
					if (Idse_ref_max <= 25.) Idse_ref_max = 25.;	// 20
					Idse_ref = Idse_ref_max;
				}

				Wrpm_ref = Wrpm_ref + 5000. * Tsamp; // 2500 3000, 4000
				thetar_openloop = BOUND_PI(thetar_openloop + Wr_ref*Tsamp);
				cos_theta = cos(thetar_openloop);
				sin_theta = sin(thetar_openloop);
				thetar_hat1 = BOUND_PI(thetar_openloop + 1.5 * Wr_ref*Tsamp);
				cos_theta1 = cos(thetar_hat1);
				sin_theta1 = sin(thetar_hat1);
				Wr_hat = Wr_ref - Kwr*(Iqse - IqcM);
				thetar_hat = BOUND_PI(thetar_openloop + Ktr*(Idse - IdcM) + Wr_ref*Tsamp);
				IqcM = (Tsamp/Lds*(-Lds*Wr_ref*Idse - Rstator*Iqse + Vqse_ref - Wr_ref*K_back_EMF))+Iqse;
				IdcM = (Tsamp/Lds*(Lds*Wr_ref*Iqse - Rstator*Idse + Vdse_ref)) + Idse;
				Wrm_hat = Wr_hat;
				Wrpm_hat = Wrm_hat * 9.549298;
				tmp_cnt = 0;
				break;
			case 3 :
				break;
			case 4:
				if 	(Wrpm_ref < 24000.) Idse_ref = 12.;
				else 
				{
					Idse_ref_sensorless = Idse_ref_sensorless - Tsamp * 0.5;	    // 기준치를 1초 당 1A 씩 감소 
					if (Idse_ref_sensorless <= 5.) Idse_ref_sensorless = 5.;	// 센서리스 운전의 최소 전류 기준치 
					Idse_ref = Idse_ref_sensorless;
				}
				Idse_ref = Idse_ref_sensorless;
				Wr_hat = Wr_hat - Kwr*(Iqse - IqcM);
				thetar_hat = BOUND_PI(thetar_hat + Ktr*(Idse - IdcM) + Wr_hat*Tsamp);
				IqcM = (Tsamp/Lds*(-Lds*Wr_hat*Idse - Rstator*Iqse + Vqse_ref - Wr_hat*K_back_EMF))+Iqse;
				IdcM = (Tsamp/Lds*(Lds*Wr_hat*Iqse - Rstator*Idse + Vdse_ref)) + Idse;
				Wrm_hat = Wr_hat;
				Wrpm_hat = Wrm_hat * 9.549298;
				Err_thetar = thetar_openloop - thetar_hat;
				cos_theta = cos(thetar_hat);
				sin_theta = sin(thetar_hat);
				Err_IdcM = Idse - IdcM;
				thetar_hat1 = BOUND_PI(thetar_hat + 1.5 * Wr_hat*Tsamp);
				cos_theta1 = cos(thetar_hat1);
				sin_theta1 = sin(thetar_hat1);
				
				// 2010_09_02
				ref_control_mode = 1;
				if(flag_speed_cmd == 1) Wrpm_ref_user = Wrpm_set_user;
				else 					Wrpm_ref_user = Base_spd;			
			//	if(tmp_cnt > 30000)
			//	{
			//		Wrpm_ref_user = Base_spd;
			//		if(flag_speed_cmd == 1) Wrpm_ref_user = Wrpm_set_user;
			//		ref_control_mode = 1;
			//	}
				if (Wrpm_set_user == 0.)
				{
					Wrpm_ref_user = 0;
					Idse_ref = 3.0;
				}
				if(Wrpm_ref < 10.0)
				{
					Idse_ref = 0.0;
					Wrpm_ref = 0.0;
					tmp_cnt = 0;
					ref_control_mode = 0;
					sensorless_mode = 100;
					flag.FOC = 0;
					flag.control = 0;
				}
//				tmp_cnt++; //유이를 찾아야 함...
				break;
			case 5:
				break;
			case 100 :
				thetar_openloop = 0.;
				Vdse_ref_integ = 0.;
				Vqse_ref_integ = 0.;
				IqcM = 0.;
				IdcM = 0.;
				thetar_hat = 0.;
				run_Sensorless = 0.;
				Wr_hat = 0.;
				Wr_ref = 0.;
				Wrm_err = 0.;
				Wrm_err_integ = 0.;
				Wrm_hat = 0.;
				Wrm_ref = 0.;
				Wrpm_hat = 0.;
				Wrpm_ref = 0.;
				thetar_hat = 0.;
				Vdse_ref_fb = 0.;
				Vqse_ref_fb = 0.;
				IdcM = 0.;
				IqcM = 0.;
				break;							
			case 99 :
				default :
				break;														
			}
	}
	/* Idqss to Idqse Conversion */
	Idse =  cos_theta*Idss + sin_theta*Iqss;
	Iqse = -sin_theta*Idss + cos_theta*Iqss;

	if(flag.FOC)
	{
	/* Synchronous PI Controller with feedforward compensation */
		Err_Idse = Idse_ref - Idse;
		Vdse_anti = Vdse_ref - Vdse_sat;
		Vdse_ref_integ += Ki_ccT*(Err_Idse - Ka_cc * Vdse_anti);
//		Vdse_ref_integ += Ki_ccT*Err_Idse;	// 2010.06.21
		Vdse_ref_integ = (Vdse_ref_integ > Vdc_measured*0.5) ? Vdc_measured*0.5 :
						 (Vdse_ref_integ < -Vdc_measured*0.5) ? -Vdc_measured*0.5 : Vdse_ref_integ;
		Vdse_ref_fb = Vdse_ref_integ + alpha_cc*Kp_cc*Err_Idse - (1 - alpha_cc)*Kp_cc*Idse;
		Err_Iqse = Iqse_ref - Iqse;
		Vqse_anti = Vqse_ref - Vqse_sat;
		Vqse_ref_integ += Ki_ccT*(Err_Iqse - Ka_cc * Vqse_anti);
//		Vqse_ref_integ += Ki_ccT*Err_Iqse;	// 2010.06.21
		Vqse_ref_integ = (Vqse_ref_integ > Vdc_measured*0.5) ? Vdc_measured * 0.5 :
						 (Vqse_ref_integ < -Vdc_measured*0.5) ? -Vdc_measured * 0.5 : Vqse_ref_integ;
		Vqse_ref_fb = Vqse_ref_integ + alpha_cc*Kp_cc*Err_Iqse - (1 - alpha_cc) * Kp_cc * Iqse;

	/* Feedforward */
		Vqse_ref_ff = K_back_EMF * Wr_hat + Wr_hat * Lds * Idse;
		Vqse_ref_ffa = K_back_EMF * Wr_hat + Wr_hat * Lds * Idse;
		Vdse_ref_ff = 0.0;
		Vdse_ref_ffa = - Wr_hat * Lds * Iqse; // 5.22
	}
	else
	{
		Vdse_ref_fb = 0.0;		Vdse_ref_ff = 0.0;
		Vqse_ref_fb = 0.0;		Vqse_ref_ff = 0.0;
	}

	// Voltage Reference 
		Vdse_ref = Vdse_ref_fb + Vdse_ref_ff;
		Vqse_ref = Vqse_ref_fb + Vqse_ref_ff;

	// Vdqse to Vdqss Conversion 
		Vdss_ref = cos_theta1*Vdse_ref - sin_theta1*Vqse_ref;
		Vqss_ref = sin_theta1*Vdse_ref + cos_theta1*Vqse_ref;

	// Vdqss to Vabcs Conversion 
		Vas_ref = Vdss_ref;
		Vbs_ref = -0.5*(Vdss_ref - SQRT3*Vqss_ref);
		Vcs_ref = -(Vas_ref + Vbs_ref);
		INV_Vdc = 1.6/Vdc_measured;

// Space Vector PWM based on offset voltage method
	if(Vas_ref > Vbs_ref)
	{
		Vmax = Vas_ref;
		Vmin = Vbs_ref;
	}
	else
	{
		Vmax = Vbs_ref;
		Vmin = Vas_ref;
	}
	if(Vcs_ref > Vmax)	Vmax = Vcs_ref;	
	if(Vcs_ref < Vmin)	Vmin = Vcs_ref;


//	Vsn = (Vsn_max + Vsn_min)*0.5;
	Vsn = - (Vmax + Vmin)*0.5;
	// Select OVM mode
	switch(mode.OVM)
	{
		case 1 : // Absolute Value
			cal_tmp1 = 0.5*Vdc_measured;
			Van_ref = (Van_ref > cal_tmp1) ? cal_tmp1 : ((Van_ref < -cal_tmp1) ? -cal_tmp1 : Van_ref);
			Vbn_ref = (Vbn_ref > cal_tmp1) ? cal_tmp1 : ((Vbn_ref < -cal_tmp1) ? -cal_tmp1 : Vbn_ref);
			Vcn_ref = (Vcn_ref > cal_tmp1) ? cal_tmp1 : ((Vcn_ref < -cal_tmp1) ? -cal_tmp1 : Vcn_ref);
			break;
		case 0 :		// Preserve Angle Information
		default :
			cal_tmp0 = Vmax - Vmin;
			if(fabs(cal_tmp0) > Vdc_measured)
			{
				cal_tmp1 = Vdc_measured/cal_tmp0;
				Van_ref *= cal_tmp1;
				Vbn_ref *= cal_tmp1;
				Vcn_ref *= cal_tmp1;
			}
			break;
	}

	/* Pole Voltage Reference */

	Van_ref = Vas_ref + Vsn;
	Vbn_ref = Vbs_ref + Vsn;
	Vcn_ref = Vcs_ref + Vsn;

	if(flag.FOC)
	{
		flag.PWM_CTR = 1;
	}

	switch(flag.PWM_CTR)
	{
		case 1 :	// PWM control by controller
			cal_tmp0 = (float)pwm_g1.phase_duty_half_scaled*Van_ref*INV_Vdc;
			cal_tmp0 = (cal_tmp0 > ((float)pwm_g1.phase_duty_half_scaled)) ? (float)pwm_g1.phase_duty_half_scaled :
						(cal_tmp0 < -((float)pwm_g1.phase_duty_half_scaled)) ? -(float)pwm_g1.phase_duty_half_scaled :
						 cal_tmp0;
			pwm_g1.phase_a_duty_scaled = (int)cal_tmp0;
			cal_tmp0 = (float)pwm_g1.phase_duty_half_scaled*Vbn_ref*INV_Vdc;
			cal_tmp0 = (cal_tmp0 > ((float)pwm_g1.phase_duty_half_scaled)) ? (float)pwm_g1.phase_duty_half_scaled :
						(cal_tmp0 < -((float)pwm_g1.phase_duty_half_scaled)) ? -(float)pwm_g1.phase_duty_half_scaled :
						 cal_tmp0;
			pwm_g1.phase_b_duty_scaled = (int)cal_tmp0;
	
			cal_tmp0 = (float)pwm_g1.phase_duty_half_scaled*Vcn_ref*INV_Vdc;
			cal_tmp0 = (cal_tmp0 > ((float)pwm_g1.phase_duty_half_scaled)) ? (float)pwm_g1.phase_duty_half_scaled :
						(cal_tmp0 < -((float)pwm_g1.phase_duty_half_scaled)) ? -(float)pwm_g1.phase_duty_half_scaled :
						 cal_tmp0;
			pwm_g1.phase_c_duty_scaled = (int)cal_tmp0;
			break;
		case 2 :
			flag.FOC = 0;
			break;
		case 0 :
			flag.FOC = 0;
			pwm_g1.phase_a_duty_scaled = 0;
			pwm_g1.phase_b_duty_scaled = 0;
			pwm_g1.phase_c_duty_scaled = 0;
		default :
			break;

	}

	/* Dead time Compensation */
/*		if((Ias < Ilim) && (Ias > - Ilim)) pwm_g1.phase_a_duty_scaled += (int)(Ias /Ilim * 75.);
   else if(Ias > Ilim) 	pwm_g1.phase_a_duty_scaled += (int)(75);
   else if(Ias < -Ilim) pwm_g1.phase_a_duty_scaled -= (int)(75);

		if((Ibs < Ilim) && (Ibs > - Ilim)) pwm_g1.phase_b_duty_scaled += (int)(Ibs /Ilim * 75.);
   else if(Ibs > Ilim) 	pwm_g1.phase_b_duty_scaled += (int)(75);
   else if(Ibs < -Ilim)	pwm_g1.phase_b_duty_scaled -= (int)(75);

		if((Ics < Ilim) && (Ics > - Ilim)) pwm_g1.phase_c_duty_scaled += (int)(Ics /Ilim * 75.);
   else if(Ics > Ilim) 	pwm_g1.phase_c_duty_scaled += (int)(75);
   else if(Ics < -Ilim)	pwm_g1.phase_c_duty_scaled -= (int)(75); 
*/

	EPwm1Regs.CMPA.half.CMPA = pwm_g1.phase_duty_half_scaled + pwm_g1.phase_a_duty_scaled;
	EPwm2Regs.CMPA.half.CMPA = pwm_g1.phase_duty_half_scaled + pwm_g1.phase_b_duty_scaled;
	EPwm3Regs.CMPA.half.CMPA = pwm_g1.phase_duty_half_scaled + pwm_g1.phase_c_duty_scaled;

	// Recalculation of Voltage
	Vdss_sat = (2.0*Van_ref - Vbn_ref - Vcn_ref)/3.0;
	Vqss_sat = INV_SQRT3*(Vbn_ref - Vcn_ref);

	Vas_ref = Vdss_sat;
	Vbs_ref = -0.5*Vdss_sat + 0.5*SQRT3*Vqss_sat;
	Vcs_ref = -(Vas_ref + Vbs_ref);

	// Limited voltages due to the inverter saturation (for anti-windup)
	Vdse_sat =  cos_theta*Vdss_sat + sin_theta*Vqss_sat;
	Vqse_sat = -sin_theta*Vdss_sat + cos_theta*Vqss_sat;

	ms_cnt++;

	if(ms_cnt >= 65535) ms_cnt = 0 ;

	//=========================================================================

	if(ms_cnt > 3) // 3
	{
		ms_cnt = 0;

		if (TXD_StackReadPtr != TXD_StackWritePtr)
		{
			if(ScicRegs.SCICTL2.bit.TXRDY) 				// 송신버퍼가 비어 있음
			{
				ScicRegs.SCITXBUF = TXD_Stack[TXD_StackReadPtr++] ;
				if (TXD_StackReadPtr >= TXD_STACK_LENGTH)
				{
					TXD_StackReadPtr = 0 ;
				}
			}
		}
	}
	//=========================================================================

/*	Vmag_delta_avg = (Vmag_delta + Vmag_delta1+Vmag_delta2+Vmag_delta3+Vmag_delta4)/5.;
	Vmag_delta4  = Vmag_delta3;
	Vmag_delta3  = Vmag_delta2;
	Vmag_delta2  = Vmag_delta1;
	Vmag_delta1  = Vmag_delta; */
}

