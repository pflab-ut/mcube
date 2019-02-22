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
#define CONFIG_ARCH_X86 1
/** Architecture. */
#define CONFIG_ARCH_ARM_RASPI3 0
/** Architecture. */
#define CONFIG_ARCH_ARM_SYNQUACER 0
/** Architecture. */
#define CONFIG_ARCH_AXIS 0
/** Compiler. */
#define CONFIG_COMPILER_CLANG 0
/** Compiler. */
#define CONFIG_COMPILER_GCC 1
/** Print. */
#define CONFIG_PRINT2CONSOLE 1
/** Print. */
#define CONFIG_PRINT2UART 0
/** Algorithm. */
#define CONFIG_ALGO_FP 1
/** Algorithm. */
#define CONFIG_ALGO_RM 0
/** Algorithm. */
#define CONFIG_ALGO_EDF 0
/** Task Queue. */
#define CONFIG_TQ_O1 1
/** Task Queue. */
#define CONFIG_TQ_BHEAP 0
/** Task Queue. */
#define CONFIG_TQ_DLIST 0
/** Tie-Break. */
#define CONFIG_TIE_BREAK_FIFO 1
/** Tie-Break. */
#define CONFIG_TIE_BREAK_ID 0
/** Option. */
#define CONFIG_OPTION_DEBUG 0
/** Option. */
#define CONFIG_OPTION_FS_EXT2 0
/** Option. */
#define CONFIG_OPTION_FS_FAT 0
/** Memory. */
#define CONFIG_MEMORY_VARIABLE 0
/** Memory. */
#define CONFIG_MEMORY_256KB 1
/** Memory. */
#define CONFIG_MEMORY_1MB 0
/** Memory. */
#define CONFIG_MEMORY_8MB 0
#endif /* __MCUBE_MCUBE_CONFIG_H__ */
