/**
 * @file arch/x86/init.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


void init_arch(void)
{
#if 0
#if 0
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
#if 1
  for (;;) {
    //    printk("a");
    //printk("read_hpet_counter() = %lu\n", read_hpet_counter());
    //    printk("TIMER_COMPARATOR_64(0) = %lu\n", mmio_in64(TIMER_COMPARATOR_64(0)));
  }
#endif
#endif
#endif
}

void init_arch_ap(void)
{
}


void exit_arch(void)
{
}

void exit_arch_ap(void)
{
}
