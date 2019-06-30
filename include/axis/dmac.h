/**
 * @file include/axis/dmac.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AXIS_DMAC_H__
#define __MCUBE_AXIS_DMAC_H__

/**
 * @def DMAC_CTRL
 * @brief DMAC control.
 */
#define DMAC_CTRL (DMAC_BASE + 0x0)

/**
 * @def DMAC_TRANSFER_SIZE
 * @brief DMAC transfer size.
 */
#define DMAC_TRANSFER_SIZE (DMAC_BASE + 0x4)

/**
 * @def DMAC_SRC_ADDR
 * @brief DMAC source address. */
#define DMAC_SRC_ADDR (DMAC_BASE + 0x8)

/**
 * @def DMAC_DST_ADDR
 * @brief DMAC destination address.
 */
#define DMAC_DST_ADDR (DMAC_BASE + 0xc)

/**
 * @def DMAC_STATUS
 * @brief DMAC status.
 */
#define DMAC_STATUS (DMAC_BASE + 0x10)

/**
 * @def DMAC_STATUS_CLEAR
 * @brief DMAC status clear.
 */
#define DMAC_STATUS_CLEAR (DMAC_BASE + 0x14)

/**
 * @def DMAC_NET_BASE
 * @brief DMAC network base address.
 */
#define DMAC_NET_BASE (DMAC_BASE + 0x18)

/**
 * @def DMAC_NET_CTRL
 * @brief DMAC network control.
 */
#define DMAC_NET_CTRL (DMAC_BASE + 0x1c)


#ifndef __ASSEMBLY__

/**
 * @fn static inline void enable_dmac(void)
 * @brief enable DMAC.
 */
static inline void enable_dmac(void)
{
  mmio_out32(DMAC_CTRL, 1);
}

/**
 * @fn static inline void disable_dmac(void)
 * @brief disable DMAC.
 */
static inline void disable_dmac(void)
{
  mmio_out32(DMAC_CTRL, 0);
}

/**
 * @fn static inline uint32_t get_dmac_ctrl(void)
 * @brief get DMAC control.
 *
 * @return DMAC control.
 */
static inline uint32_t get_dmac_ctrl(void)
{
  volatile uint32_t data;
  data = mmio_in32(DMAC_CTRL);
  return data;
}

/**
 * @fn static inline uint32_t get_dmac_transfer_size(void)
 * @brief get DMAC transfer size.
 *
 * @return DMAC transfer size.
 */
static inline uint32_t get_dmac_transfer_size(void)
{
  volatile uint32_t data;
  data = mmio_in32(DMAC_TRANSFER_SIZE);
  return data;
}

/**
 * @fn static inline void set_dmac_transfer_size(volatile uint32_t data)
 * @brief set DMAC transfer size.
 *
 * @param data Data.
 */
static inline void set_dmac_transfer_size(volatile uint32_t data)
{
  mmio_out32(DMAC_TRANSFER_SIZE, data);
}

/**
 * @fn static inline uint32_t get_dmac_source_address(void)
 * @brief get DMAC source address.
 *
 * @return DMAC source address.
 */
static inline uint32_t get_dmac_source_address(void)
{
  volatile uint32_t data;
  data = mmio_in32(DMAC_SRC_ADDR);
  return data;
}

/**
 * @fn static inline void set_dmac_source_address(volatile uint32_t data)
 * @brief set DMAC source address.
 *
 * @param data Data.
 */
static inline void set_dmac_source_address(volatile uint32_t data)
{
  mmio_out32(DMAC_SRC_ADDR, data);
}

/**
 * @fn static inline uint32_t get_dmac_destination_address(void)
 * @brief get DMAC destination address.
 */
static inline uint32_t get_dmac_destination_address(void)
{
  volatile uint32_t data;
  data = mmio_in32(DMAC_DST_ADDR);
  return data;
}

/**
 * @fn static inline void set_dmac_destination_address(volatile uint32_t data)
 * @brief set DMAC destination address.
 *
 * @param data Data.
 */
static inline void set_dmac_destination_address(volatile uint32_t data)
{
  mmio_out32(DMAC_DST_ADDR, data);
}

/**
 * @fn static inline uint32_t get_dmac_status(void)
 * @brief get DMAC status.
 *
 * @return DMAC status.
 */
static inline uint32_t get_dmac_status(void)
{
  volatile uint32_t data;
  data = mmio_in32(DMAC_STATUS);
  return data;
}

/**
 * @fn static inline void set_dmac_status_clear(volatile uint32_t data)
 * @brief set DMAC status clear.
 *
 * @param data Data.
 */
static inline void set_dmac_status_clear(volatile uint32_t data)
{
  mmio_out32(DMAC_STATUS_CLEAR, data);
}

/**
 * @fn static inline uint32_t get_dmac_net_base(void)
 * @brief get DMAC net base.
 */
static inline uint32_t get_dmac_net_base(void)
{
  volatile uint32_t data;
  data = mmio_in32(DMAC_NET_BASE);
  return data;
}

/**
 * @fn static inline void set_dmac_net_base(volatile uint32_t data)
 * @brief set DMAC net base.
 *
 * @param data Data.
 */
static inline void set_dmac_net_base(volatile uint32_t data)
{
  mmio_out32(DMAC_NET_BASE, data);
}

/**
 * @fn static inline uint32_t get_dmac_net_ctrl(void)
 * @brief get DMAC net control.
 *
 * @return DMAC net control.
 */
static inline uint32_t get_dmac_net_ctrl(void)
{
  volatile uint32_t data;
  data = mmio_in32(DMAC_NET_CTRL);
  return data;
}

/**
 * @fn static inline void set_dmac_net_ctrl(volatile uint32_t data)
 * @brief set DMAC net control.
 *
 * @param data Data.
 */
static inline void set_dmac_net_ctrl(volatile uint32_t data)
{
  mmio_out32(DMAC_NET_CTRL, data);
}


#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_AXIS_DMAC_H__ */

