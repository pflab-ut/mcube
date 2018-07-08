/**
 * @file include/mcube/bitops.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_BITOPS_H__
#define __MCUBE_MCUBE_BITOPS_H__

#ifndef __ASSEMBLY__

/* find first bit */
static inline uint64_t ffb64(uint64_t bitmap)
{
	//	PDEBUG("ffs(): bitmap = %x\n", bitmap);
	//	asm volatile("bsf %1, %0" :"=r" (bitmap) :"rm" (bitmap));
	int num = 0;

	if ((bitmap & 0xffffffffffffffff) == 0) {
		num += 64;
    bitmap = 0;
	}
	if ((bitmap & 0xffffffff) == 0) {
		num += 32;
		bitmap >>= 32;
	}
	if ((bitmap & 0xffff) == 0) {
		num += 16;
		bitmap >>= 16;
	}
	if ((bitmap & 0xff) == 0) {
		num += 8;
		bitmap >>= 8;
	}
	if ((bitmap & 0xf) == 0) {
		num += 4;
		bitmap >>= 4;
	}
	if ((bitmap & 0x3) == 0) {
		num += 2;
		bitmap >>= 2;
	}
	if ((bitmap & 0x1) == 0) {
		num += 1;
	}
	return num;
}

static inline uint64_t find_first_bit64(uint64_t *b, int nr)
{
	int i;
	for (i = 0; i < nr; i++) {
		if (b[i]) {
			return 64 * i + ffb64(b[i]);
		}
	}
	return NR_PRIORITIES;
}

static inline uint64_t find_first_zero_bit64(uint64_t *b, int nr)
{
	int i;
	for (i = 0; i < nr; i++) {
		if (~b[i]) {
			return 64 * i + ffb64(~b[i]);
		}
	}
	return NR_PRIORITIES;
}



/* find last bit */
static inline uint64_t flb64(uint64_t bitmap)
{
	int num = 63;

	if (!(bitmap & 0xffffffff00000000)) {
		num -= 32;
		bitmap <<= 32;
	}
	if (!(bitmap & 0xffffffffffff0000)) {
		num -= 16;
		bitmap <<= 16;
	}
	if (!(bitmap & 0xffffffffffffff00)) {
		num -= 8;
		bitmap <<= 8;
	}
	if (!(bitmap & 0xfffffffffffffff0)) {
		num -= 4;
		bitmap <<= 4;
	}
	if (!(bitmap & 0xfffffffffffffffc)) {
		num -= 2;
		bitmap <<= 2;
	}
	if (!(bitmap & 0xfffffffffffffffe)) {
		num -= 1;
  }
	return num;
}

static inline uint64_t find_last_bit64(uint64_t *b, int nr)
{
	int i;
	for (i = nr - 1; i >= 0; i--) {
		if (b[i]) {
      return 64 * i + flb64(b[i]);
		}
	}
	return NR_PRIORITIES;
}

static inline uint64_t find_last_zero_bit64(uint64_t *b, int nr)
{
	int i;
	for (i = nr - 1; i >= 0; i--) {
		if (~b[i]) {
      return 64 * i + flb64(b[i]);
		}
	}
	return NR_PRIORITIES;
}

static inline void set_bit64(volatile uint64_t *addr, int nr)
{
	int mask;
	uint64_t flags;

	addr += nr >> 6;
	mask = 1 << (nr & 0x3f);
	save_local_irq64(&flags);
	//	PDEBUG("addr = %x mask = %x\n", addr, mask);
	*addr |= mask;
	restore_local_irq64(&flags);
}


static inline void clear_bit64(volatile uint64_t *addr, int nr)
{
	int mask;
	uint64_t flags;

	addr += nr >> 6;
	mask = 1 << (nr & 0x3f);
	save_local_irq64(&flags);
	//	PDEBUG("addr = %x mask = %x\n", addr, mask);
	*addr &= ~mask;
	restore_local_irq64(&flags);
}

static inline void change_bit64(volatile uint64_t *addr, int nr)
{
	int mask;
	uint64_t flags;

	addr += nr >> 6;
	mask = 1 << (nr & 0x3f);
	save_local_irq64(&flags);
	*addr ^= mask;
	restore_local_irq64(&flags);
}

static inline int test_and_set_bit64(volatile uint64_t *addr, int nr)
{
	int mask, retval;
	uint64_t flags;

	addr += nr >> 6;
	mask = 1 << (nr & 0x3f);
	save_local_irq64(&flags);
	retval = (mask & *addr) != 0;
	*addr |= mask;
	restore_local_irq64(&flags);

	return retval;
}

static inline int test_and_clear_bit64(volatile uint64_t *addr, int nr)
{
	int mask, retval;
	uint64_t flags;

	addr += nr >> 6;
	mask = 1 << (nr & 0x3f);
	save_local_irq64(&flags);
	retval = (mask & *addr) != 0;
	*addr &= ~mask;
	restore_local_irq64(&flags);

	return retval;
}

static inline int test_and_change_bit64(volatile uint64_t *addr, int nr)
{
	int mask, retval;
	uint64_t flags;

	addr += nr >> 6;
	mask = 1 << (nr & 0x3f);
	save_local_irq64(&flags);
	retval = (mask & *addr) != 0;
	*addr ^= mask;
	restore_local_irq64(&flags);

	return retval;
}


/* find first bit */
static inline uint32_t ffb(uint32_t bitmap)
{
	//	PDEBUG("ffs(): bitmap = %x\n", bitmap);
	//	asm volatile("bsf %1, %0" :"=r" (bitmap) :"rm" (bitmap));
	int num = 0;

	if ((bitmap & 0xffff) == 0) {
		num += 16;
		bitmap >>= 16;
	}
	if ((bitmap & 0xff) == 0) {
		num += 8;
		bitmap >>= 8;
	}
	if ((bitmap & 0xf) == 0) {
		num += 4;
		bitmap >>= 4;
	}
	if ((bitmap & 0x3) == 0) {
		num += 2;
		bitmap >>= 2;
	}
	if ((bitmap & 0x1) == 0) {
		num += 1;
	}
	return num;
}


static inline uint32_t find_first_bit(uint32_t *b, int nr)
{
	int i;
	for (i = 0; i < nr; i++) {
		if (b[i]) {
			return 32 * i + ffb(b[i]);
		}
	}
	return 32 * nr;
}

static inline uint32_t find_first_zero_bit(uint32_t *b, int nr)
{
	int i;
	for (i = 0; i < nr; i++) {
		if (~b[i]) {
			return 32 * i + ffb(~b[i]);
		}
	}
	return 32 * nr;
}


/* find last bit */
static inline uint32_t flb(uint32_t bitmap)
{
	int num = 31;

	if (!(bitmap & (~0ul << 16))) {
		num -= 16;
		bitmap <<= 16;
	}
	if (!(bitmap & (~0ul << 24))) {
		num -= 8;
		bitmap <<= 8;
	}
	if (!(bitmap & (~0ul << 28))) {
		num -= 4;
		bitmap <<= 4;
	}
	if (!(bitmap & (~0ul << 30))) {
		num -= 2;
		bitmap <<= 2;
	}
	if (!(bitmap & (~0ul << 31)))
		num -= 1;
	return num;
}

static inline uint32_t find_last_bit(uint32_t *b, int nr)
{
	int i;
	for (i = nr - 1; i >= 0; i--) {
		if (b[i]) {
			return 32 * i + flb(b[i]);
		}
	}
	return 32 * nr;
}


static inline void set_bit(volatile uint32_t *addr, int nr)
{
	int mask;
  //	uint32_t flags;

	addr += nr >> 5;
	mask = 1 << (nr & 0x1f);
  //	save_local_irq(&flags);
	//	PDEBUG("addr = %x mask = %x\n", addr, mask);
	*addr |= mask;
  //	restore_local_irq(&flags);
}


static inline void clear_bit(volatile uint32_t *addr, int nr)
{
	int mask;
  //	uint32_t flags;

	addr += nr >> 5;
	mask = 1 << (nr & 0x1f);
  //	save_local_irq(&flags);
	//	PDEBUG("addr = %x mask = %x\n", addr, mask);
	*addr &= ~mask;
  //	restore_local_irq(&flags);
}

static inline void change_bit(volatile uint32_t *addr, int nr)
{
	int mask;
  //	uint32_t flags;

	addr += nr >> 5;
	mask = 1 << (nr & 0x1f);
  //	save_local_irq(&flags);
	*addr ^= mask;
  //	restore_local_irq(&flags);
}

static inline int test_and_set_bit(volatile uint32_t *addr, int nr)
{
	int mask, retval;
  //	uint32_t flags;

	addr += nr >> 5;
	mask = 1 << (nr & 0x1f);
  //	save_local_irq(&flags);
	retval = (mask & *addr) != 0;
	*addr |= mask;
  //	restore_local_irq(&flags);

	return retval;
}

static inline int test_and_clear_bit(volatile uint32_t *addr, int nr)
{
	int mask, retval;
  //	uint32_t flags;

	addr += nr >> 5;
	mask = 1 << (nr & 0x1f);
  //	save_local_irq(&flags);
	retval = (mask & *addr) != 0;
	*addr &= ~mask;
  //	restore_local_irq(&flags);

	return retval;
}

static inline int test_and_change_bit(volatile uint32_t *addr, int nr)
{
	int mask, retval;
  //	uint32_t flags;

	addr += nr >> 5;
	mask = 1 << (nr & 0x1f);
  //	save_local_irq(&flags);
	retval = (mask & *addr) != 0;
	*addr ^= mask;
  //	restore_local_irq(&flags);

	return retval;
}

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_BITOPS_H__ */
