/**
 * @file include/mcube/io.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_IO_H__
#define __MCUBE_MCUBE_IO_H__

#ifndef __ASSEMBLY__


/**
 * @def mmio_in8(addr)
 * @brief Memory-mapped I/O input with 8-bits.
 *
 * @param addr Address.
 */
#define mmio_in8(addr) (*(volatile uint8_t *) (unsigned long) (addr))

/**
 * @def mmio_in16(addr)
 * @brief Memory-mapped I/O input with 16-bits.
 *
 * @param addr Address.
 */
#define mmio_in16(addr) (*(volatile uint16_t *) (unsigned long) (addr))

/**
 * @def mmio_in32(addr)
 * @brief Memory-mapped I/O input with 32-bits.
 *
 * @param addr Address.
 */
#define mmio_in32(addr) (*(volatile uint32_t *) (unsigned long) (addr))

/**
 * @def mmio_in64(addr)
 * @brief Memory-mapped I/O input with 64-bits.
 *
 * @param addr Address.
 */
#define mmio_in64(addr) (*(volatile uint64_t *) (unsigned long) (addr))

/**
 * @def mmio_out8(addr, data)
 * @brief Memory-mapped I/O output with 8-bits.
 *
 * @param addr Address.
 * @param data Data.
 */
#define mmio_out8(addr, data) (*(volatile uint8_t *) (unsigned long) (addr) = (uint8_t) (data))

/**
 * @def mmio_out16(addr, data)
 * @brief Memory-mapped I/O output with 16-bits.
 *
 * @param addr Address.
 * @param data Data.
 */
#define mmio_out16(addr, data) (*(volatile uint16_t *) (unsigned long) (addr) = (uint16_t) (data))

/**
 * @def mmio_out32(addr, data)
 * @brief Memory-mapped I/O output with 32-bits.
 *
 * @param addr Address.
 * @param data Data.
 */
#define mmio_out32(addr, data) (*(volatile uint32_t *) (unsigned long) (addr) = (uint32_t) (data))

/**
 * @def mmio_out64(addr, data)
 * @brief Memory-mapped I/O output with 64-bits.
 *
 * @param addr Address.
 * @param data Data.
 */
#define mmio_out64(addr, data) (*(volatile uint64_t *) (unsigned long) (addr) = (uint64_t) (data))


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_IO_H__ */
