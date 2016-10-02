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
#include "ProtoMod.h"
#include "serial.h"

//#include <SIOAPI.H>

extern BYTE UART_RX_buf[MX_RX_TX_BUFFERSIZE] ; // buffer for PIC UART transmit
extern BYTE UART_TX_Buffer[MX_RX_TX_BUFFERSIZE] ;   //Transmit Buffer
extern BYTE UART_rcv_count;





