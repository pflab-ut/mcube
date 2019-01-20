/**
 * @file include/x86/vm.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_VM_H__
#define __MCUBE_X86_VM_H__

/*
 * Kernel virtual memory
 *
 * Copyright (C) 2010 Ahmed S. Darwish <darwish.07@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 2.
 */

#ifndef __ASSEMBLY__

void vm_init(void);
void *vm_kmap(uintptr_t pstart, uint64_t len);

#if VM_TESTS

void vm_run_tests(void);

#else /* !VM_TESTS */

static inline void vm_run_tests(void) { }

#endif /* VM_TESTS */

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_VM_H__ */
