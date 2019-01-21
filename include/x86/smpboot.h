/**
 * @file include/x86/smpboot.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_SMPBOOT_H__
#define __MCUBE_X86_SMPBOOT_H__
/*
 * Multiple-Processor (MP) Initialization
 *
 * Copyright (C) 2009-2010 Ahmed S. Darwish <darwish.07@gmail.com>
 */



#define SMPBOOT_START    0x10000  /* Trampoline start; 4k-aligned */

/*
 * AP cores parameters base and their offsets. To be used
 * in trampoline assembly code.
 */

#define SMPBOOT_PARAMS    0x20000  /* AP parameters base */

#define SMPBOOT_CR3    (0)
#define SMPBOOT_IDTR    (SMPBOOT_CR3 + 8)
#define SMPBOOT_IDTR_LIMIT  (SMPBOOT_IDTR)
#define SMPBOOT_IDTR_BASE  (SMPBOOT_IDTR_LIMIT + 2)
#define SMPBOOT_GDTR    (SMPBOOT_IDTR + 10)
#define SMPBOOT_GDTR_LIMIT  (SMPBOOT_GDTR)
#define SMPBOOT_GDTR_BASE  (SMPBOOT_GDTR_LIMIT + 2)
#define SMPBOOT_STACK_PTR  (SMPBOOT_GDTR + 10)
#define SMPBOOT_PERCPU_PTR  (SMPBOOT_STACK_PTR + 8)

#define SMPBOOT_PARAMS_END  (SMPBOOT_PARAMS + SMPBOOT_PERCPU_PTR + 8)
#define SMPBOOT_PARAMS_SIZE  (SMPBOOT_PARAMS_END - SMPBOOT_PARAMS)

#ifndef __ASSEMBLY__


#define TRAMPOLINE_START  VIRTUAL(SMPBOOT_START)
#define TRAMPOLINE_PARAMS  VIRTUAL(SMPBOOT_PARAMS)

void __no_return secondary_start(void); /* Silence-out GCC */
int smpboot_get_nr_alive_cpus(void);
void smpboot_init(void);
void smpboot_trigger_secondary_cores_testcases(void);

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_SMPBOOT_H__ */
