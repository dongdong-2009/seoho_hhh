/********************************************************


*********************************************************/
#define _MAIN_

#include "main_def.h"
#include "math.h"


float F_ref = 0.;
#pragma DATA_SECTION(ZONE0_BUF,"ZONE0DATA");

volatile unsigned int ZONE0_BUF[2048];

int main_loop_cnt = 0;

extern void Relay_setup();

void GetAnaMonitCount(unsigned int * piChA, unsigned * piChB);

int flag_relay = 0;
Uint16 data1=0;
Uint16 EEPROM_WRITE_CHECK = 0;
Uint16 addr_ch = 0;

void main(void)
{
	int itmp_cnt;
	unsigned int i;

    InitSysCtrl();

    DINT;

    InitPieCtrl();

    IER = 0x0000;
    IFR = 0x0000;

    InitPieVectTable();
	Relay_setup();
  	Init_dsc();

	MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);

	// Call Flash Initialization to setup flash waitstates
	// This function must reside in RAM
	InitFlash();

	nRESET_DRIVER_SET;	//316J PWM on

//	nBOOT_MODE_SET;		////	nPWM_ENABLE_SET;	//
	nDC_CONTACT_SET;

	// Initialize SCI-A for data monitoring 
	sci_debug_init();


	// Initialize CAN-A/B
	init_can();

	// Initialize SCI-B, SCI-C
	scib_init();
	scic_init();

	ZONE0_BUF[0x0060] = 0x0006; //h DAC_reset on
	asm ("      nop");
	ZONE0_BUF[0x0060] = 0x0007; // DAC_reset off
	asm ("      nop");			

//	calculateOffset();

	nDC_CONTACT_CLEAR;
//	delay(1000000); //delay_msecs(100);		// Delay for Setting Time

	func_flag.all = 0;

//======================================================
	for(itmp_cnt=0 ; itmp_cnt<3000 ; itmp_cnt++) 
	{
		Comm_array[itmp_cnt] = 1234 ;
	}
//======================================================

 //   Init_I2C_eeprom();
    delay_ms(1);

// test eeprom
//	Word_Write_data(100, 15000);
//	Word_Read_data (100, &data1); 

//	Word_Read_data(0, &EEPROM_WRITE_CHECK);
//	if(EEPROM_WRITE_CHECK != EEPROM_WRITED) {EEPROM_DATA_INIT();}//

	EINT;   // Enable Global interrupt INTM
	ERTM;   // Enable Global realtime interrupt DBGM 
	//Word_Write_data(2106, 0);	// 초기 시 Run_stop 를 항상 stop로 만든다. 2010.06.16
//	eeprom2sys();
	
	while(1) {
/*
	//==========================
//	Word_Read_data(addr_ch, &data1);
	//Serial_Comm_Service();
	Update_state();
	digital_input_proc ();
	digital_out_proc (); 
	Get_Monitor();
	Is_mag = sqrt(Idss * Idss + Iqss * Iqss);
	F_ref = (Base_spd + AICMD)/60.;
	if(F_ref < 400.) F_ref = 400.;

	if(flag.control == 1)
	{
		if(fault_chk() != 0) TripProc();
	}

		Vmag_ref = Vdse_ref * Vdse_ref + Vqse_ref * Vqse_ref;
		Vs_mag = sqrt(Vmag_ref);
		Vmag_ffa = Vdse_ref_ffa * Vdse_ref_ffa + Vqse_ref_ffa * Vqse_ref_ffa;
		Vmag_delta = Vmag_ref - Vmag_ffa;
		Vmag_delta_est = Wrpm_hat/(35.02-Wrpm_hat*3.67/10000);
		Update_var();
*/
//Data_Registers[0]=0xABCD;
//SCI_Registers[0]=0x0000;

	//cana_Tx_process();
	//cana_Rx_process();

	SCIC_Process();//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11111

//delay(30000);
//scic_putc(0x55);
//scic_TxChar(0x55);
		
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


	if(Machine_state == RUNN)
	{
		flag.GT1_ENA = 1;
//	Speed_ref = (float)(Comm_array[2102]);
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
	if (Machine_state == STOP)
	{
		Init_var();
//		if(Wrpm_hat > 3000.) Wrpm_set_user = 0.0;
//		else 
//		{
//			Init_var();
//			tmp_cnt = 0;
//		}
	}
	else
	{
		Speed_ref = (float)(Comm_array[2102]);		
	}
	// Update control variables
	Wc_cc = 6.283184 * Cur_Ctrl_B;		// CC BW : 2*PI*fc = 200[Hz]
	Kp_cc = Wc_cc * Lds * (float)(Cur_Ctrl_Kp)/100.;
	Ki_ccT = Wc_cc * Rstator * Tsamp * (float)(Cur_Ctrl_Ki)/100.;

	a = 2*PI*Wrm_L.fc;
	Wrm_L.Tsamp = 0.002;
//	Wrm_L.La = (2. - a*Wrm_L.Tsamp)/(2. + a*Wrm_L.Tsamp);		// 무사용 2010.06.24
//	Wrm_L.Lb = a*Wrm_L.Tsamp / (2. + a*Wrm_L.Tsamp);			// 무사용 2010.06.24

	faultIas_OC = fault.Ias_OC;
	faultIbs_OC = fault.Ibs_OC;
	faultIcs_OC = fault.Ics_OC;
	faultFT_SWPROT_Vdc_UV = fault.FT_SWPROT_Vdc_UV;

	if(op_cnt == 100) { op_cnt = 0;	}
	else { op_cnt++; flag.control = 0.; }


	flagGT1_ENA = flag.GT1_ENA;
	DRIVE_ENABLE = func_flag.bit.DRIVE_ENABLE;
	MULTI_STEP_BIT0 = func_flag.bit.MULTI_STEP_BIT0;
	MULTI_STEP_BIT1 = func_flag.bit.MULTI_STEP_BIT1;
	MULTI_STEP_BIT2 = func_flag.bit.MULTI_STEP_BIT2;
	MULTI_STEP_BIT3 = func_flag.bit.MULTI_STEP_BIT3;
	FAULT_RESET = func_flag.bit.FAULT_RESET;
	JOG = func_flag.bit.JOG;
}


void Update_state()
{
	switch(Run_Stop_meth)
	{
		case 0	: 
		{
			if((func_flag.bit.DRIVE_ENABLE == 1) && (func_flag.bit.MULTI_STEP_BIT0 == 0))	Machine_state = RUNN;
			else if((func_flag.bit.DRIVE_ENABLE==0)||(func_flag.bit.MULTI_STEP_BIT0 == 1))	Machine_state = STOP;
		}
			break ;

		case 1	:
		{
			if(Run_Stop == 255)	Machine_state = RUNN;
			else				Machine_state = STOP ;
		}
			break ;
	}
	if (Local_Remote == 1)	// 속도 지령을 AI로 조절 
	{
		if((flag_speed_cmd == 1)&&(Machine_state == RUNN))
		{
			Wrpm_set_user = Base_spd + AICMD;
			if(Wrpm_set_user >= 60000.) Wrpm_set_user = 60000.;
//			if(Wrpm_set_user <= Base_spd ) Wrpm_set_user = Base_spd;
			if(Wrpm_set_user <= 24000. ) Wrpm_set_user = 24000.;	// Aentl
		}

	}
	else					// 속도 지령을 KeyPad로 조절
	{
		if((flag_speed_cmd == 1)&&(Machine_state == RUNN))
		{
			Wrpm_set_user = Speed_ref;
			if(Wrpm_set_user >= 60000.) Wrpm_set_user = 60000.;
			if(Wrpm_set_user <= Base_spd) Wrpm_set_user = Base_spd;
		}
	}
}

void Get_Monitor()
{
	Comm_array[2110] = (unsigned int)(Wrpm_hat);
	Comm_array[2111] = (unsigned int)(F_ref * 10);
	Comm_array[2112] = (unsigned int)(Vdc_measured*10.);
	Comm_array[2113] = (unsigned int)(Is_mag/0.1414);
	Comm_array[2114] = (unsigned int)(Vs_mag/0.1414);
}


