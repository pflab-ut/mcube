/**
 * @file include/x86/irqflags.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_IRQFLAGS_H__
#define __MCUBE_X86_IRQFLAGS_H__

#ifndef __ASSEMBLY__

static inline void enable_local_irq(void)
{
	sti();
}

static inline void disable_local_irq(void)
{
	cli();
}

static inline void save_local_irq64(uint64_t *flags)
{
}

static inline void restore_local_irq64(uint64_t *flags)
{
}


static inline void save_local_irq(uint32_t *flags)
{
#if 0
	asm volatile("# raw_save_flags\n\t"
							 "pushf ; pop %0"
							 : "=rm" (*flags)
							 : /* no input */
							 : "memory");
#endif
}


static inline void restore_local_irq(uint32_t *flags)
{
#if 0
	asm volatile("push %0 ; popf"
							 : /* no output */
							 :"g" (*flags)
							 :"memory", "cc");
#endif
}

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_IRQFLAGS_H__ */
