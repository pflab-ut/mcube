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
  "ERROR_INVALID_EL0_32",

  "SYNC_ERROR",
  "SYSCALL_ERROR"
};

void wait_until_next_interrupt(void)
{
  asm volatile("wfi");
}

void show_invalid_entry_message(int type, unsigned long esr,
                                unsigned long address, struct full_regs *regs)
{
  printk("%s, ESR: 0x%lx, EC: 0x%lx, address: 0x%lx\r\n",
         entry_error_messages[type], esr, esr >> ESR_ELx_EC_SHIFT, address);
  dump_registers(regs);

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

int handle_timer_interrupt(void)
{
  unsigned long cpu = get_cpu_id();
  // timer tick
  printk("call do_timer_tick()\n");
  printk("handler CNTV_TVAL: %lu\n", get_cntvct_el0());
  //  printk("handler CNTVCT   : 0x%lx\n", get_cntvct_el0());
  pdebug_array(run_tq[cpu].array);

  if (current_th[cpu] != &kernel_th[cpu]) {
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
    sched_end = true;
    disable_timer_interrupt();
    current_th[cpu] = &kernel_th[cpu];
  } else {
    do_release();
    //    do_timer_tick();
    do_sched();
  }

  return IRQ_HANDLED;
}



void init_irq(void)
{
  //  printk("init_irq()\n");
  enable_local_irq();

#if CONFIG_ARCH_ARM_RASPI3
  enable_mailbox_interrupt();
#endif /* CONFIG_ARCH_ARM_RASPI3 */
}
