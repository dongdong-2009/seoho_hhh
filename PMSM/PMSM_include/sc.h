typedef struct{
	float La;
	float Lb;
	float old;
	float fc;
	float Tsamp;
	float out;
	}	LPF;
#ifdef _SC_
	int op_cnt = 0;
	int tmp_sc_cnt = 0;
	float Kp_sc = 0.84; //0.7;
	float Ki_scT = 6e-4;//5e-005;
	float Wrpm_ref = 0.0;
	float  ref_slew_up = 0.0, ref_slew_dn = 0.0;
	float thetarm_pre = 0.0, dthetarm = 0.0;
	float Wrpm = 0.0, Wrm = 0.0;
	float Wrpm_err = 0.0, Wrpm_err_integ = 0.0;
	float Wrm_err = 0.0, Wrm_err_integ = 0.0;
	void speed_controller();
	LPF	Wrm_L;


#else
	extern int op_cnt;
	extern int tmp_sc_cnt;
//	extern float Wr;
	extern float Wrpm_ref;
	extern float ref_slew_up, ref_slew_dn;
	extern float thetarm_pre, dthetarm;
	extern float Wrpm, Wrm;
	extern float Kp_sc, Ki_scT;
	extern float Wrpm_err, Wrpm_err_integ;
	extern float Wrm_err, Wrm_err_integ;

	extern void speed_controller();
	extern LPF	Wrm_L;
#endif

extern void op_master();


