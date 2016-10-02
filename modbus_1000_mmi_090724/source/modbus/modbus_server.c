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
#include <avr/pgmspace.h>
#include <serial.h>
//#include <SIOAPI.H>

UINT16 main_data_array[MX_DATA_ARRAY];

/*These are the main input and output buffers for the modbus messages*/
BYTE UART_RX_buf[MX_RX_TX_BUFFERSIZE] ;      //Receive Buffer
BYTE UART_TX_Buffer[MX_RX_TX_BUFFERSIZE] ;   //Transmit Buffer
BYTE UART_rcv_count=0;                         //Receive Buffer Counter

BYTE coil_write=0;
BYTE discrete_input_read=0;
BYTE register_write=0;
BYTE coil_read=0;
BYTE register_read=0;
BYTE current_node_ref;
BYTE poll_control=0;

QUERY  QUERY_buf;
QUERY  QUERY_buf1;
QUERY  QUERY_buf2;

READ_DATA ReadinReg;




/*============================================================================*/
//CRC Tables
 const   char  auchCRCHi_exp[256] = {
   0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40,
   0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41,
   0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41,
   0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40,
   0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41,
   0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40,
   0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40,
   0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41,
   0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41,
   0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40,
   0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40,
   0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 
   0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 
   0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 
   0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41,
   0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40
} ;

 const  char  auchCRCLo_exp[256] = {
   0x00, 0xc0, 0xc1, 0x01, 0xc3, 0x03, 0x02, 0xc2, 0xc6, 0x06, 0x07, 0xc7, 0x05, 0xc5, 0xc4, 0x04,
   0xcc, 0x0c, 0x0d, 0xcd, 0x0f, 0xcf, 0xce, 0x0e, 0x0a, 0xca, 0xcb, 0x0b, 0xc9, 0x09, 0x08, 0xc8,
   0xd8, 0x18, 0x19, 0xd9, 0x1b, 0xdb, 0xda, 0x1a, 0x1e, 0xde, 0xdf, 0x1f, 0xdd, 0x1d, 0x1c, 0xdc,
   0x14, 0xd4, 0xd5, 0x15, 0xd7, 0x17, 0x16, 0xd6, 0xd2, 0x12, 0x13, 0xd3, 0x11, 0xd1, 0xd0, 0x10,
   0xf0, 0x30, 0x31, 0xf1, 0x33, 0xf3, 0xf2, 0x32, 0x36, 0xf6, 0xf7, 0x37, 0xf5, 0x35, 0x34, 0xf4,
   0x3c, 0xfc, 0xfd, 0x3d, 0xff, 0x3f, 0x3e, 0xfe, 0xfa, 0x3a, 0x3b, 0xfb, 0x39, 0xf9, 0xf8, 0x38,
   0x28, 0xe8, 0xe9, 0x29, 0xeb, 0x2b, 0x2a, 0xea, 0xee, 0x2e, 0x2f, 0xef, 0x2d, 0xed, 0xec, 0x2c,
   0xe4, 0x24, 0x25, 0xe5, 0x27, 0xe7, 0xe6, 0x26, 0x22, 0xe2, 0xe3, 0x23, 0xe1, 0x21, 0x20, 0xe0,
   0xa0, 0x60, 0x61, 0xa1, 0x63, 0xa3, 0xa2, 0x62, 0x66, 0xa6, 0xa7, 0x67, 0xa5, 0x65, 0x64, 0xa4,
   0x6c, 0xac, 0xad, 0x6d, 0xaf, 0x6f, 0x6e, 0xae, 0xaa, 0x6a, 0x6b, 0xab, 0x69, 0xa9, 0xa8, 0x68,
   0x78, 0xb8, 0xb9, 0x79, 0xbb, 0x7b, 0x7a, 0xba, 0xbe, 0x7e, 0x7f, 0xbf, 0x7d, 0xbd, 0xbc, 0x7c,
   0xb4, 0x74, 0x75, 0xb5, 0x77, 0xb7, 0xb6, 0x76, 0x72, 0xb2, 0xb3, 0x73, 0xb1, 0x71, 0x70, 0xb0,
   0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54,
   0x9c, 0x5c, 0x5d, 0x9d, 0x5f, 0x9f, 0x9e, 0x5e, 0x5a, 0x9a, 0x9b, 0x5b, 0x99, 0x59, 0x58, 0x98,
   0x88, 0x48, 0x49, 0x89, 0x4b, 0x8b, 0x8a, 0x4a, 0x4e, 0x8e, 0x8f, 0x4f, 0x8d, 0x4d, 0x4c, 0x8c,
   0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80, 0x40
} ;

void makeQuery1(unsigned int address,unsigned int length,unsigned int function,unsigned int node_id)
{

	QUERY_buf1.address = address;
	QUERY_buf1.length = length;
	QUERY_buf1.function = function;
	QUERY_buf1.node_id = node_id;
	packet_handler =1;
}
void makeQuery2(unsigned int address,unsigned int length,unsigned int function,unsigned int node_id)
{

	QUERY_buf2.address = address;
	QUERY_buf2.length = length;
	QUERY_buf2.function = function;
	QUERY_buf2.node_id = node_id;
	packet_handler =2;
}


/*============================================================================*/
// Make sure that the modbus poll received is complete as a message.
BYTE modbusPollReceiveComplete ( VOID )
{
	BYTE   target,count_tmp ;
	UINT16 temp_code1,temp_code2,temp_code3 ;
	UINT16 check ;

	get_UART_data () ;

	/******************************************************************/
	/*      LESS THAN FOUR BYTES RECEIVED CMPLT_HOPE                  */
	/******************************************************************/
	if ( UART_rcv_count < (BYTE)4 )  //received less than 4 bytes
	{
		return CMPLT_HOPE ; // hope yet
	}

   /******************************************************************/
   /*      CHECK FOR CORRECT FUNCTION CODE                           */
   /******************************************************************/
	switch ( UART_RX_buf[1] )
	{
		case MODBUS_READ_INPUTREGISTERS :
		case MODBUS_WRITE_REGISTER :
			target = 6 ;
			break ;
		default :
			return CMPLT_NO_HOPE ;
	}

   /******************************************************************/
   /*      HANDLES RESPONSES FROM OTHER SERVERS                      */
   /******************************************************************/
	if ( UART_rcv_count == target )
	{
		temp_code1 = UART_RX_buf[target-2] ;
		temp_code2 = (UINT16) (temp_code1 << 8) ;
		temp_code1 = UART_RX_buf[target-1] ;
		temp_code3 = (UINT16)(temp_code2|temp_code1) ;
		if ( modrtuChecksum ( UART_RX_buf, (BYTE)(target-2)) == temp_code3 )
		{
			return CMPLT_COMPLETE;
		}
	}

   /******************************************************************/
   /*      CHECK FOR CORRECT MESSAGE LENGTH                          */
   /******************************************************************/
	temp_code1 = (UINT16)(target + 2) ;
	count_tmp=(BYTE)temp_code1;
	if ( UART_rcv_count < count_tmp )
	{
		return CMPLT_HOPE ;
	}

   /******************************************************************/
   /*      CHECK FOR CHECKSUM                                        */
   /******************************************************************/
	check = modrtuChecksum ( UART_RX_buf, target ) ;

	temp_code1 = UART_RX_buf[target] ;
	temp_code2 = (UINT16) (temp_code1 << 8) ;
	temp_code1 = UART_RX_buf[target+1] ;
	temp_code3 = (UINT16)(temp_code2|temp_code1) ;
	if ( check == (UINT16) ( temp_code3 ) )
	{
		return CMPLT_COMPLETE ;
	}
	return CMPLT_NO_HOPE ;

}



/*=========================================================================*/
// Check the modbus address
BYTE modbusCheckAddress( VOID )
{
	BYTE id_temp;
	id_temp= (BYTE)Get_Modbus_Node_ID();
	if ( UART_RX_buf[0] == id_temp )
	{
		return TRUE;
	}
	return FALSE;
}


/*============================================================================*/
// Extract relevant data from the message received from the client
BYTE modbusParseMessage( VOID )
{
	BYTE basic_function=0;

	QUERY_buf.node_id= UART_RX_buf[0];
	QUERY_buf.function=  UART_RX_buf[1];

	QUERY_buf.address = (UINT16)(((UINT16)(UART_RX_buf[2]) << 8)|((UINT16)(UART_RX_buf[3])));
	QUERY_buf.length  = (UINT16)(((UINT16)(UART_RX_buf[4]) << 8)|((UINT16)(UART_RX_buf[5])));;
	QUERY_buf.checksum= (UINT16) ((UINT16)(UART_RX_buf[6] << 8) | UART_RX_buf[7] ) ;


   
	switch (QUERY_buf.function)
	{
		case MODBUS_READ_INPUTREGISTERS:
		//case MODBUS_READ_INPUTREGISTERS:
			basic_function=READ;
			break;
		case MODBUS_WRITE_REGISTER:
			basic_function=WRITE;
			break;
		default:
			break;
	}
   	return basic_function;
}



/*============================================================================*/
// Returns a map descriptor containing the data required
void modbusFetchData( QUERY* query)
{
	switch ( query->function )
	{
		case MODBUS_READ_INPUTREGISTERS:
			ReadinReg.data = main_data_array[query->address];
			break;
		default:	break;
	}

}

/*============================================================================*/
// Return a map descriptor that the data will be stored to
void modbusStoreData(QUERY* query )
{
	switch ( query->function )
	{
		case MODBUS_WRITE_REGISTER:
			main_data_array[query->address] = query->length;
			break;
		default:	break;
	}
}


/*=========================================================================*/
// Respond to the modbus poll from the client
VOID modbusRespond (QUERY* query)
{
//	BYTE    byte_cnt, i , t, bool_val ;
//	BYTE    byte_store, temp_bool ;

	UINT16 check ;

	switch (query->function )
	{
/******************************************************************/
/*       FUNCTION CODE 0X04  -> READ REGISTER                     */
/******************************************************************/
		case MODBUS_READ_INPUTREGISTERS :
		{
			UART_TX_Buffer[0] = Get_Modbus_Node_ID();
			UART_TX_Buffer[1] = query->function;
			UART_TX_Buffer[2] = (BYTE)( query->length * 2) ; // num data bytes

			UART_TX_Buffer[3] = (BYTE)(ReadinReg.data  >> 8) ;
			UART_TX_Buffer[4] = (BYTE)(ReadinReg.data  & 0x00ff) ; // num data bytes
			
			check = modrtuChecksum( UART_TX_Buffer,  5 ) ;
			UART_TX_Buffer[5] = (BYTE)(check >> 8) ;
			UART_TX_Buffer[6] = (BYTE)(check & 0xff) ;

			WriteTxBuf_Size((char *) UART_TX_Buffer,7);
		}
		return ;
/******************************************************************/
/*       FUNCTION CODE 0X06 -> WRITE REGISTER                     */
/******************************************************************/
		case MODBUS_WRITE_REGISTER:
		{
			UART_TX_Buffer[0] = Get_Modbus_Node_ID();
			UART_TX_Buffer[1] = query->function;
			UART_TX_Buffer[2] = (BYTE)( query->length * 2) ; // num data bytes

			UART_TX_Buffer[3] = (BYTE)(query->length  >> 8) ;
			UART_TX_Buffer[4] = (BYTE)(query->length  & 0x00ff) ; // num data bytes
			
			check = modrtuChecksum( UART_TX_Buffer,  5 ) ;
			UART_TX_Buffer[5] = (BYTE)(check >> 8) ;
			UART_TX_Buffer[6] = (BYTE)(check & 0xff) ;

			WriteTxBuf_Size((char *) UART_TX_Buffer,7);
		}
		return;
	}
}





/*============================================================================*/
// Procedure to check the checksum of the message
UINT16 modrtuChecksum ( BYTE *chkbuf, BYTE len )
{

   BYTE    uchCRCHi = 0xff ;
   BYTE    uchCRCLo = 0xff ;
   BYTE  uIndex ;
   UINT16  temp_code;

   while ( len )
      {
      uIndex = (UINT16) ( uchCRCHi ^ *chkbuf++ ) ;

      uchCRCHi = (BYTE) ( uchCRCLo ^ auchCRCHi_exp[uIndex] ) ;
      uchCRCLo =  auchCRCLo_exp[uIndex];
      len-- ;
      }
   temp_code = (UINT16) uchCRCHi;
   temp_code = (UINT16) (temp_code << 8);
   
   return(UINT16) (  temp_code | uchCRCLo ) ;
}
/*============================================================================*/
// Access the node ID of the system
BYTE Get_Modbus_Node_ID ( VOID )
{
   BYTE Node_Address = MODBUS_NODE_ID;

   return Node_Address ;
}



/*=========================================================================*/
// Send exception response to client
VOID modbusReturnException ( BYTE exception )
{
   UART_TX_Buffer[1] |= 0x80 ;      //function code for exception
   UART_TX_Buffer[2]  = exception ;
   WriteTxBuf_Size((char *) UART_TX_Buffer,3);
}


/*============================================================================*/
// In the event of the message being incomplete give up one character and recheck the message
VOID modbusGiveUpCharacter( VOID )
{
	BYTE i;
	for ( i = 1 ; i < UART_rcv_count ; i++ )
	{
		UART_RX_buf[i-1] = UART_RX_buf[i] ;
	}
	UART_rcv_count--;
	if ( UART_rcv_count==(BYTE)0 )
	{
		modbusReturnException (ILLEGAL_DATA_ADDRESS) ;
		
	}
		clr_rx();
	return ;
}



/*============================================================================*/
VOID clr_rx ( VOID)
{
	QUERY_buf.function = 0;
	QUERY_buf.address = 0;
	QUERY_buf.length = 1;

   UART_rcv_count = 0;
}


/*============================================================================*/
// Initiate the modbus buffers
VOID modbus_init ( VOID )
{
   BYTE i ;

   clr_rx () ;
   for ( i = 0 ; i < (BYTE)MX_RX_TX_BUFFERSIZE ; i++ )
      {
      UART_RX_buf[i]=0;
      UART_TX_Buffer[i]=0;
      }
}

/*============================================================================*/
// Extract characters from the UART buffer
VOID get_UART_data ( VOID)
{
  CHAR ch ;
   // this function reads data from uart circular buffer into UART_RX_buf

	if( ReadRxBuf_Size(&ch,-1))
	{
		ReadRxBuf_Size(&ch,1);
		UART_RX_buf[UART_rcv_count++]=(BYTE)ch;
	}
}







//____________________________________________________________________________________________________________________________________//
//____________________________________________________________________________________________________________________________________//
//____________________________________________________________________________________________________________________________________//
//____________________________________________________________________________________________________________________________________//
//____________________________________________________________________________________________________________________________________//
//____________________________________________________________________________________________________________________________________//
//____________________________________________________________________________________________________________________________________//
//____________________________________________________________________________________________________________________________________//



/*===========================================================================*/
// Checks for the next message to be sent
void check_for_next_modbus_message ( void )
{
//   BYTE  i;
	switch ( poll_control )
	{
		case 0:
		/******************************************************************/
		/*      READ INPUT REGISTERS                                      */
		/******************************************************************/
			
			if ( (BYTE)register_read >= (BYTE)MX_REG ) register_read = 0 ;

			QUERY_buf.function = MODBUS_READ_INPUTREGISTERS;
			QUERY_buf.address = register_read;
			QUERY_buf.length = 1;

			register_read++;
			poll_control++;
			break;

		case 1:
		/******************************************************************/
		/*      WRITE REGISTER                                            */
		/******************************************************************/
			
			if ( (BYTE)register_write > (BYTE)MX_REG ) register_write = 0 ;

			QUERY_buf.function = MODBUS_WRITE_REGISTER;
			QUERY_buf.address = register_write + MX_REG;
			QUERY_buf.length = 1;

			register_write++;
			poll_control=0;
			break;
			
		default:  break;
	}
}



/*============================================================================*/
// Builds the modbus message
VOID poll_modbus_packet(BYTE ID, BYTE function, WORD addr, WORD length)
{
   UINT16 crc ;

   /******************************************************************/
   /*      NODE ID TO MESSAGE                                        */
   /******************************************************************/
   UART_TX_Buffer[0]=ID;
   /******************************************************************/
   /*      FUNCTION TO MESSAGE                                       */
   /******************************************************************/
   UART_TX_Buffer[1]=function;
   /******************************************************************/
   /*      ADDRESS TO MESSAGE HIGH THEN LOW                          */
   /******************************************************************/
   UART_TX_Buffer[2]=(BYTE)((addr >> 8)&0x00ff) ;
   UART_TX_Buffer[3]=(BYTE)((addr)&0x00ff) ;
   /******************************************************************/
   /*      LENGTH/DATA TO MESSAGE HIGH THEN LOW                      */
   /******************************************************************/
   UART_TX_Buffer[4]=(BYTE)((length >> 8)&0x00ff) ;
   UART_TX_Buffer[5]=(BYTE)((length)&0x00ff) ;
   /******************************************************************/
   /*      CHECKSUM TO MESSAGE  HIGH THEN LOW                        */
   /******************************************************************/
   crc = modrtuChecksum (UART_TX_Buffer, 6) ;
   UART_TX_Buffer[6]=(BYTE)((crc >> 8) & 0xff) ;
   UART_TX_Buffer[7]=(BYTE)((crc) & 0xff) ;

   current_node_ref = UART_TX_Buffer[0];

	QUERY_buf.address = addr;
	QUERY_buf.length = length;
	QUERY_buf.function = function;
	QUERY_buf.node_id = ID;


}



/*============================================================================*/
// Builds the modbus message
VOID build_modbus_poll ( QUERY* query)
{
   UINT16 crc ;

   /******************************************************************/
   /*      NODE ID TO MESSAGE                                        */
   /******************************************************************/
   UART_TX_Buffer[0]=Get_Modbus_Node_ID ( );
   /******************************************************************/
   /*      FUNCTION TO MESSAGE                                       */
   /******************************************************************/
   UART_TX_Buffer[1]=query->function;
   /******************************************************************/
   /*      ADDRESS TO MESSAGE HIGH THEN LOW                          */
   /******************************************************************/
   UART_TX_Buffer[2]=(BYTE)((query->address >> 8)&0x00ff) ;
   UART_TX_Buffer[3]=(BYTE)((query->address)&0x00ff) ;
   /******************************************************************/
   /*      LENGTH/DATA TO MESSAGE HIGH THEN LOW                      */
   /******************************************************************/
   UART_TX_Buffer[4]=(BYTE)((query->length >> 8)&0x00ff) ;
   UART_TX_Buffer[5]=(BYTE)((query->length)&0x00ff) ;
   /******************************************************************/
   /*      CHECKSUM TO MESSAGE  HIGH THEN LOW                        */
   /******************************************************************/
   crc = modrtuChecksum (UART_TX_Buffer, 6) ;
   UART_TX_Buffer[6]=(BYTE)((crc >> 8) & 0xff) ;
   UART_TX_Buffer[7]=(BYTE)((crc) & 0xff) ;

   current_node_ref = UART_TX_Buffer[0];

}


/*============================================================================*/

BYTE modbusPacketResponseComplete (VOID)
{
   UINT16  crc ;
   BYTE    target=0 ;

   get_UART_data () ;

   /******************************************************************/
   /*      LESS THAN FOUR BYTES RECEIVED CMPLT_HOPE                  */
   /******************************************************************/
   if ( UART_rcv_count < (BYTE)4 )
      {
      return( CMPLT_HOPE ) ;
      }

   /******************************************************************/
   /*      IS THIS THE REPLY TO THE SENT MESSAGE                     */
   /******************************************************************/
   if ( UART_RX_buf[0] != current_node_ref )
      {
      return CMPLT_NO_HOPE;
      }

   /******************************************************************/
   /*      CHECK FOR EXCEPTION CODE                                  */
   /******************************************************************/
   if ( UART_RX_buf[1] & 0x80 )
      {
      target = 3 ;
      }
   else
      {
      switch ( UART_RX_buf[1] )
         {
         case MODBUS_READ_COILS            :
         case MODBUS_READ_DISCRETEINPUTS   :
         case MODBUS_READ_INPUTREGISTERS   :
	  case 	MODBUS_READ_HOLDINGREGSTERS:
            target = (BYTE)(UART_RX_buf[2] + 3) ;
            break ;
         case MODBUS_WRITE_COIL            :
         case MODBUS_WRITE_REGISTER        :
            target = 5 ;
            break ;
         default:
            break;
         }
      }

   /******************************************************************/
   /*      'target' IS THE LENGTH OF THE MESSAGE WITHOUT CHECKSUM    */
   /******************************************************************/
   if ( UART_rcv_count < ( target + 2 ) )
      {
      return CMPLT_HOPE ;
      }
   else
      {
      crc = modrtuChecksum (UART_RX_buf, (BYTE)(UART_rcv_count - 2)) ;
      if ( UART_RX_buf[UART_rcv_count-2] == ((BYTE)(crc >> 8) & 0xff) && ((BYTE)UART_RX_buf[UART_rcv_count-1] == (BYTE)(crc & 0xFF)) )
         {
         return CMPLT_COMPLETE ;
         }
      else
         {
        return CMPLT_NO_HOPE;
		 //return CMPLT_COMPLETE ;
         }
      }
}


/*============================================================================*/
// Handle the response from the server
VOID parse_reponse_and_store_data ( QUERY* q)
{
BYTE c;
   BYTE function ;
   BYTE node_id ;
   BYTE cnt;
//   UINT16 value;
   
   
   node_id = UART_RX_buf[0];
   function = UART_RX_buf[1];
   cnt = UART_RX_buf[2];

	switch ( function )
	{
		case MODBUS_READ_INPUTREGISTERS :
		case MODBUS_READ_HOLDINGREGSTERS:
		{
		/******************************************************************/
		/*      READ INPUT REGISTERS                                      */
		/******************************************************************/
			for(c=0;  c<cnt; c=c+2)
			{
				main_data_array[q->address++] =  (UINT16)(((UINT16)(UART_RX_buf[c+3]) << 8)|((UINT16)(UART_RX_buf[c+4]))); 
			}

		}
		break ;
		case MODBUS_WRITE_REGISTER :
		{
		/******************************************************************/
		/*      WRITE REGISTER                                             */
		/******************************************************************/


		}
		break ;
		default:
		break;
	}
}

