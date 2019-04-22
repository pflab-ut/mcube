/**
 * @file arch/x86/ramdisk.c
 *
 * @author Hiroyuki Chishiro
 */
/*
 * Ramdisk-header parsing
 *
 * Copyright (C) 2012 Ahmed S. Darwish <darwish.07@gmail.com>
 *
 * After basic sanity checks, export the ramdisk start address & length
 * to the rest of the kernel.
 *
 * Check the 'build-hdimage.py' script and the early assembly code which
 * loads the ramdisk for details & rationale. Check them NOW as the info
 * stated there will NOT be redundantly repeated here!
 */

#include <mcube/mcube.h>

/*
 * Ramdisk header format.
 */
#define RDSIG_LEN   8
#define RDSIG_START "McubeSta"
#define RDSIG_END   "McubeEnd"

static struct ramdisk_header {
  char start_signature[RDSIG_LEN];
  uint32_t sectors;    /* Rdisk len in 512-byte sectors (+hdr)*/
  uint32_t length;    /* Rdisk len in bytes (without hdr) */
  char end_signature[RDSIG_LEN];
} __packed *rdheader;

static struct ramdisk {
  char *buf;
  int len;
} ramdisk;

/*
 * The page allocator puts its 'page frame descriptor table'
 * directly after the ramdisk image. Inform it about our end
 * mark.
 */
void *ramdisk_memory_area_end(void)
{
  assert(rdheader);
  return (char *) ramdisk.buf + ramdisk.len;
}

void ramdisk_init(void)
{
  /* Ramdisk header is loaded directly after kernel image */
  rdheader = VIRTUAL(KTEXT_PHYS(__kernel_end));

  if (memcmp(rdheader->start_signature, RDSIG_START, RDSIG_LEN)) {
    panic("Ramdisk: Invalid header start signature");
  }

  if (memcmp(rdheader->end_signature, RDSIG_END, RDSIG_LEN)) {
    panic("Ramdisk: Invalid header end signature");
  }

  ramdisk.buf = (char *)(rdheader + 1);
  ramdisk.len = rdheader->length;

  if (ramdisk.len != 0) {
    printk("Ramdisk: start address = 0x%lx, length = %d KB\n",
           ramdisk.buf, ramdisk.len / 1024);
  } else {
    printk("Ramdisk: No disk image loaded\n");
  }
}

/*
 * Export ramdisk details to the rest of our kernel
 */

int ramdisk_get_len(void)
{
  assert(rdheader);
  return ramdisk.len;
}

char *ramdisk_get_buf(void)
{
  assert(rdheader);
  return ramdisk.buf;
}
