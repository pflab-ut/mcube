/**
 * @file include/mcube/fs/fat.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_FS_FAT_H__
#define	__MCUBE_MCUBE_FS_FAT_H__


#ifndef __ASSEMBLY__

/* the BIOS Parameter Block (in Volume Boot Record) */
typedef struct {
  char            jmp[3];
  char            oem[8];
  unsigned char   bps0;
  unsigned char   bps1;
  unsigned char   spc;
  unsigned short  rsc;
  unsigned char   nf;
  unsigned char   nr0;
  unsigned char   nr1;
  unsigned short  ts16;
  unsigned char   media;
  unsigned short  spf16;
  unsigned short  spt;
  unsigned short  nh;
  unsigned int    hs;
  unsigned int    ts32;
  unsigned int    spf32;
  unsigned int    flg;
  unsigned int    rc;
  char            vol[6];
  char            fst[8];
  char            dmy[20];
  char            fst2[8];
} __attribute__((packed)) bpb_t;

/* directory entry structure */
typedef struct {
  char            name[8];
  char            ext[3];
  char            attr[9];
  unsigned short  ch;
  unsigned int    attr2;
  unsigned short  cl;
  unsigned int    size;
} __attribute__((packed)) fatdir_t;

int fat_getpartition(void);
void fat_listdirectory(void);

#endif /* !__ASSEMBLY__ */


#endif /*	__MCUBE_MCUBE_FS_FAT_H__ */

