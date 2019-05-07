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


int user_arch_main(__unused int argc, __unused char *argv[])
{
  return 0;
}
