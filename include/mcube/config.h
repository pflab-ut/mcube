/**
 * @file include/mcube/config.h 
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_CONFIG_H__
#define __MCUBE_MCUBE_CONFIG_H__
/* Don't edit this file.
 * Please edit configure then do make configure. */
/** Architecture. */
#define CONFIG_ARCH_X86 1
/** Compiler. */
#define CONFIG_COMPILER_GCC 1
/** Printk. */
#define CONFIG_PRINTK2CONSOLE 1
/** Algorithm. */
#define CONFIG_ALGO_EDF 1
/** Task Queue. */
#define CONFIG_TQ_DLIST 1
/** Tie-Break. */
#define CONFIG_TIE_BREAK_ID 1
#define CONFIG_DEBUG 1
#define CONFIG_TRACE 1
/** Memory. */
#define CONFIG_MEMORY_8MB 1
#endif /* __MCUBE_MCUBE_CONFIG_H__ */
