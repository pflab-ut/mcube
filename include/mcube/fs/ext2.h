/**
 * @file include/mcube/fs/ext2.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_FS_EXT2_H__
#define __MCUBE_MCUBE_FS_EXT2_H__


#ifndef __ASSEMBLY__

/*
 * In-memory Super Block - Global State for our FS code
 *
 * NOTE! The inodes repository hash lock could be fine-grained
 * by having a lock on each hash collision linked-list instead.
 */
struct isb {
  union super_block  *sb;    /* On-disk Superblock */
  struct group_descriptor *bgd;    /* On-disk Group Desc Table */
  char      *buf;    /* Ramdisk buffer */
  uint64_t    block_size;  /* 1, 2, or 4K */
  uint64_t    frag_size;  /* 1, 2, or 4K */
  uint64_t    blockgroups_count;
  uint64_t    last_blockgroup;
  spinlock_t    inode_allocation_lock;
  spinlock_t    block_allocation_lock;
  struct hash    *inodes_hash;
  spinlock_t    inodes_hash_lock;
};

extern struct isb isb;

#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_MCUBE_FS_EXT2_H__ */

