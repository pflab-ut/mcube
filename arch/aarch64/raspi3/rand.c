/**
 * @file arch/aarch64/raspi3/rand.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


void init_rand_raspi3(void)
{
  /* initialize status */
  mmio_out32(RANDOM_NUMBER_GEN_STATUS, RANDOM_NUMBER_GEN_STATUS_INIT);
  /* mask interrupt */
  mmio_out32(RANDOM_NUMBER_GEN_INT_MASK,
             mmio_in32(RANDOM_NUMBER_GEN_INT_MASK)
             | RANDOM_NUMBER_GEN_INT_MASK_SET);
  /* enable random number generator */
  mmio_out32(RANDOM_NUMBER_GEN_CTRL,
             mmio_in32(RANDOM_NUMBER_GEN_CTRL)
             | RANDOM_NUMBER_GEN_CTRL_ENABLE);

  while (!(mmio_in32(RANDOM_NUMBER_GEN_STATUS) >> 24)) {
    nop();
  }
}


unsigned int rand_raspi3(unsigned int min, unsigned int max)
{
  return mmio_in32(RANDOM_NUMBER_GEN_DATA) % (max - min) + min;
}
