/**
 * @file user/test/test_kthread.c
 *
 * @author Hiroyuki Chishiro
 */
/*
 * Copyright (C) 2010 Ahmed S. Darwish <darwish.07@gmail.com>
 */

#include <mcube/mcube.h>
#if CONFIG_ARCH_X86


/* testcases */

static void __noreturn test0(void)
{
  loop_print('G', VGA_LIGHT_GREEN);
}
static void __noreturn test1(void)
{
  loop_print('H', VGA_LIGHT_GREEN);
}
static void __noreturn test2(void)
{
  loop_print('I', VGA_LIGHT_GREEN);
}
static void __noreturn test3(void)
{
  loop_print('J', VGA_LIGHT_MAGNETA);
}
static void __noreturn test4(void)
{
  loop_print('K', VGA_LIGHT_MAGNETA);
}
static void __noreturn test5(void)
{
  loop_print('L', VGA_LIGHT_MAGNETA);
}

/*
 * This code runs on each secondary core after finishing its
 * own internal initialization.
 */
static void run_secondary_core_testcases(void)
{
  for (int i = 0; i < 20; i++) {
    kthread_create(test0);
    kthread_create(test1);
    kthread_create(test2);
    kthread_create(test3);
    kthread_create(test4);
    kthread_create(test5);
  }

  //  test_ext2();
}

bool test_kthread(void)
{
  run_secondary_core_testcases();
  return true;
}


#endif /* CONFIG_ARCH_X86 */

