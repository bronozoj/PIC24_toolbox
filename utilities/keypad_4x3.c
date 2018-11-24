/**
 * @file keypad_4x3.c
 * @brief This file contains function wrappers for keypad access
 * @author Jaime Bronozo
 * 
 * This is a library for accessing a generic 4x3 matrix keypad to abstract
 * the hardware addressing to read the effective keypad value. This library
 * works in conjunction with the settings found in the configuration file
 * toolbox_settings.h in order to set the proper pin connections to the
 * module.
 * 
 * @note This file is excluded from compilation when
 * __LIBKEYPAD_4x3_DISABLE macro is defined.
 * 
 * @date November 11, 2018 
 **************************************************************************/

/// @cond
#define __LIBKEYPAD_4x3_SETTINGS

#include "toolbox_settings.h"
#include "keypad_4x3.h"

#if __LIBKEYPAD_4x3_DISABLE != 1
/// @endcond

#define KEY_ROW ((keypad_value >> 2) & 3)
#define KEY_COL (keypad_value & 3)

/**
 * @brief stores the keypad value data.
 *
 * Internal variable maintained by the keypad library to store the keypad
 * value and other keypad-related flags.
 * 
 * @note Do not modify this value in any way as it may make the library
 * unstable.
 **************************************************************************/

short int keypad_value;

/**
 * @brief Sets up the necessary settings for keypad reading.
 *
 * Sets up the pins connected to the keypad for change notification
 * mode. 
 * 
 * @return none
 * 
 * @note If _LIBKEYPAD_4x3_CNISR is set to 1, then the change notification
 * interrupt subroutine is automatically managed by the library. If this
 * interrupt is needed for other purposes, configure this macro to 0 and
 * make sure to call keypad_update() inside,
 **************************************************************************/

void keypad_begin(){
    // initialize column pins
    __TRISx(_COL1) = 0;
    __TRISx(_COL2) = 0;
    __TRISx(_COL3) = 0;
    __LATx(_COL1) = 0;
    __LATx(_COL2) = 0;
    __LATx(_COL3) = 0;

    // initialize rows and change sensitivity
    __TRISx(_ROW1) = 1;
    __TRISx(_ROW2) = 1;
    __TRISx(_ROW3) = 1;
    __TRISx(_ROW4) = 1;
    __CN_PUE(_CN_ROW1) = 1;
    __CN_PUE(_CN_ROW2) = 1;
    __CN_PUE(_CN_ROW3) = 1;
    __CN_PUE(_CN_ROW4) = 1;

    // set change notification isr
    _CNIF = 0;
    __CN_IE(_CN_ROW1) = 1;
    __CN_IE(_CN_ROW2) = 1;
    __CN_IE(_CN_ROW3) = 1;
    __CN_IE(_CN_ROW4) = 1;

#if __LIBKEYPAD_4x3_CNISR == 1
    _CNIE = 1;
    _CNIP = 2;
#endif
}

/**
 * @brief Returns the keypad value.
 *
 * Returns the keypad value as such:
 * 
 * |    Keypad     |||
 * |:---:|:---:|:---:|
 * |  0  |  1  |  2  |
 * |  3  |  4  |  5  |
 * |  6  |  7  |  8  |
 * |  9  | 10  | 11  |
 * 
 * @return An integer value based on the button pressed or -1 if there are
 * no buttons pressed.
 **************************************************************************/

short int keypad_number(){
    return KEY_COL ? ((KEY_ROW * 3) + KEY_COL - 1) : -1;
}

/**
 * @brief Gives the keypad row value
 *
 * Returns the row of the keypad button being pressed. The row index is
 * from 0-3.
 * 
 * @return An integer value based on the button pressed or -1 if there are
 * no buttons pressed
 **************************************************************************/

short int keypad_row(){
    return KEY_COL ? KEY_ROW : -1;
}


/**
 * @brief Gives the keypad column value
 *
 * Returns the column of the keypad button being pressed. The column index
 * is from 0-2.
 * 
 * @return An integer value based on the button pressed or -1 if there are
 * no buttons pressed
 **************************************************************************/

short int keypad_col(){
    return KEY_COL - 1;
}

/**
 * @fn void keypad_update()
 * @brief Updates the keypad value seen by the library.
 *
 * This function must be called at least once in a _CNInterrupt()
 * subroutine given that the library has been set up correctly. This
 * function allows the library to coexist with code that requires the use
 * of a change notification interrupt besides using this library.
 * 
 * @return none
 * 
 * @note If __LIBKEYPAD_4x3_CNISR is set to 1, then this function will not
 * exist and will be replaced by a definition of _CNInterrupt(). 
 **************************************************************************/

#if __LIBKEYPAD_4x3_CNISR == 1
void __attribute__ ((interrupt, no_auto_psv)) _CNInterrupt(){
    delay_us(1500);
    _CNIF = 0;  
#else
void keypad_update(){
#endif
    if(!__PORTx(_ROW1))
        keypad_value |= 0x0;
    else if(!__PORTx(_ROW2))
        keypad_value |= 0x4;
    else if(!__PORTx(_ROW3))
        keypad_value |= 0x8;
    else if(!__PORTx(_ROW4))
        keypad_value |= 0xc;
    else{
        keypad_value &= 0xe0;
#if __LIBKEYPAD_4x3_CNISR == 1
        _CNIF = 0;
#endif
        return;
    }

    if(keypad_value & 0x10){
#if __LIBKEYPAD_4x3_CNISR == 1
        _CNIF = 0;
#endif
        return;
    }
    
    __LATx(_COL2) = 1;
    __LATx(_COL3) = 1;
    delay_us(10);
    if(!(__PORTx(_ROW1) & __PORTx(_ROW2) & __PORTx(_ROW3) & __PORTx(_ROW4))){
        __LATx(_COL2) = 0;
        __LATx(_COL3) = 0;
        keypad_value |= 0x1;
#if __LIBKEYPAD_4x3_CNISR == 1
        _CNIF = 0;
#endif
        return;
    }

    __LATx(_COL1) = 1;
    __LATx(_COL2) = 0;
    delay_us(10);
    if(!(__PORTx(_ROW1) & __PORTx(_ROW2) & __PORTx(_ROW3) & __PORTx(_ROW4))){
        __LATx(_COL1) = 0;
        __LATx(_COL3) = 0;
        keypad_value |= 0x2;
#if __LIBKEYPAD_4x3_CNISR == 1
        _CNIF = 0;
#endif
        return;
    }

    __LATx(_COL1) = 0;
    __LATx(_COL3) = 0;
    keypad_value |= 0x3;

#if __LIBKEYPAD_4x3_CNISR == 1
    _CNIF = 0;
#endif
}


/**
 * @brief Invalidates any current value until the next button press.
 * 
 * Useful for detecting between keypresses. It makes the library wait for
 * the button to be unpressed before registering a pressed value again.
 *
 * @note This does nothing when there is no button currently pressed.
 **************************************************************************/
void keypad_reset(){
    keypad_value |= ((keypad_value & 0xf) == 0) ? 0 : 0x10;
}

#endif