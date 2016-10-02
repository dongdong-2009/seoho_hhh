#ifndef		__VARIABLES_
#define		__VARIABLES_

//------------- Core Variables Start -------------//
extern double Load_Power;
extern double Supply_Voltage; 

//--------Motor Parameters--------//
extern double Rs, Rr, Lsigma, Lm;
extern double Jm, inv_Jm, Pole_pair, Bm; 
extern double LAMdre_rate, LAMdre_ref; 
extern double Lr, Lls, Llr;
extern double Ls, Kt; 
extern double Is_rate, Wrpm_rate, Wrpm_max;
extern int PPR;
//------Motor Parameters End------//
extern double Temp_HHH1, Temp_HHH2, Temp_HHH3;
extern double Jm_est;


extern double LAMdrs_cm, LAMqrs_cm;
extern double LAMdrs_vm, LAMqrs_vm; 
extern double LAMdrs, LAMqrs;

//�Է� 
extern int    Command;
extern double Init_reference;
extern double Init_reference_Auto_tuning;
extern double Final_reference; 
extern double Wrpm_scale;
extern double AnaCmdReference;
extern double Accel_time;	
extern double Decel_time;	
extern double Accel_time_Auto_tuning;	
extern double Decel_time_Auto_tuning;

//����  
extern double Vdc, Vdc_max;
extern double Vdss_ref_old, Vqss_ref_old;
extern double Vdse_ref_fb, Vqse_ref_fb; // ������ ���� 
extern double Vqse_ref_integ;
extern double Vdss_ref, Vqss_ref, Vdse_ref, Vqse_ref;

//���� 
extern double Ias, Ibs, Ics;
extern double Ias_offset, Ibs_offset;
extern double Ias_ref, Ibs_ref, Ics_ref;
extern double Is_max, Is_mag, Is_mag_rms;
extern double Idss, Iqss, Idse, Iqse; 
extern double Idss_ref, Iqss_ref, Idse_ref, Iqse_ref;
extern double Idse_ref_max, Iqse_max_fw;

//�ڼ� 
extern double LAMdre;
extern double LAMdre_flt; 


//�ӵ� & ���ļ� 
extern double freq_vf, freq_vf_set;
extern double We; 
extern double Wr;
extern double Wrpm_ref;
extern double Wrpm_fw1, Wrpm_fw2;
extern double Wrm_det, Wrpm_det, Wrm_det_flt; 

//�̵�
extern double K_CM1, K_CM2;
extern double K_fw1, K_fw2;
extern double Wc_Fst, Kp_Fst, Ki_Fst,  Freq_Fst; 
extern double Wc_fc, Kp_fc, Ki_fc, Ka_fc;
extern double Wc_sc, Kp_sc, Ki_sc, Ka_sc; 
extern double Wc_cc, Kp_cc, Ki_cc, Ka_cc;
extern double L1_sp_est, L2_sp_est, L3_sp_est, L3_sp_tmp;

//���� 
extern double Theta; 
extern double Thetar;

// ��ũ
extern double Te;  

//sin&cos
extern double Cos_Theta_vtg, Sin_Theta_vtg;
extern double Cos_Theta, Sin_Theta; 

//switch
extern int Driver_ON,Flux_build_up;

//PWM Count 
extern double 	T_dead_Tuning;
extern Uint16	EPwmPeriodCount;
extern unsigned int DutyCount[3];


//��Ÿ 
extern long Main_counter;
extern int State_Index;
extern int Auto_tuning_index;
extern double Interrupt_time_max;
extern volatile unsigned int ZONE0_BUF[2048]; // by RYU

//���� ��� ����
extern Uint16 Group_index;
extern BYTE Comm_GROUP;
extern BYTE Comm_INDEX;
extern Uint16 Comm_array[3000];
extern Packet_Head_flg Packet_Head ;
extern CRC_flg	CRC ;
extern Bit_field_	flag0;

extern char scib_tx_buf[SCIB_BUF_SIZE+1];
extern char scib_tx_pos, scib_tx_end;
extern char scib_rx_buf[SCIB_BUF_SIZE+1];
extern char scib_rx_pos, scib_rx_end;
extern char scib_rxd;

extern char scic_tx_buf[SCIC_BUF_SIZE+1];
extern char scic_tx_pos, scic_tx_end;
extern char scic_rx_buf[SCIC_BUF_SIZE+1];
extern char scic_rx_pos, scic_rx_end;
extern char scic_rxd;

extern int  scic_tx_ibuf[SCIC_BUF_SIZE+1];
extern int  scic_rx_ibuf[SCIC_BUF_SIZE+1]; 

//-- Serial Data Stack  
extern	unsigned char RRXD_Stack[RXD_STACK_LENGTH];
extern	unsigned char RXD;							// ���� ����Ÿ
extern	unsigned char RXD_StackWritePtr;			// Serial ����Ÿ�� ���� ���� ������
extern	unsigned char RXD_StackReadPtr;				// ���� �������� ���� ���� �������� �� ������
extern	unsigned char RXD_Stack[RXD_STACK_LENGTH];	// �ø��� ����Ÿ�� �����ϴ� ���� ���� �޸� ����
	
extern	unsigned char TXD_StackWritePtr;			// Serial ����Ÿ�� �۽� ���� ������
extern	unsigned char TXD_StackReadPtr;				// �۽� �������� ���� ���� �������� �� ������
extern	unsigned char Frame_StackPtr;				// ���ð����󿡼� ���� ����� �������� �ּ�
extern	unsigned char Packet_StackPtr;				// ���� �������� �ش� ��Ŷ�� �ּ�
	
extern	unsigned  char NewFrame_Detect;				// ���ο� ������ ����
	
extern	unsigned  char NewFrame_Packet_State;		// Packet�� ���� �ܰ� 
extern	unsigned  char NewFrame_ByteCnt;			// ���ŵ� ����Ʈ ���� ��Ÿ���� �ε���
extern	unsigned  char Frame_ByteCnt;
extern	unsigned  char Packet_ByteCnt;
extern	unsigned char TXD_Stack[TXD_STACK_LENGTH];	// �ø��� ����Ÿ�� �����ϴ� �۽� ���� �޸� ����
extern  unsigned char TTXD_Stack[20];

extern	unsigned char NewFrame_StackPtr;			// ���ð����󿡼� ���� ����� �������� �ּ� 

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
extern int	Adc_V_Range; 

//====================================
// F28335 
// DAC Parameter
//==================================== 
extern double	DA1,DA1_rng,DA1_mid;
extern double	DA2,DA2_rng,DA2_mid;
extern double	DA3,DA3_rng,DA3_mid;
extern double	DA4,DA4_rng,DA4_mid;
extern int	DA1_val,DA2_val,DA3_val,DA4_val;


extern FAULT_INFO FaultInfo;   //chy
extern volatile FLAG Flag;     // chy

extern char MonitorMsg[30];

//------------- Core Variables End -------------//

//chy

extern	Uint16 bin[SIZE];
extern	Uint16 g2b[SIZE];
extern Uint16 result1;
extern Uint16 Number_of_turns_buf;
extern Uint16 Steps_per_turn_buf;
extern Uint16 abs_position;
extern Uint16 rdata1;
extern Uint16 rdata2;
extern Uint16 Number_of_turns;

extern Uint16 Steps_per_turn;
extern Uint16 Steps_buf1;
extern Uint16 Steps_buf2;
extern Uint16 inv_rdata1;
extern Uint16 inv_rdata2;

extern Uint16 gNum;

// ------------ Core_Speed_Calc.c
extern double Wrpm_det1, Wrn1;   // chy
extern double test1, test2, test3, test4;
extern Uint16 etest1, etest2, etest3, etest4;
extern Uint16 rx_complete;
extern float sin_theta, cos_theta;



#endif


