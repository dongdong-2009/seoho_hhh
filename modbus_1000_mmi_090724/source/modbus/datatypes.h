/*******************************************************************************


               datatypes.h


********************************************************************************

   Written by:  ______

      It is hereby certified that the author of this code specified above
      has made reference only to information that is in the public domain
      or is available to FieldServer Technologies via a duly signed
      Non-Disclosure Agreement. This code is guaranteed free and clear of
      any claim upon it by any Third Party.

      The basis for this certification includes:
         (Choose one)
         Advice from officer of FST, name _____, date _____.
         Based on an analysis of products obtained from ____________. Details in CE File ______. (Customer Equipment File Log).
         General or specific industry knowledge obtained from ____.
         Non-Disclosure Agreement (NDA) between _____ and FST. Dated _____.
         Original work performed by ________________.
         Public domain data/knowledge obtained from _______.
         Specific written release signed by ______.

   Copyright (c) 1991-2003, FieldServer Technologies.
   1991 Tarob Court, Milpitas, CA 95035, USA
   (408) 262 2299

********************************************************************************

 Versions
 1.00aA  24 jul 03 EKH  Created.
 1.00aM  19 Nov 03 MdS  lead_lag.c created.

*******************************************************************************/

#ifndef LOCAL
   #define LOCAL
#endif


#ifndef NOREF
   #define NOREF(a)     (a=a)
#endif

#if ! defined ( BYTE ) && ! defined ( _WINDEF_ )
   /* wtypes.h typedefs BYTE */
   #define   BYTE    unsigned char
   #define   UCHAR   unsigned char
   #define   CHAR    char
#endif

#if ! defined ( INT ) && ! defined ( _WINDEF_ )
   #define   INT    int
#endif

#ifndef INT16
   #define   INT16  short
   #define   INT32  long
#endif

#if ! defined ( UINT ) && ! defined ( _WINDEF_ )
   #define   UINT   unsigned int
   #define WORD unsigned int
#endif

#ifndef UINT16
   #define   UINT16 unsigned short
   #define   UINT32 unsigned long
   #define   DWORD unsigned long
#endif

#if ! defined ( FLOAT ) && ! defined ( _WINDEF_ )
   #define   FLOAT   float
   #define   DOUBLE  double
#endif

#ifndef   VOID
   #define   VOID    void
#endif

#ifndef TRUE
   #define  TRUE     1
   #define  FALSE    0
#endif


