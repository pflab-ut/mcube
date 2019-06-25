/**
 * @file include/x86_64/segment.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_64_SEGMENT_H__
#define __MCUBE_X86_64_SEGMENT_H__

/*
 * Segmentation definitions; minimal by the nature of x86_64
 *
 * Copyright (C) 2009 Ahmed S. Darwish <darwish.07@gmail.com>
 */

/* offset */

/**
 * @def KERNEL_CS
 * @brief Offset of kernel CS.
 */
#define KERNEL_CS 0x08

/**
 * @def KERNEL_DS
 * @brief Offset of kernel DS.
 */
#define KERNEL_DS 0x10

/**
 * @def KERNEL_CS16
 * @brief Offset of kernel CS16.
 */
#define KERNEL_CS16 0x18

/**
 * @def KERNEL_DS16
 * @brief Offset of kernel DS16.
 */
#define KERNEL_DS16 0x20


/* GDT selectors */

/**
 * @def GDT_NR
 * @brief Number of GDT selectors.
 */
#define GDT_NR 7

/**
 * @def GDT_NULL_SEL
 * @brief GDT NULL selectors.
 */
#define GDT_NULL_SEL (0 << 3)

/**
 * @def GDT_RING0_CODE_SEL
 * @brief GDT ring0 code selector.
 */
#define GDT_RING0_CODE_SEL (1 << 3)

/**
 * @def GDT_RING0_DATA_SEL
 * @brief GDT ring0 data selector.
 */
#define GDT_RING0_DATA_SEL (2 << 3)

/**
 * @def GDT_RING3_CODE32_SEL
 * @brief GDT ring3 code32 selector.
 */
#define GDT_RING3_CODE32_SEL (3 << 3)

/**
 * @def GDT_RING3_DATA32_SEL
 * @brief GDT ring3 data32 selector.
 */
#define GDT_RING3_DATA32_SEL (4 << 3)

/**
 * @def GDT_RING3_CODE64_SEL
 * @brief GDT ring3 code64 selector.
 */
#define GDT_RING3_CODE64_SEL (5 << 3)

/**
 * @def GDT_RING3_DATA64_SEL
 * @brief GDT ring3 data64 selector.
 */
#define GDT_RING3_DATA64_SEL (6 << 3)

/**
 * @def GDT_TSS_SEL_BASE
 * @brief GDT TSS selector base.
 */
#define GDT_TSS_SEL_BASE (7 << 3)


#ifndef __ASSEMBLY__


/**
 * @struct gdt_descriptor
 * @brief GDT descriptor information.
 */
struct gdt_descriptor {
  /**
   * Limit.
   */
  uint16_t limit;

  /**
   * Base.
   */
  uint64_t base;
} __packed /** packed. */;

/**
 * @fn static inline void load_gdt(const struct gdt_descriptor *gdt_desc)
 * @brief load GDT.
 *
 * FIXME: Reload the segment caches with the new GDT
 * values. Just changing the GDTR won't cut it.
 *
 * @param gdt_desc GDT descriptor.
 */
static inline void load_gdt(const struct gdt_descriptor *gdt_desc)
{
  asm volatile("lgdt %0"
               :
               :"m"(*gdt_desc));
}

/**
 * @fn static inline struct gdt_descriptor get_gdt(void)
 * @brief get GDT descriptor.
 *
 * @return GDT descriptor.
 */
static inline struct gdt_descriptor get_gdt(void)
{
  struct gdt_descriptor gdt_desc;

  asm volatile("sgdt %0"
               :"=m"(gdt_desc)
               :);

  return gdt_desc;
}

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_64_SEGMENT_H__ */
