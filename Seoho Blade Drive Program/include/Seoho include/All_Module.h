#ifndef		__FUNCTION_MODULE_DEFINITION_
#define		__FUNCTION_MODULE_DEFINITION_

#include "Core_DSP2833x_Device.h"     // Headerfile Include File
#include "Core_DSP2833x_Examples.h"

interrupt void MainPWM(void);					// PWM Interrrupt
interrupt void Driver_Calibration(void); 		

extern void F28335PowerOnSet();					// DSP �ʱ� ���� �Լ�
extern void EPWM_Initialization();  			// PWM �ʱ⼳��

extern void InitXintf();
extern void sci_debug_init(void); // by RYU		// SCI ��� Realsys monitering 
extern void	scib_init();			// Initialize SCI-B, SCI-C
extern void scic_init();

extern void EQEP_Initialization();                                              
extern void Speed_Detection(); 
extern int Temperature_Calculation(int  Adc_Temperature);			

extern void State_Management();					// �ι����� ���� ���¸� ���� 
		
extern void VF_Controller();
extern void Flux_Estimation();
extern void Current_Controller();
extern void SVPWM();
extern void Field_Weakening();
extern void Flux_Controller();
extern void Speed_Controller();
extern void Position_Controller();
extern void Auto_Tuning();
extern void Auto_Tuning_China();
extern void matrix_multiplic(int rowBorcolumnA,int rowA,int columnB,float *pA,float *pB, float *pC);//pC=pA*pB
extern void matrix_subtract(int row,int column,float *pA,float *pB,float *pC);//pC=pA-pB 

extern void PWM_ON_OFF(int On_Off);				// PWM ON/OFF (Software & Hardware)
extern void DB_Controller();					// DB Chopper Control

// extern void DAC_Setup();						
extern void GetSensorValue( );					// ADC Sensing
extern void System_Variable_Initialization();				// �ʱ� ������ ����
extern void System_Variable_Update();
extern void PWM_Frequency_Update();
extern void	Parameter_Initialization_by_Power();
extern void	Parameter_Initialization();

extern void Fault_Processing();					// Fault �߻��� ���� ����
extern void Fault_Check_1Sampling();
extern void Fault_Check_4Sampling();


extern void Get_Command(int * cmd, double * ref );		// �ܺ��Է� ����
extern void Reference_Function();									
extern void DO_Processing();
extern void	DI_Processing( int * cmd, double * ref );  
extern void AO_Processing( unsigned int * count);
extern void AI_Processing(double * ref ); 
extern void Local_Processing(int * cmd, double * ref );
extern void Fieldbus_Processing( int * cmd, double * ref );

extern void Read_Data_Registers(int Parameter_index);
extern void Write_Data_Registers_Offline(int Parameter_index); 
extern void Write_Data_Registers_Online();
extern void SCIC_Process();

extern void I2C_EEPROM_Initialization();
extern void Word_Read_data();
extern void Word_Write_data();

// extern	int motor_name_plate_check();

//TripProc.c
extern void Fault_Recording(int Fault_code,double Fault_data,char * st);


//========================================================
//	start 2010-0830
//========================================================


extern double CheckPowerVersion( );

//========================================================
//	For Blade Pitch Control 
//    by chy
//========================================================

extern void Speed_Detection_ABS (); 

interrupt void Mcbsp_TxINTA_ISR(void);       // chy added 2010.10.25 Mcbsp TX interrupt
interrupt void Mcbsp_RxINTA_ISR(void);          // chy added 2010.10.25 Mcbsp RX interrupt

extern void mcbsp_xmit(int a, int b);

// Postion

extern void DecimalToBinary(Uint16 val, Uint16 bits, Uint16 binArr[]); // chy
extern void GrayToBinary(Uint16 N, Uint16 binArr[], Uint16 conG2B[]);  // chy
extern void BinaryToDecimal(Uint16 N, Uint16 conG2B[]); // chy

extern int square(int cnt);

// ryu
// CANopen
//extern void Read_CAN_Registers(int Parameter_index);
extern void Read_CAN_Registers(void);
extern void Write_CAN_Registers(void);

#endif


