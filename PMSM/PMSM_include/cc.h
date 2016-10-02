
#define		PI				3.141592654
#define		SQRT2			1.414213562
#define		SQRT3			1.732050808
#define		INV_SQRT3		(1./SQRT3)

#define		BOUND_PI(x)		((x) - 2.*PI*(int)((x + PI)/(2.*PI)))

 	typedef struct {
		int PWM;
		int OVM;
		int speed_control;
	}	MODE;

 	typedef struct {
		float I_Lmt;
		float MaxCon_Curr;
		float Over_Load;
		float OC_set;
		float OV_set;
		float UV_set;

		float Ias_OC;
		float Ibs_OC;
		float Ics_OC;
		
		int FT_SWPROT_Ias;
		int FT_SWPROT_Ibs;
		int FT_SWPROT_Ics;
		int FT_SWPROT_Vdc_OV;
		int FT_SWPROT_Vdc_UV;
		int FT_OVER_HEAT;
	}	FAULT;

	typedef struct {
		int GT1_ENA;
		int GT2_ENA;
		int FOC;
		int PWM_CTR;
		int var_rst;
		int start;
		int ad_offset_complete;
		int control;
		int enc_rst_start;
		int enc_rst_complete;
		int cc_seq;
		int cc_test;
	} FLAG; 

#ifdef _CC_

//	float Rs = 0.024;
//	float Ls = 0.000135;
	float Wc_cc = 0.0;
//	float fc_cc = 500.;		// 2010.06.10 bayasa
	float Tsamp = 0.0;
	float ref_sec = 0.0;
	float Kp_cc = 0.0, Ki_ccT = 0.0, Ka_cc = 0.0;
	float alpha_cc = 1.0;
	
	int cc_smp = 0;	

// sensed values
	float Vdc_measured = 0.0, AICMD  = 0.0, Voltage_Range = 0.0;
	float AINI1 = 0.0, AINV1 = 0.0, AINI2 = 0.0, AINV2 = 0.0;
	float Ias = 0.0, Ibs = 0.0, Ics = 0.0, Izs = 0.0;
	float Idss = 0.0, Iqss = 0.0, Is_mag = 0.0;
	float Idse = 0.0, Iqse = 0.0;
	float Idse_ref = 0.0, Iqse_ref = 0.0;
	float Err_Idse = 0.0, Err_Iqse = 0.0;

	int   Drive_Voltage = 0;
	float Is_max = 15.0;
	float sin_thetar = 0.0, cos_thetar = 0.0;
	float INV_Vdc = 0.005;

	float Vdss_ref = 0.0, Vqss_ref = 0.0, Vs_mag = 0.0;
	float Vdse_ref = 0.0, Vqse_ref = 0.0;
	float Vdse_ref_integ = 0.0, Vqse_ref_integ = 0.0;
	float Vdse_ref_fb = 0.0, Vqse_ref_fb = 0.0;
	float Vdse_ref_ffa = 0.0, Vqse_ref_ffa =0.0;
	float Vmag_ref = 0.0, Vmag_ffa = 0.0, Vmag_delta = 0.0 ;
	int	flag_Vmag = 0;
	float Vdse_ref_ff = 0.0, Vqse_ref_ff = 0.0;
	float Vdse_anti = 0.0, Vqse_anti = 0.0;
	float Vdss_sat = 0.0, Vqss_sat = 0.0;
	float Vdse_sat = 0.0, Vqse_sat = 0.0;
	float Vmag_delta4 = 0.0, Vmag_delta3 = 0.0, Vmag_delta2 = 0.0, Vmag_delta1 = 0.0;
	float Vmag_delta_avg = 0.0, Vmag_delta_est = 0.0;

	float Vas_ref = 0.0, Vbs_ref = 0.0, Vcs_ref = 0.0;
	float Van_ref = 0.0, Vbn_ref = 0.0, Vcn_ref = 0.0, integ = 0.0;
	float Vsn = 0.0, Vsn_max = 0.0, Vsn_min = 0.0;
	float Vmax = 0.0, Vmin = 0.0;

	float cal_tmp0 = 0.0, cal_tmp1 = 0.0;

	int cc_seq_cnt = 0;
	int cc_cnt = 0;
	int cntl_cnt = 0;
	int sc_cnt = 0;
	int offset = 0;
	float scale = 1.0;
	unsigned int	Ias_offset = 0, Ibs_offset = 0, AI_offset = 0;

//  for RealDSP monitoring 
	int faultFT_SWPROT_Vdc_UV = 0;
	int faultFT_SWPROT_Ias = 0;
	int faultIas_OC = 0;
	int faultIbs_OC = 0;
	int faultIcs_OC = 0;
	int faultEVENT = 0;
	int DRIVE_ENABLE = 0;
	int MULTI_STEP_BIT0 = 0;
	int MULTI_STEP_BIT1 = 0;
	int MULTI_STEP_BIT2 = 0;
	int MULTI_STEP_BIT3 = 0;
	int FAULT_RESET = 0;
	int JOG = 0;

// DAC variables
	float	DA1,DA1_rng,DA1_mid,valda=0.0;
	float	DA2,DA2_rng,DA2_mid;
	float	DA3,DA3_rng,DA3_mid;
	float	DA4,DA4_rng,DA4_mid; 
	int		DA1_val,DA2_val,DA3_val,DA4_val, DacFlag = 0;

	

	MODE	mode;
	FAULT	fault;
	FLAG	flag;

/* Varialbes for Sensorless */
	long int tmp_cnt = 0;
	int run_Sensorless = 0, ref_control_mode = 0, sensorless_mode = 99, PP = 1;
	int flag_speed_cmd = 0;
	float Wrpm_ref_user = 0.0; // 0.13
	float Wrpm_set_user = 7000.;
	float Wrm_ref = 0.0, Wr_ref = 0.0;
	float Ktr = 0.0, Kwr = 0.0;
	float gain_tr = 0.00005, gain_wr = 0.04;
	float thetar_openloop = 0.0, thetar_hat = 0.0, Err_thetar = 0.0;
	float Wrm_hat = 0.0, Wr_hat = 0.0, Wrpm_hat = 0.0;
	float cos_theta = 1.0, sin_theta = 0.0;
	float thetar_hat1 = 0.0, cos_theta1 = 0.0, sin_theta1 = 0.0;
	float IqcM = 0.0, IdcM = 0.0, Err_IdcM = 0.0;
	float Idse_ref_fake = 0.2;
	float Idse_ref_max = 32.0;
	float Idse_ref_start = 20.0;
	float Idse_ref_sensorless = 6.0;//32.0; // 5.0

	union DIGITAL_FUNCTION func_flag;
	union DIGITAL_OUT_FUNC relay_flag; 

	void current_controller();
	int fault_chk();
	void calculateOffset();

	unsigned int ms_cnt=0;

	#define	TXD_STACK_LENGTH	20		// 송신단 스택의 버퍼 길이

extern	unsigned char TXD_StackWritePtr;			// Serial 데이타의 송신 스택 포인터
extern	unsigned char TXD_StackReadPtr;				// 송신 스택으로 부터 현재 읽혀져야 할 포인터
extern	unsigned char TXD_Stack[TXD_STACK_LENGTH];	// 시리얼 데이타를 저장하는 송신 스택 메모리 공간

#else

	extern int cc_cnt, cntl_cnt;
	extern int sc_cnt;
	extern unsigned int ms_cnt;


// Sensed values
	extern float Ias,Ibs,Ics, Izs, Vdc_measured, AICMD, Voltage_Range;
	extern float AINI1, AINV1, AINI2, AINV2;
	extern float Iasf,Ibsf,Icsf;
	extern float Idss, Iqss, Is_mag;
	extern float Idse, Iqse;
	extern float Idse_ref, Iqse_ref;
	extern float Err_Idse, Err_Iqse;

	extern float INV_Vdc;
	extern float Vdss_ref, Vqss_ref, Vs_mag;
	extern float Vdse_ref, Vqse_ref;
	extern float Vdse_ref_integ, Vqse_ref_integ;
	extern float Vdse_ref_fb, Vqse_ref_fb;
	extern float Vdse_ref_ffa, Vqse_ref_ffa;
	extern float Vmag_ref, Vmag_ffa, Vmag_delta;
	extern int	 flag_Vmag;
	extern float Vdse_ref_ff, Vqse_ref_ff;
	extern float Vdse_anti, Vqse_anti;
	extern float Vdss_sat, Vqss_sat;
	extern float Vdse_sat, Vqse_sat;
	extern float Vmag_delta4, Vmag_delta3, Vmag_delta2, Vmag_delta1;
	extern float Vmag_delta_avg, Vmag_delta_est;

	extern float Vas_ref, Vbs_ref, Vcs_ref;
	extern float Van_ref, Vbn_ref, Vcn_ref, integ;
	extern float Vsn, Vsn_max, Vsn_min;
	extern float Vmax, Vmin;

	extern float Wc_cc;
//	extern float fc_cc;

	extern float Tsamp;
	extern float ref_sec;
	extern float Kp_cc, Ki_ccT, Ka_cc;
	extern float alpha_cc;

	extern int offset;

	extern MODE	mode;
	extern FAULT fault;
	extern FLAG	flag;

	extern int   Drive_Voltage;
	extern float Is_max;
	extern float scale;
	extern unsigned int	Ias_offset, Ibs_offset, AI_offset;

//  for RealDSP monitoring 
	extern int faultFT_SWPROT_Vdc_UV;
	extern int faultFT_SWPROT_Ias;
	extern int faultIas_OC;
	extern int faultIbs_OC;
	extern int faultIcs_OC;
	extern int faultEVENT;
	extern int DRIVE_ENABLE;
	extern int MULTI_STEP_BIT0;
	extern int MULTI_STEP_BIT1;
	extern int MULTI_STEP_BIT2;
	extern int MULTI_STEP_BIT3;
	extern int FAULT_RESET;
	extern int JOG;

//DAC Variables
	extern double	DA1,DA1_rng,DA1_mid;
	extern double	DA2,DA2_rng,DA2_mid;
	extern double	DA3,DA3_rng,DA3_mid;
	extern double	DA4,DA4_rng,DA4_mid;
	extern int		DA1_val,DA2_val,DA3_val,DA4_val, DacFlag;

	extern volatile unsigned int ZONE0_BUF[2048];
// V_F variables 
	extern int 		PMSM_CNTRL;
	extern double	Freq_out, Freq_limit, we;
	extern double	theta, SinTheta, CosTheta;
	extern double 	Vs_ref, inv_motor_rate_hz, Vs_rat;

/* Varialbes for Sensorless */
	extern long int tmp_cnt;
	extern int 	 run_Sensorless, ref_control_mode, sensorless_mode;
	extern int 	 flag_speed_cmd;
	extern float Wrpm_ref_user;
	extern float Wrpm_set_user;
	extern float Wrm_ref, Wr_ref;
	extern float Ktr, gain_tr, Kwr, gain_wr;
	extern float thetar_openloop, thetar_hat, Err_thetar;
	extern float Wrm_hat, Wr_hat, Wrpm_hat;
	extern float cos_theta, sin_theta;
	extern float thetar_hat1, cos_theta1, sin_theta1;
	extern float IqcM, IdcM, Err_IdcM;
	extern float Idse_ref_fake;
	extern float Idse_ref_max;
	extern float Idse_ref_start;
	extern float Idse_ref_sensorless;


	extern void current_controller();
	extern int fault_chk();
	extern void calculateOffset();
	extern void Update_var();
// for fault check
//	extern void	fault_chk();
//	extern void digital_input_proc();
	extern int	CheckOverCurrent( );
	extern int	CheckOverVolt( );
	extern int 	CheckUnderVolt( );
	extern int 	CheckOverHeat( );
	extern int 	CheckFaultIGBT( );
	extern int 	CheckFaultDriver( );
	extern int 	CheckExtTrip( ); 
	extern int	CheckSpeedDetection();
	extern int	CheckVmagDelta();
	extern void GetAnaMonitCount(unsigned int * piChA, unsigned * piChB);

	extern union DIGITAL_FUNCTION func_flag;	// 디지털 입력의 기능을 설정한다.
	extern union DIGITAL_OUT_FUNC relay_flag; 

#endif

extern void Update_var();







