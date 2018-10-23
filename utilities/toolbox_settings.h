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
#define FCY 4000000

#include "xc.h"
#include "libpic30.h"

#define delay_s(d)  __delay32( (unsigned long) ((((unsigned long long) d)*(FCY))-2))
#define delay_ms(d) __delay32( (unsigned long) ((((unsigned long long) d)*(FCY)/1000ULL)-2))
#define delay_us(d) __delay32( (unsigned long) ((((unsigned long long) d)*(FCY)/1000000ULL)-2))


/* Generic LCD library config
 *
 * Defining the pin select (TRIS) and write (LAT) for the 
 * pins connected to DB[4:7], E, and RS will allow use of
 * this library in 4-bit mode
 * 
 * TODO: add 8-bit mode compatibility
 * 
 * constants to be used in code are found in lcd_generic.h
 * 
 */

#ifdef LCD_SETTINGS

#define TRIS_DB4 _TRISB0
#define TRIS_DB5 _TRISB1
#define TRIS_DB6 _TRISB2
#define TRIS_DB7 _TRISB3
#define TRIS_RS  _TRISB4
#define TRIS_E   _TRISB5

#define LAT_DB4  _LATB0
#define LAT_DB5  _LATB1
#define LAT_DB6  _LATB2
#define LAT_DB7  _LATB3
#define LAT_RS   _LATB4
#define LAT_E    _LATB5

#endif

#endif