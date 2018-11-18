/**
 * @file fs/fat.c
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


static unsigned int partitionlba = 0;


/**
 * Get the starting LBA address of the first partition
 * so that we know where our FAT file system starts, and
 * read that volume's BIOS Parameter Block
 */
int fat_getpartition(void)
{
  extern unsigned char __end;
  unsigned char *mbr = &__end;
  bpb_t *bpb=(bpb_t*) &__end;
  // get the end of bss segment from linker

  // read the partitioning table
  if (sd_readblock(0, &__end, 1)) {
    // check magic
    if (mbr[510] != 0x55 || mbr[511] != 0xaa) {
      printk("ERROR: Bad magic in MBR\n");
      return 0;
    }
#if 0
    // check partition type
    if (mbr[0x1c2] != 0xe /* FAT16 LBA */ && mbr[0x1c2] != 0xc /*FAT32 LBA*/) {
      printk("ERROR: Wrong partition type\n");
      return 0;
    }
#endif
    printk("MBR disk identifier: 0x%x\n",
           *((unsigned int *)((unsigned long) &__end + 0x1b8)));
    // should be this, but compiler generates bad code...
    //partitionlba = *((unsigned int*)((unsigned long) &__end + 0x1c6));
    partitionlba = mbr[0x1c6] + (mbr[0x1c7] << 8) + (mbr[0x1c8] << 16) + (mbr[0x1c9] << 24);
    printk("\nFAT partition starts at: 0x%x\n", partitionlba);
    // read the boot record
    if (!sd_readblock(partitionlba, &__end, 1)) {
      printk("ERROR: Unable to read boot record\n");
      return 0;
    }
    // check file system type. We don't use cluster numbers for that, but magic bytes
    if (!(bpb->fst[0] == 'F' && bpb->fst[1] == 'A' && bpb->fst[2] == 'T')
        && !(bpb->fst2[0] == 'F' && bpb->fst2[1] == 'A' && bpb->fst2[2] == 'T')) {
      printk("ERROR: Unknown file system type\n");
      return 0;
    }
    printk("FAT type: ");
    // if 16 bit sector per fat is zero, then it's a FAT32
    printk("%s\n", bpb->spf16 > 0 ? "FAT16" : "FAT32");
    return 1;
  }
  return 0;
}

/**
 * List root directory entries in a FAT file system
 */
void fat_listdirectory(void)
{
  extern unsigned char __end;
  bpb_t *bpb = (bpb_t *) &__end;
  fatdir_t *dir = (fatdir_t *) &__end;
  unsigned int root_sec, s;
  // find the root directory's LBA
  root_sec = ((bpb->spf16 ? bpb->spf16 : bpb->spf32) * bpb->nf) + bpb->rsc;
  s = (bpb->nr0 + (bpb->nr1 << 8));
  printk("FAT number of root diretory entries: 0x%x", s);
  s *= sizeof(fatdir_t);
  if (bpb->spf16 == 0) {
    /* adjust for FAT32 */
    root_sec += (bpb->rc - 2) * bpb->spc;
  }
  /* add partition LBA */
  root_sec += partitionlba;
  printk("\nFAT root directory LBA: 0x%x\n", root_sec);
  /* load the root directory */
  if (sd_readblock(root_sec, (unsigned char*) &__end, s / 512 + 1)) {
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

      dir->attr[0]=0;
      printk("0x%x 0x%x %s\n", ((unsigned int) dir->ch) << 16 | dir->cl, dir->size, dir->name);
    }
  } else {
    printk("ERROR: Unable to load root directory\n");
  }
}
