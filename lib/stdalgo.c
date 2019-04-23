/**
 * @file lib/stdalgo.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


uint64_t gcd(uint64_t m, uint64_t n)
{
  if (m == 0 || n == 0) {
    return 0;
  }
  while (m != n) {
    if (m > n) {
      m = m % n;
			if (m == 0) {
				return n;
			}
    } else {
      n = n % m;
			if (n == 0) {
				return m;
			}
    }
  }
  return m;
}

uint64_t lcm(uint64_t m, uint64_t n)
{
  if (m == 0 || n == 0) {
    return 0;
  }
  return ((m / gcd(m, n)) * n);
}
