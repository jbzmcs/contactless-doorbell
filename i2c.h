#ifndef I2C_H
#define I2C_H
#include <avr/io.h>   
#include <stdint.h>  

#include <avr/io.h>

// Function Prototypes
void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_write(uint8_t data);

#endif