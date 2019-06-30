/**
 * @file include/axis/ops.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AXIS_OPS_H__
#define __MCUBE_AXIS_OPS_H__

#ifndef __ASSEMBLY__

/**
 * @fn static inline void nop(void)
 * @brief Wrapper function for @c nop.
 */
static inline void nop(void)
{
  asm volatile("nop");
}

/**
 * @fn static inline void sync(void)
 * @brief Wrapper function for @c sync.
 */
static inline void sync(void)
{
  asm volatile("sync");
}

/**
 * @fn static inline void wi(void)
 * @brief Wrapper function for @c ci.
 */
static inline void wi(void)
{
  asm volatile("wi");
}

/**
 * @fn static inline void ert(void)
 * @brief Wrapper function for @c ert.
 */
static inline void ert(void)
{
  asm volatile("ert");
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


#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_AXIS_OPS_H__ */
