/**
 * @file arch/x86/strfmt.c
 *
 * @author Hiroyuki Chishiro
 */
/*_
 * Copyright (c) 2018 Hirochika Asai <asai@jar.jp>
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <mcube/mcube.h>

#define STRFMT_MOD_NONE         0
#define STRFMT_MOD_LONG         1
#define STRFMT_MOD_LONGLONG     2

/*
 * Formatting information
 */
struct strfmt_format {
  /* Leading suffix */
  int zero;
  /* Minimum length */
  int pad;
  /* Precision */
  int prec;
  /* Modifier */
  int mod;
  /* Specifier */
  int spec;
};

/* Prototype declarations */
static off_t output_percent(char *, size_t);
static off_t output_char(char *, size_t, va_list);
static off_t output_decimal(char *, size_t, struct strfmt_format *, va_list);
static off_t output_hexdecimal(char *, size_t, struct strfmt_format *, va_list, int);
static off_t output_pointer(char *, size_t, struct strfmt_format *, va_list);
static off_t output_string(char *, size_t, va_list);

/*
 * Parse the format chunk
 */
static int parse_format(const char **formatp, struct strfmt_format *strfmt)
{
  const char *format;
  /* Leading suffix */
  int zero;
  /* Minimum length */
  int pad;
  /* Precision */
  int prec;
  /* Modifier */
  int mod;
  /* Specifier */
  int spec;

  /* Reset */
  zero = 0;
  pad = 0;
  prec = 0;
  mod = 0;
  spec = 0;

  /* Take the format string */
  format = *formatp;

  /* Check and skip % */
  if (*format != '%') {
    return -1;
  }
  format++;

  /* Padding with zero? */
  if (*format == '0') {
    zero = 1;
    format++;
  }

  /* Padding length */
  if (*format >= '1' && *format <= '9') {
    pad += *format - '0';
    format++;
    while (*format >= '0' && *format <= '9') {
      pad *= 10;
      pad += *format - '0';
      format++;
    }
  }

  /* Precision */
  if (*format == '.') {
    format++;
    while ( *format >= '0' && *format <= '9' ) {
      prec *= 10;
      prec += *format - '0';
      format++;
    }
  }

  /* Modifier */
  if (*format == 'l') {
    format++;
    if (*format == 'l') {
      mod = STRFMT_MOD_LONGLONG;
      format++;
    } else {
      mod = STRFMT_MOD_LONG;
    }
  }

  /* Conversion */
  if (*format != '\0') {
    spec = *format;
    format++;
  } else {
    spec = 0;
  }

  /* Write back the current pointer of the string */
  *formatp = format;

  /* Set the parsed result */
  strfmt->zero = zero;
  strfmt->pad = pad;
  strfmt->prec = prec;
  strfmt->mod = mod;
  strfmt->spec = spec;

  return 0;
}

/*
 * Output
 */
static off_t output(char *str, size_t size, struct strfmt_format *strfmt, va_list ap)
{
  off_t ret;

  ret = 0;
  switch (strfmt->spec) {
  case '%':
    /* % */
    ret += output_percent(str, size);
    break;
  case 's':
    /* String */
    ret += output_string(str, size, ap);
    break;
  case 'c':
    /* Character */
    ret += output_char(str, size, ap);
    break;
  case 'd':
    /* Decimal */
    ret += output_decimal(str, size, strfmt, ap);
    break;
  case 'x':
    /* Hexdecimal (lower case) */
    ret += output_hexdecimal(str, size, strfmt, ap, 0);
    break;
  case 'X':
    /* Hexdecimal (upper case) */
    ret += output_hexdecimal(str, size, strfmt, ap, 1);
    break;
  case 'p':
    /* Pointer */
    ret += output_pointer(str, size, strfmt, ap);
    break;
  default:
    /* To implement */
    ;
  }

  return ret;
}
static off_t output_percent(char *str, size_t size)
{
  if (str != NULL && size > 1) {
    str[0] = '%';
  }
  return 1;
}

static off_t output_char(char *str, size_t size, va_list ap)
{
  char val;

  /* Get the value from an argument */
  val = (char) va_arg(ap, int);

  if (str != NULL && size > 1) {
    str[0] = val;
  }

  return 1;
}

static off_t output_decimal(char *str, size_t size, struct strfmt_format *strfmt,
                            va_list ap)
{
  long long int val;
  long long int q;
  long long int r;
  off_t pos;
  off_t ptr;
  int sz;
  int i;
  char *buf;
  int sign;

  /* Get the value from an argument */
  switch (strfmt->mod) {
  case STRFMT_MOD_LONG:
    val = (long int) va_arg(ap, long int);
    break;
  case STRFMT_MOD_LONGLONG:
    val = (long long int) va_arg(ap, long long int);
    break;
  default:
    val = (int) va_arg(ap, int);
  }

  /* Calculate the maximum buffer size, and allocate a buffer to store a
     decimal string */
  sz = 3 * sizeof(val) + 1; /* max. 3 chars per byte (rough approx.) + sign */
  buf = alloca(sz);
  ptr = 0;

  /* Signed? */
  if (val < 0) {
    sign = -1;
    val *= -1;
  } else {
    sign = 1;
  }

  /* Store the decimal string to the buffer */
  q = val;
  while (q) {
    r = q % 10;
    q = q / 10;
    buf[ptr] = r + '0';
    ptr++;
  }
  if (!ptr) {
    buf[ptr] = '0';
    ptr++;
  }

  /* Append a sign mark */
  if (sign < 0) {
    buf[ptr] = '-';
    ptr++;
  }

  /* Output the formatted string */
  pos = 0;

  /* Padding */
  if (strfmt->pad > strfmt->prec && strfmt->pad > ptr) {
    for (i = 0;
         i < strfmt->pad - strfmt->prec && i < strfmt->pad - ptr;
          i++) {
      if ((size_t) pos + 1 < size) {
        if (strfmt->zero) {
          str[pos] = '0';
        } else {
          str[pos] = ' ';
        }
      }
      pos++;
    }
  }

  /* Precision */
  if (strfmt->prec > ptr) {
    for (i = 0; i < strfmt->prec - ptr; i++) {
      if ((size_t) pos + 1 < size) {
        str[pos] = '0';
      }
      pos++;
    }
  }

  /* Value */
  for (i = 0; i < ptr; i++) {
    if ((size_t) pos + 1 < size) {
      str[pos] = buf[ptr - i - 1];
    }
    pos++;
  }

  return pos;
}
static off_t output_hexdecimal(char *str, size_t size, struct strfmt_format *strfmt,
                               va_list ap, int cap)
{
  unsigned long long int val;
  unsigned long long int q;
  unsigned long long int r;
  off_t pos;
  off_t ptr;
  int sz;
  int i;
  char *buf;

  /* Get the value from an argument */
  switch (strfmt->mod) {
  case STRFMT_MOD_LONG:
    val = (unsigned long int) va_arg(ap, unsigned long int);
    break;
  case STRFMT_MOD_LONGLONG:
    val = (unsigned long long int) va_arg(ap, unsigned long long int);
    break;
  default:
    val = (unsigned int) va_arg(ap, unsigned int);
  }

  /* Calculate the maximum buffer size, and allocate a buffer to store a
     decimal string */
  sz = 2 * sizeof(val); /* max. 3 chars per byte (rough approx.) */
  buf = alloca(sz);
  ptr = 0;

  /* Store the decimal string to the buffer */
  q = val;
  while (q) {
    r = q & 0xf;
    q = q >> 4;
    if (r < 10) {
      buf[ptr] = r + '0';
    } else if ( cap ) {
      buf[ptr] = r + 'A' - 10;
    } else {
      buf[ptr] = r + 'a' - 10;
    }
    ptr++;
  }
  if (!ptr) {
    buf[ptr] = '0';
    ptr++;
  }

  /* Output the formatted string */
  pos = 0;

  /* Padding */
  if (strfmt->pad > strfmt->prec && strfmt->pad > ptr) {
    for (i = 0;
         i < strfmt->pad - strfmt->prec && i < strfmt->pad - ptr;
         i++) {
      if ((size_t) pos + 1 < size) {
        if (strfmt->zero) {
          str[pos] = '0';
        } else {
          str[pos] = ' ';
        }
      }
      pos++;
    }
  }

  /* Precision */
  if (strfmt->prec > ptr) {
    for (i = 0; i < strfmt->prec - ptr; i++) {
      if ((size_t) pos + 1 < size) {
        str[pos] = '0';
      }
      pos++;
    }
  }

  /* Value */
  for (i = 0; i < ptr; i++) {
    if ((size_t) pos + 1 < size) {
      str[pos] = buf[ptr-i-1];
    }
    pos++;
  }

  return pos;
}
static off_t output_pointer(char *str, size_t size, struct strfmt_format *strfmt,
                            va_list ap)
{
  unsigned long long int val;
  unsigned long long int q;
  unsigned long long int r;
  off_t pos;
  off_t ptr;
  int sz;
  int i;
  char *buf;

  /* Get the value from an argument */
  val = (unsigned long long int) va_arg(ap, unsigned long long int);

  /* Calculate the maximum buffer size, and allocate a buffer to store a
     decimal string */
  sz = 2 * sizeof(val); /* max. 3 chars per byte (rough approx.) */
  buf = alloca(sz);
  ptr = 0;

  /* Store the decimal string to the buffer */
  q = val;
  while (q) {
    r = q & 0xf;
    q = q >> 4;
    if (r < 10) {
      buf[ptr] = r + '0';
    } else {
      buf[ptr] = r + 'a' - 10;
    }
    ptr++;
  }
  if (!ptr) {
    buf[ptr] = '0';
    ptr++;
  }

  /* Output the formatted string */
  pos = 0;

  /* Padding */
  if (strfmt->pad > strfmt->prec && strfmt->pad > ptr) {
    for (i = 0;
         i < strfmt->pad - strfmt->prec && i < strfmt->pad - ptr;
         i++) {
      if ((size_t) pos + 1 < size) {
        if (strfmt->zero) {
          str[pos] = '0';
        } else {
          str[pos] = ' ';
        }
      }
      pos++;
    }
  }

  /* Precision */
  if (strfmt->prec > ptr) {
    for (i = 0; i < strfmt->prec - ptr; i++) {
      if ((size_t) pos + 1 < size) {
        str[pos] = '0';
      }
      pos++;
    }
  }

  /* Value */
  for (i = 0; i < ptr; i++) {
    if ((size_t) pos + 1 < size) {
      str[pos] = buf[ptr-i-1];
    }
    pos++;
  }

  return pos;
}
static off_t output_string(char *str, size_t size, va_list ap)
{
  char *s;
  off_t pos;

  /* Get the value */
  if ((s = va_arg(ap, char *)) == NULL) {
    s = "(null)";
  }

  pos = 0;
  while (s[pos]) {
    if ((size_t)pos + 1 < size) {
      str[pos] = s[pos];
    }
    pos++;
  }

  return pos;
}

/*
 * kvsnprintf -- formated output conversion
 *
 * RETURN VALUES
 *      If successful, the kvsnprintf() function returns the number of
 *      characters that would have been printed if the size were unlimited (not
 *      including the trailing `\0' used to end output to strings).  It returns
 *      the value of -1 if an error occurs.
 */
int kvsnprintf(char *str, size_t size, const char *format, va_list ap)
{
  struct strfmt_format strfmt;
  off_t pos;
  char *tmpstr;

  /* Allocate memory to store the resulting string from the stack */
  if ((tmpstr = alloca(size)) == NULL) {
    return -1;
  }

  /* Look through the format string until its end */
  pos = 0;
  while (*format != '\0') {
    if (*format == '%') {
      /* % character  */
      parse_format(&format, &strfmt);
      if ((off_t) size > pos) {
        pos += output(tmpstr + pos, size - pos, &strfmt, ap);
      } else {
        pos += output(NULL, 0, &strfmt, ap);
      }
    } else {
      /* Ordinary character */
      if ((size_t) pos + 1 < size) {
        tmpstr[pos] = *format;
      }
      format++;
      pos++;
    }
  }

  /* Insert a trailing '\0' */
  if ((size_t) pos + 1 < size) {
    tmpstr[pos] = '\0';
  }
  tmpstr[size-1] = '\0';

  /* Copy the contents of the temporary buffer to the result */
  kmemcpy(str, tmpstr, size);

  return (int) pos;
}

/*
 * ksnprintf
 */
int ksnprintf(char *str, size_t size, const char *format, ...)
{
  int ret;
  va_list ap;

  va_start(ap, format);
  ret = kvsnprintf(str, size, format, ap);
  va_end(ap);

  return ret;
}
