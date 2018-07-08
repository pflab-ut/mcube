/**
 * @file arch/arm/irq.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

const char *entry_error_messages[] = {
  "SYNC_INVALID_EL1t",
  "IRQ_INVALID_EL1t",
  "FIQ_INVALID_EL1t",
  "ERROR_INVALID_EL1T",

  "SYNC_INVALID_EL1h",
  "IRQ_INVALID_EL1h",
  "FIQ_INVALID_EL1h",
  "ERROR_INVALID_EL1h",

  "SYNC_INVALID_EL0_64",
  "IRQ_INVALID_EL0_64",
  "FIQ_INVALID_EL0_64",
  "ERROR_INVALID_EL0_64",

  "SYNC_INVALID_EL0_32",
  "IRQ_INVALID_EL0_32",
  "FIQ_INVALID_EL0_32",
  "ERROR_INVALID_EL0_32"
};

void wait_until_next_interrupt(void)
{
  asm volatile("wfi");
}

void show_invalid_entry_message(int type, unsigned long esr, unsigned long address)
{
  printk("%s, ESR: 0x%lx, address: 0x%lx\r\n", entry_error_messages[type], esr, address);
}

asmlinkage int do_IRQ(unsigned long irq, struct full_regs *regs)
{
  char c;
  disable_interrupt();
  printk("do_IRQ()\n");
  // check inteerupt source
  irq = mmio_in32(TIMER_CORE0_IRQ_SOURCE);
  switch (irq) {
  case 0x8:
    // timer tick
    printk("call do_timer_tick()\n");
    printk("handler CNTV_TVAL: 0x%lx\n", get_cntvct_el0());
    set_cntv_tval_el0(timer_cntfrq);    // clear cntv interrupt and set next 1sec timer.    
    printk("handler CNTVCT   : 0x%lx\n", get_cntvct_el0());
    //    do_timer_tick();
    do_sched();
    break;
  case 0x100:
#if 1
    // uart
    if (mmio_in32(IRQ_BASIC_BASE) & (1 << 9)) {
      if (mmio_in32(IRQ_PENDING2) & (1 << 25)) {
        if (mmio_in32(UART0_MASKED_INTERRUPT_STATUS_REG) & (1 << 4)) {
          c = (unsigned char) mmio_in32(UART0_DATA_REG); // read for clear tx interrupt.
          uart_putc(c, 0);
          printk("do_IRQ()\n");
        }
      }
    }
#else
    // mini uart
    if (mmio_in32(IRQ_BASIC_BASE) & (1 << 8)) {
      if (mmio_in32(IRQ_PENDING1) & (1 << 29)) {
        c = (unsigned char) mmio_in32(AUX_MINI_UART_IO_REG); // read for clear tx interrupt.
        uart_putc(c, 0);
        printk("do_IRQ(): mini uart\n");
      }
    }
#endif
    break;
  default:
    printk("Unknown IRQ 0x%lx\n", irq);
    break;
  }
  enable_interrupt();
  return 0;
}

void init_irq(void)
{
  printk("init_irq()\n");
  /* IRQ routing to core 0 */
  mmio_out32(TIMER_GPU_INTERRUPTS_ROUTING, 0x0);
  enable_interrupt();
}
