/**
 * @file user/user.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

int user_arch_main(void);

#if !CONFIG_ARCH_X86


int ap_main(void)
{
  unsigned long cpu = get_cpu_id();
  putchar(cpu + '0');
  printk("get_cpu_id() = %lu\n", get_cpu_id());
  //  printk("ap_main()\n");
  //  cpu_ids[cpu] = cpu;
  return 0;
}
#endif /* !CONFIG_ARCH_X86 */


int user_main(void)
{
#if CONFIG_ARCH_ARM || CONFIG_ARCH_AXIS
  user_arch_main();
#endif /* CONFIG_ARCH_AIM || CONFIG_ARCH_AXIS */
  return 0;
}
