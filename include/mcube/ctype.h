/**
 * @file include/mcube/ctype.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_CTYPE_H__
#define __MCUBE_MCUBE_CTYPE_H__

#ifndef __ASSEMBLY__

/**
 * @def _U
 * @brief Upper.
 */
#define _U (0x1 << 0)

/**
 * @def _L
 * @brief Lower.
 */
#define _L (0x1 << 1)

/**
 * @def _D
 * @brief Digit.
 */
#define _D (0x1 << 2)

/**
 * @def _C
 * @brief Cntrl.
 */
#define _C (0x1 << 3)

/**
 * @def _P
 * @brief Punct.
 */
#define _P (0x1 << 4)

/**
 * @def _S
 * @brief White space (space/lf/tab).
 */
#define _S (0x1 << 5)

/**
 * @def _X
 * @brief Hex digit.
 */
#define _X (0x1 << 6)

/**
 * @def _SP
 * @brief Hard space (0x20)
 */
#define _SP (0x1 << 7)

/**
 * Character type.
 */
extern int _ctype[];

/**
 * @def __ismask(x)
 * @brief Is @a x masked?
 */
#define __ismask(x) (_ctype[(x)])

/**
 * @fn int isalnum(int c)
 * @brief checks for an alphanumeric character.
 * It is equivalent to (isalpha(c) || isdigit(c)).
 *
 * @param c Integer type character.
 * @return Nonzero if true, and zero if false.
 */
int isalnum(int c);

/**
 * @fn int isalpha(int c)
 * @brief The isalpha() function checks for an alphabetic character.
 * in the standard "C" locale,
 * it is equivalent to (isupper(c) || islower(c)).
 * In some locales, there may be additional characters for which the isalpha() function is
 * true--letters which are neither upper case nor lower case.
 *
 * @param c Integer type character.
 * @return Nonzero if true, and zero if false.
 */
int isalpha(int c);

/**
 * @fn int iscntrl(int c)
 * @brief check for a control character.
 *
 * @param c Integer type character.
 * @return Nonzero if true, and zero if false.
 */
int iscntrl(int c);

/**
 * @fn int isdigit(int c)
 * @brief check for a digit (0 through 9).
 *
 * @param c Integer type character.
 * @return Nonzero if true, and zero if false.
 */
int isdigit(int c);

/**
 * @fn int isgraph(int c)
 * @brief check for any printable character except space.
 *
 * @param c is an integer type character.
 * @return Nonzero if true, and zero if false.
 */
int isgraph(int c);

/**
 * @fn int islower(int c)
 * @brief check for a lower-case character.
 *
 * @param c Integer type character.
 * @return Nonzero if true, and zero if false.
 */
int islower(int c);

/**
 * @fn int isprint(int c)
 * @brief check for any printable character including space.
 *
 * @param c Integer type character.
 * @return Nonzero if true, and zero if false.
 */
int isprint(int c);

/**
 * @fn int ispunct(int c)
 * @brief check for any printable character which is not a space or an alphanumeric character.
 * @param c Integer type character.
 * @return Nonzero if true, and zero if false.
 */
int ispunct(int c);

/**
 * @fn int isspace(int c)
 * @brief check for white-space characters.
 * In the "C" and "POSIX" locales, these are: space, formfeed ('\\f'), newline ('\\n'),
 * carriage return ('\\r'), horizontal tab ('\\t'), and vertical tab ('\\v').
 *
 * @param c Integer type character.
 * @return Nonzero if true, and zero if false.
 */
int isspace(int c);

/**
 * @fn int isupper(int c)
 * @brief check for an uppercase letter.
 *
 * @param c is an integer type character.
 * @return Nonzero if true, and zero if false.
 */
int isupper(int c);

/**
 * @fn int isxdigit(int c)
 * @brief check for a hexadecimal digits, that is, one of
 *        0 1 2 3 4 5 6 7 8 9 a b c d e f A B C D E F.
 *
 * @param c Integer type character.
 * @return Nonzero if true, and zero if false.
 */
int isxdigit(int c);

/**
 * @fn int isascii(int c)
 * @brief check whether the character @c c is a 7-bit unsigned char value
 *        that fits into the ASCII character set.
 *
 * @param c Integer type character.
 * @return Nonzero if true, and zero if false.
 */
int isascii(int c);

/**
 * @fn int toascii(int c)
 * @brief convert @a c to a 7-bit unsigned char value that fits
 * into the ASCII character set, by clearing the high-order bits.
 *
 * @param c is an integer type character.
 * @return Nonzero if true, and zero if false.
 */
int toascii(int c);

/**
 * @fn int tolower(int c)
 * @brief convert the character @a c to lower case, if possible.
 *
 * @param c is an integer type character.
 * @return Nonzero if true, and zero if false.
 */
int tolower(int c);

/**
 * @fn int toupper(int c)
 * @brief convert the character @a c to upper case, if possible.
 * @param c is an integer type character.
 * @return Nonzero if true, and zero if false.
 */
int toupper(int c);

#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_MCUBE_CTYPE_H__ */
