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



asmlinkage int do_irq(struct full_regs *regs)
{
  unsigned int irq;
  printk("do_irq()\n");
  //  dump_registers(regs);
  unsigned long cpu = get_cpu_id();
  switch (cpu) {
  case 0:
    irq = mmio_in32(LP_CORE0_IRQ_SRC);
    break;
  case 1:
    irq = mmio_in32(LP_CORE1_IRQ_SRC);
    break;
  case 2:
    irq = mmio_in32(LP_CORE2_IRQ_SRC);
    break;
  case 3:
    irq = mmio_in32(LP_CORE3_IRQ_SRC);
    break;    
  default:
    printk("Unknown CPU 0x%lx\n", cpu);
    return 1;
    break;
  }
  
  /* check interrupt source */
  switch (irq) {    
  case LP_CORE_IRQ_SRC_CNTVIRQ_INTERRUPT:
    handle_timer_interrupt();
    break;
  case LP_CORE_IRQ_SRC_GPU_INTERRUPT:
    handle_gpu_interrupt();
    break;
  case LP_CORE_IRQ_SRC_CNTPSIRQ_INTERRUPT:
  case LP_CORE_IRQ_SRC_CNTPNSIRQ_INTERRUPT:
  case LP_CORE_IRQ_SRC_CNTHPIRQ_INTERRUPT:
  case LP_CORE_IRQ_SRC_MAILBOX0_INTERRUPT:
  case LP_CORE_IRQ_SRC_MAILBOX1_INTERRUPT:
  case LP_CORE_IRQ_SRC_MAILBOX2_INTERRUPT:
  case LP_CORE_IRQ_SRC_MAILBOX3_INTERRUPT:
  case LP_CORE_IRQ_SRC_PMU_INTERRUPT:
  case LP_CORE_IRQ_SRC_AXI_OUTSTANDING_INTERRUPT:
  case LP_CORE_IRQ_SRC_LOCAL_TIMER_INTERRUPT:
    /* LP_CORE_IRQ_SRC_PERIPHERAL(x) not used */
  default:
    printk("Unknown IRQ 0x%lx\n", irq);
    return 2;
    break;
  }
  do_switch_thread();
  return 0;
}

