#include "../include/fb.h"
#include "../include/uart1.h"
#include "../include/mbox.h"
#include "../include/delay.h"

/* Functions to delay, set/wait timer */

// void wait_msec(unsigned int msVal)
// {
//     register unsigned long f, t, r, expiredTime; // 64 bits

//     // Get the current counter frequency (Hz), 1Hz = 1 pulses/second
//     asm volatile("mrs %0, cntfrq_el0" : "=r"(f));

//     // Read the current counter value
//     asm volatile("mrs %0, cntpct_el0" : "=r"(t));

//     // Calculate expire value for counter
//     /* Note: both expiredTime and counter value t are 64 bits,
//     thus, it will still be correct when the counter is overflow */
//     expiredTime = t + f * msVal / 1000;

//     do
//     {
//         asm volatile("mrs %0, cntpct_el0" : "=r"(r));
//     } while (r < expiredTime);
// }


/**
 * Wait N microsec (ARM CPU only)
 */
void wait_msec(unsigned int n)
{
    register unsigned long f, t, r;
    // get the current counter frequency
    asm volatile ("mrs %0, cntfrq_el0" : "=r"(f));
    // read the current counter
    asm volatile ("mrs %0, cntpct_el0" : "=r"(t));
    // calculate required count increase
    unsigned long i=((f/1000)*n)/1000;
    // loop while counter increase is less than i
    do
    {
        asm volatile ("mrs %0, cntpct_el0" : "=r"(r));
    }while(r-t<i);
}
