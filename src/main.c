#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> // Required for the Sensor Timer Interrupts
#include "gpio.h"
#include "sensor.h"
#include "i2c.h"
#include "lcd.h"
#include "buzzer.h"
// #include "config.h" // Removed as we are not using it
// #include "usart.h" // Removed UART as we are using LCD

// --- System Configuration ---
#define DIST_TRIGGER_CM  10   // Distance to trigger the bell (Hand Wave)
#define DIST_WARN_CM     25   // Distance to show "Wake Up" message

int main(void) {
    // --- 1. System Initialization ---
    // Initialize all hardware drivers
    buzzer_init();   // Setup Pin 7
    i2c_init();      // Setup TWI Hardware (A4/A5)
    lcd_init();      // Setup LCD via I2C
    sensor_init();   // Setup Timer1 & Pins 8/9
    
    sei(); // Enable Global Interrupts

    // HMI: Show Boot Sequence (Visual Feedback)
    lcd_set_cursor(0, 0);
    lcd_print(" SYSTEM BOOT... ");
    buzzer_short_beep(); // Audio Feedback that system is alive
    _delay_ms(1000);
    
    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_print("   READY TO   ");
    lcd_set_cursor(1, 0);
    lcd_print("     SCAN     ");
    _delay_ms(1000);
    lcd_clear();

    // --- 2. Main Loop (Finite State Machine) ---
    while (1) {
        // A. Input Phase: Trigger Sensor
        sensor_trigger();
        
        // Wait 60ms (Ultrasonic sensors need ~60ms silence between pings to prevent echo overlap)
        _delay_ms(60); 
        
        // Get the calculated distance from the Timer Interrupt
        uint16_t dist = sensor_get_distance();

        // B. Logic Phase: HMI State Machine
        
        if (dist > 0 && dist < DIST_TRIGGER_CM) {
            // === STATE: RINGING ===
            // 1. Visual Output
            lcd_clear();
            lcd_set_cursor(0, 0);
            lcd_print(" * DING DONG * ");
            lcd_set_cursor(1, 0);
            lcd_print(" PLEASE WAIT ");
            
            // 2. Audio Output (Uses our buzzer module)
            buzzer_alarm(); 
            
            // 3. Cooldown (Prevent spamming)
            _delay_ms(1500); 
            
            // Reset Screen
            lcd_clear();
        }
        else if (dist > 0 && dist < DIST_WARN_CM) {
            // === STATE: DETECTED (Approaching) ===
            lcd_set_cursor(0, 0);
            lcd_print("  DETECTED!   ");
            lcd_set_cursor(1, 0);
            lcd_print(" Wave to Ring ");
            
            // Optional: Very short blip to acknowledge presence
            // buzzer_short_beep(); 
        }
        else {
            // === STATE: IDLE (Monitoring) ===
            // Only update if necessary to reduce LCD flicker
            lcd_set_cursor(0, 0);
            lcd_print("   SCANNING   ");
            lcd_set_cursor(1, 0);
            lcd_print("              "); // Clear second line
        }
    }
    
    return 0;
}

    /* FOR DEBUGGING LCD: insert in main function */
    /* 
    // 1. Initialization (Only I2C and LCD)
    i2c_init(); 
    lcd_init(); 
    
    // NOTE: Do NOT call sei() here. We are avoiding interrupts for now.

    // 2. Simple Test Output
    lcd_set_cursor(0, 0);
    lcd_print("LCD TEST 1.0");

    lcd_set_cursor(1, 0);
    lcd_print("ADDRESS: 0x27"); // Or 0x27, depending on what you are testing

    // 3. Keep the display running indefinitely
    while(1) {
        // Blink the backlight or a character to confirm the loop is running
        // (You would need a function to turn the backlight on/off for this)
        _delay_ms(1000); 
    }
    
    return 0; // Should never reach here
*/