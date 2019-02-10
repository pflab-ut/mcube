/**
 * @file include/mcube/fs.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_FS_H__
#define __MCUBE_MCUBE_FS_H__

#if CONFIG_OPTION_FS_FAT
#include <mcube/fs/fat.h>
#elif CONFIG_OPTION_FS_EXT2
#include <mcube/fs/ext2.h>
#endif /* CONFIG_OPTION_FS_FAT */

#ifndef __ASSEMBLY__

void init_fat(void);
void init_ext2(void);

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_FS_H__ */
