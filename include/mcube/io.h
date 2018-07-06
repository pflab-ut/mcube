/**
 * @file include/mcube/io.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_IO_H__
#define	__MCUBE_MCUBE_IO_H__

#ifndef __ASSEMBLY__

#define mmio_in8(addr) (*(volatile uint8_t *) (addr))
#define mmio_in16(addr) (*(volatile uint16_t *) (addr))
#define mmio_in32(addr) (*(volatile uint32_t *) (addr))
#define mmio_in64(addr) (*(volatile uint64_t *) (addr))

#define mmio_out8(addr, data) (*(volatile uint8_t *) (addr) = (uint8_t) (data))
#define mmio_out16(addr, data) (*(volatile uint16_t *) (addr) = (uint16_t) (data))
#define mmio_out32(addr, data) (*(volatile uint32_t *) (addr) = (uint32_t) (data))
#define mmio_out64(addr, data) (*(volatile uint64_t *) (addr) = (uint64_t) (data))



#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_MCUBE_IO_H__ */
