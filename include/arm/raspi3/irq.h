/**
 * @file include/arm/raspi3/irq.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_ARM_RASPI3_IRQ_H__
#define	__MCUBE_ARM_RASPI3_IRQ_H__


#define IRQ_BASIC_PENDING	(IRQ_BASIC_BASE + 0x00000000)
#define IRQ_PENDING1		(IRQ_BASIC_BASE + 0x00000004)
#define IRQ_PENDING2		(IRQ_BASIC_BASE + 0x00000008)
#define FIQ_CONTROL		(IRQ_BASIC_BASE + 0x0000000c)
#define ENABLE_IRQS1		(IRQ_BASIC_BASE + 0x00000010)
#define ENABLE_IRQS2		(IRQ_BASIC_BASE + 0x00000014)
#define ENABLE_BASIC_IRQS	(IRQ_BASIC_BASE + 0x00000018)
#define DISABLE_IRQS1		(IRQ_BASIC_BASE + 0x0000001c)
#define DISABLE_IRQS2		(IRQ_BASIC_BASE + 0x00000020)
#define DISABLE_BASIC_IRQS	(IRQ_BASIC_BASE + 0x00000024)

#define NR_IRQS 100

#define SYSTEM_TIMER_IRQ0	(1 << 0)
#define SYSTEM_TIMER_IRQ1	(1 << 1)
#define SYSTEM_TIMER_IRQ2	(1 << 2)
#define SYSTEM_TIMER_IRQ3	(1 << 3)

#ifndef __ASSEMBLY__


static inline void enable_timer_interrupt(void)
{
  mmio_out32(TIMER_LOCAL_TIMER_CTRL_STATUS,
             mmio_in32(TIMER_LOCAL_TIMER_CTRL_STATUS) | (0x3 << 28));
}

static inline void disable_timer_interrupt(void)
{
  mmio_out32(TIMER_LOCAL_TIMER_CTRL_STATUS,
             mmio_in32(TIMER_LOCAL_TIMER_CTRL_STATUS) & ~(0x3 << 28));
}


#endif /* !__ASSEMBLY__ */

#endif /*	__MCUBE_ARM_RASPI3_IRQ_H__ */
