/**
 * @file lib/stdlib.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>



unsigned long user_malloc[MALLOC_SIZE];
mem_block *free_list;


void *malloc(size_t size)
{
	mem_block *bestfit = NULL;
	mem_block *prev = NULL;
	mem_block *cur = free_list;
	size = ((size + sizeof(size_t) - 1) / sizeof(size_t)) * sizeof(size_t);
	PDEBUG("size = %u\n", (unsigned int) size);
	
	while (cur) {
		if (cur->size == size) {
			if (prev) {
				prev->next = cur->next;
			} else {
				free_list = cur->next;
			}
			/* find just memory size */
			return (void *)((unsigned long) cur + sizeof(mem_block));
		} else if (cur->size > size) {
			if (bestfit) {
				if (bestfit->size > cur->size) {
					bestfit = cur;
				}
			} else {
				bestfit = cur;
			}
		}
		prev = cur;
		cur = cur->next;
	}
	/* split the nearest memory size */
	if (bestfit) {
		mem_block *slice = (mem_block *)((unsigned long) bestfit + sizeof(mem_block) + size);
		slice->size = bestfit->size - (size + sizeof(mem_block));
		if (bestfit == free_list) {
			slice->next = free_list->next;
			free_list = slice;
		} else {
			slice->next = free_list;
			free_list = slice;
		}
		bestfit->size = size;
		return (void *)((unsigned long) bestfit + sizeof(mem_block));
	}
	return NULL;
}


void free(void *objp)
{
	mem_block *del_mem = (mem_block *)((unsigned long) objp - sizeof(mem_block));
	int connected = 0;
	if ((unsigned long) objp < (unsigned long) user_malloc || (unsigned long) objp >= (unsigned long) user_malloc + sizeof(user_malloc)) {
		//		print("Invalid address %x\n", objp);
		return;
	}
	do {
		//		PDEBUG("connected = %x\n", connected);
		connected &= 0x01;
		mem_block *cur = free_list;
		mem_block *prev = NULL;
		while (cur) {
			//			PDEBUG("cur = %x del_mem = %x\n", cur, del_mem);
			if (cur != del_mem) {
				if (((unsigned long) del_mem + sizeof(mem_block) + del_mem->size) == (unsigned long) cur) {
					/* merge cur memory and post memory */
					del_mem->next = cur->next;
					del_mem->size += sizeof(mem_block) + cur->size;
					connected = 0x03;
					if (!prev) {
						free_list = del_mem;
					} else if (prev != del_mem) {
						prev->next = del_mem;
					}
					break;
				} else if (((unsigned long) cur + sizeof(mem_block) + cur->size) == (unsigned long) del_mem ) {
					/* merge cur memory and prev memory */
					cur->size += sizeof(mem_block) + del_mem->size;
					del_mem = cur;
					connected = 0x03;
					break;
				}
			}
			prev = cur;
			cur = cur->next;
		}
	} while (connected & 0x02);

  if (connected == 0) {
		/* add freelist if neighbour memory does not exist */
		del_mem->next = free_list;
		free_list = del_mem;
	}
}

#if !CONFIG_ARCH_AXIS

double strtod(const char *nptr, char **endptr)
{
	const char* org = nptr;
	bool valid = FALSE;
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
		valid = TRUE;
		do {
			value = value * 10.0 + (*nptr - '0');
			nptr++;
		} while (isdigit((unsigned char) *nptr));
	}
	if (*nptr == '.') {
		valid = FALSE;
		nptr++;
		if (isdigit((unsigned char) *nptr)) {
			small = 0.1;
			valid = TRUE;
			do {
				value += small * (*nptr - '0');
				small *= 0.1;
				nptr++;
			} while (isdigit((unsigned char) *nptr));
		}
	}
	if (valid && (*nptr == 'e' || *nptr == 'E')) {
		nptr++;
		valid = FALSE;
		psign = +1.0;
		if (*nptr == '+') {
			nptr++;
		} else if (*nptr == '-') {
			psign = -1.0;
			nptr++;
		}
		if (isdigit((unsigned char) *nptr)) {
			valid = TRUE;
			p = 0.0;
			do {
				p = p * 10.0 + (*nptr - '0');
				nptr++;
			} while (isdigit((unsigned char) *nptr));
			value *= pow(10.0, psign * p);
		}
	}
	if (valid) {
		*endptr = (char*) nptr;
	} else {
		*endptr = (char*) org;
	}
	return sign * value;
}


#endif /* !CONFIG_ARCH_AXIS */

unsigned long strtoul(const char *cp, char **endp, int base)
{
  unsigned long result = 0, value;
	char *ep = (char *) cp + strlen((char *) cp);

  if (!base) {
    base = 10;
    if (*cp == '0') {
      base = 8;
      cp++;
      if ((toupper(*cp) == 'X') && isxdigit(cp[1])) {
        cp++;
        base = 16;
      }
    }
  } else if (base == 16) {
    if (cp[0] == '0' && toupper(cp[1]) == 'X') {
      cp += 2;
		}
  }
  while (isxdigit(*cp)
         && (value = isdigit(*cp) ? *cp - '0' : toupper(*cp)- 'A' + 10) < (unsigned long) base) {
		//		print("*cp = %c\n", *cp);
    result = result * base + value;
    cp++;
  }
  if (endp) {
    *endp = (char *) cp;
	}
	if (ep != cp) {
		//		print("Error!\n");
		return ULONG_MAX;
	}
	//	print("result = %d\n", result);
  return result;
}

long strtol(const char *cp, char **endp, int base)
{
  if (*cp == '-') {
    return -strtoul(cp + 1, endp, base);
	}
  return strtoul(cp, endp, base);
}

static void xtoa(unsigned long val, char *buf, unsigned radix, int negative)
{
  char *p;
  char *firstdig;
  char temp;
  unsigned digval;

  p = buf;

  if (negative) {
		/* Negative, so output '-' and negate */
		*p++ = '-';
		val = (unsigned long)(-(long) val);
	}
	
  /* Save pointer to first digit */
  firstdig = p;

  do {
			digval = (unsigned) (val % radix);
			val /= radix;

			/* Convert to ascii and store */
			if (digval > 9) {
				*p++ = (char) (digval - 10 + 'a');
			} else {
				*p++ = (char) (digval + '0');
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

char *itoa(int val, char *buf, int radix)
{
  if (radix == 10 && val < 0) {
    xtoa((unsigned long) val, buf, radix, 1);
	} else {
    xtoa((unsigned long)(unsigned int) val, buf, radix, 0);
	}
  return buf;
}

char *ltoa(long val, char *buf, int radix)
{
  xtoa((unsigned long) val, buf, radix, (radix == 10 && val < 0));
  return buf;
}

char *ultoa(unsigned long val, char *buf, int radix)
{
  xtoa(val, buf, radix, 0);
  return buf;
}


void qsort(void *base, size_t num, size_t size, sortcmp cmp)
{
  uint8_t pivot[size], tmp[size]; // use C99 VLAs instead of alloca.

  uint8_t *b = (uint8_t *)base;
  for (;;) {
    if (num < 2) {
      return;
    }

    // Use the first element as the pivot.
    memcpy(pivot, b, size);

    // Partition.
    size_t part;
    {
      // Work from outwards in (C.A.R. Hoare version of algorithm).
      uint8_t *i = b - (size * 1);
      uint8_t *j = b + (size * num);

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
    }

    // Recursively sort the left side of the partition.
    qsort(b, part + 1, size, cmp);

    // For the right side of the partition, do tail recursion.
    b   += (part + 1) * size;
    num -= part + 1;
  }
}
