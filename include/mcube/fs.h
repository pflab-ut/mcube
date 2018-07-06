/**
 * @file include/mcube/fs.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_FS_H__
#define	__MCUBE_MCUBE_FS_H__

#ifndef __ASSEMBLY__

#if CONFIG_FS_FAT
#include <mcube/fs/fat.h>
#endif /* CONFIG_FS_FAT */

#if CONFIG_DISK_RAM
#include <mcube/fs/ram.h>
#endif /* CONFIG_DISK_RAM */


#define DISK_SIZE 0x80000

extern uint8_t user_disk[DISK_SIZE];
extern void init_fs(void);
extern void print_disk(void);

#define FS_NAME_LEN	64


/* MS-DOS filesystem structures -- I included them here instead of
   including linux/msdos_fs.h since that doesn't include some fields we
   need */

#define ATTR_RO      1		/* read-only */
#define ATTR_HIDDEN  2		/* hidden */
#define ATTR_SYS     4		/* system */
#define ATTR_VOLUME  8		/* volume label */
#define ATTR_DIR     16		/* directory */
#define ATTR_ARCH    32		/* archived */

#define ATTR_NONE    0		/* no attribute bits */
#define ATTR_UNUSED  (ATTR_VOLUME | ATTR_ARCH | ATTR_SYS | ATTR_HIDDEN)
	/* attribute bits that are copied "as is" */

/* FAT values */
#define FAT_EOF      (atari_format ? 0x0fffffff : 0x0ffffff8)
#define FAT_BAD      0x0ffffff7

#define MSDOS_EXT_SIGN 0x29	/* extended boot sector signature */
#define MSDOS_FAT12_SIGN "FAT12   "	/* FAT12 filesystem signature */
#define MSDOS_FAT16_SIGN "FAT16   "	/* FAT16 filesystem signature */
#define MSDOS_FAT32_SIGN "FAT32   "	/* FAT32 filesystem signature */

#define BOOT_SIGN 0xAA55	/* Boot sector magic number */

#define MAX_CLUST_12	((1 << 12) - 16)
#define MAX_CLUST_16	((1 << 16) - 16)
#define MIN_CLUST_32    65529
/* M$ says the high 4 bits of a FAT32 FAT entry are reserved and don't belong
 * to the cluster number. So the max. cluster# is based on 2^28 */
#define MAX_CLUST_32	((1 << 28) - 16)

#define FAT12_THRESHOLD	4085

#define OLDGEMDOS_MAX_SECTORS	32765
#define GEMDOS_MAX_SECTORS	65531
#define GEMDOS_MAX_SECTOR_SIZE	(16*1024)

#define BOOTCODE_SIZE		448
#define BOOTCODE_FAT32_SIZE	420


/**
 * @brief MS DOS volume information
 *
 * The msdos_volume_info structure has MS DOS volume information.
 */
struct msdos_volume_info {
	/** BIOS drive number. */
  uint8_t	drive_number;
	/* Unused. */
  uint8_t	RESERVED;
	/** 0x29 if fields below exist (DOS 3.3+) */
  uint8_t ext_boot_sign;
	/** Volume ID number. */
  uint8_t	volume_id[4];
	/** Volume label. */
  uint8_t	volume_label[11];
	/** Typically FAT12 or FAT16. */
  uint8_t	fs_type[8];
} __attribute__ ((packed));

typedef struct msdos_volume_info msdos_volume_info;

/**
 * @brief MS DOS boot sector information
 *
 * The msdos_boot_sector structure has MS DOS boot sector information.
 */
struct msdos_boot_sector {
	/** Boot strap short or near jump. */
  uint8_t boot_jump[3];
	/** Name - can be used to special case partition manager volumes. */
  uint8_t system_id[8];
	/** Bytes per logical sector. */
  uint8_t sector_size[2];
	/** Sectors/cluster. */
  uint8_t cluster_size;
	/** Reserved sectors. */
  uint16_t reserved;
	/** number of FATs. */
  uint8_t fats;
	/** Root directory entries. */
  uint8_t dir_entries[2];
	/** Number of sectors. */
  uint8_t sectors[2];
	/** Media code (unused). */
  uint8_t media;
	/** Sectors/FAT. */
  uint16_t fat_length;
	/** Sectors per track. */
  uint16_t secs_track;
	/** Number of heads. */
  uint16_t heads;
	/** Hidden sectors (unused). */
  uint32_t hidden;
	/** Number of sectors (if sectors == 0). */
  uint32_t total_sect;
  union {
    struct {
			/** Volume information. */
      struct msdos_volume_info vi;
			/** Boot code. */
      uint8_t	boot_code[BOOTCODE_SIZE];
    } __attribute__ ((packed)) _oldfat;
    struct {
			/** Sectors/FAT. */
      uint32_t fat32_length;
			/** Bit 8: fat mirroring, low 4: active fat. */
      uint16_t flags;
			/** major, minor filesystem version. */
      uint8_t	version[2];
			/* First cluster in root directory. */
      uint32_t root_cluster;
			/* Filesystem info sector. */
      uint16_t info_sector;
			/* Backup boot sector. */
      uint16_t backup_boot;
			/* Unused. */
      uint16_t reserved2[6];
			/** Volume information. */
      struct msdos_volume_info vi;
			/** Boot code. */
      uint8_t	boot_code[BOOTCODE_FAT32_SIZE];
    } __attribute__ ((packed)) _fat32;
  } __attribute__ ((packed)) fstype;
	/** Boot sign. */
  uint16_t		boot_sign;
} __attribute__ ((packed));

typedef struct msdos_boot_sector msdos_boot_sector;

#define fat32	fstype._fat32
#define oldfat	fstype._oldfat


/**
 * @brief FAT32 file system information
 *
 * The fat32_fsinfo structure has FAT32 file system information.
 */
struct fat32_fsinfo {
	/** Nothing as far as I can tell. */
  uint32_t reserved1;
	/** 0x61417272L. */
  uint32_t signature;
	/** Free cluster count.  -1 if unknown. */
  uint32_t free_clusters;
	/** Most recently allocated cluster. Unused under Linux. */
  uint32_t next_cluster;
	/** Reserved. */
  uint32_t reserved2[4];
};

typedef struct fat32_fsinfo fat32_fsinfo;

/**
 * @brief MS DOS directory entry information
 *
 * The msdos_dir_entry structure has MS DOS directory entry information.
 */
struct msdos_dir_entry {
	/** Name. */
	char name[8];
	/** Extension. */
	char ext[3];
	/** Attribute bits. */
	uint8_t attr;
	/** Case for base and extension. */
	uint8_t	lcase;
	/** Creation time, milliseconds. */
	uint8_t	ctime_ms;
	/** Creation time. */
	uint16_t ctime;
	/** Creation date. */
	uint16_t cdate;
	/** Last access date. */
	uint16_t adate;
	/** high 16 bits of first cl. (FAT32). */
	uint16_t starthi;
	/** Time. */
	uint16_t time;
	/** Date. */
	uint16_t date;
	/** First cluster. */
	uint16_t start;
	/** file size (in bytes). */
	uint32_t size;
} __attribute__ ((packed));

typedef struct msdos_dir_entry msdos_dir_entry;

/**
 * @brief Process directory entry
 *
 * The proc_dir_entry structure has process directory entry.
 */
/*
 * This is not completely implemented yet. The idea is to
 * create an in-memory tree (like the actual /proc filesystem
 * tree) of these proc_dir_entries, so that we can dynamically
 * add new files to /proc.
 *
 * The "next" pointer creates a linked list of one /proc directory,
 * while parent/subdir create the directory structure (every
 * /proc file has a parent, but "subdir" is NULL for all
 * non-directory entries).
 */
struct proc_dir_entry {
	/** Low inode number. */
	unsigned int low_ino;
	/** Mode. */
	unsigned int mode;
#if 0
	nlink_t nlink;
	kuid_t uid;
	kgid_t gid;
	loff_t size;
	const struct inode_operations *proc_iops;
	const struct file_operations *proc_fops;
#endif
	struct proc_dir_entry *next, *parent, *subdir;
	void *data;
	atomic count;         /* use count */
	atomic in_use;        /* number of callers into module in progress; */
#if 0
	/* negative -> it's going away RSN */
	struct completion *pde_unload_completion;
	struct list_head pde_openers;   /* who did ->open, but not ->release */
#endif
	/** proc_fops checks and pde_users bumps. */
	spinlock pde_unload_lock;
	/** Name length. */
	uint8_t namelen;
	/** Name. */
	char name[FS_NAME_LEN];
};

typedef struct proc_dir_entry proc_dir_entry;

#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_MCUBE_FS_H__ */
