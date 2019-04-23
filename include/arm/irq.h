/**
 * @file include/arm/irq.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_ARM_IRQ_H__
#define __MCUBE_ARM_IRQ_H__


#define AARCH64_DAIF_FIQ (0x1 << 0)
#define AARCH64_DAIF_IRQ (0x1 << 1)
#define AARCH64_DAIF_SERROR (0x1 << 2)
#define AARCH64_DAIF_DEBUG_EXCEPTION (0x1 << 3)


#ifdef __ASSEMBLY__

.macro enable_local_irq
msr daifclr, #AARCH64_DAIF_IRQ
.endm

.macro disable_local_irq
msr   daifset, #AARCH64_DAIF_IRQ
.endm

#else

static inline uint64_t get_irq_disable_flag(void)
{
  uint64_t data;
  asm volatile("mrs %0, daif\n\t":"=r"(data));
  return data;
}

static inline void set_irq_disable_flag(uint64_t *flags)
{
  asm volatile("msr daif, %0\n\t":"=r"(flags));
}

#define PSR_I_BIT     0x00000080

static inline void enable_local_irq(void)
{
  asm volatile("msr daifclr, %0\n\t" :: "i"(AARCH64_DAIF_IRQ));
}

static inline void disable_local_irq(void)
{
  asm volatile("msr daifset, %0\n\t" :: "i"(AARCH64_DAIF_IRQ));
}

static inline int is_irq_enabled(unsigned long flags)
{
  return !(flags & PSR_I_BIT);
}


static inline void save_local_irq(union rflags *flags)
{
  asm volatile("mrs    %0, daif\n"
               : "=r"(flags->raw)
               :
               : "memory");

  if (is_irq_enabled(flags->raw)) {
    disable_local_irq();
  }
}

static inline void restore_local_irq(union rflags *flags)
{
  asm volatile("msr    daif, %0"
               :
               : "r"(flags->raw)
               : "memory");

  if (is_irq_enabled(flags->raw)) {
    enable_local_irq();
  }
}



void init_vector(void);
void show_invalid_entry_message(int type, unsigned long esr,
                                unsigned long address, struct full_regs *regs);


int handle_dmac_interrupt(void);
int handle_uart_interrupt(void);
int handle_timer_interrupt(void);


#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_ARM_IRQ_H__ */
