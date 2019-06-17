/**
 * @file lib/mm.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

unsigned long LOW_MEMORY;
unsigned long HIGH_MEMORY;
unsigned short *mem_map;


void memdump(void *ptr, size_t n)
{
  unsigned long a, b;
  unsigned char c;

  for (a = (unsigned long) ptr; a < (unsigned long) ptr + n; a += 16) {
    printk("%lx: ", a);

    for (b = 0; b < 16; b++) {
      printk("%02x ", mmio_in8(a + b));

      if (b % 4 == 3) {
        putchar(' ');
      }
    }

    for (b = 0; b < 16; b++) {
      c = mmio_in8(a + b);
      putchar(c < 32 || c >= 127 ? '.' : c);
    }

    putchar('\r');
    putchar('\n');
  }
}
