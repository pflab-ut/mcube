/**
 * @file include/x86/segment.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_SEGMENT_H__
#define __MCUBE_X86_SEGMENT_H__

/*
 * Segmentation definitions; minimal by the nature of x86-64
 *
 * Copyright (C) 2009 Ahmed S. Darwish <darwish.07@gmail.com>
 */

#define KERNEL_CS  0x08
#define KERNEL_DS  0x10
#define KERNEL_CS16  0x18
#define KERNEL_DS16  0x20

#ifndef __ASSEMBLY__


struct gdt_descriptor {
  uint16_t limit;
  uint64_t base;
} __packed;

/*
 * FIXME: Reload the segment caches with the new GDT
 * values. Just changing the GDTR won't cut it.
 */
static inline void load_gdt(const struct gdt_descriptor *gdt_desc)
{
  asm volatile("lgdt %0"
         :
         :"m"(*gdt_desc));
}

static inline struct gdt_descriptor get_gdt(void)
{
  struct gdt_descriptor gdt_desc;

  asm volatile("sgdt %0"
         :"=m"(gdt_desc)
         :);

  return gdt_desc;
}

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_SEGMENT_H__ */
