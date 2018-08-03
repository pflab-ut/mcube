/**
 * @file drivers/uart/uart_arm.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

#if PL011_UART

/* PL011 UART in Raspberry Pi3 and SynQuacer */

uint8_t uart_pol_getc(uint8_t ch)
{
  // Wait for UART to have received something.
  while (mmio_in64(UART0_FLAG_REG) & UART_FLAG_REG_RECEIVE_FIFO_EMPTY)
    ;
  return mmio_in64(UART0_DATA_REG) & UART_DATA_REG_DATA_MASK;
}

void uart_pol_putc(uint8_t c, int32_t ch)
{
  // Wait for UART to become ready to transmit.
  while (mmio_in64(UART0_FLAG_REG) & UART_FLAG_REG_TRANSMIT_FIFO_FULL)
    ;
  mmio_out64(UART0_DATA_REG, c & UART_DATA_REG_DATA_MASK);
}

void init_uart(void)
{
  /* enable uart */
  mmio_out32(UART0_CTRL_REG,
             mmio_in32(UART0_CTRL_REG) | UART_CTRL_REG_UART_ENABLE);
  /* enable UART RX interrupt */
  mmio_out32(UART0_INTERRUPT_MASK_SET_CLEAR_REG, 1 << 4);
#if CONFIG_ARCH_ARM_RASPI3
  /* set UART interrupt routing */
  mmio_out32(ENABLE_IRQS2, 1 << 25);
#elif CONFIG_ARCH_ARM_SYNQUACER
  /* TODO: implement */
#else
#error "Unknown Machine"
#endif /* CONFIG_ARCH_ARM_RASPI3 */
}

#elif MINI_UART

/* Mini UART in Raspberry Pi3 */

uint8_t uart_pol_getc(uint8_t ch)
{
  while (mmio_in32(IRQ_PENDING1) & (1 << 29))
    ;
  return mmio_in8(AUX_MINI_UART_IO_REG);
}

void uart_pol_putc(uint8_t c, int32_t ch)
{
  while (!(mmio_in8(AUX_MINI_UART_LSR_REG) & (1 << 5)))
    ;
  mmio_out8(AUX_MINI_UART_IO_REG, c);
}


void init_uart(void)
{
  /* enable mini UART */
  mmio_out32(AUX_ENABLES, 1);

  /* clear transmit/receive FIFO */
  mmio_out32(AUX_MINI_UART_IIR_REG, 6);

  /* enable transmit interrupt */
  mmio_out32(AUX_MINI_UART_IER_REG, 2);

  /* set UART interrupt routing */
  mmio_out32(ENABLE_IRQS1, 1 << 29);
}

#else
#error "Unknown UART"
#endif


