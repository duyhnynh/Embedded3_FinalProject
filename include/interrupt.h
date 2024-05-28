#include "../include/gpio.h"

// Base MMIO address
// #define MMIO_BASE 0x3F000000

// Interrupt controller base address
#define REGS_IRQ (MMIO_BASE + 0xB000)

// Interrupt controller registers for RPI3
#define IRQ_PENDING_0 (*(volatile unsigned int *)(REGS_IRQ + 0x200))
#define IRQ_PENDING_1 (*(volatile unsigned int *)(REGS_IRQ + 0x204))
#define FIQ_CONTROL (*(volatile unsigned int *)(REGS_IRQ + 0x20C))
#define INTERRUPT_ENABLE_IRQS                                                  \
(*(volatile unsigned int *)(REGS_IRQ + 0x210))
#define ENABLE_BASIC_IRQS (*(volatile unsigned int *)(REGS_IRQ + 0x218))
#define INTERRUPT_DISABLE_IRQS                                                 \
  (*(volatile unsigned int *)(REGS_IRQ + 0x21C))
#define DISABLE_BASIC_IRQS (*(volatile unsigned int *)(REGS_IRQ + 0x224))

// IRQ definitions
#define SYS_TIMER_IRQ (1 << 1)
#define UART0_IRQ (1 << 25)

// Assembly functions
void init_interrupts(void);
void enable_interrupts(void);
void disable_interrupts(void);

// IRQ handlers
void enable_system_timer_irq(void);
void disable_system_timer_irq(void);
void enable_uart0_irq(void);
void disable_uart0_irq(void);
void handle_irq_elx(void);

