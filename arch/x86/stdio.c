/**
 * @file arch/x86/stdio.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

volatile atomic_int putchar_lock = SPIN_UNLOCKED;


#if CONFIG_PRINT2CONSOLE
int getchar(void)
{
	return 0;
}

int putchar(int c)
{
  unsigned long flags;
  save_local_irq(&flags);
  spin_lock(&putchar_lock);
  tty_printc(TTY_ID, c);
  spin_unlock(&putchar_lock);
  restore_local_irq(&flags);
  return c;
}

#elif CONFIG_PRINT2UART
int getchar(void)
{
	return uart_getc(STDIO_PORT);
}
int putchar(int c)
{
  unsigned long flags;
  save_local_irq(&flags);
  spin_lock(&putchar_lock);
  uart_putc(c, STDIO_PORT);
  spin_unlock(&putchar_lock);
  restore_local_irq(&flags);
  return c;
}

#else
#error "Unknown Print to Output"
#endif


int puts(const char *s)
{
	int i;
	int len = strlen(s);
	for (i = 0; i < len; i++) {
		putchar(s[i]);
	}
  return len;
}

