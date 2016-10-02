#define INT_TYPE			0
#define FLOAT_TYPE			1


#define nDAC1_CS			GpioDataRegs.GPBDAT.bit.GPIO55
#define nDAC2_CS			GpioDataRegs.GPBDAT.bit.GPIO57



	typedef struct {
		unsigned int *channel[4];
		int var_types[4];
		float limit[4];
	}	DAC_CONFIG;

#ifdef _DAC_

	DAC_CONFIG dac_configuration;

	int dac_temp = 0;
	int DACTXBUF = 0;

	float dac_calculation_tmp = 0.0;


	float dac_value[4];
	float dac_value_float[4];

	void dac_setup(void);
	void send_dac(int ch, int value_int, float value_float, float limit, char var_types);
	void background_dac(void);
#else
	
	extern DAC_CONFIG dac_configuration;
	extern float dac_value[4];
	extern float dac_value_float[4];

	extern void dac_setup(void);
	extern void send_dac(int ch, int value_int, float value_float, float limit, char var_types);
	extern void background_dac(void);
#endif

