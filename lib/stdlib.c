/**
 * @file lib/stdlib.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

#if !CONFIG_ARCH_SIM


#if defined(ENABLE_FPU)

double strtod(const char *nptr, char **endptr)
{
  const char *org = nptr;
  bool valid = false;
  double value = 0.0;
  double sign = 1.0;
  double psign;
  double small;
  double p;

  if (*nptr == '+') {
    nptr++;
  } else if (*nptr == '-') {
    sign = -1.0;
    nptr++;
  }

  if (isdigit((unsigned char) *nptr)) {
    valid = true;

    do {
      value = value * 10.0 + (*nptr - '0');
      nptr++;
    } while (isdigit((unsigned char) *nptr));
  }

  if (*nptr == '.') {
    valid = false;
    nptr++;

    if (isdigit((unsigned char) *nptr)) {
      small = 0.1;
      valid = true;

      do {
        value += small * (*nptr - '0');
        small *= 0.1;
        nptr++;
      } while (isdigit((unsigned char) *nptr));
    }
  }

  if (valid && (*nptr == 'e' || *nptr == 'E')) {
    nptr++;
    valid = false;
    psign = +1.0;

    if (*nptr == '+') {
      nptr++;
    } else if (*nptr == '-') {
      psign = -1.0;
      nptr++;
    }

    if (isdigit((unsigned char) *nptr)) {
      valid = true;
      p = 0.0;

      do {
        p = p * 10.0 + (*nptr - '0');
        nptr++;
      } while (isdigit((unsigned char) *nptr));

      value *= pow(10.0, psign * p);
    }
  }

  if (valid) {
    *endptr = (char *) nptr;
  } else {
    *endptr = (char *) org;
  }

  return sign * value;
}


#endif /* ENABLE_FPU */

unsigned long strtoul(const char *nptr, char **endp, int base)
{
  unsigned long result = 0, value;
  char *ep = (char *) nptr + strlen((char *) nptr);

  if (!base) {
    base = 10;

    if (*nptr == '0') {
      base = 8;
      nptr++;

      if ((toupper(*nptr) == 'X') && isxdigit(nptr[1])) {
        nptr++;
        base = 16;
      }
    }
  } else if (base == 16) {
    if (nptr[0] == '0' && toupper(nptr[1]) == 'X') {
      nptr += 2;
    }
  }

  while (isxdigit(*nptr)
         && (value = isdigit(*nptr) ? *nptr - '0' : toupper(*nptr) - 'A' + 10) <
         (unsigned long) base) {
    //    print("*nptr = %c\n", *nptr);
    result = result * base + value;
    nptr++;
  }

  if (endp) {
    *endp = (char *) nptr;
  }

  if (ep != nptr) {
    //    print("Error!\n");
    return ULONG_MAX;
  }

  //  print("result = %d\n", result);
  return result;
}

long strtol(const char *nptr, char **endp, int base)
{
  if (*nptr == '-') {
    return -strtoul(nptr + 1, endp, base);
  }

  return strtoul(nptr, endp, base);
}

static void xtoa(unsigned long val, char *str, unsigned base, int negative)
{
  char *p;
  char *firstdig;
  char temp;
  unsigned digval;

  p = str;

  if (negative) {
    /* Negative, so output '-' and negate */
    *p++ = '-';
    val = (unsigned long)(-(long) val);
  }

  /* Save pointer to first digit */
  firstdig = p;

  do {
    digval = (unsigned)(val % base);
    val /= base;

    /* Convert to ascii and store */
    if (digval > 9) {
      *p++ = (char)(digval - 10 + 'a');
    } else {
      *p++ = (char)(digval + '0');
    }
  } while (val > 0);

  /* We now have the digit of the number in the buffer, but in reverse
   * order.  Thus we reverse them now.
   */
  *p-- = '\0';

  do  {
    temp = *p;
    *p = *firstdig;
    *firstdig = temp;
    p--;
    firstdig++;
  } while (firstdig < p);
}

char *itoa(int val, char *str, int base)
{
  if (base == 10 && val < 0) {
    xtoa((unsigned long) val, str, base, 1);
  } else {
    xtoa((unsigned long) val, str, base, 0);
  }

  return str;
}

char *ltoa(long val, char *str, int base)
{
  xtoa((unsigned long) val, str, base, (base == 10 && val < 0));
  return str;
}

char *ultoa(unsigned long val, char *str, int base)
{
  xtoa(val, str, base, 0);
  return str;
}


void qsort(void *base, size_t nmemb, size_t size, sortcmp cmp)
{
  uint8_t pivot[size], tmp[size]; // use C99 VLAs instead of alloca.

  uint8_t *b = (uint8_t *) base;

  for (;;) {
    if (nmemb < 2) {
      return;
    }

    // Use the first element as the pivot.
    memcpy(pivot, b, size);

    // Partition.
    size_t part;
    // Work from outwards in (C.A.R. Hoare version of algorithm).
    uint8_t *i = b - (size * 1);
    uint8_t *j = b + (size * nmemb);

    for (;;) {
      do {
        i += size;
      } while (cmp(i, pivot) < 0);

      do {
        j -= size;
      } while (cmp(j, pivot) > 0);

      if (i >= j) {
        part = (j - b) / size;
        break;
      }

      // Swap elements i and j.
      memcpy(tmp, i, size);
      memcpy(i, j, size);
      memcpy(j, tmp, size);
    }

    // Recursively sort the left side of the partition.
    qsort(b, part + 1, size, cmp);

    // For the right side of the partition, do tail recursion.
    b += (part + 1) * size;
    nmemb -= part + 1;
  }
}


#endif /* !CONFIG_ARCH_SIM */
