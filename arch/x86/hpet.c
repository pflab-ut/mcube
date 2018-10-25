/**
 * @file arch/x86/hpet.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

void handle_hpet_timer_tick(interrupt_context_t *context)
{
  unsigned long cpu = get_cpu_id();
  print("handle_hpet_timer_tick(): cpu = %lu\n", cpu);
  //  inf_loop();

	/* clear timer 0 interrupt status */
	mmio_out64(GENERAL_INTERRUPT_STATUS_64, GENERAL_INTERRUPT_STATUS_T0_INTERRUPT_STS);


  if (current_th[cpu] != &idle_th[cpu]) {
    PDEBUG("current_th: id = %lu sched.remaining = %ld\n",
           current_th[cpu]->id, current_th[cpu]->sched.remaining);
    current_th[cpu]->sched.remaining -=
      CPU_CLOCK_TO_USEC(get_current_cpu_time() - current_th[cpu]->sched.begin_cpu_time);
    if (current_th[cpu]->sched.remaining <= 0) {
      do_end_job(current_th[cpu]);
    }
  }
  
  update_jiffies();

  if (sched_time <= sys_jiffies) {
		print("handle_hept_timer_tick(): sched_end: cpu = %lu\n", cpu);
    sched_end = TRUE;
    current_th[cpu] = &idle_th[cpu];
		stop_hpet_timer(0);
  }

}


int handle_hpet_one_shot_timer(int irq, void *dummy)
{
	unsigned long cpu = get_cpu_id();
	print("handle_one_shot_timer()\n");
	//	print("handle_LAPIC_timer_tick(): current_th[%d]->id = %lu\n", cpu, current_th[cpu]->id);


	/* clear timer 1 interrupt status */
	mmio_out64(GENERAL_INTERRUPT_STATUS_64, GENERAL_INTERRUPT_STATUS_T1_INTERRUPT_STS);

	/* decrease remaining of current thread before update it */
	//	smp_barrier(4);
	do_sched();

	print("current_th[%lu]->id = %lu\n", cpu, current_th[cpu]->id);

	return 0;
}



void init_hpet_timer_irq(void)
{
	int i;
	print("init_hpet_timer_irq()\n");
	//	print("GENERAL_CAP_ID_64 = %lx\n", mmio_in64(GENERAL_CAP_ID_64));
	/* Main counters are halted and zeroed */
	mmio_out64(MAIN_COUNTER_64, 0x0000000000000000);
	//	for (i = 0; i < NR_HPETS; i++) {
	for (i = 0; i < NR_HPETS; i++) {
		/* Comparator match registers reset to all 1's. */
		mmio_out64(TIMER_COMPARATOR_64(i), 0xffffffffffffffff);
    //    print("TIMER_COMPARATOR64(%d) = %lx\n", i, mmio_in64(TIMER_COMPARATOR_64(i)));
		/* All interrupts are disabled 
			 -  General Configuration and Capability Register [Offset 0x010]<1:0> = 00 
			 -  Global IRQ Enable bit comes up disabled...no comparators can deliver interrupts 
			 -  LegacyReplacement IRQ Routing Enable bit comes up disabled...8254 is on IRQ0, RTC is on IRQ8 */
		//		print("TIMER_CONFIG_CAP_64 = %lx\n", mmio_in64(TIMER_CONFIG_CAP_64(i)));
		mmio_out64(TIMER_CONFIG_CAP_64(i),
               mmio_in64(TIMER_CONFIG_CAP_64(i))
							 | TIMER_CONFIG_CAP_VAL_SET
							 | TIMER_CONFIG_CAP_PERIODIC_INTERRUPT_CAPABLE
							 | TIMER_CONFIG_CAP_INTERRUPT_ROUTE);
		/* clear all general interrupt status registers */
		mmio_out64(GENERAL_INTERRUPT_STATUS_64, 0xffffffffffffffff);
	}
#if 1
	unsigned long data;
  extern unsigned long ISR_Dispatcher;
	/* set Timer N FSB Interrupt Route Register */
  //  data = (unsigned long) &common_interrupt;
	data = (unsigned long) &ISR_Dispatcher;
	data <<= 32;
	data |= HPET_TIMER0_IRQ;
	mmio_out64(TIMER_FSB_INTERRUPT_ROUTE_64(0), data);
#endif
#if 1
	print("mmio_in32(IO_REG_SELECT) = 0x%x\n", mmio_in32(IO_REG_SELECT));
	print("mmio_in32(IO_WIN) = 0x%x\n", mmio_in32(IO_WIN));
#endif
	mmio_out32(IO_REG_SELECT, IOAPIC_ID_OFFSET);
	//	print("IOAPIC_ID = 0x%x\n", mmio_in64(IO_WIN));
	mmio_out32(IO_REG_SELECT, IOAPIC_VER_OFFSET);
	//	print("IOAPIC_VER = 0x%x\n", mmio_in64(IO_WIN));

	//	print("&common_interrupt = 0x%x\n", &common_interrupt);
#if 0
	/* Timer 0 is periodic */
	set_idsc(idt_start + HPET_REDIRECTION_OFFSET + HPET_TIMER0_IRQ,
					 (uint32_t) &common_interrupt, 2 * 8, AR_INTGATE32);
  setup_irq(HPET_REDIRECTION_OFFSET + HPET_TIMER0_IRQ, &HPET_periodic_timer_irq);

	/* Timer 1 is one-shot */
	set_idsc(idt_start + HPET_REDIRECTION_OFFSET + HPET_TIMER1_IRQ,
					 (uint32_t) &common_interrupt, 2 * 8, AR_INTGATE32);
  setup_irq(HPET_REDIRECTION_OFFSET + HPET_TIMER1_IRQ, &HPET_one_shot_timer_irq);
#endif
  // TODO: implement set_isr() for hpet.
  set_isr(HPET_REDIRECTION_OFFSET + HPET_TIMER0_IRQ, handle_hpet_timer_tick);
  set_isr(HPET_REDIRECTION_OFFSET + HPET_TIMER1_IRQ, handle_hpet_timer_tick);
  unsigned long addr = MEM_ISR_TABLE + 8 * (HPET_REDIRECTION_OFFSET + HPET_TIMER0_IRQ);
  print("addr = 0x%lx\n", *((unsigned long *) addr));
}


void start_hpet_timer(unsigned int ch)
{
  print("start_hpet_timer()\n");
  //  unsigned int tick_interval = HPET_TICK_MS(1000);
  unsigned long tick_interval = 10000000;
  //  unsigned int tick_interval = HPET_HZ;
  print("tick_interval = %lu\n", tick_interval);
  //  unsigned long cpu = get_cpu_id();
	switch (ch) {
	case 0:
		/* Route the interrupts.
			 This includes the LegacyReplacement Route bit, Interrupt Route bit (for each 
			 timer), interrupt type (to select the edge or level type for each timer).  */
    print("mmio_in64(TIMER_CONFIG_CAP_64(0)) = 0x%lx\n", mmio_in64(TIMER_CONFIG_CAP_64(ch)));
		//		print("IO_REDIRECTION_TABLE_REG_OFFSET(HPET_TIMER0_IRQ) = 0x%lx\n", mmio_in64(IO_WIN));
    //    print("tick_interval = %lu\n", tick_interval);
		/* set comparator 0 to tick_interval */
		mmio_out64(TIMER_COMPARATOR_64(0), tick_interval);
    
		/* start timer[ch] periodic interrupt */
		mmio_out64(TIMER_CONFIG_CAP_64(ch),
               mmio_in64(TIMER_CONFIG_CAP_64(ch))
							 | TIMER_CONFIG_CAP_INTERRUPT_ROUTE
							 | TIMER_CONFIG_CAP_FSB_INTERRUPT_DELIVERY
							 | TIMER_CONFIG_CAP_FSB_INTERRUPT_ENABLE
							 | TIMER_CONFIG_CAP_PERIODIC_INTERRUPT_CAPABLE
							 | TIMER_CONFIG_CAP_TYPE
							 | TIMER_CONFIG_CAP_INTERRUPT_ENABLE
							 | TIMER_CONFIG_CAP_INTERRUPT_TYPE);
		
		/* enable Timer0 IRQ */
		mmio_out32(IO_REG_SELECT, IO_REDIRECTION_TABLE_REG_OFFSET(HPET_TIMER0_IRQ));
		/* edge trigger */
		mmio_out64(IO_WIN,
               (mmio_in64(IO_WIN)
                & ~IO_REDIRECTION_TABLE_REG_INTERRUPT_MASK)
               // | IO_REDIRECTION_TABLE_REG_TRIGGER_MODE /* level trigger if set */
							 | (HPET_REDIRECTION_OFFSET + HPET_TIMER0_IRQ));
		/* save cpu time when starting main counter */
		//		start_cpu_time = current_cpu_time() + usec2tsc(1000 * 1000);
    //		begin_budget(current_th[cpu]);
		/* start main counter */
		mmio_out64(GENERAL_CONFIG_64,
               GENRAL_CONFIG_LEGACY_REPLACEMENT_ROUTE | GENERAL_CONFIG_ENABLE);
    //    inf_loop();
		break;
	case 1:

		/* start timer[ch] one-shot interrupt */
		mmio_out64(TIMER_CONFIG_CAP_64(ch),
               mmio_in64(TIMER_CONFIG_CAP_64(ch))
							 | TIMER_CONFIG_CAP_INTERRUPT_ROUTE
							 | TIMER_CONFIG_CAP_FSB_INTERRUPT_DELIVERY
							 | TIMER_CONFIG_CAP_FSB_INTERRUPT_ENABLE
							 | TIMER_CONFIG_CAP_TYPE
							 | TIMER_CONFIG_CAP_INTERRUPT_ENABLE
							 | TIMER_CONFIG_CAP_INTERRUPT_TYPE);
		
		/* enable Timer1 IRQ */
		mmio_out64(IO_REG_SELECT, IO_REDIRECTION_TABLE_REG_OFFSET(HPET_TIMER1_IRQ));
		/* edge trigger */
		mmio_out64(IO_WIN,
               (mmio_in64(IO_WIN)
								& ~IO_REDIRECTION_TABLE_REG_INTERRUPT_MASK)
							 // | IO_REDIRECTION_TABLE_REG_TRIGGER_MODE /* level trigger if set */
							 | (HPET_REDIRECTION_OFFSET + HPET_TIMER1_IRQ));
		break;
	default:
		print("%s:%s():%d: unknown channel %u\n", __FILE__, __func__, __LINE__, ch);
		break;
	}
}


void stop_hpet_timer(unsigned int ch)
{
	switch (ch) {
	case 0:
		/* stop timer[ch] periodic interrupt */
		mmio_out64(TIMER_CONFIG_CAP_64(ch),
               mmio_in64(TIMER_CONFIG_CAP_64(ch))
							 & ~TIMER_CONFIG_CAP_INTERRUPT_ROUTE
							 & ~TIMER_CONFIG_CAP_FSB_INTERRUPT_DELIVERY
							 & ~TIMER_CONFIG_CAP_FSB_INTERRUPT_ENABLE
							 & ~TIMER_CONFIG_CAP_PERIODIC_INTERRUPT_CAPABLE
							 & ~TIMER_CONFIG_CAP_TYPE
							 & ~TIMER_CONFIG_CAP_INTERRUPT_ENABLE
							 & ~TIMER_CONFIG_CAP_INTERRUPT_TYPE);
		break;
	case 1:
		/* stop timer[ch] periodic interrupt */
		mmio_out64(TIMER_CONFIG_CAP_64(ch),
               mmio_in64(TIMER_CONFIG_CAP_64(ch))
							 & ~TIMER_CONFIG_CAP_INTERRUPT_ROUTE
							 & ~TIMER_CONFIG_CAP_FSB_INTERRUPT_DELIVERY
							 & ~TIMER_CONFIG_CAP_FSB_INTERRUPT_ENABLE
							 & ~TIMER_CONFIG_CAP_TYPE
							 & ~TIMER_CONFIG_CAP_INTERRUPT_ENABLE
							 & ~TIMER_CONFIG_CAP_INTERRUPT_TYPE);
		break;
	default:
		print("%s:%s():%d: unknown channel %u\n", __FILE__, __func__, __LINE__, ch);
		break;
	}
	/* stop main counter */
  /*
  mmio_out64(GENERAL_CONFIG_64,
             ~GENRAL_CONFIG_LEGACY_REPLACEMENT_ROUTE & ~GENERAL_CONFIG_ENABLE);
  */
}

void measure_hpet_timer(void)
{
	// TODO: measure HPET frequency
}

void init_hpet_timer(unsigned long tick_us)
{
	//	print("init_timer()\n");

	measure_hpet_timer();
	stop_hpet_timer(0);
	init_hpet_timer_irq();

	print("mmio_in64(TIMER_COMPARATOR_64(0)) = %lx\n", mmio_in64(TIMER_COMPARATOR_64(0)));
	print("GENERAL_INTERRUPT_STATUS_64 = 0x%lx\n", mmio_in64(GENERAL_INTERRUPT_STATUS_64));
}
