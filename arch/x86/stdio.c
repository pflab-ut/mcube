/**
 * @file arch/x86/stdio.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

#define TAB_SPACE 2


struct cursor_struct cursor = {
	.x = 0, .y = 0};


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
		*((uint16_t *) i) = *((uint16_t *) (i + CUI_CONSOLE_WIDTH * 2));
	}
	/* fill blank in the last line */
	for (; i < CUI_VRAM_START + CUI_CONSOLE_WIDTH * CUI_CONSOLE_HEIGHT * 2; i += 2) {
		*((uint16_t *) i) = (COL8_000000 << 8) | 0x00;
	}
}


void init_console(void)
{
	int i;
	for (i = CUI_VRAM_START; i < CUI_VRAM_START + CUI_CONSOLE_WIDTH * CUI_CONSOLE_HEIGHT * 2; i += 2) {
		*((uint16_t *) i) = (COL8_848484 << 8) | 0x00;
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
		*((uint16_t *) CUI_VRAM_ADDR(cursor.x, cursor.y)) = (uint16_t) ((COL8_848484 << 8) | c);
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
	int i;
	for (i = 0; i < length; i++) {
		putchar(((uint8_t *) input_data)[i]);
	}
	return 0;
}


void do_fifo32(void)
{
	uint32_t data;
	int keycmd_wait = -1;
	uint32_t key_shift = 0;
	uint32_t key_leds = (*((uint8_t *) LEDS_ADDR) >> 4) & 7;
	char s[FIFO32_BUFSIZE];

	print_shell();
	while (1) {
		if (get_fifo32_status(&keycmd) > 0 && keycmd_wait < 0) {
			keycmd_wait = get_fifo32(&keycmd);
			wait_KBC();
			outb(keycmd_wait, PORT_KEYDATA);
		}
		//cli();
		if (get_fifo32_status(&fifo32) == 0) {
		} else {
			data = get_fifo32(&fifo32);
			//sti();
			if (KEYDATA_OFFSET <= data && data < KEYDATA_OFFSET + NR_KEYS) {
				if (data < KEYTABLE_SIZE + KEYDATA_OFFSET) {
					if (key_shift == 0) {
						s[0] = keytable0[data-KEYDATA_OFFSET];
					} else {
						s[0] = keytable1[data-KEYDATA_OFFSET];
					}
				} else {
					s[0] = 0;
				}
				if ('A' <= s[0] && s[0] <= 'Z') {
					if (((key_leds & CAPS_LOCK) == 0 && key_shift == 0) ||
							((key_leds & CAPS_LOCK) != 0 && key_shift != 0)) {
						/* upper to lower */
						s[0] += 0x20;
					}
				}
				if (s[0] != 0) {
					//					printk("data = %x %c\n", s[0] - KEYDATA_OFFSET, s[0] - KEYDATA_OFFSET);
					printk("%c", s[0] - KEYDATA_OFFSET);
					// output data to shell's buffer
					//					put_fifo32(s[0] + KEYDATA_OFFSET, &keyfifo);
				}
				switch(data) {
				case KEYDATA_OFFSET + 0x2a:
					key_shift |= LEFT_SHIFT;
					break;
				case KEYDATA_OFFSET + 0x36:
					key_shift |= RIGHT_SHIFT;
					break;
				case KEYDATA_OFFSET + 0xaa:
					key_shift &= ~LEFT_SHIFT;
					break;
				case KEYDATA_OFFSET + 0xb6:
					key_shift &= ~RIGHT_SHIFT;
					break;
				case KEYDATA_OFFSET + 0x3a:
					key_leds ^= CAPS_LOCK;
					put_fifo32(KEYCMD_LED, &keycmd);
					put_fifo32(key_leds, &keycmd);
					break;
				case KEYDATA_OFFSET + 0x45:
					key_leds ^= NUM_LOCK;
					put_fifo32(KEYCMD_LED, &keycmd);
					put_fifo32(key_leds, &keycmd);
					break;
				case KEYDATA_OFFSET + 0xfa:
					keycmd_wait = -1;
					break;
				case KEYDATA_OFFSET + 0xfe:
					wait_KBC();
					outb(keycmd_wait, PORT_KEYDATA);
					break;
				}
			}
		}
		halt();
	}	
}

