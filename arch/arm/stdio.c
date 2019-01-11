/**
 * @file arch/arm/stdio.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

static volatile atomic_int putchar_lock = MCUBE_SPIN_UNLOCKED;

int putchar(int c)
{
  unsigned long flags;
  save_local_irq(&flags);
  spin_lock(&putchar_lock);
  uart_putc(c, 0);
  spin_unlock(&putchar_lock);
  restore_local_irq(&flags);
  return c;
}

int puts(const char *s)
{
	int i;
	int len = strlen(s);
	for (i = 0; i < len; i++) {
		putchar(s[i]);
	}
  return 0;
}

