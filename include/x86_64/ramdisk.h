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

/**
 * @def RDSIG_LEN
 * @brief Ramdisk signature length.
 */
#define RDSIG_LEN 8

/**
 * @def RDSIG_START
 * @brief Ramdisk signature start.
 */
#define RDSIG_START "McubeSta"

/**
 * @def RDSIG_END
 * @brief Ramdisk signature end.
 */
#define RDSIG_END "McubeEnd"

/**
 * @struct ramdisk_header
 * @brief Ramdisk header information.
 */
struct ramdisk_header {
  /**
   * Start signature.
   */
  char start_signature[RDSIG_LEN];

  /**
   * Ramdisk length in 512-byte sectors (+hdr).
   */
  uint32_t sectors;

  /**
   * Ramdisk len in bytes (without hdr).
   */
  uint32_t length;

  /**
   * End signature.
   */
  char end_signature[RDSIG_LEN];
} __packed /** packed. */;


/**
 * @var rdheader
 * @brief Ramdisk header.
 */
extern struct ramdisk_header *rdheader;

/**
 * @struct ramdisk
 * @brief Ramdisk information.
 */
struct ramdisk {
  /**
   * Buffer.
   */
  char *buf;

  /**
   * Length.
   */
  int len;
};

/**
 * @var ramdisk
 * @brief Ramdisk.
 */
extern struct ramdisk ramdisk;

/**
 * @fn int ramdisk_get_len(void)
 * @brief get ramdisk length.
 *
 * @return Ramdisk length.
 */
int ramdisk_get_len(void);

/**
 * @fn char *ramdisk_get_buf(void)
 * @brief get ramdisk buffer.
 *
 * @return Ramdisk buffer.
 */
char *ramdisk_get_buf(void);

/**
 * @fn void *ramdisk_memory_area_end(void)
 * @brief Ramdisk memory area end.
 */
void *ramdisk_memory_area_end(void);

/**
 * @fn void ramdisk_init(void)
 * @brief initialize ramdisk.
 */
void ramdisk_init(void);

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_64_RAMDISK_H__ */
