/**
 * @file include/mcube/fs/storage.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_FS_STORAGE_H__
#define __MCUBE_MCUBE_FS_STORAGE_H__

#ifndef __ASSEMBLY__

enum IO_ERROR {
	IOE_SUCCESS = 0x0000,      /* succeeded                  */
	IOE_OUTRANGE,              /* parameter is out of range  */
	IOE_LAST,                  /* end of file                */
	IOE_FULL,                  /* storage is full            */
	IOE_NOFILESYS,             /* no file system is found    */
	
	/* SD card errors */
	IOE_FATAL   = 0x1100,      /* unknown, fatal error       */
	IOE_NOCARD  = 0x1200,      /* no card | no response      */
	IOE_TIMEOUT = 0x1300,      /* command timeout            */
	IOE_MMC     = 0x1400,      /* not SD card but MMC        */
	IOE_VDD     = 0x1500,      /* card does not support 3.3V */
	IOE_SECTOR  = 0x1600,      /* set sector size failed     */
	IOE_CMD     = 0x1700,      /* error bit in RESPONSE1     */
	IOE_READ    = 0x1800,      /* read sector failed         */
	IOE_WRITE   = 0x1900,      /* write sector failed        */
};

typedef enum IO_ERROR IO_ERROR;

enum FS_TYPE {
	FS_HDD = 0,
	FS_FAT = 1,
	FS_UNKNOWN =2
};

typedef enum FS_TYPE FS_TYPE;


/* open flags */
#define OPEN_RDONLY 0x01
#define OPEN_WRONLY 0x02
#define OPEN_RDWR 0x04
#define OPEN_APPEND 0x08
#define OPEN_CREAT 0x10
#define OPEN_EXCL 0x20

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_FS_STORAGE_H__ */
