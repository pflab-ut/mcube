/**
 * @file user/test/test_scan.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

static void test_integer(void)
{
  int a, b;
  unsigned int ua, ub;
  char c, d;
  char str[256];
  sscan("123 -4567", "%d%d", &a, &b);
  printk("a = %d, b = %d\n", a, b);
  sscan("1234 567", "%u%u", &ua, &ub);
  printk("ua = %u, ub = %u\n", ua, ub);
  sscan("c d", "%c%c", &c, &d);
  printk("c = '%c' d = '%c'\n", c, d);
  sscan("abc", "%s", str);
  printk("str = %s\n", str);

}

#if defined(ENABLE_FPU)
static void test_float(void)
{
  float f;
  double lf;
  sscan("12.345 -56.789", "%f%lf", &f, &lf);
  printk("f = %f lf = %lf\n", f, lf);
}
#endif /* ENABLE_FPU */


static void test_time(void)
{
  struct tm t;
  int ret;
  ret = sscan("20190530T12:34:56", "%4d%2d%2dT%2d:%2d:%2d",
              &t.tm_year, &t.tm_mon, &t.tm_mday, &t.tm_hour, &t.tm_min, &t.tm_sec);

  printk("year = %d mon = %d mday = %d, hour = %d min = %d sec = %d\n",
         t.tm_year, t.tm_mon, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
  printk("ret = %d\n", ret);
}


bool test_scan(void)
{
  test_integer();
#if defined(ENABLE_FPU)
  test_float();
#endif /* ENABLE_FPU */
  test_time();
  return true;
}
