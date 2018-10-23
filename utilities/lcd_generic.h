#ifndef __LCD_GENERIC_TOOLBOX_H__
#define __LCD_GENERIC_TOOLBOX_H__

#define DISPLAY_ON 0x4
#define DISPLAY_OFF 0

#define CURSOR_ON 0x2
#define CURSOR_OFF 0

#define BLINK_ON 1
#define BLINK_OFF 0

#define CURSOR_TOP 0
#define CURSOR_BOTTOM 0x40

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