// buzzer.c - Passive Buzzer Driver (Tone Generator)
#include "buzzer.h"
#include "gpio.h"       
#include <avr/io.h>     
#include <util/delay.h>

// Hardware Mapping: Pin 7 (PD7)
#define BUZZ_DDR  &DDRD
#define BUZZ_PORT &PORTD
#define BUZZ_PIN  PD7 

void buzzer_init(void) {
    gpio_pin_mode(BUZZ_DDR, BUZZ_PIN, OUTPUT);
    buzzer_off();
}

void buzzer_on(void) {
    gpio_pin_write(BUZZ_PORT, BUZZ_PIN, HIGH);
}

void buzzer_off(void) {
    gpio_pin_write(BUZZ_PORT, BUZZ_PIN, LOW);
}

// Helper: Play a tone at ~2kHz (High Pitch)
void play_high_tone(uint16_t duration_ms) {
    // 2kHz = 250us High / 250us Low
    uint32_t cycles = (2000UL * duration_ms) / 1000;
    uint32_t i;
    
    for (i = 0; i < cycles; i++) {
        gpio_pin_write(BUZZ_PORT, BUZZ_PIN, HIGH);
        _delay_us(250);
        gpio_pin_write(BUZZ_PORT, BUZZ_PIN, LOW);
        _delay_us(250);
    }
}

// Helper: Play a tone at ~1kHz (Low Pitch)
void play_low_tone(uint16_t duration_ms) {
    // 1kHz = 500us High / 500us Low
    uint32_t cycles = (1000UL * duration_ms) / 1000;
    uint32_t i;
    
    for (i = 0; i < cycles; i++) {
        gpio_pin_write(BUZZ_PORT, BUZZ_PIN, HIGH);
        _delay_us(500);
        gpio_pin_write(BUZZ_PORT, BUZZ_PIN, LOW);
        _delay_us(500);
    }
}

// A sharp chirp (High pitch)
void buzzer_short_beep(void) {
    play_high_tone(100);
}

// The "Ding Dong" Doorbell Pattern
void buzzer_alarm(void) {
    // "DING" (High Pitch)
    play_high_tone(200);
    
    _delay_ms(50); // Brief silence
    
    // "DONG" (Low Pitch)
    play_low_tone(400);
}