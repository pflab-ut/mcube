/**
 * @file include/mcube/stdlib.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_STDLIB_H__
#define	__MCUBE_MCUBE_STDLIB_H__

#ifndef __ASSEMBLY__

typedef int (*sortcmp)(const void *a, const void *b);

#if CONFIG_ARCH_SIM

#include <stdlib.h>

#elif CONFIG_ARCH_X86 || CONFIG_ARCH_ARM || CONFIG_ARCH_AXIS

long strtol(const char *cp, char **endp, int base);
unsigned long strtoul(const char *cp, char **endp, int base);
char *itoa(int val, char *buf, int radix);
char *ltoa(long val, char *buf, int radix);
char *ultoa(unsigned long val, char *buf, int radix);
void exit(int status);


void *malloc(size_t size);
void free(void *objp);


void qsort(void *base, size_t num, size_t size, sortcmp cmp);

void init_malloc(void);

#endif /* CONFIG_ARCH_SIM */

/* 4k */
#define MALLOC_SIZE 0x1000

#define BLOCK_NUM 0x10
#define BLOCK_SIZE (MALLOC_SIZE / BLOCK_NUM)


extern unsigned char user_malloc[MALLOC_SIZE];


/**
 * @brief mem_block_header
 *
 * The mem_block structure has memory block information for dynamic allocation.
 */
struct mem_block_header {
  size_t size;
  unsigned is_free;
  struct mem_block_header *next;
};

typedef struct mem_block_header mem_block_header;





#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_MCUBE_STDLIB_H__ */
