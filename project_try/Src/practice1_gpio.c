#include <stdint.h>
// Address list.
// RCC.
#define RCC_BASE 0x40021000
#define RCC_AHB2ENR *((volatile uint32_t *) (RCC_BASE + 0x4C))
// GPIOA - Used for 7-Segment Display (PA5-PA11).
#define GPIOA_BASE 0x42020000
#define GPIOA_MODER *((volatile uint32_t *) GPIOA_BASE)
#define GPIOA_ODR *((volatile uint32_t *) (GPIOA_BASE + 0x14))
// GPIOC - Used for Switch Input (PC0).
#define GPIOC_BASE 0x42020800
#define GPIOC_MODER *((volatile uint32_t *) GPIOC_BASE)
#define GPIOC_IDR *((volatile uint32_t *) (GPIOC_BASE + 0x10))
// Function for common cathode 7-segment display
void write_to_7s(uint8_t number, int shift) {
static const uint8_t seven_segment[10] = { 0x3FU, 0x06U, 0x5BU, 0x4FU,
0x66U, 0x6DU, 0x7DU, 0x07U, 0x7FU, 0x6FU };
// For common cathode, use the values directly (no inversion needed)
uint8_t value8_t = seven_segment[number];
// value8_t &= ~(1 << 7); You may manually turn on or off the Dot leg.
// Since the value array is an unsigned 8-bit, we need to convert the
// value into 32-bit.
uint32_t value = value8_t;
// Shift the value to put PA5.
value = value << shift;
// Clear the previous value.
GPIOA_ODR &= ~(0b11111111 << shift);
// Write the digit to the pins.
GPIOA_ODR |= value;
}
int practice1_main(void) {
// Enable clocks for GPIOA (Bit 0) and GPIOC (Bit 2) in RCC_AHB2ENR.
RCC_AHB2ENR |= (1 << 0) | (1 << 2);
// Set PA5 to PA12 to General Purpose Output Mode (01).
// Clear mask for bits 10 through 25.
GPIOA_MODER &= ~(0xFFFF << (5 * 2));
// Set to Output (01) for all 8 pins (5 is 0101 in binary).
GPIOA_MODER |= (0x5555 << (5 * 2));
// Set PC0 to Input Mode (00).
// Note: The mode register is 0x00 on reset, but clearing is a good
// practice.
GPIOC_MODER &= ~(0b11 << (0 * 2));
while (1) {
// Read the input value from PC0 (Bit 0 of GPIOC_IDR).
// If the switch is connected to High Voltage (logic 1), the
// result is non-zero.
if ((GPIOC_IDR & (1 << 0)) != 0) {
// Switch is ON (High Voltage/1) -> Display '1'.
write_to_7s(1, 5);
} else {
// Switch is OFF (Ground/0) -> Display '0'.
write_to_7s(0, 5);
}
}
}
