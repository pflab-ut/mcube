/**
 * @file include/mcube/dmac.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_DMAC_H__
#define __MCUBE_MCUBE_DMAC_H__

#ifndef __ASSEMBLY__

/**
 * @enum dmac
 * @brief DMAC policy.
 */
enum dmac {
  DMAC_POLLING,
  DMAC_SYNC_INTERRUPT,
  DMAC_ASYNC_INTERRUPT,
  DMAC_END
};

void do_local_dmac(uint32_t dst,
                   uint32_t src,
                   size_t n,
                   unsigned int ch,
                   enum dmac policy);



#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_DMAC_H__ */
