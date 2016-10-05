#include	<All_Header.h>
#include	<All_Extern_Variables.h>

// Wrpm_mt Wrpm_est Wrm_det -> ���� Ȯ�� 

void  EQEP_Initialization(void)
{										// 0~59999
	EQep1Regs.QUPRD=Period_SD-1;	// Unit Timer 
	EQep1Regs.QDECCTL.bit.QSRC=00;		// QEP quadrature count mode
		
	EQep1Regs.QEPCTL.bit.FREE_SOFT=2;
	EQep1Regs.QEPCTL.bit.PCRM=00;		// PCRM=00 mode - QPOSCNT reset on index event
	EQep1Regs.QEPCTL.bit.UTE=1; 		// Unit Timeout Enable
	EQep1Regs.QEPCTL.bit.QCLM=1; 		// Latch on QPOSCNT :0, unit time out :1
	EQep1Regs.QPOSMAX=0xffffffff;
	EQep1Regs.QEPCTL.bit.QPEN=1; 		// QEP enable
		
	EQep1Regs.QCAPCTL.bit.UPPS=0;   	// 1/1 for unit position
	EQep1Regs.QCAPCTL.bit.CCPS=0;		// 1/1 for CAP clock
	EQep1Regs.QCAPCTL.bit.CEN=1; 		// QEP Capture Enable
	
	EQep1Regs.QPOSCNT=0;
	EQep1Regs.QCTMR=0;
	EQep1Regs.QPOSLAT=0;
	EQep1Regs.QCTMRLAT=0;
	EQep1Regs.QCPRD=0xffff;
/*
	EQep1Regs.QPOSLAT= 0;

	EQep2Regs.QUPRD=Period_SD-1;	// Unit Timer 
	EQep2Regs.QDECCTL.bit.QSRC=00;		// QEP quadrature count mode
		
	EQep2Regs.QEPCTL.bit.FREE_SOFT=2;
	EQep2Regs.QEPCTL.bit.PCRM=00;		// PCRM=00 mode - QPOSCNT reset on index event
	EQep2Regs.QEPCTL.bit.UTE=1; 		// Unit Timeout Enable
	EQep2Regs.QEPCTL.bit.QCLM=1; 		// Latch on QPOSCNT :0, unit time out :1
	EQep2Regs.QPOSMAX=0xffffffff;
	EQep2Regs.QEPCTL.bit.QPEN=0; 		// QEP Disable
		
	EQep2Regs.QCAPCTL.bit.UPPS=0;   	// 1/1 for unit position
	EQep2Regs.QCAPCTL.bit.CCPS=0;		// 1/1 for CAP clock
	EQep2Regs.QCAPCTL.bit.CEN=0; 		// QEP Capture Disable
	
	EQep2Regs.QPOSCNT=0;
	EQep2Regs.QCTMR=0;
	EQep2Regs.QPOSLAT=0;
	EQep2Regs.QCTMRLAT=0;
	EQep2Regs.QCPRD=0xffff;


	EQep2Regs.QPOSLAT= 0;
*/
}

#pragma CODE_SECTION(Speed_Detection, "ramfuncs");
#define		SPD1			(500.*RPM2WRM)
#define		SPD2			(10.*RPM2WRM)




void Speed_Detection()
{
	long pls, mt, pulse_num, i;
	double MT_Time, a;
	double K_Wrm= 0.,K_Wrm1= 0., K_Wrm2= 0.;
	double Pulse2Thetarm= 0., Thetarm_enc= 0.; 
	double Te_tmp=0., Wrm_tmp=0., Thetarm_tmp=0., err_tmp=0., Tl_tmp=0.; 
	static long Pulse_Counter_MT= 0., Pulse_Counter_est=0.; 
	static long Dir_mt=1, Dir_est=1;
	static long	MT_Timer= 0.,  Overflow_num=0., Flag_first_MT= 0.;
//	static double Tl_est= 0.;
	static double Wrm_mt= 0., Wrm_mt_flt= 0., Wrm_mt_old= 0.;
	static double Wrm_est= 0., Wrm_est_flt= 0., Wrm_est_old= 0.;
	static double Wrm_det_old= 0.; 
	static double Thetarm_err= 0., Thetarm_est= 0.; 
//	static double Wrpm_mt=0., Wrpm_est=0., Theta_rm_mt=0.;

	

	
	if(EQep1Regs.QFLG.bit.UTO==1) // If unit timeout (one 2500Hz period)
	{ 	
	/*----------------------------------------------*/
	/*					 MT							*/
	/*----------------------------------------------*/
		
		if (OP.Run_stop.bit.FAULT_RESET) 
			Flag.Fault2.bit.SPEED_DETECTION= 0;

		
		/* to inititial routine */
		Pulse2Thetarm = (2.*PI)/(4.*PPR);

		if(Flag_first_MT==0) 
		{
			Flag_first_MT=1;
			Pulse_Counter_est = Pulse_Counter_MT = (EQep1Regs.QPOSLAT)&(0xffff);
			MT_Timer = (EQep1Regs.QCTMRLAT)&(0xffff);
		}
		else 
		{
			pls = (EQep1Regs.QPOSLAT)&(0xffff);
			mt = (EQep1Regs.QCTMRLAT)&(0xffff);

	
			/* MT Rountine */
			if(pls!=Pulse_Counter_MT) 
			{
				i = pls - Pulse_Counter_MT;
				if(i>(32768))		pulse_num = i - (65536);
				else if(i<-(32768))	pulse_num = i + (65536);
				else				pulse_num = i;
				Pulse_Counter_MT=pls;

   				/* Dir_mt : 1(positive) -1(negative) */		
				a = pulse_num;
				if(pulse_num*Dir_mt<0) 
				{
					a +=Dir_mt/2.;
					Dir_mt*=-1;
				} 

				MT_Time = Tsamp_SD*(Overflow_num + 1)+ (1./CPU_CLOCK)*(float)(MT_Timer-mt);
				MT_Timer = mt;
//				Theta_rm_mt=Pulse2Thetarm*a;
				Wrm_mt = Pulse2Thetarm*a/MT_Time; 

				Overflow_num =0;
			}
			else 
			{
				Overflow_num ++;
				if (Overflow_num>=10000) Overflow_num= 10000;

//				Theta_rm_mt=0;
				a = Tsamp_SD*Overflow_num + (1./CPU_CLOCK)*(float)(MT_Timer);
				if( (a>50.e-3) )	Wrm_mt=0.;
				else 
				{
					a = Pulse2Thetarm*Dir_mt/a;
					if( fabs(Wrm_mt)>fabs(a) )
						Wrm_mt = a;
				}
			}
		}

	/*----------------------------------------------*/
	/*		      Speed Observer					*/
	/*----------------------------------------------*/
//		L1_sp_est= 3*SPEED_OB_GAIN;
//		L2_sp_est= 3*SPEED_OB_GAIN*SPEED_OB_GAIN;
//		L3_sp_est= -(SPEED_OB_GAIN*SPEED_OB_GAIN*SPEED_OB_GAIN)*Jm;
		L3_sp_est= L3_sp_tmp*Jm; 
	
		pls = (EQep1Regs.QPOSLAT)&(0xffff);
		i = pls - Pulse_Counter_est;
		if(i>(32768))		pulse_num = i - (65536);
		else if(i<-(32768))	pulse_num = i + (65536);
		else				pulse_num = i;
		Pulse_Counter_est=pls;

		/* Direction */
		a = pulse_num;
   		if(pulse_num*Dir_est<0) {
			a +=Dir_est/2.;
			Dir_est*=-1;
		}  

		/* Thetarm_enc */
		Thetarm_enc = Pulse2Thetarm*a;

		/* Calculate torque */
		Kt = (3./2.)*Pole_pair*Lm/Lr*LAMdre_ref;
		Te_tmp = Kt*Iqse_ref;

		/* Open Loop Estimation */
		Wrm_tmp = Wrm_est + (Te-Tl_est)*(Tsamp_SD*inv_Jm);
		Thetarm_tmp = Thetarm_est + (Tsamp_SD/2.)*(Wrm_est+Wrm_tmp);

		/* Closed Loop Estimation */
		err_tmp = Thetarm_enc-Thetarm_tmp;		
		a = Thetarm_err + err_tmp;
 		Tl_tmp = Tl_est + (L3_sp_est*a)*(Tsamp_SD/2.);
  		Wrm_tmp = Wrm_est + (Tsamp_SD/2.) 
  			* ( (Te+Te_tmp-Tl_est-Tl_tmp)*inv_Jm + L2_sp_est*a );
		Thetarm_tmp = Thetarm_est + Tsamp_SD/2.*( (Wrm_est+Wrm_tmp) + L1_sp_est*a );

		/* Updating States */
		Thetarm_err = err_tmp;
		Thetarm_est = Thetarm_tmp - Thetarm_enc;
		Wrm_est = Wrm_tmp;
		Tl_est = Tl_tmp;
		Te = Te_tmp;



		//rpm calc.
//		Wrpm_mt = (60./(2.*PI))*Wrm_mt; 
//		Wrpm_est= (60./(2.*PI))*Wrm_est;
		


		Wrm_est_flt = 0.5*(Wrm_est+Wrm_est_old);
		Wrm_est_old = Wrm_est;

		Wrm_mt_flt = 0.5*(Wrm_mt+Wrm_mt_old);
		Wrm_mt_old = Wrm_mt;	

		a = fabs(Wrm_mt_flt);
		if(a>=(2.*SPD1))  	K_Wrm1=0.;
		else if(a<=SPD1) 	K_Wrm1=1.;
		else				K_Wrm1=2.-a/SPD1;

		a = fabs(Wrm_est_flt - Wrm_mt_flt);
		if( a < SPD2 ) {
			K_Wrm2 = -a*a/(2.*SPD2*SPD2)+1.;
		}
		else if ( a < (2.*SPD2) ) {
			a = a - (2.*SPD2);
			K_Wrm2 = a*a/(2.*SPD2*SPD2);
		}
		else	K_Wrm2 = 0.;
		K_Wrm = K_Wrm1*K_Wrm2; 
//		K_Wrm = 0.;
	
		Wrm_det = (1.-K_Wrm)*Wrm_mt_flt + K_Wrm*Wrm_est_flt;
		Wrm_det_flt = 0.5*(Wrm_det+Wrm_det_old);		// Wc = 2/T = 2/100u = 20,000 rad/s
		Wrm_det_old = Wrm_det;
		Wr = Wrm_det_flt*Pole_pair;
		Wrpm_det = (60./(2.*PI))*Wrm_det; 
		

		if (fabs(Wrpm_det)>Wrpm_max)	Flag.Fault1.bit.OVER_SPEED= 1; 

		// QEP initialization
		EQep1Regs.QCLR.bit.UTO=1;  // Clear Unit Timer Flag
  		EQep1Regs.QEPSTS.all=0x00; // Clear All flag
								   
	}
	else // �ӵ� ���� ���� ������  Fault �߻�
	{
		Flag.Fault2.bit.SPEED_DETECTION= 1.;
	}

}

Uint16 diff1, diff2, diff3;
Uint16 pre_abs_position=0;
Uint16 step_delta;


#pragma CODE_SECTION(Speed_Detection_ABS, "ramfuncs");
void Speed_Detection_ABS()
{
	long pls, pulse_num, i;
	double ABS_Time, a;
	double K_Wrm= 0.,K_Wrm1= 0., K_Wrm2= 0.;
	double Pulse2Thetarm= 0., Thetarm_enc= 0.; 
	double Te_tmp=0., Wrm_tmp=0., Thetarm_tmp=0., err_tmp=0., Tl_tmp=0.; 
	static long Pulse_Counter_ABS= 0., Pulse_Counter_est=0.; 
	static long Dir_ABS=1, Dir_est=1;
	static long	Overflow_num=0., Flag_first_ABS= 0.;
//	static double Tl_est= 0.;
	static double Wrm_ABS= 0., Wrm_ABS_flt= 0., Wrm_ABS_old= 0.;
	static double Wrm_est= 0., Wrm_est_flt= 0., Wrm_est_old= 0.;
	static double Wrm_det_old= 0.; 
	static double Thetarm_err= 0., Thetarm_est= 0.;  
//	static double Wrpm_mt=0., Wrpm_est=0., Theta_rm_mt=0.;

//	double turn_new;
//    double step_new, step_delta, abs_step_delta;
//	Uint16 step_new, step_delta;
//    double X_delta_buf, X_delta;
//	static double turn_old = 0, step_old = 0;
//	long y;
//	static int j;

	int Max_step=0, Max_index=0;
	int Min_step=0, Min_index=0;

	int j=0;

	for (j=0; j<4; j++)
	{
		inv_rdata2 = ~com_rdata2[j];
		inv_rdata1 = ~com_rdata1[j];
		Steps_buf1 = 0x0007 & inv_rdata2;
		Number_of_turns_buf = (0x7FF8 & inv_rdata2)>>3;
		Steps_buf2 = (inv_rdata1 >> 6) & 0x03FF;
		Steps_per_turn_buf = ((Steps_buf1 <<10) & 0x1C00) | Steps_buf2;
		number_turn[j] = Number_of_turns_buf;
		step_turn[j] = Steps_per_turn_buf;
	}

    diff1 = abs(step_turn[0] - step_turn[1]);
	diff2 = abs(step_turn[1] - step_turn[2]);
	diff3 = abs(step_turn[2] - step_turn[0]);

	if (diff1 >100)
	{
		test7 = step_turn[0];
		test8 = step_turn[1];
		test9 = step_turn[2];
		test10 = step_turn[3];
	}

	
	Number_of_turns = number_turn[1];
	

/*
   if (diff1 < diff2) {
	    if (diff2 < diff3) {
		    Steps_per_turn = (step_turn[0] + step_turn[1]) / 2;
		}
	    else {
		    Steps_per_turn = (step_turn[0] + step_turn[2]) / 2;
		}
	}
	else {
	    if (diff1 < diff3) {
		    Steps_per_turn = (step_turn[0] + step_turn[1]) / 2;
		}
	    else {
		    Steps_per_turn = (step_turn[1] + step_turn[2]) / 2;
		}
	}
*/

	//Steps_per_turn = ((step_turn[0] + step_turn[1])>>1);

	abs_position = step_turn[1];

	etest1 = (double)step_turn[0];
	etest2 = (double)step_turn[1];

	j = abs_position - pre_abs_position;
	if(j>(4096))		pulse_num = j - (8192);
	else if(j<-(4096))	pulse_num = j + (8192);
	else				pulse_num = j;
	j = abs(j);
//	step_delta = abs(abs_position - pre_abs_position);

		if (j > 100)
		{
		test7 = step_turn[0];
		test8 = step_turn[1];
		test9 = step_turn[2];
		test10 = step_turn[3];
		abs_position = step_turn[1];
		j=0;
		}



	etest6 = abs_position;

/*
	j = abs_position - pre_abs_position;
	if(j>(4096))		pulse_num = j - (8192);
	else if(j<-(4096))	pulse_num = j + (8192);
	else				pulse_num = j;
	
//	step_delta = abs(abs_position - pre_abs_position);

	if (j > 2000)
	{
		abs_position = step_turn[2];
	}

	j = abs_position - pre_abs_position;
	if(j>(4096))		pulse_num = j - (8192);
	else if(j<-(4096))	pulse_num = j + (8192);
 	else				pulse_num = j;

	if (j > 2000)
	{
		abs_position = step_turn[3];
	}

*/
	pre_abs_position = abs_position;


	Pulse2Thetarm = (2.*PI)/(double)(PPR);


	if(Flag_first_ABS==0) 
	{
		Flag_first_ABS=1;
		Pulse_Counter_est = Pulse_Counter_ABS = (long)(abs_position)&(0x1fff);
		ABS_Time = Tsamp_SD;
	}
	else 
	{
		pls = (long)(abs_position)&(0x1fff);

			/* MT Rountine */
		if(pls!=Pulse_Counter_ABS) 
	 
		{
			i = pls - Pulse_Counter_ABS;

			if(i>(4096))		pulse_num = i - (8192);
			else if(i<-(4096))	pulse_num = i + (8192);
			else				pulse_num = i;
			Pulse_Counter_ABS=pls;
			
			/* Dir_mt : 1(positive) -1(negative) */		
			a = pulse_num;

			if(pulse_num*Dir_ABS<0) 
			{
				a +=Dir_ABS*1.;
				Dir_ABS*=-1;
			} 
			ABS_Time = Tsamp_SD*(Overflow_num + 1);
			Wrm_ABS = Pulse2Thetarm*a/ABS_Time;

				
//			Wrm_ABS = Pulse2Thetarm*a/TSAMP_SPD; 
			
			Overflow_num =0;
		}


		else 
		{
			Overflow_num ++;
//				Theta_rm_mt=0;
			a = Tsamp_SD*Overflow_num;
			if( (a>50.e-3) )	Wrm_ABS=0.;
			else 
			{
				a = Pulse2Thetarm*Dir_ABS/a;
				if( fabs(Wrm_ABS)>fabs(a) )
					Wrm_ABS = a;
			}
		
		}

	}
		


	/*----------------------------------------------*/
	/*		      Speed Observer					*/
	/*----------------------------------------------*/
//	L1_sp_est= 3*SPEED_OB_GAIN;
//	L2_sp_est= 3*SPEED_OB_GAIN*SPEED_OB_GAIN;
//	L3_sp_est= -(SPEED_OB_GAIN*SPEED_OB_GAIN*SPEED_OB_GAIN)*Jm;
	L3_sp_est= L3_sp_tmp*Jm; 
	

	pls = (long)(abs_position)&(0x1fff);
	i = pls - Pulse_Counter_est;	
	if(i>(4096))		pulse_num = i - (8192);
	else if(i<-(4096))	pulse_num = i + (8192);
	else				pulse_num = i;
	Pulse_Counter_est=pls;
	/* Direction */
	a = pulse_num;
		if(pulse_num*Dir_est<0) {
		a +=Dir_est*1.;
		Dir_est*=-1;
	}  

	/* Thetarm_enc */
	Thetarm_enc = Pulse2Thetarm*a;

	/* Calculate torque */
	Kt = (3./2.)*Pole_pair*Lm/Lr*LAMdre_ref;
	Te_tmp = Kt*Iqse_ref;

	/* Open Loop Estimation */
	Wrm_tmp = Wrm_est + (Te-Tl_est)*(Tsamp_SD*inv_Jm);
	Thetarm_tmp = Thetarm_est + (Tsamp_SD/2.)*(Wrm_est+Wrm_tmp);

	/* Closed Loop Estimation */
	err_tmp = Thetarm_enc-Thetarm_tmp;		
	a = Thetarm_err + err_tmp;
		Tl_tmp = Tl_est + (L3_sp_est*a)*(Tsamp_SD/2.);
		Wrm_tmp = Wrm_est + (Tsamp_SD/2.) 
			* ( (Te+Te_tmp-Tl_est-Tl_tmp)*inv_Jm + L2_sp_est*a );
	Thetarm_tmp = Thetarm_est + Tsamp_SD/2.*( (Wrm_est+Wrm_tmp) + L1_sp_est*a );

	/* Updating States */
	Thetarm_err = err_tmp;
	Thetarm_est = (Thetarm_tmp - Thetarm_enc);
	Wrm_est = Wrm_tmp;
	Tl_est = Tl_tmp;
	Te = Te_tmp;

	etest4=Te;

	Wrm_est_flt = 0.5*(Wrm_est+Wrm_est_old);
	Wrm_est_old = Wrm_est;
	Wrm_ABS_flt = 0.5*(Wrm_ABS+Wrm_ABS_old);
	Wrm_ABS_old = Wrm_ABS;	

	//rpm calc.
	Wrpm_ABS = (60./(2.*PI))*Wrm_ABS; 
	Wrpm_est= (60./(2.*PI))*Wrm_est;



	a = fabs(Wrm_ABS_flt);
	if(a>=(2.*SPD1))  	K_Wrm1=0.;
	else if(a<=SPD1) 	K_Wrm1=1.;
	else				K_Wrm1=2.-a/SPD1;

	a = fabs(Wrm_est_flt - Wrm_ABS_flt);
	if( a < SPD2 ) {
		K_Wrm2 = -a*a/(2.*SPD2*SPD2)+1.;
	}
	else if ( a < (2.*SPD2) ) {
		a = a - (2.*SPD2);
		K_Wrm2 = a*a/(2.*SPD2*SPD2);
	}
	else	K_Wrm2 = 0.;
	K_Wrm = K_Wrm1*K_Wrm2; 
	K_Wrm = 1.;

	Wrm_det = (1.-K_Wrm)*Wrm_ABS_flt + K_Wrm*Wrm_est_flt;
	Wrm_det_flt = 0.5*(Wrm_det+Wrm_det_old);		// Wc = 2/T = 2/100u = 20,000 rad/s
	Wrm_det_old = Wrm_det;
	Wr = Wrm_det_flt*Pole_pair;
	Wrpm_det = (60./(2.*PI))*Wrm_det_flt;  
		

	if (fabs(Wrpm_det)>Wrpm_max)		Flag.Fault1.bit.OVER_SPEED= 1;

}




