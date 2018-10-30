#ifndef __KEYPAD_4x3_TOOLBOX_H__
#define __KEYPAD_4x3_TOOLBOX_H__

void keypad_begin();

#ifdef __LIBKEYPAD_4x3_CNISR
void __attribute__ ((interrupt, no_auto_psv)) _CNInterrupt(void);
#else
void keypad_update(void);
#endif

#endif