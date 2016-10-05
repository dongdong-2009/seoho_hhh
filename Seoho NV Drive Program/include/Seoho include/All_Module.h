#ifndef		__FUNCTION_MODULE_DEFINITION_
#define		__FUNCTION_MODULE_DEFINITION_

interrupt void MainPWM(void);					// PWM Interrrupt
interrupt void Initial_Driver_Calibration(void);
extern void Drive_Calibration(); 		

extern void F28335PowerOnSet();					// DSP 초기 설정 함수
extern void EPWM_Initialization();  			// PWM 초기설정

extern void InitXintf();
extern void sci_debug_init(void); // by RYU		// SCI 통신 Realsys monitering 
extern void	scib_init();			// Initialize SCI-B, SCI-C
extern void scic_init();

extern void EQEP_Initialization();                                              
extern void Speed_Detection_Enc(); 
extern void Speed_Detection_Est();
extern int Temperature_Calculation(int  Adc_Temperature);			

extern void State_Management();					// 인버터의 현재 상태를 관리 
		
extern void VF_Controller();
extern void Flux_Estimation();
extern void Current_Controller();
extern void SVPWM();
extern void Field_Weakening();
extern void Flux_Controller();
extern void Speed_Controller();
extern void Auto_Tuning();
extern void Auto_Tuning_China();
extern void matrix_multiplic(int rowBorcolumnA,int rowA,int columnB,float *pA,float *pB, float *pC);//pC=pA*pB
extern void matrix_subtract(int row,int column,float *pA,float *pB,float *pC);//pC=pA-pB 

extern void PWM_ON_OFF(int On_Off);				// PWM ON/OFF (Software & Hardware)
extern void DB_Controller();					// DB Chopper Control

// extern void DAC_Setup();						
extern void GetSensorValue( );					// ADC Sensing
extern void System_Variable_Initialization();				// 초기 변수값 설정
extern void System_Variable_Update();
extern void PWM_Frequency_Update();
extern void	Parameter_Initialization_by_Power();
extern void	Parameter_Initialization();

extern void Fault_Processing();					// Fault 발생시 상태 관리
extern void Fault_Check_1Sampling();
extern void Fault_Check_4Sampling();
extern void Fault_Recording();
extern void	RW_Fault_Record();

extern void Get_Command(int * cmd, double * ref );		// 외부입력 관리
extern void Reference_Function();		
extern void Break_Speed_Up_Controller();
extern void Break_Speed_Down_Controller();							
extern void DO_Processing();
extern void	DI_Processing( int * cmd, double * ref );  
extern void AO_Processing();
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


//========================================================
//	start 2010-0830
//========================================================
#endif


