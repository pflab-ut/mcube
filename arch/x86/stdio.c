/**
 * @file arch/x86/stdio.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


#if CONFIG_PRINTK2CONSOLE
int getchar(void)
{
	while (1) {
		halt();
		printk("a");
		/* break if getting data */
	}
	return 0;
}
#elif CONFIG_PRINTK2UART
int getchar(void)
{
	return uart_getc(STDIO_PORT);
}
#else
#error "Unknown Printk to Output"
#endif


#if CONFIG_PRINTK2CONSOLE


void nextc(void)
{
	cursor.x++;
	if (cursor.x == CUI_CONSOLE_WIDTH) {
		cursor.x = 0;
		cursor.y++;
	}
}

void do_scroll(void)
{
	int i;
	/* set cursor to the first position of last line */
	cursor.x = 0;
	cursor.y--;
	/* do scroll */
	for (i = CUI_VRAM_START; i < CUI_VRAM_START + CUI_CONSOLE_WIDTH * (CUI_CONSOLE_HEIGHT - 1) * 2; i += 2) {
    //		*((uint16_t *) i) = *((uint16_t *) (i + CUI_CONSOLE_WIDTH * 2));
		mmio_out16(i, mmio_in16(i + CUI_CONSOLE_WIDTH * 2));
	}
	/* fill blank in the last line */
	for (; i < CUI_VRAM_START + CUI_CONSOLE_WIDTH * CUI_CONSOLE_HEIGHT * 2; i += 2) {
    //		*((uint16_t *) i) = (COL8_000000 << 8) | 0x00;
		mmio_out16(i, (COL8_000000 << 8) | 0x00);
	}
}


void init_console(void)
{
	unsigned long i;
	for (i = CUI_VRAM_START; i < CUI_VRAM_START + CUI_CONSOLE_WIDTH * CUI_CONSOLE_HEIGHT * 2; i += 2) {
    //		*((uint16_t *) i) = (COL8_848484 << 8) | 0x00;
		mmio_out16(i, (COL8_848484 << 8) | 0x00);
		//		*((uint16_t *) i) = (COL8_FF0000 << 8) | 0x00;
	}
	cursor.x = 0;
	cursor.y = 0;
}


int putchar(int c)
{	
	int i;
	switch (c) {
	case CTRL_I: /* \t */
		for (i = 0; i < TAB_SPACE; i++) {
			nextc();
		}
		break;
	case CTRL_J: /* \n */
		cursor.x = 0;
		cursor.y++;
		break;
	default:
    //		*((uint16_t *) CUI_VRAM_ADDR(cursor.x, cursor.y)) = (uint16_t) ((COL8_848484 << 8) | c);
		mmio_out16(CUI_VRAM_ADDR(cursor.x, cursor.y), (uint16_t) ((COL8_848484 << 8) | c));
		//		*((uint16_t *) cui_vram_addr(&cons)) = (uint16_t) ((COL8_848484 << 8) | c);
		nextc();
		break;
	}
	if (cursor.y == CUI_CONSOLE_HEIGHT) {
		do_scroll();
	}
	set_cursor(cursor.x + CUI_CONSOLE_WIDTH * cursor.y);	
	//	inf_loop();
	//	*((uint16_t *) CUI_VRAM_ADDR(cursor.x, cursor.y)) = (uint16_t) ((COL8_848484 << 8) | (c + 2));
	//set_cursor(cursor.x);
	//	set_cursor(cursor.y);
  return c;
}



#elif CONFIG_PRINTK2UART
int putchar(int c)
{
	uart_putc((uint8_t) c, STDIO_PORT);
  return c;
}

void init_console(void)
{
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


