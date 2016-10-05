#include	<All_Header.h>
#include	<All_Extern_Variables.h>

// #pragma CODE_SECTION(Auto_Tuning, "ramfuncs");
void Auto_Tuning()
{
	static long Auto_tuning_counter= 0;
	static double Vdse_ref_sum= 0.; 		
	static double Vdse_ref_avg0= 0., Vdse_ref_avg1= 0.;
	static double Error_integ= 0.0;
	double Error= 0.;	
	
	switch (Auto_tuning_index)
	{	
		case 0 :	// Flag check

			if ( (OP.Run_stop.bit.AUTO_TUNING)&&(Auto_tuning_counter== 0) )
			{
				Auto_tuning_counter++;
				Idse_ref= Idse_ref_max*0.05;
				Iqse_ref= 0.;
				Wrpm_ref= 0.;
				PWM_ON_OFF(1);	
				Driver_ON=1;
			}
		
			if (Flux_build_up) 	
			{
				Auto_tuning_index++;
				Auto_tuning_counter= 0;
			}

		    break;

		case 1 :	// Dead time voltage_compensation & Stator resistance estimation 1
		
			Wrpm_ref= 0.;
			Idse_ref= Idse_ref_max*0.40; 
			Auto_tuning_counter++;

			if (Auto_tuning_counter>=2500)
			{
				Vdse_ref_avg0= Vdse_ref_sum*0.001;
				Vdse_ref_sum= 0.;
				Auto_tuning_counter= 0.;
				Auto_tuning_index++;
			}
			else if (Auto_tuning_counter>500)
				Vdse_ref_sum+= Vdse_ref;

			break;

		case 2 : 	// Dead time voltage_compensation & Stator resistance estimation 2
			
			Wrpm_ref= 0.;
			Idse_ref= Idse_ref_max*0.8; 
			Auto_tuning_counter++;

			if (Auto_tuning_counter>=2500)
			{
				Vdse_ref_avg1= Vdse_ref_sum*0.001;
				Vdse_ref_sum= 0.;
				Auto_tuning_counter= 0.;
				Error= 2.0- Vdse_ref_avg1/Vdse_ref_avg0;
				Error_integ+= 1.e-6*Tsamp*5000*Error;
				T_dead_Tuning= 2.5e-8*Error + Error_integ;
			
				if ( (Error>(-0.01))&&(Error<0.01) )
				{
					Auto_tuning_counter= 0;
					Idse_ref= 0.;
					Error_integ= 0.;
					identify_offline= 1;
					Auto_tuning_index= 5;
				}
				else	Auto_tuning_index--;
				
			}
			else if (Auto_tuning_counter>500) // È¸ÀüÀÚ ½ÃÁ¤¼ö·Î ¼öÁ¤ ÇÒ°Í 
				Vdse_ref_sum+= Vdse_ref;
			
			break;

		case 3 :	// Auto tuning by north china
			Auto_Tuning_China();
			if (identify_offline==0)
			{
				Auto_tuning_index++;
				Rs= Rs0;
            	Ls= Lr= Ls0; 
				Rr= Lr/Tr;
				Lm= Lm0;
				Lsigma= Ls-(Lm*Lm/Lr);
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
// #pragma CODE_SECTION(Auto_Tuning_China, "ramfuncs");
#define NN 16000 //identification times

void Auto_Tuning_China()
{

 
	// signal real value
	static float I_a= 0., I_b= 0., I_c= 0.; 
	static float U_DC = 0.; 


	static long cout=0;
	static long  pp=0;
	
	/**********increased variable**********/
	static float jili = 0.01;//the initial value of exciting signal is 0.01.
	static unsigned int jili_adjust=1;//the enable of jili amplitude adjustment.
	static unsigned int jili_adjust_count=0;
	static unsigned int I_count_max=0;
	static float I_rate=5.5;//rated current value of motor nameplate.
	/**************************************/ 



	static float dead_time_pu= 0., dead_time_pu_pos= 0., dead_time_pu_neg= 0.;



	static float Isa_f0= 0., Isa_f_1dao0= 0., Isa_f_2dao0= 0.;
	static float Isa_fp= 0., Isa_fp_1dao= 0., Isa_fp_2dao= 0.;
	static float Usa_f0= 0., Usa_f_1dao0= 0., Usa_f_2dao0= 0.;
	static float Usa_fp= 0., Usa_fp_1dao= 0., Usa_fp_2dao= 0.;

	static float detaIsa_f= 0., detaIsa_f_1dao= 0., detaIsa_f_2dao= 0.;
	static float detaUsa_f= 0., detaUsa_f_1dao= 0., detaUsa_f_2dao= 0.;
	static float detaIsa_fp= 0., detaIsa_fp_1dao= 0., detaIsa_fp_2dao= 0.;
	static float detaUsa_fp= 0., detaUsa_fp_1dao= 0., detaUsa_fp_2dao= 0.;
	static float Usa_f= 0., Usa_f_1dao= 0., Usa_f_2dao=0.;
	static float Isa_f= 0., Isa_f_1dao= 0., Isa_f_2dao=0.;


	//the variables of recursive least-squares(RLS)
	static float Kg0[4][1]={0.001,0.001,0.001,0.001};//initial value of K parameter
	static float Kg1[4][1];
	static float p0[4][4]= {{1e+6},{0,1e+6},{0,0,1e+6},{0,0,0,1e+6}};//initial value of covariance matrix
	static float p1[4][4];
	static float p0_p1[4][4];
	static float q[4][1];
	static float qzhuanzhi[1][4];
	static float qp[1][4]={0,0,0,0};
	static float p0q[4][1]={0,0,0,0};
	static float kk1[4][1];

	static float Y = 0;
	static float qpq=0.0;
	static float innovation=0.0;//ÐÂÏ¢
	static float ww=1.0;//forgetting factor

	//    the estimation value of motor parameter
//	static float Rsg=0., Rrg=0.;
	static float Sg=0;
//	static float Lsg=0., Lrg=0., Lmg=0;
	static float K1g=0., K2g=0., K3g=0., K4g=0;

	// e variables of thirteen-order exciting signals generator
	static char m_flag=0;
	static int j_count=0;  
	static int signal[14]={0,1,-1,1,-1,1,1,-1,-1,1,1,-1,1,-1};//initial value of exciting signal
	static int  randomtab = 0 ;

	static unsigned int i=0,j=0;

	static float ialfa_real=0., valfa_real=0.;
	static float Uan=0., Uan_filt=0;
	
 



// actual value
    U_DC = Vdc;
	T= Tsamp;
	I_rate=P.G01.P02_Rated_current_x10*0.1;
//    U_DC = C1 * U_DC + C2 * Vdc;              //BASE_VDC=0.392; real dc bus voltage
/*
	I_a_filt = C1 * I_a_filt + C2 * Ias;
	I_b_filt = C1 * I_b_filt + C2 * Ibs; 
	I_c_filt = C1 * I_c_filt + C2 * Ics;
	I_a = I_a_filt;
    I_b = I_b_filt;
    I_c = I_c_filt;
*/	  
    I_a = Ias;
    I_b = Ibs;
    I_c = Ics;
       
    ialfa_real=I_a;


//	svgen_dq1.Ta1= 1.0 - svgen_dq1.Ta;
//	svgen_dq1.Tb1= 1.0 - svgen_dq1.Tb;
//	svgen_dq1.Tc1= 1.0 - svgen_dq1.Tc;

	
// dead time compensation of voltage estimation 

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
 

    svgen_dq1.Ta1 = U_DC * svgen_dq1.Ta1;
    svgen_dq1.Tb1 = U_DC * svgen_dq1.Tb1;
    svgen_dq1.Tc1 = U_DC * svgen_dq1.Tc1;

    Uan = 0.6666666667 * svgen_dq1.Ta1;
    Uan -= 0.3333333333 * svgen_dq1.Tb1;
    Uan -= 0.3333333333 * svgen_dq1.Tc1;

    Uan_filt = C1 * Uan_filt + C2 * Uan;//filter equal to hardware filter of current sampling
    valfa_real = Uan_filt;

    if(identify_offline==1)
    {

/**********increased program**********/
      if(jili_adjust==1)
	  {
	    jili_adjust_count++;
        if (cishu==cs)//change exciting signal after 30 sampling periods
        {
           cishu=0;
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
        cishu++;
		svgen_dq1.Ualfa = jili * randomtab;//change amplitude of exciting signal.
        svgen_dq1.Ubeta =0.0;
		
		if (jili_adjust_count<=1000)
		{   
		    if (I_count_max<abs(I_a))
           	    I_count_max=I_a;	
		}
		else
		{
		    jili_adjust_count=0;
			if (I_count_max<=I_rate)
			{
			   jili+=0.005;
			}
			else
			{
			   jili_adjust=0;
			   cishu=cs;
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
/**************************************/

      else if (cout>100)//
      { 

	     if (pp<=NN)//identify 15000 times
         {	
            pp++; 

			//generate exciting signal,1 or -1.// 
	        if (cishu==cs)//change exciting signal after 30 sampling periods
            {
               cishu=0;
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
            cishu++;
			svgen_dq1.Ualfa = jili * randomtab;//change amplitude of exciting signal.
            svgen_dq1.Ubeta =0.0;


            //third-order butterworth digital filter//
			detaIsa_f = T * Isa_f_1dao0;
            detaIsa_f_1dao = T * Isa_f_2dao0;
            detaIsa_f_2dao = - A0 * Isa_f0 - A1 * Isa_f_1dao0 - A2 * Isa_f_2dao0 + A0 * ialfa_real;

			detaUsa_f = T * Usa_f_1dao0;
            detaUsa_f_1dao = T * Usa_f_2dao0;
		    detaUsa_f_2dao = - A0 * Usa_f0 - A1 * Usa_f_1dao0 - A2 * Usa_f_2dao0 + A0 * valfa_real;
			
			Isa_fp = Isa_f0 + detaIsa_f;                //Predictors;
            Isa_fp_1dao = Isa_f_1dao0 + detaIsa_f_1dao;
            Isa_fp_2dao = Isa_f_2dao0 + detaIsa_f_2dao;

			Usa_fp = Usa_f0 + detaUsa_f;                //Predictors;
            Usa_fp_1dao = Usa_f_1dao0 + detaUsa_f_1dao;
            Usa_fp_2dao = Usa_f_2dao0 + detaUsa_f_2dao;

			detaIsa_fp = T * Isa_fp_1dao;        
            detaIsa_fp_1dao = T * Isa_fp_2dao;
            detaIsa_fp_2dao = - A0 * Isa_fp - A1 * Isa_fp_1dao - A2 * Isa_fp_2dao + A0 * ialfa_real;

            detaUsa_fp = T * Usa_fp_1dao;
            detaUsa_fp_1dao = T * Usa_fp_2dao;
            detaUsa_fp_2dao = - A0 * Usa_fp - A1 * Usa_fp_1dao - A2 * Usa_fp_2dao + A0 * valfa_real;

			Isa_f = Isa_f0 + B1 * detaIsa_fp + B2 * detaIsa_f;                     //correctors
            Isa_f_1dao = Isa_f_1dao0 + B1 * detaIsa_fp_1dao + B2 * detaIsa_f_1dao;
            Isa_f_2dao = Isa_f_2dao0 + B1 * detaIsa_fp_2dao + B2 * detaIsa_f_2dao;
      
			Usa_f = Usa_f0 + B1 * detaUsa_fp + B2 * detaUsa_f;                     //correctors
            Usa_f_1dao = Usa_f_1dao0 + B1 * detaUsa_fp_1dao + B2 * detaUsa_f_1dao;
            Usa_f_2dao = Usa_f_2dao0 + B1 * detaUsa_fp_2dao + B2 * detaUsa_f_2dao;

			Isa_f0 = Isa_f;
            Isa_f_1dao0 = Isa_f_1dao;
            Isa_f_2dao0 = Isa_f_2dao;

			Usa_f0 = Usa_f;
            Usa_f_1dao0 = Usa_f_1dao;
            Usa_f_2dao0 = Usa_f_2dao;

            // recursive least-squares(RLS)//
			q[0][0] = Isa_f;
            q[1][0] = Usa_f;
            q[2][0] = Usa_f_1dao;
            q[3][0] = Isa_f_1dao;		   
		    Y = Isa_f_2dao;

            qzhuanzhi[0][0] = q[0][0];
            qzhuanzhi[0][1] = q[1][0];
            qzhuanzhi[0][2] = q[2][0];
	        qzhuanzhi[0][3] = q[3][0];
            matrix_multiplic(4,1,4,*qzhuanzhi,*p0, *qp);
	        qpq = ww + (qp[0][0] * q[0][0] + qp[0][1] * q[1][0] + qp[0][2] * q[2][0] + qp[0][3] * q[3][0]);
            matrix_multiplic(4,4,1,*p0,*q, *p0q);
	        for(i=0;i<4;i++)
	        {
	           kk1[i][0]=p0q[i][0]/qpq;
	        }
	        matrix_multiplic(1,4,4,*kk1,*qzhuanzhi, *p0_p1);
	        matrix_multiplic(4,4,4,*p0_p1,*p0, *p1);
	        matrix_subtract(4,4,*p0,*p1,*p1);
			  			  		     
	        for(i=0;i<4;i++)
	        {
	           for(j=0;j<4;j++)
		       {
		          p1[i][j]=p1[i][j]/ww;
			      p0[i][j]=p1[i][j];         //p0=p1.
		       }
	        }
            innovation=Y-(qzhuanzhi[0][0]* Kg0[0][0]+qzhuanzhi[0][1]* Kg0[1][0]+qzhuanzhi[0][2]* Kg0[2][0]+qzhuanzhi[0][3]* Kg0[3][0]);
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
			Rs0 = -K1g/K2g;
            Ls0 = (K1g * K3g - K2g * K4g)/(K2g * K2g);
			Tr = K3g/K2g;//rotor time constant
			Sg = 1.0/(K3g * Ls);//leakage coefficient
			Lm0= sqrt(1-Sg) * Ls;

		 }
         else
         {
            svgen_dq1.Ualfa = 0.0;
            svgen_dq1.Ubeta =0.0;
            
			//initialize variable for next identification.
			identify_offline = 0;
			pp = 0;
	        cout = 0;
	        Isa_f0=0;
            Isa_f_1dao0=0;
            Isa_f_2dao0=0;
            Usa_f0=0;
            Usa_f_1dao0=0;
            Usa_f_2dao0=0;
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
			cishu = cs;
			randomtab = 0;

/**********increased program**********/
			jili_adjust=1;//increased sentence.
		    I_count_max=0;//increased sentence.
		    jili=0.01;//increased sentence.
/*************************************/

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
         svgen_dq1.Ubeta =0.0;
	  }

    }
	else
	{
	  svgen_dq1.Ualfa = 0.0;
      svgen_dq1.Ubeta =0.0;
	}
//	Ias_ref = Ias;	Ibs_ref = Ibs;	Ics_ref = Ics;
//	Cos_Theta_vtg=1.0;
//    Sin_Theta_vtg=0.0;
//    SVPWM();

//	svgen_dq1.Ta2=((float)EPwm1Regs.CMPA.half.CMPA)/EPwmPeriodCount;
//	svgen_dq1.Tb2=((float)EPwm2Regs.CMPA.half.CMPA)/EPwmPeriodCount;
//	svgen_dq1.Tc2=((float)EPwm3Regs.CMPA.half.CMPA)/EPwmPeriodCount;
//	svgen_dq1.Ta1=((float)DutyCount[u])/EPwmPeriodCount;
//	svgen_dq1.Tb1=((float)DutyCount[v])/EPwmPeriodCount;
//	svgen_dq1.Tc1=((float)DutyCount[w])/EPwmPeriodCount;

 
	

    svgen_dq1.calc(&svgen_dq1);//

//**************************************************************************
//·¢PWM²¨

// 	pwm1.MfuncC1 = svgen_dq1.Ta; 
//	pwm1.MfuncC2 = svgen_dq1.Tb; 
//	pwm1.MfuncC3 = svgen_dq1.Tc;
//	pwm1.update(&pwm1); 

	pwm1.Mfunc_c1 = svgen_dq1.Ta; // MfuncC1 is in Q15
    pwm1.Mfunc_c2 = svgen_dq1.Tb; // MfuncC2 is in Q15  
    pwm1.Mfunc_c3 = svgen_dq1.Tc; // MfuncC3 is in Q15
	pwm1.update(&pwm1);

}

// #pragma CODE_SECTION(matrix_multiplic, "ramfuncs");
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
// #pragma CODE_SECTION(matrix_subtract, "ramfuncs");
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

// end of file

