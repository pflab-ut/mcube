/**
 * @file include/aarch64/raspi3/irq.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AARCH64_RASPI3_IRQ_H__
#define __MCUBE_AARCH64_RASPI3_IRQ_H__

/**
 * @def IRQ_BASIC_PENDING
 * @brief IRQ basic pending register.
 */
#define IRQ_BASIC_PENDING (IRQ_BASIC_BASE + 0x00000000)

/**
 * @def IRQ_PENDING1
 * @brief IRQ GPU pending 1 register.
 */
#define IRQ_PENDING1 (IRQ_BASIC_BASE + 0x00000004)

/**
 * @def IRQ_PENDING2
 * @brief IRQ GPU pending 2 register.
 */
#define IRQ_PENDING2 (IRQ_BASIC_BASE + 0x00000008)

/**
 * @def FIQ_CTRL
 * @brief FIQ register.
 */
#define FIQ_CTRL (IRQ_BASIC_BASE + 0x0000000c)

/**
 * @def ENABLE_IRQS1
 * @brief Interrupt enable register 1.
 */
#define ENABLE_IRQS1 (IRQ_BASIC_BASE + 0x00000010)

/**
 * @def ENABLE_IRQS2
 * @brief Interrupt enable register 2.
 */
#define ENABLE_IRQS2 (IRQ_BASIC_BASE + 0x00000014)

/**
 * @def ENABLE_BASIC_IRQS
 * @brief Base interrupt enable register.
 */
#define ENABLE_BASIC_IRQS (IRQ_BASIC_BASE + 0x00000018)

/**
 * @def DISABLE_IRQS1
 * @brief Interrupt disable register 1.
 */
#define DISABLE_IRQS1 (IRQ_BASIC_BASE + 0x0000001c)

/**
 * @def DISABLE_IRQS2
 * @brief Interrupt disable register 2.
 */
#define DISABLE_IRQS2 (IRQ_BASIC_BASE + 0x00000020)

/**
 * @def DISABLE_BASIC_IRQS
 * @brief Base disable register.
 */
#define DISABLE_BASIC_IRQS (IRQ_BASIC_BASE + 0x00000024)


/* Basic Pending Register */
/* 31-21: reserved */
/**
 * @def IRQ_BASIC_PENDING_GPU_IRQ62
 * @brief 20: GPU IRQ62.
 */
#define IRQ_BASIC_PENDING_GPU_IRQ62 (0x1 << 20)

/**
 * @def IRQ_BASIC_PENDING_GPU_IRQ57
 * @brief 19: GPU IRQ57.
 */
#define IRQ_BASIC_PENDING_GPU_IRQ57 (0x1 << 19)

/**
 * @def IRQ_BASIC_PENDING_GPU_IRQ56
 * @brief 18: GPU IRQ56.
 */
#define IRQ_BASIC_PENDING_GPU_IRQ56 (0x1 << 18)

/**
 * @def IRQ_BASIC_PENDING_GPU_IRQ55
 * @brief 17: GPU IRQ55.
 */
#define IRQ_BASIC_PENDING_GPU_IRQ55 (0x1 << 17)

/**
 * @def IRQ_BASIC_PENDING_GPU_IRQ54
 * @brief 16: GPU IRQ54.
 */
#define IRQ_BASIC_PENDING_GPU_IRQ54 (0x1 << 16)

/**
 * @def IRQ_BASIC_PENDING_GPU_IRQ53
 * @brief 15: GPU IRQ53.
 */
#define IRQ_BASIC_PENDING_GPU_IRQ53 (0x1 << 15)

/**
 * @def IRQ_BASIC_PENDING_GPU_IRQ19
 * @brief 14: GPU IRQ19.
 */
#define IRQ_BASIC_PENDING_GPU_IRQ19 (0x1 << 14)

/**
 * @def IRQ_BASIC_PENDING_GPU_IRQ18
 * @brief 13: GPU IRQ18.
 */
#define IRQ_BASIC_PENDING_GPU_IRQ18 (0x1 << 13)

/**
 * @def IRQ_BASIC_PENDING_GPU_IRQ10
 * @brief 12: GPU IRQ10.
 */
#define IRQ_BASIC_PENDING_GPU_IRQ10 (0x1 << 12)

/**
 * @def IRQ_BASIC_PENDING_GPU_IRQ9
 * @brief 11: GPU IRQ9.
 */
#define IRQ_BASIC_PENDING_GPU_IRQ9 (0x1 << 11)

/**
 * @def IRQ_BASIC_PENDING_GPU_IRQ7
 * @brief 10: GPU IRQ7.
 */
#define IRQ_BASIC_PENDING_GPU_IRQ7 (0x1 << 10)

/**
 * @def IRQ_BASIC_PENDING_REG2
 * @brief 9: One or more bits set in pending register 2.
 */
#define IRQ_BASIC_PENDING_REG2 (0x1 << 9)

/**
 * @def IRQ_BASIC_PENDING_REG1
 * @brief 8: One or more bits set in pending register 1.
 */
#define IRQ_BASIC_PENDING_REG1 (0x1 << 8)

/**
 * @def IRQ_BASIC_PENDING_ILLEGAL_ACCESS_TYPE0_IRQ_PENDING
 * @brief 7: Illegal access type 0 IRQ pending.
 */
#define IRQ_BASIC_PENDING_ILLEGAL_ACCESS_TYPE0_IRQ_PENDING (0x1 << 7)

/**
 * @def IRQ_BASIC_PENDING_ILLEGAL_ACCESS_TYPE1_IRQ_PENDING
 * @brief 6: Illegal access type 1 IRQ pending.
 */
#define IRQ_BASIC_PENDING_ILLEGAL_ACCESS_TYPE1_IRQ_PENDING (0x1 << 6)

/**
 * @def IRQ_BASIC_PENDING_GPU1_HALTED_IRQ_PENDING
 * @brief 5: GPU1 halted IRQ pending.
 */
#define IRQ_BASIC_PENDING_GPU1_HALTED_IRQ_PENDING (0x1 << 5)

/**
 * @def IRQ_BASIC_PENDING_GPU0_HALTED_IRQ_PENDING
 * @brief 4: GPU0 halted IRQ pending.
 * (Or GPU1 halted if bit 10 of control register 1 is set)
 */
#define IRQ_BASIC_PENDING_GPU0_HALTED_IRQ_PENDING (0x1 << 4)

/**
 * @def IRQ_BASIC_PENDING_ARM_DOORBELL1_IRQ_PENDING
 * @brief 3: ARM Doorbell 1 IRQ pending.
 */
#define IRQ_BASIC_PENDING_ARM_DOORBELL1_IRQ_PENDING (0x1 << 3)

/**
 * @def IRQ_BASIC_PENDING_ARM_DOORBELL0_IRQ_PENDING
 * @brief 2: ARM Doorbell 0 IRQ pending.
 */
#define IRQ_BASIC_PENDING_ARM_DOORBELL0_IRQ_PENDING (0x1 << 2)

/**
 * @def IRQ_BASIC_PENDING_ARM_MAILBOX_IRQ_PENDING
 * @brief 1: ARM Mailbox IRQ pending.
 */
#define IRQ_BASIC_PENDING_ARM_MAILBOX_IRQ_PENDING (0x1 << 1)

/**
 * @def IRQ_BASIC_PENDING_ARM_TIMER_IRQ_PENDING
 * @brief 0: ARM Timer IRQ pending.
 */
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
/**
 * @def IRQ_PENDINGn_SRC(x)
 * @brief GPU pending registers.
 */
#define IRQ_PENDINGn_SRC(x) (0x1 << ((x) & 0x1f))

/* FIQ Register */
/* 31-8: reserved */

/**
 * @def FIQ_CTRL_ENABLE
 * @brief 7: FIQ enable.
 * Set this bit to 1 to enable FIQ generation.
 * If set to 0 bits 6:0 are don't care.
 */
#define FIQ_CTRL_ENABLE (0x1 << 7)

/**
 * @def FIQ_CTRL_SELECT_FIQ_SRC(x)
 * @brief 6-0: Select FIQ Source.
 *
 * @param x FIQ source.
 */
#define FIQ_CTRL_SELECT_FIQ_SRC(x) (0x1 << (x))


/* Interrupt Enable Registers */
/* Interrupt Enable Register 1
 * 31-0: Set to enable IRQ source 31-0
 */
/* Interrupt Enable Register 2
 * 63-32: Set to enable IRQ source 63-32
 */
/**
 * @def ENABLE_IRQSn_SET(x)
 * @brief Interrupt enable register.
 *
 * @param x IRQ source.
 */
#define ENABLE_IRQSn_SET(x) (0x1 << ((x) & 0x1f))


/* Basic Interrupt enable register */
/* 31-8: reserved */
/**
 * @def ENABLE_BASIC_IRQS_ACCESS_ERROR_TYPE0_IRQ
 * @brief 7: Set to enable Access error type 0 IRQ.
 */
#define ENABLE_BASIC_IRQS_ACCESS_ERROR_TYPE0_IRQ (0x1 << 7)

/**
 * @def ENABLE_BASIC_IRQS_ACCESS_ERROR_TYPE1_IRQ
 * @brief 6: Set to enable Access error type 1 IRQ.
 */
#define ENABLE_BASIC_IRQS_ACCESS_ERROR_TYPE1_IRQ (0x1 << 6)

/**
 * @def ENABLE_BASIC_IRQS_GPU1_HALTED_IRQ
 * @brief 5: Set to enable GPU 1 Halted IRQ.
 */
#define ENABLE_BASIC_IRQS_GPU1_HALTED_IRQ (0x1 << 5)

/**
 * @def ENABLE_BASIC_IRQS_GPU0_HALTED_IRQ
 * @brief 4: Set to enable GPU 0 Halted IRQ.
 */
#define ENABLE_BASIC_IRQS_GPU0_HALTED_IRQ (0x1 << 4)

/**
 * @def ENABLE_BASIC_IRQS_ARM_DOORBELL1_IRQ
 * @brief 3: Set to enable ARM Doorbell 1 IRQ.
 */
#define ENABLE_BASIC_IRQS_ARM_DOORBELL1_IRQ (0x1 << 3)

/**
 * @def ENABLE_BASIC_IRQS_ARM_DOORBELL0_IRQ
 * @brief 2: Set to enable ARM Doorbell 0 IRQ.
 */
#define ENABLE_BASIC_IRQS_ARM_DOORBELL0_IRQ (0x1 << 2)

/**
 * @def ENABLE_BASIC_IRQS_ARM_MAILBOX_IRQ
 * @brief 1: Set to enable ARM Mailbox IRQ.
 */
#define ENABLE_BASIC_IRQS_ARM_MAILBOX_IRQ (0x1 << 1)

/**
 * @def ENABLE_BASIC_IRQS_ARM_TIMER_IRQ
 * @brief 0: Set to enable ARM Timer IRQ.
 */
#define ENABLE_BASIC_IRQS_ARM_TIMER_IRQ (0x1 << 0)


/* Interrupt Disable Registers */
/* Interrupt Disable Register 1
 * 31-0: Set to disable IRQ source 31-0
 */
/* Interrupt Disable Register 2
 * 63-32: Set to disable IRQ source 63-32
 */
/**
 * @def DISABLE_IRQSn_SET(x)
 * @brief Interrupt disable register.
 */
#define DISABLE_IRQSn_SET(x) (0x1 << ((x & 0x1f))

/* Basic Disable Register */
/* 31-8: reserved */
/**
 * @def DISABLE_BASIC_IRQS_ACCESS_ERROR_TYPE0_IRQ
 * @brief 7: Set to disable Access error type 0 IRQ.
 */
#define DISABLE_BASIC_IRQS_ACCESS_ERROR_TYPE0_IRQ (0x1 << 7)

/**
 * @def DISABLE_BASIC_IRQS_ACCESS_ERROR_TYPE1_IRQ
 * @brief 6: Set to disable Access error type 1 IRQ.
 */
#define DISABLE_BASIC_IRQS_ACCESS_ERROR_TYPE1_IRQ (0x1 << 6)

/**
 * @def DISABLE_BASIC_IRQS_GPU1_HALTED_IRQ
 * @brief 5: Set to disable GPU 1 Halted IRQ.
 */
#define DISABLE_BASIC_IRQS_GPU1_HALTED_IRQ (0x1 << 5)

/**
 * @def DISABLE_BASIC_IRQS_GPU0_HALTED_IRQ
 * @brief 4: Set to disable GPU 0 Halted IRQ.
 */
#define DISABLE_BASIC_IRQS_GPU0_HALTED_IRQ (0x1 << 4)

/**
 * @def DISABLE_BASIC_IRQS_ARM_DOORBELL1_IRQ
 * @brief 3: Set to disable ARM Doorbell 1 IRQ.
 */
#define DISABLE_BASIC_IRQS_ARM_DOORBELL1_IRQ (0x1 << 3)

/**
 * @def DISABLE_BASIC_IRQS_ARM_DOORBELL0_IRQ
 * @brief 2: Set to disable ARM Doorbell 0 IRQ.
 */
#define DISABLE_BASIC_IRQS_ARM_DOORBELL0_IRQ (0x1 << 2)

/**
 * @def DISABLE_BASIC_IRQS_ARM_MAILBOX_IRQ
 * @brief 1: Set to disable ARM Mailbox IRQ.
 */
#define DISABLE_BASIC_IRQS_ARM_MAILBOX_IRQ (0x1 << 1)

/**
 * @def DISABLE_BASIC_IRQS_ARM_TIMER_IRQ
 * @brief 0: Set to disable ARM Timer IRQ.
 */
#define DISABLE_BASIC_IRQS_ARM_TIMER_IRQ (0x1 << 0)

/**
 * @def NR_IRQS
 * @brief Number of IRQs.
 */
#define NR_IRQS 64

/**
 * @def SYSTEM_TIMER_IRQ0
 * @brief System timer IRQ 0.
 */
#define SYSTEM_TIMER_IRQ0 (1 << 0)

/**
 * @def SYSTEM_TIMER_IRQ1
 * @brief System timer IRQ 1.
 */
#define SYSTEM_TIMER_IRQ1 (1 << 1)

/**
 * @def SYSTEM_TIMER_IRQ2
 * @brief System timer IRQ 2.
 */
#define SYSTEM_TIMER_IRQ2 (1 << 2)

/**
 * @def SYSTEM_TIMER_IRQ3
 * @brief System timer IRQ 3.
 */
#define SYSTEM_TIMER_IRQ3 (1 << 3)


#ifndef __ASSEMBLY__


/**
 * @fn static inline void enable_timer_interrupt(void)
 * @brief enable timer interrupt.
 */
static inline void enable_timer_interrupt(void)
{
  mmio_out32(LP_LOCAL_TIMER_CTRL_STATUS,
             mmio_in32(LP_LOCAL_TIMER_CTRL_STATUS)
             | LP_LOCAL_TIMER_CTRL_STATUS_INTERRUPT_ENABLE
             | LP_LOCAL_TIMER_CTRL_STATUS_TIMER_ENABLE);
}

/**
 * @fn static inline void disable_timer_interrupt(void)
 * @brief disable timer interrupt.
 */
static inline void disable_timer_interrupt(void)
{
  mmio_out32(LP_LOCAL_TIMER_CTRL_STATUS,
             mmio_in32(LP_LOCAL_TIMER_CTRL_STATUS)
             & ~(LP_LOCAL_TIMER_CTRL_STATUS_INTERRUPT_ENABLE
                 | LP_LOCAL_TIMER_CTRL_STATUS_TIMER_ENABLE));
}

/**
 * @fn void handle_gpu_interrupt(void)
 * @brief GPU interrupt handler.
 */
void handle_gpu_interrupt(void);

/**
 * @fn void handle_mailbox_interrupt(unsigned int cpu, unsigned int mb)
 * @brief Mailbox interrupt handler.
 *
 * @param cpu CPU ID.
 * @param mb Mailbox.
 */
void handle_mailbox_interrupt(unsigned int cpu, unsigned int mb);

/**
 * @fn void do_irq_mailbox(unsigned int cpu, unsigned int mb)
 * @brief do IRQ mailbox.
 *
 * @param cpu CPU ID.
 * @param mb Mailbox.
 */
void do_irq_mailbox(unsigned int cpu, unsigned int mb);


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_AARCH64_RASPI3_IRQ_H__ */
