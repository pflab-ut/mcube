/**
 * @file arch/x86/stdio.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


#if CONFIG_PRINTK2CONSOLE
int getchar(void)
{
	return 0;
}

int putchar(int c)
{
  tty_printc(TTY_ID, c);
}

#elif CONFIG_PRINTK2UART
int getchar(void)
{
	return uart_getc(STDIO_PORT);
}
#else
#error "Unknown Printk to Output"
#endif



int puts(const char *s)
{
	int i;
	int len = strlen(s);
	for (i = 0; i < len; i++) {
		putchar(s[i]);
	}
  return 0;
}

ssize_t console_write(const void *input_data, size_t length, void *devdata)
{
#if 1
  tty_print(0, input_data);
#else
	int i;
	for (i = 0; i < length; i++) {
		putchar(((uint8_t *) input_data)[i]);
	}
#endif
	return 0;
}


