#include "main_def.h"

void DAC_Setup()
{	
	DA1_val=((int)((DA1-DA1_mid)*0x7FF/(DA1_rng)+0.5)+0x0800)&0x0FFF;
//	DA2_val=((int)((DA2-DA2_mid)*0x7FF/(DA2_rng)+0.5)+0x0800)&0x0FFF;
//	DA3_val=((int)((DA3-DA3_mid)*0x7FF/(DA3_rng)+0.5)+0x0800)&0x0FFF;
//	DA4_val=((int)((DA4-DA4_mid)*0x7FF/(DA4_rng)+0.5)+0x0800)&0x0FFF;

	// Single Buffer ¼öÁ¤Áß 
	ZONE0_BUF[0x0060] = 0x0003; //LDAC_ON, WRITE   

	ZONE0_BUF[0x0030] = DA1_val;
	asm ("      nop");
/*	ZONE0_BUF[0x0031] = DA2_val;
	asm ("      nop");
	ZONE0_BUF[0x0032] = DA3_val;
	asm ("      nop");
	ZONE0_BUF[0x0033] = DA4_val;
	asm ("      nop");
*/
	ZONE0_BUF[0x0060] = 0x0007; //LDAC_ON, WRITE   
	ZONE0_BUF[0x0060] = 0x0005; //LDAC_OFF, READ
	ZONE0_BUF[0x0060] = 0x0007; //LDAC_OFF, READ
}

