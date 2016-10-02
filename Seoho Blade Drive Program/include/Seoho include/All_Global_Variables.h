#ifndef		__GLOBAL_VARIABLES_
#define		__GLOBAL_VARIABLES_

//------------- Core Variables Start -------------//

//--------Motor Parameters--------// 절대 수정 금지 
double Rs=2.0, Rr=0.9, Lsigma=12e-3, Lm=154e-3;
double Jm=0.005, inv_Jm=200, Pole_pair=2., Bm= 0.; 
double LAMdre_rate = 0.45, LAMdre_ref = 0.45; 
double Lr, Lls= 0., Llr= 0.;
double Ls, Kt; 
double Wrpm_max=3000.;
// double Is_rate=(5.5*SQRT2), Wrpm_rate=1435., Wrpm_max=12000.;
int PPR=1024;
double Tr= 0.;
double Rs0= 0., Ls0= 0., Lm0= 0.;
//------Motor Parameters End------//  

double Temp_HHH1, Temp_HHH2,Temp_HHH3;
double Drive_Power_kW;
double LAMdre_cm= 0., LAMqre= 0.;
double Wrpm_ABS= 0., Wrpm_est= 0., Tl_est= 0.;
double Vdss=0., Vqss=0.; 



double Jm_est= 0.;
double Te_rate= 0.;

double LAMdrs_cm= 0., LAMqrs_cm= 0.;
double LAMdrs_vm= 0., LAMqrs_vm= 0.;
double LAMdrs=0., LAMqrs=0.;

// Sampling Time
double Tsamp= 0.;			
double Tsamp_VF= 0.;	
double Tsamp_CC= 0.;	
double Tsamp_SC= 0.;		
double Tsamp_SD= 0.;		
double Tsamp_FC= 0.;		
Uint32 Period_SD= 0.;	




//입력 
int    Command= 0.;
double Init_reference= 0.;
double Init_reference_Auto_tuning= 0.;
double Final_reference= 0.; 
double Wrpm_scale= 0.;
double Accel_time= 0.;	
double Decel_time= 0.;	
double Accel_time_Auto_tuning= 0.;	
double Decel_time_Auto_tuning= 0.;	

//전압  
double Vdc= 0., Vdc_max= 0.;
double Vdss_ref_old= 0., Vqss_ref_old= 0.;
double Vdse_ref_fb= 0., Vqse_ref_fb= 0.; // 관측용 변수 
double Vqse_ref_integ= 0.; 
double Vdss_ref= 0., Vqss_ref= 0., Vdse_ref= 0., Vqse_ref= 0.;


//전류 
double IGBT_current= 0.;
double I_scale= 0.;
double Ias=0., Ibs=0., Ics=0.;
double Ias_offset=0., Ibs_offset=0.;
double Ias_ref=0., Ibs_ref=0., Ics_ref=0.;  
double Is_max=0., Is_mag=0., Is_mag_rms=0.;
double Idss=0., Iqss=0., Idse=0., Iqse=0.; 
double Idss_ref=0., Iqss_ref=0., Idse_ref=0., Iqse_ref=0.;
double Idse_ref_max= 0., Iqse_max_fw=0.;
double Iqse_ref_max= 0.;
	

//자속 
double LAMdre=0.;
double LAMdre_flt=0.; //관측용 변수 


//속도 & 주파수 
double freq_vf=0., freq_vf_set=0.;
double We = 0.; 
double Wr=0.;
double Wrpm_ref=0.;
double Wrpm_fw1=0., Wrpm_fw2=0.;
double Wrm_det=0., Wrpm_det=0., Wrm_det_flt=0.; 

// Power
double Input_power_x10_kW= 0.;
double Output_power_x10_kW= 0.;

//이득
double K_CM1=0., K_CM2=0.;
double K_fw1=0., K_fw2=0.;
double Wc_Fst=0., Kp_Fst=0., Ki_Fst=0.,  Freq_Fst=0.; 
double Wc_fc=0., Kp_fc=0., Ki_fc=0., Ka_fc=0.;
double Wc_sc=0., Kp_sc=0., Ki_sc=0., Ka_sc=0.; 
double Wc_cc=0., Kp_cc=0., Ki_cc=0., Ka_cc=0.;
double L1_sp_est= 0., L2_sp_est= 0., L3_sp_est= 0., L3_sp_tmp= 0.;
double Kob_sd= 0.;

//위상 
double Theta=0.; 
double Thetar=0.;
double Theta_ref= 0.;
double Theta_angle_ref=0., Theta_angle_err=0., Theta_angle_flt=0.;
// 토크
double Te=0.;  

//sin&cos
double Cos_Theta_vtg=1., Sin_Theta_vtg=0.;
double Cos_Theta=1., Sin_Theta=0.; 

//switch
int Driver_ON=0.,Flux_build_up=0.;

//PWM  
double T_dead_Tuning= 0.;
Uint16 EPwmPeriodCount= 0.;
unsigned int DutyCount[3];
double I_DT= 0.;
int PWM_frequency_old= 0.;

// Fault
int Fault_count= 0;
double OL_limit= 0.;
double Continuous_OL_current= 0.;  
double OC_trip_value= 0.;

//기타 
int Temperature_x10= 0.;
long Main_counter= 0;
int State_Index= 0;
int Auto_tuning_index= 0;
double Interrupt_time_max= 0.;
double Interrupt_time_max_enc= 0.;
// double Interrupt_time_max_realsys= 0.;
double Interrupt_time_max_op= 0.;

int Voltage_class= 0;

//------------------auto tuning china

 

PWMGEN pwm1 = PWMGEN_DEFAULTS;
SVGENDQ svgen_dq1 = SVGENDQ_DEFAULTS;


//변수 통신 관련
Uint16 Rx_index= 0;
Uint16 Tx_index= 0;
Uint16 Tx_count_25ms= 0;
Uint16 Tx_count_1s= 0;
int Dummy_comm= 0;
CRC_flg	CRC ;

/* Variables for Serial Communication  */
//char scib_tx_buf[SCIB_BUF_SIZE+1];
//char scib_tx_pos=0, scib_tx_end=0;
//char scib_rx_buf[SCIB_BUF_SIZE+1];

//char scib_rxd=' ';

char scic_tx_buf[SCIC_BUF_SIZE+1];
char scic_tx_pos=0, scic_tx_end=0;
char scic_rx_buf[SCIC_BUF_SIZE+1];

char scic_rxd=' '; 

//-- Serial Data Stack  
WORD Data_Registers[Buf_MAX];
WORD CAN_Registers[Buf_MAX];
WORD SCI_Registers[Buf_MAX]; 

// ADC
int	Adc_Uphase;
int	Adc_Vphase;
int	Adc_Wphase;
int	Adc_Vdc	;
int	Adc_AIN_I1; 
int	Adc_AIN_V1;
int	Adc_AIN_I2;
int	Adc_AIN_V2;
int	Adc_AIN_3;
int	Adc_AIN_4;
int	Adc_AIN_5 ;
int	Adc_Temperature;
int	Adc_Vclass; 
 
//====================================
// F28335 
// DAC Parameter
//==================================== 
double	DA1,DA1_rng,DA1_offset;
double	DA2,DA2_rng,DA2_offset;
double	DA3,DA3_rng,DA3_offset;
double	DA4,DA4_rng,DA4_offset;
	

FAULT_INFO FaultInfo = {0,0,0,0,"2008:07:24  00:01:23"," No Trip Data       ",0};

FLAG Flag;
PARAMETER P;
OPERATION OP;
//------------- Core Variables End -------------//

//=======================================
// For Blade Pitch Control
// by chy
//=======================================


// communication
Uint16 tx_complete = 0;
Uint16 rx_complete = 0;
Uint16 rx_int = 0;
Uint16 rdata2=0;
Uint16 rdata1=0;
Uint16 result1;

// position
Uint16 inv_rdata1;
Uint16 inv_rdata2;

Uint16 Steps_buf1;
Uint16 Number_of_turns_buf;
Uint16 Steps_buf2;
Uint16 Steps_per_turn_buf;
Uint16 gNum;
Uint16 bin[SIZE];
Uint16 g2b[SIZE];
Uint16 Steps_per_turn;
Uint16 Number_of_turns;
Uint16 abs_position;

int test1=0, test2=0, test3=0, test4 = 0, test5=0; //, test6=0, test7=0, test8=0, test9=0; // chy
Uint16 test6=0, test7=0, test8=0, test9=0, test10=0;
double etest1=0, etest2=0, etest3=0, etest4=0, etest5=0, etest6=0;

//float etest1=0, etest2=0, etest3=0;
Uint16 com_rdata1[4];
Uint16 com_rdata2[4];
Uint16 step_turn[4]={0,};
Uint16 number_turn[4];
Uint16 abs_pos[4];

// ryu
// CANopen
volatile struct CANOPEN_PARAMETERS CANopenParameters;
UCHAR canInData[128]={0,};
UCHAR canOutData[128]={0,};
#endif

