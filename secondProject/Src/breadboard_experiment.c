/**
 ******************************************************************************
 * @file           : breadboard_experiment.c
 * @author         : Your breadboard experiments
 * @brief          : Simple LED experiments on breadboard
 ******************************************************************************
 * 
 * SIMPLE LED CONNECTION:
 * - LED (+) wire → 1kΩ resistor → A0 connector (PA0)
 * - LED (-) wire → GND (Ground pin on board)
 * 
 * NOTE: PA0 is configured as digital output (overriding analog function)
 * 
 ******************************************************************************
 */

#include <stdint.h>

// Use PA0 (A0 connector) - the pin you found
#define LED_PIN         0   // PA0

// GPIO base address for Port A
#define GPIOA_BASE      0x42020000

// Simple delay function
void breadboard_delay(uint32_t count) {
    for (uint32_t i = 0; i < count * 333; i++) {
        // Just wait
    }
}

// Initialize PA0 with proper configuration for A0 connector
void led_init(void) {
    // Enable GPIOA clock
    *((uint32_t *) (0x40021000 + 0x4C)) |= 0x01;  // Enable GPIOA
    
    // Configure PA0 as digital output (override analog function)
    *((uint32_t *) (GPIOA_BASE + 0x00)) &= ~(0x03 << (LED_PIN * 2));
    *((uint32_t *) (GPIOA_BASE + 0x00)) |= (0x01 << (LED_PIN * 2));
    
    // Set as push-pull output (strong drive)
    *((uint32_t *) (GPIOA_BASE + 0x04)) &= ~(0x01 << LED_PIN);
    
    // Set speed to medium
    *((uint32_t *) (GPIOA_BASE + 0x08)) &= ~(0x03 << (LED_PIN * 2));
    *((uint32_t *) (GPIOA_BASE + 0x08)) |= (0x01 << (LED_PIN * 2));
    
    // No pull-up/pull-down
    *((uint32_t *) (GPIOA_BASE + 0x0C)) &= ~(0x03 << (LED_PIN * 2));
}

// Turn LED on
void led_on(void) {
    *((uint32_t *) (GPIOA_BASE + 0x14)) |= (0x01 << LED_PIN);
}

// Turn LED off
void led_off(void) {
    *((uint32_t *) (GPIOA_BASE + 0x14)) &= ~(0x01 << LED_PIN);
}

// Experiment 1: Simple blink
void simple_blink(void) {
    led_init();
    
    while(1) {
        led_on();
        breadboard_delay(3000);  // Much longer ON time
        led_off();
        breadboard_delay(3000);  // Much longer OFF time
    }
}

// Test: LED always on (for troubleshooting)
void led_always_on(void) {
    led_init();
    led_on();
    
    while(1) {
        // LED stays on - good for testing connections
    }
}

// Test: Very slow blink with strong drive
void super_slow_blink(void) {
    led_init();
    
    while(1) {
        // Turn on and hold for a long time
        led_on();
        for(int i = 0; i < 10; i++) {
            breadboard_delay(1000);  // 10 seconds ON
        }
        
        // Turn off and hold for a long time  
        led_off();
        for(int i = 0; i < 10; i++) {
            breadboard_delay(1000);  // 10 seconds OFF
        }
    }
}


// Experiment 2: Fast blink
void fast_blink(void) {
    led_init();
    
    while(1) {
        led_on();
        breadboard_delay(200);
        led_off();
        breadboard_delay(200);
    }
}

// Experiment 3: SOS pattern
void sos_pattern(void) {
    led_init();
    
    while(1) {
        // S (3 short)
        for(int i = 0; i < 3; i++) {
            led_on();
            breadboard_delay(200);
            led_off();
            breadboard_delay(200);
        }
        
        breadboard_delay(400); // Gap between letters
        
        // O (3 long)
        for(int i = 0; i < 3; i++) {
            led_on();
            breadboard_delay(600);
            led_off();
            breadboard_delay(200);
        }
        
        breadboard_delay(400); // Gap between letters
        
        // S (3 short)
        for(int i = 0; i < 3; i++) {
            led_on();
            breadboard_delay(200);
            led_off();
            breadboard_delay(200);
        }
        
        breadboard_delay(2000); // Long pause before repeat
    }
}
