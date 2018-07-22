/**
 * @file include/x86/dsctbl.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_DSCTBL_H__
#define __MCUBE_X86_DSCTBL_H__

#ifndef __ASSEMBLY__


/**
 * @brief Descriptor pointer
 *
 * The desc_ptr structure has descriptor pointer information.
 */
struct desc_ptr {
	/** Size. */
	uint16_t size;
	/** Address. */
	uint64_t addr;
} __attribute__((packed));

typedef struct desc_ptr desc_ptr;

extern desc_ptr gdt, idt;

static inline void lgdt(const struct desc_ptr *dtr)
{
	asm volatile("lgdt %0"::"m" (*dtr));
}

static inline void lidt(const struct desc_ptr *dtr)
{
  asm volatile("lidt %0"::"m" (*dtr));
  //  asm volatile("lidt [%0]"::"r" (dtr));
}

static inline void sgdt(struct desc_ptr *dtr)
{
	asm volatile("sgdt %0":"=m" (*dtr));
}

static inline void sidt(struct desc_ptr *dtr)
{
	asm volatile("sidt %0":"=m" (*dtr));
}

static inline unsigned long str(void)
{
	uint32_t tr;
	asm volatile("str %0":"=r" (tr));
	return tr;
}


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_DSCTBL_H__ */
