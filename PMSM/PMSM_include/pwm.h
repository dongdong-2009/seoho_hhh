#define PWM_BUF_DIR_HIGH	1
#define PWM_BUF_DIR_LOW		0

#define PWM_ACTIVE_HIGH		1
#define PWM_ACTIVE_LOW		0
#define PWM_ACTIVE_LOW_SIMULTANEOUS	5

#define SAMPLING_TOP		0x01
#define SAMPLING_BOTTOM		0x02
#define SAMPLING_BOTH		0x03

typedef struct {
	int op_state;
	int sampling_mode;
	int phase_duty_max_scaled;
	int phase_duty_half_scaled;
	int phase_duty_min_scaled;
	int phase_a_duty_scaled;
	int phase_b_duty_scaled;
	int phase_c_duty_scaled;
}	PWM;


#ifdef _PWM_

	int  carrier_mode = 0;

	PWM	 pwm_g1;
	PWM	 pwm_g2;
	void pwm_buffer_setup();
	void pwm_g1_setup(int buf_dir,int pwm_dir, double sys_clk, double pwm_freq, double dead_time);
#else
	extern int carrier_mode;
	extern PWM pwm_g1;
	extern void pwm_buffer_setup();
	extern void pwm_g1_setup(int buf_dir,int pwm_dir, double sys_clk, double pwm_freq, double dead_time);
#endif
