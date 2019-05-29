/**
 * @file include/x86_64/ramdisk.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_64_RAMDISK_H__
#define __MCUBE_X86_64_RAMDISK_H__

#ifndef __ASSEMBLY__

int ramdisk_get_len(void);
char *ramdisk_get_buf(void);
void *ramdisk_memory_area_end(void);

void ramdisk_init(void);

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_64_RAMDISK_H__ */
