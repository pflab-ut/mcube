/**
 * @file arch/x86/apic.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

void set_lapic_error_handler(uint8_t error_handler)
{
	mmio_out32(LAPIC_LVT_ERROR,
             (mmio_in32(LAPIC_LVT_ERROR) & 0xffffff00) | error_handler);
}


void enable_lapic(uint8_t vector)
{
	mmio_out32(LAPIC_SPURIOUS_INT_VECTOR,
             mmio_in32(LAPIC_SPURIOUS_INT_VECTOR) |
						 LAPIC_SPURIOUS_INT_VECTOR_ENABLE_APIC |
						 vector);
}

void disable_lapic(void)
{
	mmio_out32(LAPIC_SPURIOUS_INT_VECTOR,
             mmio_in32(LAPIC_SPURIOUS_INT_VECTOR) &
						 ~LAPIC_SPURIOUS_INT_VECTOR_ENABLE_APIC);
}


 
void set_lapic_int_vector(uint8_t vector_addr)
{
	mmio_out32(LAPIC_SPURIOUS_INT_VECTOR,
             (mmio_in32(LAPIC_SPURIOUS_INT_VECTOR) & 0xffffff00) | vector_addr);
}



void init_lapic_sipi(void)
{
	//	printk("init_LAPIC_sipi()\n");
	mmio_out32(LAPIC_INT_COMMAND_LOW,
             LAPIC_INT_COMMAND_LOW_DESTINATION_ALLBUT |
						 LAPIC_INT_COMMAND_LOW_LEVEL_ASSERT |
						 LAPIC_INT_COMMAND_LOW_DELIVERY_MODE_INIT);
	//	delay(10 * 1000);
	wait(10000);
	/* send startup interprocessor interrupt */
#if 1
  unsigned long ap_start = 0x7c00;
	mmio_out32(LAPIC_INT_COMMAND_LOW,
             LAPIC_INT_COMMAND_LOW_DESTINATION_ALLBUT |
						 LAPIC_INT_COMMAND_LOW_LEVEL_ASSERT |
						 LAPIC_INT_COMMAND_LOW_DELIVERY_MODE_SIPI |
						 (((uint32_t) ap_start) >> 12));
#endif
	//	delay(200 * 1000);
	wait(200000);
}

void init_lapic(void)
{
	mmio_out32(LAPIC_TASK_PRIORITY, 0x00);
	mmio_out32(LAPIC_LVT_TIMER, LAPIC_LVT_TIMER_PERIODIC);
	mmio_out32(LAPIC_LVT_PERF_MONITORING_COUNTER, 0x00010000);
#if 0
	mmio_out32(LAPIC_LVT_LINT0, LAPIC_LVT_ENABLE_MASK);
	mmio_out32(LAPIC_LVT_LINT1, LAPIC_LVT_ENABLE_MASK);
#else
	mmio_out32(LAPIC_LVT_LINT0,
             LAPIC_LVT_LINT_TRIGGER_MODE_LEVEL |
						 LAPIC_LVT_DELIVERY_MODE_EXTINT);
	//	mmio_out32(LAPIC_LVT_LINT1, LAPIC_LVT_DELIVERY_MODE_NMI);
	mmio_out32(LAPIC_LVT_LINT1, 0);
#endif
	mmio_out32(LAPIC_LVT_ERROR, 0x00010000);

	enable_lapic(0x0f);	
	//	set_lapic_int_vector(0x00);
	set_lapic_int_vector(0x4e);
	set_lapic_error_handler(0x5e);
}

void init_ioapic(void)
{
}

void init_apic(void)
{
  printk("init_apic()\n");
	init_lapic();
	init_ioapic();
}
