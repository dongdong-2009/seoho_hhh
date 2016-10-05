/* ==================================================================================
File name:       F280XPWM.C
                    
Originator:	Digital Control Systems Group
			Texas Instruments

Description:   This file contains source for the PWM  drivers for the F280x
              
Target: TMS320F280x family
              
=====================================================================================
History:
-------------------------------------------------------------------------------------
 04-15-2005	Version 3.20: Using DSP280x v. 1.10 or higher 
------------------------------------------------------------------------------------*/
#include	<All_Header.h>
#include	<All_Extern_Variables.h>
#include "CH_f280xpwm.h"


void F2833X_PWM_Init(PWMGEN *p) 
{       
	p->PeriodMax =	(Uint16)( F_OSC * DSP28_PLLCR / ((double)P.G01.P07_PWM_frequency_x10_kHz*1.e+2) / 2 /2);
	
	EPwm1Regs.TBPRD =  p->PeriodMax;				// Set timer period
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
	
	// Trip Zone One-shot Enable(Fault Driver, DB, OC, OV, ZC)
	// ZC È¸·Î ¿À·ù ÇØ°áÇØ¾ßµÊ 
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
	EPwm2Regs.TBPRD =  p->PeriodMax;				// Set timer period
	EPwm2Regs.TBPHS.half.TBPHS = 0x0000;           	// Phase is 0
	EPwm2Regs.TBCTR = 0x0000;                      	// Clear counter
	EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; 		// Count up
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



	// Trip Zone One-shot Enable(Fault Driver, DB, OC, OV, ZC)
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

	EPwm3Regs.TBPRD =  p->PeriodMax;				// Set timer period
	EPwm3Regs.TBPHS.half.TBPHS = 0x0000;           	// Phase is 0
	EPwm3Regs.TBCTR = 0x0000;                      	// Clear counter
	EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; 		// Count up
	EPwm3Regs.TBCTL.bit.PHSEN = TB_ENABLE; 				// 
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


	// Trip Zone One-shot Enable(Fault Driver, DB, OC, OV, ZC)
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


}
#pragma CODE_SECTION(F2833X_PWM_Update, "ramfuncs");
void F2833X_PWM_Update(PWMGEN *p) 
{
		
		int  m_period;
        unsigned int tmp1;
        unsigned int tmp2;
        unsigned int tmp3;
//		unsigned int tmp4;
		float tmp5;
        p->PeriodMax =	(Uint16)( F_OSC * DSP28_PLLCR / ((double)P.G01.P07_PWM_frequency_x10_kHz*1.e+2) / 2 /2);
		m_period=p->PeriodMax;




	    EPwm1Regs.TBPRD = m_period;
        tmp5 = 0.5 * m_period;
        tmp1 = (unsigned int)((p->Mfunc_c1  + 1)* tmp5);
        EPwm1Regs.CMPA.half.CMPA = EPwmPeriodCount - tmp1;//tmpÓÒÒÆ16±äÎª1/2,È»ºóm_periodÓÒÒÆ1±äÎª1/2
		EPwm1Regs.CMPB = EPwmPeriodCount - tmp1;

		EPwm2Regs.TBPRD = m_period;
        tmp2 = (unsigned int)((p->Mfunc_c2  + 1)* tmp5);
		EPwm2Regs.CMPA.half.CMPA = EPwmPeriodCount - tmp2;
		EPwm2Regs.CMPB = EPwmPeriodCount - tmp2;

		EPwm3Regs.TBPRD = m_period;
        tmp3 = (unsigned int)((p->Mfunc_c3  + 1)* tmp5);
		EPwm3Regs.CMPA.half.CMPA = EPwmPeriodCount - tmp3;
		EPwm3Regs.CMPB = EPwmPeriodCount - tmp3;

        
}






