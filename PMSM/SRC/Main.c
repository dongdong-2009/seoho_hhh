/********************************************************


*********************************************************/
#define _MAIN_

#include "main_def.h"
#include "math.h"

#pragma DATA_SECTION(ZONE0_BUF,"ZONE0DATA");

volatile unsigned int ZONE0_BUF[2048];

int main_loop_cnt = 0;

extern void Relay_setup();

void GetAnaMonitCount(unsigned int * piChA, unsigned * piChB);

int flag_relay = 0;

void main(void)
{
	unsigned int ChACount,ChBCount;

    InitSysCtrl();

    DINT;

    InitPieCtrl();

    IER = 0x0000;
    IFR = 0x0000;

    InitPieVectTable();
	Relay_setup();
  	Init_dsc();
	dev_DacSetup ();
	dev_InitDACVariable(); 
	DacFlag = 0;

	MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);

	nRESET_DRIVER_SET;	//316J PWM on

//	nBOOT_MODE_SET;		////	nPWM_ENABLE_SET;	//
	nDC_CONTACT_SET;
	// Call Flash Initialization to setup flash waitstates
	// This function must reside in RAM
	InitFlash();

	// Initialize SCI-A for data monitoring 
	sci_debug_init();
		

	calculateOffset();
	EINT;   // Enable Global interrupt INTM
	ERTM;   // Enable Global realtime interrupt DBGM
//	delay_long(1000000);
	nDC_CONTACT_CLEAR;
//	delay(1000000); //delay_msecs(100);		// Delay for Setting Time

	func_flag.all = 0;

    for ( ; ; ) {
		Is_mag = sqrt(Idss * Idss + Iqss * Iqss);
		digital_input_proc ();
		digital_out_proc ();

		GetAnaMonitCount(&ChACount,&ChBCount);
		EPwm5Regs.CMPA.half.CMPA 	= 3750 - (int)( Wrpm_set_user/61.5);// ChACount;
		EPwm5Regs.CMPB  			= 3750 - (int)( Wrpm_hat/61.5);//ChBCount;
		if(flag.control == 1)
		{
		    if((fault_chk() != 0)|| fault.EVENT ==1) TripProc();
		}
		Vmag_ref = Vdse_ref * Vdse_ref + Vqse_ref * Vqse_ref;
		Vmag_ffa = Vdse_ref_ffa * Vdse_ref_ffa + Vqse_ref_ffa * Vqse_ref_ffa;
		if(Vmag_ref > Vmag_ffa) flag_Vmag = 0;
		else					flag_Vmag = 1;
		Vmag_delta = Vmag_ref - Vmag_ffa;
	    Update_var();
		// DAC Out
		DacFlag = 1;

	test_led2_on;
	dev_BackgroundDAC();
	test_led2_off;
/*		if(DacFlag) 
		{
			
			DacFlag = 0;
		}
		*/
	    rst_chk();
	    op_master();
	    //relay_control(flag_relay);
   	    main_loop_cnt++;

   }
}

void Update_var()
{
	float a;

	// system control
	if(flag.GT1_ENA)
	{
		nBOOT_MODE_CLEAR; 
		nPWM_ENABLE_CLEAR;	
	} 
	else 
	{
		nBOOT_MODE_SET;	
		nPWM_ENABLE_SET;
	}


		fault.I_Lmt 		= 65. * 1.45;
		fault.MaxCon_Curr 	= 65. * 0.95;
		fault.Over_Load 	= 65. * 1.35;
		fault.OC_set		= 65.;

	// Update slew_rate of speed estimation
//	ref_slew = 3000. * Tsamp;
//	ref_slew_dn = 3000. * Tsamp;
	if(flag_speed_cmd == 1)
	{
//		Wrpm_set_user = Wrpm_base_speed + AICMD;
		if(Wrpm_set_user >= 60000.) Wrpm_set_user = 60000.;
		if(Wrpm_set_user <= Wrpm_base_speed ) Wrpm_set_user = Wrpm_base_speed;
	}
	// Update Tsamp
	if(pwm_g1.sampling_mode == SAMPLING_BOTH)
	{
		Tsamp = 0.5/Fsw;
	} 
	else 
	{
		Tsamp = 1.0/Fsw;
	}

//	if(func_flag.bit.DRIVE_ENABLE == 1)
	if(DRIVE_ENABLE == 1)
	{
		if(sensorless_mode == 4) flag_speed_cmd = 1;
		else flag_speed_cmd = 0;

		if(FaultInfo == 0)
		{
			flag.control = 1;
			if((flag.FOC == 1)&&(flag.start ==1)) 
			{
				sensorless_mode = 0; 
				flag.start = 0;
				run_Sensorless = 1;
			}
			
		}
	}
//	if ((func_flag.bit.JOG == 1)||(func_flag.bit.DRIVE_ENABLE==0))
	if (DRIVE_ENABLE==0)
	{
		Init_var();
		thetar_openloop = 0.;
		Wrpm_set_user = 0.;
		Vdse_ref_integ = 0.;
		Vqse_ref_integ = 0.;
		IqcM = 0.;
		IdcM = 0.;
		thetar_hat = 0.;
		run_Sensorless = 0;
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
		flag.control = 0;
		sensorless_mode = 99;
		run_Sensorless = 0;
		flag.start = 1;
		flag.FOC = 0;
		flag_speed_cmd = 0;
		tmp_cnt = 0;
	}
	// Update control variables
	Wc_cc = 6.283184 * fc_cc;		// CC BW : 2*PI*fc = 200[Hz]
	Kp_cc = Wc_cc * Ls;
	Ki_ccT = Wc_cc * Rs * Tsamp;

	a = 2*PI*Wrm_L.fc;
	Wrm_L.Tsamp = 0.002;
	Wrm_L.La = (2. - a*Wrm_L.Tsamp)/(2. + a*Wrm_L.Tsamp);
	Wrm_L.Lb = a*Wrm_L.Tsamp / (2. + a*Wrm_L.Tsamp);

	faultEVENT = fault.EVENT;
	faultFT_SWPROT_Ias = fault.FT_SWPROT_Ias;
	faultIas_OC = fault.Ias_OC;
	faultIbs_OC = fault.Ibs_OC;
	faultIcs_OC = fault.Ics_OC;
	faultFT_SWPROT_Vdc_UV = fault.FT_SWPROT_Vdc_UV;


//	DRIVE_ENABLE = func_flag.bit.DRIVE_ENABLE;
	MULTI_STEP_BIT0 = func_flag.bit.MULTI_STEP_BIT0;
	MULTI_STEP_BIT1 = func_flag.bit.MULTI_STEP_BIT1;
	MULTI_STEP_BIT2 = func_flag.bit.MULTI_STEP_BIT2;
	MULTI_STEP_BIT3 = func_flag.bit.MULTI_STEP_BIT3;
	FAULT_RESET = func_flag.bit.FAULT_RESET;
	JOG = func_flag.bit.JOG;
}

void GetAnaMonitCount(unsigned int * piChA, unsigned * piChB)
{

	static int i;

	i += 8;
	
	if (i >3749) i = 0;
			
	* piChA = i;
	* piChB = i;
}
