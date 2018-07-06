/**
 * @file arch/x86/key.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

fifo32_struct keycmd;
uint32_t keycmd_buf[KEYCMD_BUFSIZE];

fifo32_struct keyfifo;
uint32_t keyfifo_buf[KEYFIFO_BUFSIZE];


char keytable0[KEYTABLE_SIZE] = {
	0,  0,  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '^', 0x08, 0,
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '@', '[', 0x0a, 0, 'A', 'S',
	'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', ':', 0,  0,  ']', 'Z', 'X', 'C', 'V',
	'B', 'N', 'M', ',', '.', '/', 0,  '*', 0,  ' ', 0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  '7', '8', '9', '-', '4', '5', '6', '+', '1',
	'2', '3', '0', '.', 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0x5c, 0, 0,  0,  0,  0,  0,  0,  0,  0,  0x5c, 0, 0
};

char keytable1[KEYTABLE_SIZE] = {
	0,  0,  '!', 0x22, '#', '$', '%', '&', 0x27, '(', ')', '~', '=', '~', 0x08, 0,
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '`', '{', 0x0a, 0, 'A', 'S',
	'D', 'F', 'G', 'H', 'J', 'K', 'L', '+', '*', 0,  0,  '}', 'Z', 'X', 'C', 'V',
	'B', 'N', 'M', '<', '>', '?', 0,  '*', 0,  ' ', 0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  '7', '8', '9', '-', '4', '5', '6', '+', '1',
	'2', '3', '0', '.', 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  '_', 0,  0,  0,  0,  0,  0,  0,  0,  0,  '|', 0,  0
};


void wait_KBC(void)
{
	while (inb(PORT_KEYSTATUS) & KEYSTATUS_SEND_NOT_READY)
		;
}


void set_cursor(uint16_t cur_pos)
{
	outb(15, 0x03d4);
	outb(cur_pos, 0x03d5);
	outb(14, 0x03d4);
	outb(cur_pos >> 8, 0x03d5);
}


irqreturn_t handle_key(int irq, void *dummy)
{
	uint8_t data;
	//	printk("handle_key()\n");
	/* read data from port */
	data = inb(PORT_KEYDATA);

#if 1
	//	put_fifo32(data, &keycmd);
	put_fifo32(data + KEYDATA_OFFSET, &fifo32);
#else
	/* check make or break */
	switch (IS_BREAK(data)) {
	case MAKE:
		printk("key data = %x\n", data);
		break;
	case BREAK:
		break;
	default:
		printk("Unexpected bit %d\n", IS_BREAK(data));
		break;
	}
#endif

	outb(clear_irq(KEY_IRQ), PIC0_OCW2);
	return 0;
}

static struct irqaction key_irq = {
  .name       = "key",
  .handler    = handle_key,
  .flags      = IRQF_DISABLED | IRQF_TIMER,
  .mask       = CPU_MASK_NONE,
};

void enable_key(void)
{
	outb(inb(PIC0_IMR) & unmask_irq(KEY_IRQ), PIC0_IMR);
}

void disable_key(void)
{
	outb(inb(PIC0_IMR) | mask_irq(KEY_IRQ), PIC0_IMR);
}

void init_key(void)
{
	init_fifo32(&keycmd, KEYCMD_BUFSIZE, keycmd_buf);
  init_fifo32(&keyfifo, KEYFIFO_BUFSIZE, keyfifo_buf);
	disable_key();
#if 0
	set_idsc(idt_start + IRQ_OFFSET + KEY_IRQ, (uint32_t) &common_interrupt,
					 2 * 8, AR_INTGATE32);
#endif
	setup_irq(KEY_IRQ, &key_irq);
	enable_key();
#if CONFIG_SCREEN_CUI
	//	set_cursor(0);
#endif /* CONFIG_SCREEN_CUI */
}

