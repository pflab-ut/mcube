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

/**
 * @def MIN(x, y)
`* @brief return minimum of @a x and @a y.
 *
 * @param x Value.
 * @param y Value.
 */
#define MIN(x, y) ({                            \
      typeof(x) _m1 = (x);                      \
      typeof(y) _m2 = (y);                      \
      (void) (&_m1 == &_m2);                    \
      _m1 < _m2 ? _m1 : _m2;                    \
    })

/**
 * @def MAX(x, y)
 * @brief return maximum of @a x and @a y.
 *
 * @param x Value.
 * @param y Value.
 */
#define MAX(x, y) ({                            \
      typeof(x) _m1 = (x);                      \
      typeof(y) _m2 = (y);                      \
      (void) (&_m1 == &_m2);                    \
      _m1 > _m2 ? _m1 : _m2;                    \
    })

/**
 * @def SWAP(x, y)
 * @brief return swap @a x and @a y.
 *
 * @param x Value.
 * @param y Value.
 */
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

/**
 * @def ROUND_DOWN(x, n)
 * @brief Round down.
 *
 * @param x Value.
 * @param n Number of digits.
 */
#define ROUND_DOWN(x, n)  (x & ~(typeof(x))(n - 1))

/**
 * @def ROUND_UP(x, n)
 * @brief Round up.
 *
 * @param x Value.
 * @param n Number of digits.
 */
#define ROUND_UP(x, n)    (((x - 1) | (typeof(x))(n - 1)) + 1)

/**
 * @fn long lpow(long x, long y)
 * @brief Power.
 *
 * @param x Value.
 * @param y Power.
 * @return @a y th power of @a x.
 */
long lpow(long x, long y);


/**
 * @def CEIL(size, unit)
 * @brief compute the smallest integral value that is not less than @a size/unit.
 *
 * @param size Size.
 * @param unit Unit.
 */
#define CEIL(size, unit) (((long) (size) * (long) (unit) < 0) \
                          || ((long) (size) % (long) (unit)) == 0 ? \
                          (size) / (unit) : (size) / (unit) + 1)

#if defined(ENABLE_FPU)

/**
 * @def FLOOR(size, unit)
 * @brief return the largest integral value that is not greater than @a size/unit.
 *
 * @param size Size.
 * @param unit Unit.
 */
#define FLOOR(size, unit)  ((size) / (unit) > 0 ? (size) / (unit) :    \
                           (long) (size) / (unit) > (double) (size) / (unit) ?  \
                           (size) / (unit) - 1 : (size) / (unit))


/**
 * @def ABS(x)
 * @brief compute the absolute value of the integer @a x.
 *
 * @param x Value.
 */
#define ABS(x) ((long) (x) > 0 ? (long) (x) : - (long) (x))

/**
 * @def FABS(x)
 * @brief compute the absolute value of the floating-point @a x.
 *
 * @param x Value.
 */
#define FABS(x) ((double) (x) > 0 ? (double) (x) : - (double) (x))


/**
 * @def PI
 * @brief PI.
 */
#define PI 3.1415926535897932384626433832795

/**
 * @def MAX_BERNOULLI_ODD_NUMBER
 * @brief Maximum bernoulli odd number ().
 */
#define MAX_BERNOULLI_ODD_NUMBER 19

/**
 * @def DIFF_RELATED_TO_PI
 * @brief @a 3217/2048 - @a PI/2.
 */
#define DIFF_RELATED_TO_PI 4.4544551033807686783083602485579e-6

/**
 * @def LOG2
 * @brief log(2).
 */
#define LOG2  0.693147180559945309417232121458L

/**
 * @def SQRT2
 * @brief sqrt(2).
 */
#define SQRT2 1.41421356237309504880168872421L

/**
 * @def EPSILON
 * @brief Epsilon.
 */
#define EPSILON 0.001


#if !CONFIG_ARCH_SIM

double copysign(double x, double y);

/**
 * @fn double cos(double x)
 * @brief return the cosine of @a x, where @a x is given in radians.
 *
 * @param x is a radian.
 * @return On success, these functions return the cosine of @a x.
 * If @a x is a NaN, a NaN is returned.
 * If @a x is positive infinity or negative infinity,
 * a domain error occurs, and a NaN is returned.
 */
double cos(double x);

/**
 * @fn double sin(double x)
 * @brief return the sine of @a x, where @a x is given in radians.
 * @param x is a radian.
 * @return On success, these functions return the sine of @a x.
 * If @a x is a NaN, a NaN is returned.
 * If @a x is positive infinity or negative infinity,
 * a domain error occurs, and a NaN is returned.
 */
double sin(double x);

/**
 * @fn double tan(double x)
 * @brief return the tangent of @a x, where @a x is given in radians.
 *
 * @param x is a radian.
 * @return On success, these functions return the tangent of @a x.
 * If @a x is a NaN, a NaN is returned.
 * If @a x is positive infinity or negative infinity,
 * a domain error occurs, and a NaN is returned.
 * If the correct result would overflow, a range error occurs
 * and @a HUGE_VAL with the mathematically correct sign is returned.
 */
double tan(double x);

/**
 * @fn double atan(double x)
 * @brief calculate the principal value of the arc tangent of @a x;
 * that is the value  whose tangent is @a x.
 *
 * @param x is an arc tangent of @a x.
 * @return On success, these functions return the principal value of the arc tangent of @a x
 * in radians; the return value is in the range [-pi/2, pi/2].
 * If @a x is a NaN, a NaN is returned.
 * If @a x is +0 (-0), +0 (-0) is returned.
 * If @a x is positive infinity (negative infinity), +pi/2 (-pi/2) is returned.
 */
double atan(double x);

/**
 * @fn double frexp(double x, int *exp)
 * @brief split @a x into a normalized fraction and an exponent stored in @a exp.
 *
 * @param x Value.
 * @param exp Exponent.
 * @return If the argument x is not zero, the normalized fraction is @a x times
 * a power of two, and its absolute value is always in the range 1/2 (inclusive)
 * to 1 (exclusive), that is, [0.5,1).
 * If x is zero, then the normalized fraction is zero and zero is stored in @a exp.
 * If x is a NaN, a NaN is returned, and the value of @a *exp is unspecified.
 * If x is positive infinity (negative infinity), positive infinity (negative infinity)
 * is returned, and the value of @a *exp is unspecified.
 */
double frexp(double x, int *exp);

/**
 * @fn double log(double x)
 * @brief return the natural logarithm of @a x.
 *
 * @param x Value.
 * @return On success, these functions return the natural logarithm of @a x.
 * If @a x is a NaN, a NaN is returned.
 * If @a x is 1, the result is +0.
 * If @a x is positive infinity, positive infinity is returned.
 * If x is zero, then a pole error occurs, and the functions return -HUGE_VAL,
 * -HUGE_VALF, or -HUGE_VALL, respectively.
 * If @a x is negative (including negative infinity), then a domain error occurs,
 * and a NaN  (not a number) is returned.
 */
double log(double x);

double ipow(double x, int n);

/**
 * @fn double pow(double x, double y) 
 * @brief return the value of @a x raised to the power of @a y.
 *
 * @param x Value.
 * @param y Power.
 * @return On success, these functions return the value of @a x to the power of @a y.
 * If @a x is a finite value less than 0, and @a y is a finite noninteger,
 * a domain error occurs, and a NaN is returned.
 * If the result overflows, a range error occurs, and the functions return HUGE_VAL,
 * HUGE_VALF, or HUGE_VALL, respectively, with the mathematically correct sign.
 * If result underflows, and is not representable, a range error occurs, and 0.0 is returned.
 * Except as specified below, if @a x or @a y is a NaN, the result is a NaN.
 * If @a x is +1, the result is 1.0 (even if @a y is a NaN).
 * If @a y is 0, the result is 1.0 (even if @a x is a NaN).
 * If @a x is +0 (-0), and @a y is an odd integer greater than 0, the result is +0 (-0).
 * If @a x is 0, and @a y greater than 0 and not an odd integer, the result is +0.
 * If @a x is -1, and @a y is positive infinity or negative infinity, the result is 1.0.
 * If the absolute value of @a x is less than 1, and @a y is negative infinity,
 * the result is positive infinity.
 * If the absolute value of @a x is greater than 1, and @a y is negative infinity,
 * the result is +0.
 * If the absolute value of @a x is less than 1, and @a y is positive infinity,
 * the result is +0.
 * If the absolute value of @a x is greater than 1, and @a y is positive infinity,
 * the result is positive infinity.
 * If @a x is negative infinity, and @a y is an odd integer less than 0, the result is -0.
 * If @a x is negative infinity, and @a y less than 0 and not an odd integer,
 * the result is +0.        
 * If @a x is negative infinity, and @a y is an odd integer greater than 0,
 * the result is negative infinity.
 * If @a x is negative infinity, and @a y greater than 0 and not an odd integer,
 * the result is positive infinity.
 * If @a x is positive infinity, and @a y less than 0, the result is +0.
 * If @a x is positive infinity, and @a y greater than 0, the result is positive infinity.
 * If @a x is +0 or -0, and @a y is an odd integer less than 0, a pole error occurs and HUGE_VAL,   
 * HUGE_VALF, or HUGE_VALL, is returned, with the same sign as @a x.
 * If @a x is +0 or -0, and @a y is less than 0 and not an odd integer,
 * a pole error occurs and +HUGE_VAL, +HUGE_VALF, or +HUGE_VALL, is returned.
 */
double pow(double x, double y);

/**
 * @fn double sqrt(double x)
 * @brief return the nonnegative square root of @a x.
 *
 * @param x Value.
 * @return  On success, these functions return the square root of @a x.
 * If @a x is a NaN, a NaN is returned.
 * If @a x is +0 (-0), +0 (-0) is returned.
 * If @a x is positive infinity, positive infinity is returned.
 * If @a x is less than -0, a domain error occurs, and a NaN is returned.
 */
double sqrt(double x);

/**
 * @fn double cbrt(double x)
 * @brief return the (real) cube root of @a x.
 * This function cannot fail; every representable real value has
 * a representable real cube root.
 *
 * @param x Value.
 * @return Cube root of @a x.
 * If @a x is +0, -0, positive infinity, negative infinity, or NaN, @a x is returned.
 */
double cbrt(double x);

/**
 * @fn double remainder(double x, double y)
 * @brief compute the remainder of dividing @a x by @a y. 
 * The return value is @a x-n*y, where @a n is the value @a x/y,
 * rounded to the nearest integer.
 * If the absolute value of @a x-n*y is 0.5, @a n is chosen to be even.
 *
 * @param x Divisor.
 * @param y Dividend.
 * @return On success, return the floating-point remainder, @a x-n*y.
 * If the return value is 0, it has the sign of @a x.
 * If @a x or @a y is a NaN, a NaN is returned.
 * If @a x is an infinity, and @a y is not a NaN, a domain error occurs,
 * and a NaN is returned.
 * If @a y is zero, and @a x is not a NaN, a domain error occurs, and a NaN is returned.
 */
double remainder(double x, double y);

/**
 * @fn double strtod(const char *nptr, char **endptr)
 * @brief convert the initial portion of the string
 * pointed to by @a nptr to double, float, and long double representation, respectively.
 *
 * @param nptr String pointer.
 * @param endptr Pointer to save unconverted string.
 * @return Converted value, if any.
 * If endptr is not NULL, a pointer to the character after the last character
 * used in the conversion is stored in the location referenced by @a endptr.
 * If no conversion is performed, zero is returned and (unless @a endptr is null)
 * the value of @a nptr is stored in the location referenced by @a endptr.
 * If the correct value would cause overflow, plus or minus HUGE_VAL (HUGE_VALF, HUGE_VALL) is
 * returned (according to the sign of the value), and ERANGE is stored in errno.
 * If the correct value would cause underflow, zero is returned and ERANGE is stored in errno.
 */
double strtod(const char *nptr, char **endptr);

/**
 * @fn int isnan(double x)
 * @brief Is @x NaN?
 *
 * @param x Value.
 * @return True if @x is NaN.
 */
int isnan(double x);

/**
 * @fn int isinf(double x)
 * @brief Is @x Inf?
 *
 * @param x Value.
 * @return True if @x is Inf.
 */
int isinf(double x);


#endif /* !CONFIG_ARCH_SIM */

/**
 * @union ieee754
 * @brief IEEE754.
 */
union ieee754 {
  /**
   * Double.
   */
  double d;

  /**
   * Array.
   */
  unsigned char s[8];

  /**
   * Unsigned long.
   */
  uint64_t ul;
};

/**
 * @def EXPONENT_MASK
 * @brief Exponent mask.
 */
#define EXPONENT_MASK 0x3ff0000000000000UL

/**
 * @def FRACTION_MASK
 * @brief Fraction mask.
 */
#define FRACTION_MASK 0x000fffffffffffffUL


#else

/**
 * @def FLOOR(size, unit)
 * @brief return the largest integral value that is not greater than @a size/unit.
 *
 * @param size Size.
 * @param unit Unit.
 */
#define FLOOR(size, unit) ((size) / (unit))


#endif /* ENABLE_FPU */


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_MATH_H__ */
