#define _FAULT_

#include "main_def.h"
#include "math.h" 

int fault_chk( )
{
	int TripCode;
	TripCode = 0;
	//	#if Fault_Check_Enable
	#if Fault_Check_Enable
		if ( ( TripCode = CheckOverCurrent()	) != 0) return TripCode	;
//		if ( ( TripCode = CheckOverVolt() 		) != 0) return TripCode	;
		if ( ( TripCode = CheckUnderVolt()		) != 0) return TripCode	;
		if ( ( TripCode = CheckOverHeat()		) != 0) return TripCode	;
//		if ( ( TripCode = CheckFaultIGBT()		) != 0) return TripCode	;
		if ( ( TripCode = CheckFaultDriver()	) != 0) return TripCode	;
//		if ( ( TripCode = CheckExtTrip()		) != 0) return TripCode	;
		if ( ( TripCode = CheckSpeedDetection()	) != 0) return TripCode ; 
//		if ( ( TripCode = CheckFaultZC()		) != 0) return TripCode ;
	#endif
	return TripCode;
}

int CheckOverCurrent( )
{
	static int MaxCon_Curr_Count = 0;		// Maximum Continues current timer
	static int OL_TimeOver_Count = 0;		// Over load timer
	static int first = 0;

//		fault.MaxCon_Curr 	= 95%
//		fault.Over_Load 	= 135%
//		fault.OC_Trip		= 220% 

// Current Limit
	if(first > 20)
	{
		if (fabs(Ias) > fault.OC_set) {
			fault.FT_SWPROT_Ias = 1;	fault.EVENT = 1;   fault.Ias_OC = Ias; FaultInfo = ERR_OVER_CURRENT_U_PHASE;
			return ERR_OVER_CURRENT_U_PHASE; }
		if (fabs(Ibs) > fault.OC_set){
			fault.FT_SWPROT_Ibs = 1;	fault.EVENT = 1;   fault.Ibs_OC = Ibs; FaultInfo = ERR_OVER_CURRENT_V_PHASE;
			return ERR_OVER_CURRENT_V_PHASE; }
		if (fabs(Ics) > fault.OC_set){
			fault.FT_SWPROT_Ics = 1;	fault.EVENT = 1;   fault.Ics_OC = Ics; FaultInfo = ERR_OVER_CURRENT_W_PHASE;
			return ERR_OVER_CURRENT_W_PHASE; } 
		first = 21;
	}
	else first ++; 

		 if (Is_mag > fault.OC_set) {
			FaultInfo = TRIP_OVER_CURRENT; fault.EVENT = 1;
			return TRIP_OVER_CURRENT; }
	else if (Is_mag > fault.Over_Load) OL_TimeOver_Count++;
	else if (Is_mag > fault.MaxCon_Curr) 
		 { 	if(OL_TimeOver_Count > 0) OL_TimeOver_Count --;
									  MaxCon_Curr_Count ++; }
	else {	if(OL_TimeOver_Count > 0) OL_TimeOver_Count --;
			if(MaxCon_Curr_Count > 0) MaxCon_Curr_Count --; }
	 	if (MaxCon_Curr_Count > 10000)	// maximum continues current 시간함수로써 조절해야함
		{
			MaxCon_Curr_Count = 10001; FaultInfo = TRIP_OVER_LOAD_TIME;  fault.EVENT = 1;
			return TRIP_OVER_LOAD_TIME;
		}
	
	return 0;
}


int CheckOverVolt()
{
	static int OverVoltCount = 0;

	if(Vdc_measured > faultOV_set) OverVoltCount++;//||(GpioDataRegs.GPADAT.bit.GPIO15==0)) OverVoltCount++;
	else if( OverVoltCount > 0) OverVoltCount --;
	if (OverVoltCount > 5)
	{
		OverVoltCount = 6;
		fault.FT_SWPROT_Vdc_OV = 1; fault.EVENT = 1; FaultInfo = CODE_OVER_VOLTAGE_LEVEL;
		return CODE_OVER_VOLTAGE_LEVEL;
	}
	return 0;
}

int CheckUnderVolt( )
{
	static int UnderVoltCount = 0;

	if (Vdc_measured < faultUV_set)			UnderVoltCount++;
	else if( UnderVoltCount > 0)	UnderVoltCount--;

	if (UnderVoltCount > 20 )
	{
		UnderVoltCount = 21;
		fault.FT_SWPROT_Vdc_UV = 1; fault.EVENT = 1; FaultInfo = TRIP_UNDER_VOLT;
		return TRIP_UNDER_VOLT;
	}
	return 0;
}

int CheckOverHeat( )
{
	static int OverHeatCount = 0;

	if( OverHeatCount > 10 )
	{
		OverHeatCount = 11;
		fault.FT_OVER_HEAT = 1, fault.EVENT = 1; FaultInfo = ERR_OVER_HEAT;
		return ERR_OVER_HEAT;
	}
	return 0 ;
}

int CheckFaultDriver( )
{
	static int FaultDriverCount = 0;

	if (GpioDataRegs.GPADAT.bit.GPIO12==0) 	FaultDriverCount++;
	else if( FaultDriverCount > 0) 	FaultDriverCount--;

	if (FaultDriverCount > 10 )
	{
		FaultDriverCount = 11; fault.EVENT = 1; FaultInfo = FAULT_DRIVER;
//		trip_recording( FAULT_DRIVER,FaultDriverCount,"Driver Fault");
		return FAULT_DRIVER;
	}
	return 0;
}

int CheckSpeedDetection( )
{
	static int OverSpeedCount=0, Speed_detec_fault_count=0;
	double OverSpeed;

	OverSpeed = motor_rate_rpm * over_speed_ratio;

	if ( fabs(Wrpm_hat) > OverSpeed ) OverSpeedCount++;
	else if( OverSpeedCount > 0) OverSpeedCount --;

	if (OverSpeedCount > 5 )
	{
		OverSpeedCount = 6;
		fault.EVENT = 1; FaultInfo = CODE_OVER_SPEED_RATIO;
//		trip_recording( CODE_OVER_SPEED_RATIO,Wrpm_MT,"Trip Over Speed");

		return CODE_OVER_SPEED_RATIO;
	}

	if (Speed_detect_fault)	Speed_detec_fault_count++;
	else if (Speed_detec_fault_count>0)	Speed_detec_fault_count--;

	if (Speed_detec_fault_count > 5)
	{
		Speed_detec_fault_count = 6;
		fault.EVENT = 1; FaultInfo = CODE_SPEED_DETECTION_ERROR;
//		trip_recording( CODE_SPEED_DETECTION_ERROR,Wrpm_MT,"Trip Speed Detect");

		return CODE_SPEED_DETECTION_ERROR;
	}


	return 0;

}

int CheckFaultZC( )
{
	static int FaultZCCount = 0;

	if (GpioDataRegs.GPADAT.bit.GPIO16==0) 	FaultZCCount++;
	else if( FaultZCCount > 0) 	FaultZCCount--;

	if (FaultZCCount > 3 )
	{
		FaultZCCount = 4;
		fault.EVENT = 1; FaultInfo = FAULT_ZC;
//		trip_recording( FAULT_ZC,FaultZCCount,"ZC Fault");

		return FAULT_ZC;
	}
	
	return 0;
}

void TripProc( )
{
//	int iCommand;
//	int LoopCtrl;
//	double fReference;

 //	gMachineState = STATE_TRIP;

//	nDC_CONTACT_SET;		// Main Charge Relay Off
	if(FaultInfo != 0) {
//	gInitChargeFlag = 1;
	flag.GT1_ENA = 0;
	flag.FOC = 0;
	flagFOC = 0;
	nFLT2_ON;
	flag.control = 0;
	flagcontrol = 0;
	}
}

