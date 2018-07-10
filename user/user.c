/**
 * @file user/user.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

extern int user_arch_main(void);

int ap_main(void)
{
  printk("ap_main()\n");
  for (;;)
    ;
  return 0;
}


int user_main(void)
{
#if CONFIG_ARCH_ARM || CONFIG_ARCH_AXIS
  user_arch_main();
#endif /* CONFIG_ARCH_AIM || CONFIG_ARCH_AXIS */
  return 0;
}
