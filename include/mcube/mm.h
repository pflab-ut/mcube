/**
 * @file include/mcube/mm.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_MM_H__
#define __MCUBE_MCUBE_MM_H__


/**
 * @def PAGE_SHIFT
 * @brief Page shift.
 */
#define PAGE_SHIFT 12

/**
 * @def PAGE_SIZE
 * @brief Page size.
 */ 
#define PAGE_SIZE (0x1 << PAGE_SHIFT)

/**
 * @def SECTION_SIZE
 * @brief Section size.
 */
#define SECTION_SIZE 0x200000

#ifndef __ASSEMBLY__

/**
 * @def PAGE_ALIGNMENT
 * @brief Page alignment.
 */
#define PAGE_ALIGNMENT __attribute__((aligned(PAGE_SIZE)))

/**
 * @var LOW_MEMORY
 * @brief Low memory address.
 */
extern unsigned long LOW_MEMORY;

/**
 * @var HIGH_MEMORY
 * @brief High memory address.
 */
extern unsigned long HIGH_MEMORY;

/**
 * @var mem_map
 * @brief Memory mapped address.
 */
extern unsigned short *mem_map;

/**
 * @fn void init_mm(void)
 * @brief initialize memory management.
 */
void init_mm(void);

/**
 * @fn void memdump(void *ptr, size_t n)
 * @brief Memory dump.
 */
void memdump(void *ptr, size_t n);

#endif /* !__ASSEMBLY__ */

/**
 * @def PAGING_MEMORY
 * @brief Paging memory.
 */
#define PAGING_MEMORY (HIGH_MEMORY - LOW_MEMORY)

/**
 * @def PAGING_PAGES
 * @brief Paging pages.
 */
#define PAGING_PAGES (PAGING_MEMORY / PAGE_SIZE)


#endif /* __MCUBE_MCUBE_MM_H__ */
