/**
 * @file include/fs/ext2.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_FS_EXT2_H__
#define __MCUBE_FS_EXT2_H__


#ifndef __ASSEMBLY__

/*
 * The Second Extended File System
 *
 * Copyright (C) 2012-2013 Ahmed S. Darwish <darwish.07@gmail.com>
 *
 * REFERENCES:
 *
 *  - "The Design of the UNIX Operating System", Maurice J. Bach,
 *    Ch. 4: 'Internal Representation of Files', 1986
 *  - "UNIX Implementation", Ken Thompson, Bell Journal, 1978
 *  - "The Second Extended File System - Internal Layout", David
 *    Poirier et. al, 2001-2011
 *  - Finally, my collection of _primary sources_ FS papers with
 *    their study notes at: http://www.webcitation.org/68IbFbOGr
 */

/**
 * @enum ext2
 * @brief EXT2 information.
 */
enum ext2 {
  /**
   * Superblock size.
   */
  EXT2_SUPERBLOCK_SIZE = 1024,

  /**
   * EF'S'.
   */
  EXT2_SUPERBLOCK_MAGIC = 0xef53,

  /**
   * 60-KB.
   */
  EXT2_MIN_FS_SIZE = (60 * 1024),

  // Below offsets are out of disk start
  /**
   * First Kilobyte.
   */
  EXT2_SUPERBLOCK_OFFSET = 1024,

  /**
   * Second Kilobyte.
   */
  EXT2_GROUP_DESC_OFFSET = 2048,

  /**
   * A NULL suffix __may__ exist.
   */
  EXT2_LABEL_LEN    = 16,

  /**
   * Max filenme length, no NULL.
   */
  EXT2_FILENAME_LEN  = 255,

  /**
   * Path when FS was Last mnted.
   */
  EXT2_LAST_MNT_LEN = 64,

  /**
   * Max supported FS block size.
   */
  EXT2_MAX_BLOCK_LEN = 4096,

  /**
   * 4 ino, 2 rec_len, 2 namelen.
   */
  EXT2_DIR_ENTRY_MIN_LEN = 8,

  /**
   * alignmnt for entries, bytes.
   */
  EXT2_DIR_ENTRY_ALIGN = 4,

  /**
   * Data blocks mapped by inode.
   */
  EXT2_INO_NR_BLOCKS = 15,

  /**
   * Nr of inode 'direct' blocks.
   */
  EXT2_INO_NR_DIRECT_BLKS = 12,

  /**
   * Inode's indirect block.
   */
  EXT2_INO_INDIRECT = 12,

  /**
   * Inode's double indirect.
   */
  EXT2_INO_DOUBLEIN = 13,

  /**
   * Inode's triple indirect.
   */
  EXT2_INO_TRIPLEIN = 14,
};

/**
 * @enum ext2_revision
 * @brief Superblock "revision level".
 */
enum ext2_revision {
  /**
   * Unsupported.
   */
  EXT2_GOOD_OLD_REVISION = 0,

  /**
   * Dynamic revision.
   */
  EXT2_DYNAMIC_REVISION = 1,
};

/**
 * @enum ext2_fs
 * @brief Superblock "state" field.
 */
enum ext2_fs {
  /**
   * Unmounted cleanly.
   */
  EXT2_VALID_FS = 1,

  /**
   * Errors detected.
   */
  EXT2_ERROR_FS = 2,
};

/**
 * @enum ext2_inode
 * @brief Reserved inode numbers.
 */
enum ext2_inode {
  /**
   * Bad blocks.
   */
  EXT2_BAD_INODE = 1,

  /**
   * Inode for the root '/'.
   */
  EXT2_ROOT_INODE = 2,

  /**
   * ACL Index (deprecated).
   */
  EXT2_ACL_IDX_INODE = 3,

  /**
   * ACL Data (deprecated).
   */
  EXT2_ACL_DATA_INODE  = 4,
  /**
   * Bootloader.
   */
  EXT2_BOOTLOADER_INODE  = 5,

  /**
   * Undelete directory.
   */
  EXT2_UNDELETE_DIR_INODE = 6,
};

/**
 * @enum ext2_inode_flags
 * @brief Inode flags - only the ones we recognize.
 */
enum ext2_inode_flags {
  /**
   * Immutable file.
   */
  EXT2_INO_IMMUTABLE_FL = 0x00000010,

  /**
   * Hash-indexed directory.
   */
  EXT2_INO_DIR_INDEX_FL = 0x00001000,

  /**
   * File data in extents.
   */
  EXT2_INO_EXTENT_FL = 0x00080000,
};

/**
 * @enum file_type
 * @brief Directory entries 1-byte File-Type field.
 */
enum file_type {
  /**
   * Unknown.
   */
  EXT2_FT_UNKNOWN = 0,

  /**
   * Regular file.
   */
  EXT2_FT_REG_FILE = 1,

  /**
   * Directory.
   */
  EXT2_FT_DIR = 2,

  /**
   * Character device.
   */
  EXT2_FT_CHRDEV = 3,

  /**
   * Block device.
   */
  EXT2_FT_BLKDEV = 4,

  /**
   * FIFO.
   */
  EXT2_FT_FIFO = 5,

  /**
   * Socket.
   */
  EXT2_FT_SOCK = 6,

  /**
   * Symbolic link.
   */
  EXT2_FT_SYMLINK = 7,

  /**
   * Maximum.
   */
  EXT2_FT_MAX
};

/**
 * @fn static inline mode_t dir_entry_type_to_inode_mode(enum file_type type)
 * @brief Directory entry type to inode mode.
 *
 * @param type Directory entry type.
 * @return Inode mode.
 */
static inline mode_t dir_entry_type_to_inode_mode(enum file_type type)
{
  switch (type) {
  case EXT2_FT_REG_FILE:
    return S_IFREG;

  case EXT2_FT_DIR:
    return S_IFDIR;

  case EXT2_FT_CHRDEV:
    return S_IFCHR;

  case EXT2_FT_BLKDEV:
    return S_IFBLK;

  case EXT2_FT_FIFO:
    return S_IFIFO;

  case EXT2_FT_SOCK:
    return S_IFSOCK;

  case EXT2_FT_SYMLINK:
    return S_IFLNK;

  default:
    assert(false);
  }
}

/**
 * @fn static inline enum file_type inode_mode_to_dir_entry_type(mode_t mode)
 * @brief Inode mode to directory entry type.
 *
 * @param mode Mode.
 * @return Directory entry type.
 */
static inline enum file_type inode_mode_to_dir_entry_type(mode_t mode)
{
  switch (mode & S_IFMT) {
  case S_IFREG:
        return EXT2_FT_REG_FILE;

  case S_IFDIR:
    return EXT2_FT_DIR;

  case S_IFCHR:
    return EXT2_FT_CHRDEV;

  case S_IFBLK:
    return EXT2_FT_BLKDEV;

  case S_IFIFO:
    return EXT2_FT_FIFO;

  case S_IFSOCK:
    return EXT2_FT_SOCK;

  case S_IFLNK:
    return EXT2_FT_SYMLINK;

  default:
    assert(false);
  }
}

/**
 * @union super_block
 * @brief On-disk Superblock Format.
 */
union super_block {
  struct {
    /**
     * Total number of inodes, used and free, in the FS.
     */
    uint32_t inodes_count;

    /**
     * Total number of blocks, used, free, and reserved.
     */
    uint32_t blocks_count;

    /**
     * Total number of blocks reserved for the superuser,
     * usable for emergency cases when the disk is full.
     */
    uint32_t r_blocks_count;

    /**
     * Total number of free blocks, including reserved.
     */
    uint32_t free_blocks_count;

    /**
     * Total number of free inodes; sum of b-groups free inodes.
     */
    uint32_t free_inodes_count;

    /**
     * First block holding data, i.e. non-bootstrap code.
     * NOTE! 1st block for Block Group #0 = first_data_block.
     */
    uint32_t first_data_block;

    /**
     * Block size = 1024 << size_in_log (1, 2, 4, 8, .. KB).
     */
    uint32_t log_block_size;

    /**
     * Fragments are not support, their size = block size.
     */
    uint32_t log_fragment_size;

    /**
     * Number of blocks per Block Group.
     */
    uint32_t blocks_per_group;

    /**
     * Number of fragments per Block Group.
     */
    uint32_t frags_per_group;

    /**
     * Number of inodes per Block Group.
     */
    uint32_t inodes_per_group;

    /**
     * Last time the FS was mounted, UNIX format.
     */
    uint32_t mount_time;

    /**
     * Last write access to the FS.
     */
    uint32_t write_time;

    /**
     * Number of mounts since the last fsck (FS check).
     */
    uint16_t mount_count;

    /**
     * Max number of mounts before an fsck is performed.
     */
    uint16_t max_mount_count;

    /**
     * Magic value identifying the FS as ext2.
     */
    uint16_t magic_signature;

    /**
     * mounted cleanly (VALID_FS) or errors detected (ERROR).
     */
    uint16_t state;

    /**
     * What the FS driver should do when detecting errors.
     */
    uint16_t errors_behavior;

    /**
     * Minor part of the revision level.
     */
    uint16_t minor_revision;

    /**
     * Last time of fsck.
     */
    uint32_t last_check;

    /**
     * Maximum Unix time interval allowed between fscks.
     */
    uint32_t check_interval;

    /**
     * OS that created the file system.
     */
    uint32_t creator_os;

    /**
     * Revision 0 (EXT2_GOOD_OLD) or 1 (EXT2_DYNAMIC).
     */
    uint32_t revision_level;

    /**
     * Default user ID for reserved blocks.
     */
    uint16_t reserved_uid;

    /**
     * Default group ID for reserved blocks.
     */
    uint16_t reserved_gid;

    /**
     * Index to first useable (non-reserved) inode.
     */
    uint32_t first_inode;

    /**
     * Size of the On-Disk inode structure.
     */
    uint16_t inode_size;

    /**
     * Block Group number hosting this superblock.
     */
    uint16_t block_group;

    /**
     * Bitmask for the 'compatible' feature set.
     * If FS driver doesn't know any of these features, it can still mount
     * and use the disk _without_ risk of damaging data.
     */
    uint32_t features_compat;

    /**
     * Bitmask for the 'incompatible' feature set.
     * If driver faced any unknown bit set, it should NOT mount it.
     */
    uint32_t features_incompat;

    /**
     * Any of these features unknown? mount it as readonly!
     */
    uint32_t features_ro_compat;

    /**
     * Unique 128-bit UUID for the volume
     */
    uint8_t UUID[16];

    /**
     * 16-byte, NULL-terminated, ISO-Latin-1 volume name.
     */
    char volume_label[16];

    /**
     * Obsolete, directory path wher the FS was last mounted.
     */
    char last_mounted[64];

    /**
     * LZV1, GZIP, BZIP, LZO type of compression used.
     */
    uint32_t compression_bitmap;

    /**
     * Number of blocks to try to preallocate (COMPAT_PRERALLOC).
     */
    uint8_t prealloc_blocks;

    /**
     * Number of blocks to try to preallocate for directories
     */
    uint8_t prealloc_dir_blocks;
  } __packed /** packed. */;

  /**
   * Raw data.
   */
  uint8_t raw[EXT2_SUPERBLOCK_SIZE];
};

/**
 * @struct group_descriptor
 * @brief A Block Group descriptor.
 */
struct group_descriptor {
  /**
   * 32-bit Block ID of the group 'blocks bitmap'.
   */
  uint32_t block_bitmap;

  /**
   * 32-bit Block ID of the group 'inodes bitmap'.
   */
  uint32_t inode_bitmap;

  /**
   * 32-bit Block ID for the inode's table first block.
   */
  uint32_t inode_table;

  /**
   * Number of free blocks in this group.
   */
  uint16_t free_blocks_count;

  /**
   * Number of free inodes in this group.
   */
  uint16_t free_inodes_count;

  /**
   * Number of inodes allocated for directories in this group
   */
  uint16_t used_dirs_count;

  /**
   * Reserved.
   */
  uint16_t reserved[7];
} __packed;

/**
 * @struct inode
 * @brief In-core inode image.
 *
 * Every used 'on-disk' inode is buffered on RAM as an 'in-core' inode.
 * Such buffered images are tracked by a hash table inode repository.
 * This is taken from the classical SVR2 implementation, but with some
 * SMP tweaks.
 *
 * NOTE! Before accessing this inode structure, check our notes on
 * buffered inode images and SMP locking at the top of ``ext2.c''.
 *
 * NOTE!-2 Remember addinig an initialization line for any new members.
 */
struct inode {
  /* Below fields are entirely in RAM, no equivalent exist on disk: */

  /**
   * Inode number (MUST be the first element).
   */
  uint64_t inum;

  /**
   * List node for the inodes hash table collision list.
   */
  struct list_node node;

  /**
   * Reference count (CHECK `ext2.c' notes before usage).
   */
  int refcount;

  /**
   * For protecting internal inode fields (`ext2.c').
   */
  spinlock_t lock;

  /**
   * Does the buffered inode image now differ from disk?
   */
  bool dirty;

  /**
   * Flag to delete on last use.
   */
  bool delete_on_last_use;

  /* Below fields are mirrored from the Ext2 On-Disk inode image: */

  /**
   * File type and access rights.
   */
  uint16_t mode;

  /**
   * User ID associated with the file.
   */
  uint16_t uid;

  /**
   * Lower 32-bits of file size, in bytes.
   */
  uint32_t size_low;

  /**
   * Last time this inode was accessed; UNIX format.
   */
  uint32_t atime;

  /**
   * Time when this inode was created; UNIX format.
   */
  uint32_t ctime;

  /**
   * Last time this inode was modified; UNIX format.
   */
  uint32_t mtime;

  /**
   * Time when this inode was deleted (if so); UNIX.
   */
  uint32_t dtime;

  /**
   * POSIX group ID having access to this file (low 16bits).
   */
  uint16_t gid_low;

  /**
   * How many times this inode is linked to by dir entries.
   */
  uint16_t links_count;

  /**
   * Total number of 512-byte blocks reserved to contain the
   * data of this inode, regardless of whether these blocks
   * are used or not.
   */
  uint32_t i512_blocks;

  /**
   * How should the driver behave when dealing with file's
   * data; SECRM_FL, UNRM_FL, APPEND_FL, IMMUTABLE_FL, etc.
   */
  uint32_t flags;

  /**
   * OS dependent.
   */
  uint32_t os_dependent;

  /**
   * Pointers to data blocks; blocks 0-11 are direct mapping,
   * 12 is indirect, 13 is double indirect, 14 is triple.
   * 0 value indicates that such part of file isn't mapped.
   */
  uint32_t blocks[EXT2_INO_NR_BLOCKS];

  /**
   * File version, only used by NFS.
   */
  uint32_t generation;

  /**
   * Block number containing file's extended attributes.
   */
  uint32_t file_acl;

  /**
   * Higher 32-bits of file size, in bytes.
   */
  uint32_t size_high;

  /**
   * Obsolete.
   */
  uint32_t obsolete;

  /**
   * Higher 32-bits of total number of 512-byte blocks
   */
  uint16_t blocks_count_high;

  /**
   * File's access control list with high bits.
   */
  uint16_t file_acl_high;

  /**
   * User ID with high bits.
   */
  uint16_t uid_high;

  /**
   * Group ID with high bits.
   */
  uint16_t gid_high;

  /**
   * Originally for other, non-Linux, private data.
   */
  uint32_t reserved;
} __packed;

/**
 * @fn static inline void inode_init(struct inode *inode, uint64_t inum)
 * @brief initialize inode.
 *
 * @param inode Inode.
 * @param inum Inode number.
 */
static inline void inode_init(struct inode *inode, uint64_t inum)
{
  inode->inum = inum;
  list_init(&inode->node);
  inode->refcount = 1;
  init_spin(&inode->lock);
  inode->dirty = false;
  inode->delete_on_last_use = false;
}

/**
 * @fn static inline void *dino_off(struct inode *inode)
 * @brief Offset of mode of inode.
 *
 * @param inode Inode.
 * @return Address of mode of inode.
 */
static inline void *dino_off(struct inode *inode)
{
  return (char *) inode + offsetof(struct inode, mode);
}

/**
 * @fn static inline uint64_t dino_len(void)
 * @brief subtract size of inode from offset of mode of inode.
 *
 * @return Subtract size of inode from offset of mode of inode.
 */
static inline uint64_t dino_len(void)
{
  return sizeof(struct inode) - offsetof(struct inode, mode);
}

/**
 * @fn struct inode *inode_get(uint64_t inum)
 * @brief get inode.
 *
 * @param inum Inode number.
 * @return Inode.
 */
struct inode *inode_get(uint64_t inum);

/**
 * @fn void inode_put(struct inode *inode)
 * @brief put inode.
 *
 * @param inode Inode number.
 */
void inode_put(struct inode *inode);

/**
 * @def ___test_inum(inum, INODE_MODE_TEST)
 * @brief test inode.
 *
 * @param inum Inode number.
 * @param INODE_MODE_TEST Inode mode test.
 */
#define ___test_inum(inum, INODE_MODE_TEST) ({                  \
      struct inode *dir;                                        \
      uint16_t mode;                                            \
      dir = inode_get(inum); mode = dir->mode; inode_put(dir);  \
      INODE_MODE_TEST(mode);                                    \
    })

/**
 * @fn static inline bool is_dir(uint64_t inum)
 * @brief Is given inode number directory?
 *
 * @param inum Inode number.
 * @return True if inode number is directory.
 */
static inline bool is_dir(uint64_t inum)
{
  return ___test_inum(inum, S_ISDIR);
}

/**
 * @fn static inline bool is_symlink(uint64_t inum)
 * @brief Is given inode number symbolic link?
 *
 * @param inum Inode number.
 * @return True if inode number is symbolic link.
 */
static inline bool is_symlink(uint64_t inum)
{
  return ___test_inum(inum, S_ISLNK);
}

/**
 * @enum indirection_level
 * @brief Inode data blocks level of indirection.
 */
enum indirection_level {
  /**
   * A plain block.
   */
  ZERO_INDIR = 0,

  /**
   * An indirect data block.
   */
  SINGLE_INDIR = 1,

  /**
   * A double-indirect data block.
   */
  DOUBLE_INDIR = 2,

  /**
   * A triple-indirect data block.
   */
  TRIPLE_INDIR = 3,

  /**
   * Maximum indirection level.
   */
  INDIRECTION_LEVEL_MAX,
};

/**
 * @struct dir_entry
 * @brief Directory entry format.
 *
 * A directory is a file holding variable-sized records;
 * each record represents a file contained in that folder.
 */
struct dir_entry {
  /**
   * Inode # of the file entry, 0 indicates a non-used entry.
   */
  uint32_t inode_num;

  /**
   * Unsigned displacement to the next directory entry.
   */
  uint16_t record_len;

  /**
   * File name len must never be larger than record_len - 8.
   */
  uint8_t filename_len;

  /**
   * Must match the type defined in the inode entry above.
   */
  uint8_t file_type;

  /**
   * Name of entry, in ISO-LATIN-1 character set.
   */
  char filename[EXT2_FILENAME_LEN];
} __packed /** packed. */;

/**
 * @fn uint64_t file_read(struct inode *inode, char *buf, uint64_t offset, uint64_t len)
 * @brief read given file.
 * NOTE! If this code was later modified so that errors are returned,
 * remember to check such errors in all of the callers.
 *
 * @param inode File's inode, which will map us to the data blocks
 * @param buf Buffer to put the read data into
 * @param offset File's offset
 * @param len Number of bytes to read, starting from file @a offset
 * @return Number of bytes read, or zero (out of boundary @a offset)
 */
uint64_t file_read(struct inode *inode, char *buf, uint64_t offset,
                   uint64_t len);

/**
 * @fn int64_t file_write(struct inode *inode, char *buf, uint64_t offset,
 *                        uint64_t len)
 * @brief write buffer to given file.
 *
 * @param inode File's inode, which will map us to the data blocks.
 * @param buf Buffer of the data to be written.
 * @param offset File's offset.
 * @param len Number of bytes to write.
 * @return Number of bytes actually written, or an errno.
 */
int64_t file_write(struct inode *inode, char *buf, uint64_t offset,
                   uint64_t len);

/**
 * @fn int64_t ext2_new_dir_entry(struct inode *dir, struct inode *entry_ino,
 *                                const char *name, enum file_type type)
 * @brief create a new file entry in given parent directory.
 * Check file_new() for parameters documentation.
 *
 * NOTE! This increments the entry's destination inode links count
 * NOTE! @a dir may be equal to @a entry_ino if we are adding a '.' entry
 * to a new folder.
 *
 * @param dir Directory.
 * @param entry_ino Allocated inode number for the new file.
 * @param name Name.
 * @param type Type.
 * @return Zero if success, and nonzero if failure.
 */
int64_t ext2_new_dir_entry(struct inode *dir, struct inode *entry_ino,
                           const char *name, enum file_type type);

/**
 * @fn int64_t file_new(struct inode *dir, const char *name, enum file_type type)
 * @brief create and initialize a new file in given directory.
 *
 * @param dir Parent directory where the file will be located.
 * @param name File's name.
 * @param type File's type, in Ext2 dir entry type format.
 * @return New file's inode number, or an errno.
 */
int64_t file_new(struct inode *dir, const char *name, enum file_type type);

/**
 * @fn int file_delete(struct inode *parent, const char *name)
 * @brief delete file.
 * NOTE! If given file's inode was linked by several hard links, then
 * instead of the default behaviour of deleting the file's inode and
 * truncating its entire data, only the file's directory entry will
 * get deleted.
 *
 * @param parent Parent directory where the file will be located.
 * @param name File's name.
 * @return zero if success, and nonzero if failure.
 */
int file_delete(struct inode *parent, const char *name);

/**
 * @fn void file_truncate(struct inode *inode)
 * @brief truncate given file to zero (0) bytes.
 * @param inode File's inode, which will map us to the data blocks.
 */
void file_truncate(struct inode *inode);

/**
 * @fn int64_t name_i(const char *path)
 * @brief find the inode of given file path.
 *
 * @param path Absolute, hierarchial, UNIX format, file path.
 * @return Inode num, or -ENOENT, -ENOTDIR, -ENAMETOOLONG.
 */
int64_t name_i(const char *path);

/**
 * @enum block_op
 * @brief Block operations.
 */
enum block_op {
  /**
   * Read.
   */
  BLOCK_READ,

  /**
   * Write.
   */
  BLOCK_WRTE,
};

/**
 * @struct path_translation
 * @brief Pahtname translation - Used for testing ext2 code.
 */
struct path_translation {
  /**
   * Absolute, hierarchial, UNIX format, file path.
   */
  const char *path;

  /**
   * Inode number found using name_i() on _each_ subcomponent.
   */
  uint64_t relative_inum;

  /**
   * Inode# found using name_i() on the path as a whole.
   */
  uint64_t absolute_inum;

  /**
   * File descriptor returned by open(@a path, O_RDONLY).
   */
  int fd;
};

/*
 * Globally export some internal methods if the test-cases
 * driver was enabled.
 */

/**
 * @fn void block_read(uint64_t block, char *buf, uint blk_offset, uint len)
 * @brief read given disk block into buffer.
 *
 * @param block Disk block to read from.
 * @param buf Buffer to put the read data into.
 * @param blk_offset Offset within the block to start reading from.
 * @param len Number of bytes to read, starting from @a blk_offset.
 */
void block_read(uint64_t block, char *buf, uint blk_offset, uint len);

/**
 * @fn void block_write(uint64_t block, char *buf, uint blk_offset, uint len)
 * @brief write given buffer into disk block.
 *
 * @param block Disk block to write to.
 * @param buf Buffer of data to be written.
 * @param blk_offset Offset within the block to start writing to.
 * @param len Number of bytes to write, starting from @a blk_offset.
 */
void block_write(uint64_t block, char *buf, uint blk_offset, uint len);

/**
 * @fn void block_dealloc(uint block)
 * @brief mark given block as free on-disk.
 * All the necessary counters are updated in the process
 *
 * @param block Block number to deallocate.
 */
void block_dealloc(uint block);

/**
 * @fn struct inode *inode_alloc(enum file_type type)
 * @brief assign a free disk inode to newly created files.
 * NOTE! There are obviously more SMP-friendly ways for doing this
 * than just grabbing a global lock reserved for inodes allocation.
 * An "optimistic locking" scheme would access the inode bitmap
 * locklessly, but set the inode's "used/free" bit using an atomic
 * test_bit_and_set method.  If another thread got that inode bef-
 * ore us, we continue searching the bitmap for yet another inode!
 *
 * @param type Type of file this inode is allocated for.
 * @return Locked in-core copy of allocated disk inode, or NULL.
 */
struct inode *inode_alloc(enum file_type type);

/**
 * @fn void inode_mark_delete(struct inode *inode)
 * @brief mark given inode for deletion.
 *
 * @param inode Inode.
 */
void inode_mark_delete(struct inode *inode);

/**
 * @fn uint64_t block_alloc(void)
 * @brief allocate a free data block from disk.
 * @return Block number, or 0 if no free blocks exist.
 */
uint64_t block_alloc(void);

/**
 * @fn bool dir_entry_valid(struct inode *dir, struct dir_entry *dentry,
 *                          uint64_t offset, uint64_t len)
 * @brief check the validity of given directory entry.
 * FIXME: Assure entry's type == its destination inode mode.
 *
 * @param dir inode of directory file.
 * @param dentry Directory entry.
 * @param offset Relative to the directory-file start.
 * @param len Number of bytes we were able read before reaching EOF.
 * @return True if directory entry is valid.
 */
bool dir_entry_valid(struct inode *dir, struct dir_entry *dentry,
                     uint64_t offset, uint64_t len);

/**
 * @fn int64_t find_dir_entry(struct inode *inode, const char *name, uint name_len,
 *                            struct dir_entry **dentry, int64_t *offset)
 * Search given directory for an entry with file @name.  Return such
 * entry if found, or return an errno.
 *
 * @param dir Inode for the directory to be searched.
 * @param name Wanted file name, which may not be NULL-terminated.
 * @param name_len File name len, to avoid requiring the NULL postfix.
 * @param dentry Return val, the found directory entry (if any).
 * @param offset Return val, offset of found dentry wrt to dir file.
 * @return Inode number of file, or an errorno.
 */
int64_t find_dir_entry(struct inode *dir, const char *name, uint name_len,
                       struct dir_entry **dentry, int64_t *offset);

/*
 * Dump file system On-Disk structures;  useful for testing.
 */

/**
 * @fn void ext2_debug_init(struct buffer_dumper *g_dumper)
 * @brief initialize ext2 debug.
 *
 * @param g_dumper Dumper.
 */
void ext2_debug_init(struct buffer_dumper *g_dumper);

/**
 * @fn void superblock_dump(union super_block *sb)
 * @brief dump superblock.
 *
 * @param sb Super block.
 */
void superblock_dump(union super_block *sb);

/**
 * @fn void blockgroup_dump(int bg_idx, struct group_descriptor *bgd,
 *                          uint32_t firstb, uint32_t lastb, uint64_t inodetbl_blocks)
 * @brief dump block group.
 *
 * @param bg_idx Block group index.
 * @param bgd Block group descriptor.
 * @param firstb First block.
 * @param lastb Last block.
 * @param inodetbl_blocks Inode table blocks.
 */
void blockgroup_dump(int bg_idx, struct group_descriptor *bgd,
                     uint32_t firstb, uint32_t lastb, uint64_t inodetbl_blocks);

/**
 * @fn void inode_dump(struct inode *inode, const char *path)
 * @brief dump inode.
 *
 * @param inode Inode.
 * @param path Path.
 */
void inode_dump(struct inode *inode, const char *path);

/**
 * @fn void dentry_dump(struct dir_entry *dentry)
 * @brief dump directory entry.
 *
 * @param dentry Directory entry.
 */
void dentry_dump(struct dir_entry *dentry);

/**
 * @fn void path_get_parent(const char *path, char *parent, char *child)
 * @brief get parent path.
 *
 * @param path Path.
 * @param parent Parent.
 * @param child Child.
 */
void path_get_parent(const char *path, char *parent, char *child);

/**
 * @struct imsb
 * @brief In-memory Super Block - Global State for our FS code.
 *
 * NOTE! The inodes repository hash lock could be fine-grained
 * by having a lock on each hash collision linked-list instead.
 */
struct imsb {
  /**
   * On-disk Superblock.
   */
  union super_block *sb;

  /**
   * On-disk Group Desc Table.
   */
  struct group_descriptor *bgd;

  /**
   * Ramdisk buffer.
   */
  char *buf;

  /**
   * 1, 2, or 4K.
   */
  uint64_t block_size;

  /**
   * 1, 2, or 4K.
   */
  uint64_t frag_size;

  /**
   * Block groups count.
   */
  uint64_t blockgroups_count;

  /**
   * Last block group.
   */
  uint64_t last_blockgroup;

  /**
   * Inode allocation lock.
   */
  spinlock_t inode_allocation_lock;

  /**
   * Block allocation lock.
   */
  spinlock_t block_allocation_lock;

  /**
   * Inodes hash.
   */
  struct hash *inodes_hash;

  /**
   * Inodes hash lock.
   */
  spinlock_t inodes_hash_lock;
};

/**
 * @var imsb
 * @brief In-memory Super Block.
 */
extern struct imsb imsb;


/**
 * @enum parsing_state
 * @brief States for parsing a hierarchial Unix path.
 */
enum parsing_state {
  START,      /* Start of line */
  SLASH,      /* Entry names separator */
  FILENAME,    /* Dir or reg file name */
  EOL,      /* End of line */
};


#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_FS_EXT2_H__ */

