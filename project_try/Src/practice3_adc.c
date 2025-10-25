#include <stdint.h>
#include <stdio.h>

/* =========================
   Practice 3: ADC Reading
   ========================= */

void practice3_init(void)
{
    printf("=== Practice 3: ADC Reading ===\n");
    printf("Initializing ADC...\n");
    // TODO: Add actual ADC initialization code here
}

void practice3_run(void)
{
    printf("Running ADC practice exercises...\n");
    
    // Example ADC operations
    for (int i = 0; i < 8; i++)
    {
        // Simulate ADC reading
        uint16_t adc_value = 100 + (i * 50) + (i * i * 10);
        printf("ADC Reading %d: %u (simulated)\n", i + 1, adc_value);
        
        // TODO: Add actual ADC reading code here
        
        // Simulate sampling delay
        for (volatile int delay = 0; delay < 150000; delay++);
    }
    
    printf("ADC practice completed!\n");
}

int practice3_main(void)
{
    practice3_init();
    practice3_run();
    
    // Keep running
    while(1)
    {
        // Main loop for practice 3
        for (volatile int delay = 0; delay < 2000000; delay++);
        printf("Practice 3 ADC monitoring...\n");
    }
    
    return 0;
}
