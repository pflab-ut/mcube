/**
 * @file arch/x86/lapic.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


int handle_lapic_timer_tick(int irq, void *dummy)
{
	unsigned long cpu = get_cpu_id();
	printk("handle_timer_tick(): cpu = %lu\n", cpu);
	//	inf_loop();
#if 0
	tick_count++;
	if (tick_count == 1000) {
		printk("%d count\n", tick_count);
		tick_count = 0;
	}
#endif
	//	printk("handle_timer_tick(): cpu = %lu\n", cpu);
	timer_count[cpu]++;
	//	printk("timer_count = %d\n", timer_count);x
	//	printk("timer_count[%d] = %d\n", cpu, timer_count[cpu]);
#if 0
	if (timer_count[cpu] == SU2EU(1)) {
		//		printk("cpu = %lu\n", cpu);
		//		*((unsigned long *) 0x10010) = cpu;
		tcur[cpu] = get_current_cpu_time();
		//		printk("cpu = %lu: tsc %llu: %llu us\n",
		//					 cpu, tcur[cpu] - tprev[cpu], tsc2usec(tcur[cpu] - tprev[cpu]));
		tprev[cpu] = tcur[cpu];
		timer_count[cpu] = 0;
	}
#endif
	//	printk("handle_LAPIC_timer_tick(): current_th[%d]->id = %llu\n", cpu, current_th[cpu]->id);


#if 1
  if (sched_time <= sys_jiffies) {
		//    printk("handle_LAPIC_timer_tick(): sched_end: cpu = %lu\n", cpu);
    sched_end = TRUE;
    current_th[cpu] = &idle_th[cpu];
		//		stop_lapic_timer();
    //    stop_timer(0);
    return 0;
  }
#endif

	/* decrease remaining of current thread before update it */
	//	smp_barrier(4);
	do_release();
	do_sched();

	return 0;
}


void init_lapic_timer_irq(uint8_t vector, uint8_t timer_flag, uint8_t divisor, uint32_t count)
{
	unsigned long cpu = get_cpu_id();
	uint8_t div_flag = 0xb;
	timer_count[cpu] = 0;
	// base clock: divisor / count[MHz]
	switch (divisor) {
	case 1:
		div_flag = 0xb;
		break;
  case 2:
		div_flag = 0x0;
		break;
  case 4:
		div_flag = 0x1;
		break;
  case 8:
		div_flag = 0x2;
		break;
  case 16:
		div_flag = 0x3;
		break;
  case 32:
		div_flag = 0x8;
		break;
  case 64:
		div_flag = 0x9;
		break;
  case 128:
		div_flag = 0xa;
		break;
  default:
		printk("Error: Unknown divisor %u\n", divisor);
		break;
	}
	switch (timer_flag) {
  case TIMER_PERIODIC:
		mmio_out32(LAPIC_LVT_TIMER, mmio_in32(LAPIC_LVT_TIMER) | LAPIC_LVT_TIMER_PERIODIC | (uint32_t) vector);
    break;
  case TIMER_ONESHOT:
		mmio_out32(LAPIC_LVT_TIMER, mmio_in32(LAPIC_LVT_TIMER) | (uint32_t) vector);
    break;
  default:
    printk("Error: unknown timer flag %u\n", timer_flag);
    break;
	}
	mmio_out32(LAPIC_DIV_CONFIG, div_flag);
	//	printk("idt_start = %x\n", idt_start);

	printk("cpu = %lu\n", cpu);
  //	set_idsc(idt_start + LAPIC_TIMER_IRQ, (uint32_t) &common_interrupt, 2 * 8, AR_INTGATE32);


  printk("handle_lapic_timer_tick = %lx\n", (unsigned long) handle_lapic_timer_tick);
  //	setup_irq(LAPIC_TIMER_IRQ, &LAPIC_timer_irq);

	printk("count = %u\n", count);
	mmio_out32(LAPIC_INIT_COUNT, count);
	printk("LAPIC_LVT_TIMER = %x\n", mmio_in32(LAPIC_LVT_TIMER));
}


void start_lapic_timer(unsigned int ch)
{
	unsigned long cpu = get_cpu_id();
	tprev[cpu] = rdtsc();
	wait(0x10000 * (cpu + 1));
	mmio_out32(LAPIC_EOI, 0x0);
	wait(0x10000 * (cpu + 1));
	mmio_out32(LAPIC_LVT_TIMER,
             mmio_in32(LAPIC_LVT_TIMER) & ~LAPIC_LVT_ENABLE_MASK);
	wait(0x10000 * (cpu + 1));
}


void stop_lapic_timer(unsigned int ch)
{
	//	printk("stop_timer()\n");
	mmio_out32(LAPIC_LVT_TIMER, mmio_in32(LAPIC_LVT_TIMER) | LAPIC_LVT_ENABLE_MASK);
}

void measure_lapic_timer(void)
{
	// TODO: measure CPU frequency
	cpu_bus_freq_mhz = CPU_CLOCK_MHZ_PER_USEC;
	//	printk("cpu_bus_freq_mhz = %d\n", cpu_bus_freq_mhz);
}

void init_lapic_timer(unsigned long tick_us)
{
	//	printk("init_timer()\n");
  //	unsigned long cpu = get_cpu_id();
	unsigned long cpu_bus;
	measure_lapic_timer();
	stop_lapic_timer(0);
	// default period is 1ms
	//	init_lapic_timer_irq(LAPIC_TIMER_IRQ, 1, cpu_bus_freq_mhz);
	//init_lapic_timer_irq(LAPIC_TIMER_IRQ, 2, 33000);

	//	sys_tsc = cpu_bus_freq_mhz * TICK_USEC;

	// Corei5 750: cpu bus freqnency = CPU frequency / 20
	cpu_bus = CPU_CLOCK_MHZ_PER_USEC / 20;
	sys_tsc = cpu_bus * tick_us;
	init_lapic_timer_irq(LAPIC_TIMER_IRQ, TIMER_PERIODIC, 1, sys_tsc);
}
