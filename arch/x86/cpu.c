/**
 * @file arch/x86/cpu.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

cpuid_info cpu_info;

void copy_arch_process(struct task_struct *p, unsigned long func, unsigned long arg)
{
}


void init_ap(void)
{
	init_uart();
	init_dsctbl();
	init_irq();
	init_cpu();
	init_apic();
	init_timer(TICK_USEC);
}


int ap_main(void)
{
	init_ap();
	//	printk("ap_main()\n");
#if CONFIG_ENV_SMP
	ap_usermain();
#endif /* CONFIG_ENV_SMP */
	exit(0);
	/* not reached */
	return 0;
}

void init_fpu(void)
{
	uint32_t val = get_cr0();
	val &= ~CR0_EMULATION;
	val |= CR0_MONITOR_COPROCESSOR;
	set_cr0(val);
	finit();
}


void init_cpu(void)
{
	init_fpu();
}

void init_smp(void)
{
	int cpu = get_cpu_id();
	if (cpu == 0 && NR_INTRA_KERNEL_CPUS > 1) {
		//		apic_lock.lock = SPIN_UNLOCKED;
		//		nr_apic_locks = 0;
		/* BSP notifies SIPI to all APs */
		init_lapic_sipi();
	}
}


void print_vendor_id(void)
{
	char vendor_id[VENDOR_ID_LENGTH+1];
	unsigned int num;
	cpuid(0x0, &num, (unsigned int *) &vendor_id[0],
				(unsigned int *) &vendor_id[8], (unsigned int *) &vendor_id[4]);
	printk("Largest Standard Function Number Supported: %d\n", num);
#if 1
	vendor_id[VENDOR_ID_LENGTH] = '\0';
	printk("Vendor ID: %s\n", vendor_id);
#else
	puts("Vendor ID: ");
	vendor_id[VENDOR_ID_LENGTH] = '\0';
	puts(vendor_id);
	putchar('\n');
#endif
}


void save_cpu_info(void)
{
	cpuid(0x1, &cpu_info.eax, &cpu_info.ebx,
				&cpu_info.ecx, &cpu_info.edx);
}

void set_cpu_frequency(void)
{
	cpuid_info cinfo;
	char str[5];
	cpuid(0x80000004, &cinfo.eax, &cinfo.ebx, (uint32_t *) str, &cinfo.edx);
	str[4] = '\0';
	printk("cpu_frequency = %s\n", str);
	CPU_CLOCK = strtoul(str, NULL, 10);
}




void print_cpu_brand(void)
{
	char cpu_brand[CPU_BRAND_LENGTH+1];
	cpuid(0x80000002,
				(uint32_t *) &cpu_brand[0],
				(uint32_t *) &cpu_brand[4],
				(uint32_t *) &cpu_brand[8],
				(uint32_t *) &cpu_brand[12]);
	cpuid(0x80000003,
				(uint32_t *) &cpu_brand[16],
				(uint32_t *) &cpu_brand[20],
				(uint32_t *) &cpu_brand[24],
				(uint32_t *) &cpu_brand[28]);
	cpuid(0x80000004,
				(uint32_t *) &cpu_brand[32],
				(uint32_t *) &cpu_brand[36],
				(uint32_t *) &cpu_brand[40],
				(uint32_t *) &cpu_brand[44]);
	cpu_brand[CPU_BRAND_LENGTH] = '\0';
	printk("%s\n", cpu_brand);
}
