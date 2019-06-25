/**
 * @file include/x86_64/smpboot.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_64_SMPBOOT_H__
#define __MCUBE_X86_64_SMPBOOT_H__
/*
 * Multiple-Processor (MP) Initialization
 *
 * Copyright (C) 2009-2010 Ahmed S. Darwish <darwish.07@gmail.com>
 */


/**
 * @def SMPBOOT_START
 * @brief Trampoline start; 4k-aligned.
 */
#define SMPBOOT_START 0x10000

/*
 * AP cores parameters base and their offsets. To be used
 * in trampoline assembly code.
 */

/**
 * @def SMPBOOT_PARAMS
 * @brief AP parameters base.
 */
#define SMPBOOT_PARAMS 0x20000

/**
 * @def SMPBOOT_CR3
 * @brief SMP boot CR3.
 */
#define SMPBOOT_CR3 (0)

/**
 * @def SMPBOOT_IDTR
 * @brief SMP boot IDTR.
 */
#define SMPBOOT_IDTR (SMPBOOT_CR3 + 8)

/**
 * @def SMPBOOT_IDTR_LIMIT
 * @brief SMP boot IDTR limit.
 */
#define SMPBOOT_IDTR_LIMIT (SMPBOOT_IDTR)

/**
 * @def SMPBOOT_IDTR_BASE
 * @brief SMP boot IDTR base.
 */
#define SMPBOOT_IDTR_BASE (SMPBOOT_IDTR_LIMIT + 2)

/**
 * @def SMPBOOT_GDTR
 * @brief SMP boot GDTR.
 */
#define SMPBOOT_GDTR (SMPBOOT_IDTR + 10)

/**
 * @def SMPBOOT_GDTR_LIMIT
 * @brief SMP boot GDTR limit.
 */
#define SMPBOOT_GDTR_LIMIT (SMPBOOT_GDTR)

/**
 * @def SMPBOOT_GDTR_BASE
 * @brief SMP boot GDTR base.
 */
#define SMPBOOT_GDTR_BASE (SMPBOOT_GDTR_LIMIT + 2)

/**
 * @def SMPBOOT_STACK_PTR
 * @brief SMP boot stack pointer.
 */
#define SMPBOOT_STACK_PTR (SMPBOOT_GDTR + 10)

/**
 * @def SMPBOOT_PERCPU_PTR
 * @brief SMP boot per CPU pointer.
 */
#define SMPBOOT_PERCPU_PTR (SMPBOOT_STACK_PTR + 8)

/**
 * @def SMPBOOT_PARAMS_END
 * @brief SMP boot parameters end.
 */
#define SMPBOOT_PARAMS_END (SMPBOOT_PARAMS + SMPBOOT_PERCPU_PTR + 8)

/**
 * @def SMPBOOT_PARAMS_SIZE
 * @brief SMP boot parameters size.
 */
#define SMPBOOT_PARAMS_SIZE (SMPBOOT_PARAMS_END - SMPBOOT_PARAMS)

#ifndef __ASSEMBLY__

/*
 * Assembly trampoline code start and end pointers
 */
/**
 * @var trampoline[]
 * @brief Trampoline.
 */
extern const char trampoline[];

/**
 * @var trampoline_end[]
 * @brief Trampoline end.
 */
extern const char trampoline_end[];

/**
 * @struct smpboot_params
 * @brief Parameters to be sent to other AP cores.
 */
struct smpboot_params {
  /**
   * CR3.
   */
  uintptr_t cr3;

  /**
   * Interrupt descriptor table.
   */
  struct idt_descriptor idtr;

  /**
   * Global descriptor table.
   */
  struct gdt_descriptor gdtr;

  /* Unique values for each core */

  /**
   * Stack pointer.
   */
  char *stack_ptr;

  /**
   * Per CPU area pointer.
   */
  void *percpu_area_ptr;
} __packed /** packed. */ ;

/**
 * @fn static inline void smpboot_params_validate_offsets(void)
 * @brief validate the manually calculated parameters offsets
 * we're sending to the assembly trampoline code.
 */
static inline void smpboot_params_validate_offsets(void)
{
  compiler_assert(SMPBOOT_CR3
                  == offsetof(struct smpboot_params, cr3));

  compiler_assert(SMPBOOT_IDTR
                  == offsetof(struct smpboot_params, idtr));

  compiler_assert(SMPBOOT_IDTR_LIMIT
                  == offsetof(struct smpboot_params, idtr)
                  + offsetof(struct idt_descriptor, limit));

  compiler_assert(SMPBOOT_IDTR_BASE
                  == offsetof(struct smpboot_params, idtr)
                  + offsetof(struct idt_descriptor, base));

  compiler_assert(SMPBOOT_GDTR
                  == offsetof(struct smpboot_params, gdtr));

  compiler_assert(SMPBOOT_GDTR_LIMIT
                  == offsetof(struct smpboot_params, gdtr)
                  + offsetof(struct gdt_descriptor, limit));

  compiler_assert(SMPBOOT_GDTR_BASE
                  == offsetof(struct smpboot_params, gdtr)
                  + offsetof(struct gdt_descriptor, base));

  compiler_assert(SMPBOOT_STACK_PTR
                  == offsetof(struct smpboot_params, stack_ptr));

  compiler_assert(SMPBOOT_PERCPU_PTR
                  == offsetof(struct smpboot_params, percpu_area_ptr));

  compiler_assert(SMPBOOT_PARAMS_SIZE
                  == sizeof(struct smpboot_params));
}


/**
 * @def TRAMPOLINE_START
 * @brief Trampoine start.
 */
#define TRAMPOLINE_START VIRTUAL(SMPBOOT_START)

/**
 * @def TRAMPOLINE_PARAMS
 * @brief Trampoline parameters.
 */
#define TRAMPOLINE_PARAMS VIRTUAL(SMPBOOT_PARAMS)

/**
 * @fn __noreturn void secondary_start(void)
 * @brief Secondary start.
 */
__noreturn void secondary_start(void);

/**
 * @fn int smpboot_get_nr_alive_cpus(void)
 * @brief get Number of alive CPUs.
 *
 * @return Number of alive CPUs.
 */
int smpboot_get_nr_alive_cpus(void);

/**
 * @fn void smpboot_init(void)
 * @brief initiazlize SMP boot.
 * NOTE! This function is called by panic();
 * it should not include any asserts or panics.
 */
void smpboot_init(void);


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_64_SMPBOOT_H__ */
