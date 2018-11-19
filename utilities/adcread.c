/**
 * @file  adcread.c
 * @brief This file contains function wrappers for dynamic ADC usage.
 * @author Jaime Bronozo
 * 
 * This is a library for accessing the functionality of the ADC module in
 * a manner that does not require external variables or interrupts. It also
 * abstracts most of the work needed to set it up and makes it operate
 * similar to the **analogRead()** function available in the Arduino
 * framework. This library works in conjunction with the settings found in
 * the configuration file toolbox_settings.h in order to set the proper
 * flags to the module.
 * 
 * @date November 19, 2018
 **************************************************************************/

/// @cond
#define __LIBADCREAD_SETTINGS

#include "toolbox_settings.h"
#include "adcread.h"

#if __LIBADCREAD_DISABLE != 1
/// @endcond


/**
 * @brief Sets up the nexessary values to read analog values.
 * 
 * Sets up the necessary ADC configuration flags to allow it to be used
 * as an on-demand analog value reader. This requires the user to configure
 * the AD1PCFG register to manually exclude pins to be used only for
 * digital I/O purposes.
 **************************************************************************/
void ADC_begin(){
    AD1CON1 = 0xE0; // set form to integer
    AD1CON2 = 0;
    AD1CON3bits.SAMC = _SAMPLE_PERIOD; // Tsamp
    AD1CON3bits.ADCS = _ADC_PERIOD; // Tad
    AD1CHS = 0;
    AD1CON1bits.ADON = 1;
}

/**
 * @param pin Analog pin number.
 * 
 * @brief Reads the specified analog pin.
 * 
 * This returns a value proportional to the voltage drop from the pin to
 * ground. This assumes that the pin is set up for usage as an analog
 * reading pin (by setting the ADC pin configuration and digital pin mode).
 * 
 * @return A value from 0-1023 proportional to Vdd and ground.
 **************************************************************************/

uint16_t analogRead(unsigned short pin){
    AD1CHSbits.CH0SA = pin;
    AD1CON1bits.SAMP = 1;
    while(!AD1CON1bits.DONE);
    AD1CON1bits.DONE = 0;
    return ADC1BUF0;
}

#endif