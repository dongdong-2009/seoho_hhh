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
	
	switch ( Drive_power )
	{
		case 0:       
			P.G00.P10_Drive_power_x10_kW = 55;
			I_scale= 50.0/1.8; //200V : 50/1.8, 400V 25/1.8
			P.G00.P09_IGBT_current_x10= 250;	//FP25R12KE3
			P.G01.P00_Rated_power_x10_kW= 15;
			P.G01.P01_Rated_voltage= 230;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P09_Supply_voltage= 230;

			Td = 4.0/1000;// dead-time (ms)
			Ton = 0.085/1000;//1.0/1000; Switching Times of IGBT (ms)
			Toff = 0.42/1000;//2.0/1000;Switching Times of IGBT (ms)
			Vs = 1.8;//1.8; Collector-Emitter Saturation Voltage (v)
			Vd = 1.75;//1.75; FWD Forward Voltage (v)	
			Wc = 20;// the cutoff frequency of third-order butterworth digital filter (Hz)
			exciting_step = 0.005;

			break;
		case 1:
			P.G00.P10_Drive_power_x10_kW = 75;
			I_scale= 50.0/1.43;
			P.G00.P09_IGBT_current_x10= 400;	//FP40R12KE3
			P.G01.P00_Rated_power_x10_kW= 75;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 25;
			P.G01.P09_Supply_voltage= 230;
			break; 	
		case 2:
		 	P.G00.P10_Drive_power_x10_kW = 110; 
			I_scale= 50.0/1.8;
			P.G00.P09_IGBT_current_x10= 500;	//BSM50GP120
			P.G01.P00_Rated_power_x10_kW= 110;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P09_Supply_voltage= 230;

			Td = 4.0/1000;// dead-time (ms)
			Ton = 0.16/1000;//0.16/1000;// Switching Times of IGBT (ms)
			Toff = 0.33/1000;//0.4/1000;// Switching Times of IGBT (ms)
			Vs = 1.75;// Collector-Emitter Saturation Voltage (v)
			Vd = 1.7;// FWD Forward Voltage (v)
			Wc=10;// the cutoff frequency of third-order butterworth digital filter (Hz)
			exciting_step = 0.005;

			break;
		case 3: 
			P.G00.P10_Drive_power_x10_kW = 150; 
			P.G00.P09_IGBT_current_x10= 750;	//FP75R12KE3
			P.G01.P00_Rated_power_x10_kW= 150;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P09_Supply_voltage= 230;
			break;
		case 4: 
			P.G00.P10_Drive_power_x10_kW = 185; 
			I_scale= 200.0/0.85;
			P.G00.P09_IGBT_current_x10= 1000;	//FP100R12KE3
			P.G01.P00_Rated_power_x10_kW= 185;
			P.G01.P01_Rated_voltage= 415;
			P.G01.P02_Rated_current_x10= 350;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1465;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P09_Supply_voltage= 380;
			break;
		case 5: // (110104 by HHH) «ˆ¿Á Ω««Ë¡ﬂ
			P.G00.P10_Drive_power_x10_kW = 220;
			I_scale= 100.0/0.9;
			P.G00.P09_IGBT_current_x10= 1000;	//BSM100GD12DN2


/*			//7.5kW
			P.G01.P00_Rated_power_x10_kW= 75;
			P.G01.P01_Rated_voltage= 380;
			P.G01.P02_Rated_current_x10= 163;
			P.G01.P03_Rated_frequency= 60;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1760;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P09_Supply_voltage= 380;
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
			P.G01.P09_Supply_voltage= 380;
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
			P.G01.P09_Supply_voltage= 380;
*/


			//18.5kW
			P.G01.P00_Rated_power_x10_kW= 185; // 18.5kW
			P.G01.P01_Rated_voltage= 415;
			P.G01.P02_Rated_current_x10= 350; //35.0A
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1465;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50; 
			P.G01.P09_Supply_voltage= 380;

			Td = 4.0/1000;// dead-time (ms)
			Ton = 0.16/1000;//0.16/1000;// Switching Times of IGBT (ms)
			Toff = 0.33/1000;//0.4/1000;// Switching Times of IGBT (ms)
			Vs = 1.75;// Collector-Emitter Saturation Voltage (v)
			Vd = 1.7;// FWD Forward Voltage (v)
			Wc=10;// the cutoff frequency of third-order butterworth digital filter (Hz)
			exciting_step = 0.005;

			break;
		case 6: 
			P.G00.P10_Drive_power_x10_kW = 300;
			P.G00.P09_IGBT_current_x10= 1500;	//SKM150GB12T4G
			P.G01.P00_Rated_power_x10_kW= 300;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P09_Supply_voltage= 230;
			break;
		case 7: 
			P.G00.P10_Drive_power_x10_kW = 370; 
			P.G00.P09_IGBT_current_x10= 1500;	//SKM150GB12T4G
			P.G01.P00_Rated_power_x10_kW= 370;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P09_Supply_voltage= 230;
			break;
		case 8: 
			P.G00.P10_Drive_power_x10_kW = 450;
			P.G00.P09_IGBT_current_x10= 2000;	//SKM200GB128D
			P.G01.P00_Rated_power_x10_kW= 450;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P09_Supply_voltage= 230;
			break;
		case 9: 
			// (110228 by HHH)
			I_scale= 100.0/0.9 *(410./176.3);
			P.G00.P10_Drive_power_x10_kW = 550;
			P.G00.P09_IGBT_current_x10= 3000;	//FF300R12KE4

			//37kW
			P.G01.P00_Rated_power_x10_kW= 370;
			P.G01.P01_Rated_voltage= 440;
			P.G01.P02_Rated_current_x10= 643;
			P.G01.P03_Rated_frequency= 60;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1775;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P09_Supply_voltage= 480;


/*			P.G01.P00_Rated_power_x10_kW= 550;
			P.G01.P01_Rated_voltage= 415;
			P.G01.P02_Rated_current_x10= 1060;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1465;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P09_Supply_voltage= 380;
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
			P.G01.P09_Supply_voltage= 380;
*/

			Td = 4.0/1000;// dead-time (ms)
			Ton = 0.2/1000;//1.0/1000; Switching Times of IGBT (ms)
			Toff = 0.5/1000;//2.0/1000;Switching Times of IGBT (ms)
			Vs = 1.75;//1.75; Collector-Emitter Saturation Voltage (v)
			Vd = 1.65;//1.65; FWD Forward Voltage (v)
			Wc=10;// the cutoff frequency of third-order butterworth digital filter (Hz)
			exciting_step = 0.005;

			break;
		case 10: 
			P.G00.P10_Drive_power_x10_kW = 750;
			I_scale= 100.0/0.9 *(574./176.3);
			P.G00.P09_IGBT_current_x10= 3000;	//FF300R12KE4 	
			P.G01.P00_Rated_power_x10_kW= 750;
			P.G01.P01_Rated_voltage= 415;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P09_Supply_voltage= 380;

			Td = 4.0/1000;// dead-time (ms)
			Ton = 0.2/1000;//1.0/1000; Switching Times of IGBT (ms)
			Toff = 0.5/1000;//2.0/1000;Switching Times of IGBT (ms)
			Vs = 1.75;//1.75; Collector-Emitter Saturation Voltage (v)
			Vd = 1.65;//1.65; FWD Forward Voltage (v)
			Wc=10;// the cutoff frequency of third-order butterworth digital filter (Hz)
			exciting_step = 0.005;
			break;
		case 11: 
			P.G00.P10_Drive_power_x10_kW = 900;
			I_scale= 100.0/0.9 *(656./176.3); 
			P.G00.P09_IGBT_current_x10= 4500;	//FF450R12KE4
			P.G01.P00_Rated_power_x10_kW= 370;
			P.G01.P01_Rated_voltage= 440;
			P.G01.P02_Rated_current_x10= 643;
			P.G01.P03_Rated_frequency= 60;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1775;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P09_Supply_voltage= 480;

			Td = 4.0/1000;// dead-time (ms)
			Ton = 0.2/1000;//1.0/1000; Switching Times of IGBT (ms)
			Toff = 0.5/1000;//2.0/1000;Switching Times of IGBT (ms)
			Vs = 1.75;//1.75; Collector-Emitter Saturation Voltage (v)
			Vd = 1.7;//1.65; FWD Forward Voltage (v)
			Wc=10;// the cutoff frequency of third-order butterworth digital filter (Hz)
			exciting_step = 0.005;
			break;
		case 12: 
			P.G00.P10_Drive_power_x10_kW = 1100;
			P.G00.P09_IGBT_current_x10= 5750;	//FF300R12KE4 X2 -50A 
			P.G01.P00_Rated_power_x10_kW= 1100;
			P.G01.P01_Rated_voltage= 415.;
			P.G01.P02_Rated_current_x10= 2200;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 25;
			P.G01.P09_Supply_voltage= 380;


			Td = 4.0/1000;// dead-time (ms)
			Ton = 0.2/1000;//1.0/1000; Switching Times of IGBT (ms)
			Toff = 0.5/1000;//2.0/1000;Switching Times of IGBT (ms)
			Vs = 1.75;//1.75; Collector-Emitter Saturation Voltage (v)
			Vd = 1.65;//1.65; FWD Forward Voltage (v)
			Wc=10;// the cutoff frequency of third-order butterworth digital filter (Hz)
			exciting_step = 0.005;

			break;
		case 13: 
			P.G00.P10_Drive_power_x10_kW = 1320; 
			I_scale= 100.0/0.9 *(890.7/176.3); 
			P.G00.P09_IGBT_current_x10= 5750;	//FF300R12KE4 X2 -50A 
			P.G01.P00_Rated_power_x10_kW= 370; //1320
			P.G01.P01_Rated_voltage= 415; 
			P.G01.P02_Rated_current_x10= 700; //2820
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1470; // 1470
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 25;
			P.G01.P09_Supply_voltage= 490;


			Td = 4.0/1000;// dead-time (ms)
			Ton = 0.2/1000;//1.0/1000; Switching Times of IGBT (ms)
			Toff = 0.5/1000;//2.0/1000;Switching Times of IGBT (ms)
			Vs = 1.75;//1.75; Collector-Emitter Saturation Voltage (v)
			Vd = 1.65;//1.65; FWD Forward Voltage (v)
			Wc=10;// the cutoff frequency of third-order butterworth digital filter (Hz)
			exciting_step = 0.005;

			break;
		case 14: 
			I_scale= 100.0/0.9 *(1069.3/176.3);
			P.G00.P10_Drive_power_x10_kW = 1600; 
			P.G00.P09_IGBT_current_x10= 8750;	//FF450R12KE4 X2 -50A 	
			P.G01.P00_Rated_power_x10_kW= 550;
			P.G01.P01_Rated_voltage= 415;
			P.G01.P02_Rated_current_x10= 1060;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1465;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 25;
			P.G01.P09_Supply_voltage= 380; 

			/*	// 400V 315kW motor
			P.G01.P00_Rated_power_x10_kW= 3150;
			P.G01.P01_Rated_voltage= 380;
			P.G01.P02_Rated_current_x10= 5250;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1480;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 25;
			P.G01.P09_Supply_voltage= 690;
			*/


			Td = 4.0/1000;// dead-time (ms)
			Ton = 0.28/1000;//0.16/1000;// Switching Times of IGBT (ms)
			Toff = 0.80/1000;//0.4/1000;// Switching Times of IGBT (ms)
			Vs = 2;// Collector-Emitter Saturation Voltage (v)
			Vd = 1.8;// FWD Forward Voltage (v)
			Wc=10;// the cutoff frequency of third-order butterworth digital filter (Hz)
			exciting_step = 0.005;

			break;
		case 15:
			I_scale= 100.0/0.9 *(1139.5/176.3); 
			P.G00.P10_Drive_power_x10_kW = 2000;
			P.G00.P09_IGBT_current_x10= 8750;	//FF450R12KE4 X2 -50A
			P.G01.P00_Rated_power_x10_kW= 3150;
			P.G01.P01_Rated_voltage= 380.;
			P.G01.P02_Rated_current_x10= 5250;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1480;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 25;
			P.G01.P09_Supply_voltage= 380;

			Td = 4.0/1000;// dead-time (ms)
			Ton = 0.20/1000;//0.16/1000;// Switching Times of IGBT (ms)
			Toff = 0.50/1000;//0.4/1000;// Switching Times of IGBT (ms)
			Vs = 1.75;// Collector-Emitter Saturation Voltage (v)
			Vd = 1.7;// FWD Forward Voltage (v)
			Wc=5;// the cutoff frequency of third-order butterworth digital filter (Hz)
			exciting_step = 0.005;


			break;
		case 16: 
			P.G00.P10_Drive_power_x10_kW = 2500;
			P.G00.P09_IGBT_current_x10= 11830;	//SEMIX604GB12E4s X2 -50A
			P.G01.P00_Rated_power_x10_kW= 2500;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 25;
			P.G01.P09_Supply_voltage= 230;
			break;
		case 17: 
			P.G00.P10_Drive_power_x10_kW = 3150;
			P.G00.P09_IGBT_current_x10= 11830;	//SEMIX604GB12E4s X2 -50A 
			P.G01.P00_Rated_power_x10_kW= 3150;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 25;
			P.G01.P09_Supply_voltage= 230;
			break;
		case 18: 
			I_scale= 100.0/1.05 *(1366.7/176.3);
			P.G00.P10_Drive_power_x10_kW= 4000; 
			P.G00.P09_IGBT_current_x10= 11830;	//SEMIX604GB12E4s X2 -50A
			P.G01.P00_Rated_power_x10_kW= 1100;
			P.G01.P01_Rated_voltage= 1140;
			P.G01.P02_Rated_current_x10= 680;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1489;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 25;
			P.G01.P09_Supply_voltage= 1200; 


			Td = 4.0/1000;// dead-time (ms)
			Ton = 1.3/1000;//0.16/1000;// Switching Times of IGBT (ms)
			Toff =3.0/1000;//0.4/1000;// Switching Times of IGBT (ms)
			Vs = 1.5;// Collector-Emitter Saturation Voltage (v)
			Vd = 1.0;// FWD Forward Voltage (v)
//			cs = 300;//P.G01.P07_PWM_frequency_x10_kHz * 4;//change exciting signal after cs sampling periods
			Wc=15;// the cutoff frequency of third-order butterworth digital filter (Hz)
			exciting_step = 0.005;

			break;
		case 19:
		 	P.G00.P10_Drive_power_x10_kW = 5000; 
		 	P.G00.P09_IGBT_current_x10= 11830;	//SEMIX604GB12E4s X2 -50A	
			P.G01.P00_Rated_power_x10_kW= 5000;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 25;
			P.G01.P09_Supply_voltage= 230;
			break;
		case 20: 
			P.G00.P10_Drive_power_x10_kW = 5600; 
			P.G00.P09_IGBT_current_x10= 350;
			P.G01.P00_Rated_power_x10_kW= 5600;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 25;
			P.G01.P09_Supply_voltage= 230;
			break;
		case 21: 
			I_scale= 100.0/0.9 *(410./176.3);
			P.G00.P10_Drive_power_x10_kW= 6300; 
			P.G00.P09_IGBT_current_x10= 5250;
			P.G01.P00_Rated_power_x10_kW= 6300;
			P.G01.P01_Rated_voltage= 1140.;
			P.G01.P02_Rated_current_x10= 4500;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 25;
			P.G01.P09_Supply_voltage= 1140;
			break;
		case 22: 
			P.G00.P10_Drive_power_x10_kW = 7150; 
			P.G00.P09_IGBT_current_x10= 350;
			P.G01.P00_Rated_power_x10_kW= 7150;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 25;
			P.G01.P09_Supply_voltage= 230;
			break;
		case 23: 
			P.G00.P10_Drive_power_x10_kW = 7800; 
			P.G00.P09_IGBT_current_x10= 350;
			P.G01.P00_Rated_power_x10_kW= 7800;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 25;
			P.G01.P09_Supply_voltage= 230;
			break;
		case 30: 
			P.G00.P10_Drive_power_x10_kW = 22; 
			P.G00.P09_IGBT_current_x10= 350;
			P.G01.P00_Rated_power_x10_kW= 22;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P09_Supply_voltage= 230;
			break;
		case 31: 
			P.G00.P10_Drive_power_x10_kW = 37; 
			P.G00.P09_IGBT_current_x10= 350;
			P.G01.P00_Rated_power_x10_kW= 37;
			P.G01.P01_Rated_voltage= 230.;
			P.G01.P02_Rated_current_x10= 55;
			P.G01.P03_Rated_frequency= 50;
			P.G01.P04_Number_of_poles= 4;
			P.G01.P05_Rated_speed= 1435;
			P.G01.P06_Control_method= 3;
			P.G01.P07_PWM_frequency_x10_kHz= 50;
			P.G01.P09_Supply_voltage= 230;
			break;
	} 
}

void Parameter_Initialization()
{
	Flag.Fault1.all= 0;
	Flag.Fault2.all= 0;	
	Flag.Fault3.all= 0;	
	Fault_count= 0;
	
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
	OP.Run_stop.bit.MOTOR_TUNING= 0;
	OP.Fault_recording.READ= 0;						
	OP.Fault_recording.CLEAR= 0;					
	OP.Run_stop.bit.FAULT_RESET= 0;				
	OP.Fault_recording.TOTAL= 0;				
	OP.Fault_recording.NOW= 0;					

					
	P.G00.P00_Device_number= 11;	 				
	P.G00.P01_Software_version1= 200;			
	P.G00.P02_Software_version2= 'A';			
	P.G00.P03_Software_version3= 0;	
	P.G00.P04_Ias_offset1= -2;					
	P.G00.P05_Ias_offset2= -6909;					
	P.G00.P06_Ibs_offset1= -2;					
	P.G00.P07_Ibs_offset2= -7524;					
	P.G00.P08_Password= 0;
												// default //
	P.G03.P00_Run_stop_source= 0; 				// 0: terminal
	P.G03.P01_Reference_source= 0;				// 0: terminal
//	P.G03.P03_Stop_mode= 0;						// ramp stop
//	P.G03.P04_Stop_hold_time= 0;				// 0s
//	P.G03.P05_Output_off_hold_time= 1; 			// 1s
	P.G03.P09_Accel_switching_ref1_x1000= 500;		// 100%
//	P.G03.P10_Accel_switching_ref2_x1000= 1000;		// 100%
	P.G03.P16_Accel_time1_x10= 20;					// 5s
	P.G03.P17_Accel_time2_x10= 10;					// 5s
	P.G03.P26_Decel_switching_ref1_x1000= 500;			// 100%	
//	P.G03.P27_Decel_switching_ref2_x1000= 1000;		// 150%	
	P.G03.P33_Decel_time1_x10= 20;					// 5s	 
	P.G03.P34_Decel_time2_x10= 10;					// 5s
//	P.G03.P42_Conunter_deceleration_ramp= 0;	// disabled
//	P.G03.P43_Conunter_deceleration_time_x10= 50;	// 5s
//	P.G03.P49_Emergency_stop_mode= 0;			// ramp stop
//	P.G03.P50_Emergency_stop_decel_time_x10= 10;	// 1s
	P.G03.P53_S_curve_enable= 0;
	P.G03.P54_S_curve_weight_x1000= 800;

	P.G05.P00_Current_limit_x1000= 1450;				// 130%
	P.G05.P07_Max_continuous_current_x1000= 950;		// 95%
	P.G05.P08_Overload_current_x1000= 1350;			// 135%
	P.G05.P09_Over_load_time_x10= 600;				// 60s
//	P.G05.P10_Over_load_fault_action= 0;		// normal stop
	P.G05.P11_Over_current_trip_x1000= 2200;			// 220%
	P.G05.P12_Zero_sequence_current_trip_x1000= 150;	// 15%
//	P.G05.P13_Over_voltage_limiting_function= 0;// disabled
	#if (VOLTAGE_CLASS== 0)
//		P.G05.P14_Over_voltage_limit= 375;			
		P.G05.P15_Over_voltage_trip= 385;	
	#elif (VOLTAGE_CLASS== 1)
//		P.G05.P14_Over_voltage_limit= 750;			
		P.G05.P15_Over_voltage_trip= 780;			
	#elif (VOLTAGE_CLASS== 2)
//		P.G05.P14_Over_voltage_limit= 1100;			 
		P.G05.P15_Over_voltage_trip= 1150;		
	#elif (VOLTAGE_CLASS== 3)
//		P.G05.P14_Over_voltage_limit= 1800;			
		P.G05.P15_Over_voltage_trip= 1850;		
	#else
//		P.G05.P14_Over_voltage_limit= 0;		
		P.G05.P15_Over_voltage_trip= 0;	
	#endif				

//	P.G05.P16_Under_voltage_compensation= 0;	// disabled

	#if (VOLTAGE_CLASS== 0)
//		P.G05.P17_UV_compensation_voltage= 280;
		P.G05.P18_Under_voltage_trip= 250; 	
	#elif (VOLTAGE_CLASS== 1)
//		P.G05.P17_UV_compensation_voltage= 400;
		P.G05.P18_Under_voltage_trip= 360; 			
	#elif (VOLTAGE_CLASS== 2)
//		P.G05.P17_UV_compensation_voltage= 750;
		P.G05.P18_Under_voltage_trip= 780; 			
	#elif (VOLTAGE_CLASS== 3)
//		P.G05.P17_UV_compensation_voltage= 1350;
		P.G05.P18_Under_voltage_trip= 1450;			
	#else
//		P.G05.P17_UV_compensation_voltage= 2000;
		P.G05.P18_Under_voltage_trip= 2000;	
	#endif		

//	P.G05.P19_Open_phase_protection= 0;			// disabled
//	P.G05.P20_Supply_frequency= 60;				// 60 Hz
//	P.G05.P21_Built_in_dynamic_brake= 0;		// disabled
//	P.G05.P22_DB_switching_frequency_x10= 50;		// 5.0 ¿”¿«∞™ 
	#if (VOLTAGE_CLASS== 0)
		P.G05.P23_DB_start_voltage= 350;		
		P.G05.P24_DB_full_voltage= 370;	
	#elif (VOLTAGE_CLASS== 1)
		P.G05.P23_DB_start_voltage= 690;		
		P.G05.P24_DB_full_voltage= 710;				
	#elif (VOLTAGE_CLASS== 2)
		P.G05.P23_DB_start_voltage= 970;			
		P.G05.P24_DB_full_voltage= 1050;					
	#elif (VOLTAGE_CLASS== 3)
		P.G05.P23_DB_start_voltage= 1800;		
		P.G05.P24_DB_full_voltage= 1900;				
	#else
		P.G05.P23_DB_start_voltage= 0;		
		P.G05.P24_DB_full_voltage= 10;	
	#endif		

//	P.G05.P25_Over_temperature_trip_action= 0;	// Control off
//	P.G05.P30_Auto_restart_count= 0;			// 0
//	P.G05.P31_Retry_delay_time_x10= 15;			// 1.5s
//	P.G05.P32_OC_auto_reset= 0;					// disabled
//	P.G05.P33_OV_auto_reset= 0;					// disabled
//	P.G05.P34_UV_auto_reset= 0;					// disabled
//	P.G05.P37_Out_of_control_auto_reset= 0;		// disabled
//	P.G05.P38_Out_of_control_time_x10= 50;			// 5.0s
//	P.G05.P39_Out_of_control_current_x1000= 900;		// 90%
	P.G05.P40_Over_temperature_trip_x10= 750;		// 75 deg
//	P.G05.P42_Line_under_voltage_trip_x1000= 900;		// 90%
//	P.G05.P43_Line_under_voltage_auto_reset= 0;	// disabled
//	P.G05.P44_Line_unbalance_auto_reset= 0;		// disabled

	P.G06.P00_Analog_reference_source= 1;		// AI 1
	P.G06.P01_AI1_Function= 1;					// AI
	P.G06.P02_AI1_Type= 0;						// 0~10V
	P.G06.P03_AI1_Filter_time_constant_ms= 25;	// 25ms
	P.G06.P04_AI1_Offset_x10= 0;					// 0
	P.G06.P05_AI1_Min_value_x10= 0;				// 0%
	P.G06.P06_AI1_Max_value_x10= 100;			// 10.0V
	P.G06.P07_AI1_Min_scale_x1000= 0;				// 0%
	P.G06.P08_AI1_Max_scale_x1000= 1000;			// 100%
	P.G06.P09_AI1_Inversion= 0;					// disabled
	P.G06.P10_AI1_Discretness= 0;				// disabled
	P.G06.P11_AI1_Unit_selection= 0;			// V
	P.G06.P12_AI2_Function= 0;					// AI
	P.G06.P13_AI2_Type= 0;						// 0~10V
	P.G06.P14_AI2_Filter_time_constant_ms= 25;	// 25ms
	P.G06.P15_AI2_Offset_x10= 0;					// 0
	P.G06.P16_AI2_Min_value_x10= 0;				// 1%
	P.G06.P17_AI2_Max_value_x10= 100;			// 20.0mA
	P.G06.P18_AI2_Min_scale_x1000= 0;				// 1%
	P.G06.P19_AI2_Max_scale_x1000= 1000;			// 100%
	P.G06.P20_AI2_Inversion= 0;					// disabled
	P.G06.P21_AI2_Discretness= 0;				// disabled
	P.G06.P22_AI2_Unit_selection= 0;			// V


//	P.G07.P00_Control_mode= 0;					// disabled

	P.G08.P00_Run_stop_control= 1;				// FWD/REV
	P.G08.P01_DI03_function= 2;					// none	- > MULTI_STEP_BIT0
	P.G08.P02_DI04_function= 3;					// none	- > MULTI_STEP_BIT1	
	P.G08.P03_DI05_function= 4;					// none	- > MULTI_STEP_BIT2			
	P.G08.P04_DI06_function= 5;					// none	- > MULTI_STEP_BIT3	
	P.G08.P05_DI07_function= 0;					// none			
	P.G08.P06_DI08_function= 0;					// none	
//	P.G08.P07_DI09_function= 0;					// none			
//	P.G08.P08_DI10_function= 0;					// none	
//	P.G08.P09_DI11_function= 0;					// none			
//	P.G08.P10_DI12_function= 0;					// none	
//	P.G08.P11_Run_delay_time= 0;				// 0s


	P.G09.P00_JOG_set_x1000= 200;						// 20%
	P.G09.P01_Step01_set_x10= 150;					// 15%	
	P.G09.P02_Step02_set_x10= 300;					// 30%
	P.G09.P03_Step03_set_x10= 500;					// 50%
	P.G09.P04_Step04_set_x10= 1000;					// 100%
	P.G09.P05_Step05_set_x10= 1000;					// 100%		
	P.G09.P06_Step06_set_x10= 1000;					// 100%
	P.G09.P07_Step07_set_x10= 1000;					// 100%		
	P.G09.P08_Step08_set_x10= 1000;					// 100%
	P.G09.P09_Step09_set_x10= 1000;					// 100%		
	P.G09.P10_Step10_set_x10= 1000;					// 100%
	P.G09.P11_Step11_set_x10= 1000;					// 100%		
	P.G09.P12_Step12_set_x10= 1000;					// 100%
	P.G09.P13_Step13_set_x10= 1000;					// 100%		
	P.G09.P14_Step14_set_x10= 1000;					// 100%
	P.G09.P15_Step15_set_x10= 1000;					// 100%	
	P.G09.P16_Unit_selection= 0;				// percent[%] 

	P.G11.AO1.P00_Output_selection= 1;			// motor speed
	P.G11.AO1.P01_Type= 0;
//	P.G11.AO1.P02_Adjustment_0mA= 0;
//	P.G11.AO1.P03_Adjustment_4mA= 0;
//	P.G11.AO1.P04_Adjustment_20mA= 0;
	P.G11.AO1.P05_Max_output_x1000= 1000;				// 100%
//	P.G11.AO1.P06_Inversion= 0;					// disabled
//	P.G11.AO2.P00_Output_selection= 0;			// output frequency
//	P.G11.AO2.P01_Type= 1;
//	P.G11.AO2.P02_Adjustment_0mA= 0;
//	P.G11.AO2.P03_Adjustment_4mA= 0;
//	P.G11.AO2.P04_Adjustment_20mA= 0;
//	P.G11.AO2.P05_Max_output_x1000= 1000;
//	P.G11.AO2.P06_Inversion= 0;	
		
	P.G12.P00_DO1_function= 0;					// disabled
	P.G12.P00_DO2_function= 0;	
	P.G12.P00_DO3_function= 0;

	P.G13.P00_M1_locked_state_up_speed_set_x1000= 40;// 4%
	P.G13.P01_M1_locked_state_down_speed_set_x1000= 0;// 0%
	P.G13.P02_M1_break_open_current_x1000= 250;// 25%
	P.G13.P03_M1_start_delay_time_x10= 0;// 0s
	P.G13.P04_M1_break_close_speed_set_x1000= 10;// 1%
	P.G13.P05_M1_break_open_torque_build_time_x10= 2;// 0.2s

	P.G14.P09_Dead_time_compansation_x100_us= 22;

	P.G19.P00_Number_of_encoder_pulses= 1024;	//1024
	P.G19.P01_Inversion_of_PG_direction= 0;		//disabled
	P.G19.P03_Minimum_speed= 0;					// 0 rpm
	P.G19.P04_Maximum_speed_x1000= 1000;					// 100%
	P.G19.P05_Over_speed_limit_x1000= 1250;			// 100%

	// leroy somer 1.5kW motor 
/*	P.G21.P00_Stator_resistance_x10_mOhm= 20000; //0.055 ohm
	P.G21.P01_Rotor_resistance_x10_mOhm= 9000;
	P.G21.P02_Stator_inductance_x10_mH= 1600;  // 0.018 H
	P.G21.P03_Rotor_inductance_x10_mH= 1600;
	P.G21.P04_Stator_transient_inductance_x10_mH= 120;
	P.G21.P05_Inertia_x1000= 5;
*/

	// jiamusi 110kW 1140V motor 
/*	P.G21.P00_Stator_resistance_x10_mOhm= 870;
	P.G21.P01_Rotor_resistance_x10_mOhm= 210;
	P.G21.P02_Stator_inductance_x10_mH= 1160;
	P.G21.P03_Rotor_inductance_x10_mH= 1150;
	P.G21.P04_Stator_transient_inductance_x10_mH= 50;
	P.G21.P05_Inertia_x1000= 10;
*/

	// siemens 55kW motor (PS πﬂ¿¸±‚)
/*
	// 400V
	P.G21.P00_Stator_resistance_x10_mOhm= 550; //0.055 ohm
	P.G21.P01_Rotor_resistance_x10_mOhm= 420; 
	P.G21.P02_Stator_inductance_x10_mH= 170;  // 0.018 H
	P.G21.P03_Rotor_inductance_x10_mH= 170;
	P.G21.P04_Stator_transient_inductance_x10_mH= 20;
	P.G21.P05_Inertia_x1000= 30; 
*/

	// 600V
/*	P.G21.P00_Stator_resistance_x10_mOhm= 490; //0.055 ohm
	P.G21.P01_Rotor_resistance_x10_mOhm= 400; 
	P.G21.P02_Stator_inductance_x10_mH= 180;  // 0.018 H
	P.G21.P03_Rotor_inductance_x10_mH= 180;
	P.G21.P04_Stator_transient_inductance_x10_mH= 20;
	P.G21.P05_Inertia_x1000= 50;
*/

	// siemens 18.5kW motor
	P.G21.P00_Stator_resistance_x10_mOhm= 2086; 
	P.G21.P01_Rotor_resistance_x10_mOhm= 1046;
	P.G21.P02_Stator_inductance_x10_mH= 671;  
	P.G21.P03_Rotor_inductance_x10_mH= 671;
	P.G21.P04_Stator_transient_inductance_x10_mH= 43;
	P.G21.P05_Inertia_x1000= 10;

	// PS ∫Œ«œøÎ 11kW Motor
/*
	P.G21.P00_Stator_resistance_x10_mOhm= 4500;
	P.G21.P01_Rotor_resistance_x10_mOhm= 1510;
	P.G21.P02_Stator_inductance_x10_mH= 770;
	P.G21.P03_Rotor_inductance_x10_mH= 760;
	P.G21.P04_Stator_transient_inductance_x10_mH= 70;
	P.G21.P05_Inertia_x1000= 10;
*/
}

void System_Variable_Initialization()
{
	double OL_current;

	
//	Flag.DI.all= 0;
	Flag.DO.all= 0;
	Flag.Fault1.all= 0;
	Flag.Fault2.all= 0;
	Flag.Fault3.all= 0;
	Flag.Fault_Neglect1.all= 0;
	Flag.Fault_Neglect2.all= 0;
	Flag.Fault_Neglect3.all= 0;
	Flag.Monitoring.all= 0;
	Flag.Monitoring.bit.FAULT_RESET_COMPLETE= 1; 
//	OP.Run_stop.all= 0;
	Voltage_class= 0;
	Command= CMD_STOP;
	Init_reference= 0.;
	
	OP.Run_stop.bit.Local_DIR= 0;					
	OP.Run_stop.bit.Fieldbus_RUN= 0;				
	OP.Run_stop.bit.Emergency_STOP= 0;
	OP.Run_stop.bit.MOTOR_TUNING= 0;
	OP.Fault_recording.READ= 0;						
	OP.Fault_recording.CLEAR= 0;					
	OP.Run_stop.bit.FAULT_RESET= 0;				

	

	PWM_frequency_old= P.G01.P07_PWM_frequency_x10_kHz;

#if (DUAL_PWM_INTERRUPT)
	Tsamp= Tsamp_VF= Tsamp_CC= 1./((double)P.G01.P07_PWM_frequency_x10_kHz*1.e+2)/2.;
#else
	Tsamp= Tsamp_VF= Tsamp_CC= 1./((double)P.G01.P07_PWM_frequency_x10_kHz*1.e+2);
#endif 

//	Tsamp_SC= Tsamp_SD= Tsamp_FC= Tsamp*4.;
//	Period_SD= (Uint32)(CPU_CLOCK*Tsamp*4.); 		// EQEP Timer Period 
 
	Tsamp_SC= Tsamp_FC= Tsamp*4.;
	Tsamp_SD= Tsamp*2.;
	Period_SD= (Uint32)(CPU_CLOCK*Tsamp_SD); 		// EQEP Timer Period
	 

	Vdc_max=0;
	I_DT= (double)P.G00.P09_IGBT_current_x10*0.001;
	T_dead_tuning= (double)P.G14.P09_Dead_time_compansation_x100_us * 1.e-2;

//--------Motor Parameters--------//
//	Rs= 0.28175;
//	Rr= 0.09948;
// 	Ls= 0.05135;
//	Lr= 0.05084;
//	Lsigma= 4.09e-3;


	Rs= (double)(P.G21.P00_Stator_resistance_x10_mOhm)*1.e-4;
	Rr= (double)(P.G21.P01_Rotor_resistance_x10_mOhm)*1.e-4;
 	Ls= (double)(P.G21.P02_Stator_inductance_x10_mH)*1.e-4;
	Lr= (double)(P.G21.P03_Rotor_inductance_x10_mH)*1.e-4;
	Lsigma= (double)(P.G21.P04_Stator_transient_inductance_x10_mH)*1.e-4;
	Lm= sqrt((Ls-Lsigma)*Lr);
	Jm= (double)(P.G21.P05_Inertia_x1000)*1.e-3;
	inv_Jm=1./Jm;
	Pole_pair=(double)(P.G01.P04_Number_of_poles)/2.; Bm= 0.; 
	LAMdre_rate = 0.45*(double)(P.G01.P01_Rated_voltage)/220.;
	LAMdre_ref = 0.45*(double)(P.G01.P01_Rated_voltage)/220.; 
//	Is_rate=(20.6*SQRT2), Wrpm_rated=1740., 
	// Wrpm_max=3500.;
	PPR= P.G19.P00_Number_of_encoder_pulses;

 
//------Motor Parameters End------//  
	Is_rated= (double)P.G01.P02_Rated_current_x10*1.e-1;
	Is_max= Is_rated*SQRT2*(double)P.G05.P00_Current_limit_x1000*1.e-3;

	OC_trip_value= Is_rated*SQRT2*(double)P.G05.P11_Over_current_trip_x1000*1.e-3;
	if (OC_trip_value>((double)P.G00.P09_IGBT_current_x10*0.09))	OC_trip_value= (double)P.G00.P09_IGBT_current_x10*0.09;

	ZSC_limit= (double)P.G05.P12_Zero_sequence_current_trip_x1000*1.e-3*Is_rated*SQRT2*(double)P.G00.P10_Drive_power_x10_kW/(double)P.G01.P00_Rated_power_x10_kW;


	OL_current= Is_rated*SQRT2*(double)P.G05.P08_Overload_current_x1000*1.e-3;
	Continuous_OL_current= Is_rated*SQRT2*(double)P.G05.P07_Max_continuous_current_x1000*1.e-3;
	OL_limit= (OL_current*OL_current - Continuous_OL_current*Continuous_OL_current)*(double)P.G05.P09_Over_load_time_x10*1.e-1;


	// Motor Parmeter
 //	Ls = Lr = (Lsigma + sqrt(Lsigma*Lsigma + 4.*Lm*Lm) ) / 2.;


	

	Init_reference=0.;
	Final_reference=0.; 
//	Wrpm_rated=(double)P.G01.P05_Rated_speed*(double)P.G01.P09_Supply_voltage/(double)P.G01.P01_Rated_voltage;
	Wrpm_rated= (double)P.G01.P05_Rated_speed;
	Wrpm_limit= (double)P.G19.P05_Over_speed_limit_x1000*1.e-3*Wrpm_rated;
	Wrpm_max= (double)P.G19.P04_Maximum_speed_x1000*1.e-3*Wrpm_rated;
	Wrpm_min= (double)P.G19.P03_Minimum_speed;
	Accel_time1= (double)P.G03.P16_Accel_time1_x10 * 0.1;
	Accel_time2= (double)P.G03.P17_Accel_time2_x10 * 0.1;	
	Decel_time1= (double)P.G03.P33_Decel_time1_x10 * 0.1;
	Decel_time2= (double)P.G03.P34_Decel_time2_x10 * 0.1;


	// ¡§∞›√‚∑¬(W)/¡§∞›º”µµ(rad/sec) -> ¡§∞›≈‰≈©
	// ¡§∞›≈‰≈© = ¡§∞›√‚∑¬/( (2xPI/60) x rpm)
	//         =  ¡§∞›√‚∑¬ x (30/PI) / rpm 
	// Te_rated_f = (1.0e3*(1.0e-1*P.G01.P00_Rated_power_x10_kW)*30)/(PI * P.G01.P05_Rated_speed) 
	// 				->	1.0e2 x 30 / PI x (P.G01.P00_Rated_power_x10_kW / P.G01.P05_Rated_speed)
	//				->	954.9305 x (P.G01.P00_Rated_power_x10_kW / P.G01.P05_Rated_speed) 
	Te_rate =  (954.9305*(double)P.G01.P00_Rated_power_x10_kW)/Wrpm_rated;
	 
	Theta=0.0;

	// ¿¸∑˘
	Is_mag=0.0;
	Is_mag_rms=0.0;


	// Flux Estimation  
	K_CM1= (1.-(Tsamp_CC/2.)*(Rr/Lr))/(1.+(Tsamp_CC/2.)*(Rr/Lr));
	K_CM2= (Tsamp_CC/2.)*(Rr/Lr)*Lm/(1.+(Tsamp_CC/2.)*(Rr/Lr));

	if (P.G01.P06_Control_method== 3)
	{
		Freq_Fst= 10.;
		Wc_Fst= (2.*PI)*Freq_Fst;
		Kp_Fst= (Lm/Lr)*SQRT2*Wc_Fst;
		Ki_Fst= (Lm/Lr)*Wc_Fst*Wc_Fst;
		
		// Current Controller
		Wc_cc= 2000.*(double)P.G01.P07_PWM_frequency_x10_kHz/50.; //1250

		
		// Flux Controller
		Wc_fc= 80.*(double)P.G01.P07_PWM_frequency_x10_kHz/50.;

		// Speed Controller
		Wc_sc= 400.*(double)P.G01.P07_PWM_frequency_x10_kHz/50.;
	}
	else if (P.G01.P06_Control_method== 4)
	{
		Freq_Fst= 2.;
		Wc_Fst= (2.*PI)*Freq_Fst;
		Kp_Fst= (Lm/Lr)*SQRT2*Wc_Fst;
		Ki_Fst= 0.;
		
		// Current Controller
		Wc_cc= 1000.*(double)P.G01.P07_PWM_frequency_x10_kHz/50.; //1250
		
		// Flux Controller
		Wc_fc= 40.*(double)P.G01.P07_PWM_frequency_x10_kHz/50.;

		// Speed Controller
		Wc_sc= 200.*(double)P.G01.P07_PWM_frequency_x10_kHz/50.;
	}


	Kp_cc= Lsigma*Wc_cc; 
	Ki_cc= Rs*2.*Wc_cc;  
	Ka_cc= 3./Kp_cc;

	Kp_fc= (Lr/Lm)*Wc_fc/Rr;
	Ki_fc= Wc_fc/Lm;
	Ka_fc= 2./Kp_fc;

	Idse_ref_max = Is_max/SQRT2; 		
	

//	Kp_sc= Jm*Wc_sc/Kt;
//	Ki_sc= Kp_sc*(Wc_sc/7.);
//	Ka_sc= 2./Kp_sc;

	// Speed_Detection
	EQep1Regs.QPOSLAT= 0;	//encoder
	Kob_sd=	1000.*200.e-6/(Tsamp_SD);	// Speed Observer 
	L1_sp_est= 3.*Kob_sd;
	L2_sp_est= 3.*Kob_sd*Kob_sd;
	L3_sp_tmp= -(Kob_sd*Kob_sd*Kob_sd);
//	L3_sp_est= -(SPEED_OB_GAIN*SPEED_OB_GAIN*SPEED_OB_GAIN)*Jm;

	//Field Weakening
	K_fw1= (LAMdre_rate*LAMdre_rate)*(Ls*Ls-Lsigma*Lsigma)/(Lm*Lm)
	           +(Lsigma*Lsigma)*(Is_max*Is_max);
	K_fw2= sqrt( (Ls*Ls+Lsigma*Lsigma)/(2.*Ls*Ls*Lsigma*Lsigma) );

	Kt = (3./2.)*Pole_pair*Lm/Lr*LAMdre_ref;

}


#pragma CODE_SECTION(System_Variable_Update, "ramfuncs");
void System_Variable_Update()
{
	double OL_current;

	if( Main_counter & 0x1 )	
	{
		if( Main_counter & 0x2 )
		{
			Is_rated= (double)P.G01.P02_Rated_current_x10*1.e-1*SQRT2;
			Is_max= Is_rated*SQRT2*(double)P.G05.P00_Current_limit_x1000*1.e-3;

			OC_trip_value= Is_rated*SQRT2*(double)P.G05.P11_Over_current_trip_x1000*1.e-3;
			if (OC_trip_value>((double)P.G00.P09_IGBT_current_x10*0.09))	OC_trip_value= (double)P.G00.P09_IGBT_current_x10*0.09;

			ZSC_limit= (double)P.G05.P12_Zero_sequence_current_trip_x1000*1.e-3*Is_rated*SQRT2*(double)P.G00.P10_Drive_power_x10_kW/(double)P.G01.P00_Rated_power_x10_kW;
			
			OL_current= Is_rated*SQRT2*(double)P.G05.P08_Overload_current_x1000*1.e-3;
			Continuous_OL_current= Is_rated*SQRT2*(double)P.G05.P07_Max_continuous_current_x1000*1.e-3;
			OL_limit= (OL_current*OL_current - Continuous_OL_current*Continuous_OL_current)*(double)P.G05.P09_Over_load_time_x10*1.e-1;
		}
		else
		{
			if (P.G01.P06_Control_method== 3)
			{
				Freq_Fst= 10.;
				Wc_Fst= (2.*PI)*Freq_Fst;
				Kp_Fst= (Lm/Lr)*SQRT2*Wc_Fst;
				Ki_Fst= (Lm/Lr)*Wc_Fst*Wc_Fst;
				
				// Current Controller
				Wc_cc= 2000.*(double)P.G01.P07_PWM_frequency_x10_kHz/50.; //1250

				
				// Flux Controller
				Wc_fc= 80.*(double)P.G01.P07_PWM_frequency_x10_kHz/50.;

				// Speed Controller
				Wc_sc= 400.*(double)P.G01.P07_PWM_frequency_x10_kHz/50.;
			}
			else if (P.G01.P06_Control_method== 4)
			{
				Freq_Fst= 2.;
				Wc_Fst= (2.*PI)*Freq_Fst;
				Kp_Fst= (Lm/Lr)*SQRT2*Wc_Fst;
				Ki_Fst= 0.;
				
				// Current Controller
				Wc_cc= 1000.*(double)P.G01.P07_PWM_frequency_x10_kHz/50.; //1250
				
				// Flux Controller
				Wc_fc= 40.*(double)P.G01.P07_PWM_frequency_x10_kHz/50.;

				// Speed Controller
				Wc_sc= 200.*(double)P.G01.P07_PWM_frequency_x10_kHz/50.;
			}


			Kp_cc= Lsigma*Wc_cc; 
			Ki_cc= Rs*2.*Wc_cc;  
			Ka_cc= 3./Kp_cc;

			Kp_fc= (Lr/Lm)*Wc_fc/Rr;
			Ki_fc= Wc_fc/Lm;
			Ka_fc= 2./Kp_fc;

			Idse_ref_max = Is_max/SQRT2; 		


			//Field Weakening
			K_fw1= (LAMdre_rate*LAMdre_rate)*(Ls*Ls-Lsigma*Lsigma)/(Lm*Lm)
			           +(Lsigma*Lsigma)*(Is_max*Is_max);
			K_fw2= sqrt( (Ls*Ls+Lsigma*Lsigma)/(2.*Ls*Ls*Lsigma*Lsigma) );

		//	Kt = (3./2.)*Pole_pair*Lm/Lr*LAMdre_ref;
		}
	}
	else
	{
		if( Main_counter & 0x2 )
		{
			if (Flag.Monitoring.bit.DRIVE_CAL== 1)
			{
				Ias_offset= (double)P.G00.P04_Ias_offset1+0.0001*(double)P.G00.P05_Ias_offset2;
				Ibs_offset= (double)P.G00.P06_Ibs_offset1+0.0001*(double)P.G00.P07_Ibs_offset2;
			}
			Rs= (double)(P.G21.P00_Stator_resistance_x10_mOhm)*1.e-4;
			Rr= (double)(P.G21.P01_Rotor_resistance_x10_mOhm)*1.e-4;
		 	Ls= (double)(P.G21.P02_Stator_inductance_x10_mH)*1.e-4;
			Lr= (double)(P.G21.P03_Rotor_inductance_x10_mH)*1.e-4;
			Lsigma= (double)(P.G21.P04_Stator_transient_inductance_x10_mH)*1.e-4;
			Lm= sqrt((Ls-Lsigma)*Lr);
			Jm= (double)(P.G21.P05_Inertia_x1000)*1.e-3;
			inv_Jm=1./Jm;
			Pole_pair=(double)(P.G01.P04_Number_of_poles)/2.; Bm= 0.; 
			LAMdre_rate = 0.45*(double)(P.G01.P01_Rated_voltage)/220.;
			LAMdre_ref = 0.45*(double)(P.G01.P01_Rated_voltage)/220.;  
			PPR= P.G19.P00_Number_of_encoder_pulses; 


		}
		else
		{
			// Flux Estimation  
			K_CM1= (1.-(Tsamp_CC/2.)*(Rr/Lr))/(1.+(Tsamp_CC/2.)*(Rr/Lr));
			K_CM2= (Tsamp_CC/2.)*(Rr/Lr)*Lm/(1.+(Tsamp_CC/2.)*(Rr/Lr));

			//	Wrpm_rated= (double)P.G01.P05_Rated_speed*(double)P.G01.P09_Supply_voltage/(double)P.G01.P01_Rated_voltage;
			Wrpm_rated= (double)P.G01.P05_Rated_speed;
			Wrpm_limit= (double)P.G19.P05_Over_speed_limit_x1000*1.e-3*Wrpm_rated;
			Wrpm_max= (double)P.G19.P04_Maximum_speed_x1000*1.e-3*Wrpm_rated;
			Wrpm_min= (double)P.G19.P03_Minimum_speed;
			Accel_time1= (double)P.G03.P16_Accel_time1_x10 * 0.1;
			Accel_time2= (double)P.G03.P17_Accel_time2_x10 * 0.1;	
			Decel_time1= (double)P.G03.P33_Decel_time1_x10 * 0.1;
			Decel_time2= (double)P.G03.P34_Decel_time2_x10 * 0.1;

			Te_rate =  (954.9305*(double)P.G01.P00_Rated_power_x10_kW)/Wrpm_rated;


			if (P.G01.P07_PWM_frequency_x10_kHz!= PWM_frequency_old)
			{
				Flag.Monitoring.bit.PWM_FREQUENCY_CHANGE= 1;
				PWM_frequency_old= P.G01.P07_PWM_frequency_x10_kHz;
			}
		}
	}
}

// Main loop ∑Œ ¿Ãµø«œø© Interrupt∏¶ ∏∑∞Ì ¡÷∆ƒºˆ ∫Ø∞Ê¿ª ¡¯«‡«— »ƒ Interrupt ¿ÁΩ√¿€  
void PWM_Frequency_Update()
{
	pwm1.init(&pwm1);

	EPwmPeriodCount =	(Uint16)( F_OSC * DSP28_PLLCR / ((double)P.G01.P07_PWM_frequency_x10_kHz*1.e+2) / 2 /2);
	EPwm1Regs.TBPRD =	EPwmPeriodCount;	// Set timer period
	EPwm2Regs.TBPRD =	EPwmPeriodCount;	// Set timer period
	EPwm3Regs.TBPRD =	EPwmPeriodCount;	// Set timer period
	EPwm4Regs.TBPRD =	EPwmPeriodCount;	// Set timer period
	EPwm5Regs.TBPRD =	EPwmPeriodCount;	// Set timer period



#if (DUAL_PWM_INTERRUPT)
	Tsamp= Tsamp_VF= Tsamp_CC= 1./((double)P.G01.P07_PWM_frequency_x10_kHz*1.e+2)/2.;
#else
	Tsamp= Tsamp_VF= Tsamp_CC= 1./((double)P.G01.P07_PWM_frequency_x10_kHz*1.e+2);
#endif 
 
	Tsamp_SC= Tsamp_FC= Tsamp*4.;
	Tsamp_SD= Tsamp*2.;

	Period_SD= (Uint32)(CPU_CLOCK*Tsamp_SD); 		// EQEP Timer Period 
	EQep1Regs.QUPRD=Period_SD-1;

	// Flux Estimation  
	K_CM1= (1.-(Tsamp_CC/2.)*(Rr/Lr))/(1.+(Tsamp_CC/2.)*(Rr/Lr));
	K_CM2= (Tsamp_CC/2.)*(Rr/Lr)*Lm/(1.+(Tsamp_CC/2.)*(Rr/Lr));

	if (P.G01.P06_Control_method== 3)
	{
		Freq_Fst= 10.;
		Wc_Fst= (2.*PI)*Freq_Fst;
		Kp_Fst= (Lm/Lr)*SQRT2*Wc_Fst;
		Ki_Fst= (Lm/Lr)*Wc_Fst*Wc_Fst;
		
		// Current Controller
		Wc_cc= 2000.*(double)P.G01.P07_PWM_frequency_x10_kHz/50.; //1250

		
		// Flux Controller
		Wc_fc= 80.*(double)P.G01.P07_PWM_frequency_x10_kHz/50.;

		// Speed Controller
		Wc_sc= 400.*(double)P.G01.P07_PWM_frequency_x10_kHz/50.;
	}
	else if (P.G01.P06_Control_method== 4)
	{
		Freq_Fst= 2.;
		Wc_Fst= (2.*PI)*Freq_Fst;
		Kp_Fst= (Lm/Lr)*SQRT2*Wc_Fst;
		Ki_Fst= 0.;
		
		// Current Controller
		Wc_cc= 1000.*(double)P.G01.P07_PWM_frequency_x10_kHz/50.; //1250
		
		// Flux Controller
		Wc_fc= 40.*(double)P.G01.P07_PWM_frequency_x10_kHz/50.;

		// Speed Controller
		Wc_sc= 200.*(double)P.G01.P07_PWM_frequency_x10_kHz/50.;
	}


	Kp_cc= Lsigma*Wc_cc; 
	Ki_cc= Rs*2.*Wc_cc;  
	Ka_cc= 3./Kp_cc;

	Kp_fc= (Lr/Lm)*Wc_fc/Rr;
	Ki_fc= Wc_fc/Lm;
	Ka_fc= 2./Kp_fc;


	// Speed Observer 
	EQep1Regs.QPOSLAT= 0;
	Kob_sd=	1000.*200.e-6/(Tsamp_SD);	
	L1_sp_est= 3.*Kob_sd;
	L2_sp_est= 3.*Kob_sd*Kob_sd;
	L3_sp_tmp= -(Kob_sd*Kob_sd*Kob_sd);


	T_dead_tuning= (double)P.G14.P09_Dead_time_compansation_x100_us * 1.e-2;

}

#pragma CODE_SECTION(DB_Controller, "ramfuncs");
void DB_Controller()
{
	double Temp, Temp1;

	if (GpioDataRegs.GPADAT.bit.GPIO13==0) 	Flag.Fault2.bit.GATE_DRIVER_FAIL= 1;

	// PWM duty √÷º“ 20% ~ √÷¥Î 90%
	// High ¿œ∂ß ¿¸æ– ¬˜¥‹ , Low¿œ∂ß ¿¸æ– ¿Œ∞° 
	Temp1= (double)EPwmPeriodCount;
	Temp= (double)(P.G05.P24_DB_full_voltage - P.G05.P23_DB_start_voltage);
	Temp= Temp1*(0.7*((Vdc-(double)P.G05.P23_DB_start_voltage)/Temp)+0.2);
	if ( Temp<(Temp1*0.2) ){ Temp= 0.;}
	else if ( Temp>(Temp1*0.9) ) Temp= Temp1*0.90; // √÷¥Î 90% Open
	if (Flag.Fault2.bit.GATE_DRIVER_FAIL == 0)
		EPwm4Regs.CMPA.half.CMPA= EPwmPeriodCount - (Uint16)Temp;
	else EPwm4Regs.AQCSFRC.bit.CSFA= 2;
/*	
	if (Flag.Fault.bit.DB == 0)
	{
		if ( Vdc >  (double)P.G05.P24_DB_full_voltage)	EPwm4Regs.AQCSFRC.bit.CSFA= 1;
		else if ( Vdc <  (double)P.G05.P23_DB_start_voltage)	EPwm4Regs.AQCSFRC.bit.CSFA= 2;
	}
*/
}


void Drive_Calibration(void)
{
	static long	Counter= 0, Calibration_Counter= 0;
	static int Calibration_index= 0;
	static double Ias_sum=0., Ibs_sum=0.;
	int COUNT_START, COUNT_END;
	double Temp;

	Temp= (double)P.G01.P07_PWM_frequency_x10_kHz*1.e+2;
	COUNT_START=		((int)(0.01/((1./Temp)/2.)+0.5));		// √ ±‚ ADC »¡§»≠∏¶ ±‚¥Ÿ∏Æ¥¬ Ω√∞£ 
	COUNT_END=			((int)(0.3/((1./Temp)/2.)+0.5)); 


	switch (Calibration_index)
	{
		case 0:	// PWM ON 
			if (OP.Run_stop.bit.DRIVE_CALIBRATION== 1)
			{
				Counter++;
				Flag.Monitoring.bit.DRIVE_CAL= 0;
				if (Counter>(long)(COUNT_END*0.2))
				{
					PWM_ON_OFF(1);	
					Driver_ON= 1;
					Calibration_index++;
					Counter= 0;
				}
				else if (Counter<=1)
				{
					Driver_ON= 0;
					PWM_ON_OFF(0);
				}				
			}
			else Calibration_index= 2;
					
			break;

		case 1:
			EPwm1Regs.CMPA.half.CMPA = EPwmPeriodCount * 0.5;
			EPwm2Regs.CMPA.half.CMPA = EPwmPeriodCount * 0.5;
			EPwm3Regs.CMPA.half.CMPA = EPwmPeriodCount * 0.5;
			
//			Idse_ref= 0.;
//			Iqse_ref= 0.;
			if (Counter<=(long)COUNT_START)
			{
				Counter++;
				Ias_sum=0.;
				Ibs_sum=0.;
			}
			else if (Counter<=(long)COUNT_END)
			{
//				Ias_offset = 0.;
//				Ibs_offset = 0.;
				Ias_sum += Ias;		
				Ibs_sum += Ibs;
				Counter++;
			}
			else
			{
				Calibration_Counter++;
				Ias_offset += Ias_sum/(COUNT_END-COUNT_START);
				Ibs_offset += Ibs_sum/(COUNT_END-COUNT_START);

				
				if (Calibration_Counter>10)	Calibration_index++;
				else
				{
					Ias_sum= 0.;
					Ibs_sum= 0.;
					Calibration_index--;
					Counter= 0;
				}				
			}
			
			break;

		case 2:
			if (OP.Run_stop.bit.DRIVE_CALIBRATION== 1)
			{
				Flag.Monitoring.bit.DRIVE_CAL= 1;
				P.G00.P04_Ias_offset1= (int)(Ias_offset);
				P.G00.P05_Ias_offset2= (int)(((double)(Ias_offset-(int)(Ias_offset))*10000.)+0.5);
				P.G00.P06_Ibs_offset1= (int)(Ibs_offset);
				P.G00.P07_Ibs_offset2= (int)(((double)(Ibs_offset-(int)(Ibs_offset))*10000.)+0.5);
				OP.Run_stop.bit.DRIVE_CALIBRATION= 0;
				Driver_ON= 0;
				PWM_ON_OFF(0);
			}
			Calibration_Counter= 0;

			break;

		default:
			PWM_ON_OFF(0);	
			Driver_ON=0;

			break;
	}

}

void Break_Speed_Up_Controller()
{
	double a;
	static long Break_counter= 0;
	
	if 	(Init_reference> 0.)	Final_reference= (double)P.G13.P00_M1_locked_state_up_speed_set_x1000*1.e-3;
	else if (Init_reference< 0.)Final_reference= (double)P.G13.P01_M1_locked_state_down_speed_set_x1000*(-1.e-3);
	else 						Final_reference= 0.;

	switch(Break_index)
	{
		case 0:
			if (Init_reference!= 0.)
			{
				Break_counter= Main_counter;
				Break_index++;
			}

			break;

		case 1:
			a= (double)P.G13.P02_M1_break_open_current_x1000*Is_rated*SQRT2*1.e-3;
			if (fabs(Is_mag)>= a)
			{
				a= (double)P.G13.P05_M1_break_open_torque_build_time_x10*1.e-1/Tsamp;
				if ( (Main_counter-Break_counter)>= (long)a )	
				{
					Flag.DO.bit.MOTOR_BRAKE= 1;
					Break_index++;
				}
			}
			
			break;		
			
		case 2:
			a= (double)(P.G13.P03_M1_start_delay_time_x10+P.G13.P05_M1_break_open_torque_build_time_x10)*1.e-1/Tsamp;
			if ( (Main_counter-Break_counter)>= a )
				Break_index++;
	
			break;

		case 3:
			// NOP;
			break;

	}

}


void Break_Speed_Down_Controller()
{
	double a;
	
	if (Command==CMD_STOP) 
	{
		a= (double)P.G13.P04_M1_break_close_speed_set_x1000*1.e-3*Wrpm_rated;
		if (fabs(Wrpm_det_flt)<= a)
		{
			Flag.DO.bit.MOTOR_BRAKE= 0;
			Break_index= 0;
		}
	}
}



//---------------------------------
// end of file
//----------------------------------

