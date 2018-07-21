/**
 * @file arch/x86/init.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


void init_arch(void)
{
#if 1
  disable_interrupt();
  /* initialize console */
  init_tty();
  //  tty_set_textcolor(TTY_ID, TEXTCOLOR_LTGRAY, TEXTCOLOR_BLACK);
  tty_clear(TTY_ID);
#if 0  
  /* initialize memory */
  init_acpi();
  init_pmap();
  init_page();
  //  print_pmap();
#endif
  init_irq();
  init_exception();
  
  //  init_syscall();
  
  timer_init(20);
  //  init_keyboard();
#if 0
  print_cpu_brand();
  print_simd_info();
  print_vendor_id();
#endif
 
  //asm volatile("int 0x0");
  enable_interrupt();
  for (;;)
    ;
#else
  //	init_shell();
  //  init_uart();
  //	init_console();
  init_tty();
  //  tty_set_textcolor(TTY_ID, TEXTCOLOR_LTGRAY, TEXTCOLOR_BLACK);
  tty_clear(TTY_ID);

  //	init_key();
  //	init_processor();

  //	init_buffer();

  //	init_ipc();
	/* use printk after init_ipc() */

  //	init_bss();

  //	init_overhead();

  init_irq();
  init_dsctbl();

  //  timer_init(20);
  //init_keyboard();

  //	init_apic();
  init_pit_timer(TICK_USEC);
  //	init_cache(); 

	/* enable interrupt */
	sti();
  start_pit_timer(0);
  for (;;)
    ;
#endif
}


void exit_arch(void)
{
}
