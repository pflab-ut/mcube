/**
 * @file include/mcube/math.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_MATH_H__
#define __MCUBE_MCUBE_MATH_H__

#ifndef __ASSEMBLY__

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

long lpow(long x, long y);


#if defined(ENABLE_FPU)

/**
 * The CEIL() macro computes the smallest integral value that is not less than @c size/unit.
 * @return Ceiling of @c size/unit.
 */
#define CEIL(size, unit) (((int64_t) (size) * (int64_t) (unit) < 0) \
                          || ((int64_t) (size) % (int64_t) (unit)) == 0 ? \
                          (size) / (unit) : (size) / (unit) + 1)
/**
 * The FLOOR() macro returns the largest integral value that is not greater than @c size/unit.
 * @return Floor of @c size/unit.
 */
#define FLOOR(size, unit)  ((size) / (unit) > 0 ? (size) / (unit) :    \
                           (int64_t) (size) / (unit) > (double) (size) / (unit) ?  \
                           (size) / (unit) - 1 : (size) / (unit))


/**
 * The ABS() macro computes the absolute value of the integer @c x.
 * @return Absolute value of @c x.
 */
#define ABS(x) ((int64_t) (x) > 0 ? (int64_t) (x) : - (int64_t) (x))
/**
 * The FABS() macro computes the absolute value of the floating-point @c x.
 * @return Absolute value of @c x.
 */
#define FABS(x) ((double) (x) > 0 ? (double) (x) : - (double) (x))


#define PI 3.1415926535897932384626433832795

/* termination by aplitude */
#define TERM_AMPLITUDE 0.000000001

/* termination by # of terms */
#define MAX_BERNOULLI_ODD_NUMBER 19
#define TERM_NUMBER 24

#define D 4.4544551033807686783083602485579e-6

#if CONFIG_ARCH_SIM
#include <math.h>
#else
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

#endif /* CONFIG_ARCH_SIM */

#else
/**
 * The CEIL() macro computes the smallest integral value that is not less than @c size/unit.
 * @return Ceiling of @c size/unit.
 */
#define CEIL(size, unit) (((int32_t) (size) * (int32_t) (unit) < 0) \
                          || ((int32_t) (size) % (int32_t) (unit)) == 0 ? \
                          (size) / (unit) : (size) / (unit) + 1)
/**
 * The FLOOR() macro returns the largest integral value that is not greater than @c size/unit.
 * @return Floor of @c size/unit.
 */
#define FLOOR(size, unit)  ((size) / (unit))

#endif /* ENABLE_FPU */


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_MATH_H__ */
