#include "fb.h"
#include "uart1.h"
#include "mbox.h"

/* Functions to delay, set/wait timer */

void wait_msec(unsigned int msVal)
{
    register unsigned long f, t, r, expiredTime; // 64 bits

    // Get the current counter frequency (Hz), 1Hz = 1 pulses/second
    asm volatile("mrs %0, cntfrq_el0" : "=r"(f));

    // Read the current counter value
    asm volatile("mrs %0, cntpct_el0" : "=r"(t));

    // Calculate expire value for counter
    /* Note: both expiredTime and counter value t are 64 bits,
    thus, it will still be correct when the counter is overflow */
    expiredTime = t + f * msVal / 1000;

    do
    {
        asm volatile("mrs %0, cntpct_el0" : "=r"(r));
    } while (r < expiredTime);
}