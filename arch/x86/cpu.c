/**
 * @file arch/x86/cpu.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


void copy_arch_process(struct task_struct *p, unsigned long func, unsigned long arg)
{
}


void init_fpu(void)
{
  uint32_t val = get_cr0();
  val &= ~CR0_EMULATION;
  val |= CR0_MONITOR_COPROCESSOR;
  set_cr0(val);
  finit();
}


void init_ap(void)
{
	init_uart();
  //	init_dsctbl();
  //	init_irq();
	init_fpu();
	init_apic();
	init_timer(TICK_USEC);
}


int ap_main(void)
{
	init_ap();
	//	print("ap_main()\n");
#if CONFIG_ENV_SMP
	ap_usermain();
#endif /* CONFIG_ENV_SMP */
	exit(0);
	/* not reached */
	return 0;
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
	cpuid(0x0, &regs4);
	print("Largest Standard Function Number Supported: %d\n", regs4.eax);
  memcpy(&vendor_id[0], &regs4.ebx, 4);
  memcpy(&vendor_id[4], &regs4.edx, 4);
  memcpy(&vendor_id[8], &regs4.ecx, 4);
	vendor_id[VENDOR_ID_LENGTH] = '\0';
	print("Vendor ID: %s\n", vendor_id);
}


void print_simd_info(void)
{
  registers4_t regs4;
	cpuid(0x1, &regs4);
  print("MMX:    %s\n", regs4.edx & 1 << 23 ? "OK" : "NG");
  print("SSE:    %s\n", regs4.edx & 1 << 25 ? "OK" : "NG");
  print("AVX:    %s\n", regs4.ecx & 1 << 28 ? "OK" : "NG");
  print("FMA:    %s\n", regs4.ecx & 1 << 12 ? "OK" : "NG");
  cpuid(0x7, &regs4);
  print("AVX2:   %s\n", regs4.ebx & 1 <<  5 ? "OK" : "NG");
}



void print_cpu_brand(void)
{
  registers4_t regs4;  
	char cpu_brand[CPU_BRAND_LENGTH+1];
	cpuid(0x80000002, &regs4);
  memcpy(&cpu_brand[0], &regs4, sizeof(regs4));
	cpuid(0x80000003, &regs4);
  memcpy(&cpu_brand[16], &regs4, sizeof(regs4));
	cpuid(0x80000004, &regs4);
  memcpy(&cpu_brand[32], &regs4, sizeof(regs4));
	cpu_brand[CPU_BRAND_LENGTH] = '\0';
	print("%s\n", cpu_brand);
}

void init_cpu(void)
{
  // TODO: set CPU frequency dynamically.
  // Core 2 Duo Clock Hz
  CPU_CLOCK = (unsigned long) 2400 * 1000 * 1000;
  CPU_CLOCK_MHZ_PER_USEC = CPU_CLOCK / (1000 * 1000);
}
