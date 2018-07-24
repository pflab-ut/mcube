/**
 * @file arch/x86/init.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


void init_arch(void)
{
  disable_interrupt();
  /* initialize console */
  init_tty();
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
  init_hpet_timer(TICK_USEC);
 
  //asm volatile("int 0x0");
  enable_interrupt();
  //  kshell();
  //  init_uart();
  //	init_apic();
  //	init_cache(); 
  for (;;)
    ;
}


void exit_arch(void)
{
}
