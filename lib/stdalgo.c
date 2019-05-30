/**
 * @file lib/stdalgo.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


unsigned long gcd(unsigned long m, unsigned long n)
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

unsigned long lcm(unsigned long m, unsigned long n)
{
  if (m == 0 || n == 0) {
    return 0;
  }

  return ((m / gcd(m, n)) * n);
}

unsigned long get_digit(unsigned long num, unsigned long radix)
{
  unsigned long digit = 0;

  while (num != 0) {
    num /= radix;
    digit++;
  }

  return digit;
}
