/**
 * @file include/x86_64/vm.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_64_VM_H__
#define __MCUBE_X86_64_VM_H__

/*
 * Kernel virtual memory
 *
 * Copyright (C) 2010 Ahmed S. Darwish <darwish.07@gmail.com>
 */

#ifndef __ASSEMBLY__

void vm_init(void);
void *vm_kmap(uintptr_t pstart, uint64_t len);
bool vaddr_is_mapped(void *vaddr);

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_64_VM_H__ */
