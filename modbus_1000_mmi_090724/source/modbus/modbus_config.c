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


/*******************************************************************************/
/*         AS A SERVER IN ANY OF THE MAP_DESC_TYP ARRAYS YOU ONLY NEED         */
/*         THE REF, ADDRESS, DATA AND OFFSET FIELDS                            */
/*         AS A CLIENT THE ADDITIONAL FUNCTION AND NODE_ID FIELDS              */
/*         ARE REQUIRED. YOU CAN INCLUDE THESE AS A SERVER THEY WILL SIMPLY    */
/*         BE IGNORED.                                                         */
/*******************************************************************************/


UINT16 main_data_array[MX_DATA_ARRAY];

/*============================================================================*/
//
//       Coil Mappings

MAP_DESC_TYP coil_map[MX_COILS] = {

//   ref       address  function           data  old_data offset  node_id

   { COIL_1,      1,  MODBUS_WRITE_COIL ,   0,       0,     1,      11},
   { COIL_2,      2,  MODBUS_WRITE_COIL ,   0,       0,     2,      11},
   { COIL_3,      3,  MODBUS_WRITE_COIL ,   0,       0,     3,      11},
   { COIL_4,      4,  MODBUS_WRITE_COIL ,   0,       0,     4,      11},
   { COIL_5,      5,  MODBUS_WRITE_COIL ,   0,       0,     5,      11},
   { COIL_6,      6,  MODBUS_WRITE_COIL ,   0,       0,     6,      11},
   { COIL_7,      7,  MODBUS_WRITE_COIL ,   0,       0,     7,      11},
   { COIL_8,      8,  MODBUS_WRITE_COIL ,   0,       0,     8,      11},

} ;


/*============================================================================*/
//
//       Discrete Inputs Mappings

MAP_DESC_TYP discrete_inputs_map[MX_DI] = {

//   ref             address function                                          data  old_data  offset  node_id

   { DI_1       ,       1,   MODBUS_READ_DISCRETEINPUTS     , 0,     0,     10,      11},
   { DI_2       ,       2,   MODBUS_READ_DISCRETEINPUTS     , 0,     0,     11,      11},
   { DI_3       ,       3,   MODBUS_READ_DISCRETEINPUTS     , 0,     0,     12,      11},
   { DI_4       ,       4,   MODBUS_READ_DISCRETEINPUTS     , 0,     0,     13,      11},
   { DI_5       ,       5,   MODBUS_READ_DISCRETEINPUTS     , 0,     0,     14,      11},
   { DI_6       ,       6,   MODBUS_READ_DISCRETEINPUTS     , 0,     0,     15,      11},
   { DI_7       ,       7,   MODBUS_READ_DISCRETEINPUTS     , 0,     0,     16,      11},
   { DI_8       ,       8,   MODBUS_READ_DISCRETEINPUTS     , 0,     0,     17,      11},
   { DI_9       ,       9,   MODBUS_READ_DISCRETEINPUTS     , 0,     0,     18,      11},

} ;


/*============================================================================*/
//
//       Register Mappings


MAP_DESC_TYP register_map[MX_REG] = {

//   ref             address  function                      data   old_data offset  node_id

   { REG_1      ,       1,    MODBUS_READ_INPUTREGISTERS   ,  0,      0,    20,      11},
   { REG_2      ,       2,    MODBUS_READ_INPUTREGISTERS   ,  0,      0,    21,      11},
   { REG_3      ,       3,    MODBUS_READ_INPUTREGISTERS   ,  0,      0,    22,      11},
   { REG_4      ,       4,    MODBUS_READ_INPUTREGISTERS   ,  0,      0,    23,      11},
   { REG_5      ,       5,    MODBUS_READ_INPUTREGISTERS   ,  0,      0,    24,      11},
   { REG_6      ,       6,    MODBUS_READ_INPUTREGISTERS   ,  0,      0,    25,      11},
   { REG_7      ,       7,    MODBUS_READ_INPUTREGISTERS   ,  0,      0,    26,      11},
   { REG_8      ,       8,    MODBUS_READ_INPUTREGISTERS   ,  0,      0,    27,      11},
   { REG_9        ,       9,    MODBUS_WRITE_REGISTER        ,  0,      0,    28,      11},
   { REG_10      ,       10,    MODBUS_WRITE_REGISTER        ,  0,      0,    29,      11},
   { REG_11      ,       11,    MODBUS_WRITE_REGISTER        ,  0,      0,    30,      11},
   { REG_12      ,       12,    MODBUS_WRITE_REGISTER        ,  0,      0,    31,      11},
   { REG_13      ,       13,    MODBUS_WRITE_REGISTER        ,  0,      0,    32,      11},
   { REG_14      ,       14,    MODBUS_WRITE_REGISTER        ,  0,      0,    33,      11},
   { REG_15      ,       15,    MODBUS_WRITE_REGISTER        ,  0,      0,    34,      11},
   { REG_16      ,       16,    MODBUS_WRITE_REGISTER        ,  0,      0,    35,      11},
} ;


/*============================================================================*/
// Puts unsigned integers into the data array
VOID da_put_UINT16(MAP_DESC_TYP *temp_md,UINT16 value)
{
   main_data_array[temp_md->offset]=value;
   return;
}
/*============================================================================*/
// Puts bits into the data array
VOID da_put_BIT(MAP_DESC_TYP *temp_md,UINT16 value)
{
   main_data_array[temp_md->offset]=(UINT16)(value&0x01);
   return;
}
/*============================================================================*/
// Gets bits from the data array
VOID da_get_BIT(MAP_DESC_TYP *temp_md)
{
   temp_md->data=(main_data_array[temp_md->offset]&0x01);
   return;
}
/*============================================================================*/
// Gets unsigned integers from the data array
VOID da_get_UINT16(MAP_DESC_TYP *temp_md)
{
   temp_md->data=main_data_array[temp_md->offset];
   return;
}
/*============================================================================*/
// Initiates all data arrays to zero values
VOID init_da( VOID )
{
   BYTE i;
   for ( i=0;i<(BYTE)MX_DATA_ARRAY;i++ )
      {
      main_data_array[i]=0;
      }
}








