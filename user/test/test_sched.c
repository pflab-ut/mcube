/**
 * @file user/test/test_sched.c
 *
 * @author Hiroyuki Chishiro
 */
/*
 * Copyright (C) 2010-2011 Ahmed S. Darwish <darwish.07@gmail.com>
 */

#include <mcube/mcube.h>
#if CONFIG_ARCH_X86


__noreturn void loop_print(char ch, int color)
{
  while (true) {
    putchar_colored(ch, color);
    for (int i = 0; i < 0xffff; i++) {
      cpu_pause();
    }
  }
}

__noreturn static void test0(void) { loop_print('A', VGA_LIGHT_BLUE); }
__noreturn static void test1(void) { loop_print('B', VGA_LIGHT_BLUE); }
__noreturn static void test2(void) { loop_print('C', VGA_LIGHT_BLUE); }
__noreturn static void test3(void) { loop_print('D', VGA_LIGHT_CYAN); }
__noreturn static void test4(void) { loop_print('E', VGA_LIGHT_CYAN); }
__noreturn static void test5(void) { loop_print('F', VGA_LIGHT_CYAN); }

bool test_sched(void)
{
  for (int i = 0; i < 20; i++) {
    kthread_create(test0);
    kthread_create(test1);
    kthread_create(test2);
    kthread_create(test3);
    kthread_create(test4);
    kthread_create(test5);
  }
  return true;
}

#endif /* CONFIG_ARCH_X86 */

