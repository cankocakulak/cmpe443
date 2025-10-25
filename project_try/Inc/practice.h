#ifndef PRACTICE_H
#define PRACTICE_H

#include <stdint.h>

/* =========================
   Practice File Declarations
   ========================= */

// Practice 1: GPIO Control (7-Segment Display)
int practice1_main(void);
void write_to_7s(uint8_t number, int shift);

// Practice 2: Timer Control  
int practice2_main(void);
void practice2_init(void);
void practice2_run(void);

// Practice 3: ADC Reading
int practice3_main(void);
void practice3_init(void);
void practice3_run(void);

#endif /* PRACTICE_H */
