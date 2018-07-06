/**
 * @file arch/sim/tlb.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

void tlb_unmap_area(struct tlb_info *private_info, unsigned long top,
			unsigned long end)
{
}

int tlb_map_area(unsigned long pstart, unsigned long vstart, int length,
			struct tlb_info *private_info, unsigned long flag)
{
	return 0;
}
