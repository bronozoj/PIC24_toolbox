/** 
 * @file  adcread.h
 * @brief This file contains function wrappers for dynamic ADC usage
 * @author Jaime Bronozo
 * 
 * This is a header file for adcread.c which must be included to any source
 * files that require usage of analog reading related functions. This
 * library is dynamically included in the main header PIC24_toolbox.h
 * 
 * @date November 8, 2018
 **************************************************************************/

#ifndef __ADCREAD_TOOLBOX_H__
#define __ADCREAD_TOOLBOX_H__

void ADC_begin();
uint16_t analogRead(unsigned short pin);

#endif