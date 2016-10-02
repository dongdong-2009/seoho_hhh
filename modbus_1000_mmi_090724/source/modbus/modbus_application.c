 /*******************************************************************************

   Written by:  Haemish Kyd

      It is hereby certified that the author of this code specified above
      has made reference only to information that is in the public domain
      or is available to FieldServer Technologies via a duly signed
      Non-Disclosure Agreement. This code is guaranteed free and clear of
      any claim upon it by any Third Party.

      The basis for this certification includes:
         Original work performed by Eddie Hague.

   Copyright (c) 1991-2003, FieldServer Technologies.
   1991 Tarob Court, Milpitas, CA 95035, USA
   (408) 262 2299

********************************************************************************

 Versions
  1.00aA   15 Oct 04 HLK   Created
  1.00aB   09 Nov 04 HLK   Beautified and organised

*******************************************************************************/

#include <datatypes.h>
#include <ProtoMod.h>
#include "serial.h"

#include "display.h"



unsigned int modbus_addr=0;
unsigned char Poll_flag=0;
unsigned char packet_handler=0;

unsigned char communication_fault=0;
unsigned char _communication_fault=0;
unsigned char communication_fault_cnt=0;


UINT32 mseconds =0;
UINT32 start_time = 0;;










/*============================================================================*/
#ifdef OPTION_CLIENT
VOID idle_modbus(VOID)
{
   static BYTE modbus_state=0;
	BYTE rc_poll;


	switch ( modbus_state )
	{
	case 0:
		if(Poll_flag)
		{
			modbus_state++;
		}
		break;
		
	case 1:
		 WriteTxBuf_Size((char *) UART_TX_Buffer,8);
 		 start_time=0;//get_milliseconds();
 		 mseconds = 0;
		modbus_state++;
			break;
	case 2:
	/******************************************************************/
	/*       GET THE PACKET AND MAKE SURE IT IS COMPLETE              */
	/******************************************************************/
	rc_poll=modbusPacketResponseComplete();

	if ( rc_poll==(BYTE)CMPLT_COMPLETE )
	{
		modbus_state++;
		 start_time=0;//get_milliseconds();
 		 mseconds = 0;
		communication_fault = 0;
		communication_fault_cnt = 0;
		break;
	}
	else if(rc_poll==(BYTE)CMPLT_NO_HOPE)
	{
		 start_time=0;//get_milliseconds();
 		 mseconds = 0;
		communication_fault = 0;
		communication_fault_cnt = 0;

		modbus_state=0;
		clr_rx() ;
		Poll_flag=0;
	}

	/******************************************************************/
	/*       IF NO RESPONSE RECEIVED TIME OUT AND SEND NEXT POLL      */
	/******************************************************************/
	if ((get_milliseconds()-start_time)>=(UINT32)MB_POLL_TIMEOUT)
	{
		modbus_state=0;
		clr_rx() ;

		communication_fault_cnt++;
		if(2<communication_fault_cnt)
		{
			communication_fault = 1;
		}		
		Poll_flag=0;
	}
         break;
      case 3:
         /******************************************************************/
         /*       PARSE THE RESPONSE AND STORE THE DATA                    */
         /******************************************************************/
         parse_reponse_and_store_data (&QUERY_buf);
         clr_rx() ;
         modbus_state++;
         break;
      case 4:
         /******************************************************************/
         /*       POLL DELAY                                               */
         /******************************************************************/
         if ((get_milliseconds()-start_time)>=(UINT32)MB_POLL_DELAY)
            {
            modbus_state=0;
		Poll_flag=0;
            }
      default:
         break;
      }
}
#endif




UINT32 get_milliseconds ( VOID )
{
   return mseconds;
}





void MODBUS_handler(void)
{
	if(!Poll_flag)
	{
		if(packet_handler == 2)
		{
			build_modbus_poll ( &QUERY_buf2);
			packet_handler--;
		}
		else if(packet_handler == 1)
		{
			build_modbus_poll ( &QUERY_buf1);
			packet_handler--;
		}
		else if(packet_handler == 0)
		{
			poll_modbus_packet(MODBUS_NODE_ID, MODBUS_READ_HOLDINGREGSTERS, 0, 100);
		}
		
		Poll_flag=1;
	}
}
