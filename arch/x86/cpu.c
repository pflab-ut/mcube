/**
 * @file arch/x86/cpu.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


void copy_arch_process(struct task_struct *p, unsigned long func, unsigned long arg)
{
}


void init_ap(void)
{
	init_uart();
  //	init_dsctbl();
  //	init_irq();
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
  registers4_t regs4;
  int i;
	cpuid(0x0, &regs4);
	printk("Largest Standard Function Number Supported: %ld\n", regs4.rax);
  for (i = 0; i < 4; i++) {
    vendor_id[i] = get_byte(regs4.rbx, i);
  }
  for (i = 0; i < 4; i++) {
    vendor_id[i+4] = get_byte(regs4.rdx, i);
  }
  for (i = 0; i < 4; i++) {
    vendor_id[i+8] = get_byte(regs4.rcx, i);
  }
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
  registers4_t regs4;
	cpuid(0x1, &regs4);
}

void print_cpu_frequency(void)
{
  registers4_t regs4;
	char str[17];
  int i;
  //	cpuid(0x80000004, &regs4);
	cpuid(0x80000004, &regs4);
  for (i = 0; i < 4; i++) {
    str[i] = get_byte(regs4.rax, i);
  }
  for (i = 0; i < 4; i++) {
    str[i+4] = get_byte(regs4.rbx, i);
  }
  for (i = 0; i < 4; i++) {
    str[i+8] = get_byte(regs4.rcx, i);
  }
  for (i = 0; i < 4; i++) {
    str[i+12] = get_byte(regs4.rdx, i);
  }
	str[16] = '\0';
#if 1 
  for (i = 0; i < 16; i++) {
    printk("%d ", str[i]);
  }
  printk("\n");
#endif
	printk("cpu_frequency = %s\n", str);
  //	CPU_CLOCK = strtoul(str, NULL, 10);
}



void print_cpu_brand(void)
{
  registers4_t regs4;  
	char cpu_brand[CPU_BRAND_LENGTH+1];
  int i;
	cpuid(0x80000002, &regs4);
  for (i = 0; i < 4; i++) {
    cpu_brand[i] = get_byte(regs4.rax, i);
  }
  for (i = 0; i < 4; i++) {
    cpu_brand[i+4] = get_byte(regs4.rbx, i);
  }
  for (i = 0; i < 4; i++) {
    cpu_brand[i+8] = get_byte(regs4.rcx, i);
  }
  for (i = 0; i < 4; i++) {
    cpu_brand[i+12] = get_byte(regs4.rdx, i);
  }
	cpuid(0x80000003, &regs4);
  for (i = 0; i < 4; i++) {
    cpu_brand[i+16] = get_byte(regs4.rax, i);
  }
  for (i = 0; i < 4; i++) {
    cpu_brand[i+20] = get_byte(regs4.rbx, i);
  }
  for (i = 0; i < 4; i++) {
    cpu_brand[i+24] = get_byte(regs4.rcx, i);
  }
  for (i = 0; i < 4; i++) {
    cpu_brand[i+28] = get_byte(regs4.rdx, i);
  }
	cpuid(0x80000004, &regs4);
  for (i = 0; i < 4; i++) {
    cpu_brand[i+32] = get_byte(regs4.rax, i);
  }
  for (i = 0; i < 4; i++) {
    cpu_brand[i+36] = get_byte(regs4.rbx, i);
  }
  for (i = 0; i < 4; i++) {
    cpu_brand[i+40] = get_byte(regs4.rcx, i);
  }
  for (i = 0; i < 4; i++) {
    cpu_brand[i+44] = get_byte(regs4.rdx, i);
  }
	cpu_brand[CPU_BRAND_LENGTH] = '\0';
	printk("%s\n", cpu_brand);
}
