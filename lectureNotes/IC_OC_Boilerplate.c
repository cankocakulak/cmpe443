/**
 * @file IC_OC_Boilerplate.c
 * @brief Comprehensive Input Capture / Output Compare Boilerplate
 * @author CMPE 443 Course
 * @date 2025
 * 
 * This file provides a complete template for Input Capture and Output Compare
 * operations using STM32L552 timers. Includes pulse width measurement,
 * PWM generation, and frequency measurement examples.
 */

#include <stdint.h>

// ============================================================================
// HARDWARE REGISTER STRUCTURES
// ============================================================================

typedef struct {
    volatile uint32_t CR1;      // 0x00 - Control Register 1
    volatile uint32_t CR2;      // 0x04 - Control Register 2
    volatile uint32_t SMCR;     // 0x08 - Slave Mode Control Register
    volatile uint32_t DIER;     // 0x0C - DMA/Interrupt Enable Register
    volatile uint32_t SR;       // 0x10 - Status Register
    volatile uint32_t EGR;      // 0x14 - Event Generation Register
    volatile uint32_t CCMR1;    // 0x18 - Capture/Compare Mode Register 1
    uint32_t reserved1;         // 0x1C - Reserved
    volatile uint32_t CCER;     // 0x20 - Capture/Compare Enable Register
    volatile uint32_t CNT;      // 0x24 - Counter Register
    volatile uint32_t PSC;      // 0x28 - Prescaler Register
    volatile uint32_t ARR;      // 0x2C - Auto-Reload Register
    volatile uint32_t RCR;      // 0x30 - Repetition Counter Register
    volatile uint32_t CCR1;     // 0x34 - Capture/Compare Register 1
    volatile uint32_t CCR2;     // 0x38 - Capture/Compare Register 2
    uint32_t reserved2[2];      // 0x3C-0x40 - Reserved
    volatile uint32_t BDTR;     // 0x44 - Break and Dead-Time Register
    volatile uint32_t DCR;      // 0x48 - DMA Control Register
    volatile uint32_t DMAR;     // 0x4C - DMA Address for Burst Mode
    volatile uint32_t OR1;      // 0x50 - Option Register 1
    uint32_t reserved3[3];      // 0x54-0x5C - Reserved
    volatile uint32_t OR2;      // 0x60 - Option Register 2
} TIM15_General_Purpose_Type;

typedef struct {
    volatile uint32_t CR1;      // 0x00 - Control Register 1
    volatile uint32_t CR2;      // 0x04 - Control Register 2
    uint32_t reserved1;         // 0x08 - Reserved
    volatile uint32_t DIER;     // 0x0C - DMA/Interrupt Enable Register
    volatile uint32_t SR;       // 0x10 - Status Register
    volatile uint32_t EGR;      // 0x14 - Event Generation Register
    volatile uint32_t CCMR1;    // 0x18 - Capture/Compare Mode Register 1
    uint32_t reserved2;         // 0x1C - Reserved
    volatile uint32_t CCER;     // 0x20 - Capture/Compare Enable Register
    volatile uint32_t CNT;      // 0x24 - Counter Register
    volatile uint32_t PSC;      // 0x28 - Prescaler Register
    volatile uint32_t ARR;      // 0x2C - Auto-Reload Register
    volatile uint32_t RCR;      // 0x30 - Repetition Counter Register
    volatile uint32_t CCR1;     // 0x34 - Capture/Compare Register 1
    uint32_t reserved3[3];      // 0x38-0x40 - Reserved
    volatile uint32_t BDTR;     // 0x44 - Break and Dead-Time Register
    volatile uint32_t DCR;      // 0x48 - DMA Control Register
    volatile uint32_t DMAR;     // 0x4C - DMA Address for Burst Mode
    volatile uint32_t OR1;      // 0x50 - Option Register 1
    uint32_t reserved4[3];      // 0x54-0x5C - Reserved
    volatile uint32_t OR2;      // 0x60 - Option Register 2
} TIM16_17_General_Purpose_Type;

typedef struct {
    volatile uint32_t MODER;    // Mode Register
    volatile uint32_t OTYPER;   // Output Type Register
    volatile uint32_t OSPEEDR;  // Output Speed Register
    volatile uint32_t PUPDR;    // Pull-up/Pull-down Register
    volatile uint32_t IDR;      // Input Data Register
    volatile uint32_t ODR;      // Output Data Register
    volatile uint32_t BSRR;     // Bit Set/Reset Register
    volatile uint32_t LCKR;     // Lock Register
    volatile uint32_t AFRL;     // Alternate Function Low Register
    volatile uint32_t AFRH;     // Alternate Function High Register
    volatile uint32_t BRR;      // Bit Reset Register
    uint32_t reserved;          // Reserved
    volatile uint32_t SECCFGR;  // Security Configuration Register
} GPIO;

// ============================================================================
// MEMORY MAP DEFINITIONS
// ============================================================================

// RCC (Reset and Clock Control) Registers
#define RCC_AHB2ENR     *((volatile uint32_t *) 0x4002104C)
#define RCC_APB1ENR1    *((volatile uint32_t *) 0x40021058)
#define RCC_APB2ENR     *((volatile uint32_t *) 0x40021060)

// NVIC (Nested Vectored Interrupt Controller)
#define ISER2           *((volatile uint32_t *) 0xE000E108)

// GPIO Port Base Addresses
#define GPIOA           ((GPIO *) 0x42020000)
#define GPIOB           ((GPIO *) 0x42020400)
#define GPIOC           ((GPIO *) 0x42020800)
#define GPIOD           ((GPIO *) 0x42020C00)
#define GPIOE           ((GPIO *) 0x42021000)

// Timer Base Addresses
#define TIM15           ((TIM15_General_Purpose_Type *) 0x40014000)
#define TIM16           ((TIM16_17_General_Purpose_Type *) 0x40014400)
#define TIM17           ((TIM16_17_General_Purpose_Type *) 0x40014800)

// ============================================================================
// CONFIGURATION STRUCTURES
// ============================================================================

typedef struct {
    uint32_t prescaler;         // Timer prescaler value
    uint32_t period;            // Auto-reload value (ARR)
    uint32_t clock_division;    // Clock division factor
    uint32_t counter_mode;      // Up/Down counting mode
} Timer_Config_t;

typedef struct {
    uint32_t channel;           // Timer channel (1-4)
    uint32_t mode;              // IC/OC mode
    uint32_t polarity;          // Active high/low
    uint32_t selection;         // Input selection for IC
    uint32_t prescaler;         // Input capture prescaler
    uint32_t filter;            // Input filter
} Channel_Config_t;

typedef struct {
    uint32_t pulse;             // Compare value for OC
    uint32_t oc_mode;           // Output compare mode
    uint32_t output_state;      // Output enable/disable
    uint32_t output_n_state;    // Complementary output state
    uint32_t oc_polarity;       // Output polarity
    uint32_t oc_n_polarity;     // Complementary output polarity
} OC_Config_t;

// ============================================================================
// GLOBAL VARIABLES
// ============================================================================

// Input Capture State Variables
static volatile uint8_t capture_state = 0;
static volatile uint16_t pulse_start = 0;
static volatile uint16_t pulse_end = 0;
static volatile uint32_t pulse_width = 0;
static volatile uint32_t frequency = 0;
static volatile uint16_t last_capture = 0;

// Error and Status Flags
static volatile uint8_t overflow_flag = 0;
static volatile uint8_t overcapture_flag = 0;

// ============================================================================
// LED CONTROL FUNCTIONS (Debug/Feedback)
// ============================================================================

void turn_RED_on(void) {
    GPIOA->ODR |= (1 << 9);
}

void turn_RED_off(void) {
    GPIOA->ODR &= ~(1 << 9);
}

void turn_GREEN_on(void) {
    GPIOC->ODR |= (1 << 7);
}

void turn_GREEN_off(void) {
    GPIOC->ODR &= ~(1 << 7);
}

void turn_BLUE_on(void) {
    GPIOB->ODR |= (1 << 7);
}

void turn_BLUE_off(void) {
    GPIOB->ODR &= ~(1 << 7);
}

void toggle_LED(GPIO* port, uint32_t pin) {
    port->ODR ^= (1 << pin);
}

// ============================================================================
// GPIO INITIALIZATION FUNCTIONS
// ============================================================================

void init_GPIO_basic(void) {
    // Enable GPIO clocks
    RCC_AHB2ENR |= 0b111;  // Enable GPIOA, GPIOB, GPIOC
    
    // Configure User LEDs as outputs
    // PA9 - RED LED
    GPIOA->MODER &= ~(0b11 << (9 * 2));
    GPIOA->MODER |= (0b01 << (9 * 2));
    
    // PC7 - GREEN LED
    GPIOC->MODER &= ~(0b11 << (7 * 2));
    GPIOC->MODER |= (0b01 << (7 * 2));
    
    // PB7 - BLUE LED
    GPIOB->MODER &= ~(0b11 << (7 * 2));
    GPIOB->MODER |= (0b01 << (7 * 2));
}

void init_GPIO_timer_pins(void) {
    // Configure PA2 and PA3 for TIM15 CH1 and CH2
    // Set to Alternate Function mode
    GPIOA->MODER &= ~(0b1111 << (2 * 2));  // Clear PA2, PA3 mode bits
    GPIOA->MODER |= (0b1010 << (2 * 2));   // Set AF mode for PA2, PA3
    
    // Select AF14 for TIM15 channels
    GPIOA->AFRL &= ~(0b11111111 << (2 * 4));  // Clear AF bits for PA2, PA3
    GPIOA->AFRL |= (0b11101110 << (2 * 4));   // Set AF14 for both pins
    
    // Configure pin characteristics
    GPIOA->OSPEEDR |= (0b1111 << (2 * 2));    // High speed
    GPIOA->PUPDR &= ~(0b1111 << (2 * 2));     // No pull-up/pull-down
}

void init_GPIO_complete(void) {
    init_GPIO_basic();
    init_GPIO_timer_pins();
}

// ============================================================================
// TIMER CONFIGURATION FUNCTIONS
// ============================================================================

void init_timer_basic(TIM15_General_Purpose_Type* timer, Timer_Config_t* config) {
    // Enable timer clock (assuming TIM15 on APB2)
    RCC_APB2ENR |= (1 << 16);  // TIM15 clock enable
    
    // Configure basic timer parameters
    timer->PSC = config->prescaler;
    timer->ARR = config->period;
    timer->CR1 &= ~(1 << 4);  // Up counting mode
    
    // Clear counter
    timer->CNT = 0;
}

void configure_output_compare(TIM15_General_Purpose_Type* timer, uint32_t channel, OC_Config_t* config) {
    // Enable all outputs (required for advanced timers)
    timer->BDTR |= (1 << 15);  // MOE - Main Output Enable
    
    if (channel == 1) {
        // Configure Channel 1 for Output Compare
        timer->CCMR1 &= ~(0b11 << 0);     // CC1S = 00 (output mode)
        timer->CCMR1 &= ~(0b111 << 4);    // Clear OC1M bits
        timer->CCMR1 |= (config->oc_mode << 4);  // Set output compare mode
        
        // Set compare value
        timer->CCR1 = config->pulse;
        
        // Configure output polarity and enable
        if (config->oc_polarity) {
            timer->CCER |= (1 << 1);      // CC1P = 1 (active low)
        } else {
            timer->CCER &= ~(1 << 1);     // CC1P = 0 (active high)
        }
        
        timer->CCER |= (1 << 0);          // CC1E = 1 (enable output)
    }
    
    if (channel == 2) {
        // Configure Channel 2 for Output Compare
        timer->CCMR1 &= ~(0b11 << 8);     // CC2S = 00 (output mode)
        timer->CCMR1 &= ~(0b111 << 12);   // Clear OC2M bits
        timer->CCMR1 |= (config->oc_mode << 12);  // Set output compare mode
        
        // Set compare value
        timer->CCR2 = config->pulse;
        
        // Configure output polarity and enable
        if (config->oc_polarity) {
            timer->CCER |= (1 << 5);      // CC2P = 1 (active low)
        } else {
            timer->CCER &= ~(1 << 5);     // CC2P = 0 (active high)
        }
        
        timer->CCER |= (1 << 4);          // CC2E = 1 (enable output)
    }
}

void configure_input_capture(TIM15_General_Purpose_Type* timer, uint32_t channel, Channel_Config_t* config) {
    if (channel == 1) {
        // Configure Channel 1 for Input Capture
        timer->CCMR1 &= ~(0b11 << 0);     // Clear CC1S bits
        timer->CCMR1 |= (config->selection << 0);  // Set input selection
        
        // Configure input capture filter and prescaler
        timer->CCMR1 &= ~(0b1111 << 4);   // Clear IC1F (filter)
        timer->CCMR1 |= (config->filter << 4);
        
        timer->CCMR1 &= ~(0b11 << 2);     // Clear IC1PSC (prescaler)
        timer->CCMR1 |= (config->prescaler << 2);
        
        // Configure capture polarity
        if (config->polarity & 0x01) {
            timer->CCER |= (1 << 1);      // CC1P = 1 (falling edge)
        } else {
            timer->CCER &= ~(1 << 1);     // CC1P = 0 (rising edge)
        }
        
        if (config->polarity & 0x02) {
            timer->CCER |= (1 << 3);      // CC1NP = 1 (both edges)
        }
        
        timer->CCER |= (1 << 0);          // CC1E = 1 (enable capture)
    }
}

// ============================================================================
// INTERRUPT CONFIGURATION
// ============================================================================

void configure_timer_interrupts(TIM15_General_Purpose_Type* timer, uint32_t interrupt_mask) {
    // Enable specified interrupts
    timer->DIER |= interrupt_mask;
    
    // Clear all status flags
    timer->SR = 0;
    
    // Enable TIM15 interrupt in NVIC
    ISER2 |= (1 << 5);  // TIM15 interrupt enable
}

// ============================================================================
// INTERRUPT SERVICE ROUTINES
// ============================================================================

void TIM15_IRQHandler(void) {
    // Input Capture Interrupt (Channel 1)
    if ((TIM15->SR & (1 << 1)) != 0) {
        if (capture_state == 0) {
            // Rising edge detected
            capture_state = 1;
            pulse_start = TIM15->CCR1;
            turn_BLUE_on();  // Visual feedback
            
            // Switch to falling edge detection for next capture
            TIM15->CCER |= (1 << 1);  // Set CC1P for falling edge
        } else {
            // Falling edge detected
            capture_state = 0;
            pulse_end = TIM15->CCR1;
            
            // Calculate pulse width (handle overflow)
            if (pulse_end >= pulse_start) {
                pulse_width = pulse_end - pulse_start;
            } else {
                pulse_width = (TIM15->ARR + 1) - pulse_start + pulse_end;
            }
            
            // Store result in CCR2 for output compare
            TIM15->CCR2 = pulse_width;
            
            turn_BLUE_off();  // Visual feedback
            
            // Switch back to rising edge detection
            TIM15->CCER &= ~(1 << 1);  // Clear CC1P for rising edge
        }
        
        // Reading CCR1 automatically clears the flag
        // TIM15->SR &= ~(1 << 1);  // Not needed when reading CCR1
    }
    
    // Timer Overflow Interrupt
    if ((TIM15->SR & (1 << 0)) != 0) {
        TIM15->SR &= ~(1 << 0);  // Clear UIF flag
        overflow_flag = 1;
        turn_RED_on();  // Visual feedback for overflow
    }
    
    // Output Compare Interrupt (Channel 2)
    if ((TIM15->SR & (1 << 2)) != 0) {
        TIM15->SR &= ~(1 << 2);  // Clear CC2IF flag
        turn_RED_off();  // Turn off overflow indicator
    }
    
    // Overcapture Flag Check
    if ((TIM15->SR & (1 << 9)) != 0) {
        TIM15->SR &= ~(1 << 9);  // Clear CC1OF flag
        overcapture_flag = 1;
        turn_GREEN_on();  // Visual feedback for overcapture
    }
}

// ============================================================================
// HIGH-LEVEL INITIALIZATION FUNCTIONS
// ============================================================================

void init_pulse_width_measurement(void) {
    // Timer configuration for pulse width measurement
    Timer_Config_t timer_config = {
        .prescaler = 3999,      // 4 MHz / 4000 = 1 kHz
        .period = 3999,         // 4 seconds maximum
        .clock_division = 0,
        .counter_mode = 0       // Up counting
    };
    
    // Input capture configuration
    Channel_Config_t ic_config = {
        .channel = 1,
        .mode = 0,              // Input capture mode
        .polarity = 0,          // Start with rising edge
        .selection = 1,         // TI1 input
        .prescaler = 0,         // No prescaler
        .filter = 0             // No filter
    };
    
    // Output compare configuration (for visual feedback)
    OC_Config_t oc_config = {
        .pulse = 999,           // 1 second pulse
        .oc_mode = 6,           // PWM mode 1
        .output_state = 1,      // Enable output
        .oc_polarity = 1        // Active low
    };
    
    // Initialize components
    init_GPIO_complete();
    init_timer_basic(TIM15, &timer_config);
    configure_input_capture(TIM15, 1, &ic_config);
    configure_output_compare(TIM15, 2, &oc_config);
    
    // Enable interrupts
    configure_timer_interrupts(TIM15, 0b111);  // CC1, CC2, and Update interrupts
    
    // Start timer
    TIM15->CR1 |= 1;  // Counter enable
}

void init_frequency_measurement(void) {
    // Similar to pulse width but optimized for frequency measurement
    Timer_Config_t timer_config = {
        .prescaler = 79,        // 1 MHz timer frequency
        .period = 0xFFFF,       // Maximum period for wide range
        .clock_division = 0,
        .counter_mode = 0
    };
    
    Channel_Config_t ic_config = {
        .channel = 1,
        .mode = 0,
        .polarity = 0,          // Rising edge only
        .selection = 1,
        .prescaler = 0,
        .filter = 0
    };
    
    init_GPIO_complete();
    init_timer_basic(TIM15, &timer_config);
    configure_input_capture(TIM15, 1, &ic_config);
    configure_timer_interrupts(TIM15, (1 << 1));  // CC1 interrupt only
    
    TIM15->CR1 |= 1;
}

void init_pwm_generation(uint32_t frequency_hz, uint32_t duty_percent) {
    // Calculate timer parameters for desired frequency
    uint32_t timer_freq = 4000000;  // 4 MHz after prescaler
    uint32_t period = (timer_freq / frequency_hz) - 1;
    uint32_t pulse = (period * duty_percent) / 100;
    
    Timer_Config_t timer_config = {
        .prescaler = 19,        // 80 MHz / 20 = 4 MHz
        .period = period,
        .clock_division = 0,
        .counter_mode = 0
    };
    
    OC_Config_t oc_config = {
        .pulse = pulse,
        .oc_mode = 6,           // PWM mode 1
        .output_state = 1,
        .oc_polarity = 0        // Active high
    };
    
    init_GPIO_complete();
    init_timer_basic(TIM15, &timer_config);
    configure_output_compare(TIM15, 1, &oc_config);
    
    TIM15->CR1 |= 1;
}

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

uint32_t get_pulse_width_us(void) {
    return pulse_width;  // Returns pulse width in timer ticks
}

uint32_t get_frequency_hz(void) {
    return frequency;
}

void set_pwm_duty_cycle(uint32_t duty_percent) {
    uint32_t pulse = (TIM15->ARR * duty_percent) / 100;
    TIM15->CCR1 = pulse;
}

void clear_error_flags(void) {
    overflow_flag = 0;
    overcapture_flag = 0;
    turn_RED_off();
    turn_GREEN_off();
}

// ============================================================================
// MAIN FUNCTION EXAMPLES
// ============================================================================

int main_pulse_width_measurement(void) {
    // Initialize for pulse width measurement
    init_pulse_width_measurement();
    
    // Enable global interrupts
    __asm volatile(
        "mov r0, #0 \n\t"
        "msr primask, r0 \n\t"
    );
    
    while(1) {
        // Sleep until interrupt
        __asm volatile("wfi");
        
        // Process results when available
        if (pulse_width > 0) {
            // Convert to microseconds or other units as needed
            // pulse_width_us = pulse_width * timer_period_us;
        }
    }
    
    return 0;
}

int main_pwm_generation(void) {
    // Initialize PWM: 1kHz, 50% duty cycle
    init_pwm_generation(1000, 50);
    
    uint32_t duty = 0;
    uint32_t direction = 1;
    
    while(1) {
        // Create breathing LED effect
        set_pwm_duty_cycle(duty);
        
        if (direction) {
            duty += 5;
            if (duty >= 100) direction = 0;
        } else {
            duty -= 5;
            if (duty == 0) direction = 1;
        }
        
        // Simple delay
        for(volatile int i = 0; i < 100000; i++);
    }
    
    return 0;
}

int main_combined_example(void) {
    // This is the main function from your example
    init_GPIO_complete();
    init_pulse_width_measurement();
    
    __asm volatile(
        "mov r0, #0 \n\t"
        "msr primask, r0 \n\t"
    );
    
    while(1) {
        __asm volatile("wfi");
    }
    
    return 0;
}

// Use this as your main function
int main(void) {
    return main_combined_example();
}
