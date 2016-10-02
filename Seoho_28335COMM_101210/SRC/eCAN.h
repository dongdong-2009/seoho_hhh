/*****************************************************************************
*
* File Name: SCI_BC.H
*
* Description: Header file for SCI_B, SCI_C driver
*
* Modules Included:
*
*****************************************************************************/


#ifndef __eCAN_H
#define __eCAN_H

//#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
//#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

//#include "DSK2833x_Define.h"
//#include "main_def.h"

interrupt void ecan0_inta_isr(void);
interrupt void ecan1_inta_isr(void);
interrupt void ecan0_intb_isr(void);
interrupt void ecan1_intb_isr(void);

//통신 데이터 어레이
extern	WORD Data_Registers[Buf_MAX];
extern WORD CAN_Registers[Buf_MAX];

void SendDataToECanA(LONG id, BYTE length,  LONG high_data,LONG low_data);

void cana_Tx_process(void);
void cana_Rx_process(void);
#endif



