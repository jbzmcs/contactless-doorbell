#include "buzzer.h"
#include "gpio.h"       
#include <avr/io.h>     
#include <util/delay.h>
#define BUZZ_DDR  &DDRD
#define BUZZ_PORT &PORTD
#define BUZZ_PIN  PD7 

void buzzer_init(void) {
    // Set Pin 7 as Output
    gpio_pin_mode(BUZZ_DDR, BUZZ_PIN, OUTPUT);
    // Ensure it starts OFF
    buzzer_off();
}

void buzzer_on(void) {
    gpio_pin_write(BUZZ_PORT, BUZZ_PIN, HIGH);
}

void buzzer_off(void) {
    gpio_pin_write(BUZZ_PORT, BUZZ_PIN, LOW);
}

// A quick 50ms beep (Useful for "Button Pressed" or "Detected" feedback)
void buzzer_short_beep(void) {
    buzzer_on();
    _delay_ms(50);
    buzzer_off();
}

// The main "Doorbell" sound
void buzzer_alarm(void) {
    // "Ding"
    buzzer_on();
    _delay_ms(200);
    buzzer_off();
    
    // Pause
    _delay_ms(100);
    
    // "Dong" (Longer)
    buzzer_on();
    _delay_ms(400);
    buzzer_off();
}