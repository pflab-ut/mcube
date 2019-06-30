/**
 * @file arch/aarch64/stdio.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

spinlock_t putchar_lock = INIT_SPINLOCK;

int putchar(int c)
{
  union rflags flags;
  save_local_irq(&flags);
  spin_lock(&putchar_lock);
  uart_putc(c, 0);
  spin_unlock(&putchar_lock);
  restore_local_irq(&flags);
  return c;
}

