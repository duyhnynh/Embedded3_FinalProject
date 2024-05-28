#include "../include/interrupt.h"
#include "../include/timer.h"

void init_interrupts(void) {
  // Enable system timer 1 and 3 interrupts
  init_system_timer();

  // Enable IRQ for system timer
  enable_system_timer_irq();
}

// Enable system timer 1 interrupt
void enable_system_timer_irq(void) { INTERRUPT_ENABLE_IRQS |= SYS_TIMER_IRQ; }

// Disable system timer 1 interrupt
void disable_system_timer_irq(void) { INTERRUPT_DISABLE_IRQS |= SYS_TIMER_IRQ; }

void handle_irq_elx(void) {
  unsigned int irq_pending;

  // Check for pending interrupts on IRQ_PENDING_1
  irq_pending = IRQ_PENDING_1;

  if (irq_pending & SYS_TIMER_IRQ) {
    handle_system_timer();
    // Clear the interrupt after handling
    IRQ_PENDING_1 |= SYS_TIMER_IRQ;
  }
}
