/**
 * @file user/user.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


int user_main(int argc, char *argv[])
{
  printk("user_main()\n");
  user_arch_main(argc, argv);
  //  user_test_main(argc, argv);
  return 0;
}
