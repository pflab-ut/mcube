/**
 * @file include/mcube/stdalgo.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_STDALGO_H__
#define __MCUBE_MCUBE_STDALGO_H__

#ifndef __ASSEMBLY__

/**
 * @fn unsigned long gcd(unsigned long m, unsigned long n)
 * @brief return Gratest Common Divisor (GCD).
 *
 * @param m Dividend.
 * @param n Divisor.
 * @return GCD.
 */
unsigned long gcd(unsigned long m, unsigned long n);

/**
 * @fn unsigned long lcm(unsigned long m, unsigned long n)
 * @brief return Least Common Multiple (LCM).
 *
 * @param m Multiplicand.
 * @param n Multiplier.
 */
unsigned long lcm(unsigned long m, unsigned long n);

/**
 * @fn unsigned long get_digit(unsigned long num, unsigned long radix)
 * @brief get number of digits.
 *
 * @param num Number.
 * @param radix Radix.
 * @return Number of digits.
 */
unsigned long get_digit(unsigned long num, unsigned long radix);

#endif /* !__ASSEMBLY__ */

#endif  /* __MCUBE_MCUBE_STDALGO_H__ */
