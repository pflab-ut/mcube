/**
 * @file include/mcube/fs/fat.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_FS_FAT_H__
#define	__MCUBE_MCUBE_FS_FAT_H__

#ifndef __ASSEMBLY__


extern IO_ERROR write_sector(uint32_t addr, const uint8_t *buf);
extern IO_ERROR read_sector(uint32_t addr, uint8_t *buf);


/* ------------------------------------------------------------------------- */
/*  define FAT basic constants                                               */
/* ------------------------------------------------------------------------- */

#define FAT_NR_FILE 16


#define OPEN_CLOSE         0x00
#define OPEN_DIRECTORY     0x40



/* FAT cluster values */
#define FAT_CLST_EMPTY  0x0000  /* empty cluster      */
#define FAT_CLST_RESRV  0x0001  /* reserved cluster   */
#define FAT_CLST_BAD    0xfff7  /* bad cluster        */
#define FAT_CLST_LAST   0xffff  /* last cluster       */
#define FAT_CLST_ROOT   0x0000  /* */

/* directory entry */
#define FAT_DIRENT_SIZE  32
#define FAT_DIRENT_LAST  0x00
#define FAT_DIRENT_EMPTY 0xe5

/* file attribute */
#define ATTR_READ_ONLY 0x01
#define ATTR_HIDDEN   0x02
#define ATTR_SYSTEM   0x04
#define ATTR_VOLUME_ID 0x08
#define ATTR_DIRECTORY 0x10
#define ATTR_ARCHIVE  0x20
#define ATTR_LONGNAME (ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID)

#define FAT_CREATE_ERR  0x00000000
#define FAT_CREATE_NEW  0x00000001

#define FAT_CACHE_INVALID 0xffff

/* default time = 2000-01-01T00:00:00 */
#define FAT_TIME_DEFAULT 0x28000000

enum FAT_SEARCH {
  FAT_SEARCH_FILEFOUND,   /* file is found                              */
  FAT_SEARCH_DIRFOUND,    /* directory is found                         */
  FAT_SEARCH_CANCREATE,   /* parent directory is found, but file is not */
  FAT_SEARCH_INVALIDNM,   /* invalid filename                           */
  FAT_SEARCH_NOTFOUND,
};

typedef enum FAT_SEARCH FAT_SEARCH;

enum FAT_ALLOCATE {
  FAT_ALLOC_EXIST,
  FAT_ALLOC_NEW,
};

typedef enum FAT_ALLOCATE FAT_ALLOCATE;

enum FAT_FLUSH_MODE {
  FAT_FLUSH_CLOSE,
  FAT_FLUSH_TMP,
};

typedef enum FAT_FLUSH_MODE FAT_FLUSH_MODE;

/* ------------------------------------------------------------------------- */
/*  Structures for internal use                                              */
/* ------------------------------------------------------------------------- */

/**
 * @brief FAT directory entry result
 *
 * The FAT_DIRENT_RESULT structure has results of FAT direcotry entry.
 */
struct FAT_DIRENT_RESULT {
	/** Error code. */
  IO_ERROR err;
	/** Directory entry. */
  uint16_t dirent;
};

typedef struct FAT_DIRENT_RESULT FAT_DIRENT_RESULT;

/**
 * @brief FAT information
 *
 * The FAT_DIRENT_RESULT structure has FAT information.
 */
struct FAT_INFO {
	/** Time. */
  uint32_t tm_now;
	/** FAT sector address. */
  uint32_t addr_fat;
	/** Root directory entry address. */
  uint32_t addr_root;
	/** User data area address. */
  uint32_t addr_usr;
	/** Root directory entry num (should be 512). */
  uint16_t root_ent;
	/** FAT sectors. */
  uint16_t fat_sectors;
	/** FAT sector which has empty cluster. */
  uint16_t empty_fat;
	/** Number of of sectors/clusters. */
  uint8_t clst_size;
	/** FAT number. */
  uint8_t fat_num;
};

typedef struct FAT_INFO FAT_INFO;

/**
 * @brief FAT create information
 *
 * The FAT_CREATE_INFO structure has FAT create information.
 */
struct FAT_CREATE_INFO {
	/** Empty directory entry sector. */
  uint32_t empty_sec;
	/** Empty directory entry position. */
  uint16_t empty_pos;
	/** Parent directory's cluster. */
  uint16_t parent_clst;
  /* 8+3 name. */
  char fat_name[12];
};

typedef struct FAT_CREATE_INFO FAT_CREATE_INFO;

/**
 * @brief FAT file information
 *
 * The FAT_FILE structure has FAT file information.
 */
struct FAT_FILE {
	/** Buffer. */
  uint8_t  buf[SECTOR_SIZE];
	/** File size. */
  uint32_t size;
	/** Directory entry sector. */
  uint32_t ent_sec;
	/** Current read/write position. */
  uint32_t cur_pos;
	/* Directory entry position in sector. */
  uint16_t ent_pos;
	/** Current read/write cluster. */
  uint16_t cur_clst;
	/** Current read/write sector in cluster. */
  uint8_t cur_sec;
	/** Mode for open(). */
  uint8_t mode;
};

typedef struct FAT_FILE FAT_FILE;

/* ------------------------------------------------------------------------- */
/*  Macro functions                                                          */
/* ------------------------------------------------------------------------- */

#define FAT_ISDIR(mode) ((mode) & ATTR_DIRECTORY)
#define FAT_ISHIDDEN(mode) ((mode) & ATTR_HIDDEN)

#define fat_is_user_clst(clst) (FAT_CLST_RESRV < (clst) && (clst) < FAT_CLST_BAD)
#define fat_pos_in_sector(cur_pos) ((cur_pos) & (SECTOR_SIZE - 1))


#define FAT_NAME_MAX 12

/**
 * @brief FAT directory entry information
 *
 * The fat_dirent structure has FAT directory entry information.
 */
struct fat_dirent {
	/** Directory name. */
	char d_name[FAT_NAME_MAX+1];
};

typedef struct fat_dirent fat_dirent;

extern IO_ERROR fat_init(void);
extern void fat_term(void);
extern void fat_sync(void);
extern int fat_mkdir(const char *pathname);
extern int fat_open(const char *pathname, int flags);
extern int fat_close(int fd);
extern int fat_read(int fd, void *buf, int count);
extern int fat_write(int fd, const void *buf, int count);
extern int fat_readdir(int fd, struct fat_dirent *dirp);
extern int fat_opendir(const char *pathname);
extern int fat_closedir(int fd);
extern void fat_settime(uint16_t year, uint8_t month, uint8_t day,
												uint8_t hour, uint8_t min, uint8_t sec);

#endif /* !__ASSEMBLY__ */


#endif /*	__MCUBE_MCUBE_FS_FAT_H__ */

