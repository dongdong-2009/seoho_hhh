#define _SYSTEM_

#include "main_def.h"

void Relay_setup()
{
    EALLOW;
	GpioCtrlRegs.GPBMUX1.bit.GPIO38 = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO38 = 1;
	EDIS;
}

void Init_GPIO(void)
{
	EALLOW;
	GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO13 = 1;
	GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO12 = 1;
	GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;
	GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;
	EDIS;
}

void delay(int delay_cnt)
{
	while(delay_cnt--);
}

void delay_long(long int delay_cnt)
{
	while(delay_cnt--);
}

void LSPCLK_setup()
{
    EALLOW;
	SysCtrlRegs.LOSPCP.bit.LSPCLK = 1;			// SYSCLKOUT/2
    EDIS;
}


void Init_AD_converter()
{
	AdcRegs.ADCTRL3.bit.ADCBGRFDN = 0x3;
	delay(10000);
	AdcRegs.ADCTRL3.bit.ADCPWDN = 1;
	delay(10000);
	AdcRegs.ADCTRL1.bit.ACQ_PS = 8;
	AdcRegs.ADCTRL1.bit.CPS = 1;
	AdcRegs.ADCTRL3.bit.ADCCLKPS = 0;
	AdcRegs.ADCMAXCONV.all = 0x000F;
	AdcRegs.ADCTRL1.bit.SEQ_CASC = 1;
	AdcRegs.ADCTRL1.bit.CONT_RUN = 0;
	AdcRegs.ADCCHSELSEQ1.all = 0x3210;
	AdcRegs.ADCCHSELSEQ2.all = 0x7654;
	AdcRegs.ADCCHSELSEQ3.all = 0xBA98;
	AdcRegs.ADCCHSELSEQ4.all = 0xFEDC;
}

int system_first_rst = 0;

void Init_var()
{

	int i = 0;	
 
	if(!system_first_rst)
	{
		for(i=0;i<16;i++)
		{
			adc.scale[i] = 0.0;
//			adc.offset[i] = 0.0;
			adc.conv_val[i] = 0;
			adc.tmp[i] = 0.0;
		}
	system_first_rst = 1;
	}
	// current controller variables 
	flag.FOC = 0;
	flag.GT1_ENA = 0;
	flag.PWM_CTR = 0;
	flag.var_rst = 0;
	flag.control = 0;
	flag.ad_offset_complete = 0;
	flag.enc_rst_complete = 0;
	flag.enc_rst_start = 0;
	flag.cc_seq = 0;

	// Configurate slew_rate of speed estimation
	ref_slew_up = 800. * Tsamp;	// 속도 증가 기울기 = 1000 * Tsamp * Fsw = 1000rpm/sec
	ref_slew_dn = 800. * Tsamp;

	gain_tr = 0.0001, gain_wr = 0.08;
	Kwr = gain_wr;
	Ktr = gain_tr;
	Idse_ref_max = 32.;			// 42
	Idse_ref_sensorless = 6.;	// 12

	Vdse_ref_integ = 0.0;		Vqse_ref_integ = 0.0;
	Vdse_ref = 0.0;				Vqse_ref = 0.0;
	Idse_ref = 0.0;				Iqse_ref = 0.0;
	Idse = 0.0;					Iqse = 0.0;
	Vdse_ref_fb = 0.0;			Vqse_ref_fb = 0.0;
	Vdse_ref_ff = 0.0;			Vqse_ref_ff = 0.0;
	IqcM = 0.;					IdcM = 0.;

	//	Is_max = 50.0;

	thetar_openloop = 0.;
	Wrpm_set_user = 0.;
	thetar_hat = 0.;			Wr_hat = 0.;
	Wr_ref = 0.;				Wrm_err = 0.;
	Wrm_err_integ = 0.;			Wrm_hat = 0.;
	Wrm_ref = 0.;				Wrpm_hat = 0.;
	Wrpm_ref = 0.;

			
			
	flag.control = 0;
	sensorless_mode = 99;		run_Sensorless = 0;
	flag.FOC = 0;				flag_speed_cmd = 0;
	
	flag.GT1_ENA = 0;			flag.PWM_CTR = 0;
	
	sc_cnt = 0.0;				cc_cnt = 0.0;
	tmp_cnt = 0.0;
	first1 =0;					flag.start = 1;

						
	mode.OVM = 0;
	mode.PWM = 0;
	mode.speed_control = 0;

	cc_cnt = 0;
	tmp_sc_cnt = 0;
	sc_cnt = 0;
	flag.start = 1;

	// Drive_Voltage의 값 설정
	// Key_pad 설정 값에 의해서 Drive_Voltage의 값을 설정 하도록 했다. 
	// 2010_09_01, dbsgln
	if 		(V_rate <= 250.) 					   Drive_Voltage=200;
	else if (( V_rate >250.) && ( V_rate <= 460 )) Drive_Voltage=400; 
	else if (( V_rate >460.) && ( V_rate <= 600 )) Drive_Voltage=500; 

	// VD 1.29 version으로 부터 펄트값을 사용하였음 
	// 2010_08_26, dbsgln
	// fault process variables
	if(Drive_Voltage==200)			// 200V급 : ~250V
	{
		fault.OV_set = 410.0;
		fault.UV_set = 180.0;
	}
	else if (Drive_Voltage==400)	// 400V급 : 330 ~ 460V
	{
		fault.OV_set = 815.0;
		fault.UV_set = 350.0;
	}
	else if (Drive_Voltage==500)	// 500V급 : 460 ~ 600V
	{
		fault.OV_set = 900.0;
		fault.UV_set = 450.0;
	}

	// fault process variables
//	adc.scale[0] = 0.03913;// 00.07826; //0.073260073;		// HTB 100-P
//	adc.scale[1] = 0.03913;//0.07826;
	adc.scale[0] = 0.03902;			 // for LAH50-P
	adc.scale[1] = 0.03902;
//	adc.scale[0] = 0.05;			 // for LTS25-NP
//	adc.scale[1] = 0.05;

	adc.scale[3] = 0.225672529;
	adc.scale[4] = 1.0;
	
//	adc.scale[5] = 29.5;		// for analog speed reference (base_speed = 7000, 
	adc.scale[5] = 27.6242;	//	6./4095. * 50000/2.69;
	adc.scale[6] = 1.0;
	adc.scale[7] = 29.5;
	adc.scale[15]= 3./4095.;
	nFLT2_OFF;
	ref_slew_up = 1000. * Tsamp;
	ref_slew_dn = 1000. * Tsamp;

}

void Init_dsc()
{
	pwm_g1.sampling_mode = SAMPLING_TOP;
	// Configurate Tsamp
	if(pwm_g1.sampling_mode == SAMPLING_BOTH) Tsamp = 0.5/(float)(Fpwm);
	else 									  Tsamp = 1.0/(float)(Fpwm);

	pwm_buffer_setup();
	pwm_g1_setup(PWM_BUF_DIR_LOW, PWM_ACTIVE_HIGH, SYS_CLK, Deadtime);
//	LSPCLK_setup();
	Init_AD_converter();
	InitXintf();
	InitGpio();
	Init_var();
}

// 호출 되지 않고 있슴.
// 2010_09_02 dbsgln
void rst_chk()
{
	if(flag.var_rst)
	{
		Init_var();

	}
}

void calculateOffset(void)
{
	Uint16 i;
	Uint32 	Ias_sum, Ibs_sum, AI_sum;

	Ias_sum = 0, Ibs_sum = 0, AI_sum = 0;

	for(i=0; i<4096; i++){

	AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;	// Start of conversion trigger for SEQ1
	AdcRegs.ADCTRL2.bit.SOC_SEQ2 = 1;	// Start of conversion trigger for SEQ2
	DELAY_US(10);

	// Ias Current Sensing
	Ias_sum += ((long)AdcRegs.ADCRESULT0);

	// Ibs Current Sensing
	Ibs_sum += ((long)AdcRegs.ADCRESULT1);

	// AICMD sensing
	AI_sum  += ((long)AdcRegs.ADCRESULT5);
	
	AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;	// 14    Restart sequencer 1
	AdcRegs.ADCTRL2.bit.RST_SEQ2 = 1;	// 14    Restart sequencer 2
	DELAY_US(1);
	}
	Ias_offset = ((unsigned int)(Ias_sum >> 12) >> 4);
	Ibs_offset = ((unsigned int)(Ibs_sum >> 12) >> 4);
	AI_offset  = ((unsigned int)(AI_sum >> 12) >> 4);

	AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 1;
	EPwm1Regs.ETSEL.bit.SOCAEN = 1;
	EPwm1Regs.ETSEL.bit.SOCASEL = 1;
	EPwm1Regs.ETPS.bit.SOCAPRD = 1;

}
