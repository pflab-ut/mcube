/**
 * @file include/x86_64/x86_64.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_64_X86_64_H__
#define __MCUBE_X86_64_X86_64_H__

/*
 * General x86_64 properties
 *
 * Copyright (C) 2009 Ahmed S. Darwish <darwish.07@gmail.com>
 */

/**
 * @def MSR_FS_BASE
 * @brief MSR FS base.
 */
#define MSR_FS_BASE 0xc0000100

/**
 * @def MSR_GS_BASE
 * @brief MSR GS base.
 */
#define MSR_GS_BASE 0xc0000101

/* Machine-specific registers (MSRs) */
/**
 * @def MSR_APIC_BASE
 * @brief MSR APIC base.
 */
#define MSR_APIC_BASE 0x1b

/**
 * @def MSR_PLATFORM_INFO
 * @brief MSR platform information.
 */
#define MSR_PLATFORM_INFO 0xce

/**
 * @def MSR_IA32_EFER
 * @brief MSR IA32 efer.
 */
#define MSR_IA32_EFER 0xc0000080

/**
 * @def MSR_IA32_STAR
 * @brief MSR IA32 star.
 */
#define MSR_IA32_STAR 0xc0000081

/**
 * @def MSR_IA32_LSTAR
 * @brief MSR IA32 lstar.
 */
#define MSR_IA32_LSTAR 0xc0000082

/**
 * @def MSR_IA32_FMASK
 * @brief MSR IA32 fmask.
 */
#define MSR_IA32_FMASK 0xc0000084

#ifndef __ASSEMBLY__

/**
 * @fn static inline union rflags get_rflags(void)
 * @brief get rflags.
 *
 * @return Rflags.
 */
static inline union rflags get_rflags(void)
{
  union rflags flags;

  asm volatile("pushfq;"
               "popq %0;"
               :"=rm"(flags.raw)
               :);

  return flags;
}

/**
 * @fn static inline void set_rflags(union rflags flags)
 * @brief Setting %rflags may enable interrupts, but we often want to
 * do so in the _exact_ location specified: e.g. spin_unlock()
 * should be compiled to mark the lock as available (lock->val
 * = 1) before enabling interrupts, but never after.
 *
 * @param rflags Rflags.
 */
static inline void set_rflags(union rflags flags)
{
  asm volatile("pushq %0;"
               "popfq;"
               :
               :"g"(flags.raw)
               :"cc", "memory");
}

/**
 * @fn static inline union rflags default_rflags(void)
 * @brief Default rflags: set it to rflags of new threads, etc
 * This is same as the CPU rflags value following RESET or
 * INIT SIPI, with the difference of having IRQs enabled.
 *
 * @return Default rflags.
 */
static inline union rflags default_rflags(void)
{
  union rflags flags;

  flags.raw = 0;
  flags.__reserved1_0 = 1;
  flags.irqs_enabled = 1;

  return flags;
}

/*
 * The given FS.base and GS.base values must be in canonical
 * form or a general-protection (#GP) exception will occur.
 */

/**
 * @fn static inline void set_fs(uint64_t val)
 * @brief set FS.
 *
 * @param val Value.
 */
static inline void set_fs(uint64_t val)
{
  wrmsr(MSR_FS_BASE, val);
}

/**
 * @fn static inline void set_gs(uint64_t val)
 * @brief set GS.
 *
 * @param val Value.
 */
static inline void set_gs(uint64_t val)
{
  wrmsr(MSR_GS_BASE, val);
}

/**
 * @fn static inline uint64_t get_gs(void)
 * @brief get GS.
 */
static inline uint64_t get_gs(void)
{
  return rdmsr(MSR_GS_BASE);
}

#endif /* !__ASSEMBLY__ */
#endif /* __MCUBE_X86_64_X86_64_H__ */
