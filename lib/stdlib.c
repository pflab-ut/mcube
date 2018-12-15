/**
 * @file lib/stdlib.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

#if !CONFIG_ARCH_SIM


atomic_int global_malloc_lock = SPIN_UNLOCKED;
unsigned char user_malloc[MALLOC_SIZE];
struct mem_block_header *head;
struct mem_block_header *tail;

void init_malloc(void)
{
  print("init_malloc()\n");
  int i;
  head = (struct mem_block_header *) user_malloc;
  tail = (struct mem_block_header *) &user_malloc[MALLOC_SIZE];
  struct mem_block_header *current = head;
  for (i = 0; i < BLOCK_NUM; i++) {
    current->next = current + BLOCK_SIZE;
    current->size = BLOCK_SIZE - sizeof(struct mem_block_header);
    current->is_free = TRUE;
    current = current->next;
  }
}


void *sbrk(size_t increment)
{
  return (void *) -1;
}


struct mem_block_header *get_free_block(size_t size)
{
  struct mem_block_header *current = head;
  while (current) {
    if (current->is_free && current->size >= size) {
      return current;
    }
    current = current->next;
  }
  return NULL;
}

void *malloc(size_t size)
{
  size_t total_size;
  void *block;
  struct mem_block_header *header;
  if (!size) {
    return NULL;
  }
  spin_lock(&global_malloc_lock);
  header = get_free_block(size);
  if (header) {
    header->is_free = FALSE;
    spin_unlock(&global_malloc_lock);
    return (void *)(header + 1);
  }
  total_size = sizeof(struct mem_block_header) + size;
  block = sbrk(total_size);
  if (block == (void *) -1) {
    spin_unlock(&global_malloc_lock);
    return NULL;
  }
  header = block;
  header->size = size;
  header->is_free = FALSE;
  header->next = NULL;
  if (!head) {
    head = header;
  }
  if (tail) {
    tail->next = header;
  }
  tail = header;
  spin_unlock(&global_malloc_lock);
  return (void*)(header + 1);
}


void free(void *block)
{
  struct mem_block_header *header, *tmp;
  void *programbreak;

  if (!block) {
    return;
  }
  spin_lock(&global_malloc_lock);
  header = (struct mem_block_header*)block - 1;

  programbreak = sbrk(0);
  if ((char *) block + header->size == programbreak) {
    if (head == tail) {
      head = tail = NULL;
    } else {
      tmp = head;
      while (tmp) {
        if(tmp->next == tail) {
          tmp->next = NULL;
          tail = tmp;
        }
        tmp = tmp->next;
      }
    }
    sbrk(0 - sizeof(struct mem_block_header) - header->size);
    spin_unlock(&global_malloc_lock);
    return;
  }
  header->is_free = TRUE;
  spin_unlock(&global_malloc_lock);
}

void *calloc(size_t num, size_t nsize)
{
  size_t size;
  void *block;
  if (!num || !nsize) {
    return NULL;
  }
  size = num * nsize;
  /* check mul overflow */
  if (nsize != size / num) {
    return NULL;
  }
  block = malloc(size);
  if (!block) {
    return NULL;
  }
  memset(block, 0, size);
  return block;
}

void *realloc(void *block, size_t size)
{
  struct mem_block_header *header;
  void *ret;
  if (!block || !size) {
    return malloc(size);
  }
  header = (struct mem_block_header*) block - 1;
  if (header->size >= size) {
    return block;
  }
  ret = malloc(size);
  if (ret) {

    memcpy(ret, block, header->size);
    free(block);
  }
  return ret;
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

#endif /* CONFIG_ARCH_SIM */
