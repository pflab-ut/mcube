/**
 * @file arch/x86/lapic.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>



void handle_lapic_timer_tick(interrupt_context_t *context)
{
	unsigned long cpu = get_cpu_id();
	printk("handle_lapic_timer_tick(): cpu = %lu\n", cpu);
  //  inf_loop();

	//	printk("handle_LAPIC_timer_tick(): current_th[%d]->id = %llu\n", cpu, current_th[cpu]->id);
  if (current_th[cpu] != &kernel_th[cpu]) {
    PDEBUG("current_th: id = %lu sched.remaining = %ld\n",
           current_th[cpu]->id, current_th[cpu]->sched.remaining);
    current_th[cpu]->sched.remaining -= CPU_CLOCK_TO_USEC(get_current_cpu_time()
                                                          - current_th[cpu]->sched.begin_cpu_time);
    if (current_th[cpu]->sched.remaining <= 0) {
      do_end_job(current_th[cpu]);
    }
  }
  
  update_jiffies();
  
  if (sched_time <= sys_jiffies) {
		//    printk("handle_LAPIC_timer_tick(): sched_end: cpu = %lu\n", cpu);
    sched_end = TRUE;
    current_th[cpu] = &kernel_th[cpu];
    stop_lapic_timer(0);
    mmio_out64(LAPIC_EOI, 0x0);
  } else {
    do_release();
    do_sched();
  }
  do_switch_thread_arch(context);
  mmio_out64(LAPIC_EOI, 0x0);
	return;
}


void init_lapic_timer_irq(uint8_t vector, uint8_t timer_flag, uint8_t divisor, uint32_t count)
{
	unsigned long cpu = get_cpu_id();
	uint8_t div_flag = 0xb;
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
  set_isr(LAPIC_TIMER_IRQ, handle_lapic_timer_tick);

  printk("handle_lapic_timer_tick = %lx\n", (unsigned long) handle_lapic_timer_tick);
  //	setup_irq(LAPIC_TIMER_IRQ, &LAPIC_timer_irq);

	printk("count = %u\n", count);
	mmio_out32(LAPIC_INIT_COUNT, count);
	printk("LAPIC_LVT_TIMER = %x\n", mmio_in32(LAPIC_LVT_TIMER));
}


void start_lapic_timer(unsigned int ch)
{
	unsigned long cpu = get_cpu_id();
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
  //	cpu_bus = CPU_CLOCK_MHZ_PER_USEC / 20;
	cpu_bus = CPU_CLOCK_MHZ_PER_USEC / 2;
  sys_tsc = cpu_bus * tick_us;
  printk("sys_tsc = %lu\n", sys_tsc);
	init_lapic_timer_irq(LAPIC_TIMER_IRQ, TIMER_PERIODIC, 1, sys_tsc);
}
