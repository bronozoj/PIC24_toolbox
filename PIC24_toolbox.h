/** 
 * @file  PIC24_toolbox.h
 * @brief This file is to be included for all source files that will use
 * this library.
 * @author Jaime Bronozo
 * 
 * This header manages the libraries to be included and excluded,
 * allowing the use of the whole library with only a single file to
 * include.
 * 
 * @note To configure the behavior of the individual libraries, see
 * toolbox_settings.h.
 * 
 * @date November 1, 2018
 **************************************************************************/

#ifndef __PIC24_EXTRAS_TOOLBOX_H__
#define __PIC24_EXTRAS_TOOLBOX_H__

/** 
 * @mainpage PIC24 General Usage Toolbox
 * 
 * @section intromain Introduction
 * 
 * This is a collection of libraries meant for usage on a PIC24F family
 * device. The whole codebase is written in C and requires a fair bit of
 * knowledge concerning programming with PIC microcontrollers using MPLAB
 * X IDE. This library is fully compatible and made for the XC15 compiler
 * and uses libraries from it to function.
 * 
 * @section usagemain Usage
 * 
 * Simply copy the whole directory to your project folder and add all the
 * files in the directory to the sources folder. Then, simply include
 * PIC24_toolbox.h to your main source file (and any other files that will
 * use the library). Tweak the settings of the libraries you need in
 * toolbox_settings.h and youre good to use this for your projects.
 * 
 * For more information on how to use each library, see the following pages
 * for configuration and the respective library header file for the list of
 * available functions that each library has to offer.
 **************************************************************************/

#include "utilities/toolbox_settings.h"

#if __LIBLCD_DISABLED != 1
#include "utilities/lcd_generic.h"
#endif

#if __LIBKEYPAD_4x3_DISABLE != 1
#include "utilities/keypad_4x3.h"
#endif

#if __LIBADCREAD_DISABLE != 1
#include "utilities/adcread.h"
#endif

#endif