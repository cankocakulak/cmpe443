#include <stdint.h>
#include <stdio.h>

/* =========================
   Practice 2: Timer Control
   ========================= */

void practice2_init(void)
{
    printf("=== Practice 2: Timer Control ===\n");
    printf("Initializing timers...\n");
    // TODO: Add actual timer initialization code here
}

void practice2_run(void)
{
    printf("Running timer practice exercises...\n");
    
    // Example timer operations
    for (int i = 0; i < 5; i++)
    {
        printf("Timer cycle %d - measuring time intervals\n", i + 1);
        // TODO: Add actual timer measurement code here
        
        // Simulate timer delay
        for (volatile int delay = 0; delay < 200000; delay++);
    }
    
    printf("Timer practice completed!\n");
}

int practice2_main(void)
{
    practice2_init();
    practice2_run();
    
    // Keep running
    while(1)
    {
        // Main loop for practice 2
        for (volatile int delay = 0; delay < 1500000; delay++);
        printf("Practice 2 timer tick...\n");
    }
    
    return 0;
}
