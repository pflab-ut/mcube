/**
 * @file arch/arm/raspi3/mm.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


void init_mm(void)
{
  LOW_MEMORY = 2 * SECTION_SIZE;
  HIGH_MEMORY = GPU_BASE;
  mem_map = (unsigned short *) (LOW_MEMORY - sizeof(short) * PAGING_PAGES);
}
