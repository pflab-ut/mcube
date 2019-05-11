/**
 * @file include/mcube/kernel.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_KERNEL_H__
#define __MCUBE_MCUBE_KERNEL_H__


#define SYMBOL_NAME(name) name

#define SYMBOL_NAME_LABEL(name) name##:

#define ENTRY(name)                             \
  .globl name;                                  \
  SYMBOL_NAME_LABEL(name)


#define END(name)                               \
  .size name, .-name


#define ASM_SIZE_DIRECTIVE(name) .size name,.-name;



#if CONFIG_ARCH_X86
#define asmlinkage __attribute__((regparm(0)))
#else
#define asmlinkage
#endif /* CONFIG_ARCH_X86 */


#define NR_TASKS   16
#define NR_THREADS 16

/* 4K */
#define KERNEL_STACK_SIZE 0x1000
#define USER_STACK_SIZE 0x1000

#define STACK_ALIGN(x)  (((x) + 7) & -8)

#define NR_PRIORITIES NR_THREADS

/* each bitmap length is 32 bit */
#define NR_PRIORITY_BITMAPS CEIL(NR_PRIORITIES, 32)


#ifndef  __ASSEMBLY__

/*
 * Check if given 'x' value is 'n'-aligned
 * 'n' must be power of the radix 2.
 */
#define __MASK(x, n)    ((typeof(x))((n) - 1))
#define IS_ALIGNED(x, n)  (((x) & __MASK(x, n)) == 0)


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



/*
 * Common methods and definitions
 *
 * Copyright (C) 2009-2012 Ahmed S. Darwish <darwish.07@gmail.com>
 *
 */


__noreturn void panic(const char *fmt, ...);



#if !CONFIG_ARCH_SIM


#ifndef NULL
#define NULL  ((void *) 0)
#endif /* !NULL */


#ifndef EOF
#define EOF -1
#endif /* !EOF */


/*
 * Critical failures
 */
extern void halt_cpu_ipi_handler(void);

#define assert(condition)                       \
  do {                                          \
    if (__unlikely(!(condition))) {             \
      panic("%s:%d - !(" #condition ")\n",      \
            __FILE__, __LINE__);                \
    }                                           \
  } while (0);


/*
 * Compiler memory barrier (fence)
 *
 * The 'memory' constraint will "cause GCC to not keep memory
 * values cached in registers across the assembler instruction
 * and not optimize stores or loads to that memory." --GCC
 */
#define barrier() asm volatile("":::"memory")


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
    compiler_assert(__arr_size(arr) <= (uint64_t) INT32_MAX); \
    (int32_t)__arr_size(arr);                                 \
  })

void __noreturn kernel_start(void);

#endif /* !CONFIG_ARCH_SIM */



#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_KERNEL_H__ */
