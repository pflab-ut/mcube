/**
 * @file include/x86/x86.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_X86_H__
#define __MCUBE_X86_X86_H__

/*
 * General x86 properties
 *
 * Copyright (C) 2009 Ahmed S. Darwish <darwish.07@gmail.com>
 */

#define MSR_FS_BASE  0xC0000100
#define MSR_GS_BASE  0xC0000101

#ifndef __ASSEMBLY__


static inline union rflags get_rflags(void)
{
  union rflags flags;

  asm volatile("pushfq;"
               "popq %0;"
               :"=rm"(flags.raw)
               :);

  return flags;
}

/*
 * Setting %rflags may enable interrupts, but we often want to
 * do so in the _exact_ location specified: e.g. spin_unlock()
 * should be compiled to mark the lock as available (lock->val
 * = 1) before enabling interrupts, but never after.
 */
static inline void set_rflags(union rflags flags)
{
  asm volatile("pushq %0;"
               "popfq;"
               :
               :"g"(flags.raw)
               :"cc", "memory");
}

/*
 * Default rflags: set it to rflags of new threads, etc
 * This is same as the CPU rflags value following #RESET or
 * INIT SIPI, with the difference of having IRQs enabled.
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

static inline void set_fs(uint64_t val)
{
  wrmsr(MSR_FS_BASE, val);
}

static inline void set_gs(uint64_t val)
{
  wrmsr(MSR_GS_BASE, val);
}

static inline uint64_t get_gs(void)
{
  return rdmsr(MSR_GS_BASE);
}

#endif /* !__ASSEMBLY__ */
#endif /* __MCUBE_X86_X86_H__ */
