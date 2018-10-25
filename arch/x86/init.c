/**
 * @file arch/x86/init.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


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
  init_apic();
#if 0
  print_cpu_brand();
  print_simd_info();
  print_vendor_id();
#endif

  sched_time = 100;
  
#if 0
  /* use PIT timer */
  print("use PIT timer\n");
  init_pit_timer(TICK_USEC);
#endif

#if 0
  /* use HPET timer but not work */
  print("use HPET timer\n");
  init_hpet_timer(TICK_USEC);
  start_hpet_timer(0);
#endif

#if 0
  /* use LAPIC timer */
  print("use LAPIC timer\n");
  init_lapic_timer(TICK_USEC);
  print("start_lapic_timer()\n");
  start_lapic_timer(0);
#endif
  enable_local_irq();
  
  //asm volatile("int 0x0");
  //  asm volatile("int 0x12");
  //  asm volatile("int 0x32");
  //  kshell();
  //	init_cache();
#if 0  
  for (;;) {
    //    print("a");
    //print("read_hpet_counter() = %lu\n", read_hpet_counter());
    //    print("TIMER_COMPARATOR_64(0) = %lu\n", mmio_in64(TIMER_COMPARATOR_64(0)));
  }
#endif
}


void exit_arch(void)
{
}
