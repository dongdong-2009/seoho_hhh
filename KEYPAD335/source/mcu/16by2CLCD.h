#ifndef _16by2CLCD_H_
#define _16by2CLCD_H_

//#include <avr/pgmspace.h>

//-- LCD RAM Address		
#define	ROW1_ADDR			0	// LCD Row1 Base Address : 0 ~ 15
#define	ROW2_ADDR			16	// LCD Row2 Base Address : 16 ~ 31

extern unsigned char posInpage;

void CLCD_command(unsigned char command);	
void CLCD_data(unsigned char data);
void CLCD_string(unsigned char command, char *string);
void CLCD_initialize(void);

void CLCD_BackLightOnOff(char OnOff);
void CLCD_cursor_ON(unsigned char row, unsigned char offset);
void CLCD_cursor_OFF(void);

#endif
