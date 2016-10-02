//#define _DEFAULT_VAR_
//#define _VARIABLES_
#include "main_def.h"
Uint16 addr_cnt= 0;

void EEPROM_DATA_INIT()
{
	int	itmp_cnt;
	int Comm_GROUP1;

	for(itmp_cnt=0 ; itmp_cnt<3000 ; itmp_cnt++)
	{
		addr2par_num(itmp_cnt);
		if(Comm_GROUP != Comm_GROUP1) Comm_INDEX = 0;

		Word_Write_data(itmp_cnt, Var_initialize(Comm_GROUP,Comm_INDEX));
		Comm_INDEX ++;
		Comm_GROUP1 = Comm_GROUP;
	}
}

void eeprom2sys()
{
Uint16 Init_RAM = 0;
int itmp_cnt1;
int Comm_GROUP2;
Comm_INDEX = 0;

	for(itmp_cnt1=0 ; itmp_cnt1<3000 ; itmp_cnt1++)
	{
		addr2par_num(itmp_cnt1);
		if(Comm_GROUP != Comm_GROUP2) Comm_INDEX = 0;
		Word_Read_data(itmp_cnt1, &Init_RAM);
		Comm_array[itmp_cnt1] = Init_RAM ;
		get_cmd(Comm_GROUP,Comm_INDEX);
		Comm_INDEX ++;
		Comm_GROUP2 = Comm_GROUP;
	}
} 

void Key_pad2eeprom(unsigned char GROUP_par, unsigned char INDEX_par, unsigned char Key_DATA)
{
	par_num2addr(GROUP_par, INDEX_par);
	Word_Write_data(addr_cnt, Key_DATA);
	get_cmd(GROUP_par, INDEX_par);
//	eeprom2sys(GROUP_par, INDEX_par, 0);
}
int Var_initialize(unsigned char GROUP,unsigned char INDEX)
{
	int i= 0;
	Group_index = (unsigned int)(GROUP * 100 + INDEX);
	switch(Group_index)
	{
		case   0 :	i = EEPROM_WRITED;
					break ;
		case 100 :	i=  		220;	//P_rate = 100 * Comm_array[40];
		 		 	break ;
		case 101 : 	i=  		2200;	//	V_rate = Comm_array[41] / 10;
		 		 	break ;
		case 102 :	i=  		320;	//I_rate = Comm_array[42] / 10;
		 		 	break ;
		case 103 : 	i=  		1000;	//F_rate = Comm_array[43];
		 		 	break ;
		case 104 : 	i=  		2;		//Pn = Comm_array[44];
		 		 	break ;
		case 105 : 	i=  		6000;	//Wrpm_rate = Comm_array[45];
		 		 	break ;
		case 106 : 	i=  		1;		//Cntrl_Method = Comm_array[46];
		 		 	break ;
		case 107 : 	i=  		20000;	//Fpwm = Comm_array[47];
		 		 	break ;
		case 109 : 	i=  		2200;	//Vsup = Comm_array[49] / 10;
		 		 	break ;
		case 300 : 	i=  		1;		//Run_Stop_meth = Comm_array[80];
		 		 	break ;
		case 301 : 	i=  		0;		//RampFunc_Input_Src = Comm_array[81];
		 		 	break ; 
		case 308 : 	i=  		0;		//Accel_Time = Comm_array[88];
		 		 	break ; 
		case 325 : 	i=  		0;		//Decel_Time = Comm_array[105];
		 		 	break ; 
		case 500 : 	i=  		1450;	//Curr_Lim = Comm_array[300] / 10;
		 		 	break ; 
		case 507 : 	i=  		950;	//Max_Con_cur = Comm_array[307] / 10;
		 		 	break ; 
		case 508 : 	i=  		1350;	//Over_Load = Comm_array[308] / 10;
		 		 	break ; 
		case 511 : 	i=  		2200;	//OC_Trip = Comm_array[311] / 10;
		 		 	break ; 
		case 512 : 	i= 			150;	//I_zero_Lim = Comm_array[312] / 10;
		 		 	break ; 	
		case 513 : 	i= 			0;		//OV_Ltd_Fn = Comm_array[313];
		 		 	break ; 
		case 514 : 	i=  		3600;	//OV_Limit = Comm_array[314] / 10;
		 		 	break ; 
		case 515 : 	i=  		3600;	//OV_Trip = Comm_array[315] / 10;
		 		 	break ; 
		case 518 : 	i=  		2600;	//UV_Trip = Comm_array[318] / 10;
		 		 	break ; 
		case 519 : 	i=  		1;		//OP_Ph_Trip = Comm_array[319];
		 		 	break ;
		case 538 : 	i=  		5;		//Out_of_Cnl_t = Comm_array[338];
		 		 	break ; 
		case 1701: 	i=  		50;		//Min_Speed = Comm_array[951];
		 		 	break ; 
		case 1702: 	i=  		1000;	//Max_Speed = Comm_array[952] / 10;
		 		 	break ; 
		case 1703: 	i=  		1250;	//Over_Spd_Lim = Comm_array[953] / 10;
		 		 	break ; 
		case 1711: 	i=  		1000;	//Cur_Ctrl_Kp = Comm_array[961] / 10;
		 		 	break ;  
		case 1712: 	i=  		1000;	//Cur_Ctrl_Ki = Comm_array[962] / 10;
		 		 	break ;
		case 1718: 	i=  		1000;	//Spd_Ctrl_Kp = Comm_array[968] / 10;
		 		 	break ; 
		case 1719: 	i=  		1000;	//Spd_Ctrl_Ki = Comm_array[969] / 10;
		 		 	break ;
		case 1750: 	i=  		100;	//Spd_Ctrl_B = Comm_array[1000];
		 		 	break ;
		case 1751: 	i=  		500;	//Cur_Ctrl_B = Comm_array[1001];
		 		 	break ;
		case 2109: 	i=  		2400;	//Rstator = Comm_array[1359]/1000;
		 		 	break ;
		case 2110: 	i=  		13500;	//Lds = Comm_array[1360]/1000000;
		 		 	break ;
		case 2111: 	i=  		13500;	//Lqs = Comm_array[1361]/1000000;
		 		 	break ;
		case 2112: 	i=  		139;	//K_back_EMF = Comm_array[1362]/10000;
		 		 	break ;
		case 2113: 	i=  		1;	//Inertia = Comm_array[1363];
		 		 	break ;
		case 2114: 	i=  		1;	//B_fric = Comm_array[1364];
		 		 	break ;
		case 2115: 	i=  		7000;	//Base_spd = Comm_array[1365];
		 		 	break ; 
		case 5000: 	i=  		0;		//Local Remote
					break ;
		case 5001: 	i=  		0;
					break ;
		case 5002: 	i=  		7000;
					break ;
		case 5003: 	i=  		0;
					break ;
		case 5004: 	i=  		0;
					break ;
		case 5005: 	i=  		0;
					break ;
		default	:	i= 			0;
					break;
	}
	return  i;
}


void addr2par_num(int add_cnt)
{
	if 		(add_cnt  < 40						   )		Comm_GROUP = 0; 
	else if	((add_cnt >=40)  	&&  (add_cnt < 60))			Comm_GROUP = 1;
	else if	((add_cnt >=60)  	&&  (add_cnt < 80))			Comm_GROUP = 2;
	else if	((add_cnt >=80)  	&&  (add_cnt < 190))		Comm_GROUP = 3;
	else if	((add_cnt >=190) 	&&  (add_cnt < 300))		Comm_GROUP = 4;
	else if	((add_cnt >=300) 	&&  (add_cnt < 400))		Comm_GROUP = 5;
	else if	((add_cnt >=400) 	&&  (add_cnt < 450))		Comm_GROUP = 6;
	else if	((add_cnt >=550) 	&&  (add_cnt < 610))		Comm_GROUP = 7;
	else if	((add_cnt >=610) 	&&  (add_cnt < 660))		Comm_GROUP = 8;
	else if	((add_cnt >=660) 	&&  (add_cnt < 700))		Comm_GROUP = 9;
	else if	((add_cnt >=700) 	&&  (add_cnt < 740))		Comm_GROUP = 10;
	else if	((add_cnt >=740) 	&&  (add_cnt < 760))		Comm_GROUP = 11;
	else if	((add_cnt >=760) 	&&  (add_cnt < 780))		Comm_GROUP = 12;
	else if	((add_cnt >=780) 	&&  (add_cnt < 810))		Comm_GROUP = 13;
	else if	((add_cnt >=810) 	&&  (add_cnt < 830))		Comm_GROUP = 14;
	else if	((add_cnt >=830) 	&&  (add_cnt < 890))		Comm_GROUP = 15;
	else if	((add_cnt >=890) 	&&  (add_cnt < 950))		Comm_GROUP = 16;
	else if	((add_cnt >=950)	&&  (add_cnt < 1060))		Comm_GROUP = 17;
	else if	((add_cnt >=1060) 	&&  (add_cnt < 1170))		Comm_GROUP = 18;
	else if	((add_cnt >=1170)	&&  (add_cnt < 1260))		Comm_GROUP = 19;
	else if	((add_cnt >=1260)	&&  (add_cnt < 1350))		Comm_GROUP = 20;
	else if	((add_cnt >=1350)	&&  (add_cnt < 1370))		Comm_GROUP = 21;
	else if	((add_cnt >=1370)	&&  (add_cnt < 1390))		Comm_GROUP = 22;
	else if	((add_cnt >=1390)	&&  (add_cnt < 1420))		Comm_GROUP = 23;
	else if	((add_cnt >=1420)	&&  (add_cnt < 1460))		Comm_GROUP = 24;
	else if	((add_cnt >=1460)	&& 	(add_cnt < 1490))		Comm_GROUP = 25;
	else if	((add_cnt >=1490)	&&	(add_cnt < 1550))		Comm_GROUP = 26;
	else if	((add_cnt >=1550)	&&	(add_cnt < 1640))		Comm_GROUP = 27;
	else if	((add_cnt >=1640)	&&	(add_cnt < 1670))		Comm_GROUP = 28;
	else if	((add_cnt >=1670)	&&	(add_cnt < 1740))		Comm_GROUP = 29;
	else if	((add_cnt >=1740)	&&	(add_cnt < 1950))		Comm_GROUP = 30;
	else if	((add_cnt >=1950)	&&	(add_cnt < 2000))		Comm_GROUP = 31;
	else if	((add_cnt >=2000)	&&	(add_cnt < 2100))		Comm_GROUP = 32;
	else if	((add_cnt >=2100)	&&	(add_cnt < 2110))		Comm_GROUP = 50;
	else if	((add_cnt >=2110)	&&	(add_cnt < 2140))		Comm_GROUP = 51;
	else if	((add_cnt >=2140)	&&	(add_cnt < 2150))		Comm_GROUP = 52;
	else if	((add_cnt >=2150)	&&	(add_cnt < 2170))		Comm_GROUP = 53;
	else if	((add_cnt >=2170)	&&	(add_cnt < 2180))		Comm_GROUP = 60;
	else if	((add_cnt >=2180)	&&	(add_cnt < 2280))		Comm_GROUP = 70;
	else if	((add_cnt >=2280)	&&	(add_cnt < 2290))		Comm_GROUP = 80;
	else if	((add_cnt >=2290)	&&	(add_cnt < 2291))		Comm_GROUP = 90;
	else if	((add_cnt >=2291)	&&	(add_cnt < 2296))		Comm_GROUP = 91;
	else if	((add_cnt >=2296)	&&	(add_cnt < 3000))		Comm_GROUP = 92;
}

void par_num2addr(unsigned char GROUP_par1, unsigned char INDEX_par1)
{

	switch(GROUP_par1)
	{
	// Mode 2
		case 0 : addr_cnt = INDEX_par1;
				 break ;
		case 1 : addr_cnt = 40+INDEX_par1;
				 break ;
		case 2 : addr_cnt = 60+INDEX_par1;
				 break ;
		case 3 : addr_cnt = 80+INDEX_par1;
				 break ;
		case 4 : addr_cnt = 190+INDEX_par1;
				 break ;
		case 5 : addr_cnt = 300+INDEX_par1;
				 break ;
		case 6 : addr_cnt = 400+INDEX_par1;
				 break ;
		case 7 : addr_cnt = 550+INDEX_par1;
				 break ;
		case 8 : addr_cnt = 610+INDEX_par1;
				 break ;
		case 9 : addr_cnt = 660+INDEX_par1;
				 break ;
		case 10: addr_cnt = 700+INDEX_par1;
				 break ;
		case 11: addr_cnt = 740+INDEX_par1;
				 break ;
		case 12: addr_cnt = 760+INDEX_par1;
				 break ;
		case 13: addr_cnt = 780+INDEX_par1;
				 break ;
		case 14: addr_cnt = 810+INDEX_par1;
				 break ;
		case 15: addr_cnt = 830+INDEX_par1;
				 break ;
		case 16: addr_cnt = 890+INDEX_par1;
				 break ;
		case 17: addr_cnt = 950+INDEX_par1;
				 break ;
		case 18: addr_cnt = 1060+INDEX_par1;
				 break ;
		case 19: addr_cnt = 1170+INDEX_par1;
				 break ;
		case 20: addr_cnt = 1260+INDEX_par1;
				 break ;
		case 21: addr_cnt = 1350+INDEX_par1;
				 break ;
		case 22: addr_cnt = 1370+INDEX_par1;
				 break ;
		case 23: addr_cnt = 1390+INDEX_par1;
				 break ;
		case 24: addr_cnt = 1420+INDEX_par1;
				 break ;
		case 25: addr_cnt = 1460+INDEX_par1;
				 break ;
		case 26: addr_cnt = 1490+INDEX_par1;
				 break ;
		case 27: addr_cnt = 1550+INDEX_par1;
				 break ;
		case 28: addr_cnt = 1640+INDEX_par1;
				 break ;
		case 29: addr_cnt = 1670+INDEX_par1;
				 break ;
		case 30: addr_cnt = 1740+INDEX_par1;
				 break ;
		case 31: addr_cnt = 1950+INDEX_par1;
				 break ;
		case 32: addr_cnt = 2000+INDEX_par1;
				 break ;
		// Mode 0
		case 50: addr_cnt = 2100+INDEX_par1;
				 break ;
		// Mode 3
		case 60: addr_cnt = 2170+INDEX_par1;
				 break ;
		// Mode 5
		case 80: addr_cnt = 2280+INDEX_par1;
				 break ;
		// Mode 6
		case 90: addr_cnt = 2290+INDEX_par1;
				 break ;
		case 91: addr_cnt = 2291+INDEX_par1;
				 break ;
		case 92: addr_cnt = 2296+INDEX_par1;
				 break ;	
	}
}

