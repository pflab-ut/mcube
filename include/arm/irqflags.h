/**
 * @file include/arm/irqflags.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_ARM_IRQFLAGS_H__
#define __MCUBE_ARM_IRQFLAGS_H__

#ifndef __ASSEMBLY__

static inline void enable_local_irq(void)
{
}

static inline void disable_local_irq(void)
{
}

static inline void save_local_irq64(uint64_t *flags)
{
}

static inline void restore_local_irq64(uint64_t *flags)
{
}

static inline void save_local_irq(uint32_t *flags)
{
}

static inline void restore_local_irq(uint32_t *flags)
{
}

#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_ARM_IRQFLAGS_H__ */
