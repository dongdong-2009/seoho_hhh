/*=====================================================================================
 File name:        SVGEN_DQ.C  (IQ version)                  
                    
 Originator:	Digital Control Systems Group
			Texas Instruments

 Description:  Space-vector PWM generation based on d-q components                    

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 04-15-2005	Version 3.20
-------------------------------------------------------------------------------------*/

#include "Core_IQmathLib.h"         // Include header for IQmath library 
// Don't forget to set a proper GLOBAL_Q in "IQmathLib.h" file
#include "CH_svgen_dq.h"
#pragma CODE_SECTION(svgendq_calc, "ramfuncs");
void svgendq_calc(SVGENDQ *v)
{	  
	float Va,Vb,Vc,t1,t2;       //_iq表示数据类型为32位数（全局Q格式）
	int sector = 0;  /* sector is treated as Q0 - independently with global Q */

//step1																
/* Clarke逆变换        IQmpy表示浮点的乘法    Va、Vb、Vc为参考电压*/
    Va = v->Ubeta;
    Vb = -0.5 * v->Ubeta + 0.8660254 * v->Ualfa;  /* 0.8660254 = sqrt(3)/2 */
    Vc = -0.5 * v->Ubeta - 0.8660254 * v->Ualfa;  /* 0.8660254 = sqrt(3)/2 */

//step2  确定扇区
/* 扇区的确定方法 */       //sector=4*c+2*b+a
    if (Va>0)         //a=1;else a=0
       sector = 1; 
    if (Vb>0)         //b=1;else b=0
       sector = sector + 2;
    if (Vc>0)          //c=1;else c=0
       sector = sector + 4;

//step3  各扇区中t1、t2、Ta、Tb、Tc       
/* X,Y,Z (Va,Vb,Vc) calculations  */
    Va = v->Ubeta;                                                  /* X = Va */
    Vb = 0.5 * v->Ubeta + 0.8660254 * v->Ualfa;   /* Y = Vb */
    Vc = 0.5 * v->Ubeta - 0.8660254 * v->Ualfa;   /* Z = Vc */
   
    if (sector==0)  /*  第0扇区: this is special case for (Ualfa,Ubeta) = (0,0)  */
    {
       v->Ta = 0.5;
       v->Tb = 0.5;
       v->Tc = 0.5;
    }

    if (sector==1)  /*第1扇区: t1=Z and t2=Y (abc ---> Tb,Ta,Tc) */
    {
       t1 = Vc;                                      //Vc=Z,Vb=Y
       t2 = Vb;
       v->Tb = 0.5 * (1-t1-t2);      /* Tb=taon = (1-t1-t2)/2 */
       v->Ta = v->Tb+t1;                             /* Ta=tbon = taon+t1 */
       v->Tc = v->Ta+t2;                             /* Tc=tcon = tbon+t2 */
    }
    else if (sector==2)  /* 第2扇区: t1=Y and t2=-X (abc ---> Ta,Tc,Tb) */
    {
       t1 = Vb;                                      //Vb=Y.Vc=X
       t2 = -Va;
       v->Ta = 0.5 * (1-t1-t2);      /* Ta=taon = (1-t1-t2)/2 */
       v->Tc = v->Ta+t1;                             /* Tc=tbon = taon+t1 */
       v->Tb = v->Tc+t2;                             /* Tb=tcon = tbon+t2 */
    }      
    else if (sector==3)  /*第3扇区: t1=-Z and t2=Y (abc ---> Ta,Tb,Tc) */
    {
       t1 = -Vc;                                     //Vc=Z,Va=X
       t2 = Va;
       v->Ta = 0.5 * (1-t1-t2);      /* Ta=taon = (1-t1-t2)/2 */
       v->Tb = v->Ta+t1;                             /* Tb=tbon = taon+t1 */
       v->Tc = v->Tb+t2;                             /* Tc=tcon = tbon+t2 */
    }   
    else if (sector==4)  /*第4扇区: t1=-X and t2=Z (abc ---> Tc,Tb,Ta) */
    {
       t1 = -Va;                                     //Va=X,Vc=Z
       t2 = Vc;
       v->Tc = 0.5 * (1-t1-t2);      /* Tc=taon = (1-t1-t2)/2 */
       v->Tb = v->Tc+t1;                             /* Tb=tbon = taon+t1 */
       v->Ta = v->Tb+t2;                             /* Ta=tcon = tbon+t2 */
    }   
    else if (sector==5)  /*第5扇区: t1=X and t2=-Y (abc ---> Tb,Tc,Ta) */
    {
       t1 = Va;                                      //Va=X,Vb=Y
       t2 = -Vb;
       v->Tb = 0.5 * (1-t1-t2);      /* Tb=taon = (1-t1-t2)/2 */
       v->Tc = v->Tb+t1;                             /* Tc=tbon = taon+t1 */
       v->Ta = v->Tc+t2;                             /* Ta=tcon = tbon+t2 */
    }   
    else if (sector==6)  /* 第6扇区: t1=-Y and t2=-Z (abc ---> Tc,Ta,Tb) */
    {
       t1 = -Vb;
       t2 = -Vc;
       v->Tc = 0.5 * (1-t1-t2);      /* Tc=taon = (1-t1-t2)/2 */
       v->Ta = v->Tc+t1;                             /* Ta=tbon = taon+t1 */
       v->Tb = v->Ta+t2;                             /* Tb=tcon = tbon+t2 */
    }  

	v->Ta1 = 1.0 - v->Ta;
	v->Tb1 = 1.0 - v->Tb;
	v->Tc1 = 1.0 - v->Tc;
    v->Ta2 =  v->Ta;
	v->Tb2 =  v->Tb;
	v->Tc2 =  v->Tc;
    v->Ta = 2 * (v->Ta-0.5);//电压赋值
    v->Tb = 2 * (v->Tb-0.5);
    v->Tc = 2 * (v->Tc-0.5);        
  
}


