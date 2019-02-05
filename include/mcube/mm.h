/**
 * @file include/mcube/mm.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_MM_H__
#define __MCUBE_MCUBE_MM_H__


/* 4KB */
#define PAGE_SHIFT    12 
#define PAGE_SIZE (0x1 << PAGE_SHIFT)

/* 2MB */
#define SECTION_SIZE 0x200000

#ifndef __ASSEMBLY__

#define PAGE_ALIGNMENT __attribute__((aligned(PAGE_SIZE)))


extern unsigned long LOW_MEMORY;
extern unsigned long HIGH_MEMORY;

extern unsigned short *mem_map;
//unsigned long get_free_page(void);
//void free_page(unsigned long p);
void init_mm(void);

void memdump(void *ptr, size_t n);

#endif /* !__ASSEMBLY__ */


#define PAGING_MEMORY           (HIGH_MEMORY - LOW_MEMORY)
#define PAGING_PAGES            (PAGING_MEMORY / PAGE_SIZE)


#endif /* __MCUBE_MCUBE_MM_H__ */
