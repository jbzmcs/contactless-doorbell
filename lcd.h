#ifndef LCD_H
#define LCD_H
#include <stdint.h>

#include <stdint.h>

// Standard I2C Address for LCD Backpacks (Try 0x27 or 0x3F)
#define LCD_ADDR 0x27 

void lcd_init(void);
void lcd_send_cmd(uint8_t cmd);
void lcd_send_data(uint8_t data);
void lcd_print(char *str);
void lcd_clear(void);
void lcd_set_cursor(uint8_t row, uint8_t col);

#endif