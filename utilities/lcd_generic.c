#define LCD_SETTINGS

#include "toolbox_settings.h"
#include "lcd_generic.h"

/* send_4bits()
 * 
 * sends 4 bits of data to the lcd directly
 * this function does not handle reading the
 * busy flag before transmitting data
 * 
 */

void send_4bits(uint8_t rs, uint8_t data){
    LAT_RS = rs;
    LAT_E = 1;
    LAT_DB7 = (data & 8) ? 1 : 0;
    LAT_DB6 = (data & 4) ? 1 : 0;
    LAT_DB5 = (data & 2) ? 1 : 0;
    LAT_DB4 = data & 1;
    LAT_E = 0;
}

/* send_8bits()
 *
 * sends 8 bits of data to the lcd directly
 * this function does not handle reading the
 * busy flag before transmitting data
 * 
 * TODO: add 8-bit mode operation for completeness
 * 
 */

void send_8bits(uint8_t rs, uint8_t data){
    LAT_RS = rs;
    LAT_E = 1;
    LAT_DB7 = (data & 0x80) ? 1 : 0;
    LAT_DB6 = (data & 0x40) ? 1 : 0;
    LAT_DB5 = (data & 0x20) ? 1 : 0;
    LAT_DB4 = (data & 0x10) ? 1 : 0;
    LAT_E = 0;
    delay_us(100);
    LAT_E = 1;
    LAT_DB7 = (data & 0x8) ? 1 : 0;
    LAT_DB6 = (data & 0x4) ? 1 : 0;
    LAT_DB5 = (data & 0x2) ? 1 : 0;
    LAT_DB4 = (data & 0x1) ? 1 : 0;
    LAT_E = 0;
}

/* lcd_clear()
 *
 * fills the lcd memory with 0x20 (space) to clear the
 * screen and resets the position of the cursor and screen.
 * this waits for a minimum of 15.2ms instead of checking
 * if the lcd is ready for a new command
 */

void lcd_clear(){
    send_8bits(0, 0x1);
    delay_us(15200);
}

/* lcd_home()
 *
 * resets the position of the cursor and screen to the
 * zero position. Sends lcd command b'(1x) and waits
 * for a minimum of 15.2ms instead of checking if the
 * lcd is ready for a new command
 * 
 */

void lcd_home(){
    send_8bits(0, 0x2);
    delay_us(15200);
}

/* lcd_display()
 *
 * sets display, cursor, and blink state and waits
 * for 40us instead of checking if the lcd is ready
 * for a new command. use the appropriate macros 
 * to set the values.
 * 
 * DISPLAY_ON/OFF - turns the display on/off
 * CURSOR_ON/OFF - turns the cursor under the characters
 *      on or off
 * BLINK_ON/OFF - turns the character blink on/off
 * 
 */

void lcd_display(uint8_t d, uint8_t c, uint8_t b){
    send_8bits(0, 0x8 | d | c | b);
    delay_us(40);
}

/* lcd_shift()
 *
 * shifts the display one unit to the left or right
 * 
 * SHIFT_LEFT/RIGHT - shifts the display left/right
 * 
 */

void lcd_shift(uint8_t direction){
    send_8bits(0, 0x18 | direction);
    delay_us(40);
}

/* lcd_cursor()
 *
 * moves the cursor to the specified position in the display
 * 
 * CURSOR_TOP/BOTTOM - positions the cursor to the
 *      top/bottom part of the lcd
 * 
 */

void lcd_cursor(uint8_t pos, uint8_t offset){
    send_8bits(0, 0x80 | pos | offset);
    delay_us(40);
}

/* lcd_text()
 *
 * display a string to the lcd starting at the current
 * cursor position
 * 
 * returns the number of characters displayed
 * 
 */

int lcd_text(char *str){
    int j = 0;
    while(str[j] != '\0'){
        send_8bits(1, (str[j]));
        delay_us(40);
        j++;
    }
    return j;
}

/* lcd_text_offset()
 *
 * moves the cursor to the specified position like
 * lcd_cursor() and displays a string starting from it
 * 
 * returns the number of characters displayed
 *
 */

int lcd_text_offset(char *str, uint8_t pos, uint8_t offset){
    int j = 0;
    send_8bits(0, 0x80 | pos | offset);
    delay_us(40);
    while(str[j] != '\0'){
        send_8bits(1, (str[j]));
        delay_us(40);
        j++;
    }
    return j;
}

/* lcd_num()
 *
 * displays an unsigned 16-bit digit to the lcd starting
 * at the current cursor position
 * 
 * returns the number of characters displayed
 * 
 */

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

/* lcd_num_offset
 *
 * moves the cursor to the specified position like
 * lcd_cursor() and displays a 16-bit digit starting
 * from it
 * 
 * returns the number of characters displayed
 * 
 */

int lcd_num_offset(uint16_t number, uint8_t pos, uint8_t offset){
    int j = 1;
    send_8bits(0, 0x80 | pos | offset);
    delay_us(40);
    if(number >= 10000){
        send_8bits(1, ((number / 10000) % 10) + '0');
        delay_us(40);
        j++;
    }
    
    if(number >= 1000){
        send_8bits(1, ((number / 1000) % 10) + '0');
        delay_us(40);
        j++;
    }

    if(number >= 100){
        send_8bits(1, ((number / 100) % 10) + '0');
        delay_us(40);
        j++;
    }
    
    if(number >= 10){
        send_8bits(1, ((number / 10) % 10) + '0');
        delay_us(40);
        j++;
    }
    
    send_8bits(1, (number % 10) + '0');
    delay_us(40);
    return j;
}

/* lcd_begin()
 *
 * initializes the lcd through a software reset. This
 * removes the need for controlling the lcd power supply
 * to initialize.
 * 
 * TODO: add 8-bit mode operation for completeness
 * 
 */

void lcd_begin(){
    // initialize pins as outputs
    TRIS_DB4 = 0;
    TRIS_DB5 = 0;
    TRIS_DB6 = 0;
    TRIS_DB7 = 0;
    TRIS_RS  = 0;
    TRIS_E   = 0;

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