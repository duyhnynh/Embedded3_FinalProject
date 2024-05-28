#include "../include/timer.h"
#include "../include/uart1.h"

volatile unsigned long system_millis = 0;
volatile int countdown = 60;

// Function to get the current time in milliseconds
unsigned long get_time_ms() { return system_millis; }

void init_system_timer() {
  // Set GPIO3 as output
  GPFSEL0 |= (1 << 9);

  // Calculate timer period for 1 second
  unsigned int timer1_period = TIMER_CLOCK_HZ;

  // Load the period into the compare register
  TIMER_C1 = TIMER_CLO + timer1_period;

  // Enable match 1 interrupt
  TIMER_CS |= TIMER_CS_MATCH;
}

void handle_system_timer() {
  // Clear the match 1 interrupt flag
  TIMER_CS |= TIMER_CS_MATCH;

  // Increment the system timer
  TIMER_C1 += TIMER_CLOCK_HZ;

  // Decrement countdown
  if (countdown > 0) {
    countdown--;
    uart_puts("Countdown: ");
    uart_dec(countdown);
    uart_puts("\n");
  } else {
    // uart_puts("Countdown complete!\n");

    // Stop interrupts
    return;
  }

  // Toggle GPIO3
  GPSET0 ^= (1 << 3);
}

void wait_msec_irq(unsigned int n) {
  unsigned long start_time = TIMER_CLO;
  unsigned long end_time = start_time + n * (TIMER_CLOCK_HZ / 1000);

  while (TIMER_CLO < end_time) {
    // Wait until the specified time has elapsed
    uart_puts("Waiting...\n");
  }
}

void set_wait_timer_irq(int set, unsigned int msVal) {
  // Set the timer to expire in msVal milliseconds
  static unsigned long expiredTime = 0;

  // If set is true, set the timer
  if (set) {
    expiredTime = TIMER_CLO + msVal * (TIMER_CLOCK_HZ / 1000);
  } else {
    while (TIMER_CLO < expiredTime) {
      // Wait until the timer has expired
    }
  }
}
