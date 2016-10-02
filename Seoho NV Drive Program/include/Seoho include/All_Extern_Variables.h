#ifndef		__VARIABLES_
#define		__VARIABLES_

//------------- Core Variables Start -------------//

//--------Motor Parameters--------//
extern double Rs, Rr, Lsigma, Lm;
extern double Jm, inv_Jm, Pole_pair, Bm; 
extern double LAMdre_rate, LAMdre_ref; 
extern double Lr, Lls, Llr;
extern double Ls, Kt; 
extern double Wrpm_limit, Wrpm_min, Wrpm_max;
// extern double Is_rate, Wrpm_rated, Wrpm_max;
extern int PPR;
extern double Tr;
extern double Rs0, Ls0, Lm0;
//------Motor Parameters End------//

extern double Temp_HHH1, Temp_HHH2,Temp_HHH3;
extern double Drive_Power_kW;

extern double Jm_est;
extern double Te_rate;




// extern int Run_Drive_cal;

extern double LAMdrs, LAMqrs;

extern double Thetar;
extern double Thetae;
extern double LAMdrs_cm, LAMqrs_cm;


// Sampling Time
extern double Tsamp;			
extern double Tsamp_VF;	
extern double Tsamp_CC;	
extern double Tsamp_SC;		
extern double Tsamp_SD;		
extern double Tsamp_FC;		
extern Uint32 Period_SD;	
	

//입력 
extern int    Command;
extern int    DI_bit_all, DO_bit_all;
extern double Init_reference;
extern double Init_reference_Auto_tuning;
extern double Final_reference; 
extern double Wrpm_rated;
extern double Accel_time;
extern double Accel_time1;
extern double Accel_time2;
extern double Decel_time;	
extern double Decel_time1;
extern double Decel_time2;	
extern double Accel_time_Auto_tuning;	
extern double Decel_time_Auto_tuning;

//전압  
extern double Vdc, Vdc_max;
extern double Vdss_ref_old, Vqss_ref_old;
extern double Vdse_ref_fb, Vqse_ref_fb; // 관측용 변수 
extern double Vqse_ref_integ;
extern double Vdss_ref, Vqss_ref, Vdse_ref, Vqse_ref;

//전류 
extern double Is_rated;
extern double IGBT_current;
extern double I_scale;
extern double Ias, Ibs, Ics;
extern double Ias_offset, Ibs_offset;
extern double Ias_ref, Ibs_ref, Ics_ref;
extern double Is_max, Is_mag, Is_mag_rms;
extern double Idss, Iqss, Idse, Iqse; 
extern double Idss_ref, Iqss_ref, Idse_ref, Iqse_ref;
extern double Idse_ref_max, Iqse_max_fw;
extern double Iqse_ref_max;

//자속 
extern double LAMdre;
extern double LAMdre_flt; 


//속도 & 주파수 
extern double freq_vf, freq_vf_set;
extern double We, Wr, Wsl; 
extern double Wrpm_ref;
extern double Wrpm_fw1, Wrpm_fw2;
extern double Wrm_det, Wrpm_det, Wrm_det_flt, Wrpm_det_flt; 
extern double Wrpm_est, Wrpm_mt; 

// Power
extern double Input_power_x10_kW;
extern double Output_power_x10_kW;

//이득
extern double K_CM1, K_CM2;
extern double K_fw1, K_fw2;
extern double Wc_Fst, Kp_Fst, Ki_Fst,  Freq_Fst; 
extern double Wc_fc, Kp_fc, Ki_fc, Ka_fc;
extern double Wc_sc, Kp_sc, Ki_sc, Ka_sc; 
extern double Wc_cc, Kp_cc, Ki_cc, Ka_cc;
extern double L1_sp_est, L2_sp_est, L3_sp_est, L3_sp_tmp;
extern double Kob_sd;

//위상 
extern double Theta; 

// 토크
extern double Te;  

//sin&cos
extern double Cos_Theta_vtg, Sin_Theta_vtg;
extern double Cos_Theta, Sin_Theta; 

//switch
extern int Driver_ON,Flux_build_up;

//PWM  
extern double 	T_dead_tuning;
extern Uint16	EPwmPeriodCount;
extern unsigned int DutyCount[3];
extern double I_DT;
extern int PWM_frequency_old;

// Fault
extern int Fault_count;
extern volatile int Fault_bits1, Fault_bits2, Fault_bits3;
extern double OL_limit;
extern double ZSC_limit;
extern double Continuous_OL_current;  
extern double OC_trip_value;

//기타 
extern int Temperature_x10;
extern int Flag_brake_control;
extern long Main_counter;
extern double Ref_old;
extern long Time_counter_500ms;
extern int State_index, State_index_old;
extern int Break_index;
extern int Auto_tuning_index;
extern double Interrupt_time_max;
extern int Voltage_class;
extern volatile unsigned int ZONE0_BUF[2048]; // by RYU

//------------------auto tuning china
extern unsigned long NN;
extern float I_rated;//rated current value of motor nameplate.
extern float exciting_step;
//  variables of thirteen-order exciting signals generator
extern int cs_count;
extern float Hui; //cut off frequency of digital filter
extern float C1;     //the coefficient of digital filter
extern float C2;     //the coefficient of digital filter   
//extern float T;   // Samping period (sec), SWITCH_FREQ=2500. 
extern int cs;             //change exciting signal after 30 sampling periods
////////dead-time compensation//////
extern float Td;// dead-time (ms)
extern float Ton;//1.0/1000; Switching Times of IGBT (ms)
extern float Toff;//2.0/1000;Switching Times of IGBT (ms)
extern float Vs;//2.5; Collector-Emitter Saturation Voltage (v)
extern float Vd;//2.5; FWD Forward Voltage (v)
extern float M;
extern float M1;
/*
extern float K_Vs_slope0; 
extern float K_Vs_slope1;
extern float K_Vd_slope0;
extern float K_Vd_slope1;
*/
// the variables of third-order butterworth digital filter
extern float Wc;// the cutoff frequency of third-order butterworth digital filter (Hz)
extern float a0;//the coefficient of third-order butterworth digital filter transfer function
extern float a1;//the coefficient of third-order butterworth digital filter transfer function
extern float a2;//the coefficient of third-order butterworth digital filter transfer function
extern float A0;
extern float A1;
extern float A2;
extern float predict_weight;//prediction weight
extern float B1;
extern float B2;

extern PWMGEN pwm1;
extern SVGENDQ svgen_dq1;

//변수 통신 관련
extern Uint16 Rx_index;
extern Uint16 Tx_index;
extern Uint16 TxIntervalCnt;
extern Uint16 TxInterval_1s;
extern int Dummy_comm;
extern CRC_flg	CRC;

extern char scib_tx_buf[SCIB_BUF_SIZE+1];
extern char scib_tx_pos, scib_tx_end;
extern char scib_rx_buf[SCIB_BUF_SIZE+1];
extern char scib_rxd;

extern char scic_tx_buf[SCIC_BUF_SIZE+1];
extern char scic_tx_pos, scic_tx_end;
extern char scic_rx_buf[SCIC_BUF_SIZE+1];
extern char scic_rxd;

//-- Serial Data Stack  
extern WORD Data_Registers[Buf_MAX];
extern WORD CAN_Registers[Buf_MAX];
extern WORD SCI_Registers[Buf_MAX];  

// ADC
extern int	Adc_Uphase;
extern int	Adc_Vphase;
extern int	Adc_Wphase;
extern int	Adc_Vdc	;
extern int	Adc_AIN_I1; 
extern int	Adc_AIN_V1;
extern int	Adc_AIN_I2;
extern int	Adc_AIN_V2;
extern int	Adc_AIN_3;
extern int	Adc_AIN_4;
extern int	Adc_AIN_5 ;
extern int	Adc_Temperature;
extern int	Adc_Vclass; 

//====================================
// F28335 
// DAC Parameter
//==================================== 
extern double	DA1,DA1_range,DA1_offset,DA1_sign;
extern double	DA2,DA2_range,DA2_offset,DA2_sign;
extern double	DA3,DA3_range,DA3_offset,DA3_sign;
extern double	DA4,DA4_range,DA4_offset,DA4_sign;

extern FLAG Flag;
extern PARAMETER P;
extern OPERATION OP;
extern TIME time;



// extern double Err_Wrm_integ, Err_Wrm_integ1;

//------------- Core Variables End -------------//




#endif


