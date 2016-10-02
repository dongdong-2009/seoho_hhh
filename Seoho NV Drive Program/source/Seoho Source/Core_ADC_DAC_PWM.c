#include	<All_Header.h>
#include	<All_Extern_Variables.h>

void InitAdc()
{
//	SysCtrlRegs.HISPCP.all = 0x0001;		// system clock out/2	150MHz/2= 75
	AdcRegs.ADCTRL3.bit.ADCCLKPS = 1;		// 75/2= 37.5
	AdcRegs.ADCTRL1.bit.CPS = 1;			// 37.5/2= 18.75
	
	AdcRegs.ADCMAXCONV.all = 0x55;       // dual 6channel(2), 6channel(1)
	AdcRegs.ADCTRL1.bit.SUSMOD = 1;
	AdcRegs.ADCTRL1.bit.ACQ_PS = 7;
	
	AdcRegs.ADCTRL1.bit.CONT_RUN = 0;
	AdcRegs.ADCTRL1.bit.SEQ_OVRD = 0;
	AdcRegs.ADCTRL1.bit.SEQ_CASC = 0;
/*
	AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0; // current U
	AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 1; // current V
	AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 2; // current W
	AdcRegs.ADCCHSELSEQ1.bit.CONV03 = 3; // voltage Vdc
	AdcRegs.ADCCHSELSEQ2.bit.CONV04 = 4; // current 1
	AdcRegs.ADCCHSELSEQ2.bit.CONV05 = 5; // voltage 1
//	AdcRegs.ADCCHSELSEQ2.bit.CONV06 = 6; // current 2
//	AdcRegs.ADCCHSELSEQ2.bit.CONV07 = 7; // voltage 2
//	AdcRegs.ADCCHSELSEQ3.bit.CONV08 = 8; // analog input 3
//	AdcRegs.ADCCHSELSEQ3.bit.CONV09 = 9; // analog input 4
//	AdcRegs.ADCCHSELSEQ3.bit.CONV10 = 10; // analog input 5
//	AdcRegs.ADCCHSELSEQ3.bit.CONV11 = 14; // temperature
//	AdcRegs.ADCCHSELSEQ4.bit.CONV12 = 15; // voltage range
	AdcRegs.ADCCHSELSEQ3.bit.CONV08 = 14; // temperature
	AdcRegs.ADCCHSELSEQ3.bit.CONV09 = 15; // voltage range
*/
	AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0; // current U
	AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 1; // current V
	AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 2; // current W
	AdcRegs.ADCCHSELSEQ1.bit.CONV03 = 3; // voltage Vdc
	AdcRegs.ADCCHSELSEQ2.bit.CONV04 = 4; // current 1
	AdcRegs.ADCCHSELSEQ2.bit.CONV05 = 5; // voltage 1

	AdcRegs.ADCCHSELSEQ3.bit.CONV08 = 6; // current 2
	AdcRegs.ADCCHSELSEQ3.bit.CONV09 = 7; // voltage 2
	AdcRegs.ADCCHSELSEQ3.bit.CONV10 = 8; // analog input 3
	AdcRegs.ADCCHSELSEQ3.bit.CONV11 = 9; // analog input 4
	AdcRegs.ADCCHSELSEQ4.bit.CONV12 = 14; // temperature
	AdcRegs.ADCCHSELSEQ4.bit.CONV13 = 15; // voltage range


	AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 1;// Enable SOCA from ePWM to start SEQ1
	AdcRegs.ADCTRL2.bit.EPWM_SOCB_SEQ2 = 1;// Enable SOCB from ePWM to start SEQ1
	AdcRegs.ADCTRL3.bit.ADCBGRFDN= 3;
	AdcRegs.ADCTRL3.bit.ADCPWDN= 1;
	AdcRegs.ADCREFSEL.all = 0x0000;

	//for adc irq 
	EPwm1Regs.ETSEL.bit.SOCAEN= 1;   // Enable SOC on A group
	EPwm1Regs.ETSEL.bit.SOCBEN= 1;   // Enable SOC on B group
	EPwm1Regs.ETSEL.bit.SOCASEL= ET_CTR_ZERO;
	EPwm1Regs.ETSEL.bit.SOCBSEL= ET_CTR_ZERO;
	EPwm1Regs.ETPS.bit.SOCAPRD= 1;        // Generate pulse on every event
	EPwm1Regs.ETPS.bit.SOCBPRD= 1;        // Generate pulse on every event
	EPwm1Regs.ETCLR.bit.SOCA= 1;	// Clear SOCA flag
	EPwm1Regs.ETCLR.bit.SOCB= 1;	// Clear SOCB flag


}



// 28335 Voltage Range: 0~3V
// 22kW CT Simulation Result: 50A=4V(CT Output)=2.43V(28335 input), by 1.5V Offset
// I=(50/0.93)*(3/4095)*(bit-2047)
// (100413) 재수정
// 5.5 7.5kW에서 ADC 입력이 반전됨

// 50A=4V(CT Output)=-0.3V(28335 input), by 1.5V Offset
// -50A=-4V(CT Output)=3.3V(28335 input), by 1.5V Offset
// I=(50/-1.8)*(3/4095)*(bit-2047) 5.5kW
// I=(50/-1.43)*(3/4095)*(bit-2047) 7.5kW
// 전류 측정지연 32.81us(5.5) 33.42(7.5)
// I_scale 전역 변수 이동 
//#define I_scale					(50.0/(1.8))
// 2407 VDC gain 1000.84=3.28
// 28335: 1000.84*(3/3.28)= 915.4 -> manual tuning 970
// 1200V 급 2683.6
// 600V 급 
#if ((VOLTAGE_CLASS==0)||(VOLTAGE_CLASS==1))
	#define Vdc_scale				(970.)
#elif (VOLTAGE_CLASS==2)
	#define Vdc_scale				(970.*1573./1000.84)
#elif (VOLTAGE_CLASS==3)
	#define Vdc_scale				(970.*2683.6/1000.84)
#endif
#define Vdc_offset				(0.)

#pragma CODE_SECTION(GetSensorValue, "ramfuncs");
void GetSensorValue( )
{
	double Temp;
/*
	AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0; // current U
	AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 1; // current V
	AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 2; // current W
	AdcRegs.ADCCHSELSEQ1.bit.CONV03 = 3; // voltage Vdc
	AdcRegs.ADCCHSELSEQ2.bit.CONV04 = 4; // current 1
	AdcRegs.ADCCHSELSEQ2.bit.CONV05 = 5; // voltage 1

	AdcRegs.ADCCHSELSEQ3.bit.CONV08 = 6; // current 2
	AdcRegs.ADCCHSELSEQ3.bit.CONV09 = 7; // voltage 2
	AdcRegs.ADCCHSELSEQ3.bit.CONV10 = 8; // analog input 3
	AdcRegs.ADCCHSELSEQ3.bit.CONV11 = 9; // analog input 4
	AdcRegs.ADCCHSELSEQ4.bit.CONV12 = 14; // temperature
	AdcRegs.ADCCHSELSEQ4.bit.CONV13 = 15; // voltage range 
*/

	Adc_Uphase		= (int)(AdcRegs.ADCRESULT0	>> 4); // current U
	Adc_Vphase		= (int)(AdcRegs.ADCRESULT1  >> 4); // current V
	Adc_Wphase 		= (int)(AdcRegs.ADCRESULT2 	>> 4); // current W
	Adc_Vdc			= (int)(AdcRegs.ADCRESULT3 	>> 4); // voltage Vdc
	Adc_AIN_I1 		= (int)(AdcRegs.ADCRESULT4 	>> 4); // current 1
	Adc_AIN_V1 		= (int)(AdcRegs.ADCRESULT5 	>> 4); // voltage 1

	Adc_AIN_I2 		= (int)(AdcRegs.ADCRESULT8 	>> 4); // current 1
	Adc_AIN_V2 		= (int)(AdcRegs.ADCRESULT9 	>> 4); // voltage 1
	Adc_AIN_3 		= (int)(AdcRegs.ADCRESULT10 >> 4); // analog input 3
	Adc_AIN_4 		= (int)(AdcRegs.ADCRESULT11 >> 4); // analog input 4
	Adc_Temperature	= (int)(AdcRegs.ADCRESULT12	>> 4); // temperature
	Adc_Vclass 		= (int)(AdcRegs.ADCRESULT13	>> 4); // voltage range 

	Temp= (double)(Adc_Uphase);
	Ias= I_scale * (3.0/4095.0) * (Temp-2047) - Ias_offset;
	Temp= (double)(Adc_Vphase);
	Ibs= I_scale * (3.0/4095.0) * (Temp-2047) - Ibs_offset;
	Ics= -(Ias+Ibs);

    Idss = Ias;
	Iqss = (Ibs-Ics)/SQRT3;
	Is_mag = sqrt(Idss*Idss+Iqss*Iqss);           
	Is_mag_rms = Is_mag/SQRT2;  

	Temp= (double)(Adc_Vdc);
	Vdc=  Vdc_scale *(Temp/4095.0) - Vdc_offset;

	Temperature_x10= Temperature_Calculation(Adc_Temperature);

	// 입력 파워= (3/2) x [(Vd x Id) + (Vq x Iq)]
	// 스케일 : kW -> x 1.0e-3, x10 -> x 10 
	// =>  Te x [(PI/30) x rpm] /1000
	//   = 1.0471967e-3 x [Te x rpm]		: x10
	Input_power_x10_kW= (int)(1.5e-2*(Vdss_ref*Idss+Vqss_ref*Iqss));
						
	// 출력 파워
	// Pwr[kW] = 1.0e-3 x {Trq[Nm] x (PI/30) x Spd[rpm]}
 	//         = 1.047197e-3 {Trq[Nm] x Spd[rpm]}
	Output_power_x10_kW= (int)((1.047197e-3*Te)*Wrpm_det_flt);	

	if (Vdc>Vdc_max) Vdc_max=Vdc;

	AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;         // Reset SEQ1
	AdcRegs.ADCTRL2.bit.RST_SEQ2 = 1;         // Reset SEQ2
}

DAC	deg_Dac [ 4 ];
S16 deg_sDacTmp;

void dev_SendDAC ( DAC *pDac );
void DAC_Out(WORD ch1, WORD ch2);

void dev_DacSetup ( void )
{
	EALLOW;
/*
	// nDAC1_CS pin setup
	GpioCtrlRegs.GPBMUX2.bit.GPIO55  = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO55   = 1;

	// nDAC2_CS pin setup
	GpioCtrlRegs.GPBMUX2.bit.GPIO57  = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO57   = 1;

    GpioCtrlRegs.GPBPUD.bit.GPIO54   = 0;     // Enable pull-up on GPIO24 (SPISIMOB)                                                  
    GpioCtrlRegs.GPBPUD.bit.GPIO56   = 0;     // Enable pull-up on GPIO26 (SPICLKB) 
    GpioCtrlRegs.GPBQSEL2.bit.GPIO54 = 3;     // Asynch input GPIO24 (SPISIMOB)
    GpioCtrlRegs.GPBQSEL2.bit.GPIO56 = 3;     // Asynch input GPIO26 (SPICLKB) 
    GpioCtrlRegs.GPBMUX2.bit.GPIO54  = 1;     // Configure GPIO24 as SPISIMOB
    GpioCtrlRegs.GPBMUX2.bit.GPIO56  = 1;     // Configure GPIO26 as SPICLKB         
*/

	SpiaRegs.SPICCR.bit.SPISWRESET   = 0;	   // SPI SW RESET = 0
	SpiaRegs.SPICCR.all              = 0x004F; // Reset on, falling edge, 16-bit char bits  0100 1111 
	SpiaRegs.SPICTL.all              = 0x0006; // Enable master mode, normal phase,         0000 0110
	SpiaRegs.SPICTL.bit.CLK_PHASE    = 1;     
	SpiaRegs.SPICCR.bit.CLKPOLARITY  = 1;
 	SpiaRegs.SPIBRR                  = 5;	    // 6.25MHz							
    SpiaRegs.SPIPRI.bit.FREE         = 1;      // Set so breakpoints don't disturb xmission
	SpiaRegs.SPICTL.bit.SPIINTENA    = 1;
	SpiaRegs.SPICCR.bit.SPISWRESET   = 1;      // SPI SW RESET = 1
 
    EDIS;
}

void dev_InitDACVariable ( void )
{
	char i;

	DAC	*deg_pDac;
	deg_pDac = &deg_Dac [ 0 ]; 

	for ( i = 0; i < 4; i++ )
	{
		deg_pDac->ucCh   = i + 1;
		deg_pDac->ucType = FLOAT_TYPE;
 		deg_pDac->fValue = 0;
		deg_pDac->fScale = 1;
		deg_pDac++;	
	}
}

#pragma CODE_SECTION(dev_SendDAC, "ramfuncs");
void dev_SendDAC ( DAC *pDac )
{

	static	S16 sTemp;

    sTemp =  ( int )( (float)( ( pDac->fValue * 2048.) + 2048. ));

	if ( sTemp > 4095)	sTemp= 4095;
	else if ( sTemp < 0 ) sTemp= 0;
	
	switch ( pDac->ucCh )
	{
		case DAC_CH1 : 
		{
			DAC1_CS = 0;
			SpiaRegs.SPITXBUF = 0xC000 | (sTemp & 0xfff);
			while ( !SpiaRegs.SPISTS.bit.INT_FLAG );
			deg_sDacTmp = SpiaRegs.SPIRXBUF;			// empty RX buffer data  : to clear INT FLAG
			DAC1_CS = 1;
			break;
		}
		case DAC_CH2 : 
		{
			DAC1_CS = 0;
			SpiaRegs.SPITXBUF = 0x4000 | (sTemp & 0xfff);
			while ( !SpiaRegs.SPISTS.bit.INT_FLAG );
			deg_sDacTmp = SpiaRegs.SPIRXBUF;
			DAC1_CS = 1;
			break;
		}
		case DAC_CH3 : 
		{
			DAC2_CS = 0;
			SpiaRegs.SPITXBUF = 0xC000 | (sTemp & 0xfff);
			while ( !SpiaRegs.SPISTS.bit.INT_FLAG );
			deg_sDacTmp = SpiaRegs.SPIRXBUF;
			DAC2_CS = 1;
			break;
		}
		case DAC_CH4 : 
		{
			DAC2_CS = 0;
			SpiaRegs.SPITXBUF = 0x4000 | (sTemp & 0xfff);
			while ( !SpiaRegs.SPISTS.bit.INT_FLAG );
			deg_sDacTmp = SpiaRegs.SPIRXBUF;
			DAC2_CS = 1;
			break;
		}
		default : 
			break;
	}

}

#pragma CODE_SECTION(dev_BackgroundDAC, "ramfuncs");
void dev_BackgroundDAC ( void )
{

	DAC *deg_pDac;
	float Temp;

	deg_pDac = &deg_Dac [0];
	if (DA1_sign==0)	Temp= (DA1-DA1_offset)/DA1_range;
	else				Temp= (DA1-DA1_offset)/(0.5*DA1_range) -1.;		
	deg_pDac->fValue= BOUND(Temp, 1, -1);
	dev_SendDAC ( deg_pDac );
	NOP; 
	deg_pDac++;
	if (DA2_sign==0)	Temp= (DA2-DA2_offset)/DA2_range;
	else				Temp= (DA2-DA2_offset)/(0.5*DA2_range) -1.;	
	deg_pDac->fValue= BOUND(Temp, 1, -1);
	dev_SendDAC ( deg_pDac );
	NOP; 
	deg_pDac++;
	if (DA3_sign==0)	Temp= (DA3-DA3_offset)/DA3_range;
	else				Temp= (DA3-DA3_offset)/(0.5*DA3_range) -1.;	
	deg_pDac->fValue= BOUND(Temp, 1, -1);
	dev_SendDAC ( deg_pDac );
	NOP; 
	deg_pDac++;
	if (DA4_sign==0)	Temp= (DA4-DA4_offset)/DA4_range;
	else				Temp= (DA4-DA4_offset)/(0.5*DA4_range) -1.;	
	deg_pDac->fValue= BOUND(Temp, 1, -1);
	dev_SendDAC ( deg_pDac );
	NOP;

}


void EPWM_Initialization()
{  
	EPwmPeriodCount =	(Uint16)( F_OSC * DSP28_PLLCR / ((double)P.G01.P07_PWM_frequency_x10_kHz*1.e+2) / 2 /2);
	
	EPwm1Regs.TBPRD =  EPwmPeriodCount;				// Set timer period
	EPwm1Regs.TBPHS.half.TBPHS = 0x0000;           	// Phase is 0
	EPwm1Regs.TBCTR = 0x0000;                      	// Clear counter

	// Setup TBCLK
	EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;	// Count up
	EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;			// Disable phase loading
	EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;					// Clock ratio to SYSCLKOUT
	EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;
	EPwm1Regs.TBCTL.bit.SYNCOSEL= TB_CTR_ZERO;

	EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;	
	EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

	#if (DUAL_PWM_INTERRUPT)
		EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;
		EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;
	#else
		EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
		EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	#endif	  



	// Setup compare 
	EPwm1Regs.CMPA.half.CMPA = 0;
   
	// Set actions
	EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;					// Set PWM1A on Zero
	EPwm1Regs.AQCTLA.bit.CAD = AQ_SET;
	   
	EPwm1Regs.DBCTL.bit.OUT_MODE 	= DB_FULL_ENABLE;
	EPwm1Regs.DBCTL.bit.POLSEL 	= DB_ACTV_LOC;
	EPwm1Regs.DBCTL.bit.IN_MODE 	= DBA_ALL;
	EPwm1Regs.DBRED = DEAD_TIME_COUNT;					// debug set to 4usec
	EPwm1Regs.DBFED = DEAD_TIME_COUNT;
   
	// Interrupt where we will change the Deadband
	EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;			// Select INT on Zero event
	EPwm1Regs.ETPS.bit.INTPRD = 1;						// Generate INT on 1st debug   

	EPwm1Regs.ETSEL.bit.INTEN = 1;    		            // Enable INT
	
	// Trip Zone One-shot Enable(Fault Driver, DB, OC, OV, ZSC)
	// ZSC 회로 오류 해결해야됨 
	EALLOW;
	EPwm1Regs.TZSEL.bit.OSHT1= 1;
	EPwm1Regs.TZSEL.bit.OSHT2= 1;
	EPwm1Regs.TZSEL.bit.OSHT3= 1;
	EPwm1Regs.TZSEL.bit.OSHT4= 1;
//	EPwm1Regs.TZSEL.bit.CBC1= 1;
//	EPwm1Regs.TZSEL.bit.CBC2= 1;
//	EPwm1Regs.TZSEL.bit.CBC3= 1;
//	EPwm1Regs.TZSEL.bit.CBC4= 1;
	EPwm1Regs.TZCTL.bit.TZA=0; // High impedance  
	EDIS;

	//---------------------------------
	// Sset PWM2   
	//---------------------------------

	// Setup TBCLK
	EPwm2Regs.TBPRD =  EPwmPeriodCount;				// Set timer period
	EPwm2Regs.TBPHS.half.TBPHS = 0x0000;           	// Phase is 0
	EPwm2Regs.TBCTR = 0x0000;                      	// Clear counter
	EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; 		// Count up
//	EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;        		// Disable phase loading
	EPwm2Regs.TBCTL.bit.PHSEN = TB_ENABLE;        		// Disable phase loading
	EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       			// Clock ratio to SYSCLKOUT
	EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;          			// Slow just to observe on the scope
	EPwm2Regs.TBCTL.bit.SYNCOSEL= TB_SYNC_IN;

	EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;	
	EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;	
		

 	#if (DUAL_PWM_INTERRUPT)
		EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;
		EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;
	#else
		EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
		EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	#endif 
	
	// Setup compare 
	EPwm2Regs.CMPA.half.CMPA = 0;

	// Set actions
	EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;             		// Set PWM2A on Zero
	EPwm2Regs.AQCTLA.bit.CAD = AQ_SET;
	// Active Low complementary PWMs - setup the deadband
	EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_LOC;
	EPwm2Regs.DBCTL.bit.IN_MODE = DBA_ALL;
	EPwm2Regs.DBRED = DEAD_TIME_COUNT;
	EPwm2Regs.DBFED = DEAD_TIME_COUNT;
	EPwm2Regs.ETSEL.bit.INTEN = 0;                 		// Enable INT



	// Trip Zone One-shot Enable(Fault Driver, DB, OC, OV, ZSC)
	EALLOW;
	EPwm2Regs.TZSEL.bit.OSHT1= 1;
	EPwm2Regs.TZSEL.bit.OSHT2= 1;
	EPwm2Regs.TZSEL.bit.OSHT3= 1;
	EPwm2Regs.TZSEL.bit.OSHT4= 1;
//	EPwm2Regs.TZSEL.bit.CBC1= 1;
//	EPwm2Regs.TZSEL.bit.CBC2= 1;
//	EPwm2Regs.TZSEL.bit.CBC3= 1;
//	EPwm2Regs.TZSEL.bit.CBC4= 1;
	EPwm2Regs.TZCTL.bit.TZA=0; // High impedance
	EDIS;
	//------------------------------
	//InitEPwm3Example()
	//------------------------------

	EPwm3Regs.TBPRD =  EPwmPeriodCount;				// Set timer period
	EPwm3Regs.TBPHS.half.TBPHS = 0x0000;           	// Phase is 0
	EPwm3Regs.TBCTR = 0x0000;                      	// Clear counter
	EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; 		// Count up
	EPwm3Regs.TBCTL.bit.PHSEN = TB_ENABLE; 	
//	EPwm3Regs.TBCTL.bit.PHSEN = TB_DISABLE; 			// 
	EPwm3Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;				// 
	EPwm3Regs.TBCTL.bit.CLKDIV = TB_DIV1;				// Slow so we can observe on the scope
	EPwm3Regs.TBCTL.bit.SYNCOSEL= TB_SYNC_IN;

	EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;	
	EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;	

 	#if (DUAL_PWM_INTERRUPT)
		EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;
		EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;
	#else
		EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
		EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	#endif 

	
	// Setup compare 
	EPwm3Regs.CMPA.half.CMPA = 0;

	// Set actions
	EPwm3Regs.AQCTLA.bit.CAU = AQ_CLEAR;              // Set PWM3A on Zero
	EPwm3Regs.AQCTLA.bit.CAD = AQ_SET;

	// Active high complementary PWMs - Setup the deadband
	EPwm3Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	EPwm3Regs.DBCTL.bit.POLSEL = DB_ACTV_LOC;
	EPwm3Regs.DBCTL.bit.IN_MODE = DBA_ALL;
	EPwm3Regs.DBRED = DEAD_TIME_COUNT;
	EPwm3Regs.DBFED = DEAD_TIME_COUNT;
	EPwm3Regs.ETSEL.bit.INTEN = 0;                  // Enable INT


	// Trip Zone One-shot Enable(Fault Driver, DB, OC, OV, ZSC)
	EALLOW;
	EPwm3Regs.TZSEL.bit.OSHT1= 1;
	EPwm3Regs.TZSEL.bit.OSHT2= 1;
	EPwm3Regs.TZSEL.bit.OSHT3= 1;
	EPwm3Regs.TZSEL.bit.OSHT4= 1;
//	EPwm3Regs.TZSEL.bit.CBC1= 1;
//	EPwm3Regs.TZSEL.bit.CBC2= 1;
//	EPwm3Regs.TZSEL.bit.CBC3= 1;
//	EPwm3Regs.TZSEL.bit.CBC4= 1; 
	EPwm3Regs.TZCTL.bit.TZA=0; // High impedance
	EDIS;
	//------------------------------
	//InitEPwm4   A == db B INIT_Charge 
	//------------------------------

	EPwm4Regs.TBPRD =  EPwmPeriodCount;			// Set timer period
	EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;	// Count up
	EPwm4Regs.TBCTL.bit.PHSEN = TB_DISABLE; 
	EPwm4Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
	EPwm4Regs.TBCTL.bit.CLKDIV = TB_DIV1;

	EPwm4Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
//	EPwm4Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
 
 	#if (DUAL_PWM_INTERRUPT)
		EPwm4Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;
//		EPwm4Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;
	#else
		EPwm4Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
//		EPwm4Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	#endif  
 
	// Setup compare 
	EPwm4Regs.CMPA.half.CMPA =  0;

	// Set actions
	EPwm4Regs.AQCTLA.bit.CAU = AQ_CLEAR;             // Set PWM4A on Zero
	EPwm4Regs.AQCTLA.bit.CAD = AQ_SET;

	// Active high complementary PWMs - Setup the deadband
	EPwm4Regs.DBCTL.bit.OUT_MODE = DB_DISABLE;
	EPwm4Regs.ETSEL.bit.INTEN = 0;                  // Enable INT
	EPwm4Regs.DBCTL.bit.POLSEL = DB_ACTV_LOC;
	EPwm4Regs.DBCTL.bit.IN_MODE = DBA_ALL;

 

	//------------------------------
	//InitEPwm5
	//------------------------------

	EPwm5Regs.TBPRD 				= EPwmPeriodCount;   // Set timer period
	EPwm5Regs.TBCTL.bit.CTRMODE 	= TB_COUNT_UPDOWN; 		// Count up
	EPwm5Regs.TBCTL.bit.PHSEN 	= TB_DISABLE;        // 
	EPwm5Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;      // 
	EPwm5Regs.TBCTL.bit.CLKDIV 	= TB_DIV1;         // Slow so we can observe on the scope

	// Setup compare 
	EPwm5Regs.CMPA.half.CMPA 	= 0;
	EPwm5Regs.CMPB 	= 0;

	// Set actions
	EPwm5Regs.AQCTLA.bit.CAU 		= AQ_CLEAR;             
	EPwm5Regs.AQCTLA.bit.CAD 		= AQ_SET;

	EPwm5Regs.AQCTLB.bit.CBU 		= AQ_CLEAR;              
	EPwm5Regs.AQCTLB.bit.CBD 		= AQ_SET;
   
	EPwm5Regs.DBCTL.bit.OUT_MODE 	= DB_DISABLE;
	EPwm5Regs.ETSEL.bit.INTEN 		= 0;                 

	EPwm5Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm5Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

 	#if (DUAL_PWM_INTERRUPT)
		EPwm5Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;
		EPwm5Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;
	#else
		EPwm5Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
		EPwm5Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	#endif  

	// Active high complementary PWMs - Setup the deadband
	EPwm5Regs.DBCTL.bit.OUT_MODE = DB_DISABLE;
	EPwm5Regs.ETSEL.bit.INTEN = 0;                  // Enable INT
	EPwm5Regs.DBCTL.bit.POLSEL = DB_ACTV_LOC;
	EPwm5Regs.DBCTL.bit.IN_MODE = DBA_ALL;

}

#pragma CODE_SECTION(PWM_ON_OFF, "ramfuncs");
void PWM_ON_OFF(int On_Off)
{
	if (On_Off)
	{	
		EPwm1Regs.AQCSFRC.bit.CSFA= 0;
		EPwm2Regs.AQCSFRC.bit.CSFA= 0;
		EPwm3Regs.AQCSFRC.bit.CSFA= 0;
		PWM_HARDWARE_ON;
		BOOT_MODE_ON;
		Flag.Monitoring.bit.PWM_ON= 1.;
	}
	else
	{
		PWM_HARDWARE_OFF;
		BOOT_MODE_OFF;		
		EPwm1Regs.CMPA.half.CMPA= 0;
		EPwm2Regs.CMPA.half.CMPA= 0;
		EPwm3Regs.CMPA.half.CMPA= 0;
		EPwm1Regs.AQCSFRC.bit.CSFA= 1; //Forced Low-Value
		EPwm2Regs.AQCSFRC.bit.CSFA= 1; //Forced Low-Value
		EPwm3Regs.AQCSFRC.bit.CSFA= 1; //Forced Low-Value
		Flag.Monitoring.bit.PWM_ON= 0.;
	}
}



int Temperature_Calculation(int  Adc_Temperature)
{

	double C1_NTC, C2_NTC, R_NTC, B_NTC, Temp;
	// [5.5kW ~ 22kW]
	// Vi = 3.3V/3.7k = 0.891892V 
	// Rm = 470 Ohm
	// Iref = 1.8976mA
	// R1=100k, R2=30k, R3=0.47k

	// T_25=298K, R_25=5.0 kOhm
	// T_100=373K, R_100=0.4933kOhm
	// B = (T_25 x T_100) ln (R_100/R_25) / (T_25 - T_100) = 3432.5477

	// 온도 검출 회로의 차동 증폭기에 아래와 같은 결과를 갖도록 
	// 회로가 연결되어 있음.
	// R_NTC^ = (R1+R2+R3)Vo/((R2/R1)(R1+R2)I-Vo)
	// Vo = (u/1024)3.28Vref
	// R_NTC = 400k R_NTC^/(400k-R_NTC^)		: 2 x 200kOhm 과 병렬

	// R_NTC^ = (100+30+0.47) x (u/1024) x 3.28 / {(30/100)(100+30)1.8976-(u/1024)3.28}
	//        = 0.417912 u / (74.0064 - 3.203e-3 u) x 1.0e3 [Ohm]
	//        = C1 x u /(C2 - 3.203e-3 x u) x 1.0e3	
	// C1 = 0.417912		-> tmp_float1
	// C2 = 74.0064	-> tmp_float2
	// R_NTC = 400k R_NTC^ / (400k - R_NTC^)	

	C1_NTC=0.417912;
	C2_NTC=74.0064;
	R_NTC = 5.0;				// 5.0kOhm

	// B=3433 
	B_NTC=3433;

	// ADC valid bit: 10.9 bit -> 9 bit만 사용함, 분해능 8/4096  
	Temp= (double)(Adc_Temperature>>3);
	Temp = (C1_NTC*2*Temp)/(C2_NTC-6.406e-3*Temp);		// 2 x 3.203e-3 -> 6.406e-3
	Temp = (400*Temp) / (400-Temp);				// R_NTC = 400 x R_NTC^ / (400 - R_NTC^)
	
	// tmp_int -> B + T1 ln(R2/R1)
	Temp = B_NTC + (298*log(Temp/R_NTC));
	if (Temp<1)	Temp=1;
		
	// (B x T1)_x10 / [B + T1 ln(R2/R1)] -> (T_x10 - 2730) -> 온도 [degC]		
	Temp = (B_NTC * 2980/Temp)-2730; 			// x10 : (25 + 273)k x 10 -> 2980
	
	// -25도 ~ 150도
	if (Temp>1500)								
			Temp=1500;
	else if (Temp<-250)
			Temp=-250;
	
	return Temp;
}

// end of file

