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

void Init_Encoder()
{
	InitEQep1Gpio();
    EQep1Regs.QUPRD=SYS_CLK;			// Unit Timer for 100Hz at 150 MHz SYSCLKOUT
	EQep1Regs.QDECCTL.bit.QSRC=00;		// QEP quadrature count mode

	EQep1Regs.QEPCTL.bit.IEI = 0;
//	EQep1Regs.QEPCTL.bit.IEI = 2;

	EQep1Regs.QEPCTL.bit.FREE_SOFT=2;
	EQep1Regs.QEPCTL.bit.PCRM=01;		// 
	EQep1Regs.QEPCTL.bit.UTE=1; 		// Unit Timeout Enable 
	EQep1Regs.QEPCTL.bit.QCLM=1; 		// Latch on unit time out
	EQep1Regs.QPOSMAX = 8000;//		ENC_PPR - 1;
	EQep1Regs.QEPCTL.bit.QPEN=1; 		// QEP enable
		
	EQep1Regs.QCAPCTL.bit.UPPS=5;   	// 1/32 for unit position
	EQep1Regs.QCAPCTL.bit.CCPS=7;		// 1/128 for CAP clock
	EQep1Regs.QCAPCTL.bit.CEN=1; 		// QEP Capture Enable

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

	Vdse_ref_integ = 0.0;		Vqse_ref_integ = 0.0;
	Vdse_ref = 0.0;				Vqse_ref = 0.0;
	Idse_ref = 0.0;				Iqse_ref = 0.0;
	Idse = 0.0;					Iqse = 0.0;
	Vdse_ref_fb = 0.0;			Vqse_ref_fb = 0.0;
	Vdse_ref_ff = 0.0;			Vqse_ref_ff = 0.0;

	Is_max = 50.0;
	
	Rs = 0.024;
	Ls = 0.000135;

	mode.OVM = 0;
	mode.PWM = 0;
	mode.speed_control = 0;

	cc_cnt = 0;
	tmp_sc_cnt = 0;
	sc_cnt = 0;
	flag.start = 1;

	// fault process variables
	fault.EVENT = 0;

	fault.OC_set = 65.0;
	fault.OV_set = 360.0;
	fault.UV_set = 7.0;
	fault.FT_SWPROT_Ias = 0;
	fault.FT_SWPROT_Ibs = 0;
	fault.FT_SWPROT_Ics = 0;
	fault.FT_SWPROT_Vdc_OV = 0;
	fault.FT_SWPROT_Vdc_UV = 0;
	fault.Ias_OC = 0;
	fault.Ibs_OC = 0;
	fault.Ics_OC = 0;

	adc.scale[0] = 0.03913;	// HC-PRX
	adc.scale[1] = 0.03913;
//	adc.scale[0] = 0.03902;			 // for LAH50-P
//	adc.scale[1] = 0.03902;
//	adc.scale[0] = 0.05;			 // for LTS25-NP
//	adc.scale[1] = 0.05; 
	adc.scale[3] = 0.225672529;
	adc.scale[4] = 1.0;
	adc.scale[5] = 29.5;
	adc.scale[6] = 1.0;
	adc.scale[7] = 29.5;
	nFLT2_OFF;

}

void Init_dsc()
{

	pwm_buffer_setup();
	pwm_g1_setup(PWM_BUF_DIR_LOW, PWM_ACTIVE_HIGH, SYS_CLK, Fsw, Deadtime);
	pwm_g1.sampling_mode = SAMPLING_TOP;
	LSPCLK_setup();
	Init_AD_converter();
	Init_Encoder();
	InitXintf();
	InitGpio();
	Init_var();

}

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
	Uint32 	Ias_sum, Ibs_sum;

	Ias_sum = 0, Ibs_sum = 0;

	for(i=0; i<4096; i++){

	AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;	// Start of conversion trigger for SEQ1
	AdcRegs.ADCTRL2.bit.SOC_SEQ2 = 1;	// Start of conversion trigger for SEQ2
	DELAY_US(10);

	// Ias Current Sensing
	Ias_sum += ((long)AdcRegs.ADCRESULT0);

	// Ibs Current Sensing
	Ibs_sum += ((long)AdcRegs.ADCRESULT1);
	
	AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;	// 14    Restart sequencer 1
	AdcRegs.ADCTRL2.bit.RST_SEQ2 = 1;	// 14    Restart sequencer 2
	DELAY_US(1);
	}
	Ias_offset = ((unsigned int)(Ias_sum >> 12) >> 4);
	Ibs_offset = ((unsigned int)(Ibs_sum >> 12) >> 4);


	AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 1;
	EPwm1Regs.ETSEL.bit.SOCAEN = 1;
	EPwm1Regs.ETSEL.bit.SOCASEL = 1;
	EPwm1Regs.ETPS.bit.SOCAPRD = 1;

}
