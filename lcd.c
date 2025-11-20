#include <avr/io.h>       
#include <util/delay.h>   
#include "i2c.h"          
#include "lcd.h"

// P0=RS, P1=RW, P2=EN, P3=Backlight, P4-P7=Data
#define LCD_BACKLIGHT 0x08
#define LCD_EN 0x04
#define LCD_RW 0x02
#define LCD_RS 0x01

// Internal helper to send 4 bits at a time
void lcd_i2c_write_byte(uint8_t val) {
    i2c_start();
    i2c_write(LCD_ADDR << 1); // Write mode
    i2c_write(val | LCD_BACKLIGHT); // Keep backlight ON
    i2c_stop();
}

void lcd_pulse_enable(uint8_t val) {
    lcd_i2c_write_byte(val | LCD_EN);  // Enable HIGH
    _delay_us(1);
    lcd_i2c_write_byte(val & ~LCD_EN); // Enable LOW
    _delay_us(50);
}

void lcd_send_nibble(uint8_t val, uint8_t mode) {
    // Send upper 4 bits
    uint8_t high_nibble = val & 0xF0;
    lcd_pulse_enable(high_nibble | mode); // mode is RS (Cmd=0 or Data=1)
    
    // Send lower 4 bits
    uint8_t low_nibble = (val << 4) & 0xF0;
    lcd_pulse_enable(low_nibble | mode);
}

void lcd_send_cmd(uint8_t cmd) {
    lcd_send_nibble(cmd, 0); // RS = 0 for Command
}

void lcd_send_data(uint8_t data) {
    lcd_send_nibble(data, LCD_RS); // RS = 1 for Data
}

void lcd_init(void) {
    i2c_init(); // Make sure I2C hardware is ON
    _delay_ms(50);
    
    // Magic Initialization Sequence (HD44780 datasheet)
    lcd_send_cmd(0x02); // Initialize to 4-bit mode
    lcd_send_cmd(0x28); // 2 lines, 5x7 matrix
    lcd_send_cmd(0x0C); // Display ON, Cursor OFF
    lcd_send_cmd(0x06); // Increment cursor
    lcd_send_cmd(0x01); // Clear display
    _delay_ms(2);
}

void lcd_clear(void) {
    lcd_send_cmd(0x01);
    _delay_ms(2);
}

void lcd_set_cursor(uint8_t row, uint8_t col) {
    // Row 0 starts at 0x80, Row 1 starts at 0xC0
    uint8_t pos = (row == 0) ? 0x80 : 0xC0;
    lcd_send_cmd(pos + col);
}

void lcd_print(char *str) {
    while (*str) {
        lcd_send_data(*str++);
    }
}