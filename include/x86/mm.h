/**
 * @file include/x86/mm.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_MM_H__
#define	__MCUBE_X86_MM_H__



#ifndef __ASSEMBLY__

/**
 *  @function       init_kmem
 * @brief          Using the contents of the physical memory map, identity
 *                 map all physical memory into the kernel's page table.
 * @param[inout]   pt  The pagetable structure to hold a description of the
 *                     kernel's page table.
 */
void init_kmem(pagetable_t *pt);

void *memzero(void *dst, size_t num);


#endif /* !__ASSEMBLY__ */


#endif	/* __MCUBE_X86_MM_H__ */
