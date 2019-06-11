/**
 * @file include/x86_64/ramdisk.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_64_RAMDISK_H__
#define __MCUBE_X86_64_RAMDISK_H__

#ifndef __ASSEMBLY__

/*
 * Ramdisk header format.
 */
#define RDSIG_LEN   8
#define RDSIG_START "McubeSta"
#define RDSIG_END   "McubeEnd"

/**
 * @struct ramdisk_header
 * @brief Ramdisk header information.
 */
struct ramdisk_header {
  char start_signature[RDSIG_LEN];
  uint32_t sectors;    /* Rdisk len in 512-byte sectors (+hdr)*/
  uint32_t length;    /* Rdisk len in bytes (without hdr) */
  char end_signature[RDSIG_LEN];
} __packed;

extern struct ramdisk_header *rdheader;

/**
 * @struct ramdisk
 * @brief Ramdisk information.
 */
struct ramdisk {
  char *buf;
  int len;
};

extern struct ramdisk ramdisk;

int ramdisk_get_len(void);
char *ramdisk_get_buf(void);
void *ramdisk_memory_area_end(void);

void ramdisk_init(void);

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_64_RAMDISK_H__ */
