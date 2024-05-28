#ifndef TIMER_H
#define TIMER_H

#include "../include/gpio.h"

// Base MMIO address
// #define MMIO_BASE 0x3F000000

// System timer base address
#define TIMER_BASE (MMIO_BASE + 0x3000)

// System timer registers
#define TIMER_CS (*(volatile unsigned int *)(TIMER_BASE + 0x00))
#define TIMER_CLO (*(volatile unsigned int *)(TIMER_BASE + 0x04))
#define TIMER_CHI (*(volatile unsigned int *)(TIMER_BASE + 0x08))
#define TIMER_C1 (*(volatile unsigned int *)(TIMER_BASE + 0x10))

// Timer control bits
#define TIMER_CS_MATCH (1 << 1)
#define TIMER_CLOCK_HZ 1000000

// Timer initialization
void init_system_timer();

// Timer IRQ handlers
void handle_system_timer();

// Timer functions
void wait_msec_irq(unsigned int n);
void set_wait_timer_irq(int set, unsigned int msVal);

#endif
