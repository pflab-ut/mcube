/**
 * @file include/debug/regs_debug.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_DEBUG_REGS_DEBUG_H__
#define	__MCUBE_DEBUG_REGS_DEBUG_H__

/* Never include this file directly. Include <debug/debug.h> instead. */

#ifndef __ASSEMBLY__
#if CONFIG_DEBUG


#if CONFIG_ARCH_SIM

#define pdebug_registers(ptr) do {							\
  } while (0)

#elif CONFIG_ARCH_X86

#define pdebug_registers(ptr) do {							\
  } while (0)

#elif CONFIG_ARCH_ARM

#define pdebug_registers(ptr) do {							\
  } while (0)

#elif CONFIG_ARCH_AXIS

#define pdebug_registers(ptr) do {							\
  } while (0)

#else
#error "Unknown Architecture"
#endif

#else
#define pdebug_registers(ptr)
#endif
#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_DEBUG_REGS_DEBUG_H__ */
