/**
 * @file user/sim/user_sim.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


int ap_main(void)
{
  unsigned long cpu = get_cpu_id();
  putchar(cpu + '0');
  print("ap_main()\n");
  print("get_cpu_id() = %lu\n", get_cpu_id());
  //  print("ap_main()\n");
  //  cpu_ids[cpu] = cpu;
  return 0;
}

int user_arch_main(int argc, char *argv[])
{
  long l = 1234;
  //  printk("user_arch_main()\n");
  printk("%%\n");
  printk("%c%%\n", 'a');
  //  printk("%% %% %% \n\n");
  //  printf("%% %% %% \n\n");
  // fprintf(stdout, "%% %% %% \n\n");
#if 1
  printk("%d\n", l);
  printk("%09d\n", l);
  printk("0x%lx\n", l);
  printk("0x%016lx\n", l);
#endif
  return 0;
}
