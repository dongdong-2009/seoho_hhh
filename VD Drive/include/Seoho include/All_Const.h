#ifndef		__COMMON_CONSTANTS_
#define		__COMMON_CONSTANTS_

//------------- Core Define Start -------------//

#define NOP				asm("	NOP")

#define NULL        	(void *)0

#define TRUE			1
#define FALSE			0

#define HIGH			1
#define LOW				0

// #define	HI_BYTE(val)	((val >> 8) & 0xff)
// #define	LO_BYTE(val)	(val & 0xff) 


// 변경 필요 
#define	T_DEAD_US				(4.)				// Dead time
#define DEAD_TIME_COUNT		(T_DEAD_US*1.e+2) 		// PWM Dead Time Variable 

//-------
/* micro-seconds delay function */
// TI SDK 1.10의 소스 DSP2833x_usDelay.asm에서 제공하는 DELAY_US 함수를 사용
#define delay_us(us)		DELAY_US(us)	 

/* milli-seconds delay function */
// TI SDK 1.10의 소스 DSP2833x_usDelay.asm에서 제공하는 DELAY_US 함수를 사용
#define delay_ms(ms)		DELAY_US(ms*1000) 

#define EEPROM_WRITED			(1)
#define F_OSC					(30.e+6)		// Osciilator Freq.
#define CPU_CLOCK				(150.e+6)	
#define DUAL_PWM_INTERRUPT		(0) 			//	PWM Top & Bottom Interrupt
#define VF_MODE					(0)


#define	PI					(3.141593)		
#define	SQRT2				(1.414213562)
#define	inv_SQRT2			(0.707106781)
#define	SQRT3				(1.732050808)
#define	inv_SQRT3			(0.577350269) 


#define	RPM2WRM				(2.*PI/60.) 
#define	WRM2RPM				(60./(2.*PI)) 

#define		u				(0)
#define		v				(1)
#define		w				(2)

/* Bound Macro */
#define	BOUND_PI(x)			((x)+(((x)>PI)?(-2.*PI):((x)<(-PI))?(2.*PI): 0.))
#define	BOUND(x,xmax,xmin)	(((x)>(xmax))?(xmax):((x)<(xmin))?(xmin): (x) )   

#define RESET_DRIVER_SET	(GpioDataRegs.GPASET.bit.GPIO7 		= 1)  
#define RESET_DRIVER_CLEAR	(GpioDataRegs.GPACLEAR.bit.GPIO7	= 1)  
#define DC_CONTACT_ON		(GpioDataRegs.GPACLEAR.bit.GPIO10	= 1)
#define DC_CONTACT_OFF		(GpioDataRegs.GPASET.bit.GPIO10		= 1)
#define PWM_HARDWARE_ON		(GpioDataRegs.GPBCLEAR.bit.GPIO35	= 1)
#define PWM_HARDWARE_OFF	(GpioDataRegs.GPBSET.bit.GPIO35		= 1)
#define BOOT_MODE_ON		(GpioDataRegs.GPBCLEAR.bit.GPIO34	= 1)
#define BOOT_MODE_OFF		(GpioDataRegs.GPBSET.bit.GPIO34		= 1)

//------------- Core Define End -------------//



// - Status_Index
#define	STATE_POWER_ON			'Y'
#define	STATE_CALIBRATION		'Z'
#define	STATE_READY				'R'
#define	STATE_STOP				'S'
#define	STATE_ACCELERATING		'A'
#define	STATE_DECELERATING		'D'
#define	STATE_RUNNING			'O'
#define	STATE_TUNING			'T'
#define	STATE_FAULT				'F'


//-- uart 
#define CMD_RUN					'R'
#define CMD_STOP				'S'
// #define CMD_RESET				'T'


#define Buf_MAX 3400

#endif

