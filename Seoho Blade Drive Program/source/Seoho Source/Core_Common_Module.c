#include	<All_Header.h>
#include	<All_Extern_Variables.h>

void Parameter_Initialization_by_Power()
{
	int Drive_power= 0;

	if (GpioDataRegs.GPCDAT.bit.GPIO68) Drive_power+= 1;
	if (GpioDataRegs.GPCDAT.bit.GPIO67) Drive_power+= 2;
	if (GpioDataRegs.GPCDAT.bit.GPIO66) Drive_power+= 4;
	if (GpioDataRegs.GPCDAT.bit.GPIO65) Drive_power+= 8;
	if (GpioDataRegs.GPCDAT.bit.GPIO64) Drive_power+= 16;	

	Drive_power = 2;
	
	switch ( Drive_power )
	{
		case 0:
			P.G00.P10_Drive_power_x10_kW = 55;
			I_scale= 25.0/1.8;
			P.G00.P09_IGBT_current= 35;
			P.G01.P00_Rated_power_x10_kW= 55;
			P.G01.P01_Rated_voltage= 220;
			P.G01.P02_Rated_current_x10= 210;
			P.G01.P03_Rated_frequency= 60;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1755;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 25;
			P.G01.P08_Supply_voltage= 380;
			break;
		case 1:
			P.G00.P10_Drive_power_x10_kW = 75;
			I_scale= 25.0/1.43;
			P.G00.P09_IGBT_current= 32;
			P.G01.P00_Rated_power_x10_kW= 95;
			P.G01.P01_Rated_voltage= 400.;
			P.G01.P02_Rated_current_x10= 115;
			P.G01.P03_Rated_frequency= 87;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1740;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 380;
			break; 	
		case 2:
		 	P.G00.P10_Drive_power_x10_kW = 110; 
			//I_scale= 100.0/1.8;
			I_scale= 100.0/1.8;  // ryu  
/*
			P.G00.P09_IGBT_current= 32.1;
			P.G01.P00_Rated_power_x10_kW= 95;
			P.G01.P01_Rated_voltage= 400;
			P.G01.P02_Rated_current_x10= 200;
			P.G01.P03_Rated_frequency= 87; //50;// 87;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 2510; //1470;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 40;
			P.G01.P08_Supply_voltage= 380; 
*/
			P.G00.P09_IGBT_current= 50.;        // 32;   
			P.G01.P00_Rated_power_x10_kW= 95; // 75
			P.G01.P01_Rated_voltage= 380;
			P.G01.P02_Rated_current_x10= 330; // 167->250;
			P.G01.P03_Rated_frequency= 87;  //60
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 2540;  // 2540  1490
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50; 
			P.G01.P08_Supply_voltage= 380;

			break;
		case 3: 
			P.G00.P10_Drive_power_x10_kW = 150; 
			P.G00.P09_IGBT_current= 35.;
			P.G01.P00_Rated_power_x10_kW= 150;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 4: 
			P.G00.P10_Drive_power_x10_kW = 185; 
			I_scale= 200.0/0.85;
			P.G00.P09_IGBT_current= 63.;
			P.G01.P00_Rated_power_x10_kW= 185;
			P.G01.P01_Rated_voltage= 415;
			P.G01.P02_Rated_current_x10= 350;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1465;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 380;
			break;
		case 5: // (110104 by HHH) 현재 실험중
			P.G00.P10_Drive_power_x10_kW = 220;
			I_scale= 100.0/0.9;
			P.G00.P09_IGBT_current= 63;


/*			//7.5kW
			P.G01.P00_Rated_power_x10_kW= 75;
			P.G01.P01_Rated_voltage= 380;
			P.G01.P02_Rated_current_x10= 163;
			P.G01.P03_Rated_frequency= 60;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1760;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 380;
*/
			//11kW
/*			P.G01.P00_Rated_power_x10_kW= 110;
			P.G01.P01_Rated_voltage= 380;
			P.G01.P02_Rated_current_x10= 232;
			P.G01.P03_Rated_frequency= 60;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1770;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 380;
*/
/*			//15kW
			P.G01.P00_Rated_power_x10_kW= 150;
			P.G01.P01_Rated_voltage= 380;
			P.G01.P02_Rated_current_x10= 320;
			P.G01.P03_Rated_frequency= 60;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1750;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 25;
			P.G01.P08_Supply_voltage= 380;
*/


			//18.5kW
			P.G01.P00_Rated_power_x10_kW= 185; // 18.5kW
			P.G01.P01_Rated_voltage= 415;
			P.G01.P02_Rated_current_x10= 350; //35.0A
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1465;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz=50; // 2.5kHz
			P.G01.P08_Supply_voltage= 380;

			break;
		case 6: 
			P.G00.P10_Drive_power_x10_kW = 300;
			P.G00.P09_IGBT_current= 35.;
			P.G01.P00_Rated_power_x10_kW= 300;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 7: 
			P.G00.P10_Drive_power_x10_kW = 370; 
			P.G00.P09_IGBT_current= 35.;
			P.G01.P00_Rated_power_x10_kW= 370;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 8: 
			P.G00.P10_Drive_power_x10_kW = 450;
			P.G00.P09_IGBT_current= 35.;
			P.G01.P00_Rated_power_x10_kW= 450;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 9: 
			// (110228 by HHH)
			I_scale= 100.0/0.9 *(410./176.3);
			P.G00.P10_Drive_power_x10_kW = 550;
			P.G00.P09_IGBT_current= 151;

			//15kW
			P.G01.P00_Rated_power_x10_kW= 150;
			P.G01.P01_Rated_voltage= 380;
			P.G01.P02_Rated_current_x10= 320;
			P.G01.P03_Rated_frequency= 60;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1750;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 25;
			P.G01.P08_Supply_voltage= 380;


/*			P.G01.P00_Rated_power_x10_kW= 550;
			P.G01.P01_Rated_voltage= 415;
			P.G01.P02_Rated_current_x10= 1060;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1465;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 380;
*/
/*
			P.G01.P00_Rated_power_x10_kW= 185; // 18.5kW
			P.G01.P01_Rated_voltage= 415;
			P.G01.P02_Rated_current_x10= 350; //35.0A
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1465;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz=50; // 2.5kHz
			P.G01.P08_Supply_voltage= 380;
*/
			break;
		case 10: 
			P.G00.P10_Drive_power_x10_kW = 750;
			P.G00.P09_IGBT_current= 35.; 	
			P.G01.P00_Rated_power_x10_kW= 750;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 11: 
			P.G00.P10_Drive_power_x10_kW = 900; 
			P.G00.P09_IGBT_current= 35.;
			P.G01.P00_Rated_power_x10_kW= 900;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 12: 
			P.G00.P10_Drive_power_x10_kW = 1100;
			P.G00.P09_IGBT_current= 35.;
			P.G01.P00_Rated_power_x10_kW= 1100;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 13: 
			P.G00.P10_Drive_power_x10_kW = 1320; 
			P.G00.P09_IGBT_current= 35.;
			P.G01.P00_Rated_power_x10_kW= 1320;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 14: 
			I_scale= 100.0/0.9 *(820./176.3);
			P.G00.P10_Drive_power_x10_kW = 1600; 
			P.G00.P09_IGBT_current= 247.;	
			P.G01.P00_Rated_power_x10_kW= 550;
			P.G01.P01_Rated_voltage= 415;
			P.G01.P02_Rated_current_x10= 1060;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1465;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 25;
			P.G01.P08_Supply_voltage= 690; 

			/*	// 400V 315kW motor
			P.G01.P00_Rated_power_x10_kW= 3150;
			P.G01.P01_Rated_voltage= 380;
			P.G01.P02_Rated_current_x10= 5250;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1480;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 25;
			P.G01.P08_Supply_voltage= 690;
			*/
			break;
		case 15:
			I_scale= 100.0/1.11*(1640./176.3); 
			P.G00.P10_Drive_power_x10_kW = 2000;
			P.G00.P09_IGBT_current= 540.;
			P.G01.P00_Rated_power_x10_kW= 3150;
			P.G01.P01_Rated_voltage= 380.;
			P.G01.P02_Rated_current_x10= 5250;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1480;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 25;
			P.G01.P08_Supply_voltage= 380;
			break;
		case 16: 
			P.G00.P10_Drive_power_x10_kW = 2500;
			P.G00.P09_IGBT_current= 35.;
			P.G01.P00_Rated_power_x10_kW= 2500;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 17: 
			P.G00.P10_Drive_power_x10_kW = 3150;
			P.G00.P09_IGBT_current= 35.; 
			P.G01.P00_Rated_power_x10_kW= 3150;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 18: 
			I_scale= 100.0/1.05 *(1366.7/176.3);
			P.G00.P10_Drive_power_x10_kW= 4000; 
			P.G00.P09_IGBT_current= 525.;
			P.G01.P00_Rated_power_x10_kW= 1100;
			P.G01.P01_Rated_voltage= 1140;
			P.G01.P02_Rated_current_x10= 680;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1489;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 25;
			P.G01.P08_Supply_voltage= 1200; 
			break;
		case 19:
		 	P.G00.P10_Drive_power_x10_kW = 5000; 
		 	P.G00.P09_IGBT_current= 35.;	
			P.G01.P00_Rated_power_x10_kW= 5000;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 20: 
			P.G00.P10_Drive_power_x10_kW = 5600; 
			P.G00.P09_IGBT_current= 35.;
			P.G01.P00_Rated_power_x10_kW= 5600;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 21: 
			I_scale= 100.0/0.9 *(410./176.3);
			P.G00.P10_Drive_power_x10_kW= 6300; 
			P.G00.P09_IGBT_current= 525.;
			P.G01.P00_Rated_power_x10_kW= 6300;
			P.G01.P01_Rated_voltage= 1140.;
			P.G01.P02_Rated_current_x10= 4500;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 1140;
			break;
		case 22: 
			P.G00.P10_Drive_power_x10_kW = 7150; 
			P.G00.P09_IGBT_current= 35.;
			P.G01.P00_Rated_power_x10_kW= 7150;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 23: 
			P.G00.P10_Drive_power_x10_kW = 7800; 
			P.G00.P09_IGBT_current= 35.;
			P.G01.P00_Rated_power_x10_kW= 7800;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 30: 
			P.G00.P10_Drive_power_x10_kW = 22; 
			P.G00.P09_IGBT_current= 35.;
			P.G01.P00_Rated_power_x10_kW= 22;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
		case 31: 
			P.G00.P10_Drive_power_x10_kW = 37; 
			P.G00.P09_IGBT_current= 35.;
			P.G01.P00_Rated_power_x10_kW= 37;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P08_Supply_voltage= 230;
			break;
	} 
}

void Parameter_Initialization()
{
	Flag.Fault1.all= 0;
	Flag.Fault2.all= 0;	
	Fault_count= 0;
	
	OP.Run_stop.bit.Emergency_STOP= 0;
	OP.Run_stop.bit.Local= 0;
	OP.Run_stop.bit.Local_RUN= 0;					
	OP.Run_stop.bit.Local_DIR= 0;					
	OP.Reference.Local.Speed= 0;					
	OP.Reference.Local.Frequency_x10= 0;			
	OP.Reference.Local.Torque_x1000= 0;		
	OP.Reference.Local.PID_x1000= 0;				
	OP.Run_stop.bit.Fieldbus_RUN= 0;				
	OP.Run_stop.bit.Fieldbus_DIR= 0;				
	OP.Reference.Fieldbus.Speed= 0;				
	OP.Reference.Fieldbus.Frequency_x10= 0;		
	OP.Reference.Fieldbus.Torque_x1000=	0;		 
	OP.Reference.Fieldbus.PID_x1000= 0;				
	OP.Run_stop.bit.Emergency_STOP= 0;
	OP.Run_stop.bit.AUTO_TUNING= 0;
					
	P.G00.P00_Device_number= 11;	 				
	P.G00.P01_Software_version1= 200;			
	P.G00.P02_Software_version2= 'A';			
	P.G00.P03_Software_version3= 0;			
	P.G00.P08_Password= 0;
												// default //
	P.G03.P00_Run_stop_source= 0; 				// 0: terminal
	P.G03.P01_Reference_source= 0;				// 0: terminal
	P.G03.P03_Stop_mode= 0;						// ramp stop
	P.G03.P04_Stop_hold_time= 0;				// 0s
	P.G03.P05_Output_off_hold_time= 1; 			// 1s
	P.G03.P09_Accel_switching_ref1_x1000= 1000;		// 100%
	P.G03.P10_Accel_switching_ref2_x1000= 1500;		// 150%
	P.G03.P16_Accel_time1_x10= 100;					// 5s
	P.G03.P17_Accel_time2_x10= 100;					// 5s
	P.G03.P26_Decel_switching_ref1_x1000= 1000;			// 100%	
	P.G03.P27_Decel_switching_ref2_x1000= 1500;		// 150%	
	P.G03.P33_Decel_time1_x10= 100;					// 5s	 
	P.G03.P34_Decel_time2_x10= 100;					// 5s
	P.G03.P42_Conunter_deceleration_ramp= 0;	// disabled
	P.G03.P43_Conunter_deceleration_time_x10= 50;	// 5s
	P.G03.P49_Emergency_stop_mode= 0;			// ramp stop
	P.G03.P50_Emergency_stop_decel_time_x10= 10;	// 1s
	P.G03.P51_S_curve_enable= 0;
	P.G03.P52_S_curve_weight_x1000= 800;

	P.G05.P00_Current_limit_x1000= 1300;				// 130%
	P.G05.P07_Max_continuous_current_x1000= 950;		// 95%
	P.G05.P08_Overload_current_x1000= 1350;			// 135%
	P.G05.P09_Over_load_time_x10= 600;				// 60s
	P.G05.P10_Over_load_fault_action= 0;		// normal stop
	P.G05.P11_Over_current_trip_x1000= 1300;			// 200% -> 130%
	P.G05.P12_Zero_sequence_current_trip_x1000= 0.15;	// 15%
	P.G05.P13_Over_voltage_limiting_function= 0;// disabled
	#if (VOLTAGE_CLASS== 0)
		P.G05.P14_Over_voltage_limit= 700;			
		P.G05.P15_Over_voltage_trip= 750;			
	#elif (VOLTAGE_CLASS== 1)
		P.G05.P14_Over_voltage_limit= 1100;			 
		P.G05.P15_Over_voltage_trip= 1150;		
	#elif (VOLTAGE_CLASS== 2)
		P.G05.P14_Over_voltage_limit= 1800;			
		P.G05.P15_Over_voltage_trip= 1850;		
	#else
		P.G05.P14_Over_voltage_limit= 0;		
		P.G05.P15_Over_voltage_trip= 0;	
	#endif				

	P.G05.P16_Under_voltage_compensation= 0;	// disabled
	P.G05.P17_UV_compensation_voltage= 450;		// 270V 임의값

	#if (VOLTAGE_CLASS== 0)
//		P.G05.P17_UV_compensation_voltage= 430;
//		P.G05.P18_Under_voltage_trip= 450;
		P.G05.P17_UV_compensation_voltage= 380;
		P.G05.P18_Under_voltage_trip= 400; 		 			
	#elif (VOLTAGE_CLASS== 1)
		P.G05.P17_UV_compensation_voltage= 750;
		P.G05.P18_Under_voltage_trip= 780; 			
	#elif (VOLTAGE_CLASS== 2)
		P.G05.P17_UV_compensation_voltage= 1350;
		P.G05.P18_Under_voltage_trip= 1450;			
	#else
		P.G05.P17_UV_compensation_voltage= 2000;
		P.G05.P18_Under_voltage_trip= 2000;	
	#endif		

	P.G05.P19_Open_phase_protection= 0;			// disabled
	P.G05.P20_Supply_frequency= 60;				// 60 Hz
	P.G05.P21_Built_in_dynamic_brake= 0;		// disabled
	P.G05.P22_DB_switching_frequency= 5.0;		// 5.0 임의값 
	#if (VOLTAGE_CLASS== 0)
		P.G05.P23_DB_start_voltage= 600;		
		P.G05.P24_DB_full_voltage= 650;				
	#elif (VOLTAGE_CLASS== 1)
		P.G05.P23_DB_start_voltage= 970;			
		P.G05.P24_DB_full_voltage= 1050;					
	#elif (VOLTAGE_CLASS== 2)
		P.G05.P23_DB_start_voltage= 1800;		
		P.G05.P24_DB_full_voltage= 1900;				
	#else
		P.G05.P23_DB_start_voltage= 0;		
		P.G05.P24_DB_full_voltage= 10;	
	#endif		

	P.G05.P25_Over_temperature_trip_action= 0;	// Control off
	P.G05.P30_Auto_restart_count= 0;			// 0
	P.G05.P31_Retry_delay_time_x10= 15;			// 1.5s
	P.G05.P32_OC_auto_reset= 0;					// disabled
	P.G05.P33_OV_auto_reset= 0;					// disabled
	P.G05.P34_UV_auto_reset= 0;					// disabled
	P.G05.P37_Out_of_control_auto_reset= 0;		// disabled
	P.G05.P38_Out_of_control_time_x10= 50;			// 5.0s
	P.G05.P39_Out_of_control_current_x1000= 900;		// 90%
	P.G05.P40_Over_temperature_trip_x10= 850;		// 85 deg
	P.G05.P42_Line_under_voltage_trip_x1000= 900;		// 90%
	P.G05.P43_Line_under_voltage_auto_reset= 0;	// disabled
	P.G05.P44_Line_unbalance_auto_reset= 0;		// disabled

	P.G06.AI0_Analog_reference_source= 1;		// AI 1
	P.G06.AI1.P01_Function= 1;					// AI
	P.G06.AI1.P02_Type= 0;						// 0~10V
	P.G06.AI1.P03_Filter_time_constant_x10_mA= 5000;	// 0.5s
	P.G06.AI1.P04_Offset_x10= 0;					// 0
	P.G06.AI1.P05_Minimum_x10= 50;			// 5%
	P.G06.AI1.P06_Maximum_x10= 1000;			// 100%
	P.G06.AI1.P07_Inversion= 0;					// disabled
	P.G06.AI1.P08_Discretness= 0;				// disabled
	P.G06.AI1.P09_Unit_selection= 1;			// %
	P.G06.AI2.P01_Function= 1;					// AI
	P.G06.AI2.P02_Type= 0;						// 0~10V
	P.G06.AI2.P03_Filter_time_constant_x10_mA= 5000;	// 0.5s
	P.G06.AI2.P04_Offset_x10= 0;					// 0
	P.G06.AI2.P05_Minimum_x10= 50;			// 5%
	P.G06.AI2.P06_Maximum_x10= 1000;			// 100%
	P.G06.AI2.P07_Inversion= 0;					// disabled
	P.G06.AI2.P08_Discretness= 0;				// disabled
	P.G06.AI2.P09_Unit_selection= 1;			// %
/*
	P.G06.AI3.P01_Function= 1;					// AI
	P.G06.AI3.P02_Type= 0;						// 0~10V
	P.G06.AI3.P03_Filter_time_constant_x10_mA= 5000;	// 0.5s
	P.G06.AI3.P04_Offset_x10= 0;					// 0
	P.G06.AI3.P05_Minimum_x10= 50;			// 5%
	P.G06.AI3.P06_Maximum_x10= 1000;			// 100%
	P.G06.AI3.P07_Inversion= 0;					// disabled
	P.G06.AI3.P08_Discretness= 0;				// disabled
	P.G06.AI3.P09_Unit_selection= 1;			// %
	P.G06.AI4.P01_Function= 1;					// AI
	P.G06.AI4.P02_Type= 0;						// 0~10V
	P.G06.AI4.P03_Filter_time_constant_x10_mA= 5000;	// 0.5s
	P.G06.AI4.P04_Offset_x10= 0;					// 0
	P.G06.AI4.P05_Minimum_x10= 50;			// 5%
	P.G06.AI4.P06_Maximum_x10= 1000;			// 100%
	P.G06.AI4.P07_Inversion= 0;					// disabled
	P.G06.AI4.P08_Discretness= 0;				// disabled
	P.G06.AI4.P09_Unit_selection= 1;			// % 
	P.G06.AI5.P01_Function= 1;					// AI
	P.G06.AI5.P02_Type= 0;						// 0~10V
	P.G06.AI5.P03_Filter_time_constant_x10_mA= 5000;	// 0.5s
	P.G06.AI5.P04_Offset_x10= 0;					// 0
	P.G06.AI5.P05_Minimum_x10= 50;			// 5%
	P.G06.AI5.P06_Maximum_x10= 1000;			// 100%
	P.G06.AI5.P07_Inversion= 0;					// disabled
	P.G06.AI5.P08_Discretness= 0;				// disabled
	P.G06.AI5.P09_Unit_selection= 1;			// % 
*/

	P.G07.P00_Control_mode= 0;					// disabled

	P.G08.P00_Run_stop_control= 0;				// FWD/REV
	P.G08.P01_DI03_function= 2;					// none	- > MULTI_STEP_BIT0
	P.G08.P02_DI04_function= 3;					// none	- > MULTI_STEP_BIT1	
	P.G08.P03_DI05_function= 4;					// none	- > MULTI_STEP_BIT2			
	P.G08.P04_DI06_function= 5;					// none	- > MULTI_STEP_BIT3	
	P.G08.P05_DI07_function= 0;					// none			
	P.G08.P06_DI08_function= 0;					// none	
	P.G08.P07_DI09_function= 0;					// none			
	P.G08.P08_DI10_function= 0;					// none	
	P.G08.P09_DI11_function= 0;					// none			
	P.G08.P10_DI12_function= 0;					// none	
	P.G08.P11_Run_delay_time= 0;				// 0s


	P.G09.P00_JOG_set_x1000= 20;						// 20%
	P.G09.P01_Step01_set_x10= 100;					// 15%	
	P.G09.P02_Step02_set_x10= 200;					// 30%
	P.G09.P03_Step03_set_x10= 300;					// 50%
	P.G09.P04_Step04_set_x10= 400;					// 100%
	P.G09.P05_Step05_set_x10= 500;					// 100%		
	P.G09.P06_Step06_set_x10= 600;					// 100%
	P.G09.P07_Step07_set_x10= 700;					// 100%		
	P.G09.P08_Step08_set_x10= 800;					// 100%
	P.G09.P09_Step09_set_x10= 900;					// 100%		
	P.G09.P10_Step10_set_x10= 1000;					// 100%
	P.G09.P11_Step11_set_x10= 1000;					// 100%		
	P.G09.P12_Step12_set_x10= 1000;					// 100%
	P.G09.P13_Step13_set_x10= 1000;					// 100%		
	P.G09.P14_Step14_set_x10= 1000;					// 100%
	P.G09.P15_Step15_set_x10= 1000;					// 100%	
	P.G09.P16_Unit_selection= 0;				// percent[%]

	P.G11.AO1.P00_Output_selection= 1;			// motor speed
	P.G11.AO1.P01_Type= 0;
	P.G11.AO1.P02_Adjustment_0mA= 0;
	P.G11.AO1.P03_Adjustment_4mA= 0;
	P.G11.AO1.P04_Adjustment_20mA= 0;
	P.G11.AO1.P05_Max_output_x1000= 1000;				// 100%
	P.G11.AO1.P06_Inversion= 0;					// disabled
	P.G11.AO2.P00_Output_selection= 0;			// output frequency
	P.G11.AO2.P01_Type= 1;
	P.G11.AO2.P02_Adjustment_0mA= 0;
	P.G11.AO2.P03_Adjustment_4mA= 0;
	P.G11.AO2.P04_Adjustment_20mA= 0;
	P.G11.AO2.P05_Max_output_x1000= 1000;
	P.G11.AO2.P06_Inversion= 0;	
		
	P.G12.P00_DO1_function= 0;					// disabled
	P.G12.P00_DO2_function= 0;	
	P.G12.P00_DO3_function= 0;

	P.G13.P00_M1_locked_state_up_speed_up_x1000= 4000;// 4%

	P.G14.P09_Dead_time_compansation_x100_us= 16;

	P.G19.P00_Number_of_encoder_pulses= 8192;	//1024
	P.G19.P01_Inversion_of_PG_direction= 0;		//disabled
	P.G19.P03_Minimum_speed= 0;					// 0 rpm
	P.G19.P04_Maximum_speed_x1000= 1000;					// 100%
	P.G19.P05_Over_speed_limit_x1000= 1250;			// 125%

	// leroy somer 1.5kW motor 
/*	P.G21.P00_Stator_resistance_x1000= 2000; //0.055 ohm
	P.G21.P01_Rotor_resistance_x1000= 900;
	P.G21.P02_Stator_inductance_x1000= 160;  // 0.018 H
	P.G21.P03_Rotor_inductance_x1000= 160;
	P.G21.P04_Stator_transient_inductance_x1000= 12;
	P.G21.P05_Inertia_x1000= 5;
*/

	// jiamusi 110kW 1140V motor 
/*	P.G21.P00_Stator_resistance_x1000= 87;
	P.G21.P01_Rotor_resistance_x1000= 21;
	P.G21.P02_Stator_inductance_x1000= 116;
	P.G21.P03_Rotor_inductance_x1000= 115;
	P.G21.P04_Stator_transient_inductance_x1000= 5;
	P.G21.P05_Inertia_x1000= 100;
*/

	// siemens 55kW motor (PS 발전기) Lsigma= Ls-(Lm*Lm/Lr);
/*
	// 400V
	P.G21.P00_Stator_resistance_x1000= 55; //0.055 ohm
	P.G21.P01_Rotor_resistance_x1000= 42; 
	P.G21.P02_Stator_inductance_x1000= 17;  // 0.018 H
	P.G21.P03_Rotor_inductance_x1000= 17;
	P.G21.P04_Stator_transient_inductance_x1000= 2;
	P.G21.P05_Inertia_x1000= 30; 
*/

	// 600V
/*	P.G21.P00_Stator_resistance_x1000= 49; //0.055 ohm
	P.G21.P01_Rotor_resistance_x1000= 40; 
	P.G21.P02_Stator_inductance_x1000= 18;  // 0.018 H
	P.G21.P03_Rotor_inductance_x1000= 18;
	P.G21.P04_Stator_transient_inductance_x1000= 2;
	P.G21.P05_Inertia_x1000= 50;
*/

/*	// siemens 18.5kW motor
	P.G21.P00_Stator_resistance_x1000= 282; //0.282 ohm
	P.G21.P01_Rotor_resistance_x1000= 99;
	P.G21.P02_Stator_inductance_x1000= 51;  // 0.051 H
	P.G21.P03_Rotor_inductance_x1000= 51;
	P.G21.P04_Stator_transient_inductance_x1000= 4;
	P.G21.P05_Inertia_x1000= 10;
*/
/*	// PS 부하용 11kW Motor
	P.G21.P00_Stator_resistance_x1000= 450;
	P.G21.P01_Rotor_resistance_x1000= 151;
	P.G21.P02_Stator_inductance_x1000= 77;
	P.G21.P03_Rotor_inductance_x1000= 76;
	P.G21.P04_Stator_transient_inductance_x1000= 7;
	P.G21.P05_Inertia_x1000= 10;
*/
	// Blade Pitch Control 실험 9.5kW Motor
	P.G21.P00_Stator_resistance_x1000= 336;
	P.G21.P01_Rotor_resistance_x1000= 195;
	P.G21.P02_Stator_inductance_x1000= 82;
	P.G21.P03_Rotor_inductance_x1000= 82;
	P.G21.P04_Stator_transient_inductance_x1000= 7;
	P.G21.P05_Inertia_x1000= 10;

}

void System_Variable_Initialization()
{
	double OL_current;

	Theta_angle_ref = 0;
	Theta_ref= 0.;



//	Flag.DI.all= 0;
	Flag.DO.all= 0;
	Flag.Fault1.all= 0;
	Flag.Fault2.all= 0;
	Flag.Fault_Neglect1.all= 0;
	Flag.Fault_Neglect2.all= 0;
//	Flag.Fault_Neglect1.bit.OVER_SPEED= 1;
	Flag.Monitoring.all= 0;
	Flag.Monitoring.bit.FAULT_RESET_COMPLETE= 1; 
//	OP.Run_stop.all= 0;
	FaultInfo.DATA= 0.;
	Voltage_class= 0;
	Command= CMD_STOP;
	Init_reference= 0.;

	

	PWM_frequency_old= P.G01.P07_PWM_frequency_x10_kHz;

#if (DUAL_PWM_INTERRUPT)
	Tsamp= Tsamp_VF= Tsamp_CC= 1./((double)P.G01.P07_PWM_frequency_x10_kHz*1.e+2)/2.;
#else
	Tsamp= Tsamp_VF= Tsamp_CC= 1./((double)P.G01.P07_PWM_frequency_x10_kHz*1.e+2);
#endif 
 
	Tsamp_SC= Tsamp_SD= Tsamp_FC= Tsamp*4.;
	Period_SD= (Uint32)(CPU_CLOCK*Tsamp*4.); 		// EQEP Timer Period
	
//	Tsamp_SD= Tsamp*2.;
//	Tsamp_SC= Tsamp_FC= Tsamp*4.;
//	Period_SD= (Uint32)(CPU_CLOCK*Tsamp*2.); 		// EQEP Timer Period HHH 

	Vdc_max=0;
	I_DT= (double)P.G00.P09_IGBT_current*0.01;
	T_dead_Tuning= (double)P.G14.P09_Dead_time_compansation_x100_us * 1.e-2;

//--------Motor Parameters--------//
//	Rs= 0.28175;
//	Rr= 0.09948;
// 	Ls= 0.05135;
//	Lr= 0.05084;
//	Lsigma= 4.09e-3;


	Rs= (double)(P.G21.P00_Stator_resistance_x1000)*1.e-3;
	Rr= (double)(P.G21.P01_Rotor_resistance_x1000)*1.e-3;
 	Ls= (double)(P.G21.P02_Stator_inductance_x1000)*1.e-3;
	Lr= (double)(P.G21.P03_Rotor_inductance_x1000)*1.e-3;
	Lsigma= (double)(P.G21.P04_Stator_transient_inductance_x1000)*1.e-3;
	Lm= sqrt((Ls-Lsigma)*Lr);
	Jm= (double)(P.G21.P05_Inertia_x1000)*1.e-3;
	inv_Jm=1./Jm;
	Pole_pair=(double)(P.G01.P04_Number_of_poles)/2.; Bm= 0.; 
	LAMdre_rate = 0.45*(double)(P.G01.P08_Supply_voltage)/220.;
	LAMdre_ref = 0.45*(double)(P.G01.P08_Supply_voltage)/220.; 
//	Is_rate=(20.6*SQRT2), Wrpm_rate=1740., 
	// Wrpm_max=3500.;
	PPR= P.G19.P00_Number_of_encoder_pulses;

	// 정격출력(W)/정격속도(rad/sec) -> 정격토크
	// 정격토크 = 정격출력/( (2xPI/60) x rpm)
	//         =  정격출력 x (30/PI) / rpm 
	// Te_rated_f = (1.0e3*(1.0e-1*P.G01.P00_Rated_power_x10_kW)*30)/(PI * P.G01.P05_Rated_speed) 
	// 				->	1.0e2 x 30 / PI x (P.G01.P00_Rated_power_x10_kW / P.G01.P05_Rated_speed)
	//				->	954.9305 x (P.G01.P00_Rated_power_x10_kW / P.G01.P05_Rated_speed) 
	Te_rate =  (954.9305*(double)P.G01.P00_Rated_power_x10_kW)/(double)P.G01.P05_Rated_speed;
 
//------Motor Parameters End------//  
	
	Is_max= ((double)P.G01.P02_Rated_current_x10)*0.1*SQRT2;
	if (Is_max>((double)P.G00.P09_IGBT_current*0.85))	OC_trip_value= (double)P.G00.P09_IGBT_current*0.85;
	else OC_trip_value= Is_max;

	OL_current= Is_max*(double)P.G05.P08_Overload_current_x1000*1.e-3;
	Continuous_OL_current= Is_max*(double)P.G05.P07_Max_continuous_current_x1000*1.e-3;
	OL_limit= (OL_current*OL_current - Continuous_OL_current*Continuous_OL_current)*(double)P.G05.P09_Over_load_time_x10*1.e-1;


	// Motor Parmeter
 //	Ls = Lr = (Lsigma + sqrt(Lsigma*Lsigma + 4.*Lm*Lm) ) / 2.;


	

	Init_reference=0.;
	Final_reference=0.; 
//	Wrpm_scale=(double)P.G01.P05_Rated_speed*(double)P.G01.P08_Supply_voltage/(double)P.G01.P01_Rated_voltage;
	Wrpm_scale=(double)P.G01.P05_Rated_speed;
	Accel_time=(double)P.G03.P16_Accel_time1_x10 * 0.1;	
	Decel_time=(double)P.G03.P33_Decel_time1_x10 * 0.1;
	 
	Theta=0.0;

	// 전류
	Is_mag=0.0;
	Is_mag_rms=0.0;


	// Flux Estimation  
	K_CM1= (1.-(Tsamp_CC/2.)*(Rr/Lr))/(1.+(Tsamp_CC/2.)*(Rr/Lr));
	K_CM2= (Tsamp_CC/2.)*(Rr/Lr)*Lm/(1.+(Tsamp_CC/2.)*(Rr/Lr));

	Freq_Fst= 30.;  // before modification 50
	Wc_Fst= (2.*PI)*Freq_Fst;
	Kp_Fst= (Lm/Lr)*SQRT2*Wc_Fst;
	Ki_Fst= (Lm/Lr)*Wc_Fst*Wc_Fst;

	// Current Controller
	Wc_cc= 2000.*(double)P.G01.P07_PWM_frequency_x10_kHz/50.; // 700 //1250
	Kp_cc= Lsigma*Wc_cc; 
	Ki_cc= Rs*2.*Wc_cc;  
	Ka_cc= 3./Kp_cc;

	// Flux Controller
	Wc_fc= 80.*(double)P.G01.P07_PWM_frequency_x10_kHz/50.;  // 50
	Kp_fc= (Lr/Lm)*Wc_fc/Rr;
	Ki_fc= Wc_fc/Lm;
	Ka_fc= 2./Kp_fc;
	Idse_ref_max = Is_max/SQRT2; 		
	

	// Speed Controller 
	Wc_sc= 400.*(double)P.G01.P07_PWM_frequency_x10_kHz/50.; // 100 //250
//	Kp_sc= Jm*Wc_sc/Kt;
//	Ki_sc= Kp_sc*(Wc_sc/7.);
//	Ka_sc= 2./Kp_sc;

	// Speed_Detection
	EQep1Regs.QPOSLAT= 0;	//encoder
	Kob_sd=	1000.*200.e-6/(Tsamp_SD);	// Speed Observer 1000
//	Kob_sd=	250.;	// Speed Observer 1000
	L1_sp_est= 3.*Kob_sd;
	L2_sp_est= 3.*Kob_sd*Kob_sd;
	L3_sp_tmp= -(Kob_sd*Kob_sd*Kob_sd);
//	L3_sp_est= -(SPEED_OB_GAIN*SPEED_OB_GAIN*SPEED_OB_GAIN)*Jm;

	//Field Weakening
	K_fw1= (LAMdre_rate*LAMdre_rate)*(Ls*Ls-Lsigma*Lsigma)/(Lm*Lm)
	           +(Lsigma*Lsigma)*(Is_max*Is_max);
	K_fw2= sqrt( (Ls*Ls+Lsigma*Lsigma)/(2.*Ls*Ls*Lsigma*Lsigma) );
/*
	cs_count =cs;
	//  M = (Td + Ton - Toff) * ISR_FREQUENCY;//[(Td + Ton - Toff)/1000]/T=(d + Ton - Toff)*ISR_FREQUENCY;(pu).used by voltage estimation
	M = (Td + Ton - Toff) * 2.5;//[(Td + Ton - Toff)/1000]/T=(d + Ton - Toff)*ISR_FREQUENCY;(pu).used by voltage estimation/
	M1 = M * (Vd-Vs);//used by voltage estimation.
	C1 = 1.0/(6.2831853 * Hui * T+1.0);//RC/( T + RC);//
	C2 = 1.0 - C1;

	A2 = Wc * T * a2;
	A1 = Wc * Wc * T * a1;
	A0 = Wc * Wc * Wc * a0 * T;
	B1 = (1 + predict_weight) * 0.5;
	B2 = (1 - predict_weight) * 0.5; 
*/


}



void System_Variable_Update()
{
	double OL_current;

	Rs= (double)(P.G21.P00_Stator_resistance_x1000)*1.e-3;
	Rr= (double)(P.G21.P01_Rotor_resistance_x1000)*1.e-3;
 	Ls= (double)(P.G21.P02_Stator_inductance_x1000)*1.e-3;
	Lr= (double)(P.G21.P03_Rotor_inductance_x1000)*1.e-3;
	Lsigma= (double)(P.G21.P04_Stator_transient_inductance_x1000)*1.e-3;
	Lm= sqrt((Ls-Lsigma)*Lr);
	Jm= (double)(P.G21.P05_Inertia_x1000)*1.e-3;
	inv_Jm=1./Jm;
	Pole_pair=(double)(P.G01.P04_Number_of_poles)/2.; Bm= 0.; 
	LAMdre_rate = 0.45*(double)(P.G01.P08_Supply_voltage)/220.;
	LAMdre_ref = 0.45*(double)(P.G01.P08_Supply_voltage)/220.;  
	PPR= P.G19.P00_Number_of_encoder_pulses;
 	
	Te_rate =  (954.9305*(double)P.G01.P00_Rated_power_x10_kW)/(double)P.G01.P05_Rated_speed;
//------Motor Parameters End------//  
	
	Is_max= ((double)P.G01.P02_Rated_current_x10)*0.1*SQRT2;
	if (Is_max>((double)P.G00.P09_IGBT_current*0.85))	OC_trip_value= (double)P.G00.P09_IGBT_current*0.85;
	else OC_trip_value= Is_max;
	
	OL_current= Is_max*(double)P.G05.P08_Overload_current_x1000*1.e-3;
	Continuous_OL_current= Is_max*(double)P.G05.P07_Max_continuous_current_x1000*1.e-3;
	OL_limit= (OL_current*OL_current - Continuous_OL_current*Continuous_OL_current)*(double)P.G05.P09_Over_load_time_x10*1.e-1;

//	Wrpm_scale=(double)P.G01.P05_Rated_speed*(double)P.G01.P08_Supply_voltage/(double)P.G01.P01_Rated_voltage;
	Wrpm_scale=(double)P.G01.P05_Rated_speed;
	Accel_time=(double)P.G03.P16_Accel_time1_x10 * 0.1;	
	Decel_time=(double)P.G03.P33_Decel_time1_x10 * 0.1;
	 

//	Freq_Fst= 10.;
//	Wc_Fst= (2.*PI)*Freq_Fst;
	Kp_Fst= (Lm/Lr)*SQRT2*Wc_Fst;
	Ki_Fst= (Lm/Lr)*Wc_Fst*Wc_Fst;

	// Current Controller
	Kp_cc= Lsigma*Wc_cc; 
	Ki_cc= Rs*2.*Wc_cc;  
	Ka_cc= 3./Kp_cc;

	// Flux Controller
	Kp_fc= (Lr/Lm)*Wc_fc/Rr;
	Ki_fc= Wc_fc/Lm;
	Ka_fc= 2./Kp_fc;
	Idse_ref_max = Is_max/SQRT2; 		


	//Field Weakening
	K_fw1= (LAMdre_rate*LAMdre_rate)*(Ls*Ls-Lsigma*Lsigma)/(Lm*Lm)
	           +(Lsigma*Lsigma)*(Is_max*Is_max);
	K_fw2= sqrt( (Ls*Ls+Lsigma*Lsigma)/(2.*Ls*Ls*Lsigma*Lsigma) );


	if (P.G01.P07_PWM_frequency_x10_kHz!= PWM_frequency_old)
	{
		Flag.Monitoring.bit.PWM_FREQUENCY_CHANGE= 1;
		PWM_frequency_old= P.G01.P07_PWM_frequency_x10_kHz;
	}

}

// PWM Freq. 가 변경 될경우 초기화를 다시 해줘야 한다. 
// Main loop 로 이동하여 Interrupt를 막고 초기 진행한 후 Interrupt 재시작  
void PWM_Frequency_Update()
{

#if (DUAL_PWM_INTERRUPT)
	Tsamp= Tsamp_VF= Tsamp_CC= 1./((double)P.G01.P07_PWM_frequency_x10_kHz*1.e+2)/2.;
#else
	Tsamp= Tsamp_VF= Tsamp_CC= 1./((double)P.G01.P07_PWM_frequency_x10_kHz*1.e+2);
#endif 
 
	Tsamp_SC= Tsamp_SD= Tsamp_FC= Tsamp*4.;
	Period_SD= (Uint32)(CPU_CLOCK*Tsamp*4.); 		// EQEP Timer Period
	
	// Flux Estimation  
	K_CM1= (1.-(Tsamp_CC/2.)*(Rr/Lr))/(1.+(Tsamp_CC/2.)*(Rr/Lr));
	K_CM2= (Tsamp_CC/2.)*(Rr/Lr)*Lm/(1.+(Tsamp_CC/2.)*(Rr/Lr));

	// Current Controller
	Wc_cc= 2000.*(double)P.G01.P07_PWM_frequency_x10_kHz/50.; //1250
	Kp_cc= Lsigma*Wc_cc; 
	Ki_cc= Rs*2.*Wc_cc;  
	Ka_cc= 3./Kp_cc;

	// Flux Controller
	Wc_fc= 80.*(double)P.G01.P07_PWM_frequency_x10_kHz/50.;  // 80
	Kp_fc= (Lr/Lm)*Wc_fc/Rr;
	Ki_fc= Wc_fc/Lm;
	Ka_fc= 2./Kp_fc;


	// Speed Controller 
	Wc_sc= 400.*(double)P.G01.P07_PWM_frequency_x10_kHz/50.;

	// Speed Observer 
	EQep1Regs.QPOSLAT= 0;
	Kob_sd=	1000.*200.e-6/(Tsamp_SD);	
	L1_sp_est= 3.*Kob_sd;
	L2_sp_est= 3.*Kob_sd*Kob_sd;
	L3_sp_tmp= -(Kob_sd*Kob_sd*Kob_sd);

}






//---------------------------------
// end of file
//----------------------------------

