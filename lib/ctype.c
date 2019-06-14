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


int isalnum(int c)
{
  return (__ismask(c) & (_U | _L | _D)) != 0;
}

int isalpha(int c)
{
  return (__ismask(c) & (_U | _L)) != 0;
}

int iscntrl(int c)
{
  return (__ismask(c) & (_C)) != 0;
}

int isdigit(int c)
{
  return (__ismask(c) & (_D)) != 0;
}

int isgraph(int c)
{
  return (__ismask(c) & (_P | _U | _L | _D)) != 0;
}

int islower(int c)
{
  return (__ismask(c) & (_L)) != 0;
}

int isprint(int c)
{
  return (__ismask(c) & (_P | _U | _L | _D | _SP)) != 0;
}

int ispunct(int c)
{
  return (__ismask(c) & (_P)) != 0;
}

int isspace(int c)
{
  return (__ismask(c) & (_S)) != 0;
}

int isupper(int c)
{
  return (__ismask(c) & (_U)) != 0;
}

int isxdigit(int c)
{
  return (__ismask(c) & (_D | _X)) != 0;
}

int isascii(int c)
{
  return ((unsigned char)(c)) <= 0x7f;
}

int toascii(int c)
{
  return ((unsigned char)(c)) & 0x7f;
}

int tolower(int c)
{
  if (isupper(c)) {
    c -= 'A' - 'a';
  }

  return c;
}

int toupper(int c)
{
  if (islower(c)) {
    c -= 'a' - 'A';
  }

  return c;
}
