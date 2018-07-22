/**
 * @file include/axis/dmac.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AXIS_DMAC_H__
#define __MCUBE_AXIS_DMAC_H__

/** DMAC control. */
#define DMAC_CTRL          (DMAC_BASE + 0x0)
/** DMAC transfer size. */
#define DMAC_TRANSFER_SIZE (DMAC_BASE + 0x4)
/** DMAC source address. */
#define DMAC_SRC_ADDR      (DMAC_BASE + 0x8)
/** DMAC destination address. */
#define DMAC_DST_ADDR      (DMAC_BASE + 0xc)
/** DMAC status. */
#define DMAC_STATUS        (DMAC_BASE + 0x10)
/** DMAC status clear. */
#define DMAC_STATUS_CLEAR  (DMAC_BASE + 0x14)
/** DMAC network base address. */
#define DMAC_NET_BASE      (DMAC_BASE + 0x18)
/** DMAC network control. */
#define DMAC_NET_CTRL      (DMAC_BASE + 0x1c)


#ifndef __ASSEMBLY__

static inline void enable_dmac(void)
{
  mmio_out32(DMAC_CTRL, 1);
}

static inline void disable_dmac(void)
{
  mmio_out32(DMAC_CTRL, 0);
}

static inline uint32_t get_dmac_ctrl(void)
{
  volatile uint32_t data;
  data = mmio_in32(DMAC_CTRL);
  return data;
}

static inline uint32_t get_dmac_transfer_size(void)
{
  volatile uint32_t data;
  data = mmio_in32(DMAC_TRANSFER_SIZE);
  return data;
}

static inline void set_dmac_transfer_size(volatile uint32_t data)
{
  mmio_out32(DMAC_TRANSFER_SIZE, data);
}

static inline uint32_t get_dmac_source_address(void)
{
  volatile uint32_t data;
  data = mmio_in32(DMAC_SRC_ADDR);
  return data;
}

static inline void set_dmac_source_address(volatile uint32_t data)
{
  mmio_out32(DMAC_SRC_ADDR, data);
}

static inline uint32_t get_dmac_destination_address(void)
{
  volatile uint32_t data;
  data = mmio_in32(DMAC_DST_ADDR);
  return data;
}

static inline void set_dmac_destination_address(volatile uint32_t data)
{
  mmio_out32(DMAC_DST_ADDR, data);
}

static inline uint32_t get_dmac_status(void)
{
  volatile uint32_t data;
  data = mmio_in32(DMAC_STATUS);
  return data;
}

static inline void set_dmac_status_clear(volatile uint32_t data)
{
  mmio_out32(DMAC_STATUS_CLEAR, data);
}

static inline uint32_t get_dmac_net_base(void)
{
  volatile uint32_t data;
  data = mmio_in32(DMAC_NET_BASE);
  return data;
}

static inline void set_dmac_net_base(volatile uint32_t data)
{
  mmio_out32(DMAC_NET_BASE, data);
}

static inline uint32_t get_dmac_net_ctrl(void)
{
  volatile uint32_t data;
  data = mmio_in32(DMAC_NET_CTRL);
  return data;
}

static inline void set_dmac_net_ctrl(volatile uint32_t data)
{
  mmio_out32(DMAC_NET_CTRL, data);
}

enum dmac {
  DMAC_POLLING,
  DMAC_SYNC_INTERRUPT,
  DMAC_ASYNC_INTERRUPT,
  DMAC_END
};

void do_local_dmac(volatile uint32_t dst,
                   volatile uint32_t src,
                   volatile size_t n,
                   enum dmac policy);


#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_AXIS_DMAC_H__ */

