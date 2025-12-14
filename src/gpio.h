#ifndef GPIO_H
#define GPIO_H
#include <avr/io.h>
#define INPUT  0
#define OUTPUT 1
#define LOW    0
#define HIGH   1

// Function Prototypes
void gpio_pin_mode(volatile uint8_t *ddr_reg, uint8_t pin, uint8_t mode);
void gpio_pin_write(volatile uint8_t *port_reg, uint8_t pin, uint8_t value);
void gpio_pin_toggle(volatile uint8_t *port_reg, uint8_t pin);
uint8_t gpio_pin_read(volatile uint8_t *pin_reg, uint8_t pin);

#endif