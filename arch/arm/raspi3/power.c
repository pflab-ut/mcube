/**
 * @file arch/arm/raspi3/power.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


/* reboot */
void reset()
{
  unsigned int r;
  /* trigger a restart by instructing the GPU to boot from partition 0 */
  r = mmio_in32(POWER_MANAGEMENT_RSTS);
  r &= ~0xfffffaaa;
  /* boot from partition 0 */
  mmio_out32(POWER_MANAGEMENT_RSTS, POWER_MANAGEMENT_WDOG_MAGIC | r);
  mmio_out32(POWER_MANAGEMENT_WDOG, POWER_MANAGEMENT_WDOG_MAGIC | 10);
  mmio_out32(POWER_MANAGEMENT_RSTC,
             POWER_MANAGEMENT_WDOG_MAGIC | POWER_MANAGEMENT_RSTC_FULLRST);
}
