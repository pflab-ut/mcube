/**
 * @file include/aarch64/ops.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AARCH64_OPS_H__
#define __MCUBE_AARCH64_OPS_H__

#ifndef __ASSEMBLY__


/**
 * @fn static inline void svc(unsigned long id)
 * @brief Wrapper function for (supervisor call) @c svc.
 *
 * @param id ID.
 */
static inline void svc(unsigned long id)
{
  asm volatile("svc %0" :: "r"(id));
}

/**
 * @fn static inline void wfi(void)
 * @brief Wrapper function for (wait for interrupt) @c wfi.
 */
static inline void wfi(void)
{
  asm volatile("wfi");
}

/**
 * @fn static inline void wfe(void)
 * @brief Wrapper function for (wait for event) @c wfe.
 */
static inline void wfe(void)
{
  asm volatile("wfe");
}

/**
 * @fn static inline void nop(void)
 * @brief Wrapper function for @c nop.
 */
static inline void nop(void)
{
  asm volatile("nop");
}

/**
 * @fn static inline void isb(void)
 * @brief Wrapper function for instruction synchronization barrier @c isb.
 */
static inline void isb(void)
{
  asm volatile("isb");
}

/**
 * @fn static inline void dbg(void)
 * @brief Wrapper function for debug @c dbg.
 */
static inline void dbg(void)
{
  asm volatile("dbg");
}

/**
 * @fn static inline void dsb(void)
 * @brief Wrapper function for data synchronization barrier @c dsb.
 */
static inline void dsb(void)
{
  asm volatile("dsb");
}

/**
 * @fn static inline void dmb(void)
 * @brief Wrapper function for data memory barrier @c dmb.
 */
static inline void dmb(void)
{
  asm volatile("dmb 0");
}

/**
 * @fn static inline void sev(void)
 * @brief Wrapper function for send event @c sev.
 */
static inline void sev(void)
{
  asm volatile("sev");
}

/**
 * @fn static inline void sevl(void)
 * @brief Wrapper function for send event local @c sev.
 */
static inline void sevl(void)
{
  asm volatile("sevl");
}

/**
 * @def halt()
 * @brief Wrapper definition for infinite @c hlt.
 */
#define halt()                                  \
  do {                                          \
    printk("halt\n");                           \
    inf_loop();                                 \
  } while (0);




/**
 * @fn static inline void sync(void)
 * @brief Wrapper function for @c sync.
 */
static inline void sync(void)
{
  dmb();
}

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_AARCH64_OPS_H__ */
