/**
 * @file arch/x86/pit.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

irqreturn_t handle_pit_timer_tick(int irq, void *dummy)
{
  //	unsigned long cpu = get_cpu_id();
	printk("handle_timer_tick()\n");
	//	printk("handle_timer_tick(): current_th[0]->id = %llu\n", current_th[0]->id);

  for (;;)
    ;
#if 0
	//	printk("sched_time = %llu sys_jiffies = %llu\n", sched_time, sys_jiffies);  
  if (sched_time <= SU2EU(sys_jiffies)) {
		printk("handle_timer_tick(): sched_end\n");
    sched_end = TRUE;
		current_th[cpu] = &idle_th[cpu];

		//		stop_pit_timer(0);
		//		cli();
    return IRQ_HANDLED;
  } else {
		/* clear IRQ bit to accept new interrupt */
		outb(PIC0_OCW2, clear_irq(PIT_IRQ));
	}
#endif


	do_sched();

	//	update_jiffies();
	//	printk("handle_timer_tick() end\n");
	return IRQ_HANDLED;
}

static struct irqaction timer_irq = {
  .name       = "pit timer",
  .handler    = handle_pit_timer_tick,
  .flags      = IRQF_DISABLED | IRQF_TIMER,
  .mask       = CPU_MASK_NONE,
};

void init_pit_timer(unsigned long tick_us)
{
	/* upper bound of cnt is 65535 */
	/* 11932 = 100Hz = 10ms */
	/* 1193 = 1KHz = 1ms */
	//	uint16_t cnt = (PIT_HZ * tick_us) / 1000000;
	uint16_t cnt = 1193;
	//	uint16_t cnt = 11932;
	//	uint16_t cnt = 65535;
	stop_pit_timer(0);
	/* change interrupt period */
	outb(PIT_CTRL, 0x34);
	/* lower 8bit of interrupt period */
	outb(PIT_CNT0, cnt & 0xff);
	/* upper 8bit of interrupt period */
	outb(PIT_CNT0, cnt >> 8);

#if 1
	set_idsc(idt_start + IRQ_OFFSET + PIT_IRQ, (unsigned long) &common_interrupt,
					 2 * 8, AR_INTGATE32);
#endif
  setup_irq(PIT_IRQ, &timer_irq);
}

void start_pit_timer(unsigned int ch)
{
	/* enable IRQ0 */
	outb(PIC0_IMR, inb(PIC0_IMR) & unmask_irq(PIT_IRQ));
}

void stop_pit_timer(unsigned int ch)
{
	/* disable IRQ0 */
	outb(PIC0_IMR, inb(PIC0_IMR) | mask_irq(PIT_IRQ));
	/* clear PIT flag */
	outb(PIC0_OCW2, clear_irq(PIT_IRQ));
}

