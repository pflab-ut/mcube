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

void do_switch_thread(void)
{
  unsigned long cpu = get_cpu_id();
  PDEBUG("%s()\n", __func__);
  if (current_th[cpu] != prev_th[cpu]) {
    /* save interrupt program counter */
    asm volatile("mrs %0, elr_el1" : "=r"(prev_th[cpu]->interrupt_program_counter));
    /* save stack pointer. */
    asm volatile("mov %0, sp" : "=r"(prev_th[cpu]->current_sp));
    
    prev_th[cpu] = current_th[cpu];
    if (!(current_th[cpu]->thflags & THFLAGS_START_TH)) {
      /* start thread */
      current_th[cpu]->thflags |= THFLAGS_START_TH;
      /* save interrupt program counter */
      asm volatile("msr elr_el1, %0" :: "r"(current_th[cpu]->run_func));
      asm volatile("mov sp, %0" :: "r"(get_context_top(current_th[cpu])));
      asm volatile("eret");
    }
    /* resume stack pointer. */
    asm volatile("mov sp, %0" :: "r"(current_th[cpu]->current_sp));
    /* save interrupt program counter */
    asm volatile("msr elr_el1, %0" :: "r"(current_th[cpu]->interrupt_program_counter));    
  }
}


void handle_timer_interrupt(void)
{
  unsigned long cpu = get_cpu_id();
  // timer tick
  printk("call do_timer_tick()\n");
  printk("handler CNTV_TVAL: 0x%lx\n", get_cntvct_el0());
  //  printk("handler CNTVCT   : 0x%lx\n", get_cntvct_el0());
  pdebug_array(run_tq[cpu].array);
  if (current_th[cpu] != &idle_th[cpu]) {
    printk("current_th: id = %lu sched.remaining = %ld\n",
           current_th[cpu]->id, current_th[cpu]->sched.remaining);
    current_th[cpu]->sched.remaining -= CPU_CLOCK_TO_USEC(get_current_cpu_time()
                                                          - current_th[cpu]->sched.begin_cpu_time);
    printk("current_th[%lu]->sched.remaining = %ld\n", cpu, current_th[cpu]->sched.remaining);
    if (current_th[cpu]->sched.remaining <= 0) {
      do_end_job(current_th[cpu]);
    }
  }
  update_jiffies();
  // clear cntv interrupt and set next 1sec timer.
  set_cntv_tval_el0(timer_cntfrq);
  
  if (get_current_jiffies() >= sched_time) {
    printk("sched_time expires!!!!\n");
    sched_end = TRUE;
    disable_timer_interrupt();
    current_th[cpu] = &idle_th[cpu];
  } else {
    do_release();
    //    do_timer_tick();
    do_sched();
  }
}

void handle_uart_interrupt(void)
{
#if CONFIG_ARCH_ARM_RASPI3
  int c;
  uint32_t irq_bp = mmio_in32(IRQ_BASIC_PENDING);
#if PL011_UART
  uint32_t irq_p2 = mmio_in32(IRQ_PENDING2);
  // uart
  if (irq_bp & IRQ_BASIC_PENDING_REG2) {
    if (irq_p2 & IRQ_PENDINGn_SRC(PL011_UART_IRQ)) {
      if (mmio_in32(UART0_MASKED_INTERRUPT_STATUS_REG)
          & UART_MIS_REG_RECEIVE_MASKED_INTERRUPT_STATUS) {
        c = (unsigned char) mmio_in32(UART0_DATA_REG); // read for clear tx interrupt.
        uart_putc(c, 0);
        printk("handle_uart_interrupt(): uart\n");
      }
    }
  }
#elif MINI_UART
  uint32_t irq_p1 = mmio_in32(IRQ_PENDING1);
  // mini uart
  if (irq_bp & IRQ_BASIC_PENDING_REG1) {
    if (irq_p1 & IRQ_PENDINGn_SRC(MINI_UART_IRQ)) {
      c = (unsigned char) mmio_in32(AUX_MU_IO_REG); // read for clear tx interrupt.
      uart_putc(c, 0);
      printk("handle_uart_interrupt(): mini uart\n");
    }
  }
#else
#error "Unknown UART"
#endif
#elif CONFIG_ARCH_ARM_SYNQUACER
    /* TODO: implement */
#else
#error "Unknown Machine"
#endif /* CONFIG_ARCH_ARM_RASPI3 */
}


asmlinkage int do_irq(unsigned long irq, struct full_regs *regs)
{
  printk("do_irq()\n");
#if CONFIG_ARCH_ARM_RASPI3  
  // check inteerupt source
  irq = mmio_in32(TIMER_CORE0_IRQ_SRC);
  switch (irq) {
  case 0x8:
    handle_timer_interrupt();
    break;
  case 0x100:
    handle_uart_interrupt();
    break;
  default:
    printk("Unknown IRQ 0x%lx\n", irq);
    break;
  }
#elif CONFIG_ARCH_ARM_SYNQUACER
    /* TODO: implement */
#else
#error "Unknown Machine"
#endif /* CONFIG_ARCH_ARM_RASPI3 */
  do_switch_thread();
  return 0;
}

void init_irq(void)
{
  printk("init_irq()\n");
#if CONFIG_ARCH_ARM_RASPI3  
  /* IRQ routing to core 0 */
  mmio_out32(TIMER_GPU_INTERRUPTS_ROUTING,
             TIMER_GPU_INTERRUPT_ROUTING_FIQ_CORE0
             | TIMER_GPU_INTERRUPT_ROUTING_IRQ_CORE0);
#elif CONFIG_ARCH_ARM_SYNQUACER
  /* TODO: implement */
#else
#error "Unknown Machine"
#endif /* CONFIG_ARCH_ARM_RASPI3 */
  enable_local_irq();
}
