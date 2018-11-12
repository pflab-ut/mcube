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
  printk("handler CNTV_TVAL: %lu\n", get_cntvct_el0());
  //  printk("handler CNTVCT   : 0x%lx\n", get_cntvct_el0());
  pdebug_array(run_tq[cpu].array);
  if (current_th[cpu] != &idle_th[cpu]) {
    printk("current_th: id = %lu sched.remaining = %ld\n",
           current_th[cpu]->id, current_th[cpu]->sched.remaining);
    current_th[cpu]->sched.remaining
      -= CPU_CLOCK_TO_USEC(get_current_cpu_time()
                           - current_th[cpu]->sched.begin_cpu_time);
    printk("current_th[%lu]->sched.remaining = %ld\n",
           cpu, current_th[cpu]->sched.remaining);
    if (current_th[cpu]->sched.remaining <= 0) {
      do_end_job(current_th[cpu]);
    }
  }
  update_jiffies();
  // clear cntv interrupt and set next timer.
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

int handle_uart_interrupt(void)
{
  int c;
  uint32_t irq_bp = mmio_in32(IRQ_BASIC_PENDING);
#if PL011_UART
  uint32_t irq_p2 = mmio_in32(IRQ_PENDING2);
  // uart
  if ((irq_bp & IRQ_BASIC_PENDING_REG2)
      && (irq_p2 & IRQ_PENDINGn_SRC(PL011_UART_IRQ))
      && (mmio_in32(UART0_MASKED_INTERRUPT_STATUS_REG)
          & UART_MIS_REG_RECEIVE_MASKED_INTERRUPT_STATUS)) {
    c = (unsigned char) mmio_in32(UART0_DATA_REG); // read for clear tx interrupt.
    uart_putc(c, 0);
    printk("handle_uart_interrupt(): uart\n");
    return IRQ_HANDLED;
  }
#elif MINI_UART
  uint32_t irq_p1 = mmio_in32(IRQ_PENDING1);
  // mini uart
  if ((irq_bp & IRQ_BASIC_PENDING_REG1)
      && (irq_p1 & IRQ_PENDINGn_SRC(MINI_UART_IRQ))) {
    c = (unsigned char) mmio_in32(AUX_MU_IO_REG); // read for clear tx interrupt.
    uart_putc(c, 0);
    printk("handle_uart_interrupt(): mini uart\n");
    return IRQ_HANDLED;
  }
#else
#error "Unknown UART"
#endif
  return IRQ_UNHANDLED;
}

int handle_dmac_interrupt(void)
{
  unsigned long cpu = get_cpu_id();
  printk("handle_dmac_interrupt()\n");
  if ((mmio_in32(IRQ_BASIC_PENDING) & IRQ_BASIC_PENDING_REG1)
      && (mmio_in32(IRQ_PENDING1) & DMAC_IRQ(cpu))
      && (mmio_in32(DMAC_CH_CS(cpu)) & DMAC_CH_CS_INT)) {
    mmio_out32(DMAC_CH_CS(cpu), mmio_in32(DMAC_CH_CS(cpu)) | DMAC_CH_CS_INT);
    return IRQ_HANDLED;
  }
            
  return IRQ_UNHANDLED;
}


void handle_gpu_interrupt(void)
{
  printk("handle_gpu_interrupt()\n");
#if CONFIG_ARCH_ARM_RASPI3
  if (handle_uart_interrupt() == IRQ_HANDLED) {
    return;
  }

  if (handle_dmac_interrupt() == IRQ_HANDLED) {
    return;
  }
  
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
  unsigned long cpu = get_cpu_id();
  switch (cpu) {
  case 0:
    irq = mmio_in32(LP_CORE0_IRQ_SRC);
    break;
  case 1:
    irq = mmio_in32(LP_CORE1_IRQ_SRC);
    break;
  case 2:
    irq = mmio_in32(LP_CORE2_IRQ_SRC);
    break;
  case 3:
    irq = mmio_in32(LP_CORE3_IRQ_SRC);
    break;    
  default:
    printk("Unknown CPU 0x%lx\n", cpu);
    return 1;
    break;
  }
  
  /* check inteerupt source */
  switch (irq) {    
  case LP_CORE_IRQ_SRC_CNTVIRQ_INTERRUPT:
    handle_timer_interrupt();
    break;
  case LP_CORE_IRQ_SRC_GPU_INTERRUPT:
    handle_gpu_interrupt();
    break;
  case LP_CORE_IRQ_SRC_CNTPSIRQ_INTERRUPT:
  case LP_CORE_IRQ_SRC_CNTPNSIRQ_INTERRUPT:
  case LP_CORE_IRQ_SRC_CNTHPIRQ_INTERRUPT:
  case LP_CORE_IRQ_SRC_MAILBOX0_INTERRUPT:
  case LP_CORE_IRQ_SRC_MAILBOX1_INTERRUPT:
  case LP_CORE_IRQ_SRC_MAILBOX2_INTERRUPT:
  case LP_CORE_IRQ_SRC_MAILBOX3_INTERRUPT:
  case LP_CORE_IRQ_SRC_PMU_INTERRUPT:
  case LP_CORE_IRQ_SRC_AXI_OUTSTANDING_INTERRUPT:
  case LP_CORE_IRQ_SRC_LOCAL_TIMER_INTERRUPT:
    /* LP_CORE_IRQ_SRC_PERIPHERAL(x) not used */
  default:
    printk("Unknown IRQ 0x%lx\n", irq);
    return 2;
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
  enable_local_irq();
}
