/**
 * @file lib/spinlock.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


spinlock_t putchar_lock;

void init_spinlock(void)
{
  init_spin(&pbuf_lock);
  init_spin(&sbuf_lock);
  init_spin(&kmalloc_lock);
  init_spin(&putchar_lock);
  init_spin(&sched_lock);
#if !CONFIG_ARCH_SIM
  init_spin(&socket_lock);
#endif /* !CONFIG_ARCH_SIM */
  
#if CONFIG_ARCH_X86_64
  init_spin(&panic_lock);
  init_spin(&vga_lock);
  init_spin(&printstats_lock);
  init_spin(&port_lock);
#endif /* CONFIG_ARCH_X86_64 */
}
