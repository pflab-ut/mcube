/**
 * @file lib/math.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

long lpow(long x, long y)
{
  long ret = 1;
  int i;

  for (i = 0; i < y; i++) {
    ret *= x;
  }

  return ret;
}

#if !CONFIG_ARCH_SIM

double cos(double x)
{
  double t;
  double y;
  int n;

  y = 1.0;
  t = 1.0;
  n = 1;

  while (true) {
    /* maclaurin expansion of cos(x) */
    t = - (t * x * x) / ((2 * n) * (2 * n - 1));

    if (FABS(t) <= EPSILON) {
      break;
    }

    y += t;
    n++;
  }

  return y;
}


double sin(double x)
{
  double t;
  double y;
  int n;

  y = x;
  t = x;
  n = 1;

  while (true) {
    /* maclaurin expansion of sin(x) */
    t = - (t * x * x) / ((2 * n + 1) * (2 * n));

    if (FABS(t) <= EPSILON) {
      break;
    }

    y += t;
    n++;
  }

  return y;
}

double tan(double x)
{
  int k;
  double t;
  int i;
  double x2;

  /* maclaurin expansion of tan(x) */
  k = (int)(x / (PI / 2) + (x >= 0 ? 0.5 : -0.5));
  x = (x - (3217.0 / 2048) * k) + DIFF_RELATED_TO_PI * k;
  x2 = x * x;
  t = 0;

  for (i = MAX_BERNOULLI_ODD_NUMBER; i >= 3; i -= 2) {
    t = x2 / (i - t);
  }

  t = x / (1 - t);

  if ((k % 2) == 0) {
    return t;
  }

  if (t != 0) {
    return -1 / t;
  }

  /* overflow */
  return HUGE_VAL;
}


double atan(double x)
{
  int n, sign;
  double t;

  if (x > 1) {
    sign = 1;
    x = 1 / x;
  } else if (x < -1) {
    sign = -1;
    x = 1 / x;
  } else {
    sign = 0;
  }

  t = 0;
  n = 0;

  while (true) {
    t = (n * n * x * x) / (2 * n + 1 + t);

    if (FABS(t) <= EPSILON) {
      break;
    }

    n++;
  }

  if (sign > 0) {
    return PI / 2 - x / (1 + t);
  }

  if (sign < 0) {
    return -PI / 2 - x / (1 + t);
  }

  return x / (1 + t);
}




double remainder(double x, double y)
{
  long n = x / y;
  return x - n * y;
}


double copysign(double x, double y)
{
  if (y > 0.0) {
    return FABS(x);
  } else {
    return -FABS(x);
  }
}


double frexp(double x, int *exp)
{
  union {
    double d;
    uint64_t i;
  } y = { x };
  int ee = y.i >> 52 & 0x7ff;

  if (!ee) {
    if (x) {
      x = frexp(x * 0x1p64, e);
      *e -= 64;
    } else {
      *e = 0;
    }

    return x;
  } else if (ee == 0x7ff) {
    return x;
  }

  *e = ee - 0x3fe;
  y.i &= 0x800fffffffffffffull;
  y.i |= 0x3fe0000000000000ull;
  return y.d;
}


double log(double x)
{
  int i, k;
  long double x2, s, last;

  if (x <= 0) {
    fprintf(stderr, "llog(x): x <= 0\n");
    return 0;
  }

  frexp(x / SQRT2, &k);  /* 1 / 2 <= (x / SQRT2) / pow(2, k) < 1 */
  x /= ldexp(1, k);      /* x = x / pow(2, k) */
  x = (x - 1) / (x + 1);
  x2 = x * x;
  i = 1;
  s = x;

  do {
    x *= x2;
    i += 2;
    last = s;
    s += x / i;
  } while (last != s);

  return LOG2 * k + 2 * s;
}


double ipow(double x, int n)
{
  int abs_n;
  double r;

  abs_n = abs(n);
  r = 1;

  while (abs_n != 0) {
    if (abs_n & 1) {
      r *= x;
    }

    x *= x;
    abs_n >>= 1;
  }

  if (n >= 0) {
    return r;
  }

  return 1 / r;
}


double pow(double x, double y)
{
  if (y <= INT_MAX && y >= -INT_MAX && y == (int) y) {
    return ipow(x, y);
  }

  if (x > 0) {
    return exp(y * log(x));
  }

  if (x != 0 || y <= 0) {
    printk("power: domain error\n");
  }

  return 0;
}


double sqrt(double x)
{
  double s, last;

  if (x > 0.0) {
    if (x > 1.0) {
      s = x;
    } else {
      s = 1.0;
    }

    do {
      last = s;
      s = (x / s + s) / 2.0;
    } while (s < last);

    return last;
  }

  if (x != 0.0) {
    print("ksqrt: domain error\n");
  }

  return 0;
}

double cbrt(double x)
{
  double s, prev;
  int positive;

  if (x == 0.0) {
    return 0.0;
  }

  if (x > 0.0) {
    positive = 1;
  } else {
    positive = 0;
    x = -x;
  }

  if (x > 1.0) {
    s = x;
  } else {
    s = 1.0;
  }

  do {
    prev = s;
    s = (x / (s * s) + 2.0 * s) / 3.0;
  } while (s < prev);

  if (positive) {
    return prev;
  } else {
    return -prev;
  }
}


int isinf(double x)
{
  union ieee754 d2ul;
  d2ul.d = x;

  if ((d2ul.ul & EXPONENT_MASK) == EXPONENT_MASK
      && (d2ul.ul & FRACTION_MASK) == 0) {
    return true;
  }

  return false;
}

int isnan(double x)
{
  union ieee754 d2ul;
  d2ul.d = x;

  if ((d2ul.ul & EXPONENT_MASK) == EXPONENT_MASK
      && (d2ul.ul & FRACTION_MASK) != 0) {
    return true;
  }

  return false;
}


#endif /* !CONFIG_ARCH_SIM */

