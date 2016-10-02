#ifndef debug_printf_arm_h_
#define debug_printf_arm_h_

extern void debug_devopen( void(*put)(char) );
extern void debug_printf(char const *format, ...);

#endif
