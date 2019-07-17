/**
 * @file drivers/uart/uart_x86_64.c
 *
 * @author Hiroyuki Chishiro
 */
/*
 * Serial Port, 8250/16550 UART
 *
 * Copyright (C) 2010 Ahmed S. Darwish <darwish.07@gmail.com>
 *
 * UART is the chip that picks a (parallel) byte and send it one bit at
 * a time, sequentially, over the serial line and vice versa. Check our
 * references repository for datasheets of 16550-compatible UARTS.
 *
 * NOTE! My laptop is void of serial ports, making that state the only
 * real use-case tested. Transmitting bytes was only tested using Qemu
 * and Bochs virtual ports.
 */

#include <mcube/mcube.h>

/**
 * @var port_base
 * @brief Port base.
 */
static uint16_t port_base;

/**
 * @fn static void reset_port_set_8n1_mode(void)
 * @brief reset port set 8 and 1 mode.
 */
static void reset_port_set_8n1_mode(void)
{
  union line_control_reg reg;

  reg.raw = 0;
  reg.stop_bit = STOP_BIT_1;
  reg.data_len = DATA_LEN_8;
  outb(reg.raw, port_base + UART_LINE_CTRL);
}

/**
 * @fn static void enable_DLAB(void)
 * @brief enable DLAB.
 */
static void enable_DLAB(void)
{
  union line_control_reg reg;

  reg.raw = inb(port_base + UART_LINE_CTRL);
  reg.DLAB = 1;
  outb(reg.raw, port_base + UART_LINE_CTRL);
}

/**
 * @fn static void disable_DLAB(void)
 * @brief disable DLAB.
 */
static void disable_DLAB(void)
{
  union line_control_reg reg;

  reg.raw = inb(port_base + UART_LINE_CTRL);
  reg.DLAB = 0;
  outb(reg.raw, port_base + UART_LINE_CTRL);
}

void init_uart(void)
{
  union modem_control_reg reg;
  uint16_t divisor = MAX_BAUD / DESIRED_BAUD;
  assert(divisor <= UINT16_MAX);

  if ((port_base = *(uint16_t *) VIRTUAL(BDA_COM1_ENTRY)) == 0) {
    printk("COM1: BIOS-reported I/O address = 0; "
           "no uart port attached\n");
    return;
  }

  printk("COM1: BIOS-reported I/O address = 0x%x\n", port_base);
  reset_port_set_8n1_mode();
  outb(0x00, port_base + UART_INTERRUPT_ENABLE);

  enable_DLAB();
  outb(divisor & 0xff, port_base + UART_DIVISOR_LATCH_LOW);
  outb(divisor >> 8, port_base + UART_DIVISOR_LATCH_HIGH);
  disable_DLAB();

  outb(0x00, port_base + UART_FIFO_CTRL);
  reg.raw = 0;
  reg.dt_ready = 1;
  reg.req_send = 1;
  outb(reg.raw, port_base + UART_MODEM_CTRL);
}

/**
 * @fn static bool tx_buffer_empty(void)
 * @brief Is transmission buffer empty?
 *
 * @return True if transmission buffer is empty.
 */
static bool tx_buffer_empty(void)
{
  union line_status_reg reg;

  reg.raw = inb(port_base + UART_LINE_STATUS);

  return reg.tx_empty;
}

/**
 * @fn static bool remote_ready(void)
 * @brief Is remote ready?
 *
 * @return True if remote is ready.
 */
static bool remote_ready(void)
{
  union modem_status_reg reg;

  /* Safest approach is to poll the DSR and CTS
   * status lines for 2ms till both are high */
  reg.raw = inb(port_base + UART_MODEM_STATUS);

  return reg.remote_ready && reg.clr_to_send;
}

/**
 * @var port_is_broken
 * @brief Is port broken?
 */
static int port_is_broken = false;

spinlock_t port_lock;

/**
 * @fn static int __putc(uint8_t byte)
 * @brief putchar.
 *
 * @param byte Byte.
 * @return Zero if remote is not ready or success, and nonzero if failure.
 */
static int __putc(uint8_t byte)
{
  int timeout;

  /* Next byte may have a better luck */
  if (!remote_ready()) {
    return 0;
  }

  timeout = 0xfffff;

  while (!tx_buffer_empty() && timeout--) {
    pause();
  }

  if (__unlikely(timeout == -1)) {
    port_is_broken = true;
    return 1;
  }

  outb(byte, port_base + UART_TRANSMIT_BUF);
  return 0;
}

size_t uart_write(const char *data, size_t length, uint8_t ch)
{
  int ret = 0;

  if (ch != 0 || port_base == 0) {
    return 0;
  }

  spin_lock(&port_lock);

  if (port_is_broken) {
    goto out;
  }

  while (*data && length-- && ret == 0) {
    ret = __putc(*data++);
  }

out:
  spin_unlock(&port_lock);
  return ret;
}

void uart_putc(char c, uint8_t ch)
{
  uart_write(&c, 1, ch);
}

