/**
 * @file user/test/test_vm.c
 *
 * @author Hiroyuki Chishiro
 */
/*
 * Memory Management: kernel virtual memory
 *
 * Copyright (C) 2010 Ahmed S. Darwish <darwish.07@gmail.com>
 *
 * So far, we've depended on the boot page tables being built early-on
 * by head.S setup code. Here, we build and apply our permanent mappings
 * for all kinds of kernel virtual addresses -- check paging.h
 *
 * NOTE! Always ask the page allocator for pages residing in the first
 * physical GB zone. Up to this point, _only_ virtual addresses represe-
 * nting that zone (beside kernel text addresses) were mapped by head.S
 */

#include <mcube/mcube.h>
#if CONFIG_ARCH_X86


/*
 * Kernel virtual memory test cases
 */


/*
 * Check if all physical memory is really
 * mapped as supossed to be; bailout otherwise
 */
static void vm_check_phys_memory(void)
{
  uint64_t phys_end;

  phys_end = e820_get_phys_addr_end();

  for (uintptr_t vaddr = KERN_PAGE_OFFSET;
       vaddr < (KERN_PAGE_OFFSET + phys_end); vaddr++) {

    /* This also assures that given address is mapped
     * to the expected physical address, according to
     * our kernel address space mapping rules */
    if (!vaddr_is_mapped((char *) vaddr)) {
      panic("_VM: Reporting supposedly mapped address 0x%lx "
            "as unmapped", vaddr);
    }
    /* Limit the output a bit .. */
    if (vaddr > (KERN_PAGE_OFFSET + 0x20000) &&
        is_aligned(vaddr, 0x200000)) {
      printk("Success: e820-avail phys range [0x%lx - 0x%lx] "
             "mapped\n", PHYS(vaddr - 0x200000), PHYS(vaddr));
    }
  }
}

/*
 * Check if 1-byte regions mapped using kmap()
 * are really mapped, with right values
 */
static void vm_check_kmap1(void)
{
  uintptr_t paddr;
  void *vaddr;
  int count;

  count = PAGE_SIZE_2MB * 10;
  paddr = 0x100000000000;

  while (count--) {
    vaddr = vm_kmap(paddr, 1);
    assert(vaddr == VIRTUAL(paddr));

    if (!vaddr_is_mapped(vaddr)) {
      panic("_VM: Reporting supposedly mapped address 0x%lx "
            "as unmapped", vaddr);
    }
    if (is_aligned(paddr, 0x200000)) {
      printk("Success: phys addrs [0x%lx - 0x%lx] mapped\n",
             paddr - 0x200000, paddr);
    }
    paddr++;
  }
}

/*
 * Check if variable length regions mapped using
 * kmap() are really mapped, with right values
 */
static void vm_check_kmap2(void)
{
  uintptr_t paddr;
  void *vaddr;
  int count, len;

  count = PAGE_SIZE_2MB * 10;
  paddr = 0x200000000000;

  for (len = 1; len <= count; len += PAGE_SIZE_2MB / 8) {

    /* To let the test be effective, assure we're
     * mapping previously unmapped address */
    assert(!vaddr_is_mapped((void *) round_up((uintptr_t) VIRTUAL(paddr), PAGE_SIZE_2MB)));

    vaddr = vm_kmap(paddr, len);
    assert(vaddr == VIRTUAL(paddr));

    for (int i = 0; i < len; i++) {
      if (!vaddr_is_mapped(vaddr)) {
        panic("_VM: Reporting supposedly mapped "
              "address 0x%lx as unmapped", vaddr);
      }
      vaddr++;
    }

    printk("Success: [region len=0x%lx] phys [0x%lx - 0x%lx] "
           "mapped\n", len, paddr, paddr + len);

    paddr += len;
  }
}

/*
 * Test cases driver
 */
bool test_vm(void)
{
  vm_check_phys_memory();
  vm_check_kmap1();
  vm_check_kmap2();
  return true;
}

#else

bool test_vm(void)
{
  return true;
}

#endif /* CONFIG_ARCH_X86 */

