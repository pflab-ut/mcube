/**
 * @file user/user.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


int user_arch_main(void);
int user_test_main(void);


int user_main(void)
{
  //  user_arch_main();
  user_test_main();
  return 0;
}
