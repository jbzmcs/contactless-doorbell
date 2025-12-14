#include <avr/io.h>       
#include <util/twi.h>     
#include "i2c.h"          

void i2c_init(void) {
    // Set I2C Clock Speed to 100kHz
    // Formula: SCL_freq = CPU_freq / (16 + 2(TWBR) * Prescaler)
    // 16MHz / (16 + 2(72) * 1) ~= 100kHz
    TWSR = 0x00; // Prescaler = 1
    TWBR = 0x48; // Decimal 72
    
    // Enable TWI (Two Wire Interface) hardware
    TWCR = (1 << TWEN);
}

void i2c_start(void) {
    // Send START condition
    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
    
    // Wait for hardware flag (TWINT) to show it's done
    while (!(TWCR & (1<<TWINT)));
}

void i2c_stop(void) {
    // Send STOP condition
    TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
}

void i2c_write(uint8_t data) {
    // Load data into register
    TWDR = data;
    
    // Start transmission (Clear TWINT flag)
    TWCR = (1<<TWINT) | (1<<TWEN);
    
    // Wait for transmission to finish
    while (!(TWCR & (1<<TWINT)));
}