/**
 * @file arch/x86_64/init.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

/**
 * @fn static void setup_idt(void)
 * @brief setup IDT.
 */
static void setup_idt(void)
{
  for (int i = 0; i < EXCEPTION_GATES; i ++) {
    set_idt_gate(i, (void *) &idt_exception_stubs[i]);
  }

  set_idt_gate(HALT_CPU_IPI_VECTOR, halt_cpu_ipi_handler);

  load_idt(&idtdesc);
}

/**
 * @fn static void print_memory_info(void)
 * @brief print memory information.
 */
static void print_memory_info(void)
{
  printk("Mcube Kernel\n\n");

  printk("Text start = 0x%lx\n", __text_start);
  printk("Text end   = 0x%lx\n", __text_end);
  printk("Text size  = %d bytes\n\n", __text_end - __text_start);

  printk("Data start = 0x%lx\n", __data_start);
  printk("Data end   = 0x%lx\n", __data_end);
  printk("Data size  = %d bytes\n\n", __data_end - __data_start);

  printk("BSS start  = 0x%lx\n", __bss_start);
  printk("BSS end    = 0x%lx\n", __bss_end);
  printk("BSS size   = %d bytes\n\n", __bss_end - __bss_start);
}

/**
 * @fn static void print_vendor_id(void)
 * @brief print vendor ID.
 */
static void print_vendor_id(void)
{
  char vendor_id[VENDOR_ID_LENGTH + 1];
  cpuid_info_t cinfo;
  cpuid(0x0, &cinfo);
  printk("Largest Standard Function Number Supported: %d\n", cinfo.rax);
  memcpy(&vendor_id[0], &cinfo.rbx, 4);
  memcpy(&vendor_id[4], &cinfo.rdx, 4);
  memcpy(&vendor_id[8], &cinfo.rcx, 4);
  vendor_id[VENDOR_ID_LENGTH] = '\0';
  printk("Vendor ID: %s\n", vendor_id);
}

/**
 * @fn static void print_simd_info(void)
 * @brief print SIMD information.
 */
static void print_simd_info(void)
{
  cpuid_info_t cinfo;
  cpuid(0x1, &cinfo);
  printk("MMX: %s, ", cinfo.rdx & 1 << 23 ? "OK" : "NG");
  printk("SSE: %s, ", cinfo.rdx & 1 << 25 ? "OK" : "NG");
  printk("AVX: %s, ", cinfo.rcx & 1 << 28 ? "OK" : "NG");
  printk("FMA: %s, ", cinfo.rcx & 1 << 12 ? "OK" : "NG");
  cpuid(0x7, &cinfo);
  printk("AVX2: %s\n", cinfo.rbx & 1 <<  5 ? "OK" : "NG");
}

/**
 * @fn static void print_cpu_brand_info(void)
 * @brief print CPU brand information.
 */
static void print_cpu_brand_info(void)
{
  cpuid_info_t cinfo;
  char cpu_brand[CPU_BRAND_LENGTH + 1];
  cpuid(0x80000002, &cinfo);
  memcpy(&cpu_brand[0], &cinfo, sizeof(cinfo));
  cpuid(0x80000003, &cinfo);
  memcpy(&cpu_brand[16], &cinfo, sizeof(cinfo));
  cpuid(0x80000004, &cinfo);
  memcpy(&cpu_brand[32], &cinfo, sizeof(cinfo));
  cpu_brand[CPU_BRAND_LENGTH] = '\0';
  printk("CPU Brand: %s\n", cpu_brand);
}

/**
 * @fn static void print_info(void)
 * @brief print information.
 */
static void print_info(void)
{
  print_memory_info();
  print_vendor_id();
  print_simd_info();
  print_cpu_brand_info();
}

void clear_bss(void)
{
  memset(__bss_start, 0, __bss_end - __bss_start);
}


void init_arch(void)
{
  /* Before anything else, zero the bss section. As said by C99:
   * ¡ÈAll objects with static storage duration shall be inited
   * before program startup¡É, and that the implicit init is done
   * with zero. Kernel assembly code also assumes a zeroed BSS
   * space */
  clear_bss();
  init_spinlock();
  init_uart();

  /*
   * Very-early setup: Do not call any code that will use
   * printk(), `current', per-CPU vars, or a spin lock.
   */

  setup_idt();

  schedulify_this_code_path(BOOTSTRAP);

  /* print information */
  print_info();

  init_socket();
  /*
   * Memory Management init
   */
  /* First, don't override the ramdisk area (if any) */
  ramdisk_init();

  /* Then discover our physical memory map .. */
  e820_init();

  /* and tokenize the available memory into allocatable pages */
  pagealloc_init();

  /* With the page allocator in place, git rid of our temporary
   * early-boot page tables and setup dynamic permanent ones */
  vm_init();

  /* MM basics done, enable dynamic heap memory to kernel code
   * early on .. */
  init_kmalloc();

  //  init_syscall();

  /*
   * Secondary-CPUs startup
   */

  /* Discover our secondary-CPUs and system IRQs layout before
   * initializing the local APICs */
  mptables_init();

  /* Remap and mask the PIC; it's just a disturbance */
  pic_init();

  /* Initialize the APICs (and map their MMIO regs) before enabling
   * IRQs, and before firing other cores using Inter-CPU Interrupts */
  apic_init();
  ioapic_init();


  /* SMP infrastructure ready, fire the CPUs! */
  smpboot_init();

  keyboard_init();

  /* Startup finished, roll-in the scheduler! */
  sched_init();
  enable_local_irq();


  /*
   * Second part of kernel initialization (Scheduler is now on!)
   */

  init_ext2();


  // Signal the secondary cores to run their own test-cases code.
  // They've been waiting for us (thread 0) till all of kernel
  // subsystems has been properly initialized.  Wait No More!
  //  smpboot_trigger_secondary_cores_testcases();

  //  run_test_cases();
  //  user_main(0, NULL);

}

void exit_arch(void)
{
}


/*
 * Bootstrap-CPU start; we came from head.S
 */
__noreturn void kernel_start(void)
{
  int argc = 1;
  char exec_file[] = {"build/mcube"};
  char *argv[NR_ARGS] = {exec_file};
  main(argc, argv);
  halt();
}

void init_arch_ap(void)
{
}


void exit_arch_ap(void)
{
}
