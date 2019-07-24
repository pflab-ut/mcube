/**
 * @file arch/x86_64/pic.c
 *
 * @author Hiroyuki Chishiro
 */
/*
 * The i8259A PIC
 *
 * Copyright (C) 2009 Ahmed S. Darwish <darwish.07@gmail.com>
 *
 * We exlusively use the I/O APICs for interrupt control. The PIC is
 * just a disturbance to be completely masked and ignored afterwards.
 *
 * Check Intel's "8259A Programmable Interrupt Controller" datasheet
 * for more details.
 */

#include <mcube/mcube.h>


/**
 * @fn static inline void outb_pic(uint8_t val, uint16_t port)
 * As said in a linux kernel comment, delay for access to PIC on
 * motherboard or in chipset must be at least one microsecnod.
 * FIXME: use a time-based delay function once it's ready.
 *
 * @param val Value.
 * @param port Port.
 */
static inline void outb_pic(uint8_t val, uint16_t port)
{
  outb(val, port);
  io_delay();
}

/**
 * @fn static inline void i8259_mask(void)
 * @brief mask the now-obsolete 8259A PICs by setting all the bits
 * of the Interrupt Mask Register.
 */
static inline void i8259_mask(void)
{
  outb_pic(0xff, PIC_PORT_DATA_MASTER);   /* IMR, OCW1 master */
  outb_pic(0xff, PIC_PORT_DATA_SLAVE);    /* IMR, OCW1 slave */
}

/*
 * The PICs will be entirely masked. Map the IRQs in case of a
 * triggered 'spurious' interrupt.
 *
 * Unfortunately spuruious PIC interrupts do occur even if the
 * PIC is entirely masked. Thus, we remap the chips away from
 * IBM programmed reserved Intel exception numbers 0x8-0xf to
 * saner values at PIC_IRQ0_VECTOR and mask it afterwards.
 *
 * Those spurious interrupts do occur for an important reason:
 * sometimes electrical noise makes the CPU INTR pin go high,
 * so the CPU waits for the vector number from the PIC (using
 * the i8259A cycle). The PIC does not know why the INTR line
 * was high, but it has to reply with  __something__, or the
 * entire CPU will halt! That's also why spurious PIC IRQs can
 * NOT get masked.  -- http://goo.gl/BMGXl
 */
void pic_init(void)
{
  /* Init command 1, cascade mode (D1 = 0), init mode
   * (D4 = 1), requires init command 4 (D0 = 1), other
   * bits useless in AT 80x86 mode */
  outb_pic(0x11, PIC_PORT_CMD_MASTER);
  outb_pic(0x11, PIC_PORT_CMD_SLAVE);

  /* Init command 2, set the most significant five bits
   * of the vectoring byte. The PIC sets the least three
   * bits accoding to the interrupt level */
  outb_pic(PIC_IRQ0_VECTOR, PIC_PORT_DATA_MASTER);
  outb_pic(PIC_IRQ8_VECTOR, PIC_PORT_DATA_SLAVE);

  /* Init command 3, in master mode, a "1" is set for each
   * slave in the system. Through the cascade lines, the
   * master will enable the relevant slave chip to send
   * its vectoring data.
   *
   * In slave mode, bits 2-0 identifies the slave. Slave
   * compares its cascade input with those bits, and if
   * they're equal, it releases the vectoring data to the
   * data bus */
  outb_pic(1 << SLAVE_IRQ, PIC_PORT_DATA_MASTER);
  outb_pic(SLAVE_IRQ, PIC_PORT_DATA_SLAVE);

  /* Init command 4, 80x86 mode (D0 = 1), Automatic EOI
   * (D1 = 1), nonbuffered (D3 = 0) */
  outb_pic(0x3, PIC_PORT_DATA_MASTER);
  outb_pic(0x3, PIC_PORT_DATA_SLAVE);

  /* FIXME: wait for the chip to initialize */

  i8259_mask();

  /* Now assure that any misbheaving IRQ that get triggered
   * by the PIC, despite of its masked status, get ignored */

  for (int i = PIC_IRQ0_VECTOR; i <= PIC_IRQ7_VECTOR; i++) {
    set_idt_gate(i, (void *) PIC_handler);
  }

  for (int i = PIC_IRQ8_VECTOR; i <= PIC_IRQ15_VECTOR; i++) {
    set_idt_gate(i, (void *) PIC_handler);
  }
}
