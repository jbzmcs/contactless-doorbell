#ifndef CONFIG_H
#define CONFIG_H

#define F_CPU 16000000UL
#define BAUD_RATE 9600

// --- Actuator Pins ---
#define BUZZER_DDR  DDRD
#define BUZZER_PORT PORTD
#define BUZZER_PIN  PD3 // Example: Using Pin PD3

// --- Sensor Pins ---
// (We will add these later)

#endif