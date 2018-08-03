/**
 * @file arch/x86/init.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


void init_arch(void)
{
  disable_local_irq();
  /* initialize console */
  init_tty();
  //  init_uart();
  //  tty_set_textcolor(TTY_ID, TEXTCOLOR_LTGRAY, TEXTCOLOR_BLACK);
  tty_clear(TTY_ID);
  /* initialize memory */
  init_acpi();
  init_pmap();
  init_page();
  //  print_pmap();
  init_irq();
  init_exception();
  
  init_syscall();
  
  init_keyboard();
#if 0
  print_cpu_brand();
  print_simd_info();
  print_vendor_id();
#endif
  //  init_pit_timer(TICK_USEC);
#if 1
  //  unsigned long val = 100000000;
  unsigned long val = 10000000;
  unsigned int n;
  n = printk("valx = 0x%lx\n", val);
  printk("n = %d\n", n);
  n = printk("val  = %lu\n", val);
  printk("n = %d\n", n);
#endif
  
  init_hpet_timer(TICK_USEC);
  sched_time = 5;
  enable_local_irq();
  start_hpet_timer(0);
  //asm volatile("int 0x0");
  //  asm volatile("int 0x12");
  //  asm volatile("int 0x32");
  //  kshell();
  //  init_uart();
  //	init_apic();
  //	init_cache(); 
  for (;;) {
    //printk("read_hpet_counter() = %lu\n", read_hpet_counter());
    //    printk("TIMER_COMPARATOR_64(0) = %lu\n", mmio_in64(TIMER_COMPARATOR_64(0)));
  }
}


void exit_arch(void)
{
}
