/**
 * @file include/mcube/math.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_MATH_H__
#define __MCUBE_MCUBE_MATH_H__

#ifndef __ASSEMBLY__


/*
 * Semi type-safe min and max using GNU extensions
 * The type-checking trick is taken from Linux-2.6.
 */
#define MIN(x, y) ({                            \
      typeof(x) _m1 = (x);                      \
      typeof(y) _m2 = (y);                      \
      (void) (&_m1 == &_m2);                    \
      _m1 < _m2 ? _m1 : _m2;                    \
    })
#define MAX(x, y) ({                            \
      typeof(x) _m1 = (x);                      \
      typeof(y) _m2 = (y);                      \
      (void) (&_m1 == &_m2);                    \
      _m1 > _m2 ? _m1 : _m2;                    \
    })
#define SWAP(x, y) ({                           \
      typeof(x) _m1 = (x);                      \
      typeof(y) _m2 = (y);                      \
      typeof(x) _m3;                            \
      (void) (&_m1 == &_m2);                    \
      _m3 = (x);                                \
      (x) = (y);                                \
      (y) = _m3;                                \
    })


/*
 * In a binary system, a value 'x' is said to be n-byte
 * aligned when 'n' is a power of the radix 2, and x is
 * a multiple of 'n' bytes.
 *
 * A n-byte-aligned value has at least a log2n number of
 * least-significant zeroes.
 *
 * Return given x value 'n'-aligned.
 *
 * Using two's complement, rounding = (x & (typeof(x))-n)
 */
#define ROUND_DOWN(x, n)  (x & ~(typeof(x))(n - 1))
#define ROUND_UP(x, n)    (((x - 1) | (typeof(x))(n - 1)) + 1)


long lpow(long x, long y);


/**
 * The CEIL() macro computes the smallest integral value that is not less than @c size/unit.
 * @return Ceiling of @c size/unit.
 */
#define CEIL(size, unit) (((long) (size) * (long) (unit) < 0) \
                          || ((long) (size) % (long) (unit)) == 0 ? \
                          (size) / (unit) : (size) / (unit) + 1)

#if defined(ENABLE_FPU)

/**
 * The FLOOR() macro returns the largest integral value that is not greater than @c size/unit.
 * @return Floor of @c size/unit.
 */
#define FLOOR(size, unit)  ((size) / (unit) > 0 ? (size) / (unit) :    \
                           (long) (size) / (unit) > (double) (size) / (unit) ?  \
                           (size) / (unit) - 1 : (size) / (unit))


/**
 * The ABS() macro computes the absolute value of the integer @c x.
 * @return Absolute value of @c x.
 */
#define ABS(x) ((long) (x) > 0 ? (long) (x) : - (long) (x))
/**
 * The FABS() macro computes the absolute value of the floating-point @c x.
 * @return Absolute value of @c x.
 */
#define FABS(x) ((double) (x) > 0 ? (double) (x) : - (double) (x))


#define PI 3.1415926535897932384626433832795
#define MAX_BERNOULLI_ODD_NUMBER 19
#define D 4.4544551033807686783083602485579e-6
#define EPSILON 0.001


#if !CONFIG_ARCH_SIM

double __ieee754_sqrt(double x);
double copysign(double x, double y);
double pow(double x, double y);

double cos(double x);
double sin(double x);
double tan(double x);
double atan(double x);

double sqrt(double x);
double cbrt(double x);
double remainder(double x, double y);
double strtod(const char *nptr, char **endptr);

int isnan(double x);
int isinf(double x);


#endif /* !CONFIG_ARCH_SIM */

union ieee754 {
  double d;
  unsigned char s[8];
  uint64_t ul;
};

#define EXPONENT_MASK 0x3ff0000000000000ul
#define FRACTION_MASK 0x000ffffffffffffful



#else

/**
 * The FLOOR() macro returns the largest integral value that is not greater than @c size/unit.
 * @return Floor of @c size/unit.
 */
#define FLOOR(size, unit) ((size) / (unit))


#endif /* ENABLE_FPU */


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_MATH_H__ */
