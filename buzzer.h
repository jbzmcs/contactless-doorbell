#ifndef BUZZER_H
#define BUZZER_H
#include <avr/io.h> 

// Function Prototypes
void buzzer_init(void);      // Sets up the pin
void buzzer_on(void);        // Turns sound on continuously
void buzzer_off(void);       // Turns sound off
void buzzer_short_beep(void);// A quick "chirp" for UI feedback
void buzzer_alarm(void);     // The "Ding Dong" doorbell pattern

#endif