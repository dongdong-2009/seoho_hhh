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

*******************************************************************************/

#ifndef _MODBUS_H_
#define _MODBUS_H_


#include "datatypes.h"


//#define OPTION_SERVER
#define OPTION_CLIENT
#define DIAGNOSTICS 'N'



/******************************************************************/
/*       MESSAGE STATES (WHILE RECEIVING)                         */
/******************************************************************/
#define CMPLT_COMPLETE  0x01
#define CMPLT_HOPE      0x02
#define CMPLT_NO_HOPE   0x03

/******************************************************************/
/*       MODBUS PARAMETERS                                        */
/******************************************************************/
#define MODBUS_NODE_ID  0x0b

#define MB_POLL_TIMEOUT 1000
#define MB_POLL_DELAY   200

/******************************************************************/
/*       USEFUL DEFINITIONS                                       */
/******************************************************************/
#ifndef NOREF
   #define NOREF(a)     (a=a)
#endif

#define kick_watchdog   ClrWdt

#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))

/******************************************************************/
/*      MODBUS FUNCTION CODES                                     */
/******************************************************************/
#define MODBUS_READ_COILS               0x01
#define MODBUS_READ_DISCRETEINPUTS      0x02
#define MODBUS_READ_HOLDINGREGSTERS      0x03


#define MODBUS_READ_INPUTREGISTERS      0x04
#define MODBUS_WRITE_COIL               0x05
#define MODBUS_WRITE_REGISTER           0x06
#define MODBUS_WRITE_MULT_REG           0x10
#define MODBUS_WRITE_MULT_COIL          0x0F
#define EX_MODBUS_READ_COILS            0x81
#define EX_MODBUS_READ_DISCRETEINPUTS   0x82
#define EX_MODBUS_READ_INPUTREGISTERS   0x84
#define EX_MODBUS_WRITE_COIL            0x85
#define EX_MODBUS_WRITE_REGISTER        0x86
/******************************************************************/
/*      OTHER MODBUS #DEFINES                                     */
/******************************************************************/
#define MX_RX_TX_BUFFERSIZE        256
#define ILLEGAL_DATA_ADDRESS        02
#define ILLEGAL_DATA_VALUE          03
#define BYTE_SIZE                   8
#define COIL_ON                     0xFF00
#define COIL_OFF                    0x0000
#define ON                          1
#define OFF                         0
#define MX_COILS                    8
#define MX_DI                       9
#define MX_REG                     50
#define MX_DATA_ARRAY               100
#define WRITE_COIL_RESPONSE_LENGTH  6
#define READ                        0x01
#define WRITE                       0x02

/******************************************************************/
/*       MAP DESCRIPTOR STRUCTURE                                 */
/******************************************************************/

typedef struct
   {
	UINT16	data;
   } READ_DATA ;

typedef struct
   {
   BYTE    node_id ;
   BYTE    function ;
   UINT16  address;
   UINT16  length;
   UINT16  checksum;

   } QUERY ;



extern QUERY  QUERY_buf;
extern QUERY  QUERY_buf1;
extern QUERY  QUERY_buf2;

extern READ_DATA ReadinReg;

extern UINT32 mseconds ;
extern UINT32 start_time;

extern unsigned int modbus_addr;
extern unsigned char Poll_flag;
extern unsigned char packet_handler;

extern unsigned char communication_fault;
extern unsigned char _communication_fault;

extern BYTE UART_RX_buf[MX_RX_TX_BUFFERSIZE] ;      //Receive Buffer
extern BYTE UART_TX_Buffer[MX_RX_TX_BUFFERSIZE] ;   //Transmit Buffer
extern BYTE UART_rcv_count;                         //Receive Buffer Counter

extern UINT16 main_data_array[MX_DATA_ARRAY];




void makeQuery1(unsigned int address,unsigned int length,unsigned int function,unsigned int node_id);
void makeQuery2(unsigned int address,unsigned int length,unsigned int function,unsigned int node_id);


UINT32 get_milliseconds ( VOID );

/******************************************************************/
/*       PROTOTYPES                                               */
/******************************************************************/
VOID    modbus_init ( VOID ) ;
VOID     idle_modbus(VOID);




BYTE modbusPollReceiveComplete ( VOID );
BYTE modbusCheckAddress( VOID );
BYTE modbusParseMessage( VOID );
void modbusFetchData( QUERY* query);
void modbusStoreData(QUERY* query );
VOID modbusRespond (QUERY* query);
UINT16 modrtuChecksum ( BYTE *chkbuf, BYTE len );
BYTE Get_Modbus_Node_ID ( VOID );
VOID modbusReturnException ( BYTE exception );
VOID modbusGiveUpCharacter( VOID );
VOID clr_rx ( VOID);
VOID modbus_init ( VOID );
VOID get_UART_data ( VOID);

//____________________________________________________________________________________________________________________________________//
//____________________________________________________________________________________________________________________________________//


void check_for_next_modbus_message ( VOID );
VOID build_modbus_poll ( QUERY* query);
VOID poll_modbus_packet(BYTE ID, BYTE function, WORD addr, WORD length);

BYTE modbusPacketResponseComplete (VOID);
VOID parse_reponse_and_store_data ( QUERY* q);

void MODBUS_handler(void);



#endif
