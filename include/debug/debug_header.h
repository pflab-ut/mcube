/**
 * @file include/debug/debug_header.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_DEBUG_DEBUG_HEADER_H__
#define __MCUBE_DEBUG_DEBUG_HEADER_H__

#ifndef __ASSEMBLY__
#if CONFIG_OPTION_DEBUG

extern unsigned int Debug;

/** The PDEBUG() macro. */
#define PDEBUG(fmt, args...) do {                    \
    if (Debug == true) {                             \
      printk(fmt, ## args);                          \
    }                                                \
  } while (0)

/** The PDEBUG_WHERE() macro. */
#define PDEBUG_WHERE() PDEBUG("%s:%s():%d: ", __FILE__, __func__, __LINE__)

/** Assert macro. */
#define mcube_assert(expr) do {                       \
    if (!(expr)) {                              \
      PDEBUG_WHERE();                           \
      printk("assert() failes\n");              \
      inf_loop();                               \
    }                                           \
  } while (0)

#else

/* nullify debugging macros */
#define PDEBUG(fmt, args...)
#define PDEBUG_WHERE()

#define mcube_assert(expr)

#endif /* CONFIG_OPTION_DEBUG */
#endif /* __ASSEMBLY__ */

#include <debug/regs_debug.h>
#include <debug/sched_queue_debug.h>
#include <debug/task_debug.h>
#include <debug/thread_debug.h>
#include <debug/jiffies_debug.h>

#endif /* __MCUBE_DEBUG_DEBUG_HEADER_H__ */

