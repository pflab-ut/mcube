/**
 * @file include/mcube/stat.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_STAT_H__
#define	__MCUBE_MCUBE_STAT_H__

#ifndef __ASSEMBLY__

#if CONFIG_ARCH_SIM
#include <sys/stat.h>
#include <unistd.h>
#else

/**
 * @brief Status of device
 *
 * The stat structure has status information of device.
 */
struct stat {
#if 0
	dev_t st_dev;
	ino_t st_ino;
	mode_t st_mode;
	nlink_t st_nlink;
	uid_t st_uid;
	gid_t st_gid;
	dev_t st_rdev;
	off_t st_size;
	time_t st_atime;
	time_t st_mtime;
	time_t st_ctime;
	blksize_t st_blksize;
	blkcnt_t st_blocks;
	mode_t st_attr;
#endif
}; 

typedef struct stat stat;


#endif /* CONFIG_ARCH_SIM */

#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_MCUBE_STAT_H__ */
