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

__noreturn void loop_print(char ch, __unused int color)
{
  printk("loop_print()\n");

  while (true) {
#if 1
    putchar(ch);
#else
    putchar_colored(ch, color);
#endif

    for (int i = 0; i < 0xffff; i++) {
      pause();
    }
  }
}

__noreturn static void test0(void *arg)
{
  printk("test0(): &id = 0x%lx id = %d\n", (int *) arg, *(int *) arg);
  loop_print('A', VGA_LIGHT_BLUE);
}

__noreturn static void test1(void *arg)
{
  printk("test0(): &id = 0x%lx id = %d\n", (int *) arg, *(int *) arg);
  loop_print('B', VGA_LIGHT_BLUE);
}

__noreturn static void test2(void *arg)
{
  printk("test0(): &id = 0x%lx id = %d\n", (int *) arg, *(int *) arg);
  loop_print('C', VGA_LIGHT_BLUE);
}

__noreturn static void test3(void *arg)
{
  printk("test0(): &id = 0x%lx id = %d\n", (int *) arg, *(int *) arg);
  loop_print('D', VGA_LIGHT_CYAN);
}

__noreturn static void test4(void *arg)
{
  printk("test0(): &id = 0x%lx id = %d\n", (int *) arg, *(int *) arg);
  loop_print('E', VGA_LIGHT_CYAN);
}

__noreturn static void test5(void *arg)
{
  printk("test0(): &id = 0x%lx id = %d\n", (int *) arg, *(int *) arg);
  loop_print('F', VGA_LIGHT_CYAN);
}

static int id[] = {1, 2, 3, 4, 5, 6};

bool test_sched(void)
{
  printk("test_sched()\n");

  kthread_create(test0, &id[0], NULL);
  kthread_create(test1, &id[1], NULL);
  kthread_create(test2, &id[2], NULL);
  kthread_create(test3, &id[3], NULL);
  kthread_create(test4, &id[4], NULL);
  kthread_create(test5, &id[5], NULL);

  return true;
}

#endif /* CONFIG_ARCH_X86 */

