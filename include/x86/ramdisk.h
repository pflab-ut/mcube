#ifndef _RAMDISK_H
#define _RAMDISK_H

#ifndef __ASSEMBLY__

int ramdisk_get_len(void);
char *ramdisk_get_buf(void);
void *ramdisk_memory_area_end(void);

void ramdisk_init(void);

#endif /* !__ASSEMBLY__ */

#endif /* _RAMDISK_H */
