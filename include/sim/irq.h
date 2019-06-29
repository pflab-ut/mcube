/**
 * @file include/sim/irq.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_SIM_IRQ_H__
#define __MCUBE_SIM_IRQ_H__

#ifndef __ASSEMBLY__


/**
 * @fn static inline void enable_local_irq(void)
 * @brief enable local IRQ.
 */
static inline void enable_local_irq(void)
{
}

/**
 * @fn static inline void disable_local_irq(void)
 * @brief disable local IRQ.
 */
static inline void disable_local_irq(void)
{
}

/**
 * @fn static inline bool is_irq_enabled(__unused unsigned long flags)
 * @brief Is IRQ enabled?
 *
 * @param flags Flags.
 * @return True if IRQ enabled.
 */
static inline bool is_irq_enabled(__unused unsigned long flags)
{
  return true;
}

/**
 * @fn static inline void save_local_irq(__unused union rflags *flags)
 * @brief save local IRQ.
 *
 * @param flags Flags.
 */
static inline void save_local_irq(__unused union rflags *flags)
{
}


/**
 * @fn static inline void restore_local_irq(__unused union rflags *flags)
 * @brief restore local IRQ.
 *
 * @param flags Flags.
 */
static inline void restore_local_irq(__unused union rflags *flags)
{
}


/**
 * @fn static inline void generate_software_interrupt(__unused volatile unsigned long
 *                                                   id)
 * @brief generate software interrupt.
 *
 * @param id ID.
 */
static inline void generate_software_interrupt(__unused volatile unsigned long
                                               id)
{
}


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_SIM_IRQ_H__ */
