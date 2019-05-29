/**
 * @file user/test/test_scan.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


bool test_scan(void)
{
  int a, b;
  unsigned int ua, ub;
  char c, d;
  char str[256];
  float f;
  double lf;
  sscan("123 -4567", "%d%d", &a, &b);
  printf("a = %d, b = %d\n", a, b);
  sscan("1234 567", "%u%u", &ua, &ub);
  printf("ua = %u, ub = %u\n", ua, ub);
  sscan("c d", "%c%c", &c, &d);
  printf("c = '%c' d = '%c'\n", c, d);
  sscan("abc", "%s", str);
  printf("str = %s\n", str);
  sscan("12.34 -56.789", "%f%lf", &f, &lf);
  printf("f = %f lf = %lf\n", f, lf);
  return true;
}
