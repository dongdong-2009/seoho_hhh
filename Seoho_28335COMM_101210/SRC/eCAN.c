//###############################################################################################################
// ���ϸ�   : eCAN.c
// ȸ���   : RealSYS
// �ۼ���¥ : 2008.10.20
// �ۼ���   : R.N.K
// ����     : F28335�� ���� CAN-A/B �ֺ���ġ ���� ���� �ڵ�
//
// [ ����� ���� ȯ�� ]
// 1) TI CCS3.3�� Service Pack v11.0(CCS_v3.3_SR11_81.6.2.exe ����) - 2008.10.20
// 2) CCS3.3 Setup�� Target Driver Package(setupCCSPlatinum_v30329.exe ����) - 2008.10.20
// 3) C2000 Code Generation Tools v5.1.1 Release(C2000CodeGenerationTools5.1.1.exe ����) - 2008.10.20
//
// [ ������ Software Developer Kit(SDK) ]
// 1) TI�� SDK�� C/C++ Header Files and Peripheral Examples v1.20 ��� - 2008.10.20
//
// [ Revision History ]
//
//###############################################################################################################

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

#include "sci_bc.h"
#include "eCAN.h"

// ������ ���������� ���ͷ�Ʈ ���� ��ƾ�� "ramfuncs" �̶�� ���ǿ� �Ҵ��ϰ�
// "ramfuncs" ������ ���� ���� ó���� ���� ���� RAM���� �����ϵ��� ��
// eCAN-A/B Interrupt Service Function ����
#pragma CODE_SECTION(ecan0_inta_isr, "ramfuncs");
#pragma CODE_SECTION(ecan1_inta_isr, "ramfuncs");
#pragma CODE_SECTION(ecan0_intb_isr, "ramfuncs");
#pragma CODE_SECTION(ecan1_intb_isr, "ramfuncs");




// eCAN-A/B ó���� ���� ���� ------------------------------------------------
BOOL cana_rx_flag=FALSE;	// CAN-A�� ���ο� ���� �����͸� �޾Ҵ��� ����
LONG cana_rx_id=0;			// CAN-A ���� ID
BYTE cana_rx_length=0;    	// CAN-A ���� ������ ũ��
LONG cana_rx_low_data=0;	// CAN-A ���� Low Word Data
LONG cana_rx_high_data=0;	// CAN-A ���� High Word Data

BOOL canb_rx_flag=FALSE;	// CAN-B�� ���ο� ���� �����͸� �޾Ҵ��� ����
LONG canb_rx_id=0;			// CAN-B ���� ID
BYTE canb_rx_length=0;    	// CAN-B ���� ������ ũ��
LONG canb_rx_low_data=0;	// CAN-B ���� Low Word Data
LONG canb_rx_high_data=0;	// CAN-B ���� High Word Data
//----------------------------------------------------------------------------


WORD CAN_Registers[Buf_MAX];

#ifndef __SCI_BC_H
	WORD Data_Registers[Buf_MAX];
#endif
WORD CAN_TxOffset=0;
WORD test=0;
void cana_Tx_process(void)
{
	if(Data_Registers[CAN_TxOffset] != CAN_Registers[CAN_TxOffset])
	{
		if(!ECanaRegs.CANTRS.bit.TRS1)
		{
			test++;

			SendDataToECanA(0x1L, 0x08, ((LONG)Data_Registers[CAN_TxOffset] & 0x0000FFFF), ((LONG)CAN_TxOffset & 0x0000FFFF));
			//SendDataToECanA(0x1L, 0x08, ((LONG)reg_TxOffset & 0x0000FFFF), ((LONG)reg_TxOffset & 0x0000FFFF));

			//while(!ECanaRegs.CANTA.bit.TA1);///scic ��ſ� ������ ��ħ
			
			// Clear transmit-acknowledge pending flag
			ECanaRegs.CANTA.bit.TA1 = 1;	

			CAN_Registers[CAN_TxOffset] = Data_Registers[CAN_TxOffset];
			CAN_TxOffset ++;	
		}
	}

	
	if(Buf_MAX <= CAN_TxOffset) CAN_TxOffset = 0;
}



void cana_Rx_process(void)
{
      if(cana_rx_flag)
      	{
      	       Data_Registers[cana_rx_low_data & 0x0000FFFF] = (WORD)(cana_rx_high_data & 0x0000FFFF);
		CAN_Registers[cana_rx_low_data & 0x0000FFFF] = (WORD)(cana_rx_high_data & 0x0000FFFF);
    		switch(cana_rx_low_data & 0x0000FFFF)
		{
			case 0 : P_rate = 100 * (float)( Comm_array[40] = Data_Registers[0]); break;
    		}

		cana_rx_flag = 0;
      	}

}



// CAN-A MBOX1�� ����Ͽ� �޽����� ������
void SendDataToECanA(LONG id, BYTE length,  LONG high_data,LONG low_data){
	// ���� CAN-A MBOX1�� ���� ���̸� ������.
	if(ECanaRegs.CANTRS.bit.TRS1 == 1) return;
	
	ECanaRegs.CANME.bit.ME1 = 0;
	ECanaMboxes.MBOX1.MSGID.all = 0x00000000 | (id & 0x1fffffff);
	ECanaRegs.CANME.bit.ME1 = 1;

	ECanaMboxes.MBOX1.MSGCTRL.bit.DLC = length & 0xf;
	ECanaMboxes.MBOX1.MDL.all = low_data;
	ECanaMboxes.MBOX1.MDH.all = high_data;
	
	ECanaRegs.CANTRS.bit.TRS1 = 1;
}

// CAN-B MBOX1�� ����Ͽ� �޽����� ������
void SendDataToECanB(LONG id, BYTE length,  LONG high_data,LONG low_data){
	// ���� CAN-B MBOX1�� ���� ���̸� ������.
	if(ECanbRegs.CANTRS.bit.TRS1 == 1) return;
	
	ECanbRegs.CANME.bit.ME1 = 0;
	ECanbMboxes.MBOX1.MSGID.all = 0x80000000 | (id & 0x1fffffff);
	ECanbRegs.CANME.bit.ME1 = 1;
	
	ECanbMboxes.MBOX1.MDL.all = low_data;
	ECanbMboxes.MBOX1.MDH.all = high_data;
	ECanbMboxes.MBOX1.MSGCTRL.bit.DLC = length & 0xf;
	
	ECanbRegs.CANTRS.bit.TRS1 = 1;
}

void init_can(void){
	// Initialize CAN-A/B peripheral with 1M Bps
	// refer to DSP2833x_ECan.c file
	InitECanGpio();
	InitECan();
	
	// Initialize CAN-A Transmiter/Receiver MailBox
	// CAN-A MBOX0 -> Receiver MailBox
	// CAN-A MBOX1 -> Transmiter MailBox
	ECanaLAMRegs.LAM0.all = 0x1fffffff; 	  // MBOX1 acceptance mask(IDE match, IDs is don't care)
	ECanaMboxes.MBOX0.MSGID.all = 0x40000000; // IDE=1, AME=1
	
	ECanaRegs.CANME.bit.ME0 = 1;
	
	ECanaRegs.CANMD.bit.MD0 = 1; // Receiver
	ECanaRegs.CANMD.bit.MD1 = 0; // Transmiter
	
	// Initialize CAN-B Transmiter/Receiver MailBox
	// CAN-B MBOX0 -> Receiver MailBox
	// CAN-B MBOX1 -> Transmiter MailBox
	ECanbLAMRegs.LAM0.all = 0x1fffffff; 	  // MBOX1 acceptance mask(IDE match, IDs is don't care)
	ECanbMboxes.MBOX0.MSGID.all = 0xc0000000; // IDE=1, AME=1
	
	ECanbRegs.CANME.bit.ME0 = 1;
	
	ECanbRegs.CANMD.bit.MD0 = 1; // Receiver
	ECanbRegs.CANMD.bit.MD1 = 0; // Transmiter
	
	// Initialize CAN-A/B interrupt vector
	EALLOW;
	PieVectTable.ECAN0INTA = &ecan0_inta_isr;
	//PieVectTable.ECAN1INTA = &ecan1_inta_isr;
	//PieVectTable.ECAN0INTB = &ecan0_intb_isr;
	//PieVectTable.ECAN1INTB = &ecan1_intb_isr;
	EDIS;

	EALLOW;
	// Initialize CAN-A/B Receiver and Transmiter MailBox interrupt level
	// CAN-A/B Receiver interrupt level   -> 0
	// CAN-A/B Transmiter interrupt level -> 1
	ECanaRegs.CANMIL.bit.MIL0 = 0;
//	ECanaRegs.CANMIL.bit.MIL1 = 1;
//	ECanbRegs.CANMIL.bit.MIL0 = 0;
//	ECanbRegs.CANMIL.bit.MIL1 = 1;

	// Initialize CAN-A/B Receiver and Transmiter MailBox interrupt mask
	ECanaRegs.CANMIM.bit.MIM0 = 1;
//	ECanaRegs.CANMIM.bit.MIM1 = 1;
//	ECanbRegs.CANMIM.bit.MIM0 = 1;
//	ECanbRegs.CANMIM.bit.MIM1 = 1;
	
	// Initialize CAN-A/B Global interrupt mask
	ECanaRegs.CANGIM.bit.I0EN = 1;
//	ECanaRegs.CANGIM.bit.I1EN = 1;
//	ECanbRegs.CANGIM.bit.I0EN = 1;
//	ECanbRegs.CANGIM.bit.I1EN = 1;
	EDIS;

	// Enable CPU INT9 for CAN-A/B interrupt
	IER |= M_INT9;
	
    // Enable CAN-A/B INT0/1 in the PIE: Group 9 interrupt 5/6/7/8
	PieCtrlRegs.PIEIER9.bit.INTx5 = 1;
	//PieCtrlRegs.PIEIER9.bit.INTx6 = 1;
	//PieCtrlRegs.PIEIER9.bit.INTx7 = 1;
	//PieCtrlRegs.PIEIER9.bit.INTx8 = 1;

	for(i=0;i<Buf_MAX;i++)//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11111
	{
		SCI_Registers[i]=0x0000;
	}
}

// CAN-A MBOX0 ���� �޽��� ó���� ���� ���ͷ�Ʈ
interrupt void ecan0_inta_isr(void){
	if(ECanaRegs.CANRMP.bit.RMP0 == 1){
			cana_rx_id = ECanaMboxes.MBOX0.MSGID.all & 0x1fffffff;
			cana_rx_length = ECanaMboxes.MBOX0.MSGCTRL.bit.DLC;
			cana_rx_low_data = ECanaMboxes.MBOX0.MDL.all;
			cana_rx_high_data = ECanaMboxes.MBOX0.MDH.all;

			//if(Data_Registers[cana_rx_low_data & 0x0000FFFF] != (WORD)(cana_rx_high_data & 0x0000FFFF))
			//{
			//	Data_Registers[cana_rx_low_data & 0x0000FFFF] = (WORD)(cana_rx_high_data & 0x0000FFFF);
			//}
			
			cana_rx_flag = TRUE;

		
		// Clear receive pending flag
		ECanaRegs.CANRMP.bit.RMP0 = 1;	
	}	

	// Acknowledge this interrupt to receive more interrupts from group 9
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

// CAN-A MBOX1 �۽� �޽��� ó���� ���� ���ͷ�Ʈ
interrupt void ecan1_inta_isr(void){
	if(ECanaRegs.CANTA.bit.TA1 == 1){
		
		// Clear transmit-acknowledge pending flag
		ECanaRegs.CANTA.bit.TA1 = 1;
	}

	// Acknowledge this interrupt to receive more interrupts from group 9
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}










// CAN-B MBOX0 ���� �޽��� ó���� ���� ���ͷ�Ʈ
interrupt void ecan0_intb_isr(void){
	if(ECanbRegs.CANRMP.bit.RMP0 == 1){
		if(!canb_rx_flag){
			canb_rx_id = ECanbMboxes.MBOX0.MSGID.all & 0x1fffffff;
			canb_rx_length = ECanbMboxes.MBOX0.MSGCTRL.bit.DLC;
			canb_rx_low_data = ECanbMboxes.MBOX0.MDL.all;
			canb_rx_high_data = ECanbMboxes.MBOX0.MDH.all;
			canb_rx_flag = TRUE;
		}
		
		// Clear receive pending flag
		ECanbRegs.CANRMP.bit.RMP0 = 1;	
	}

	// Acknowledge this interrupt to receive more interrupts from group 9
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

// CAN-B MBOX1 �۽� �޽��� ó���� ���� ���ͷ�Ʈ
interrupt void ecan1_intb_isr(void){
	if(ECanbRegs.CANTA.bit.TA1 == 1){
		
		// Clear transmit-acknowledge pending flag
		ECanbRegs.CANTA.bit.TA1 = 1;
	}

	// Acknowledge this interrupt to receive more interrupts from group 9
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

