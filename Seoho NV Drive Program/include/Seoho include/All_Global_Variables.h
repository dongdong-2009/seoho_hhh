#ifndef		__GLOBAL_VARIABLES_
#define		__GLOBAL_VARIABLES_

//------------- Core Variables Start -------------//

//--------Motor Parameters--------// 절대 수정 금지 
double Rs=2.0, Rr=0.9, Lsigma=12e-3, Lm=154e-3;
double Jm=0.005, inv_Jm=200, Pole_pair=2., Bm= 0.; 
double LAMdre_rate = 0.45, LAMdre_ref = 0.45; 
double Lr, Lls= 0., Llr= 0.;
double Ls, Kt; 
double Wrpm_limit=12000., Wrpm_min= 0., Wrpm_max= 1.;
// double Is_rate=(5.5*SQRT2), Wrpm_rated=1435., Wrpm_limit=12000.;
int PPR=1024;
double Tr= 0.;
double Rs0= 0., Ls0= 0., Lm0= 0.;
//------Motor Parameters End------//  

double Temp_HHH1= 0., Temp_HHH2= 0.,Temp_HHH3= 0.;
double Drive_Power_kW;

double Jm_est= 0.;
double Te_rate= 0.;

// int Run_Drive_cal= 1;

double LAMdrs=0., LAMqrs=0.;

double Te_hat= 0.;
double Thetar= 0.;
double Thetae= 0.;
double LAMdrs_cm=0., LAMqrs_cm=0.;



// Sampling Time
double Tsamp= 0.;			
double Tsamp_VF= 0.;	
double Tsamp_CC= 0.;	
double Tsamp_SC= 0.;		
double Tsamp_SD= 0.;		
double Tsamp_FC= 0.;		
Uint32 Period_SD= 0.;	




//입력 
int    Command= 0;
int	   DI_bit_all= 0, DO_bit_all= 0;
double Init_reference= 0.;
double Init_reference_Auto_tuning= 0.;
double Final_reference= 0.; 
double Wrpm_rated= 0.;
double Accel_time= 0.;  
double Accel_time1= 0.;
double Accel_time2= 0.;	
double Decel_time= 0.; 
double Decel_time1= 0.;
double Decel_time2= 0.;	
double Accel_time_Auto_tuning= 0.;	
double Decel_time_Auto_tuning= 0.;	

//전압  
double Vdc= 0., Vdc_max= 0.;
double Vdss_ref_old= 0., Vqss_ref_old= 0.;
double Vdse_ref_fb= 0., Vqse_ref_fb= 0.; // 관측용 변수 
double Vqse_ref_integ= 0.; 
double Vdss_ref= 0., Vqss_ref= 0., Vdse_ref= 0., Vqse_ref= 0.;


//전류 
double Is_rated= 0.;
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
double We= 0., Wr= 0., Wsl= 0.; 
double Wrpm_ref=0.;
double Wrpm_fw1=0., Wrpm_fw2=0.;
double Wrm_det=0., Wrpm_det=0., Wrm_det_flt=0., Wrpm_det_flt= 0.;
double Wrpm_est= 0., Wrpm_mt= 0.; 

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

// 토크
double Te=0.;  

//sin&cos
double Cos_Theta_vtg=1., Sin_Theta_vtg=0.;
double Cos_Theta=1., Sin_Theta=0.; 

//switch
int Driver_ON=0.,Flux_build_up=0.;

//PWM  
double T_dead_tuning= 0.;
Uint16 EPwmPeriodCount= 0.;
unsigned int DutyCount[3]= {0};
double I_DT= 0.;
int PWM_frequency_old= 0.;

// Fault
int Fault_count= 0;
volatile int Fault_bits1= 0, Fault_bits2= 0, Fault_bits3= 0;
double OL_limit= 0.;
double ZSC_limit= 0.;
double Continuous_OL_current= 0.;  
double OC_trip_value= 0.;

//기타 
int Temperature_x10= 0;
int Flag_brake_control= 0;
double Ref_old= 0.;
long Main_counter= 0;
long Time_counter_500ms= 0;
int State_index= 0, State_index_old= 0;
int Break_index= 0;
int Auto_tuning_index= 0;
double Interrupt_time_max= 0.; // us
int Voltage_class= 0;
double LPF[20]= {0.}; 

//------------------auto tuning china

 

PWMGEN pwm1 = PWMGEN_DEFAULTS;
SVGENDQ svgen_dq1 = SVGENDQ_DEFAULTS;


//변수 통신 관련
Uint16 Rx_index= 0;
Uint16 Tx_index= 0;
Uint16 TxIntervalCnt= 0;
Uint16 TxInterval_1s= 0;
int Dummy_comm= 0;
CRC_flg	CRC ;
long Test_count_Rx= 0;
long Test_count_Tx= 0;
int Rx_counter_1s= 0;
int Tx_counter_1s= 0;
long CRC_check_counter= 0;

/* Variables for Serial Communication  */
char scib_tx_buf[SCIB_BUF_SIZE+1];
char scib_tx_pos=0, scib_tx_end=0;
char scib_rx_buf[SCIB_BUF_SIZE+1];

char scib_rxd=' ';

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
double	DA1= 0.,DA1_range= 0.,DA1_offset= 0.,DA1_sign= 0.;
double	DA2= 0.,DA2_range= 0.,DA2_offset= 0.,DA2_sign= 0.;
double	DA3= 0.,DA3_range= 0.,DA3_offset= 0.,DA3_sign= 0.;
double	DA4= 0.,DA4_range= 0.,DA4_offset= 0.,DA4_sign= 0.;
	

FLAG Flag;
PARAMETER P;
OPERATION OP;



// double Err_Wrm_integ= 0., Err_Wrm_integ1= 0.;
//------------- Core Variables End -------------//
#endif

