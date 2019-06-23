/**
 * @file include/fs/fat.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_FS_FAT_H__
#define __MCUBE_FS_FAT_H__


#ifndef __ASSEMBLY__

/**
 * @struct bios_parameter_block
 * @brief The BIOS Parameter Block (in Volume Boot Record).
 */
struct bios_parameter_block {
  /**
   * Dummy jump instruction.
   */
  char jmp[3];

  /**
   * Original Equipment Manufacturing (OEM).
   */
  char oem[8];

  /**
   * Bytes per logical sector 0.
   */
  unsigned char bps0;

  /**
   * Bytes per logical sector 1.
   */
  unsigned char bps1;

  /**
   * Logical sectors per cluster.
   */
  unsigned char lspc;

  /**
   * Reserved logical sectors.
   */
  unsigned short rlsc;

  /**
   * Number of FATs.
   */
  unsigned char nf;

  /**
   * Number of root directory entries 0.
   */
  unsigned char nr0;

  /**
   * Number of root directory entries 1.
   */
  unsigned char nr1;

  /**
   * Total sectors.
   */
  unsigned short ts16;

  /**
   * Media descriptor.
   */
  unsigned char media;

  /**
   * Sectors per FAT.
   */
  unsigned short spf16;

  /**
   * Physical sectors per track.
   */
  unsigned short pspt;

  /**
   * Number of heads.
   */
  unsigned short nh;

  /**
   * Hidden sectors.
   */
  unsigned int hs;

  /**
   * Total sectors.
   */
  unsigned int ts32;

  /* FAT32 */
  /**
   * Logical sectors per FAT.
   */
  unsigned int lspf32;

  /**
   * Mirroring flags etc. and version.
   */
  unsigned int flg;

  /**
   * Root directory cluster.
   */
  unsigned int rc;

  /**
   * Location of FS information sector.
   */
  unsigned short lfsis;

  /**
   * Location of backup sector(s);
   */
  unsigned short lbs;

  /**
   * Reserved boot filename.
   */
  char rbf[12];

  /**
   * Physical drive number.
   */
  char pdn;

  /**
   * Flags etc.
   */
  char flg2;

  /**
   * Extended boot signature (0x29).
   */
  char ebs;

  /**
   * Volume serial number.
   */
  unsigned int vsn;

  /**
   * Volume label.
   */
  char vol[11];

  /**
   * File-system type.
   */
  char fst[8];
} __packed /** packed. */;

typedef struct bios_parameter_block bpb_t;


/**
 * @struct fatdir
 * @brief Directory entry structure.
 */
struct fatdir {
  /**
   * Name.
   */
  char name[8];

  /**
   * Extension.
   */
  char ext[3];

  /**
   * Attribute.
   */
  char attr[9];

  /**
   * Cluster number's higher 16-bits.
   */
  unsigned short ch;

  /**
   * Attribute 2.
   */
  unsigned int attr2;

  /**
   * Cluster number's lower 16-bits.
   */
  unsigned short cl;

  /**
   * File size.
   */
  unsigned int size;
} __packed /** packed. */;


/**
 * @typedef fatdir_t
 * @brief Typedef of @struct fatdir.
 */
typedef struct fatdir fatdir_t;


/**
 * @fn int fat_getpartition(void)
 * @brief get the starting LBA address of the first partition.
 * We know where our FAT file system starts, and read that volume's BIOS Parameter Block.
 *
 * @return Zero if success, and nonzero if failure.
 */
int fat_getpartition(void);

/**
 * @fn void fat_listdirectory(void)
 * @brief list root directory entries in a FAT file system.
 */
void fat_listdirectory(void);

/**
 * @fn unsigned int fat_getcluster(const char *fn)
 * @brief get cluster in FAT.
 *
 * @param fn Filename.
 * @return Starting cluster.
 */
unsigned int fat_getcluster(const char *fn);

/**
 * @fn char *fat_readfile(unsigned int cluster)
 * @brief read file in FAT.
 *
 * @param cluster Cluster.
 * @return File data.
 */
char *fat_readfile(unsigned int cluster);

#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_FS_FAT_H__ */

