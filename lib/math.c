/**
 * @file lib/math.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

#if CONFIG_ARCH_SIM || CONFIG_ARCH_ARM


/**
 * The cos() function returns the cosine of @c x, where @c x is given in radians.
 * @param x is a radian.
 * @return On success, these functions return the cosine of @c x.
 * If @c x is a NaN, a NaN is returned.
 * If @c x is positive infinity or negative infinity, a domain error occurs, and a NaN is returned.
 */
double cos(double x)
{
  double t;
  double y;
  int n;
    
  y = 1.0;
  t = 1.0;
  n = 1;
  while (1) {
    /* maclaurin expansion of cos(x) */
    t = - (t * x * x) / ((2 * n) * (2 * n - 1));
#if defined(TERM_NUMBER)
    if (n > TERM_NUMBER) {
      break;
    }
#elif defined(TERM_AMPLITUDE)
    if (FABS(t) <= TERM_AMPLITUDE) {
      break;
    }
#else
#error "Unknown Termination Policy"
#endif /* TERM_NUMBER */
    y += t;
    n++;
  }
  return y;
}


/**
 * The sin() function returns the sine of @c x, where @c x is given in radians.
 * @param x is a radian.
 * @return On success, these functions return the sine of @c x.
 * If @c x is a NaN, a NaN is returned.
 * If @c x is positive infinity or negative infinity, a domain error occurs, and a NaN is returned.
 */
double sin(double x)
{
  double t;
  double y;
  int n;
    
  y = x;
  t = x;
  n = 1;
  while (1) {
    /* maclaurin expansion of sin(x) */
    t = - (t * x * x) / ((2 * n + 1) * (2 * n));
#if defined(TERM_NUMBER)
    if (n > TERM_NUMBER) {
      break;
    }
#elif defined(TERM_AMPLITUDE)
    if (FABS(t) <= TERM_AMPLITUDE) {
      break;
    }
#else
#error "Unknown Termination Policy"
#endif /* TERM_NUMBER */
    y += t;
    n++;
  }
  return y;
}

/**
 * The tan() function returns the tangent of @c x, where @c x is given in radians.
 * @param x is a radian.
 * @return On success, these functions return the tangent of @c x.
 * If @c x is a NaN, a NaN is returned.
 * If @c x is positive infinity or negative infinity, a domain error occurs, and a NaN is returned.
 * If the correct result would overflow, a range error occurs and the tan() function returns @c HUGE_VAL
 * with the mathematically correct sign.
 */
double tan(double x)
{
  int k;
  double t;
  int i;
  double x2;
  
  /* maclaurin expansion of tan(x) */
  k = (int)(x / (PI / 2) + (x >= 0 ? 0.5 : -0.5));
  x = (x - (3217.0 / 2048) * k) + D * k;
  x2 = x * x;
  t = 0;
  /* cannot use termination by amplitude due to divergent sequence */
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


/**
 * The atan() function calculates the principal value of the arc tangent of @c x;
 * that is the value  whose tangent is @c x.
 * @param x is an arc tangent of @c x.
 * @return On success, these functions return the principal value of the arc tangent of @c x
 * in radians; the return value is in the range [-pi/2, pi/2].
 * If @c x is a NaN, a NaN is returned.
 * If @c x is +0 (-0), +0 (-0) is returned.
 * If @c x is positive infinity (negative infinity), +pi/2 (-pi/2) is returned.
 */
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
  while (1) {
    t = (n * n * x * x) / (2 * n + 1 + t);
#if defined(TERM_NUMBER)
    if (n > TERM_NUMBER) {
      break;
    }
#elif defined(TERM_AMPLITUDE)
    if (FABS(t) <= TERM_AMPLITUDE) {
      break;
    }
#else
#error "Unknown Termination Policy"
#endif /* TERM_NUMBER */
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



/* NOTE: work if y is integer */
/* TODO: work if y is not integer */
double pow(double x, double y)
{
  //  double z = __ieee754_pow(x, y);  
#if 1
  double ret = 1.0;
  int i;
  for (i = 0; i < y; i++) {
    ret *= x;
  }
  return ret;
#endif
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

#endif /* CONFIG_ARCH_SIM || CONFIG_ARCH_X86 || CONFIG_ARCH_ARM */

long lpow(long x, long y)
{
  long ret = 1;
  int i;
  for (i = 0; i < y; i++) {
    ret *= x;
  }
  return ret;
}
