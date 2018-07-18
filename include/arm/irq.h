/**
 * @file include/arm/irq.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_ARM_IRQ_H__
#define __MCUBE_ARM_IRQ_H__

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

#define AARCH64_DAIF_FIQ (0x1 << 0)
#define AARCH64_DAIF_IRQ (0x1 << 1)
#define AARCH64_DAIF_SERROR (0x1 << 2)
#define AARCH64_DAIF_DEBUG_EXCEPTION (0x1 << 3)
 

#ifdef __ASSEMBLY__

.macro enable_interrupt
  msr daifclr, #AARCH64_DAIF_IRQ
.endm

.macro disable_interrupt
  msr   daifset, #AARCH64_DAIF_IRQ
.endm

#else

static inline uint64_t get_interrupt_disable_flag(void)
{
  uint64_t data; 
  asm volatile("mrs %0, daif\n\t":"=r"(data));
  return data;
}

static inline void enable_interrupt(void)
{
  asm volatile("msr daifclr, %0\n\t" :: "i"(AARCH64_DAIF_IRQ));
}

static inline void disable_interrupt(void)
{
  asm volatile("msr daifset, %0\n\t" :: "i"(AARCH64_DAIF_IRQ));
}


void init_vector(void);

void show_invalid_entry_message(int type, unsigned long esr, unsigned long address);
  
#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_ARM_IRQ_H__ */
