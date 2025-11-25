#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "gpio.h"
#include "sensor.h"

// --- Hardware Mapping ---
// Trigger is on Arduino Pin 9 (PB1)
#define TRIG_DDR  &DDRB
#define TRIG_PORT &PORTB
#define TRIG_PIN  PB1

// Echo is on Arduino Pin 8 (PB0) - MUST match Input Capture Pin
#define ECHO_DDR  &DDRB
#define ECHO_PIN  PB0

// --- Global Variables ---
volatile uint16_t pulse_ticks = 0; // Raw timer ticks
volatile uint8_t measurement_ready = 0;

// --- The Magic: Input Capture Interrupt ---
ISR(TIMER1_CAPT_vect) {
    static uint16_t start_time = 0;
    
    // Check if we are looking for Rising Edge (Start of Echo)
    if (TCCR1B & (1 << ICES1)) {
        // We found the start!
        start_time = ICR1;
        
        // Switch to Falling Edge detection
        TCCR1B &= ~(1 << ICES1);
    } 
    else {
        // We found the end! (Falling Edge)
        pulse_ticks = ICR1 - start_time;
        measurement_ready = 1;
        
        // Switch back to Rising Edge for next time
        TCCR1B |= (1 << ICES1);
    }
}

void sensor_init(void) {
    // 1. Configure GPIOs using our new driver
    gpio_pin_mode(TRIG_DDR, TRIG_PIN, OUTPUT); // Trig Output
    gpio_pin_mode(ECHO_DDR, ECHO_PIN, INPUT);  // Echo Input

    // 2. Initialize Timer1
    TCCR1A = 0; // Normal mode
    TCCR1B = 0;

    // Prescaler = 8 (16MHz / 8 = 2MHz timer). 
    // Each tick is 0.5 microseconds.
    TCCR1B |= (1 << CS11);

    // Enable Input Capture Interrupt
    TIMSK1 |= (1 << ICIE1);

    // Start looking for Rising Edge
    TCCR1B |= (1 << ICES1);

    sei(); // Enable Global Interrupts
}

void sensor_trigger(void) {
    // Send 10us Pulse
    gpio_pin_write(TRIG_PORT, TRIG_PIN, HIGH);
    _delay_us(10);
    gpio_pin_write(TRIG_PORT, TRIG_PIN, LOW);
    
    measurement_ready = 0;
}

// Returns distance in Centimeters
uint16_t sensor_get_distance(void) {
    if (!measurement_ready) return 0; // No new data

    // Math:
    // 1 tick = 0.5us
    // Pulse Width (us) = ticks * 0.5
    // Distance (cm) = Pulse Width (us) / 58
    // Combined: Distance = ticks / 116
    
    return (pulse_ticks / 116);
} 	