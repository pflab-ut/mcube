/**
 * @file include/x86_64/vm_map.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_64_VM_MAP_H__
#define __MCUBE_X86_64_VM_MAP_H__

/*
 * Kernel virtual memory
 *
 * Copyright (C) 2010 Ahmed S. Darwish <darwish.07@gmail.com>
 */

#ifndef __ASSEMBLY__

/**
 * @fn void vm_init(void)
 * @brief initialize virtual memory.
 */
void vm_init(void);

/**
 * @fn void *vm_kmap(uintptr_t pstart, uint64_t len)
 * @brief map given physical range (@a pstart -> @a pstart+@a len)
 * at kernel physical mappings space.
 *
 * @param pstart Physical page start.
 * @param len Length.
 * @return Mapped virtual address.
 */
void *vm_kmap(uintptr_t pstart, uint64_t len);

/**
 * @fn bool vaddr_is_mapped(void *vaddr)
 * @brief Is virtual address mapped?
 *
 * @param vaddr Virtual address.
 */
bool vaddr_is_mapped(void *vaddr);

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_64_VM_MAP_H__ */
