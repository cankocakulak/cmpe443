/**
 ******************************************************************************
 * @file           : simple_experiment.c
 * @author         : Your experiments
 * @brief          : Simple experiment functions
 ******************************************************************************
 */

#include <stdint.h>

// Simple delay function
void simple_delay(uint32_t count) {
    for (uint32_t i = 0; i < count * 333; i++) {
        // Just wait
    }
}

// Simple experiment: LEDs in sequence
void led_sequence(void) {
    // Turn on the GPIO ports
    *((uint32_t *) (0x40021000 + 0x4C)) |= 0x07;
    
    // Configure LEDs as outputs
    *((uint32_t *) (0x42020000 + 0x00)) &= ~(0x03 << (9 * 2));
    *((uint32_t *) (0x42020000 + 0x00)) |= (0x01 << (9 * 2));
    *((uint32_t *) (0x42020400 + 0x00)) &= ~(0x03 << (7 * 2));
    *((uint32_t *) (0x42020400 + 0x00)) |= (0x01 << (7 * 2));
    *((uint32_t *) (0x42020800 + 0x00)) &= ~(0x03 << (7 * 2));
    *((uint32_t *) (0x42020800 + 0x00)) |= (0x01 << (7 * 2));
    
    while(1) {
        // Red LED on
        *((uint32_t *) (0x42020000 + 0x14)) |= (0x01 << 9);
        simple_delay(500);
        *((uint32_t *) (0x42020000 + 0x14)) &= ~(0x01 << 9);
        
        // Blue LED on
        *((uint32_t *) (0x42020400 + 0x14)) |= (0x01 << 7);
        simple_delay(500);
        *((uint32_t *) (0x42020400 + 0x14)) &= ~(0x01 << 7);
        
        // Yellow LED on
        *((uint32_t *) (0x42020800 + 0x14)) |= (0x01 << 7);
        simple_delay(500);
        *((uint32_t *) (0x42020800 + 0x14)) &= ~(0x01 << 7);
        
        simple_delay(500); // Pause between cycles
    }
}

// Original blinking experiment: All LEDs blink together
void led_blink_original(void) {
    // Turn on the GPIO ports.
    *((uint32_t *) (0x40021000 + 0x4C)) |= 0x07;
    // To access the Red Light.
    *((uint32_t *) (0x42020000 + 0x00)) &= ~(0x03 << (9 * 2));
    *((uint32_t *) (0x42020000 + 0x00)) |= (0x01 << (9 * 2));
    // To access the Blue Light.
    *((uint32_t *) (0x42020400 + 0x00)) &= ~(0x03 << (7 * 2));
    *((uint32_t *) (0x42020400 + 0x00)) |= (0x01 << (7 * 2));
    // To access the Yellow Light.
    *((uint32_t *) (0x42020800 + 0x00)) &= ~(0x03 << (7 * 2));
    *((uint32_t *) (0x42020800 + 0x00)) |= (0x01 << (7 * 2));
    
    while(1) {
        // Turn on the Red Light.
        *((uint32_t *) (0x42020000 + 0x14)) |= (0x01 << 9);
        // Turn on the Blue Light.
        *((uint32_t *) (0x42020400 + 0x14)) |= (0x01 << 7);
        // Turn on the Yellow Light.
        *((uint32_t *) (0x42020800 + 0x14)) |= (0x01 << 7);
        // Useless loop to make time for the human eye.
        simple_delay(1000);
        
        // Turn off the Red Light.
        *((uint32_t *) (0x42020000 + 0x14)) &= ~(0x01 << 9);
        // Turn off the Blue Light.
        *((uint32_t *) (0x42020400 + 0x14)) &= ~(0x01 << 7);
        // Turn off the Yellow Light.
        *((uint32_t *) (0x42020800 + 0x14)) &= ~(0x01 << 7);
        // Useless loop to make time for the human eye.
        simple_delay(1000);
    }
}

// Another simple experiment: Alternating LEDs
void led_alternate(void) {
    // Turn on the GPIO ports
    *((uint32_t *) (0x40021000 + 0x4C)) |= 0x07;
    
    // Configure LEDs as outputs
    *((uint32_t *) (0x42020000 + 0x00)) &= ~(0x03 << (9 * 2));
    *((uint32_t *) (0x42020000 + 0x00)) |= (0x01 << (9 * 2));
    *((uint32_t *) (0x42020400 + 0x00)) &= ~(0x03 << (7 * 2));
    *((uint32_t *) (0x42020400 + 0x00)) |= (0x01 << (7 * 2));
    *((uint32_t *) (0x42020800 + 0x00)) &= ~(0x03 << (7 * 2));
    *((uint32_t *) (0x42020800 + 0x00)) |= (0x01 << (7 * 2));
    
    while(1) {
        // Red and Yellow on, Blue off
        *((uint32_t *) (0x42020000 + 0x14)) |= (0x01 << 9);
        *((uint32_t *) (0x42020800 + 0x14)) |= (0x01 << 7);
        *((uint32_t *) (0x42020400 + 0x14)) &= ~(0x01 << 7);
        simple_delay(1000);
        
        // Blue on, Red and Yellow off
        *((uint32_t *) (0x42020400 + 0x14)) |= (0x01 << 7);
        *((uint32_t *) (0x42020000 + 0x14)) &= ~(0x01 << 9);
        *((uint32_t *) (0x42020800 + 0x14)) &= ~(0x01 << 7);
        simple_delay(1000);
    }
}
