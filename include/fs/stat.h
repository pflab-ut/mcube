/**
 * @file include/fs/stat.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_FS_STAT_H__
#define __MCUBE_FS_STAT_H__

/*
 * stat.h - Data returned by the stat() function
 *
 * Taken from POSIX:2001
 */

#ifndef __ASSEMBLY__

#if !CONFIG_ARCH_SIM

/**
 * @typedef dev_t
 * @brief Typedef of device.
 */
typedef uint64_t dev_t;

/**
 * @typedef ino_t
 * @brief Typedef of inode.
 */
typedef uint64_t ino_t;

/**
 * @typedef nlink_t
 * @brief Typedef of number of links.
 */
typedef uint64_t nlink_t;

/**
 * @typedef uid_t
 * @brief Typedef of user ID.
 */
typedef uint32_t uid_t;


/**
 * @typedef gid_t
 * @brief Typedef of group ID.
 */
typedef uint32_t gid_t;

/**
 * @typedef off_t
 * @brief Typedef of offset.
 */
typedef int64_t off_t;

/**
 * @typedef blkcnt_t
 * @brief Typedef of block count.
 */
typedef int64_t blkcnt_t;

/**
 * @typedef blksize_t
 * @brief Typedef of block size.
 */
typedef uint64_t blksize_t;

/**
 * @typedef time_t
 * @brief Typedef of time.
 */
typedef uint64_t time_t;

/**
 * @enum mode
 * @brief File permission bits, (or) creation mode specified at open(,,@a mode).
 *
 * I_[R,W,X][USR,GRP,OTH], I_[S,G]UID, & I_SVTX "shall be unique." --SUSv3
 *
 * NOTE! The symbol values are taken from Ext2 specification to ease up
 * stat() system call implementation.
 */
enum mode {
  // 1- Process execution user/group override
  S_ISUID    = 0x0800,  /* Set Process UID bit */
  S_IGUID    = 0x0400,  /* Set Group UID bit */
  S_ISVTX    = 0x0200,  /* Sticky bit */

  // 2- Access rights bit mask
  S_IRUSR    = 0x0100,  /* R for owner */
  S_IWUSR    = 0x0080,  /* W for owner */
  S_IXUSR    = 0x0040,  /* X for owner */
  S_IRGRP    = 0x0020,  /* R for group */
  S_IWGRP    = 0x0010,  /* W for group */
  S_IXGRP    = 0x0008,  /* X for group */
  S_IROTH    = 0x0004,  /* R for other */
  S_IWOTH    = 0x0002,  /* W for other */
  S_IXOTH    = 0x0001,  /* X for other */

  S_IRWXU    = S_IRUSR | S_IWUSR | S_IXUSR,
  S_IRWXG    = S_IRGRP | S_IWGRP | S_IXGRP,
  S_IRWXO    = S_IROTH | S_IWOTH | S_IXOTH,

  // 3- File Type
  S_IFMT    = 0xf000,  /* Mask for the 'file type' mode bits */
  S_IFSOCK  = 0xc000,  /* Socket */
  S_IFLNK    = 0xa000,  /* Symbolic link */
  S_IFREG    = 0x8000,  /* Regular file */
  S_IFBLK    = 0x6000,  /* Block device */
  S_IFDIR    = 0x4000,  /* Directory */
  S_IFCHR    = 0x2000,  /* Character device */
  S_IFIFO    = 0x1000,  /* FIFO */

};

/**
 * @typedef mode_t
 * @brief Typedef of @enum mode.
 */
typedef enum mode mode_t;

/**
 * @def S_ISDIR(mode)
 * @brief Is mode directory?
 *
 * @param mode Mode.
 */
#define S_ISDIR(mode) (((mode) & S_IFMT) == S_IFDIR)

/**
 * @def S_ISCHR(mode)
 * @brief Is mode character device?
 *
 * @param mode Mode.
 */
#define S_ISCHR(mode) (((mode) & S_IFMT) == S_IFCHR)

/**
 * @def S_ISBLK(mode)
 * @brief Is mode block device?
 *
 * @param mode Mode.
 */
#define S_ISBLK(mode) (((mode) & S_IFMT) == S_IFBLK)

/**
 * @def S_ISREG(mode)
 * @brief Is mode regular file?
 *
 * @param mode Mode.
 */
#define S_ISREG(mode) (((mode) & S_IFMT) == S_IFREG)

/**
 * @def S_ISFIFO(mode)
 * @brief Is mode FIFO?
 *
 * @param mode Mode.
 */
#define S_ISFIFO(mode) (((mode) & S_IFMT) == S_IFIFO)

/**
 * @def S_ISLNK(mode)
 * @brief Is mode symbolic link?
 */
#define S_ISLNK(mode) (((mode) & S_IFMT) == S_IFLNK)

/**
 * @def S_ISSOCK(mode)
 * @brief Is mode socket?
 */
#define S_ISSOCK(mode) (((mode) & S_IFMT) == S_IFSOCK)

/**
 * @struct stat
 * @brief Inode information returned by the stat() group of functions.
 */
struct stat {
  /**
   * Device ID of device containing file.
   */
  dev_t st_dev;

  /**
   * File serial (inode) number.
   */
  ino_t st_ino;

  /**
   * Inode mode bits (see above).
   */
  mode_t st_mode;

  /**
   * Number of hard links to the file.
   */
  nlink_t st_nlink;

  /**
   * User ID of file.
   */
  uid_t st_uid;

  /**
   * Group ID of file.
   */
  gid_t st_gid;

  /**
   * Device ID, if file is char or block special.
   */
  dev_t st_rdev;

  /**
   * For regular files, the file size in bytes.
   * For symbolic links, the length in bytes of
   * the pathname contained in the symbolic link.
   */
  off_t st_size;

  /**
   * Time of last access.
   */
  time_t st_atime;

  /**
   * Time of last data modification.
   */
  time_t st_mtime;

  /**
   * Time of last file status change.
   */
  time_t st_ctime;
};

#endif /* !CONFIG_ARCH_SIM */

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_FS_STAT_H__ */
