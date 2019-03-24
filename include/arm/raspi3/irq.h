/**
 * @file include/arm/raspi3/irq.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_ARM_RASPI3_IRQ_H__
#define __MCUBE_ARM_RASPI3_IRQ_H__


#define IRQ_BASIC_PENDING  (IRQ_BASIC_BASE + 0x00000000)
#define IRQ_PENDING1       (IRQ_BASIC_BASE + 0x00000004)
#define IRQ_PENDING2       (IRQ_BASIC_BASE + 0x00000008)
#define FIQ_CTRL           (IRQ_BASIC_BASE + 0x0000000c)
#define ENABLE_IRQS1       (IRQ_BASIC_BASE + 0x00000010)
#define ENABLE_IRQS2       (IRQ_BASIC_BASE + 0x00000014)
#define ENABLE_BASIC_IRQS  (IRQ_BASIC_BASE + 0x00000018)
#define DISABLE_IRQS1      (IRQ_BASIC_BASE + 0x0000001c)
#define DISABLE_IRQS2      (IRQ_BASIC_BASE + 0x00000020)
#define DISABLE_BASIC_IRQS (IRQ_BASIC_BASE + 0x00000024)


/* Basic Pending Register */
/* 31-21: reserved */
/* 20: GPU IRQ62 */
#define IRQ_BASIC_PENDING_GPU_IRQ62 (0x1 << 20)
/* 19: GPU IRQ57 */
#define IRQ_BASIC_PENDING_GPU_IRQ57 (0x1 << 19)
/* 18: GPU IRQ56 */
#define IRQ_BASIC_PENDING_GPU_IRQ56 (0x1 << 18)
/* 17: GPU IRQ55 */
#define IRQ_BASIC_PENDING_GPU_IRQ55 (0x1 << 17)
/* 16: GPU IRQ54 */
#define IRQ_BASIC_PENDING_GPU_IRQ54 (0x1 << 16)
/* 15: GPU IRQ53 */
#define IRQ_BASIC_PENDING_GPU_IRQ53 (0x1 << 15)
/* 14: GPU IRQ19 */
#define IRQ_BASIC_PENDING_GPU_IRQ19 (0x1 << 14)
/* 13: GPU IRQ18 */
#define IRQ_BASIC_PENDING_GPU_IRQ18 (0x1 << 13)
/* 12: GPU IRQ10 */
#define IRQ_BASIC_PENDING_GPU_IRQ10 (0x1 << 12)
/* 11: GPU IRQ9 */
#define IRQ_BASIC_PENDING_GPU_IRQ9  (0x1 << 11)
/* 10: GPU IRQ7 */
#define IRQ_BASIC_PENDING_GPU_IRQ7  (0x1 << 10)
/* 9: One or more bits set in pending register 2 */
#define IRQ_BASIC_PENDING_REG2      (0x1 << 9)
/* 8: One or more bits set in pending register 1 */
#define IRQ_BASIC_PENDING_REG1      (0x1 << 8)
/* 7: Illegal access type 0 IRQ pending */
#define IRQ_BASIC_PENDING_ILLEGAL_ACCESS_TYPE0_IRQ_PENDING (0x1 << 7)
/* 6: Illegal access type 1 IRQ pending */
#define IRQ_BASIC_PENDING_ILLEGAL_ACCESS_TYPE1_IRQ_PENDING (0x1 << 6)
/* 5: GPU1 halted IRQ pending */
#define IRQ_BASIC_PENDING_GPU1_HALTED_IRQ_PENDING (0x1 << 5)
/* 4: GPU0 halted IRQ pending
 * (Or GPU1 halted if bit 10 of control register 1 is set)
 */
#define IRQ_BASIC_PENDING_GPU0_HALTED_IRQ_PENDING (0x1 << 4)
/* 3: ARM Doorbell 1 IRQ pending */
#define IRQ_BASIC_PENDING_ARM_DOORBELL1_IRQ_PENDING (0x1 << 3)
/* 2: ARM Doorbell 0 IRQ pending */
#define IRQ_BASIC_PENDING_ARM_DOORBELL0_IRQ_PENDING (0x1 << 2)
/* 1: ARM Mailbox IRQ pending */
#define IRQ_BASIC_PENDING_ARM_MAILBOX_IRQ_PENDING (0x1 << 1)
/* 0: ARM Timer IRQ pending */
#define IRQ_BASIC_PENDING_ARM_TIMER_IRQ_PENDING (0x1 << 0)


/* GPU Pending Registers */
/* GPU Pending 1 Register
 * 31-0: IRQ pending source 31-0
 * That is all bits except 7, 9, 10, 18, 19.
 */
/* GPU pending 2 Register
 * 31-0: IRQ pending source 63-32
 * That is all bits except register bits 21..25, 30 (Interrupts 53..57,62).
 */
#define IRQ_PENDINGn_SRC(x) (0x1 << ((x) & 0x1f))

/* FIQ Register */
/* 31-8: reserved */
/* 7: FIQ enable.
 * Set this bit to 1 to enable FIQ generation.
 * If set to 0 bits 6:0 are don't care.
 */
#define FIQ_CTRL_ENABLE (0x1 << 7)
/* 6-0: Select FIQ Source */
#define FIQ_CTRL_SELECT_FIQ_SRC(x) (0x1 << (x))


/* Interrupt Enable Registers */
/* Interrupt Enable Register 1
 * 31-0: Set to enable IRQ source 31-0
 */
/* Interrupt Enable Register 2
 * 63-32: Set to enable IRQ source 63-32
 */
#define ENABLE_IRQSn_SET(x) (0x1 << ((x) & 0x1f))


/* Basic Interrupt enable register */
/* 31-8: reserved */
/* 7: Set to enable Access error type 0 IRQ */
#define ENABLE_BASIC_IRQS_ACCESS_ERROR_TYPE0_IRQ (0x1 << 7)
/* 6: Set to enable Access error type 1 IRQ */
#define ENABLE_BASIC_IRQS_ACCESS_ERROR_TYPE1_IRQ (0x1 << 6)
/* 5: Set to enable GPU 1 Halted IRQ */
#define ENABLE_BASIC_IRQS_GPU1_HALTED_IRQ (0x1 << 5)
/* 4: Set to enable GPU 0 Halted IRQ */
#define ENABLE_BASIC_IRQS_GPU0_HALTED_IRQ (0x1 << 4)
/* 3: Set to enable ARM Doorbell 1 IRQ */
#define ENABLE_BASIC_IRQS_ARM_DOORBELL1_IRQ (0x1 << 3)
/* 2: Set to enable ARM Doorbell 0 IRQ */
#define ENABLE_BASIC_IRQS_ARM_DOORBELL0_IRQ (0x1 << 2)
/* 1: Set to enable ARM Mailbox IRQ */
#define ENABLE_BASIC_IRQS_ARM_MAILBOX_IRQ (0x1 << 1)
/* 0: Set to enable ARM Timer IRQ */
#define ENABLE_BASIC_IRQS_ARM_TIMER_IRQ (0x1 << 0)


/* Interrupt Disable Registers */
/* Interrupt Disable Register 1
 * 31-0: Set to disable IRQ source 31-0
 */
/* Interrupt Disable Register 2
 * 63-32: Set to disable IRQ source 63-32
 */
#define DISABLE_IRQSn_SET(x) (0x1 << ((x & 0x1f))

/* Basic Disable Register */
/* 31-8: reserved */
/* 7: Set to disable Access error type 0 IRQ */
#define DISABLE_BASIC_IRQS_ACCESS_ERROR_TYPE0_IRQ (0x1 << 7)
/* 6: Set to disable Access error type 1 IRQ */
#define DISABLE_BASIC_IRQS_ACCESS_ERROR_TYPE1_IRQ (0x1 << 6)
/* 5: Set to disable GPU 1 Halted IRQ */
#define DISABLE_BASIC_IRQS_GPU1_HALTED_IRQ (0x1 << 5)
/* 4: Set to disable GPU 0 Halted IRQ */
#define DISABLE_BASIC_IRQS_GPU0_HALTED_IRQ (0x1 << 4)
/* 3: Set to disable ARM Doorbell 1 IRQ */
#define DISABLE_BASIC_IRQS_ARM_DOORBELL1_IRQ (0x1 << 3)
/* 2: Set to disable ARM Doorbell 0 IRQ */
#define DISABLE_BASIC_IRQS_ARM_DOORBELL0_IRQ (0x1 << 2)
/* 1: Set to disable ARM Mailbox IRQ */
#define DISABLE_BASIC_IRQS_ARM_MAILBOX_IRQ (0x1 << 1)
/* 0: Set to disable ARM Timer IRQ */
#define DISABLE_BASIC_IRQS_ARM_TIMER_IRQ (0x1 << 0)


#define NR_IRQS 64


#define SYSTEM_TIMER_IRQ0 (1 << 0)
#define SYSTEM_TIMER_IRQ1 (1 << 1)
#define SYSTEM_TIMER_IRQ2 (1 << 2)
#define SYSTEM_TIMER_IRQ3 (1 << 3)


#ifndef __ASSEMBLY__


static inline void enable_timer_interrupt(void)
{
  mmio_out32(LP_LOCAL_TIMER_CTRL_STATUS,
             mmio_in32(LP_LOCAL_TIMER_CTRL_STATUS)
             | LP_LOCAL_TIMER_CTRL_STATUS_INTERRUPT_ENABLE
             | LP_LOCAL_TIMER_CTRL_STATUS_TIMER_ENABLE);
}

static inline void disable_timer_interrupt(void)
{
  mmio_out32(LP_LOCAL_TIMER_CTRL_STATUS,
             mmio_in32(LP_LOCAL_TIMER_CTRL_STATUS)
             & ~(LP_LOCAL_TIMER_CTRL_STATUS_INTERRUPT_ENABLE
                 | LP_LOCAL_TIMER_CTRL_STATUS_TIMER_ENABLE));
}

void handle_gpu_interrupt(void);
void handle_mailbox_interrupt(unsigned int cpu, unsigned int mb);
void do_irq_mailbox(unsigned int cpu, unsigned int mb);


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_ARM_RASPI3_IRQ_H__ */
