#include	<All_Header.h>
#include	<All_Extern_Variables.h>


unsigned long NN =0;//identification times
float I_rated=0.0;//rated current value of motor nameplate.
float exciting_step=0.0;
unsigned int identify_offline=0;
float Hui=0.0;    // cut off frequency of digital filter
int cs_count=0;
float C1=0.0;     // the coefficient of digital filter
float C2=0.0;     // the coefficient of digital filter
float T=0.0;      // Samping period (sec), SWITCH_FREQ=2500.
int cs=0.0;       // change exciting signal after 30 sampling periods 

float Td = 0.0;   // dead-time (ms)
float Ton = 0.0;  // Switching Times of IGBT (ms)
float Toff = 0.0; // witching Times of IGBT (ms)
float Vs = 0.0;   // Collector-Emitter Saturation Voltage (v)
float Vd = 0.0;   // FWD Forward Voltage (v)
float M=0;
float M1=0;

float Wc=0.0;     // the cutoff frequency of third-order butterworth digital filter (Hz)
float a0=1.0;     // the coefficient of third-order butterworth digital filter transfer function
float a1=2.0;     // the coefficient of third-order butterworth digital filter transfer function
float a2=2.0;     // the coefficient of third-order butterworth digital filter transfer function
float A0=0;
float A1=0;
float A2=0;
float predict_weight=0.001;// prediction weight
float B1=0;
float B2=0;
/*
float K_Vs_slope0 = 0.0; 
float K_Vs_slope1 = 0.0;
float K_Vd_slope0 = 0.0;
float K_Vd_slope1 = 0.0;
*/


#pragma CODE_SECTION(Auto_Tuning, "ramfuncs");
void Auto_Tuning()
{
	static long Auto_tuning_counter= 0, T_dead_tuning_counter= 0;
	static double Vdse_ref_sum= 0.; 		
	static double Vdse_ref_avg0= 0., Vdse_ref_avg1= 0.;
//	static double Error_integ= 0.;
	double Error= 0.;
	
	
		
	
	switch (Auto_tuning_index)
	{
		
		case 0 :	// Flag check

			if ( (OP.Run_stop.bit.AUTO_TUNING)&&(Auto_tuning_counter== 0) )
			{
				
//				Idse_ref= Idse_ref_max*0.05;
//				Idse_ref= Idse_ref_max*0.;
//				Iqse_ref= 0.;
				Wrpm_ref= 0.;
				PWM_ON_OFF(1);	
				Driver_ON=1;

			}
			Auto_tuning_counter++;
/*			
			Idse_ref= 0.;
			Iqse_ref= 0.;
			Wrpm_ref= 0.;			
			
			Auto_tuning_counter++;
			if (Auto_tuning_counter>=100000)
			{
				Auto_tuning_index=5;
//				Auto_tuning_counter= 0;				
			}

			
*/			
//				Auto_tuning_index=3;
//				identify_offline= 1;
//				Auto_tuning_counter= 0;

			if (Auto_tuning_counter>=1000) 	
			{
				Auto_tuning_index++;
//				identify_offline= 1;
				Auto_tuning_counter= 0;
				Idse_ref= Idse_ref_max*0.45;
				Iqse_ref= 0.;
				Wrpm_ref= 0.;
			}

		    break;

		case 1 :	// Dead time voltage_compensation & Stator resistance estimation 1
		
			Wrpm_ref= 0.;
			Idse_ref= Idse_ref_max*0.45; 
			Auto_tuning_counter++;

			if (Auto_tuning_counter>=1200)
			{
				Vdse_ref_avg0= Vdse_ref_sum/1000.;
				Vdse_ref_sum= 0.;
				Auto_tuning_counter= 0.;
				Auto_tuning_index++;
			}
			else if (Auto_tuning_counter>=200)
				Vdse_ref_sum+= Vdse_ref;

			break;

		case 2 : 	// Dead time voltage_compensation & Stator resistance estimation 2
			
			Wrpm_ref= 0.;
			Idse_ref= Idse_ref_max*0.9; 
			Auto_tuning_counter++;

			if (Auto_tuning_counter>=1200)
			{
				Vdse_ref_avg1= Vdse_ref_sum/1000.;
				Vdse_ref_sum= 0.;
				Auto_tuning_counter= 0.;
				Error= 2.0- Vdse_ref_avg1/Vdse_ref_avg0;
//				Error_integ+= 2.*Tsamp*3000*Error;
//				T_dead_Tuning= 2.5e-2*Error + Error_integ;
								
				if ( (Error>(-0.01))&&(Error<0.01) )
					T_dead_tuning_counter++;
				else if (Error<= -0.01)
				{
					if (Error<= -0.1)			T_dead_Tuning+=0.1;
					else if (Error<= -0.05) 	T_dead_Tuning+=0.05;
					else if (Error<= -0.025)	T_dead_Tuning+=0.01;
					else 						T_dead_Tuning+=0.005;

					if (T_dead_tuning_counter<0)	T_dead_tuning_counter--;
				}
				else if (Error>= 0.01)
				{
					if (Error>= 0.1)			T_dead_Tuning-=0.1;
					else if (Error>= 0.05)		T_dead_Tuning-=0.05;
					else if (Error>= 0.025)		T_dead_Tuning-=0.01;
					else 						T_dead_Tuning-=0.005;

					if (T_dead_tuning_counter<0)	T_dead_tuning_counter--;
				}

				if (T_dead_tuning_counter>= 5)
				{
					T_dead_tuning_counter= 0;
					Auto_tuning_counter= 0;
					Idse_ref= 0.;
  //					Error_integ= 0.;
					identify_offline= 1;
					Data_Registers[1019]= P.G14.P09_Dead_time_compansation_x100_us= (int)(T_dead_Tuning*1.e+2);
					Auto_tuning_index++;
				}
				else	Auto_tuning_index--; 
			}
			else if (Auto_tuning_counter>=200) // 회전자 시정수로 수정 할것 
				Vdse_ref_sum+= Vdse_ref;
			
			break;

		case 3 :	// Auto tuning by north china


			Auto_Tuning_China();
			if (identify_offline==0)
			{
				Auto_tuning_index++;
//				Rs= Rs0;
				P.G21.P00_Stator_resistance_x1000= (int)(Rs0*1.e+3+0.005);
//				Ls= Lr= Ls0;
            	P.G21.P02_Stator_inductance_x1000= P.G21.P03_Rotor_inductance_x1000= (int)(Ls0*1.e+3+0.005); 
//				Rr= Lr/Tr;
				P.G21.P01_Rotor_resistance_x1000= (int)(Ls0/Tr*1.e+3+0.005);
//				Lm= Lm0;
//				Lsigma= Ls-(Lm*Lm/Lr);
				P.G21.P04_Stator_transient_inductance_x1000= (int)((Ls0-(Lm0*Lm0/Ls0))*1.e+3+0.005);
			}

			break;

		case 4 :	// gain update
			
			// Flux Estimation  
			K_CM1= (1.-(Tsamp_CC/2.)*(Rr/Lr))/(1.+(Tsamp_CC/2.)*(Rr/Lr));
			K_CM2= (Tsamp_CC/2.)*(Rr/Lr)*Lm/(1.+(Tsamp_CC/2.)*(Rr/Lr));

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

			Kt = (3./2.)*Pole_pair*Lm/Lr*LAMdre_ref;

			Auto_tuning_index++;

		    break;

		case 5 :	// complete

			OP.Run_stop.bit.AUTO_TUNING= 0;
			PWM_ON_OFF(0);	
			Driver_ON=0;

		    break;

		default : 
			
			Auto_tuning_index= 0;
			OP.Run_stop.bit.AUTO_TUNING= 0;
			PWM_ON_OFF(0);	
			Driver_ON=0;

			break;
	}

}	

#pragma CODE_SECTION(Auto_Tuning_China, "ramfuncs");
void Auto_Tuning_China()
{
   static unsigned int i=0,j=0;
   static float PWM_dutycycle_a_last = 0;
   static float PWM_dutycycle_b_last = 0;
   static float PWM_dutycycle_c_last = 0;

   static unsigned int exciting_adjust=1;//the enable of exciting amplitude adjustment.
   static unsigned int exciting_adjust_count=0;
   static float I_max=0;
   static float I_max1=0;
   static float I_a_abs=0;

//  variables of thirteen-order exciting signals generator

   static char m_flag=0;
   static int j_count=0;  
   static int signal[14]={0,1,-1,1,-1,1,1,-1,-1,1,1,-1,1,-1};//initial value of exciting signal
   static int  randomtab = 0 ;
 
// signal real value
   static float I_a = 0;                      
   static float I_b = 0; 
   static float I_c = 0; 
   static float I_a_filt=0;//A phase current value after digital filter
   static float I_b_filt=0;//B phase current value after digital filter
   static float I_c_filt=0;//C phase current value after digital filter
   static float U_DC = 0; 

   static float ialfa_real=0.0;
   static float valfa_real=0.0;
   static float Uao=0;      //voltage between A phase and ground.
   static float Ubo=0;      //voltage between B phase and ground.
   static float Uco=0;      //voltage between C phase and ground.
   static float Uan=0;      //A phase voltage
   static float Uan_filt=0; //A phase voltage after digital filter

   static long cout=0;
   static long pp=0;
   static float exciting = 0.01; //the initial value of exciting signal is 0.01.

////////dead-time compensation//////
/*
   static float Vs_a = 0.0;
   static float Vd_a = 0.0;
   static float Vs_b = 0.0;
   static float Vd_b = 0.0;
   static float Vs_c = 0.0;
   static float Vd_c = 0.0;

   static float M1_a=0;
   static float M1_b=0;
   static float M1_c=0;
*/
   static float dead_time_pu=0; //dead time compensation of duty cycle
   static float dead_time_pu_pos=0; //dead time compensation of duty cycle when current>0
   static float dead_time_pu_neg=0; //dead time compensation of duty cycle when current<0

// the variables of third-order butterworth digital filter

   static float Isa_f0=0;     //Isa_f value of last time
   static float Isa_f_FD0=0;  //Isa_f first derivative value of last time 
   static float Isa_f_SD0=0;  //Isa_f second derivative value of last time
   static float Usa_f0=0;     //Usa_f value of last time
   static float Usa_f_FD0=0;  //Usa_f first derivative value of last time 
   static float Usa_f_SD0=0;  //Usa_f second derivative value of last time

   static float Usa_fp=0;
   static float Usa_fp_FD=0;  //Usa_fp first derivative value  
   static float Usa_fp_SD=0;  //Usa_fp second derivative value
   static float Isa_fp=0;
   static float Isa_fp_FD=0;  //Isa_fp first derivative value
   static float Isa_fp_SD=0;  //Isa_fp second derivative value


   static float detaIsa_f = 0;
   static float detaIsa_f_FD = 0; //detaIsa_f first derivative value
   static float detaIsa_f_SD = 0; //detaIsa_f second derivative value
   static float detaUsa_f = 0;
   static float detaUsa_f_FD = 0; //detaUsa_f first derivative value
   static float detaUsa_f_SD = 0; //detaUsa_f second derivative value

   static float detaIsa_fp = 0;
   static float detaIsa_fp_FD = 0;//detaIsa_fp first derivative value
   static float detaIsa_fp_SD = 0;//detaIsa_fp second derivative value
   static float detaUsa_fp = 0;
   static float detaUsa_fp_FD = 0;//detaUsa_fp first derivative value
   static float detaUsa_fp_SD = 0;//detaUsa_fp second derivative value

   static float Usa_f=0;
   static float Usa_f_FD=0;  //Usa_f first derivative value
   static float Usa_f_SD=0;  //Usa_f second derivative value
   static float Isa_f=0;
   static float Isa_f_FD=0;  //Isa_f first derivative value
   static float Isa_f_SD=0;  //Isa_f second derivative value


//the variables of recursive least-squares(RLS)
   static float Kg0[4][1]={0.001,0.001,0.001,0.001};//initial value of K parameter
   static float Kg1[4][1];
   static float p0[4][4]= {{1e+6},{0,1e+6},{0,0,1e+6},{0,0,0,1e+6}};//initial value of covariance matrix
   static float p1[4][4];
   static float p0_p1[4][4];
   static float q[4][1];
   static float q_transpose[1][4];//the transposation of matrix q
   static float qp[1][4]={0,0,0,0};
   static float p0q[4][1]={0,0,0,0};
   static float kk1[4][1];

   static float Y = 0;
   static float qpq=0.0;
   static float innovation=0.0;
   static float forget_factor=1.0;//forgetting factor

//    the estimation value of motor parameter
//   static float Rsg=0;
  // static float Rrg=0;
//	 static float Trg=0;
   static float Sg=0;
   static float Lsg=0;
//   static float Lrg=0;
//   static float Lmg=0;

   static float K1g=0;
   static float K2g=0;
   static float K3g=0;
   static float K4g=0;

// actual value 
   U_DC = Vdc;

//    U_DC = C1 * U_DC + C2 * Vdc;

	I_a_filt = C1 * I_a_filt + C2 * Ias;
	I_b_filt = C1 * I_b_filt + C2 * Ibs;
	I_c_filt = C1 * I_c_filt + C2 * Ics;
	I_a = I_a_filt;
    I_b = I_b_filt;
    I_c = I_c_filt;


/*
    if (I_max1<(float)abs(I_a))
	{
	   I_max1 = (float)abs(I_a);
	}
*/
    if (I_a<0)
	   I_a_abs =-I_a;
	else
	   I_a_abs =I_a;
	if (I_max1<I_a_abs)
       I_max1=I_a_abs;
	

    ialfa_real=I_a;


/*
    //55kW inverter	
    if (abs(I_a)<50.0)
	{  
	    Vs_a = K_Vs_slope0 * abs(I_a) + 0.4;
		Vd_a = K_Vd_slope0 * abs(I_a) + 0.7;
		
	}
	else
	{
	    Vs_a = K_Vs_slope1 * (abs(I_a)-300.0) + 1.75;
		Vd_a = K_Vd_slope1 * (abs(I_a)-300.0) + 1.65;
		
	}
	M1_a = M * (Vd_a - Vs_a);

	if (abs(I_b)<50.0)
	{  
	    Vs_b = K_Vs_slope0 * abs(I_b) + 0.4;
		Vd_b = K_Vd_slope0 * abs(I_b) + 0.7;
	}
	else
	{
	    Vs_b = K_Vs_slope1 * (abs(I_b)-300.0) + 1.75;
		Vd_b = K_Vd_slope1 * (abs(I_b)-300.0) + 1.65;
	}
	M1_b = M * (Vd_b - Vs_b);

	if (abs(I_c)<50.0)
	{  
	    Vs_c = K_Vs_slope0 * abs(I_c) + 0.4;
		Vd_c = K_Vd_slope0 * abs(I_c) + 0.7;
	}
	else
	{
	    Vs_c = K_Vs_slope1 * (abs(I_c)-300.0) + 1.75;
		Vd_c = K_Vd_slope1 * (abs(I_c)-300.0) + 1.65;
	}
	M1_c = M * (Vd_c - Vs_c);

*/
/*
    //5.5kW inverter
	if (abs(I_a)<10.0)
	{  
	    Vs_a = K_Vs_slope0 * abs(I_a) + 0.5;
		Vd_a = K_Vd_slope0 * abs(I_a) + 0.7;
		
	}
	else
	{
	    Vs_a = K_Vs_slope1 * (abs(I_a)-40.0) + 1.8;
		Vd_a = K_Vd_slope1 * (abs(I_a)-40.0) + 1.75;
		
	}
	M1_a = M * (Vd_a - Vs_a);

	if (abs(I_b)<10.0)
	{  
	    Vs_b = K_Vs_slope0 * abs(I_b) + 0.5;
		Vd_b = K_Vd_slope0 * abs(I_b) + 0.7;
	}
	else
	{
	    Vs_b = K_Vs_slope1 * (abs(I_b)-40.0) + 1.8;
		Vd_b = K_Vd_slope1 * (abs(I_b)-40.0) + 1.75;
	}
	M1_b = M * (Vd_b - Vs_b);

	if (abs(I_c)<10.0)
	{  
	    Vs_c = K_Vs_slope0 * abs(I_c) + 0.5;
		Vd_c = K_Vd_slope0 * abs(I_c) + 0.7;
	}
	else
	{
	    Vs_c = K_Vs_slope1 * (abs(I_c)-40.0) + 1.8;
		Vd_c = K_Vd_slope1 * (abs(I_c)-40.0) + 1.75;
	}
	M1_c = M * (Vd_c - Vs_c);




	if (I_a>=0.0)
    {
       dead_time_pu_pos = svgen_dq1.Ta1 * Vs_a + svgen_dq1.Ta2 * Vd_a;//svgen_dq1.Ta1(duty cycle)=1.0-svgen_dq1.Ta;
       dead_time_pu = M + dead_time_pu_pos/U_DC + M1_a/U_DC;
       svgen_dq1.Ta1 = svgen_dq1.Ta1 - dead_time_pu;
    }
    else
    {
       dead_time_pu_neg = svgen_dq1.Ta1 * Vd_a + svgen_dq1.Ta2 * Vs_a;
       dead_time_pu = M + dead_time_pu_neg/U_DC + M1_a/U_DC;
       svgen_dq1.Ta1 = svgen_dq1.Ta1 + dead_time_pu;
    }
    if (I_b>=0.0)
    {
       dead_time_pu_pos = svgen_dq1.Tb1 * Vs_b + svgen_dq1.Tb2 * Vd_b;
       dead_time_pu = M + dead_time_pu_pos/U_DC + M1_b/U_DC;
       svgen_dq1.Tb1 = svgen_dq1.Tb1 - dead_time_pu;
    }
    else
    {
       dead_time_pu_neg = svgen_dq1.Tb1 * Vd_b + svgen_dq1.Tb2 * Vs_b;
       dead_time_pu = M + dead_time_pu_neg/U_DC + M1_b/U_DC;
       svgen_dq1.Tb1 = svgen_dq1.Tb1 + dead_time_pu;
    }
    if (I_c>=0.0)
    {
       dead_time_pu_pos = svgen_dq1.Tc1 * Vs_c + svgen_dq1.Tc2 * Vd_c;
       dead_time_pu = M + dead_time_pu_pos/U_DC + M1_c/U_DC;
       svgen_dq1.Tc1 = svgen_dq1.Tc1 - dead_time_pu; 
    }
    else
    {
       dead_time_pu_neg = svgen_dq1.Tc1 * Vd_c + svgen_dq1.Tc2 * Vs_c;
       dead_time_pu = M + dead_time_pu_neg/U_DC + M1_c/U_DC;
       svgen_dq1.Tc1 = svgen_dq1.Tc1 + dead_time_pu;
    }
*/

    if (I_a>=0.0)
    {
       dead_time_pu_pos = svgen_dq1.Ta1 * Vs + svgen_dq1.Ta2 * Vd;//svgen_dq1.Ta1(duty cycle)=1.0-svgen_dq1.Ta;
       dead_time_pu = M + dead_time_pu_pos/U_DC + M1/U_DC;
       svgen_dq1.Ta1 = svgen_dq1.Ta1 - dead_time_pu;
    }
    else
    {
       dead_time_pu_neg = svgen_dq1.Ta1 * Vd + svgen_dq1.Ta2 * Vs;
       dead_time_pu = M + dead_time_pu_neg/U_DC + M1/U_DC;
       svgen_dq1.Ta1 = svgen_dq1.Ta1 + dead_time_pu;
    }
    if (I_b>=0.0)
    {
       dead_time_pu_pos = svgen_dq1.Tb1 * Vs + svgen_dq1.Tb2 * Vd;
       dead_time_pu = M + dead_time_pu_pos/U_DC + M1/U_DC;
       svgen_dq1.Tb1 = svgen_dq1.Tb1 - dead_time_pu;
    }
    else
    {
       dead_time_pu_neg = svgen_dq1.Tb1 * Vd + svgen_dq1.Tb2 * Vs;
       dead_time_pu = M + dead_time_pu_neg/U_DC + M1/U_DC;
       svgen_dq1.Tb1 = svgen_dq1.Tb1 + dead_time_pu;
    }
    if (I_c>=0.0)
    {
       dead_time_pu_pos = svgen_dq1.Tc1 * Vs + svgen_dq1.Tc2 * Vd;
       dead_time_pu = M + dead_time_pu_pos/U_DC + M1/U_DC;
       svgen_dq1.Tc1 = svgen_dq1.Tc1 - dead_time_pu; 
    }
    else
    {
       dead_time_pu_neg = svgen_dq1.Tc1 * Vd + svgen_dq1.Tc2 * Vs;
       dead_time_pu = M + dead_time_pu_neg/U_DC + M1/U_DC;
       svgen_dq1.Tc1 = svgen_dq1.Tc1 + dead_time_pu;
    }



    Uao = U_DC * PWM_dutycycle_a_last;
    Ubo = U_DC * PWM_dutycycle_b_last;
    Uco = U_DC * PWM_dutycycle_c_last;

    Uan = 0.6666666667 * Uao;
    Uan -= 0.3333333333 * Ubo;
    Uan -= 0.3333333333 * Uco;

    Uan_filt = C1 * Uan_filt + C2 * Uan;
    valfa_real = Uan_filt;//Uan;

	PWM_dutycycle_a_last= svgen_dq1.Ta1;
    PWM_dutycycle_b_last= svgen_dq1.Tb1;
    PWM_dutycycle_c_last= svgen_dq1.Tc1;


   if(identify_offline==1)
    {     
      if(exciting_adjust==1)
	  {
	    exciting_adjust_count++;
        if (cs_count==cs)//change exciting signal after 30 sampling periods
        {
           cs_count=0;
           randomtab=signal[13];

	       if (signal[13]==signal[4])	
	           m_flag=-1;
	       else						
	           m_flag=1;
	       if (m_flag==signal[3])	
	       	   m_flag=-1;
	       else				           
	           m_flag=1;
	       if (m_flag==signal[1])      
	           m_flag=-1;
	       else				            
	           m_flag=1;
	 
	       for (j_count=12;j_count>=1;j_count--)
	           signal[j_count+1]=signal[j_count];
	   
	       signal[1]=m_flag;
	    }
        cs_count++;
		svgen_dq1.Ualfa = exciting * randomtab;//change amplitude of exciting signal.
        svgen_dq1.Ubeta =0.0;
		
		if (exciting_adjust_count<=1000)
		{   
		    if (I_a<0)
			    I_a_abs =-I_a;
			else
			    I_a_abs =I_a;
		    if (I_max<I_a_abs)
           	    I_max=I_a_abs;	
		}
		else
		{
		    exciting_adjust_count=0;
			if (I_max<=I_rated)
			{
			   exciting+=exciting_step;
			}
			else
			{

			   exciting_adjust=0;
			   cs_count=cs;
			   randomtab=0;


			   signal[1]=1;
               signal[2]=-1;
               signal[3]=1;
               signal[4]=-1;
               signal[5]=1;
               signal[6]=1;
               signal[7]=-1;
               signal[8]=-1;
               signal[9]=1;
               signal[10]=1;
               signal[11]=-1;
               signal[12]=1;
               signal[13]=-1;

			}

		}
		
	  }

      else if (cout>100)//delay 100 sampling periods
      { 

	     if (pp<=NN)//identify 15000 times
         {	
            pp++; 

			//generate exciting signal,1 or -1.// 
	        if (cs_count==cs)//change exciting signal after 30 sampling periods
            {
               cs_count=0;
               randomtab=signal[13];

	           if (signal[13]==signal[4])
	           	  m_flag=-1;
	           else							
	              m_flag=1;
	           if (m_flag==signal[3])		
	              m_flag= -1;
	           else				           
	              m_flag=1;
	           if (m_flag==signal[1])      
	              m_flag=-1;
	           else				            
	              m_flag= 1;
	 
	           for (j_count=12;j_count>=1;j_count--)
	               signal[j_count+1]=signal[j_count];
	   
	           signal[1]=m_flag;
	        }
            cs_count++;
			svgen_dq1.Ualfa = exciting * randomtab;//change amplitude of exciting signal.
            svgen_dq1.Ubeta =0.0;


          //third-order butterworth digital filter//
			detaIsa_f = T * Isa_f_FD0;
            detaIsa_f_FD = T * Isa_f_SD0;
            detaIsa_f_SD = - A0 * Isa_f0 - A1 * Isa_f_FD0 - A2 * Isa_f_SD0 + A0 * ialfa_real;

			detaUsa_f = T * Usa_f_FD0;
            detaUsa_f_FD = T * Usa_f_SD0;
		    detaUsa_f_SD = - A0 * Usa_f0 - A1 * Usa_f_FD0 - A2 * Usa_f_SD0 + A0 * valfa_real;
			
			Isa_fp = Isa_f0 + detaIsa_f;                //Predictors;
            Isa_fp_FD = Isa_f_FD0 + detaIsa_f_FD;
            Isa_fp_SD = Isa_f_SD0 + detaIsa_f_SD;

			Usa_fp = Usa_f0 + detaUsa_f;                //Predictors;
            Usa_fp_FD = Usa_f_FD0 + detaUsa_f_FD;
            Usa_fp_SD = Usa_f_SD0 + detaUsa_f_SD;

			detaIsa_fp = T * Isa_fp_FD;        
            detaIsa_fp_FD = T * Isa_fp_SD;
            detaIsa_fp_SD = - A0 * Isa_fp - A1 * Isa_fp_FD - A2 * Isa_fp_SD + A0 * ialfa_real;

            detaUsa_fp = T * Usa_fp_FD;
            detaUsa_fp_FD = T * Usa_fp_SD;
            detaUsa_fp_SD = - A0 * Usa_fp - A1 * Usa_fp_FD - A2 * Usa_fp_SD + A0 * valfa_real;

			Isa_f = Isa_f0 + B1 * detaIsa_fp + B2 * detaIsa_f;                     //correctors
            Isa_f_FD = Isa_f_FD0 + B1 * detaIsa_fp_FD + B2 * detaIsa_f_FD;
            Isa_f_SD = Isa_f_SD0 + B1 * detaIsa_fp_SD + B2 * detaIsa_f_SD;
      
			Usa_f = Usa_f0 + B1 * detaUsa_fp + B2 * detaUsa_f;                     //correctors
            Usa_f_FD = Usa_f_FD0 + B1 * detaUsa_fp_FD + B2 * detaUsa_f_FD;
            Usa_f_SD = Usa_f_SD0 + B1 * detaUsa_fp_SD + B2 * detaUsa_f_SD;

			Isa_f0 = Isa_f;
            Isa_f_FD0 = Isa_f_FD;
            Isa_f_SD0 = Isa_f_SD;

			Usa_f0 = Usa_f;
            Usa_f_FD0 = Usa_f_FD;
            Usa_f_SD0 = Usa_f_SD;

            // recursive least-squares(RLS)//
			q[0][0] = Isa_f;
            q[1][0] = Usa_f;
            q[2][0] = Usa_f_FD;
            q[3][0] = Isa_f_FD;		   
		    Y = Isa_f_SD;

            q_transpose[0][0] = q[0][0];
            q_transpose[0][1] = q[1][0];
            q_transpose[0][2] = q[2][0];
	        q_transpose[0][3] = q[3][0];
            matrix_multiplic(4,1,4,*q_transpose,*p0, *qp);
	        qpq = forget_factor + (qp[0][0] * q[0][0] + qp[0][1] * q[1][0] + qp[0][2] * q[2][0] + qp[0][3] * q[3][0]);
            matrix_multiplic(4,4,1,*p0,*q, *p0q);
	        for(i=0;i<4;i++)
	        {
	           kk1[i][0]=p0q[i][0]/qpq;
	        }
	        matrix_multiplic(1,4,4,*kk1,*q_transpose, *p0_p1);
	        matrix_multiplic(4,4,4,*p0_p1,*p0, *p1);
	        matrix_subtract(4,4,*p0,*p1,*p1);
			  			  		     
	        for(i=0;i<4;i++)
	        {
	           for(j=0;j<4;j++)
		       {
		          p1[i][j]=p1[i][j]/forget_factor;
			      p0[i][j]=p1[i][j];         //p0=p1.
		       }
	        }
            innovation=Y-(q_transpose[0][0]* Kg0[0][0]+q_transpose[0][1]* Kg0[1][0]+q_transpose[0][2]* Kg0[2][0]+q_transpose[0][3]* Kg0[3][0]);
            for(i=0;i<4;i++)
	        {
	           kk1[i][0]=kk1[i][0]* innovation;
		       Kg1[i][0]=Kg0[i][0]+kk1[i][0];
               Kg0[i][0]=Kg1[i][0];          //Kg0=Kg1.
	        }

			K1g = Kg1[0][0];
            K2g = Kg1[1][0];
            K3g = Kg1[2][0];
            K4g = Kg1[3][0];

            //the estimation value of motor parameter//
//			Rsg = -K1g/K2g;
            Lsg = (K1g * K3g - K2g * K4g)/(K2g * K2g);
//			Trg = K3g/K2g;//rotor time constant
			Sg = 1.0/(K3g * Lsg);//leakage coefficient
//            Lrg=Lsg;
//			Rrg = Lrg/Trg;
//			Rrg = Rrg;
//			Lmg= sqrt(1-Sg) * Lsg;

			            //the estimation value of motor parameter//

			Rs0 = -K1g/K2g;
            Ls0 = (K1g * K3g - K2g * K4g)/(K2g * K2g);
			Tr = K3g/K2g;//rotor time constant
			Lm0= sqrt(1-Sg) * Ls0;


		 }
         else
         {
            svgen_dq1.Ualfa = 0.0;
            svgen_dq1.Ubeta = 0.0;
            
			//initialize variable for next identification.
			identify_offline = 0;
//			exciting_adjust = 1;
//            exciting_adjust_count = 0;
//			I_max = 0;
//			exciting = 0.01;
	        pp = 0;
	        cout = 0;
	        Isa_f0=0;
            Isa_f_FD0=0;
            Isa_f_SD0=0;
            Usa_f0=0;
            Usa_f_FD0=0;
            Usa_f_SD0=0;
		    Kg0[0][0]=0.001;
		    Kg0[1][0]=0.001;
		    Kg0[2][0]=0.001;
		    Kg0[3][0]=0.001;
		    for (i=0;i<4;i++)
		        for (j=0;j<4;j++)
			    {   
			        if (i==j)
			           p0[i][j]=1e+6;
				    else
				       p0[i][j]=0;
			    }
			cs_count = cs;
			randomtab = 0;

			signal[1]=1;
            signal[2]=-1;
            signal[3]=1;
            signal[4]=-1;
            signal[5]=1;
            signal[6]=1;
            signal[7]=-1;
            signal[8]=-1;
            signal[9]=1;
            signal[10]=1;
            signal[11]=-1;
            signal[12]=1;
            signal[13]=-1;

         }  
            
      }
      else
	  {   
	     cout++;
		 svgen_dq1.Ualfa = 0.0;
         svgen_dq1.Ubeta = 0.0;
	  }
    }
	else
	{
	  svgen_dq1.Ualfa = 0.0;
      svgen_dq1.Ubeta = 0.0;
	}
 

    svgen_dq1.calc(&svgen_dq1);

	pwm1.Mfunc_c1 = svgen_dq1.Ta; 
    pwm1.Mfunc_c2 = svgen_dq1.Tb; 
    pwm1.Mfunc_c3 = svgen_dq1.Tc; 
	pwm1.update(&pwm1);

}


#pragma CODE_SECTION(matrix_multiplic, "ramfuncs");
void matrix_multiplic(int rowBorcolumnA,int rowA,int columnB,float *pA,float *pB, float *pC)//pC=pA*pB
{
	int i,j,k;
	float m;
	for (i=0;i<rowA;i++)
		  for (j=0;j<columnB;j++)
			{   
			    m=0.0;
				for (k=0;k<rowBorcolumnA;k++)
					m+=*(pA+i*rowBorcolumnA+k) * *(pB+k*columnB+j);
				*(pC+i*columnB+j)=m;
			}

}

#pragma CODE_SECTION(matrix_subtract, "ramfuncs");
void matrix_subtract(int row,int column,float *pA,float *pB,float *pC)//pC=pA-pB
{
    int i,j;
	for (i=0;i<row;i++)
	{
	   for(j=0;j<column;j++)
	   {
	      *(pC+i*column+j)=*(pA+i*column+j) - *(pB+i*column+j);
	   }
	}
}

/*
void SVPWM()
{
	long i;
	double a, b;
	double Vas_ref= 0., Vbs_ref= 0., Vcs_ref= 0.;
	double Van_ref= 0., Vbn_ref= 0., Vcn_ref= 0.;
	double V_max= 0., V_min= 0., V_offset= 0.;
	double V_dt= 0., K_dt= 0., inv_Vdc= 0.;
	
	#if (DUAL_PWM_INTERRUPT)
		static int Flag_UPDN_PWM= 0.;
	#endif   

	// Synchronous -> Stationary 
	Vdss_ref_old = Vdss_ref;		Vqss_ref_old = Vqss_ref;
	Vdss_ref = Vas_ref = Cos_Theta_vtg*Vdse_ref - Sin_Theta_vtg*Vqse_ref;
	Vqss_ref = Sin_Theta_vtg*Vdse_ref + Cos_Theta_vtg*Vqse_ref;
	
	// Stationary -> 3 Phases 
	Vbs_ref = -0.5*(Vdss_ref - SQRT3*Vqss_ref);
	Vcs_ref = -(Vas_ref+Vbs_ref);
	
	// Max & Min 
	if (Vas_ref > Vbs_ref)	{ 
		V_max = Vas_ref; 			V_min = Vbs_ref;
		if (Vcs_ref > V_max) 		V_max = Vcs_ref;
		else if (Vcs_ref < V_min)	V_min = Vcs_ref;
	}
	else { 
		V_max = Vbs_ref; 			V_min = Vas_ref;
		if (Vcs_ref > V_max) 		V_max = Vcs_ref;
		else if (Vcs_ref < V_min)	V_min = Vcs_ref;
 	}

	// Calculate and Add voltage offset, V_offset = voltage offset 
	V_offset = -(V_max+V_min)/2.;
	Van_ref = Vas_ref + V_offset;
	Vbn_ref = Vbs_ref + V_offset;
	Vcn_ref = Vcs_ref + V_offset;

	// Overmodulation 
//	Flag_OM=0; 
	if( (V_max-V_min) > Vdc) {
		a = Vdc/2.;		b=-a;

		Van_ref = BOUND(Van_ref,a,b);
		Vbn_ref = BOUND(Vbn_ref,a,b);
		Vcn_ref = BOUND(Vcn_ref,a,b);

		Vas_ref = (2.*Van_ref-Vbn_ref-Vcn_ref)/3.;
		Vbs_ref = (2.*Vbn_ref-Vcn_ref-Van_ref)/3.;
		Vcs_ref = -(Vas_ref + Vbs_ref);
		  
		Vdss_ref = Vas_ref;
		Vqss_ref = (Vbs_ref-Vcs_ref)/SQRT3;
		Vdse_ref =  Cos_Theta_vtg*Vdss_ref + Sin_Theta_vtg*Vqss_ref;
		Vqse_ref = -Sin_Theta_vtg*Vdss_ref + Cos_Theta_vtg*Vqss_ref;
//		Flag_OM=1; 
	}

	// Dead_Time Compensation 
	V_dt = Vdc*( (T_DEAD_US-T_dead_Tuning)/(Tsamp_CC*1.e+6) );	
	K_dt = V_dt/(I_DT);

	#if (DUAL_PWM_INTERRUPT)
		Flag_UPDN_PWM=1-Flag_UPDN_PWM;
		if(Flag_UPDN_PWM) {			// next down mode 
			a = K_dt*(Ias_ref+I_DT);	a = BOUND(a,V_dt,0.);		Van_ref += a;
			a = K_dt*(Ibs_ref+I_DT);	a = BOUND(a,V_dt,0.);		Vbn_ref += a;
			a = K_dt*(Ics_ref+I_DT);	a = BOUND(a,V_dt,0.);		Vcn_ref += a;
		}
		else {						// next up mode 
			a = K_dt*(Ias_ref-I_DT);	a = BOUND(a,0,(-V_dt));		Van_ref += a;
			a = K_dt*(Ibs_ref-I_DT);	a = BOUND(a,0,(-V_dt));		Vbn_ref += a;
			a = K_dt*(Ics_ref-I_DT);	a = BOUND(a,0,(-V_dt));		Vcn_ref += a;
		}
		
	#else
		a= K_dt*Ias_ref;	a= BOUND(a, V_dt, (-V_dt));	Van_ref+= a;
		a= K_dt*Ibs_ref;	a= BOUND(a, V_dt, (-V_dt));	Vbn_ref+= a; 
		a= K_dt*Ics_ref;	a= BOUND(a, V_dt, (-V_dt));	Vcn_ref+= a; 
	#endif


	// Write PWM counter to ASIC 
	// Reference voltage --> Counter Value  	// max : dead_time 고려 
	inv_Vdc = 1./Vdc;
	i = (int)( (Van_ref*inv_Vdc+0.5)*EPwmPeriodCount+0.5);	DutyCount[u]=BOUND(i,EPwmPeriodCount,0);
	i = (int)( (Vbn_ref*inv_Vdc+0.5)*EPwmPeriodCount+0.5);	DutyCount[v]=BOUND(i,EPwmPeriodCount,0);
	i = (int)( (Vcn_ref*inv_Vdc+0.5)*EPwmPeriodCount+0.5);	DutyCount[w]=BOUND(i,EPwmPeriodCount,0);

	EPwm1Regs.CMPA.half.CMPA = EPwmPeriodCount - DutyCount[u];
	EPwm2Regs.CMPA.half.CMPA = EPwmPeriodCount - DutyCount[v];
	EPwm3Regs.CMPA.half.CMPA = EPwmPeriodCount - DutyCount[w];

}
*/



// end of file

