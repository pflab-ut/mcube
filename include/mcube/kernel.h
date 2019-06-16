/**
 * @file include/mcube/kernel.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_KERNEL_H__
#define __MCUBE_MCUBE_KERNEL_H__


/**
 * @def SYMBOL_NAME(name)
 * @brief Symbol name.
 *
 * @param name Symbol name.
 */
#define SYMBOL_NAME(name) name

/**
 * @def SYMBOL_NAME_LABEL(name)
 * @brief Symbol name's label.
 *
 * @param name Symbol name's label.
 */
#define SYMBOL_NAME_LABEL(name) name##:

/**
 * @def ENTRY(name)
 * @brief Entry.
 *
 * @param name Entry name.
 */
#define ENTRY(name)                             \
  .globl name;                                  \
  SYMBOL_NAME_LABEL(name)


/**
 * @def END(name)
 * @brief End.
 *
 * @param name End name.
 */
#define END(name)                               \
  .size name, .-name


/**
 * @def ASM_SIZE_DIRECTIVE(name)
 * @brief Assembler size directive.
 *
 * @param name Directive name.
 */
#define ASM_SIZE_DIRECTIVE(name) .size name,.-name;


#if CONFIG_ARCH_X86_64
/**
 * @def asmlinkage
 * @brief Asmlinkage.
 */
#define asmlinkage __attribute__((regparm(0)))
#else
/**
 * @def asmlinkage
 * @brief Asmlinkage.
 */
#define asmlinkage
#endif /* CONFIG_ARCH_X86_64 */


/**
 * @def NR_TASKS
 * @brief Number of tasks.
 */
#define NR_TASKS   16

/**
 * @def NR_THREADS
 * @brief Number of threads.
 */
#define NR_THREADS 16

/**
 * @def NR_ARGS
 * @brief Number of arguments.
 */
#define NR_ARGS 16


/**
 * @def KERNEL_STACK_SIZE
 * @brief Kernel stack size.
 */
#define KERNEL_STACK_SIZE 0x1000

/**
 * @def USER_STACK_SIZE
 * @brief User stack size.
 */
#define USER_STACK_SIZE 0x1000

/**
 * @def STACK_ALIGN(x)
 * @brief Stack alignment.
 *
 * @param x Data.
 */
#define STACK_ALIGN(x)  (((x) + 7) & -8)

/**
 * @def NR_PRIORITIES
 * @brief Number of priorities.
 */
#define NR_PRIORITIES NR_THREADS

/**
 * @def NR_PRIORITY_BITMAPS
 * @brief Number of priority bitmaps; each bitmap length is 32 bit.
 */
#define NR_PRIORITY_BITMAPS CEIL(NR_PRIORITIES, 32)


#ifndef  __ASSEMBLY__

/*
 * Check if given 'x' value is 'n'-aligned
 * 'n' must be power of the radix 2.
 */
/**
 * @def __MASK(x, n)
 * @brief Mask @a x with @a n alignment.
 *
 * @param x Data.
 * @param n Alignment.
 */
#define __MASK(x, n) ((typeof(x))((n) - 1))

/**
 * @def IS_ALIGNED(x, n)
 * @brief Is @a x aligned with @a n alignment?
 *
 * @param x Data.
 * @param n Alignment.
 */
#define IS_ALIGNED(x, n) (((x) & __MASK(x, n)) == 0)


/**
 * @def inf_loop()
 * @brief Infinite loop.
 */
#define inf_loop() do {                                                 \
    print("%s:%s():%d %s\n", __FILE__, __func__, __LINE__, "inf_loop()"); \
    if (call_sys_get_mode_level() != USER_LEVEL) {                      \
      disable_local_irq();                                              \
    }                                                                   \
    for (;;)                                                            \
      ;                                                                 \
  } while (0)

/**
 * @var Debug
 * @brief Debug flag.
 */
extern bool Debug;

/**
 * @fn int main(int argc, char *argv[])
 * @brief Main.
 *
 * @param argc Number of arguments.
 * @param argv Pointers to arguments.
 * @return zero if success, and nonzero if failure.
 */
int main(int argc, char *argv[]);

/**
 * @fn void init_arch(void)
`* @brief initialize architecture.
 */
void init_arch(void);

/**
 * @fn void init_arch_ap(void)
`* @brief initialize architecture on Application Processors (APs).
 */
void init_arch_ap(void);

/**
 * @fn void exit_arch(void)
`* @brief exit architecture.
 */
void exit_arch(void);

/**
 * @fn void exit_arch_ap(void)
`* @brief exit architecture on Application Processors (APs).
 */
void exit_arch_ap(void);

/**
 * @fn void clear_bss(void)
 * @brief clear BSS.
 */
void clear_bss(void);

/**
 * @fn void shell(void)
 * @brief do shell.
 */
void shell(void);


/*
 * Common methods and definitions
 *
 * Copyright (C) 2009-2012 Ahmed S. Darwish <darwish.07@gmail.com>
 *
 */

/**
 * @fn __noreturn void panic(const char *fmt, ...)
 * @brief panic.
 *
 * @param fmt Format.
 */
__noreturn void panic(const char *fmt, ...);


#if !CONFIG_ARCH_SIM


#ifndef NULL
/**
 * @def NULL
 * @brief NULL.
 */
#define NULL  ((void *) 0)
#endif /* !NULL */


#ifndef EOF
/**
 * @def EOF
 * @brief End Of File (EOF).
 */
#define EOF -1
#endif /* !EOF */


/**
 * @fn void halt_cpu_ipi_handler(void)
 * @brief Critical failures.
 */
void halt_cpu_ipi_handler(void);

/**
 * @def assert(condition)
 * @brief assert.
 *
 * @param condition Condition.
 */
#define assert(condition)                       \
  do {                                          \
    if (__unlikely(!(condition))) {             \
      panic("%s:%d - !(" #condition ")\n",      \
            __FILE__, __LINE__);                \
    }                                           \
  } while (0);


/**
 * @def barrier()
 * @brief Compiler memory barrier (fence).
 * The 'memory' constraint will "cause GCC to not keep memory
 * values cached in registers across the assembler instruction
 * and not optimize stores or loads to that memory." --GCC
 */
#define barrier() asm volatile("":::"memory")

/**
 * @fn __unused void __undefined_method(void)
 * @brief Undefined method for compile-time assert.
 */
__unused void __undefined_method(void);

/**
 * @def compiler_assert(condition)
 * @brief Compile-time assert for constant-folded expressions
 * We would've been better using GCC's error(msg) attribute,
 * but it doesn't work with my current GCC build :(.
 */
#define compiler_assert(condition)              \
  do {                                          \
    if (!(condition)) {                         \
      __undefined_method();                     \
    }                                           \
  } while (0);

/**
 * @def __arr_size(arr)
 * @brief Array size.
 *
 * @param arr Array.
 */
#define __arr_size(arr) (sizeof(arr) / sizeof((arr)[0]))

/**
 * @def ARRAY_SIZE(arr)
 * @brief Array size with compile-time assert.
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
 *
 * @param arr Array.
 */
#define ARRAY_SIZE(arr)                                       \
  ({                                                          \
    compiler_assert(__arr_size(arr) <= (uint64_t) INT32_MAX); \
    (int32_t)__arr_size(arr);                                 \
  })

/**
 * @fn __noreturn void kernel_start(void)
 * @brief Kernel start.
 */
__noreturn void kernel_start(void);

#endif /* !CONFIG_ARCH_SIM */



#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_KERNEL_H__ */
