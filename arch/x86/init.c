/**
 * @file arch/x86/init.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


void init_arch(void)
{
  //	init_shell();
  init_uart();
	init_console();

	init_key();
  //	init_processor();

	init_buffer();

  //	init_ipc();
	/* use printk after init_ipc() */

  //	init_bss();

  //	init_overhead();

	init_dsctbl();
	init_IRQ();

#if CONFIG_DEV_MOUSE
	init_mouse();
#endif /* CONFIG_DEV_MOUSE */

	init_apic();
  init_timer(0);

#if CONFIG_ENABLE_CACHE
	init_cache(); 
#endif /* CONFIG_ENABLE_CACHE */

	/* enable interrupt */
	sti();
}


void exit_arch(void)
{
}
