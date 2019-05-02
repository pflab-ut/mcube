/**
 * @file include/mcube/kernel.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_KERNEL_H__
#define __MCUBE_MCUBE_KERNEL_H__


#define NR_THREADS 16

/* 4K */
#define KERNEL_STACK_SIZE 0x1000
#define USER_STACK_SIZE 0x1000

#define STACK_ALIGN(x)  (((x) + 7) & -8)

#define NR_PRIORITIES NR_THREADS

/* each bitmap length is 32 bit */
#define NR_PRIORITY_BITMAPS CEIL(NR_PRIORITIES, 32)

#ifndef  __ASSEMBLY__

#define inf_loop() do {                                                 \
    print("%s:%s():%d %s\n", __FILE__, __func__, __LINE__, "inf_loop()"); \
    if (call_sys_get_mode_level() != USER_LEVEL) {                      \
      disable_local_irq();                                              \
    }                                                                   \
    for (;;)                                                            \
      ;                                                                 \
  } while (0)


extern unsigned int Debug;

int main(int argc, char *argv[]);

void init_arch(void);
void init_arch_ap(void);
void exit_arch(void);
void exit_arch_ap(void);

void clear_bss(void);

void shell(void);

#endif /* !__ASSEMBLY__ */



/*
 * Common methods and definitions
 *
 * Copyright (C) 2009-2012 Ahmed S. Darwish <darwish.07@gmail.com>
 *
 */

#ifndef __ASSEMBLY__


__noreturn void panic(const char *fmt, ...);


#include <stdbool.h>

#if CONFIG_ARCH_SIM

#include <assert.h>

#else

#define NULL  ((void *) 0)
//#define bool  _Bool
//#define true    ((_Bool) 1)
//#define false   ((_Bool) 0)


#ifdef EOF
#undef EOF
#define EOF -1
#endif /* EOF */


/*
 * Critical failures
 */
extern void halt_cpu_ipi_handler(void);

#define assert(condition)                       \
  do {                                          \
    if (__unlikely(!(condition)))               \
      panic("%s:%d - !(" #condition ")\n",      \
            __FILE__, __LINE__);                \
  } while (0);


/*
 * Compiler memory barrier (fence)
 *
 * The 'memory' constraint will "cause GCC to not keep memory
 * values cached in registers across the assembler instruction
 * and not optimize stores or loads to that memory." --GCC
 */
//#define barrier() asm volatile ("":::"memory");

/*
 * For spin-loops, use x86 'pause' and a memory barrier to:
 * - force gcc to reload any values from memory over the busy
 *   loop, avoiding the often-buggy C volatile keyword
 * - hint the CPU to avoid useless memory ordering violations
 * - for Pentium 4, reduce power usage in the busy loop state
 */
#define cpu_pause()                             \
  asm volatile ("pause":::"memory");

/*
 * Compile-time assert for constant-folded expressions
 *
 * We would've been better using GCC's error(msg) attribute,
 * but it doesn't work with my current GCC build :(.
 */
__unused void __undefined_method(void);
#define compiler_assert(condition)              \
  do {                                          \
    if (!(condition))                           \
      __undefined_method();                     \
  } while (0);

#define __arr_size(arr)  (sizeof(arr) / sizeof((arr)[0]))

/*
 * Return length of given array's first dimension.
 *
 * Per C99 spec, the 'sizeof' operator returns an unsigned
 * integer. This is bothersome in the very common idiom:
 *
 *     for (int i = 0; i < ARRAY_SIZE(arr); i++)
 *
 * as it now entails a comparison between a signed and an
 * unsigned value. Thus, _safely_ cast the overall division
 * result below to a signed 32-bit int.
 */
#define ARRAY_SIZE(arr)                                       \
  ({                                                          \
    compiler_assert(__arr_size(arr) <= (uint64_t)INT32_MAX);  \
    (int32_t)__arr_size(arr);                                 \
  })

void __noreturn kernel_start(void);

#endif /* !CONFIG_ARCH_SIM */


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_KERNEL_H__ */
