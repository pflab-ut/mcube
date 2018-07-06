/**
 * @file lib/mm.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

unsigned long LOW_MEMORY;
unsigned long HIGH_MEMORY;
unsigned short *mem_map;

unsigned long get_free_page(void)
{
  int i;
	for (i = 0; i < PAGING_PAGES; i++) {
		if (mem_map[i] == 0){
			mem_map[i] = 1;
			return LOW_MEMORY + i * PAGE_SIZE;
		}
	}
	return 0;
}

void free_page(unsigned long p)
{
	mem_map[(p - LOW_MEMORY) / PAGE_SIZE] = 0;
}
