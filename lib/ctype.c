/**
 * @file lib/ctype.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

/** Character type. */
int _ctype[] = {
  _C, _C, _C, _C, _C, _C, _C, _C, /* 0-7 */
  _C, _C | _S, _C | _S, _C | _S, _C | _S, _C | _S, _C, _C, /* 8-15 */
  _C, _C, _C, _C, _C, _C, _C, _C, /* 16-23 */
  _C, _C, _C, _C, _C, _C, _C, _C, /* 24-31 */
  _S | _SP, _P, _P, _P, _P, _P, _P, _P, /* 32-39 */
  _P, _P, _P, _P, _P, _P, _P, _P, /* 40-47 */
  _D, _D, _D, _D, _D, _D, _D, _D, /* 48-55 */
  _D, _D, _P, _P, _P, _P, _P, _P, /* 56-63 */
  _P, _U | _X, _U | _X, _U | _X, _U | _X, _U | _X, _U | _X, _U, /* 64-71 */
  _U, _U, _U, _U, _U, _U, _U, _U, /* 72-79 */
  _U, _U, _U, _U, _U, _U, _U, _U, /* 80-87 */
  _U, _U, _U, _P, _P, _P, _P, _P, /* 88-95 */
  _P, _L | _X, _L | _X, _L | _X, _L | _X, _L | _X, _L | _X, _L, /* 96-103 */
  _L, _L, _L, _L, _L, _L, _L, _L, /* 104-111 */
  _L, _L, _L, _L, _L, _L, _L, _L, /* 112-119 */
  _L, _L, _L, _P, _P, _P, _P, _C, /* 120-127 */
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 128-143 */
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 144-159 */
  _S | _SP, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, /* 160-175 */
  _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, /* 176-191 */
  _U, _U, _U, _U, _U, _U, _U, _U, _U, _U, _U, _U, _U, _U, _U, _U, /* 192-207 */
  _U, _U, _U, _U, _U, _U, _U, _P, _U, _U, _U, _U, _U, _U, _U, _L, /* 208-223 */
  _L, _L, _L, _L, _L, _L, _L, _L, _L, _L, _L, _L, _L, _L, _L, _L, /* 224-239 */
  _L, _L, _L, _L, _L, _L, _L, _P, _L, _L, _L, _L, _L, _L, _L, _L
};      /* 240-255 */


/**
 * The isalnum() function checks for an alphanumeric character; it is equivalent to
 * (isalpha(c) || isdigit(c)).
 * @param c is an integer type character.
 * @return Values returned are nonzero if the character @c c falls into the tested class,
 * and a zero value if not.
 */
int isalnum(int c)
{
  return (__ismask(c) & (_U | _L | _D)) != 0;
}

/**
 * The isalpha() function checks  for  an  alphabetic character; in the standard "C" locale,
 * it is equivalent to (isupper(c) || islower(c)).
 * In some locales, there may be additional characters for which the isalpha() function is
 * true--letters which are neither upper case nor lower case.
 * @param c is an integer type character.
 * @return Values returned are nonzero if the character @c c falls into the tested class,
 * and a zero value if not.
 */
int isalpha(int c)
{
  return (__ismask(c) & (_U | _L)) != 0;
}

/**
 * The iscntrl() function checks for a control character.
 * @param c is an integer type character.
 * @return Values returned are nonzero if the character @c c falls into the tested class,
 * and a zero value if not.
 */
int iscntrl(int c)
{
  return (__ismask(c) & (_C)) != 0;
}

/**
 * The isdigit() function checks for a digit (0 through 9).
 * @param c is an integer type character.
 * @return Values returned are nonzero if the character @c c falls into the tested class,
 * and a zero value if not.
 */
int isdigit(int c)
{
  return (__ismask(c) & (_D)) != 0;
}

/**
 * The isgraph() function checks for any printable character except space.
 * @param c is an integer type character.
 * @return Values returned are nonzero if the character @c c falls into the tested class,
 * and a zero value if not.
 */
int isgraph(int c)
{
  return (__ismask(c) & (_P | _U | _L | _D)) != 0;
}

/**
 * The islower() function checks for a lower-case character.
 * @param c is an integer type character.
 * @return Values returned are nonzero if the character @c c falls into the tested class,
 * and a zero value if not.
 */
int islower(int c)
{
  return (__ismask(c) & (_L)) != 0;
}

/**
 * The isprint() function checks for any printable character including space.
 * @param c is an integer type character.
 * @return Values returned are nonzero if the character @c c falls into the tested class,
 * and a zero value if not.
 */
int isprint(int c)
{
  return (__ismask(c) & (_P | _U | _L | _D | _SP)) != 0;
}

/**
 * The ispunct() function checks for any printable character which is not a space
 * or an alphanumeric character.
 * @param c is an integer type character.
 * @return Values returned are nonzero if the character @c c falls into the tested class,
 * and a zero value if not.
 */
int ispunct(int c)
{
  return (__ismask(c) & (_P)) != 0;
}

/**
 * The isspace() function checks for white-space characters.
 * In the "C" and "POSIX" locales, these are: space, formfeed ('\\f'), newline ('\\n'),
 * carriage return ('\\r'), horizontal tab ('\\t'), and vertical tab ('\\v').
 * @param c is an integer type character.
 * @return Values returned are nonzero if the character @c c falls into the tested class,
 * and a zero value if not.
 */
int isspace(int c)
{
  return (__ismask(c) & (_S)) != 0;
}

/**
 * The isupper() function checks for an uppercase letter.
 * @param c is an integer type character.
 * @return Values returned are nonzero if the character @c c falls into the tested class,
 * and a zero value if not.
 */
int isupper(int c)
{
  return (__ismask(c) & (_U)) != 0;
}

/**
 * The isxdigit() function checks for a hexadecimal digits, that is, one of
 * 0 1 2 3 4 5 6 7 8 9 a b c d e f A B C D E F.
 * @param c is an integer type character.
 * @return Values returned are nonzero if the character @c c falls into the tested class,
 * and a zero value if not.
 */
int isxdigit(int c)
{
  return (__ismask(c) & (_D | _X)) != 0;
}

/**
 * The isascii() function checks whether the character @c c is a 7-bit unsigned char value
 * that fits into the ASCII character set.
 * @param c is an integer type character.
 * @return Values returned are nonzero if the character @c c falls into the tested class,
 * and a zero value if not.
 */
int isascii(int c)
{
  return ((unsigned char)(c)) <= 0x7f;
}

/**
 * The toascii() function converts @c c to a 7-bit unsigned char value that fits
 * into the ASCII character set, by clearing the high-order bits.
 * @param c is an integer type character.
 * @return Values returned are nonzero if the character @c c falls into the tested class,
 * and a zero value if not.
 */
int toascii(int c)
{
  return ((unsigned char)(c)) & 0x7f;
}

/**
 * The tolower() function converts the character @c c to lower case, if possible.
 * @param c is an integer type character.
 * @return Value returned is that of the converted character, or @c c if the conversion was not possible.
 */
int tolower(int c)
{
  if (isupper(c)) {
    c -= 'A' - 'a';
  }

  return c;
}

/**
 * The toupper() function converts the character @c c to upper case, if possible.
 * @param c is an integer type character.
 * @return Value returned is that of the converted character, or @c c if the conversion was not possible.
 */
int toupper(int c)
{
  if (islower(c)) {
    c -= 'a' - 'A';
  }

  return c;
}
