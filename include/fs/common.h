/**
 * @file include/fs/common.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_FS_COMMON_H__
#define __MCUBE_FS_COMMON_H__

/**
 * @def NR_FILES
 * @brief Number of files.
 */
#define NR_FILES 128

/**
 * @def FILE_LENGTH
 * @brief File length.
 */
#define FILE_LENGTH 128

#ifndef __ASSEMBLY__

/**
 * @struct file_struct
 * @brief File information.
 */
struct file_struct {
  uint64_t inum;    /* Inode# of the open()-ed file */
  int flags;    /* Flags passed  to open() call */
  spinlock_t lock;  /* ONLY FOR offset and refcount */
  uint64_t offset;  /* MAIN FIELD: File byte offset */
  int refcount;    /* Reference count; fork,dup,.. */
  char pathname[FILE_LENGTH];
};

/**
 * @var files
 * @brief File structures.
 */
extern struct file_struct files[NR_FILES];

/**
 * @fn void init_ext2(void)
 * @brief initialize Ext2 file system.
 */
void init_ext2(void);

/**
 * @fn void init_fat(void)
 * @brief initialize FAT file system.
 */
void init_fat(void);

#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_FS_COMMON_H__ */
