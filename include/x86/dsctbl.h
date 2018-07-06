/**
 * @file include/x86/dsctbl.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_DSCTBL_H__
#define __MCUBE_X86_DSCTBL_H__

#ifndef __ASSEMBLY__


/**
 * @brief Global descriptor
 *
 * The global_descriptor structure has global descriptor information.
 */
struct global_descriptor {
	/** Limit low. */
	uint16_t limit_low;
	/** Base low. */
	uint16_t base_low;
	/** Base middle. */
	uint8_t base_mid;
	/** Access right. */
	uint8_t access_right;
	/** Limit high. */
	uint8_t limit_high;
	/** Base high. */
	uint8_t base_high;
} __attribute__((packed));

typedef struct global_descriptor global_descriptor;

/**
 * @brief Interrupt descriptor
 *
 * The interrupt_descriptor structure has interrupt descriptor information.
 */
struct interrupt_descriptor {
	uint16_t offset_low, selector;
	uint8_t dw_count, access_right;
	uint16_t offset_high;
} __attribute__((packed));

typedef struct interrupt_descriptor interrupt_descriptor;

extern struct global_descriptor *gdt_start;
extern struct interrupt_descriptor *idt_start;


#define AR_DATA32_RW 0x4092
#define AR_CODE32_ER 0x409a
#define AR_INTGATE32 0x008e
#define AR_TSS32 0x0089

#define TASK_GDT0 3


/**
 * @brief Descriptor pointer
 *
 * The desc_ptr structure has descriptor pointer information.
 */
struct desc_ptr {
	/** Size. */
	uint16_t size;
	/** Address. */
	uint32_t addr;
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

extern void set_gdsc(struct global_descriptor *gd, uint32_t limit,
										 uint32_t base, uint32_t ar);
extern void set_idsc(struct interrupt_descriptor *id, uint32_t offset,
										 uint32_t selector, uint32_t ar);

extern void init_dsctbl(void);

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_DSCTBL_H__ */
