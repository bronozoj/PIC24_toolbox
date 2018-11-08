/**************************************************************************/
/** @file  keypad_4x3.h
 *  @brief This file contains function wrappers for keypad access
 *  @author Jaime Bronozo
 * 
 *  This is a header file for keypad_4x3.c which must be included to any
 *  source files that require usage of lcd related functions. This
 *  library is dynamically included in the main header PIC24_toolbox.h
 * 
 *  @date November 8, 2018
 */
/**************************************************************************/

#ifndef __KEYPAD_4x3_TOOLBOX_H__
#define __KEYPAD_4x3_TOOLBOX_H__

void keypad_begin();
short int keypad_number();
short int keypad_row();
short int keypad_col();

#if __LIBKEYPAD_4x3_CNISR == 1
void __attribute__ ((interrupt, no_auto_psv)) _CNInterrupt(void);
#else
void keypad_update(void);
#endif

#endif