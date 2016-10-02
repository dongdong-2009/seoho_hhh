// EwInv File $Revision: /main/2 $
// Checkin $Date: June 15, 2009   
//###########################################################################
//
// FILE:	SohoGpioSet.c
// Project:	Soho F28335 
// TITLE:	DSP28335 General Purpose I/O Initialization & Support Functions.
//
//###########################################################################
// $Eunwho Release: EwInvF2808_090615 V0.10 $
// $Release Date: June 15th, 2009 $
//###########################################################################

#include "DSP2833x_Device.h"     // Headerfile Include File
#include "DSP2833x_Examples.h"   // Examples Include File

void InitGpio(void)
{ 

     EALLOW;
 
   // Enable PWM1-3 on GPIO0-GPIO5
   GpioCtrlRegs.GPAPUD.bit.GPIO0 	= 0;   // Disable pullup on GPIO0
   GpioCtrlRegs.GPAPUD.bit.GPIO1 	= 0;   // Disble pullup on GPIO1
   GpioCtrlRegs.GPAPUD.bit.GPIO2 	= 0;   // Disble pullup on GPIO2
   GpioCtrlRegs.GPAPUD.bit.GPIO3 	= 0;   // Disble pullup on GPIO3
   GpioCtrlRegs.GPAPUD.bit.GPIO4 	= 0;   // Disble pullup on GPIO4
   GpioCtrlRegs.GPAPUD.bit.GPIO5 	= 0;   // Disble pullup on GPIO5

   GpioCtrlRegs.GPADIR.bit.GPIO0 	= 1;  // GPIO0 = Output
   GpioCtrlRegs.GPADIR.bit.GPIO1 	= 1;  // GPIO1 = Output
   GpioCtrlRegs.GPADIR.bit.GPIO2 	= 1;  // GPIO2 = Output
   GpioCtrlRegs.GPADIR.bit.GPIO3 	= 1;  // GPIO3 = Output
   GpioCtrlRegs.GPADIR.bit.GPIO4 	= 1;  // GPIO4 = Output
   GpioCtrlRegs.GPADIR.bit.GPIO5 	= 1;  // GPIO5 = Output

   GpioCtrlRegs.GPAMUX1.bit.GPIO0 	= 1;  // GPIO0 = PWM1A
   GpioCtrlRegs.GPAMUX1.bit.GPIO1 	= 1;  // GPIO1 = PWM1B
   GpioCtrlRegs.GPAMUX1.bit.GPIO2 	= 1;  // GPIO2 = PWM2A
   GpioCtrlRegs.GPAMUX1.bit.GPIO3 	= 1;  // GPIO3 = PWM2B
   GpioCtrlRegs.GPAMUX1.bit.GPIO4 	= 1;  // GPIO4 = PWM3A
   GpioCtrlRegs.GPAMUX1.bit.GPIO5 	= 1;  // GPIO5 = PWM3B

   // GPIO6 DynamicBrakePWMOut
   GpioCtrlRegs.GPAPUD.bit.GPIO6 	= 1;  // Enable pullup on GPIO6
   GpioDataRegs.GPACLEAR.bit.GPIO6 	= 1;  // Load output latch
   GpioCtrlRegs.GPAMUX1.bit.GPIO6 	= 1;  // GPIO6 = PWM4A
   GpioCtrlRegs.GPADIR.bit.GPIO6 	= 1;  // GPIO6 = Output

   // GPIO7 /RST_DRIVER
   GpioCtrlRegs.GPAPUD.bit.GPIO7 	= 1;  // Enable pullup on GPIO7
   GpioDataRegs.GPASET.bit.GPIO7 	= 1;  // Set Output
   GpioCtrlRegs.GPAMUX1.bit.GPIO7 	= 0;  // GPIO7 = GPIO
   GpioCtrlRegs.GPADIR.bit.GPIO7 	= 1;  // GPIO7 = Output

   // GPIO8 PWM5A AnalogMonitorPWM1
	GpioCtrlRegs.GPAPUD.bit.GPIO8 	= 0;  // Disable pullup on GPIO8
	GpioCtrlRegs.GPADIR.bit.GPIO8 	= 1;  // GPIO8 = Output
	GpioCtrlRegs.GPAMUX1.bit.GPIO8 	= 1;  // GPIO8 = PWM5A
   
   // GPIO9 PWM5B AnalogMonitorPWM2
	GpioCtrlRegs.GPAPUD.bit.GPIO9 	= 0;  // Disable pullup on GPIO9
	GpioCtrlRegs.GPADIR.bit.GPIO9 	= 1;  // GPIO9 = Output
	GpioCtrlRegs.GPAMUX1.bit.GPIO9 	= 1;  // GPIO9 = PWM5B

   // GPIO10 OUT / DC_CONTACT Out
   GpioCtrlRegs.GPAPUD.bit.GPIO10 	= 1;  // Enable pullup on GPIO10
   GpioDataRegs.GPASET.bit.GPIO10 	= 1;  // Set Output High
   GpioCtrlRegs.GPAMUX1.bit.GPIO10 	= 0;  // GPIO10 = GPIO
   GpioCtrlRegs.GPADIR.bit.GPIO10 	= 1;  // GPIO10 = Output

   // GPIO11 OUT TEST_LED1
   GpioCtrlRegs.GPAPUD.bit.GPIO11 	= 1;  // Enable pullup on GPIO11
   GpioDataRegs.GPASET.bit.GPIO11 	= 1;  // Set Output High
   GpioCtrlRegs.GPAMUX1.bit.GPIO11 	= 0;  // GPIO11 = GPIO
   GpioCtrlRegs.GPADIR.bit.GPIO11 	= 1;  // GPIO11 = Output


   // GPIO12 /TZ1 : /FAULT_DRIVER INPUT 
   GpioCtrlRegs.GPAPUD.bit.GPIO12 	= 1;  // Pullup on GPIO12
   GpioDataRegs.GPASET.bit.GPIO12 	= 1;  // Set Output High
   GpioCtrlRegs.GPAMUX1.bit.GPIO12 	= 0;  // GPIO12 = /TZ1
   GpioCtrlRegs.GPADIR.bit.GPIO12 	= 0;  // GPIO12 = INPUT

   // GPIO13 /TZ2 INPUT : /FAULT_DB
   GpioCtrlRegs.GPAPUD.bit.GPIO13 	= 1;  // Disable pullup on GPIO13
   GpioDataRegs.GPASET.bit.GPIO13 	= 1;  // Set Output High
   GpioCtrlRegs.GPAMUX1.bit.GPIO13 	= 0;  // GPIO13 = GPIO
   GpioCtrlRegs.GPADIR.bit.GPIO13 	= 0;  // GPIO13 = Input
/*	FROM ENERWIZ
	GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO13 = 1;
	GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO12 = 1;
	GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;
	GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;
*/
   // GPIO14 /TZ3 INPUT : /FAULT_OC
   GpioCtrlRegs.GPAPUD.bit.GPIO14 	= 1;  // Disable pullup on GPIO14
   GpioDataRegs.GPASET.bit.GPIO14 	= 1;  // Set Output High
   GpioCtrlRegs.GPAMUX1.bit.GPIO14 	= 0;  // GPIO14 = GPIO
   GpioCtrlRegs.GPADIR.bit.GPIO14 	= 0;  // GPIO14 = Input

   // GPIO15 /TZ4 INPUT : /FAULT_OV
   GpioCtrlRegs.GPAPUD.bit.GPIO15 	= 1;  // Disable pullup on GPIO15
   GpioDataRegs.GPASET.bit.GPIO15 	= 1;  // Set Output High
   GpioCtrlRegs.GPAMUX1.bit.GPIO15 	= 0;  // GPIO15 = GPIO
   GpioCtrlRegs.GPADIR.bit.GPIO15 	= 0;  // GPIO15 = Input

   // GPIO16 /TZ5 INPUT : FAULT_ZC 
   GpioCtrlRegs.GPAPUD.bit.GPIO16 	= 1;  // Disable pullup on GPIO16
   GpioDataRegs.GPASET.bit.GPIO16 	= 1;  // Set Output High
   GpioCtrlRegs.GPAMUX2.bit.GPIO16 	= 0;  // GPIO16 = GPIO
   GpioCtrlRegs.GPADIR.bit.GPIO16 	= 0;  // GPIO16 = Input

   // GPIO17 Out Rev.
   GpioCtrlRegs.GPAPUD.bit.GPIO17 	= 1;  // Disable pullup on GPIO17
   GpioDataRegs.GPASET.bit.GPIO17 	= 1;  // Set Output High
   GpioCtrlRegs.GPAMUX2.bit.GPIO17 	= 0;  // GPIO13 = GPIO
   GpioCtrlRegs.GPADIR.bit.GPIO17 	= 1;  // GPIO13 = Output

   // GPIO18 OUT REV.
   GpioCtrlRegs.GPAPUD.bit.GPIO18 	= 1;  // Enable pullup on GPIO18     
   GpioDataRegs.GPASET.bit.GPIO18 	= 1;  // Set Output High
   GpioCtrlRegs.GPAMUX2.bit.GPIO18 	= 0;  // GPIO18 = GPIO
   GpioCtrlRegs.GPADIR.bit.GPIO18 	= 1;  // GPIO13 = OUT

   // GPIO19 OUT TEST_LED2
//   GpioCtrlRegs.GPAPUD.bit.GPIO19 	= 1;  // Enable pullup on GPIO18     
//   GpioDataRegs.GPASET.bit.GPIO19 	= 1;  // Set Output High
//   GpioCtrlRegs.GPAMUX2.bit.GPIO19 	= 0;  // GPIO18 = GPIO
//   GpioCtrlRegs.GPADIR.bit.GPIO19 	= 1;  // GPIO13 = OUT

   
   //GPIO20 QEP1A 
   GpioCtrlRegs.GPAMUX2.bit.GPIO20 	= 1;  // GPIO20 = QEP
   GpioCtrlRegs.GPADIR.bit.GPIO20 	= 0;  // GPIO20 = Input

   //GPIO21 QEP1B 
   GpioCtrlRegs.GPAMUX2.bit.GPIO21 	= 1;  // GPIO20 = QEP
   GpioCtrlRegs.GPADIR.bit.GPIO21 	= 0;  // GPIO20 = Input

   // GPIO22 OUT /PWM_EN 
   //GpioCtrlRegs.GPAPUD.bit.GPIO22 	= 0;  // Enable pullup on GPIO22               
   //GpioDataRegs.GPASET.bit.GPIO22 	= 1;  // Set Output High
   //GpioCtrlRegs.GPAMUX2.bit.GPIO22 	= 3;  // GPIO22 = SCITXDB
   //GpioCtrlRegs.GPADIR.bit.GPIO22 	= 1;  // GPIO22 = OUTPUT

   // GPIO23 Out Rev.
   //GpioCtrlRegs.GPAPUD.bit.GPIO23 	= 0;  // Enable Pullup on GPIO23
   //GpioDataRegs.GPASET.bit.GPIO23 	= 1;  // Set Output High
   //GpioCtrlRegs.GPAMUX2.bit.GPIO23 	= 3;  // GPIO23 = SCIRXDB
   //GpioCtrlRegs.GPADIR.bit.GPIO23 	= 1;  // GPIO23 = Output

   //GPIO24 QEP2A 
   GpioCtrlRegs.GPAMUX2.bit.GPIO24 	= 2;  // GPIO20 = GPIO
   GpioCtrlRegs.GPADIR.bit.GPIO24 	= 0;  // GPIO20 = Input

   //GPIO25 QEP2B 
   GpioCtrlRegs.GPAMUX2.bit.GPIO25 	= 1;  // GPIO20 = GPIO
   GpioCtrlRegs.GPADIR.bit.GPIO25 	= 0;  // GPIO20 = Input

   // GPIO26 No Connect
   GpioCtrlRegs.GPAPUD.bit.GPIO26 	= 1;  // Enable pullup on GPIO24   
   GpioDataRegs.GPASET.bit.GPIO26 	= 1;  // SET Output High            
   GpioCtrlRegs.GPAMUX2.bit.GPIO26 	= 0;  // GPIO24 = GPIO
   GpioCtrlRegs.GPADIR.bit.GPIO26 	= 1;  // GPIO24 = Output

   // GPIO27 OUT BOOT_MODE
   GpioCtrlRegs.GPAPUD.bit.GPIO27 	= 1;  // Enable pullup on GPIO24   
   GpioDataRegs.GPASET.bit.GPIO27 	= 1;  // SET Output High            
   GpioCtrlRegs.GPAMUX2.bit.GPIO27 	= 0;  // GPIO24 = GPIO
   GpioCtrlRegs.GPADIR.bit.GPIO27 	= 1;  // GPIO24 = Output

   // GPIO28 /XZCS6
  // GpioCtrlRegs.GPAPUD.bit.GPIO28 	= 0;  // Enable pullup on GPIO28      
  // GpioCtrlRegs.GPAMUX2.bit.GPIO28 	= 2;  // GPIO28 = /XZCS6
  //  GpioCtrlRegs.GPADIR.bit.GPIO28 	= 1;  // 

   // GPIO29 TXD
  // GpioCtrlRegs.GPAPUD.bit.GPIO29 	= 0;  // Enable pullup on GPIO29         
  // GpioCtrlRegs.GPAMUX2.bit.GPIO29 	= 1;  // GPIO29 = SCITXDA
   
   // GPIO30 RXD_CAN
   GpioCtrlRegs.GPAMUX2.bit.GPIO30 	= 1;  // GPIO30 = RXD_CAN
   GpioCtrlRegs.GPADIR.bit.GPIO30 	= 0;  // GPIO30 = INPUT

   // GPIO31 TXD_CAN
   GpioCtrlRegs.GPAMUX2.bit.GPIO31 	= 1;  // GPIO31 = TXD_CAN
   GpioCtrlRegs.GPADIR.bit.GPIO31 	= 1;  // GPIO31 = OUTPUT

   // GPIO32 I2C_SDAA
   GpioCtrlRegs.GPBPUD.bit.GPIO32 	= 0;  // Enable pullup on GPIO32         
   GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3;  // Asynch input                     
   GpioCtrlRegs.GPBMUX1.bit.GPIO32 	= 1;  // GPIO32 = SDAA
   // GPIO33 I2C_SCLA
   GpioCtrlRegs.GPBPUD.bit.GPIO33 	= 0;  // Enable pullup on GPIO33
   GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3;  // Asynch input               
   GpioCtrlRegs.GPBMUX1.bit.GPIO33 	= 1;  // GPIO33 = SCLA

   // GPIO34 XREADY
//   GpioCtrlRegs.GPBMUX1.bit.GPIO34 	= 2;  // GPIO31 = GPIO
//   GpioCtrlRegs.GPBDIR.bit.GPIO34 	= 0;  // GPIO31 = INPUT

//=============================================================

   // GPIO35 XR/W
//   GpioCtrlRegs.GPBMUX1.bit.GPIO35 	= 2;  // GPIO35 = GPIO
//   GpioCtrlRegs.GPBDIR.bit.GPIO35 	= 1;  // GPIO35 = OUTPUT


   // GPIO34 PWM_EN
   GpioCtrlRegs.GPBPUD.bit.GPIO34 	= 1;  // Enable pullup on GPIO24   
   GpioDataRegs.GPBSET.bit.GPIO34 	= 1;  // SET Output High            
   GpioCtrlRegs.GPBMUX1.bit.GPIO34 	= 0;  // GPIO34 = GPIO
   GpioCtrlRegs.GPBDIR.bit.GPIO34 	= 1;  // GPIO34 = Output

   // GPIO35 OUT BOOT_MODE
   GpioCtrlRegs.GPBPUD.bit.GPIO35 	= 1;  // Enable pullup on GPIO24   
   GpioDataRegs.GPBSET.bit.GPIO35 	= 1;  // SET Output High            
   GpioCtrlRegs.GPBMUX1.bit.GPIO35 	= 0;  // GPIO35 = GPIO
   GpioCtrlRegs.GPBDIR.bit.GPIO35 	= 1;  // GPIO35 = Output


   // GPIO36 RXD  
   GpioCtrlRegs.GPBMUX1.bit.GPIO36 	= 2;  // GPIO36 = /XZCS0
   GpioCtrlRegs.GPBDIR.bit.GPIO36 	= 1;  // GPIO36 = output


   // GPIO37 OUT TEST_LED2
   GpioCtrlRegs.GPBPUD.bit.GPIO37 	= 1;  // Enable pullup on GPIO37     
   GpioDataRegs.GPBSET.bit.GPIO37 	= 1;  // Set Output High
   GpioCtrlRegs.GPBMUX1.bit.GPIO37 	= 0;  // GPIO18 = GPIO
   GpioCtrlRegs.GPBDIR.bit.GPIO37 	= 1;  // GPIO13 = OUT
   // GPIO37 /XZCS7
//   GpioCtrlRegs.GPBMUX1.bit.GPIO37 	= 3;  // GPIO37 = GPIO   // dbsgln
//   GpioCtrlRegs.GPBDIR.bit.GPIO37 	= 1;  // GPIO37 = Output

   // GPIO38 XWE0
   GpioCtrlRegs.GPBMUX1.bit.GPIO38 	= 3;  // GPIO38 = /XWE0
   GpioCtrlRegs.GPBDIR.bit.GPIO38 	= 1;  // GPIO38 = OUTPUT

   // GPIO39 EEPROM_WRITE_PROTECT
   GpioCtrlRegs.GPBPUD.bit.GPIO39 	= 1;  // Enable pullup on GPIO35   
   GpioDataRegs.GPBSET.bit.GPIO39	= 1;  // Set High              
   GpioCtrlRegs.GPBMUX1.bit.GPIO39 	= 0;  // GPIO39 = GPIO
   GpioCtrlRegs.GPBDIR.bit.GPIO39 	= 1;  // GPIO39 = Output

//*************************************
//          40~47 XA00~XA07
//*************************************

   // GPIO48 INPUT GPIO_1
   GpioCtrlRegs.GPBPUD.bit.GPIO48 	= 1;  // GPIO48 Enable pullup   
   GpioDataRegs.GPBSET.bit.GPIO48	= 1;  // GPIO48 Set High              
   GpioCtrlRegs.GPBMUX2.bit.GPIO48 	= 0;  // GPIO48 = GPIO
   GpioCtrlRegs.GPBDIR.bit.GPIO48 	= 0;  // GPIO48 = INPUT

   // GPIO49 INPUT GPIO_2
   GpioCtrlRegs.GPBPUD.bit.GPIO49 	= 1;  // GPIO49 Enable pullup   
   GpioDataRegs.GPBCLEAR.bit.GPIO49	= 1;  // GPIO49 Set Low              
   GpioCtrlRegs.GPBMUX2.bit.GPIO49 	= 0;  // GPIO49 = GPIO
   GpioCtrlRegs.GPBDIR.bit.GPIO49 	= 0;  // GPIO49 = INPUT

   // GPIO50 INPUT GPIO_3
   GpioCtrlRegs.GPBPUD.bit.GPIO50 	= 1;  // GPIO49 Enable pullup   
   GpioDataRegs.GPBCLEAR.bit.GPIO50	= 1;  // GPIO49 Set Low              
   GpioCtrlRegs.GPBMUX2.bit.GPIO50 	= 0;  // GPIO49 = GPIO
   GpioCtrlRegs.GPBDIR.bit.GPIO50 	= 0;  // GPIO49 = INPUT

   // GPIO51 INPUT GPIO_4
   GpioCtrlRegs.GPBPUD.bit.GPIO51 	= 1;  // GPIO49 Enable pullup   
   GpioDataRegs.GPBCLEAR.bit.GPIO51	= 1;  // GPIO49 Set Low              
   GpioCtrlRegs.GPBMUX2.bit.GPIO51 	= 0;  // GPIO49 = GPIO
   GpioCtrlRegs.GPBDIR.bit.GPIO51 	= 0;  // GPIO49 = INPUT

   // GPIO52 INPUT GPIO_5
   GpioCtrlRegs.GPBPUD.bit.GPIO52 	= 1;  // GPIO49 Enable pullup   
   GpioDataRegs.GPBCLEAR.bit.GPIO52	= 1;  // GPIO49 Set Low              
   GpioCtrlRegs.GPBMUX2.bit.GPIO52 	= 0;  // GPIO49 = GPIO
   GpioCtrlRegs.GPBDIR.bit.GPIO52 	= 0;  // GPIO49 = INPUT

   // GPIO53 DS_OPTION OUTPUT
   GpioCtrlRegs.GPBPUD.bit.GPIO53 	= 1;  // GPIO49 Enable pullup   
   GpioDataRegs.GPBCLEAR.bit.GPIO53	= 1;  // GPIO49 Set Low              
   GpioCtrlRegs.GPBMUX2.bit.GPIO53 	= 0;  // GPIO49 = GPIO
   GpioCtrlRegs.GPBDIR.bit.GPIO53 	= 0;  // GPIO49 = OUTPUT

   // GPIO54 OPTION_BRD_IO
   GpioCtrlRegs.GPBPUD.bit.GPIO54 	= 1;  
   GpioDataRegs.GPBSET.bit.GPIO54	= 1;          
   GpioCtrlRegs.GPBMUX2.bit.GPIO54 	= 0;  
   GpioCtrlRegs.GPBDIR.bit.GPIO54 	= 1; 
   
   // GPIO55 RSV IO2 OUTPUT
   GpioCtrlRegs.GPBPUD.bit.GPIO55 	= 1;  
   GpioDataRegs.GPBSET.bit.GPIO55	= 1;          
   GpioCtrlRegs.GPBMUX2.bit.GPIO55 	= 0;  
   GpioCtrlRegs.GPBDIR.bit.GPIO55 	= 1; 

   // GPIO56 422_485_CTRL
   GpioCtrlRegs.GPBPUD.bit.GPIO56 	= 1;  
   GpioDataRegs.GPBSET.bit.GPIO56	= 1;          
   GpioCtrlRegs.GPBMUX2.bit.GPIO56 	= 0;  
   GpioCtrlRegs.GPBDIR.bit.GPIO56 	= 1; 

   // GPIO57 DETECT_OPTION INPUT
   GpioCtrlRegs.GPBPUD.bit.GPIO57 	= 1;  
   GpioDataRegs.GPBSET.bit.GPIO57	= 1;          
   GpioCtrlRegs.GPBMUX2.bit.GPIO57 	= 0;  
   GpioCtrlRegs.GPBDIR.bit.GPIO57 	= 0; 

   // GPIO58 DO1
   GpioCtrlRegs.GPBPUD.bit.GPIO58 	= 1;  
   GpioDataRegs.GPBSET.bit.GPIO58	= 1;          
   GpioCtrlRegs.GPBMUX2.bit.GPIO58 	= 0;  
   GpioCtrlRegs.GPBDIR.bit.GPIO58 	= 1; 

   // GPIO59 DO2
   GpioCtrlRegs.GPBPUD.bit.GPIO59 	= 1;  
   GpioDataRegs.GPBSET.bit.GPIO59	= 1;          
   GpioCtrlRegs.GPBMUX2.bit.GPIO59 	= 0;  
   GpioCtrlRegs.GPBDIR.bit.GPIO59 	= 1; 
   
   // GPIO60 DO3
   GpioCtrlRegs.GPBPUD.bit.GPIO60 	= 1;  
   GpioDataRegs.GPBSET.bit.GPIO60	= 1;          
   GpioCtrlRegs.GPBMUX2.bit.GPIO60 	= 0;  
   GpioCtrlRegs.GPBDIR.bit.GPIO60 	= 1; 
   
   // GPIO61 RSV_IO_1
   GpioCtrlRegs.GPBPUD.bit.GPIO61 	= 1;  
   GpioDataRegs.GPBSET.bit.GPIO61	= 1;          
   GpioCtrlRegs.GPBMUX2.bit.GPIO61 	= 0;  
   GpioCtrlRegs.GPBDIR.bit.GPIO61 	= 1; 

   // GPIO62 RS232_SCIRXDC
   //GpioCtrlRegs.GPBMUX2.bit.GPIO62 	= 1;  
   //GpioCtrlRegs.GPBDIR.bit.GPIO62 	= 0; 

   // GPIO63 RS232_SCITXDC
   //GpioCtrlRegs.GPBMUX2.bit.GPIO63 	= 1;  
   //GpioCtrlRegs.GPBDIR.bit.GPIO63 	= 1; 
     EDIS;

//*************************************
// external bus set 
//*************************************
     EALLOW;
     GpioCtrlRegs.GPCMUX1.bit.GPIO64 = 3;  // XD15
     GpioCtrlRegs.GPCMUX1.bit.GPIO65 = 3;  // XD14
     GpioCtrlRegs.GPCMUX1.bit.GPIO66 = 3;  // XD13
     GpioCtrlRegs.GPCMUX1.bit.GPIO67 = 3;  // XD12

//************** DAC Data Bit *******************//
     GpioCtrlRegs.GPCMUX1.bit.GPIO68 = 3;  // XD11
     GpioCtrlRegs.GPCMUX1.bit.GPIO69 = 3;  // XD10
     GpioCtrlRegs.GPCMUX1.bit.GPIO70 = 3;  // XD19
     GpioCtrlRegs.GPCMUX1.bit.GPIO71 = 3;  // XD8
     GpioCtrlRegs.GPCMUX1.bit.GPIO72 = 3;  // XD7
     GpioCtrlRegs.GPCMUX1.bit.GPIO73 = 3;  // XD6
     GpioCtrlRegs.GPCMUX1.bit.GPIO74 = 3;  // XD5
     GpioCtrlRegs.GPCMUX1.bit.GPIO75 = 3;  // XD4
     GpioCtrlRegs.GPCMUX1.bit.GPIO76 = 3;  // XD3
     GpioCtrlRegs.GPCMUX1.bit.GPIO77 = 3;  // XD2
     GpioCtrlRegs.GPCMUX1.bit.GPIO78 = 3;  // XD1
     GpioCtrlRegs.GPCMUX1.bit.GPIO79 = 3;  // XD0

//************** DAC Address Bit *****************//
     GpioCtrlRegs.GPBMUX1.bit.GPIO40 = 3;  // XA0/XWE1n
     GpioCtrlRegs.GPBMUX1.bit.GPIO41 = 3;  // XA1


     GpioCtrlRegs.GPBMUX1.bit.GPIO42 = 3;  // XA2
     GpioCtrlRegs.GPBMUX1.bit.GPIO43 = 3;  // XA3
     GpioCtrlRegs.GPBMUX1.bit.GPIO44 = 3;  // XA4
     GpioCtrlRegs.GPBMUX1.bit.GPIO45 = 3;  // XA5
     GpioCtrlRegs.GPBMUX1.bit.GPIO46 = 3;  // XA6
     GpioCtrlRegs.GPBMUX1.bit.GPIO47 = 3;  // XA7

     GpioCtrlRegs.GPCMUX2.bit.GPIO80 = 3;  // XA8
     GpioCtrlRegs.GPCMUX2.bit.GPIO81 = 3;  // XA9
     GpioCtrlRegs.GPCMUX2.bit.GPIO82 = 3;  // XA10
     GpioCtrlRegs.GPCMUX2.bit.GPIO83 = 3;  // XA11
     GpioCtrlRegs.GPCMUX2.bit.GPIO84 = 3;  // XA12
     GpioCtrlRegs.GPCMUX2.bit.GPIO85 = 3;  // XA13
     GpioCtrlRegs.GPCMUX2.bit.GPIO86 = 3;  // XA14
     GpioCtrlRegs.GPCMUX2.bit.GPIO87 = 3;  // XA15

     EDIS;
}

//===========================================================================
// No more.
//===========================================================================
