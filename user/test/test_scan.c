/**
 * @file user/test/test_scan.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

static bool test_time(void)
{
  struct tm t;
  int ret;
  ret = sscan("20190530T12:34:56", "%4d%2d%2dT%2d:%2d:%2d",
              &t.tm_year, &t.tm_mon, &t.tm_mday, &t.tm_hour, &t.tm_min, &t.tm_sec);

  printf("year = %d mon = %d mday = %d, hour = %d min = %d sec = %d\n",
         t.tm_year, t.tm_mon, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
  printf("ret = %d\n", ret);
  return true;
}


bool test_scan(void)
{
  int a, b;
  unsigned int ua, ub;
  char c, d;
  char str[256];
  sscan("123 -4567", "%d%d", &a, &b);
  printf("a = %d, b = %d\n", a, b);
  sscan("1234 567", "%u%u", &ua, &ub);
  printf("ua = %u, ub = %u\n", ua, ub);
  sscan("c d", "%c%c", &c, &d);
  printf("c = '%c' d = '%c'\n", c, d);
  sscan("abc", "%s", str);
  printf("str = %s\n", str);

  test_time();
#if defined(ENABLE_FPU)
  float f;
  double lf;
  sscan("12.34 -56.789", "%f%lf", &f, &lf);
  printf("f = %f lf = %lf\n", f, lf);
#endif /* ENABLE_FPU */
  return true;
}
