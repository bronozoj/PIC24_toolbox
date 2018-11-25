/**
 * @file  toolbox_settings.h
 * @brief This file contains macros to be configured by users.
 * @author Jaime Bronozo
 * 
 * This is a header file used by all libraries as a configuration hub to
 * allow library modification to be compressed into a single file.  
 * Library specific settings can be found here such as pinouts and
 * function availability.  
 * Libraries can be included or excluded by
 * commenting/uncommenting their respective disable macro.
 * 
 * @date November 1, 2018
 **************************************************************************/

/// @cond
#ifndef __TOOLBOX_SETTINGS_COMMON_H__
#define __TOOLBOX_SETTINGS_COMMON_H__
/// @endcond

/** 
 * @def FCY
 * 
 * @brief Defines the instruction cycle frequency
 *  
 * This macro is used for various functions across the library as a
 * standard unit of measurement for the number of instruction cycles per
 * second. This allows the usage of functions inside the library
 * **libpic30.h** as well as various other library functions such as the
 * delay functions.
 * 
 * Set this value in unit of Hertz (Fosc/2 for PIC24F devices).
 **************************************************************************/
#ifndef FCY
#define FCY 16000000
#endif

/// @cond
#include "xc.h"
#include "libpic30.h"
/// @endcond

/** 
 * @def delay_s(d)
 * @param d Time in unit seconds.
 * 
 * @brief Stalls execution of instructions in seconds.
 * 
 * Functions as a definite time delay function wrapped around the
 * **__delay32()** function. This converts the number seconds to stall to
 * its equivalent cycle count.
 * 
 * @note This function is dependent on the proper setting of Fcy
 * 
 * @def delay_ms(d)
 * @param d Time in unit milliseconds.
 * 
 * @brief Stalls execution of instructions in milliseconds.
 * 
 * Functions as a definite time delay function wrapped around the
 * **__delay32()** function. This converts the number milliseconds to stall
 * to its equivalent cycle count.
 * 
 * @note This function is dependent on the proper setting of Fcy.
 * 
 * @def delay_us(d)
 * @param d Time in unit microseconds.
 * 
 * @brief Stalls execution of instructions in microseconds/
 * 
 * Functions as a definite time delay function wrapped around the 
 * **__delay32()** function. This converts the number microseconds to stall
 * to its equivalent cycle count.
 * 
 * @note This function is dependent on the proper setting of Fcy.
 **************************************************************************/
#define delay_s(d)  __delay32( (unsigned long) ((((unsigned long long) d)*(FCY))-2))
#define delay_ms(d) __delay32( (unsigned long) ((((unsigned long long) d)*(FCY)/1000ULL)-2))
#define delay_us(d) __delay32( (unsigned long) ((((unsigned long long) d)*(FCY)/1000000ULL)-2))


/** 
 * @def __PIN_ACCESS(x, y)
 * @param x Term to be concatenated on by *y*.
 * @param y Term to be concatenated to *x*.
 * 
 * @brief Macro for concatenating pin value to function register.
 * 
 * This function is a macro used to expand pin value settings to their
 * respective prefix to access the respective special function registers
 * associated with it.
 * 
 * @note This is internally used by other macros and must not be used for
 * other purposes.
 * 
 * @def __PORTx(x)
 * @param x Pin assigment macro to be used for digital reading.
 * 
 * @brief Allows reading from a labeled pin macro.
 * 
 * This macro appends the term *_PORT* to the pin value associated with
 * the macro to form the _PORTx# term that refers to the specific pin
 * for reading.
 * 
 * @def __LATx(x)
 * @param x Pin assignemnt macro to be used for digital writing.
 * 
 * @brief Allows writing from a labeled pin macro.
 * 
 * This macro appends the term *_LAT* to the pin value associated with
 * the macro to form the _LATx# term that refers to the specific pin
 * for writing.
 * 
 * @def __TRISx(x)
 * @param x Pin assignemnt macro to be used for pin direction
 * 
 * @brief Allows pin direction assignemtn from a labelled pin macro.
 * 
 * This macro appends the term *_TRIS* to the pin value associated with
 * the macro to form the _TRISx# term that refers to the specific pin
 * for I/O direction assignment.
 **************************************************************************/
#define __PIN_ACCESS(x,y) x##y
#define __PORTx(x) __PIN_ACCESS(_R, x)
#define __LATx(x) __PIN_ACCESS(_LAT, x)
#define __TRISx(x) __PIN_ACCESS(_TRIS, x)

/** 
 * @def pinMode(x, y)
 * @param x Pin value.
 * @param y Pin data direction.
 * 
 * @brief Macro for setting pin data direction.
 * 
 * This function is a macro for setting the pin data direction. This works
 * similarly to the Arduino library function of the same name. However,
 * this requires the specification of the whole pin name instead of an
 * arbitrary number to represent every pin.
 * 
 * @def digitalWrite(x, y)
 * @param x Pin value.
 * @param y Pin output value.
 * 
 * @brief Macro for setting pin output value.
 * 
 * This function is a macro for setting the pin output value. This works
 * similarly to the Arduino library function of the same name. However,
 * this requires the specification of the whole pin name instead of an
 * arbitrary number to represent every pin.
 * 
 * @def digitalRead(x)
 * @param x Pin value.
 * 
 * @brief Macro for reading pin input value.
 * 
 * This function is a macro for reading the pin input value. This works
 * similarly to the Arduino library function of the same name. However,
 * this requires the specification of the whole pin name instead of an
 * arbitrary number to represent every pin.
 **************************************************************************/
#define pinMode(x, y) __PIN_ACCESS(_TRIS, x) = (y)
#define digitalWrite(x, y) __PIN_ACCESS(_LAT, x) = (y)
#define digitalRead(x) __PIN_ACCESS(_R, x)

#define __I2C_ACCESS(x, y) I2C##x##y
#define __I2C_STAT(x) __I2C_ACCESS(x, STATbits)
#define __I2C_CON(x) __I2C_ACCESS(x, CONbits) 
#define __I2C_BRG(x) __I2C_ACCESS(x, BRG)
#define __I2C_TRN(x) __I2C_ACCESS(x, TRN)
#define __I2C_RCV(x) __I2C_ACCESS(x, RCV)

/** 
 * @def __LIBLCD_DISABLED
 * 
 * @brief Set to 1 to disable the lcd library
 * 
 * Enables or disables the lcd library. Disabling using this option will
 * automatically exclude compilation of lcd_generic.c and remove
 * lcd_generic.h from inclusion in the main header PIC24_toolbox.h
 **************************************************************************/
#define __LIBLCD_DISABLED 0

/** 
 * @page lcdlib Configuring the Generic LCD Library
 * @tableofcontents
 * 
 * This page describes the available settings to modify in the code
 * section of the LCD library found in toolbox_settings.h.
 * 
 * @section lcdpin Pin Configurations
 * 
 * For proper 4-bit mode operation, the respective macros _DB4, _DB5,
 * _DB6, and _DB7 must be set with the correct pin labels as well as the
 * _RS and _E pins to allow register select and data entering. Proper
 * configuration of this macros will make the library work after a call
 * to lcd_begin().
 * 
 * ```C
 * #define _DB4 B0
 * #define _DB5 B1
 * ...
 * #define _RS  B4
 * #define _E   B5
 * ```
 * 
 * To not conflict with other macros that the user may define, all the
 * lcd setting macros have been enclosed inside a conditional definition
 * that only the lcd implementation files have access to.
 * 
 * @section lcdoff Disabling the LCD library
 * 
 * To exclude the library when not in use with the current project, set
 * the macro __LIBLCD_DISABLED to 1 to exclude the library from
 * compilation when not needed for your application
 * 
 * ```C
 * // #define __LIBLCD_DISABLED 0
 * #define __LIBLCD_DISABLED 1
 * ```
 * 
 * @section lcdlim Limitations
 * 
 * The LCD library is a work in progress and may not be functionally
 * complete. For one, there are no functions for reading back the memory
 * of the LCD as well as defining custom characters. Most of the functions,
 * however, have been wrapped in easy to use functions that abstract lcd
 * operations. This is also dependent on the proper definition of the macro
 * Fcy for the delay functions used to wait for the lcd to be available
 * for receiving commands.
 **************************************************************************/

#ifdef __LIBLCD_SETTINGS

#define __LIBLCD_READ_EN  0

#define _DB4 B0
#define _DB5 B1
#define _DB6 B2
#define _DB7 B3
#define _RS  B4
#define _E   B5
#define _RW  B7

#endif

/** 
 * @def __LIBKEYPAD_4x3_DISABLE
 * 
 * @brief Set to 1 to disable the keypad library
 * 
 * Enables or disables the keypad library. Disabling using this option
 * will automatically exclude compilation of keypad_4x3.c and remove
 * keypad_4x3.h from inclusion in the main header PIC24_toolbox.h
 **************************************************************************/
#define __LIBKEYPAD_4x3_DISABLE 0

/** 
 * @page keypadlib Configuring the 4x3 Number Pad
 * @tableofcontents
 * 
 * This page describes the available settings to modify in the code section
 * 
 * of the 4x3 Number Pad found in toolbox_settings.h.
 *  
 * @section keypadpin Pin Configurations
 *
 * For proper library mapping, the respective macros _COL1, _COL2, _COL3,
 * and _COL4 must be set to the proper pins connected to the column of the
 * matrix while the macros _ROW1, _ROW2, _ROW3, and _ROW4 must be set to
 * the proper pins connected to the rows of the matrix
 * 
 * ```C
 * #define _COL1 B7
 * #define _COL2 B8
 * ...
 * #define _ROW3  B3
 * #define _ROW4  B4
 * ```
 * 
 * The respective macros _CN_ROW1, _CN_ROW2, _CN_ROW3, and _CN_ROW4 must be
 * set to the proper change notification pin number corresponding to the
 * pin. This will ensure that the keypad will be instantaneously updated
 * whenever a button is pressed or released.
 * 
 * ```C
 * #define _CN_ROW1 3
 * #define _CN_ROW2 30
 * #define _CN_ROW3 29
 * #define _CN_ROW4 0
 * ```
 * 
 * @section keypadoff Disabling the Keypad library
 * 
 * To exclude the library when not in use with the current project, set the
 * macro __LIBKEYPAD_4x3_DISABLED to 1 to exclude the library from
 * compilation when not needed for your application
 * 
 * ```C
 * // #define __LIBKEYPAD_4x3_DISABLED 0
 * #define __LIBKEYPAD_4x3_DISABLED 1
 * ```
 * 
 * @section keypadlim Limitations
 * 
 * This library has not been tested rigorously for errors. Bugs can be
 * reported on [Github](https://github.com/bronozoj/PIC24_toolbox).
 **************************************************************************/

#ifdef __LIBKEYPAD_4x3_SETTINGS

/**
 * @def __LIBKEYPAD_4x3_CNISR
 * 
 * @brief Set to 1 to auto-manage the change notification interrupt
 * 
 * Enables or disables the automatic management of the change notification
 * interrupt. If not used, the library can be configured to use the
 * change notification interrupt by itself. If, however, other functions
 * functions must integrate with it, the interrupt priority and enable
 * must be manually set and the function keypad_update() be called inside
 * the _CNInterrupt() function at least once.
 **************************************************************************/
#define __LIBKEYPAD_4x3_CNISR 1

#define __CN_ACCESS(x,y) _CN##y##x
#define __CN_PUE(x) __CN_ACCESS(PUE, x)
#define __CN_IE(x) __CN_ACCESS(IE, x)

#define _COL1 B7
#define _COL2 B11
#define _COL3 B13
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

#define __LIBADCREAD_DISABLE 0

#ifdef __LIBADCREAD_SETTINGS

#define _SAMPLE_PERIOD 2
#define _ADC_PERIOD 1

#endif

#define __LIBEEPROM_I2C_DISABLE 0

#ifdef __LIBEEPROM_I2C_SETTINGS

#define _CLOCK_RATE 200//157
#define _I2C_NUM 1
#define _I2C_SDA B9
#define _I2C_SCL B8

#endif