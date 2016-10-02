
// 파라미터 오류 코드 	: 1 ~ 799

#define TRIP_MOTOR_RPM_UNDER		1002
#define TRIP_POWER_FACTOR_UNDER		1001
#define TRIP_POWER_FACTOR_OVER		1000

#define TRIP_OVER_LOAD_TIME			201	// 11 RMS 값으로 입력하는 것으로 수정 
#define TRIP_OVER_CURRENT			301	// 11 RMS 값으로 입력하는 것으로 수정 

#define CODE_OVER_VOLTAGE_LEVEL		302	// 12 DC link Voltage OV1은 자동으로 계산 할 것   
#define TRIP_UNDER_VOLT				303	// 13 수정 
#define CODE_OVER_SPEED_RATIO		304	// 14 수정 
#define CODE_ETHERMAL_LEVEL			305	// 15 New 수정 
#define CODE_PRE_CHARGE_TIME		306	// 초기충전 시간 
#define CODE_I_MAX_COEFF			307	// 
#define CODE_OVER_CURRENT_TIME		308	// 
#define CODE_SPEED_DETECTION_ERROR	309 // Encoder Pulse Error

#define ERR_IGBT_UH							811
#define ERR_IGBT_UL							812
#define ERR_IGBT_VH							813
#define ERR_IGBT_VL							814
#define ERR_IGBT_WH							815
#define ERR_IGBT_WL							816
#define ERR_IGBT_DB							817
#define ERR_IGBT_RESET						818

#define FAULT_DRIVER						819
#define FAULT_DB							820
#define FAULT_ZC							821

#define	ERR_OC0								821
#define	ERR_OC								822
#define	ERR_UC0								823
#define	ERR_OV0								824
#define	ERR_OV								825
#define	ERR_UV0								826
#define	ERR_UV								827
#define	ERR_OS0								828
#define	ERR_OS								829

#define	ERR_SUV								831	
#define	ERR_SOV								832
#define	ERR_SOC								833
#define ERR_OVER_CURRENT_U_PHASE			834 
#define ERR_OVER_CURRENT_V_PHASE			835 
#define ERR_OVER_CURRENT_W_PHASE			836

#define	ERR_NAME_PLATE_PAR_MISMATCH			841
#define	ERR_NAME_PLATE_PAR_MISMATCH0		842
#define	ERR_NAME_PLATE_PAR_MISMATCH1		843

#define	ERR_PM_Is							844
#define	ERR_GM_Is							845
#define	ERR_K_Damp_Is						846


#define	ERR_Tr_Over							851
#define	ERR_Tr_Under						852
#define	ERR_sigma_Under						853
#define	ERR_sigma_Over						854
#define	ERR_Ki_Is_Under						856
#define	ERR_Kp_Is_Over						857
#define	ERR_Kp_Is_Under						858

#define	ERR_EXCITATION_TIME					859

// SCI 
#define ERR_SCI_CMD_ADDR_GROUP_UNDER		861
#define ERR_SCI_CMD_ADDR_GROUP_OVER			862
#define ERR_SCI_CMD_ADDR_1ST_UNDER			863
#define ERR_SCI_CMD_ADDR_1ST_OVER			864
#define ERR_SCI_CMD_ADDR_2ND_UNDER			865
#define ERR_SCI_CMD_ADDR_2ND_OVER			867

// 오토튜닝 오류
#define	ERR_Req_Under						920
#define	ERR_Req_Over						921
#define	ERR_Leq_Under0						923
#define	ERR_Leq_Over0						924
#define	ERR_Rs_Under						925
#define	ERR_Rs_Over							926
#define	ERR_Ls_Over0						927
#define	ERR_Ls_Under0						928	
#define	ERR_Ls_Under1						929
#define	ERR_Ls_Over1						930
#define	ERR_Leq_Over1						931
#define	ERR_Rr_Over							932
#define	ERR_Rr_Under						933
#define	ERR_Jm_Under						934
#define	ERR_Jm_Over							945
#define	ERR_WRONG_INTERRUPT_CMD				950

// 하드웨어 프로텍션 (TMS320F240)
#define	ERR_PRECHARGING_FAIL				960		// 초기 충전 실패 
#define	ERR_PWM								961
#define	ERR_HOC								962
#define	ERR_HOV								963
#define	ERR_HUV								964
#define	ERR_OVER_HEAT						965

#define	ERR_EXT_TRIP						966

#define	ERR_PWM_CNT							971		// 스위칭 주기의 카운터 값 오류 

#define	ERR_INV_DISABLE						972		// 운전중 인버터 디스에이블 

#define Fault_Check_Enable	1													// 2010.05.12

/* 	typedef struct {
		float I_Lmt;
		float MaxCon_Curr;
		float Over_Load;
		float OC_Trip;
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
		int EVENT;
	}	FAULT;
*/

struct TRIP_INFO_DEFINE {
	int		CODE;				// 1
	float	CURRENT;			// 5
	float	VDC;				// 6
	float	HZ;					// 7
	float	DATA;				// 4
	char 	TIME[30];			// 3
	char 	MSG[40];			// 2
};

typedef struct TRIP_INFO_DEFINE TRIP_INFO;

#ifdef _FAULT_

// for REALSYS MONITORING
int flagcontroll = 0;
int flagFOCC = 0;  
int flagcontrol = 0;
int faultOC_set = 65;
int flagFOC = 0;
int flagGT1_ENA = 0;
int flagGT2_ENA = 0;
int flagPWM_CTR = 0;
int flagvar_rst = 0;
int flagadoffset_complete = 0;
int flagencrst_complete = 0;
int flagencrst_start = 0;
int flagccseq = 0; 

int gPWMTripCode = 0; 
int Speed_detect_fault = 0;
int FaultInfo = 0;
float faultOV_set = 360.0;
float faultUV_set = 200.0;
float over_speed_ratio = 1.05;
float motor_rate_rpm = 60000;


//int TripCode = 0;

//FAULT	fault;
#else
// for REALSYS MONITORING
extern int flagcontroll;
extern int flagFOCC;  
extern int flagcontrol;
extern int faultOC_set;
extern int flagFOC;
extern int flagGT1_ENA;
extern int flagGT2_ENA;
extern int flagPWM_CTR;
extern int flagvar_rst;
extern int flagadoffset_complete;
extern int flagencrst_complete;
extern int flagencrst_start;
extern int flagccseq; 

extern int gPWMTripCode;
extern int Speed_detect_fault;
extern int FaultInfo;
extern FAULT fault;
extern float over_speed_ratio;
extern float motor_rate_rpm;
extern float faultOV_set;
extern float faultUV_set;
//extern int TripCode;
#endif




