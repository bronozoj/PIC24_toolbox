#define __LIBKEYPAD_4x3_SETTINGS

#include "toolbox_settings.h"
#include "keypad_4x3.h"

#ifndef __LIBKEYPAD_4x3_DISABLE

#define KEY_ROW ((keypad_value >> 2) & 3)
#define KEY_COL (keypad_value & 3)


/* variable keypad_value
 *
 * stores the current row and column value of
 * the 4x3 keypad in the form:
 * xx xx <2-bit row> <2-bit column>
 * 
 */
short int keypad_value;

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
    _CNIP = 1;
    __CN_IE(_CN_ROW1) = 1;
    __CN_IE(_CN_ROW2) = 1;
    __CN_IE(_CN_ROW3) = 1;
    __CN_IE(_CN_ROW4) = 1;
    _CNIE = 1;
}

int keypad_number(){
    return KEY_COL ? ((KEY_ROW * 3) + KEY_COL - 1) : -1;
}

int keypad_row(){
    return KEY_COL ? KEY_ROW : -1;
}

int keypad_col(){
    return KEY_COL - 1;
}

#ifdef __LIBKEYPAD_4x3_CNISR
void __attribute__ ((interrupt, no_auto_psv)) _CNInterrupt(void){
#else
void keypad_update(void){
#endif
    _CNIF = 0;
    keypad_value &= 0xf0;
    if(!__PORTx(_ROW1))
        keypad_value |= 0x0;
    else if(!__PORTx(_ROW2))
        keypad_value |= 0x4;
    else if(!__PORTx(_ROW3))
        keypad_value |= 0x8;
    else if(!__PORTx(_ROW4))
        keypad_value |= 0xc;
    else
        return;
    
    __LATx(_COL2) = 1;
    __LATx(_COL3) = 1;
    delay_us(10);
    if(!(__PORTx(_ROW1) & __PORTx(_ROW2) & __PORTx(_ROW3) & __PORTx(_ROW4))){
        __LATx(_COL2) = 0;
        __LATx(_COL3) = 0;
        keypad_value |= 0x1;
        return;
    }

    __LATx(_COL1) = 1;
    __LATx(_COL2) = 0;
    delay_us(10);
    if(!(__PORTx(_ROW1) & __PORTx(_ROW2) & __PORTx(_ROW3) & __PORTx(_ROW4))){
        __LATx(_COL1) = 0;
        __LATx(_COL3) = 0;
        keypad_value |= 0x2;
        return;
    }

    __LATx(_COL1) = 0;
    __LATx(_COL3) = 0;
    keypad_value |= 0x3;
}



#endif