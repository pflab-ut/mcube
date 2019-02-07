/**
 * @file user/user.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

#if !CONFIG_FS_EXT2

void init_ext2(void)
{
}

#endif /* !CONFIG_FS_EXT2 */


#if !CONFIG_FS_FAT

void init_fat(void)
{
}

#endif /* !CONFIG_FS_FAT */
