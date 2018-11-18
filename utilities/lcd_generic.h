/**
 * @file  lcd_generic.h
 * @brief This file contains function wrappers for lcd commands
 * @author Jaime Bronozo
 * 
 * This is a header file for lcd_generic.c which must be included to any
 * source files that require usage of lcd related functions. This library
 * is dynamically included in the main header PIC24_toolbox.h
 * 
 * @date November 1, 2018
 **************************************************************************/

#ifndef __LCD_GENERIC_TOOLBOX_H__
#define __LCD_GENERIC_TOOLBOX_H__

/**
 * @def DISPLAY_ON
 *  
 * @brief Flag for turning the display on.
 *  
 * Flag used in the function lcd_display() for the *d* parameter to signify
 * that the display is to be turned on.
 *
 * @def DISPLAY_OFF
 * 
 * @brief Flag for turning the display off.
 *  
 * Flag used in the function lcd_display() for the *d* parameter to signify
 * that the display is to be turned off.
 **************************************************************************/
#define DISPLAY_ON 0x4
#define DISPLAY_OFF 0

/**
 * @def CURSOR_ON
 *  
 * @brief Flag for turning the cursor display on.
 *  
 * Flag used in the function lcd_display() for the *c* parameter to signify
 * that the display is to be shown.
 *
 * @def CURSOR_OFF
 * 
 * @brief Flag for turning the cursor display off.
 *  
 * Flag used in the function lcd_display() for the *c* parameter to signify
 * that the cursor is to be hidden.
 **************************************************************************/
#define CURSOR_ON 0x2
#define CURSOR_OFF 0

/**
 * @def BLINK_ON
 *  
 * @brief Flag for turning the blink animation on.
 *  
 * Flag used in the function lcd_display() for the *b* parameter to signify
 * that the character blinking should be enabled.
 *
 * @def BLINK_OFF
 * 
 * @brief Flag for turning the blink animation off.
 *  
 * Flag used in the function lcd_display() for the *b* parameter to signify
 * that the character blinking should be disabled.
 **************************************************************************/
#define BLINK_ON 1
#define BLINK_OFF 0

/**
 * @def CURSOR_TOP
 *  
 * @brief Flag to start the cursor from the top.
 *  
 * Flag used in the function lcd_cursor() and other functions that include
 * the cursor placing functionality for the *pos* parameter to signify that
 * the cursor must be placed at the top row.
 *
 * @def CURSOR_BOTTOM
 * 
 * @brief Flag to start the cursor from the bottom.
 *  
 * Flag used in the function lcd_cursor() and other functions that include
 * the cursor placing functionality for the *pos* parameter to signify that
 * the cursor must be placed at the bottom row.
 **************************************************************************/
#define CURSOR_TOP 0
#define CURSOR_BOTTOM 0x40

/**
 * @def SHIFT_RIGHT
 *  
 * @brief Flag to shift the display to the right.
 *  
 * Flag used in the function lcd_shift() for the *direction* parameter to
 * signify a display shift to the right.
 *
 * @def SHIFT_LEFT
 * 
 * @brief Flag to shift the display to the left.
 *  
 * Flag used in the function lcd_shift() for the *direction* parameter to
 * signify a display shift to the left.
 */
/**************************************************************************/
#define SHIFT_RIGHT 0x4
#define SHIFT_LEFT 0

void lcd_begin();
void lcd_clear();
void lcd_home();
void lcd_display(uint8_t d, uint8_t c, uint8_t b);
void lcd_shift(uint8_t direction);
void lcd_cursor(uint8_t pos, uint8_t offset);

int lcd_text(char *str);
int lcd_text_offset(char *str, uint8_t pos, uint8_t offset);
int lcd_num(uint16_t number);
int lcd_num_offset(uint16_t number, uint8_t pos, uint8_t offset);

#endif