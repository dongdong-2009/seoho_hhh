//============================================================================
//
// Copyright(C) 2008 HanYoung ELEC. Co., LTD.
// Designed by Paul B.H Park(idonno2000)
//
//
//	filename : 128by64GLCD.C
//	description : ±×·¡ÇÈLCD
//
//	history : - Designed and programmed by Duck-Yong Yoon in 2006.
//            - Revised by Paul B.H Park(idonno2000) at 2008.06.17
//
//============================================================================

#ifndef _128by64GLCD_H_
#define _128by64GLCD_H_

#include <avr/pgmspace.h>

#define BYTE unsigned char
#define WORD unsigned int

#define SCRLLBUF_SIZE 10

extern unsigned char LCDUpdateFlag;
extern unsigned char LCDScrollOnOff;

extern char LCD_buffer[128*8];

 extern char ScrollBuffer[2*SCRLLBUF_SIZE];
 extern  char* pScrollDataStart;
 extern char* pScrollDataEnd;
 extern char* pScrollDataOffset;


void GLCD_command(BYTE cs,BYTE command) ;      /* write a command */
void GLCD_data(BYTE signal,BYTE character) ;       /* write a data */
void GLCD_clear(void)	;			/* clear GLCD screen */
void GLCD_BufClear(void)	;


void GLCD_FullUpdate(void);
void GLCD_LCDUpdate(void);
void GLCD_LCDScrollUpdate(unsigned char line);



void GLCD_BackLightOnOff(char OnOff);//PORD bit 6
void GLCD_BuzzerOnOff(char OnOff);//PORD bit 7 


void GLCD_PutStringScrollBuf(char* str);

void GLCD_putchar1632(unsigned char x, unsigned char y,  char dat_c);
void GLCD_putchar0816(unsigned char x, unsigned char y,  char dat_c);
void GLCD_putchar0508(unsigned char x, unsigned char y,  char dat_c);
void GLCD_putchar1616(unsigned char x, unsigned char y,  int dat_c);

//_________________________________________________________________________________________


void GLCD_print0508(unsigned char x, unsigned char y,  char* str);
void GLCD_print0816(unsigned char x, unsigned char y,  char* str);
void GLCD_print1616(unsigned char x, unsigned char y,  char* str);

void GLCD_inversion(unsigned char x, unsigned char y,  char cnt);

void GLCD_Korean(WORD Kcode);		/* display a 16x16 Dot Korean on GLCD */

unsigned int kssm_convert(unsigned int kssm);





#endif
