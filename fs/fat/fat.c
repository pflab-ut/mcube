/**
 * @file fs/fat/fat.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>
/*
 * Copyright (C) 2018 bzt (bztsrc@github)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

/**
 * @var partitionlba
 * @brief Partition logical block address.
 */
static unsigned int partitionlba = 0;

/**
 * @var __end
 * @brief End of BSS.
 */
extern unsigned char __end;


int fat_getpartition(void)
{
  unsigned char *mbr = &__end;
  bpb_t *bpb = (bpb_t *) &__end;
  /* get the end of bss segment from linker */

  /* read the partitioning table */
  if (sd_readblock(0, &__end, 1)) {
    /* check magic */
    if (mbr[510] != 0x55 || mbr[511] != 0xaa) {
      printk("ERROR: Bad magic in MBR\n");
      return 0;
    }

#if 0

    /* check partition type */
    if (mbr[0x1c2] != 0xe /* FAT16 LBA */ && mbr[0x1c2] != 0xc /*FAT32 LBA*/) {
      printk("ERROR: Wrong partition type\n");
      return 0;
    }

#endif
    printk("MBR disk identifier: 0x%x\n",
           *((unsigned int *)((unsigned long) &__end + 0x1b8)));
    /* should be this, but compiler generates bad code... */
    // partitionlba = *((unsigned int*)((unsigned long) &__end + 0x1c6));
    partitionlba = mbr[0x1c6] + (mbr[0x1c7] << 8) + (mbr[0x1c8] << 16)
                   + (mbr[0x1c9] << 24);
    printk("\nFAT partition starts at: 0x%x\n", partitionlba);

    /* read the boot record */
    if (!sd_readblock(partitionlba, &__end, 1)) {
      printk("ERROR: Unable to read boot record\n");
      return 0;
    }

    /* check file system type. We don't use cluster numbers for that, but magic bytes */
    if (!(bpb->fst[0] == 'F' && bpb->fst[1] == 'A' && bpb->fst[2] == 'T')
        /*
        && !(bpb->fst2[0] == 'F' && bpb->fst2[1] == 'A' && bpb->fst2[2] == 'T')
        */
       ) {
      printk("ERROR: Unknown file system type\n");
      return 0;
    }

    printk("FAT type: ");
    /* if 16 bit sector per fat is zero, then it's a FAT32 */
    printk("%s\n", bpb->spf16 > 0 ? "FAT16" : "FAT32");
    return 1;
  }

  return 0;
}

void fat_listdirectory(void)
{
  bpb_t *bpb = (bpb_t *) &__end;
  fatdir_t *dir = (fatdir_t *) &__end;
  unsigned int root_sec, s;
  /* find the root directory's LBA */
  root_sec = ((bpb->spf16 ? bpb->spf16 : bpb->lspf32) * bpb->nf) + bpb->rlsc;
  s = (bpb->nr0 + (bpb->nr1 << 8));
  printk("FAT number of root diretory entries: 0x%x", s);
  s *= sizeof(fatdir_t);

  if (bpb->spf16 == 0) {
    /* adjust for FAT32 */
    root_sec += (bpb->rc - 2) * bpb->lspc;
  }

  /* add partition LBA */
  root_sec += partitionlba;
  printk("\nFAT root directory LBA: 0x%x\n", root_sec);

  /* load the root directory */
  if (sd_readblock(root_sec, (unsigned char *) &__end, s / 512 + 1)) {
    printk("\nAttrib Cluster  Size     Name\n");

    /* iterate on each entry and print out */
    for (; dir->name[0] != 0; dir++) {
      /* is it a valid entry? */
      if (dir->name[0] == 0xe5 || dir->attr[0] == 0xf) {
        continue;
      }

      /* decode attributes */
      putchar(dir->attr[0] & 1 ? 'R' : '.');  /* read-only */
      putchar(dir->attr[0] & 2 ? 'H' : '.');  /* hidden */
      putchar(dir->attr[0] & 4 ? 'S' : '.');  /* system */
      putchar(dir->attr[0] & 8 ? 'L' : '.');  /* volume label */
      putchar(dir->attr[0] & 16 ? 'D' : '.');  /* directory */
      putchar(dir->attr[0] & 32 ? 'A' : '.');  /* archive */
      putchar(' ');

      dir->attr[0] = 0;
      printk("0x%x 0x%x %s\n", ((unsigned int) dir->ch) << 16 | dir->cl, dir->size,
             dir->name);
    }
  } else {
    printk("ERROR: Unable to load root directory\n");
  }
}


/**
 * Find a file in root directory entries
 */
unsigned int fat_getcluster(const char *fn)
{
  bpb_t *bpb = (bpb_t *) &__end;
  fatdir_t *dir = (fatdir_t *)(&__end + 512);
  unsigned int root_sec, s;
  /* find the root directory's LBA */
  root_sec = ((bpb->spf16 ? bpb->spf16 : bpb->lspf32) * bpb->nf) + bpb->rlsc;
  s = (bpb->nr0 + (bpb->nr1 << 8)) * sizeof(fatdir_t);

  if (bpb->spf16 == 0) {
    /* adjust for FAT32 */
    root_sec += (bpb->rc - 2) * bpb->lspc;
  }

  /* add partition LBA */
  root_sec += partitionlba;

  /* load the root directory */
  if (sd_readblock(root_sec, (unsigned char *) dir, s / 512 + 1)) {
    /* iterate on each entry and check if it's the one we're looking for */
    for (; dir->name[0] != 0; dir++) {
      printk("dir->name = %s\n", dir->name);

      /* is it a valid entry? */
      if (dir->name[0] == 0xe5 || dir->attr[0] == 0xf) {
        continue;
      }

      /* filename match? */
      if (!__builtin_memcmp(dir->name, fn, 11)) {
        printk("FAT File %s starts at cluster: 0x%x\n",
               fn, ((unsigned int) dir->ch) << 16 | dir->cl);
        /* if so, return starting cluster */
        return ((unsigned int) dir->ch) << 16 | dir->cl;
      }
    }

    printk("ERROR: file not found\n");
  } else {
    printk("ERROR: Unable to load root directory\n");
  }

  return 0;
}

/**
 * Read a file into memory
 */
char *fat_readfile(unsigned int cluster)
{
  /* BIOS Parameter Block */
  bpb_t *bpb = (bpb_t *) &__end;
  /* File allocation tables. We choose between FAT16 and FAT32 dynamically */
  unsigned int *fat32 = (unsigned int *)(&__end + bpb->rlsc * 512);
  unsigned short *fat16 = (unsigned short *) fat32;
  /* Data pointers */
  unsigned int data_sec, s;
  unsigned char *data, *ptr;
  /* find the LBA of the first data sector */
  data_sec = ((bpb->spf16 ? bpb->spf16 : bpb->lspf32) * bpb->nf) + bpb->rlsc;
  s = (bpb->nr0 + (bpb->nr1 << 8)) * sizeof(fatdir_t);

  if (bpb->spf16 > 0) {
    /* adjust for FAT16 */
    data_sec += (s + 511) >> 9;
  }

  /* add partition LBA */
  data_sec += partitionlba;
  /* dump important properties */
  printk("FAT Bytes per Sector: 0x%x\n", bpb->bps0 + (bpb->bps1 << 8));
  printk("FAT Sectors per Cluster: 0x%x\n", bpb->lspc);
  printk("FAT Number of FAT: 0x%x\n", bpb->nf);
  printk("FAT Sectors per FAT: 0x%x\n", bpb->spf16 ? bpb->spf16 : bpb->lspf32);
  printk("FAT Reserved Sectors Count: 0x%x\n", bpb->rlsc);
  printk("FAT First data sector: 0x%x\n", data_sec);
  /* load FAT table */
  s = sd_readblock(partitionlba + 1, (unsigned char *) &__end + 512,
                   (bpb->spf16 ? bpb->spf16 : bpb->lspf32) + bpb->rlsc);
  /* end of FAT in memory */
  data = ptr = &__end + 512 + s;

  /* iterate on cluster chain */
  while (cluster > 1 && cluster < 0xfff8) {
    /* load all sectors in a cluster */
    sd_readblock((cluster - 2) * bpb->lspc + data_sec, ptr, bpb->lspc);
    /* move pointer, sector per cluster * bytes per sector */
    ptr += bpb->lspc * (bpb->bps0 + (bpb->bps1 << 8));
    /* get the next cluster in chain */
    cluster = bpb->spf16 > 0 ? fat16[cluster] : fat32[cluster];
  }

  return (char *) data;
}
