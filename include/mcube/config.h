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
#define CONFIG_ARCH_SIM 0
/** Architecture. */
#define CONFIG_ARCH_X86_64 0
/** Architecture. */
#define CONFIG_ARCH_AARCH64_RASPI3 0
/** Architecture. */
#define CONFIG_ARCH_AXIS 1
/** Compiler. */
#define CONFIG_COMPILER_CLANG 0
/** Compiler. */
#define CONFIG_COMPILER_GCC 1
/** Print. */
#define CONFIG_PRINT2CONSOLE 1
/** Print. */
#define CONFIG_PRINT2UART 0
/** Algorithm. */
#define CONFIG_ALGO_FP 0
/** Algorithm. */
#define CONFIG_ALGO_RM 0
/** Algorithm. */
#define CONFIG_ALGO_EDF 1
/** Task Queue. */
#define CONFIG_TQ_O1 0
/** Task Queue. */
#define CONFIG_TQ_BHEAP 0
/** Task Queue. */
#define CONFIG_TQ_DLIST 1
/** Tie-Break. */
#define CONFIG_TIE_BREAK_FIFO 0
/** Tie-Break. */
#define CONFIG_TIE_BREAK_ID 1
/** Option. */
#define CONFIG_OPTION_DEBUG 1
/** Option. */
#define CONFIG_OPTION_FS_EXT2 0
/** Option. */
#define CONFIG_OPTION_FS_FAT 0
#endif /* __MCUBE_MCUBE_CONFIG_H__ */
