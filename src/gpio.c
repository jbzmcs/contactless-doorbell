#include "gpio.h"

// Set pin as Input or Output
// Matches header prototype: void gpio_pin_mode(volatile uint8_t *ddr_reg, uint8_t pin, uint8_t mode);
void gpio_pin_mode(volatile uint8_t *ddr_reg, uint8_t pin, uint8_t mode) {
    if (mode == OUTPUT) {
        *ddr_reg |= (1 << pin);  // Set bit to 1
    } else {
        *ddr_reg &= ~(1 << pin); // Clear bit to 0
    }
}

// Write HIGH or LOW to a pin
void gpio_pin_write(volatile uint8_t *port_reg, uint8_t pin, uint8_t value) {
    if (value == HIGH) {
        *port_reg |= (1 << pin);
    } else {
        *port_reg &= ~(1 << pin);
    }
}

// Read the state of a pin
uint8_t gpio_pin_read(volatile uint8_t *pin_reg, uint8_t pin) {
    if (*pin_reg & (1 << pin)) {
        return HIGH;
    }
    return LOW;
}

// Toggle a pin state
void gpio_pin_toggle(volatile uint8_t *port_reg, uint8_t pin) {
    *port_reg ^= (1 << pin);
}