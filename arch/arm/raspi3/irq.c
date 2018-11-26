/**
 * @file arch/arm/raspi3/irq.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


int handle_uart_interrupt(void)
{
  int c;
  uint32_t irq_bp = mmio_in32(IRQ_BASIC_PENDING);
#if PL011_UART
  uint32_t irq_p2 = mmio_in32(IRQ_PENDING2);
  // uart
  if ((irq_bp & IRQ_BASIC_PENDING_REG2)
      && (irq_p2 & IRQ_PENDINGn_SRC(PL011_UART_IRQ))
      && (mmio_in32(UART0_MIS_REG)
          & UART_MIS_REG_RECEIVE_MASKED_INTERRUPT_STATUS)) {
    c = (unsigned char) mmio_in32(UART0_DATA_REG); // read for clear tx interrupt.
    uart_putc(c, 0);
    printk("handle_uart_interrupt(): uart\n");
    return IRQ_HANDLED;
  }
#elif MINI_UART
  uint32_t irq_p1 = mmio_in32(IRQ_PENDING1);
  // mini uart
  if ((irq_bp & IRQ_BASIC_PENDING_REG1)
      && (irq_p1 & IRQ_PENDINGn_SRC(MINI_UART_IRQ))) {
    c = (unsigned char) mmio_in32(AUX_MU_IO_REG); // read for clear tx interrupt.
    uart_putc(c, 0);
    printk("handle_uart_interrupt(): mini uart\n");
    return IRQ_HANDLED;
  }
#else
#error "Unknown UART"
#endif
  return IRQ_UNHANDLED;
}

int handle_dmac_interrupt(void)
{
  unsigned long cpu = get_cpu_id();
  printk("handle_dmac_interrupt()\n");
  if ((mmio_in32(IRQ_BASIC_PENDING) & IRQ_BASIC_PENDING_REG1)
      && (mmio_in32(IRQ_PENDING1) & DMAC_IRQ(cpu))
      && (mmio_in32(DMAC_CH_CS(cpu)) & DMAC_CH_CS_INT)) {
    mmio_out32(DMAC_CH_CS(cpu), mmio_in32(DMAC_CH_CS(cpu)) | DMAC_CH_CS_INT);
    return IRQ_HANDLED;
  }
            
  return IRQ_UNHANDLED;
}


void handle_gpu_interrupt(void)
{
  printk("handle_gpu_interrupt()\n");
  if (handle_uart_interrupt() == IRQ_HANDLED) {
    return;
  }

  if (handle_dmac_interrupt() == IRQ_HANDLED) {
    return;
  }
}


void handle_mailbox_interrupt(unsigned int cpu, unsigned int mb)
{
  switch (mb) {
  case 0:
    switch (mmio_in32(LP_CORE_MAILBOX_READ_WRITE_HIGH_TO_CLEAR(cpu, mb))) {
    case LP_MAILBOX0_INTERRUPT_DEBUG:
      printk("DEBUG: CPU %u Mailbox %u\n", cpu, mb);
      mmio_out32(LP_CORE_MAILBOX_READ_WRITE_HIGH_TO_CLEAR(cpu, mb),
                 LP_MAILBOX0_INTERRUPT_DEBUG);
      break;
    case LP_MAILBOX0_INTERRUPT_SCHED:
      do_sched();
      mmio_out32(LP_CORE_MAILBOX_READ_WRITE_HIGH_TO_CLEAR(cpu, mb),
                 LP_MAILBOX0_INTERRUPT_SCHED);
      break;
    default:
      printk("Error: Unknown Interrupt 0x%x\n",
             mmio_in32(LP_CORE_MAILBOX_READ_WRITE_HIGH_TO_CLEAR(cpu, mb)));
    }
    break;
  case 1:
    switch (mmio_in32(LP_CORE_MAILBOX_READ_WRITE_HIGH_TO_CLEAR(cpu, mb))) {
    default:
      printk("Error: Unknown Interrupt 0x%x\n",
             mmio_in32(LP_CORE_MAILBOX_READ_WRITE_HIGH_TO_CLEAR(cpu, mb)));
    }
    break;
  case 2:
    switch (mmio_in32(LP_CORE_MAILBOX_READ_WRITE_HIGH_TO_CLEAR(cpu, mb))) {
    default:
      printk("Error: Unknown Interrupt 0x%x\n",
             mmio_in32(LP_CORE_MAILBOX_READ_WRITE_HIGH_TO_CLEAR(cpu, mb)));
    }
    break;
  case 3:
    switch (mmio_in32(LP_CORE_MAILBOX_READ_WRITE_HIGH_TO_CLEAR(cpu, mb))) {
    default:
      printk("Error: Unknown Interrupt 0x%x\n",
             mmio_in32(LP_CORE_MAILBOX_READ_WRITE_HIGH_TO_CLEAR(cpu, mb)));
    }
    break;
  default:
    printk("Error: Unknown Mailbox %u\n", mb);
    break;
  }  
}


void do_irq_mailbox(unsigned int cpu, unsigned int mb)
{
  switch (cpu) {
  case 0:
  case 1:
  case 2:
  case 3:
    handle_mailbox_interrupt(cpu, mb);
    break;
  default:
    printk("Error: Unknown CPU %u\n", cpu);
    return;
    break;
  }
}


asmlinkage int do_irq(struct full_regs *regs)
{
  unsigned int irq;
  unsigned long cpu = get_cpu_id();
  printk("do_irq(): cpu = %lu\n", cpu);
  //  dump_registers(regs);
  irq = mmio_in32(LP_CORE_IRQ_SRC(cpu));
  
  /* check interrupt source */
  switch (irq) {    
  case LP_CORE_IRQ_SRC_CNTVIRQ_INTERRUPT:
    handle_timer_interrupt();
    break;
  case LP_CORE_IRQ_SRC_GPU_INTERRUPT:
    handle_gpu_interrupt();
    break;
  case LP_CORE_IRQ_SRC_MAILBOX_INTERRUPT(0):
    do_irq_mailbox(cpu, 0);
    break;
  case LP_CORE_IRQ_SRC_MAILBOX_INTERRUPT(1):
    do_irq_mailbox(cpu, 1);
    break;
  case LP_CORE_IRQ_SRC_MAILBOX_INTERRUPT(2):
    do_irq_mailbox(cpu, 2);
    break;
  case LP_CORE_IRQ_SRC_MAILBOX_INTERRUPT(3):
    do_irq_mailbox(cpu, 3);
    break;
  case LP_CORE_IRQ_SRC_CNTPSIRQ_INTERRUPT:
  case LP_CORE_IRQ_SRC_CNTPNSIRQ_INTERRUPT:
  case LP_CORE_IRQ_SRC_CNTHPIRQ_INTERRUPT:
  case LP_CORE_IRQ_SRC_PMU_INTERRUPT:
  case LP_CORE_IRQ_SRC_AXI_OUTSTANDING_INTERRUPT:
  case LP_CORE_IRQ_SRC_LOCAL_TIMER_INTERRUPT:
    /* LP_CORE_IRQ_SRC_PERIPHERAL(x) not used */
  default:
    printk("Error: Unknown IRQ 0x%lx\n", irq);
    return 2;
    break;
  }
  do_switch_thread();
  return 0;
}

