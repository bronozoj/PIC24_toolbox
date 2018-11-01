/**************************************************************************/
/** @file   lcd_generic.c
 *  @brief  This file contains function wrappers for lcd commands
 *  @author Jaime Bronozo
 * 
 *  This is a library for a generic dot matrix character lcd module. This
 *  library works in conjuction with the settings found in the
 *  configuration file toolbox_settings.h in order to set the proper pin
 *  connections to the module.
 * 
 *  @note This file is excluded from compilation when __LIBLCD_DISABLED
 *  macro is defined.
 * 
 *  @date November 1, 2018 
 */
/**************************************************************************/

/// @cond
#define __LIBLCD_SETTINGS

#include "toolbox_settings.h"
#include "lcd_generic.h"

#ifndef __LIBLCD_DISABLED
/// @endcond

/**************************************************************************/
/** @param rs Sets the register select bit. Setting this to 1 will access
 *  the character register while setting this to 0 will access the command
 *  register.
 *  @param data Sends the lower 4 bits to the lcd registers.
 * 
 *  @return none
 * 
 *  @brief Sends 4 bits of data to the lcd.
 * 
 *  This function is usually used during the lcd software initialization
 *  and must not be used for other purposes once the lcd has been
 *  initialized to avoid incorrectly registering the wrong values during
 *  normal operation.
 */
/**************************************************************************/

void send_4bits(uint8_t rs, uint8_t data){
    __LATx(_RS)  = rs;
    __LATx(_E)   = 1;
    __LATx(_DB7) = (data & 8) ? 1 : 0;
    __LATx(_DB6) = (data & 4) ? 1 : 0;
    __LATx(_DB5) = (data & 2) ? 1 : 0;
    __LATx(_DB4) = data & 1;
    __LATx(_E)   = 0;
}

/**************************************************************************/
/** @param rs Sets the register select bit. Setting this to 1 will access
 *  the character register while setting this to 0 will access the command
 *  register.
 *  @param data Sends the whole 8 bits to the lcd registers in a controlled
 *  manner.
 * 
 *  @return none
 * 
 *  @brief Sends 8 bits of data to the lcd.
 * 
 *  This function is usually used after the lcd has been initialized. This
 *  function is also used by other lcd functions to send commands or
 *  characters in a uniform manner. This is a wrapper function for sending
 *  8 bits of data to the lcd for either 4-bit or 8-bit mode operation
 * 
 *  @note This function is currently limited for 4-bit mode lcd
 *  initialization
 */
/**************************************************************************/

void send_8bits(uint8_t rs, uint8_t data){
    __LATx(_RS)  = rs;
    __LATx(_E)   = 1;
    __LATx(_DB7) = (data & 0x80) ? 1 : 0;
    __LATx(_DB6) = (data & 0x40) ? 1 : 0;
    __LATx(_DB5) = (data & 0x20) ? 1 : 0;
    __LATx(_DB4) = (data & 0x10) ? 1 : 0;
    __LATx(_E)   = 0;
    delay_us(100);
    __LATx(_E)   = 1;
    __LATx(_DB7) = (data & 0x8) ? 1 : 0;
    __LATx(_DB6) = (data & 0x4) ? 1 : 0;
    __LATx(_DB5) = (data & 0x2) ? 1 : 0;
    __LATx(_DB4) = (data & 0x1) ? 1 : 0;
    __LATx(_E)   = 0;
}

/**************************************************************************/
/** @brief Clears the screen and resets the cursor and screen position.
 *
 *  Fills the lcd memory with 0x20 (space) to clear the screen and resets
 *  the position of the cursor and screen. This sends the lcd command
 *  0x01. 
 * 
 *  @return none
 * 
 *  @note This waits for a minimum of 15.2ms instead of checking if the
 *  lcd is ready for a new command due to the lack of an implementation
 *  for lcd read back.
 */
/**************************************************************************/

void lcd_clear(){
    send_8bits(0, 0x1);
    delay_us(15200);
}

/**************************************************************************/
/** @brief Resets the cursor and screen position.
 *
 *  Sets the position of the cursor to address zero and 
 *  the screen shift back to zero. This sends the lcd command 0x02.
 * 
 *  @return none
 *  
 *  @note This waits for a minimum of 15.2ms instead of checking if the
 *  lcd is ready for a new command due to the lack of an implementation
 *  for lcd read back.
 */
/**************************************************************************/

void lcd_home(){
    send_8bits(0, 0x2);
    delay_us(15200);
}

/**************************************************************************/
/** @param d Turns the display on or off. Use flags #DISPLAY_ON for on and
 *  #DISPLAY_OFF for off.
 *  @param c Turns the cursor at the bottom of the current character on or
 *  off. Use flags #CURSOR_ON for on and #CURSOR_OFF for off.
 *  @param b Turns the character blinking on the current character on or
 *  off. Use flags #BLINK_ON for on and #BLINK_OFF for off.
 *
 *  @brief Sets the lcd display parameters
 * 
 *  Sets the parameters for the display, cursor, and blink mode of the lcd.
 *  This ends the lcd command 0x8 and the respective parameters.
 * 
 *  @return none
 * 
 *  @note This waits for a minimum of 40us instead of checking if the 
 *  lcd is ready for a new command due to the lack of an implementation
 *  for lcd read back.
 */
/**************************************************************************/

void lcd_display(uint8_t d, uint8_t c, uint8_t b){
    send_8bits(0, 0x8 | (d ? 0x4 : 0) | c | b);
    delay_us(40);
}

/**************************************************************************/
/** @param direction Sets the screen shift direction to the left or right.
 *  Use flags #SHIFT_RIGHT to shift right and #SHIFT_LEFT to shift left.
 *
 *  @brief Shifts the display one character to the left or right
 * 
 *  Moves the display address to the left or right. This sends the lcd
 *  command 0x18 and the respective parameters.
 * 
 *  @return none
 * 
 *  @note This waits for a minimum of 40us instead of checking if the 
 *  lcd is ready for a new command due to the lack of an implementation
 *  for lcd read back.
 */
/**************************************************************************/

void lcd_shift(uint8_t direction){
    send_8bits(0, 0x18 | (direction ? 0x4 : 0));
    delay_us(40);
}

/**************************************************************************/
/** @param pos Sets the line at which the cursor will start. Use flags
 *  #CURSOR_BOTTOM to place the cursor at the bottom line or #CURSOR_TOP
 *  to place the cursor at the top line.
 *  @param offset Sets the offset of the cursor position from the start of
 *  the line.
 *
 *  @brief Moves the cursor to a different position.
 * 
 *  Moves the cursor address to an absolute position. This sends the lcd
 *  command 0x80 and the respective parameters.
 * 
 *  @return none
 * 
 *  @note This waits for a minimum of 40us instead of checking if the
 *  lcd is ready for a new command due to the lack of an implementation
 *  for lcd read back.
 */
/**************************************************************************/

void lcd_cursor(uint8_t pos, uint8_t offset){
    send_8bits(0, 0x80 | pos | offset);
    delay_us(40);
}

/**************************************************************************/
/** @param str String to display in the lcd
 * 
 *  @brief Displays a string to the lcd.
 *
 *  Display an input string to the lcd starting at the current cursor
 *  position. This assumes that the cursor position is known before usage.
 *  This sends characters to the character register and assumes that the
 *  the lcd is receiving data to the DD RAM.
 * 
 *  @return The number of characters written to the lcd.
 * 
 *  @note This waits for a minimum of 40us per character instead of
 *  checking if the lcd is ready for a new command due to the lack of an
 *  implementation for lcd read back.
 */
/**************************************************************************/

int lcd_text(char *str){
    int j = 0;
    while(str[j] != '\0'){
        send_8bits(1, (str[j]));
        delay_us(40);
        j++;
    }
    return j;
}

/**************************************************************************/
/** @param str String to display in the lcd
 *  @param pos Sets the line at which the cursor will start. Use flags
 *  #CURSOR_BOTTOM to place the cursor at the bottom line or #CURSOR_TOP
 *  to place the cursor at the top line.
 *  @param offset Sets the offset of the cursor position from the start of
 *  the line.
 * 
 *  @brief Displays a string to the lcd in a specific position.
 *
 *  Combines the functions lcd_cursor() and lcd_text() to position the
 *  cursor before displaying an input string to the lcd. This sends
 *  characters to the character register and  assumes that the lcd is
 *  receiving data to the DD RAM.
 * 
 *  @return The number of characters written to the lcd.
 * 
 *  @note This waits for a minimum of 40us per character and cursor
 *  position setting instead of checking if the lcd is ready for a new
 *  command due to the lack of an implementation for lcd read back.
 */
/**************************************************************************/

int lcd_text_offset(char *str, uint8_t pos, uint8_t offset){
    int j = 0;

    // reposition cursor
    send_8bits(0, 0x80 | pos | offset);
    delay_us(40);

    // send each character
    while(str[j] != '\0'){
        send_8bits(1, (str[j]));
        delay_us(40);
        j++;
    }
    return j;
}

/**************************************************************************/
/** @param number An unsigned digit.
 *
 *  @brief Displays an unsigned digit to the lcd.
 * 
 *  Displays an input unsigned integer value to the lcd starting at the
 *  current cursor position. This assumes that the cursor position is
 *  known before usage. This sends the equivalent digit characters to the
 *  character register and assumes that the lcd is receiving data to the
 *  DDRAM register.
 * 
 *  @return The number of characters written to the lcd.
 * 
 *  @note This waits for a minimum of 40us per digit instead of checking
 *  if the lcd is ready for a new command due to the lack of an
 *  implementation for lcd read back.
 */
/**************************************************************************/

int lcd_num(uint16_t number){
    int length = 1;
    if(number >= 10000){
        send_8bits(1, ((number / 10000) % 10) + '0');
        delay_us(40);
        length++;
    }
    
    if(number >= 1000){
        send_8bits(1, ((number / 1000) % 10) + '0');
        delay_us(40);
        length++;
    }

    if(number >= 100){
        send_8bits(1, ((number / 100) % 10) + '0');
        delay_us(40);
        length++;
    }
    
    if(number >= 10){
        send_8bits(1, ((number / 10) % 10) + '0');
        delay_us(40);
        length++;
    }
    
    send_8bits(1, (number % 10) + '0');
    delay_us(40);
    return length;
}

/**************************************************************************/
/** @param number An unsigned digit.
 *  @param pos Sets the line at which the cursor will start. Use flags
 *  #CURSOR_BOTTOM to place the cursor at the bottom line or #CURSOR_TOP
 *  to place the cursor at the top line.
 *  @param offset Sets the offset of the cursor position from the start of
 *  the line.
 *
 *  @brief Displays an unsigned digit to the lcd in a specific position.
 * 
 *  Combines the functions lcd_cursor() and lcd_num() to position the
 *  cursor before displaying an input unsigned digit to the lcd. This
 *  sends the equivalent digit characters to the character register and
 *  assumes that the lcd is receiving data to the DD RAM.
 * 
 *  @return The number of characters written to the lcd.
 * 
 *  @note This waits for a minimum of 40us per digit character and cursor
 *  position setting instead of checking if the lcd is ready for a new
 *  command due to the lack of an implementation for lcd read back.
 */
/**************************************************************************/

int lcd_num_offset(uint16_t number, uint8_t pos, uint8_t offset){
    int length = 1;

    // reposition cursor
    send_8bits(0, 0x80 | pos | offset);
    delay_us(40);

    // display every digit
    if(number >= 10000){
        send_8bits(1, ((number / 10000) % 10) + '0');
        delay_us(40);
        length++;
    }
    
    if(number >= 1000){
        send_8bits(1, ((number / 1000) % 10) + '0');
        delay_us(40);
        length++;
    }

    if(number >= 100){
        send_8bits(1, ((number / 100) % 10) + '0');
        delay_us(40);
        length++;
    }
    
    if(number >= 10){
        send_8bits(1, ((number / 10) % 10) + '0');
        delay_us(40);
        length++;
    }
    
    send_8bits(1, (number % 10) + '0');
    delay_us(40);
    return length;
}

/**************************************************************************/
/** @brief Initializes the lcd for use.
 *
 *  Initializes the lcd through a software reset. This removes the need for
 *  controlling the lcd power supply to initialize. The bit mode on which
 *  the lcd is sending data from is by default in 4-bit mode
 * 
 *  @return none
 * 
 *  @note There is no support yet for 8-bit mode operation as 4-bit mode
 *  uses less pins and has negligible speed impact compared to full 8-bit
 *  mode. 
 */

void lcd_begin(){
    // initialize pins as outputs
    __TRISx(_DB4) = 0;
    __TRISx(_DB5) = 0;
    __TRISx(_DB6) = 0;
    __TRISx(_DB7) = 0;
    __TRISx(_RS)  = 0;
    __TRISx(_E)   = 0;

    // 4-bit mode initialization
    // TODO: add 8-bit mode operation for completeness
    delay_ms(15);
    send_4bits(0, 0x3);
    delay_us(4100);
    send_4bits(0, 0x3);
    delay_us(100);
    send_4bits(0, 0x3);
    delay_us(4100);
    send_4bits(0, 0x2);
    delay_us(4100);
    send_8bits(0, 0x28);
    delay_us(4100);
    send_8bits(0, 0x8);
    delay_us(4100);
    send_8bits(0, 1);
    delay_us(4100);
    send_8bits(0, 0x6);
    delay_us(4100);
    send_8bits(0, 0xf);
    delay_us(4100);
}

#endif