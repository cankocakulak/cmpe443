# Input Capture/Output Compare

## Overview

Input Capture (IC) and Output Compare (OC) are advanced timer features that allow precise timing measurements and signal generation without CPU intervention. These features use the same timer hardware but in different modes.

## Timer Internal Structure

Every timer module contains:
- **Counter (CNT)** - Main counting register
- **Prescaler (PSC)** - Clock divider
- **Auto-Reload Register (ARR)** - Maximum count value
- **Capture/Compare Registers (CCR1-CCR4)** - Store capture values or compare thresholds
- **Mode Control Registers (CCMR1, CCMR2)** - Configure channel modes
- **Enable Registers (CCER)** - Enable channels and set polarity
- **Interrupt Status (SR)** and Enable (DIER) - Interrupt management

## Output Compare (OC) Mode

Output Compare generates precise timing signals (PWM, toggle, etc.) by comparing the counter value with a preset threshold.

### Applications:
- **PWM Generation** - Motor control, LED dimming
- **Precise Timing** - Generate pulses at specific intervals
- **Waveform Generation** - Square waves, custom signals

### Configuration Steps:

#### 1. Enable Timer Clock
```c
RCC->APB1ENR1 |= (1 << 4);   // TIM6 example (TIM2-7 on APB1)
RCC->APB2ENR |= (1 << 16);   // TIM15 example (TIM15-17 on APB2)
```

#### 2. Basic Timer Configuration
```c
TIM2->PSC = 79;       // Prescaler (80 MHz → 1 MHz)
TIM2->ARR = 999;      // Auto-reload (1000 counts = 1ms period)
```

#### 3. Select Output Compare Mode
```c
// Clear mode bits for Channel 1
TIM2->CCMR1 &= ~(0x7 << 4);   
// Set PWM mode 1 (110 binary)
TIM2->CCMR1 |= (0x6 << 4);    
```

#### 4. Set Compare Value
```c
TIM2->CCR1 = 500;   // 50% duty cycle for PWM
```

#### 5. Enable Channel Output
```c
TIM2->CCER |= 1;     // Channel 1 enable
```

#### 6. Configure GPIO for Alternate Function
```c
// PA0 → TIM2_CH1 example
GPIOA->MODER |= (2 << (0 * 2));  // AF mode
GPIOA->AFRL  |= (1 << (0 * 4));  // AF1 = TIM2_CH1
```

#### 7. Start Timer
```c
TIM2->CR1 |= 1;      // Counter enable
```

### PWM Modes:
- **PWM Mode 1**: Output HIGH when CNT < CCR, LOW when CNT ≥ CCR
- **PWM Mode 2**: Output LOW when CNT < CCR, HIGH when CNT ≥ CCR

## Input Capture (IC) Mode

Input Capture measures the timing of external events by capturing the counter value when a specific edge occurs.

### Applications:
- **Pulse Width Measurement** - Ultrasonic sensors, servo feedback
- **Frequency Measurement** - Signal analysis
- **Period Measurement** - Timing analysis
- **Duty Cycle Measurement** - PWM signal analysis

### Configuration Steps:

#### 1. Enable Timer Clock
```c
RCC->APB1ENR1 |= (1 << 0);   // TIM2 clock enable
```

#### 2. Basic Timer Setup
```c
TIM2->PSC = 79;     // 80 MHz / 80 = 1 MHz (1 µs resolution)
TIM2->ARR = 0xFFFF; // Maximum count for wide range
```

#### 3. Configure Input Capture Mode
```c
// Map Channel 1 to Timer Input 1
TIM2->CCMR1 &= ~(0x3 << 0);  // Clear CC1S bits
TIM2->CCMR1 |= (0x1 << 0);   // CC1S = 01 → CC1 mapped to TI1
```

#### 4. Select Edge Detection
```c
// Rising edge capture
TIM2->CCER &= ~(1 << 1);   
// Falling edge capture
TIM2->CCER |= (1 << 1);    
// Both edges (requires additional configuration)
TIM2->CCER |= (1 << 3) | (1 << 1);
```

#### 5. Enable Capture
```c
TIM2->CCER |= 1;    // CC1E = 1 → capture enabled
```

#### 6. Configure Interrupts (Optional)
```c
TIM2->DIER |= (1 << 1);   // CC1 interrupt enable
NVIC_EnableIRQ(TIM2_IRQn);
```

#### 7. Start Timer
```c
TIM2->CR1 |= 1;   // Counter enable
```

### Reading Capture Values
```c
void TIM2_IRQHandler(void) {
    if (TIM2->SR & (1 << 1)) {  // CC1 interrupt flag
        uint32_t capture_value = TIM2->CCR1;
        TIM2->SR &= ~(1 << 1);  // Clear flag
        // Process capture_value
    }
}
```

## Advanced Techniques

### Pulse Width Measurement
Use two channels or edge switching to measure both rising and falling edges:

```c
static uint8_t state = 0;
static uint16_t pulse_start = 0;
static uint16_t pulse_width = 0;

void TIM15_IRQHandler(void) {
    if (TIM15->SR & (1 << 1)) {  // CC1 interrupt
        if (state == 0) {
            // Rising edge
            state = 1;
            pulse_start = TIM15->CCR1;
            // Switch to falling edge detection
            TIM15->CCER |= (1 << 1);
        } else {
            // Falling edge
            state = 0;
            uint16_t pulse_end = TIM15->CCR1;
            // Calculate pulse width (handle overflow)
            if (pulse_end > pulse_start) {
                pulse_width = pulse_end - pulse_start;
            } else {
                pulse_width = TIM15->ARR - pulse_start + pulse_end;
            }
            // Switch back to rising edge detection
            TIM15->CCER &= ~(1 << 1);
        }
    }
}
```

### Frequency Measurement
Measure time between consecutive rising edges:

```c
static uint16_t last_capture = 0;
static uint32_t frequency = 0;

void TIM2_IRQHandler(void) {
    if (TIM2->SR & (1 << 1)) {
        uint16_t current_capture = TIM2->CCR1;
        uint16_t period = current_capture - last_capture;
        frequency = 1000000 / period;  // Frequency in Hz (1MHz timer)
        last_capture = current_capture;
        TIM2->SR &= ~(1 << 1);
    }
}
```

## Register Details

### CCMR1 Register (Capture/Compare Mode Register 1)
- **CC1S[1:0]** - Channel 1 Selection
  - 00: Output mode
  - 01: Input, IC1 mapped to TI1
  - 10: Input, IC1 mapped to TI2
  - 11: Input, IC1 mapped to TRC
- **OC1M[3:0]** - Output Compare Mode (Output mode only)
  - 000: Frozen
  - 001: Active level on match
  - 010: Inactive level on match
  - 011: Toggle
  - 110: PWM mode 1
  - 111: PWM mode 2

### CCER Register (Capture/Compare Enable Register)
- **CC1E** - Channel 1 Enable
- **CC1P** - Channel 1 Polarity (0: rising edge, 1: falling edge)
- **CC1NP** - Channel 1 Complementary Polarity

### Status Register (SR) Flags
- **UIF** - Update Interrupt Flag (overflow)
- **CC1IF** - Capture/Compare 1 Interrupt Flag
- **CC1OF** - Capture/Compare 1 Overcapture Flag

## Common Applications

### 1. Ultrasonic Distance Sensor (HC-SR04)
```c
// Trigger pulse generation (Output Compare)
// Echo pulse measurement (Input Capture)
uint32_t distance_cm = (pulse_width_us * 34) / 2000;
```

### 2. Servo Motor Control
```c
// 20ms period, 1-2ms pulse width
TIM2->ARR = 19999;  // 20ms at 1MHz
TIM2->CCR1 = 1500;  // 1.5ms = center position
```

### 3. Encoder Reading
```c
// Use two channels for quadrature encoding
// Channel 1: A signal, Channel 2: B signal
```

## Best Practices

1. **Choose appropriate prescaler** for required resolution
2. **Handle counter overflow** in calculations
3. **Use interrupts efficiently** - clear flags promptly
4. **Consider input filtering** for noisy signals
5. **Validate capture values** - check for reasonable ranges
6. **Use DMA** for high-frequency captures to reduce CPU load

## Troubleshooting

### Common Issues:
- **No capture events**: Check GPIO alternate function configuration
- **Wrong timing values**: Verify prescaler and clock source
- **Missing interrupts**: Ensure NVIC is enabled and flags are cleared
- **Overflow handling**: Account for counter wraparound in calculations

---
*Next: [Advanced Timer Features](12_Advanced_Timer_Features.md)*
*Previous: [Interrupts](10_Interrupts.md)*
