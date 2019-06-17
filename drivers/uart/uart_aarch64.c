/**
 * @file drivers/uart/uart_aarch64.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

#if PL011_UART

/* PL011 UART in Raspberry Pi3 */

uint8_t uart_pol_getc(__unused uint32_t ch)
{
  // Wait for UART to have received something.
  while (mmio_in64(UART0_FLAG_REG) & UART_FLAG_REG_RECEIVE_FIFO_EMPTY)
    ;

  return mmio_in64(UART0_DATA_REG) & UART_DATA_REG_DATA_MASK;
}

void uart_pol_putc(uint8_t c, __unused uint32_t ch)
{
  // Wait for UART to become ready to transmit.
  while (mmio_in64(UART0_FLAG_REG) & UART_FLAG_REG_TRANSMIT_FIFO_FULL)
    ;

  mmio_out64(UART0_DATA_REG, c & UART_DATA_REG_DATA_MASK);
}

void init_uart(void)
{

#if CONFIG_ARCH_AARCH64_RASPI3
  const unsigned int dcount = 150;
  /* get GPFSEL1 to selector */
  uint32_t selector;

  /* disable uart */
  mmio_out32(UART0_CTRL_REG, 0);

  /* setup pl011 uart by mailbox */
  setup_pl011_uart();

  selector = mmio_in32(GPFSEL1);
  /* clear gpio14 */
  selector &= ~GPFSEL_FSELn4_MASK;
  /* set alt0 for gpio14 */
  selector |= GPIO_PINn_ALT0 << GPFSEL_FSELn4_SHIFT;
  /* clear gpio15 */
  selector &= ~GPFSEL_FSELn5_MASK;
  /* set alt0 for gpio15 */
  selector |= GPIO_PINn_ALT0 << GPFSEL_FSELn5_SHIFT;

  /* set GPFSEL1 as selector */
  mmio_out32(GPFSEL1, selector);

  /* 1. Write to GPPUD to set the required control signal
   * (i.e. Pull-up or Pull-Down or neither to remove the current Pull-up/down) */
  /* This implementation disables pull-up and pull-down, and hence 5 is not needed. */
  mmio_out32(GPPUD, GPPUD_DISABLE_PULL_UP_DOWN);

  /* 2. Wait 150 cycles. This provides the required set-up time for the control signal */
  delay_cycles(dcount);

  /* 3. Write to GPPUDCLK0/1 to clock the control signal into the GPIO pads you wish to
   * modify.
   * NOTE only the pads which receive a clock will be modified, all others will
   * retain their previous state.
   */
  /* assert clock on lines 14 and 15 */
  mmio_out32(GPPUDCLK0, GPPUDCLKn_SET(14) | GPPUDCLKn_SET(15));

  /* 4. Wait 150 cycles. This provides the required set-up time for the control signal */
  delay_cycles(dcount);

  /* 5. Write to GPPUD to remove the control signal. */
  /* Nothing */

  /* 6. Write to GPPUDCLK0/1 to remove the clock */
  mmio_out32(GPPUDCLK0, 0);

  /* clear interrupt */
  mmio_out32(UART0_ICR_REG, 0x7ff);

  /* set UART interrupt routing */
  mmio_out32(ENABLE_IRQS2, IRQ_PENDINGn_SRC(PL011_UART_IRQ));
#else
#error "Unknown Machine"
#endif /* CONFIG_ARCH_AARCH64_RASPI3 */

  /* enable UART RX interrupt */
  mmio_out32(UART0_IMSC_REG, UART_IMSC_REG_RECEIVE_INTERRUPT_MASK);
  /* enable uart */
  mmio_out32(UART0_CTRL_REG,
             mmio_in32(UART0_CTRL_REG) | UART_CTRL_REG_UART_ENABLE);

}

#elif MINI_UART

/* Mini UART in Raspberry Pi3 */

uint8_t uart_pol_getc(__unused uint32_t ch)
{
  while (mmio_in32(IRQ_PENDING1) & IRQ_PENDINGn_SRC(MINI_UART_IRQ))
    ;

  return mmio_in8(AUX_MU_IO_REG);
}

void uart_pol_putc(uint8_t c, __unused uint32_t ch)
{
  while (!(mmio_in8(AUX_MU_LSR_REG) & AUX_MU_LSR_REG_TRANSMITTER_EMPTY))
    ;

  mmio_out8(AUX_MU_IO_REG, c);
}


void init_uart(void)
{
  const unsigned int dcount = 150;
  uint32_t selector = mmio_in32(GPFSEL1);

  /* clear gpio14 */
  selector &= ~GPFSEL_FSELn4_MASK;
  /* set alt5 for gpio14 */
  selector |= GPIO_PINn_ALT5 << GPFSEL_FSELn4_SHIFT;
  /* clear gpio15 */
  selector &= ~GPFSEL_FSELn5_MASK;
  /* set alt5 for gpio15 */
  selector |= GPIO_PINn_ALT5 << GPFSEL_FSELn5_SHIFT;

  mmio_out32(GPFSEL1, selector);

  /* 1. Write to GPPUD to set the required control signal
   * (i.e. Pull-up or Pull-Down or neither to remove the current Pull-up/down) */
  /* This implementation disables pull-up and pull-down, and hence 5 is not needed. */
  mmio_out32(GPPUD, GPPUD_DISABLE_PULL_UP_DOWN);

  /* 2. Wait 150 cycles. This provides the required set-up time for the control signal */
  delay_cycles(dcount);

  /* 3. Write to GPPUDCLK0/1 to clock the control signal into the GPIO pads you wish to
   * modify.
   * NOTE only the pads which receive a clock will be modified, all others will
   * retain their previous state.
   */
  /* assert clock on lines 14 and 15 */
  mmio_out32(GPPUDCLK0, GPPUDCLKn_SET(14) | GPPUDCLKn_SET(15));

  /* 4. Wait 150 cycles. This provides the required set-up time for the control signal */
  delay_cycles(dcount);

  /* 5. Write to GPPUD to remove the control signal. */
  /* Nothing */

  /* 6. Write to GPPUDCLK0/1 to remove the clock */
  mmio_out32(GPPUDCLK0, 0);

  /* enable mini UART */
  mmio_out32(AUX_ENABLES, AUX_ENABLES_MINI_UART_ENABLE);

  /* disable auto flow control and disable receiver and transmitter (for now) */
  mmio_out32(AUX_MU_CTRL_REG, 0);

  /* disable receive and transmit interrupts */
  mmio_out32(AUX_MU_IER_REG, 0);

  /* enable 8 bit mode */
  mmio_out32(AUX_MU_LCR_REG,
             mmio_in32(AUX_MU_LCR_REG) | AUX_MU_LCR_REG_8BIT_MODE);

  /* set RTS line to be always high */
  mmio_out32(AUX_MU_MCR_REG, 0);

  /* TODO: find calculation method. */
  /* set baudrate to 115200 */
  mmio_out32(AUX_MU_BAUD_REG, 270);

  /* Finally, enable transmitter and receiver */
  mmio_out32(AUX_MU_CTRL_REG,
             mmio_in32(AUX_MU_CTRL_REG)
             | AUX_MU_CTRL_REG_TRANSMITTER_ENABLE
             | AUX_MU_CTRL_REG_RECEIVER_ENABLE);

  /* clear transmit/receive FIFOs */
  mmio_out32(AUX_MU_IIR_REG,
             AUX_MU_IIR_REG_CLEAR_TRANSMIT_FIFO
             | AUX_MU_IIR_REG_CLEAR_RECV_FIFO);

  /* enable receive interrupt */
  mmio_out32(AUX_MU_IER_REG, AUX_MU_IER_REG_ENABLE_RECV_INTERRUPTS);

  /* set UART interrupt routing */
  mmio_out32(ENABLE_IRQS1, IRQ_PENDINGn_SRC(MINI_UART_IRQ));

}

#else
#error "Unknown UART"
#endif

