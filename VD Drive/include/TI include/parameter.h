/* ==============================================================================
System Name:  ACI34

File Name:	PARAMETER.H

Description:	Parameters file for the Real Implementation of Sensorless Direct 
          		Field Orientation Control for a Three Phase AC Induction Motor. 

Originator:		Digital control systems Group - Texas Instruments

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 05-15-2002	Release	Rev 1.0
=================================================================================  */

#ifndef PARAMETER_H
#define PARAMETER_H

/*-------------------------------------------------------------------------------
Next, definitions used in main file.
-------------------------------------------------------------------------------*/
//#define PI 3.14159265358979

/********系统频率***********/
#define SYSTEM_FREQUENCY 150

/********中断频率**********/
#define ISR_FREQUENCY 3

/* This machine parameters are based on 1-hp WEG induction motor */
/********电机参数***********/

// #define Rs 		1.723               	/* Stator resistance (ohm) */
// #define Rr   	2.011               	/* Rotor resistance (ohm) */
// #define Ls   	0.166619     			/* Stator inductance (H) */
// #define Lr   	0.168964				/* Rotor inductance (H) */	
// #define Lm   	0.159232				/* Rotor inductance (H) */
// #define P    	4						/* Number of poles */	

/******* 定义基值**********/
#define BASE_IQ11TOF    2048
#define BASE_VOLTAGE    184.752       /* Base peak phase voltage (volt) */
#define BASE_CURRENT    21.2            /* Base peak phase current (amp) */
#define BASE_VDC        0.299
#define BASE_VPP        707
#define BASE_TORQUE     7.35105194    /* Base torque (N.m) */
#define BASE_FLUX       0.79616       /* Base flux linkage (volt.sec/rad) */
#define BASE_FREQ      	50            /* Base electrical frequency (Hz) */

#endif  // end of PARAMETER.H definition

//===========================================================================
// No more.
//===========================================================================
