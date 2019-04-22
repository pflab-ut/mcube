/**
 * @file arch/x86/init.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


static void setup_idt(void)
{
  for (int i = 0; i < EXCEPTION_GATES; i ++) {
    set_intr_gate(i, (void *) &idt_exception_stubs[i]);
  }

  set_intr_gate(HALT_CPU_IPI_VECTOR, halt_cpu_ipi_handler);

  load_idt(&idtdesc);
}

void clear_bss(void)
{
  memset(__bss_start, 0, __bss_end - __bss_start);
}

static void print_info(void)
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


void init_arch(void)
{
  /* Before anything else, zero the bss section. As said by C99:
   * ¡ÈAll objects with static storage duration shall be inited
   * before program startup¡É, and that the implicit init is done
   * with zero. Kernel assembly code also assumes a zeroed BSS
   * space */
  clear_bss();
  serial_init();

  /*
   * Very-early setup: Do not call any code that will use
   * printk(), `current', per-CPU vars, or a spin lock.
   */

  setup_idt();

  schedulify_this_code_path(BOOTSTRAP);

  /*
   * Memory Management init
   */

  print_info();

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
  main(0, NULL);
  halt();
}

void init_arch_ap(void)
{
}


void exit_arch_ap(void)
{
}


#if 0

void init_arch(void)
{
  disable_local_irq();
  init_cpu();
  /* initialize console */
#if CONFIG_PRINT2CONSOLE
  init_tty();
#elif CONFIG_PRINT2UART
  init_uart();
#else
#error "Unknown Print to Output"
#endif /* CONFIG_PRINT2UART */

  //  tty_set_textcolor(TTY_ID, TEXTCOLOR_LTGRAY, TEXTCOLOR_BLACK);
  //  tty_clear(TTY_ID);


  /* initialize memory */
  init_acpi();
  init_pmap();
  init_page();

  init_irq();
  init_exception();
  init_syscall();


  init_keyboard();
  init_apic();
  init_kmalloc();
#if 0
  print_cpu_brand();
  print_simd_info();
  print_vendor_id();
#endif

  sched_time = 100;

  init_timer(TICK_USEC);


  enable_local_irq();


  //asm volatile("int 0x0");
  //  asm volatile("int 0x12");
  //  asm volatile("int 0x32");
  //  kshell();
  //  init_cache();
  printk("init_end()\n");

  for (;;) {
    //    printk("a");
    //printk("read_hpet_counter() = %lu\n", read_hpet_counter());
    //    printk("TIMER_COMPARATOR_64(0) = %lu\n", mmio_in64(TIMER_COMPARATOR_64(0)));
  }
}

#endif
