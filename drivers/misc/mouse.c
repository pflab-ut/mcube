/**
 * @file drivers/misc/mouse.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

uint32_t mousebuf[MOUSE_BUFSIZE];

irqreturn_t handle_mouse(int irq, void *dummy)
{
	unsigned char data;
	data = mmio_in8(PORT_KEYDATA);
	print("mouse data = %x\n", data);

	mmio_out8(PIC0_OCW2, clear_irq(SLAVE_IRQ));
	mmio_out8(PIC1_OCW2, clear_irq(MOUSE_SLAVE_IRQ));
	return 0;
}

static struct irqaction mouse_irq = {
  .name       = "mouse",
  .handler    = handle_mouse,
  .flags      = IRQF_DISABLED | IRQF_TIMER,
  .mask       = CPU_MASK_NONE,
};


void enable_mouse(void)
{
	mmio_out8(PIC0_IMR, mmio_in8(PIC0_IMR) & unmask_irq(SLAVE_IRQ));
	mmio_out8(PIC1_IMR, mmio_in8(PIC1_IMR) & unmask_irq(MOUSE_SLAVE_IRQ));
}

void disable_mouse(void)
{
	mmio_out8(PIC1_IMR, mmio_in8(PIC1_IMR) | mask_irq(MOUSE_SLAVE_IRQ));
	/* disable all slave IRQs? */
	if (mmio_in8(PIC1_IMR) == DISABLE_ALL_IRQS) {
		mmio_out8(PIC0_IMR, mmio_in8(PIC0_IMR) | mask_irq(SLAVE_IRQ));
	}
}

void init_mouse(void)
{
	disable_mouse();
	wait_KBC();
	mmio_out8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
	wait_KBC();
	mmio_out8(PORT_KEYDATA, KBC_MODE);

	wait_KBC();
	mmio_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
	wait_KBC();
	mmio_out8(PORT_KEYDATA, MOUSECMD_ENABLE);

	set_idsc(idt_start + IRQ_OFFSET + MOUSE_IRQ, (uint32_t) &common_interrupt,
					 2 * 8, AR_INTGATE32);
	setup_irq(MOUSE_IRQ, &mouse_irq);
	enable_mouse();
}
