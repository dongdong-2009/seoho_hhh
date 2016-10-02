/* =================================================================================
File name:       SVGEN_DQ.H  (IQ version)                    
                    
Originator:	Digital Control Systems Group
			Texas Instruments

Description: 
Header file containing constants, data type definitions, and 
function prototypes for the SVGEN_DQ.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 04-15-2005	Version 3.20                                                 
------------------------------------------------------------------------------*/

#ifndef __SVGEN_DQ_H__
#define __SVGEN_DQ_H__

typedef struct 	{ float  Ualfa; 			// Input: reference alpha-axis phase voltage 
				  float  Ubeta;			// Input: reference beta-axis phase voltage 
				  float  Ta;				// Output: reference phase-a switching function		
				  float  Tb;				// Output: reference phase-b switching function 
				  float  Tc;				// Output: reference phase-c switching function
				  float  Ta1;				// Output: reference phase-a switching function		
				  float  Tb1;				// Output: reference phase-b switching function 
				  float  Tc1;				// Output: reference phase-c switching function
				  float  Ta2;				// Output: reference phase-a switching function		
				  float  Tb2;				// Output: reference phase-b switching function 
				  float  Tc2;
				  void (*calc)();	    // Pointer to calculation function
				} SVGENDQ;
																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																				
typedef SVGENDQ *SVGENDQ_handle;

/*-----------------------------------------------------------------------------
Default initalizer for the SVGENDQ object.
-----------------------------------------------------------------------------*/   
                 
#define SVGENDQ_DEFAULTS { 0,0,0,0,0,0,0,0,0,0,0, \
                       (void (*)(Uint32))svgendq_calc }

/*------------------------------------------------------------------------------
Prototypes for the functions in SVGEN_DQ.C
------------------------------------------------------------------------------*/

void svgendq_calc(SVGENDQ_handle);

#endif // __SVGEN_DQ_H__



