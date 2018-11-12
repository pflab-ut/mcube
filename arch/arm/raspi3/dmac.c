/**
 * @file arch/arm/raspi3/dmac.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


void init_dmac(void)
{
  printk("init_dmac()\n");
  /* clear interrupt status */
  mmio_out32(DMAC_INT_STATUS, 0);
  
}

struct dmac_info dinfo[NR_DMAC_CHS];

void do_local_dmac(uint32_t dst,
                   uint32_t src,
                   size_t n,
                   unsigned int ch,
                   enum dmac policy)
{
  unsigned long cpu = get_cpu_id();
  
  /* enable DMA channel [ch] */
  mmio_out32(DMAC_ENABLE, mmio_in32(DMAC_ENABLE) | DMAC_ENABLE_EN(ch));
  
  /* reset DMA channel [ch] */
  mmio_out32(DMAC_CH_CS(ch), mmio_in32(DMAC_CH_CS(ch)) | DMAC_CH_CS_RESET);

  /* set DMAC block as no wide bursts, src increment, and dst increment */
  dinfo[ch].transfer_information = DMAC_CH_TI_NO_WIDE_BURSTS
    | DMAC_CH_TI_SRC_INC
    | DMAC_CH_TI_DST_INC;

  if (policy == DMAC_SYNC_INTERRUPT) {
    dinfo[ch].transfer_information |= DMAC_CH_TI_INTEN;
  }
  
  dinfo[ch].src_addr = src;
  dinfo[ch].dst_addr = dst;

  dinfo[ch].next_control_block = 0;
  dinfo[ch].stride = 0;

  /* Multiples of 4 */
  dinfo[ch].transfer_length = n;
  
  dinfo[ch].debug = 0;
  dinfo[ch].padding = 0;

  mmio_out32(ENABLE_IRQS1, DMAC_IRQ(cpu));

  /* do dma */
  mmio_out32(DMAC_CH_CONBLK_ADDR(ch), (unsigned long) &dinfo[ch]);
  mmio_out32(DMAC_CH_CS(ch), mmio_in32(DMAC_CH_CS(ch)) | DMAC_CH_CS_ACTIVE);
  
}
