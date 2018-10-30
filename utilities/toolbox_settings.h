/*
 * File: toolbox_settings.h
 * Author: bronozoj
 * 
 * Modify pin values accordingly for the library to interface
 * properly with the hardware
 *
 */

#ifndef __TOOLBOX_SETTINGS_COMMON_H__
#define __TOOLBOX_SETTINGS_COMMON_H__

/*
 * Instruction cycle speed
 * 
 * Defining the instruction cycle frequency (FCY) allow
 * you to use calibrated delay functions in units of
 * seconds instead of instruction cycles
 * 
 * This also automatically includes libpic30.h functions
 * with it which allow you to use functions from that
 * library as well
 * 
 */
#ifndef FCY
#define FCY 16000000
#endif

#include "xc.h"
#include "libpic30.h"

#define delay_s(d)  __delay32( (unsigned long) ((((unsigned long long) d)*(FCY))-2))
#define delay_ms(d) __delay32( (unsigned long) ((((unsigned long long) d)*(FCY)/1000ULL)-2))
#define delay_us(d) __delay32( (unsigned long) ((((unsigned long long) d)*(FCY)/1000000ULL)-2))

#define __PIN_ACCESS(x,y) x##y
#define __PORTx(x) __PIN_ACCESS(_R, x)
#define __LATx(x) __PIN_ACCESS(_LAT, x)
#define __TRISx(x) __PIN_ACCESS(_TRIS, x)

/* Generic LCD library config
 *
 * Defining the pin select (TRIS) and write (LAT) for the 
 * pins connected to DB[4:7], E, and RS will allow use of
 * this library in 4-bit mode
 * 
 * TODO: add 8-bit mode compatibility and lcd ram reading
 * 
 * constants to be used in code are found in lcd_generic.h
 * 
 */

#ifdef __LIBLCD_SETTINGS

// uncomment this line to remove all lcd functions
// #define __LIBLCD_DISABLED

#define _DB4 B0
#define _DB5 B1
#define _DB6 B2
#define _DB7 B3
#define _RS  B4
#define _E   B5

#endif

#ifdef __LIBKEYPAD_4x3_SETTINGS

// uncomment this line to remove all keypad functions
// #define __LIBKEYPAD_4x3_DISABLE

// uncomment this line to enable automatic keypad interrupt detection
// #define __LIBKEYPAD_4x3_CNISR

#define __CN_ACCESS(x,y) _CN##y##x
#define __CN_PUE(x) __CN_ACCESS(PUE, x)
#define __CN_IE(x) __CN_ACCESS(IE, x)

#define _COL1 B7
#define _COL2 B8
#define _COL3 B9
#define _ROW1 A1
#define _ROW2 A2
#define _ROW3 A3
#define _ROW4 A4

#define _CN_ROW1 3
#define _CN_ROW2 30
#define _CN_ROW3 29
#define _CN_ROW4 0

#endif

#endif