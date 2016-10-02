#define EEPROM_WRITED	1
#define RUNN	1
#define STOP	0
#ifdef _VARIABLES_ 

// G1 Basic Control Setup[l]
		float P_rate = 0;			// P1.0
		float V_rate = 0;			// P1.1
		float I_rate = 0;			// P1.2
		float F_rate = 0;			// P1.3
		unsigned int Pn = 0;				// P1.4
		float Wrpm_rate = 0;			// P1.5
		unsigned int Cntrl_Method = 0;		// P1.6
		unsigned int Fpwm = 20000;				// P1.7
		float Vsup = 0;				// P1.9

// G3 RUN_RampCtrl_Config[l]
		unsigned int	Run_Stop_meth = 0;		// P3.0
		unsigned int	RampFunc_Input_Src = 0;	// P3.1
		float Accel_Time	= 100;			// P3.8	// 
		float Decel_Time	= 100;			// P3.25

// G5 Protection Setup
		unsigned int Curr_Lim	= 145;		// P5.0
		unsigned int Max_Con_cur	= 100;	// P5.7 
		unsigned int Over_Load	= 135;		// P5.8
		unsigned int OC_Trip		= 220;	// P5.11
		unsigned int I_zero_Lim	= 15;		// P5.12
		unsigned int OV_Ltd_Fn	= 0;		// P5.13
		float OV_Limit	= 580.;		// P5.14
		float OV_Trip	= 600.;		// p5.15
		float UV_Trip	= 240.;		// P5.18
		unsigned int OP_Ph_Trip = 1;		// P5.19
		unsigned int Out_of_Cnl_t= 1;		// P5.38

// G17 Sensorless Vector Control[l] [rpm]
		float Min_Speed	= 180;		// P17.2 [%]
		float Max_Speed	= 180;		// P17.2 [%]
		unsigned int Over_Spd_Lim= 200;		// P17.3 [%]
		unsigned int Cur_Ctrl_Kp	= 100;	// P17.11[%]
		unsigned int Cur_Ctrl_Ki	= 100;	// P17.12[%]
		unsigned int Spd_Ctrl_Kp	= 100;	// P17.18[%]
		unsigned int Spd_Ctrl_Ki	= 100;	// P17.19[%]
		float	Spd_Ctrl_B	= 500;	// P17.50[Hz]
		float Cur_Ctrl_B	= 1000;		// P17.51[Hz] 

// G21 Motor Constant[l]
		float Rstator		= 0.;	// P21.9 [mOhm]
		float Lds			= 0.;	// P21.10[mH]
		float Lqs			= 0.;	// P21.11[mH]
		float K_back_EMF	= 0.;		// P21.12[]
		float Inertia		= 0.;	// P21.13[s]
		float B_fric		= 0.;		// P21.14
		float Base_spd	= 7000.;		// P21.15[rpm]

// Operation
		unsigned int Local_Remote = 0; 	// M0.0
		float Speed_ref	= 0.0;			// M0.2 [rpm]
		float Frequency_ref = 0.0;		// M0.3 [Hz]
		float Torque_ref	= 0.0;		// M0.4 [Nm]

// RUN/STOP
		unsigned int Run_Stop	= 0;


		unsigned int Group_index = 0;		//
		unsigned int Comm_array[3000];
		unsigned char Comm_GROUP = 0;
		unsigned char Comm_INDEX = 0; 

		unsigned int Machine_state = 0;
		void get_cmd(unsigned char,unsigned char);
		void EEPROM_DATA_INIT();
		int Var_initialize(unsigned char, unsigned char);
		void addr2par_num(int);
		void par_num2addr(unsigned char, unsigned char);
		void eeprom2sys();
		void Key_pad2eeprom(unsigned char, unsigned char, unsigned char);
		void Update_state();
		void Get_Monitor();
		void Nop1();
#else
// G1 Basic Control Setup[l]
	extern unsigned int Pn, Cntrl_Method, Fpwm;
	extern float P_rate, V_rate, I_rate, F_rate, Wrpm_rate,Vsup;

// G3 RUN_RampCtrl_Config[l]
	extern unsigned int	Run_Stop_meth, RampFunc_Input_Src;
	extern float Accel_Time, Decel_Time;

// G5 Protection Setup
	extern unsigned int Curr_Lim, Max_Con_cur, Over_Load, OC_Trip, I_zero_Lim, Out_of_Cnl_t;
	extern unsigned int OV_Ltd_Fn, OP_Ph_Trip;
	extern float OV_Limit, OV_Trip, UV_Trip;

// G17 Sensorless Vector Control[l]
	extern unsigned int Over_Spd_Lim, Cur_Ctrl_Kp, Cur_Ctrl_Ki;
	extern unsigned int Spd_Ctrl_Kp, Spd_Ctrl_Ki; 
	extern float Min_Speed, Max_Speed, Spd_Ctrl_B, Cur_Ctrl_B;

// G21 Motor Constant[l]
	extern float Rstator, Lds, Lqs, K_back_EMF;
	extern float Inertia,B_fric, Base_spd;

// Operation
	extern unsigned int Local_Remote;
	extern float Speed_ref, Frequency_ref, Torque_ref;

	extern unsigned int Run_Stop;
	extern unsigned int Machine_state;
// check
	extern unsigned int Group_index;
	extern unsigned int Comm_array[3000];

	extern unsigned char Comm_GROUP;
	extern unsigned char Comm_INDEX;

	extern void get_cmd(unsigned char,unsigned char);
	extern void EEPROM_DATA_INIT();
	extern int Var_initialize(unsigned char, unsigned char);
	extern void addr2par_num(int);
	extern void par_num2addr(unsigned char, unsigned char);
	extern void eeprom2sys();
	extern void Key_pad2eeprom(unsigned char, unsigned char, unsigned char);
	extern void Update_state();
	extern void Get_Monitor();
	extern void Nop1();
#endif

