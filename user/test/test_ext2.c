/**
 * @file user/test/test_ext2.c
 *
 * @author Hiroyuki Chishiro
 */
/*
 * Copyright (C) 2012-2013 Ahmed S. Darwish <darwish.07@gmail.com>
 */

#include <mcube/mcube.h>

#if CONFIG_ARCH_X86 && CONFIG_OPTION_FS_EXT2

/**
 * Public interface:
 */

__unused static struct buffer_dumper vga_hex_dumper = {
  .pr = printk,
  .formatter = buf_hex_dump,
};

__unused static struct buffer_dumper vga_char_dumper = {
  .pr = printk,
  .formatter = buf_char_dump,
};

__unused static struct buffer_dumper vga_null_dumper = {
  .pr = print_uart,
  .formatter = buf_null_dump,
};

__unused static struct buffer_dumper serial_hex_dumper = {
  .pr = print_uart,
  .formatter = buf_hex_dump,
};

__unused static struct buffer_dumper serial_null_dumper = {
  .pr = print_uart,
  .formatter = buf_null_dump,
};

__unused static struct buffer_dumper null_null_dumper = {
  .pr = null_printer,
  .formatter = buf_null_dump,
};

void ext2_debug_init(struct buffer_dumper *g_dumper)
{
  percpu_set(dumper, (uintptr_t)g_dumper);
}

void superblock_dump(union super_block *sb)
{
  struct buffer_dumper *bd = (void *) percpu_get(dumper);
  sb->volume_label[EXT2_LABEL_LEN - 1] = '\0';
  sb->last_mounted[EXT2_LAST_MNT_LEN - 1] = '\0';
  bd->pr("Dumping Superblock contents:\n");
  bd->pr(".. Inodes count = %u inode\n", sb->inodes_count);
  bd->pr(".. Blocks count = %u block\n", sb->blocks_count);
  bd->pr(".. Reserved blocks count = %u block\n", sb->r_blocks_count);
  bd->pr(".. Free blocks count = %u block\n", sb->free_blocks_count);
  bd->pr(".. Free inodes count = %u inode\n", sb->free_inodes_count);
  bd->pr(".. First data block = #%u\n", sb->first_data_block);
  bd->pr(".. Block size = %u bytes\n", 1024U << sb->log_block_size);
  bd->pr(".. Fragment size = %u bytes\n", 1024U << sb->log_fragment_size);
  bd->pr(".. Blocks per group = %u block\n", sb->blocks_per_group);
  bd->pr(".. Fragments per group = %u frag\n", sb->frags_per_group);
  bd->pr(".. Inodes per group = %u inode\n", sb->inodes_per_group);
  bd->pr(".. Latest mount time = 0x%x\n", sb->mount_time);
  bd->pr(".. Latest write access = 0x%x\n", sb->write_time);
  bd->pr(".. Number of mounts since last fsck = %d\n", sb->mount_count);
  bd->pr(".. Max num of mounts before fsck = %d\n", sb->max_mount_count);
  bd->pr(".. FS Magic value = 0x%x\n", sb->magic_signature);
  bd->pr(".. FS State = %d\n", sb->state);
  bd->pr(".. Error behaviour = %d\n", sb->errors_behavior);
  bd->pr(".. Minor revision = %d\n", sb->minor_revision);
  bd->pr(".. Last time of fsck = 0x%x\n", sb->last_check);
  bd->pr(".. Time allowed between fscks = 0x%x\n", sb->check_interval);
  bd->pr(".. Creator OS = %u\n", sb->creator_os);
  bd->pr(".. Revision level = %u\n", sb->revision_level);
  bd->pr(".. UID for reserved blocks = %d\n", sb->reserved_uid);
  bd->pr(".. GID for reserved blocks = %d\n", sb->reserved_gid);
  bd->pr(".. First non-reserved inode = %u\n", sb->first_inode);
  bd->pr(".. Inode size = %d bytes\n", sb->inode_size);
  bd->pr(".. Block group # hosting this super: %d\n", sb->block_group);
  bd->pr(".. Compatible features bitmask = 0x%x\n", sb->features_compat);
  bd->pr(".. Incompatible features mask = 0x%x\n", sb->features_incompat);
  bd->pr(".. RO-compatible features = 0x%x\n", sb->features_ro_compat);
  bd->pr(".. Volume label = `%s'\n", sb->volume_label);
  bd->pr(".. Directory path of last mount = `%s'\n", sb->last_mounted);
  bd->pr("\n");
}

void blockgroup_dump(int idx, struct group_descriptor *bgd,
                     uint32_t firstb, uint32_t lastb, uint64_t inodetbl_blocks)
{
  struct buffer_dumper *bd = (void *)percpu_get(dumper);
  bd->pr("Group #%d: (Blocks %u-%u)\n", idx, firstb, lastb);
  bd->pr(".. Block bitmap at %u\n", bgd->block_bitmap);
  bd->pr(".. Inode bitmap at %u\n", bgd->inode_bitmap);
  bd->pr(".. Inode table at %u-%u\n", bgd->inode_table,
         bgd->inode_table + inodetbl_blocks - 1);
  bd->pr(".. %d free blocks, %d free inodes, %d directories\n",
         bgd->free_blocks_count, bgd->free_inodes_count,
         bgd->used_dirs_count);
  bd->pr("\n");
}

void inode_dump(struct inode *inode, const char *path)
{
  struct buffer_dumper *bd = (void *)percpu_get(dumper);
  bd->pr("Dumping inode contents, #%d, for '%s':\n", inode->inum, path);
  bd->pr(".. Mode = 0x%x, Flags = 0x%x\n", inode->mode, inode->flags);
  bd->pr(".. UID = %d, GID = %d\n", inode->uid, inode->gid_low);
  bd->pr(".. Last time this inode was accessed = 0x%x\n", inode->atime);
  bd->pr(".. Last time this inode was modified = 0x%x\n", inode->mtime);
  bd->pr(".. Time when this inode was deleted = 0x%x\n", inode->dtime);
  bd->pr(".. Links count = %d links\n", inode->links_count);
  bd->pr(".. File size = %d bytes\n", inode->size_low);
  bd->pr(".. 512-byte Blocks count = %u blocks\n", inode->i512_blocks);
  bd->pr(".. Block number for ACL file = #%u\n", inode->file_acl);
  bd->pr(".. Data Blocks:\n");

  for (int i = 0; i < EXT2_INO_NR_BLOCKS; i++) {
    bd->pr("%u ", inode->blocks[i]);
  }

  bd->pr("\n\n");
}

void dentry_dump(struct dir_entry *dentry)
{
  char *name;

  assert(dentry->filename_len != 0);
  assert(dentry->filename_len <= EXT2_FILENAME_LEN);

  if (!(name = kmalloc(dentry->filename_len + 1))) {
    panic("Error: cannot allocate memory %lu\n", dentry->filename_len + 1);
  }

  memcpy(name, dentry->filename, dentry->filename_len);
  name[dentry->filename_len] = '\0';

  struct buffer_dumper *bd = (void *) percpu_get(dumper);
  bd->pr("Dumping Directory Entry contents:\n");
  bd->pr(".. Inode number = %u\n", dentry->inode_num);
  bd->pr(".. Record len = %d bytes\n", dentry->record_len);
  bd->pr(".. Filename len = %d bytes\n", dentry->filename_len);
  bd->pr(".. File type = %d\n", dentry->file_type);
  bd->pr(".. Filename = '%s'\n", name);
  bd->pr("\n");
  kfree(name);
}

/*
 * Given UNIX @path, put its leaf node in @child, and the dir
 * path leading to that leaf in @parent (Mini-stateful parser)
 */
enum ext2_state { EXT2_S_NONE, EXT2_SLASH, EXT2_FILENAME, EXT2_EOL };
__unused static void path_get_parent(const char *path, char *parent,
                                     char *child)
{
  enum ext2_state state, prev_state;
  int sub_idx;

  sub_idx = 0;
  state = EXT2_S_NONE;

  for (size_t i = 0; i <= strlen(path); i++) {
    prev_state = state;

    if (path[i] == '/') {
      state = EXT2_SLASH;
      assert(prev_state != EXT2_SLASH);

      if (prev_state == EXT2_S_NONE) {
        sub_idx = i + 1;
        continue;
      }
    } else if (path[i] == '\0') {
      state = EXT2_EOL;

      if (prev_state == EXT2_SLASH) {
        continue;
      }
    } else {
      state = FILENAME;

      if (i - sub_idx > EXT2_FILENAME_LEN) {
        panic("File name in path '%s' too long", path);
      }
    }

    if (path[i] == '/' || path[i] == '\0') {
      memcpy(child, &path[sub_idx], i - sub_idx);
      memcpy(parent, path, sub_idx);
      child[i - sub_idx] = '\0';
      parent[sub_idx] = '\0';
      sub_idx = i + 1;
    }
  }
}

/*
 * File System test cases (I)
 */


#define TEST_INODES      1
#define TEST_BLOCK_READS    1
#define TEST_FILE_READS      1
#define TEST_DIR_ENTRIES    1
#define TEST_PATH_CONVERSION    1
#define TEST_INODE_ALLOC_DEALLOC  1
#define TEST_BLOCK_ALLOC_DEALLOC  1
#define TEST_FILE_WRITES    1
#define TEST_FILE_EXISTENCE    1
#define TEST_FILE_CREATION    1
#define TEST_FILE_TRUNCATE    1
#define TEST_FILE_DELETION    1


enum {
  BUF_LEN      = 4096,
};

/*
 * For good testing of the code which matches Unix file pathes to
 * inodes, a comprehensive list of file  paths should be put here.
 * Check the 'ext2/files_list.c' file for further information.
 */
extern struct path_translation ext2_files_list[];

/*
 * Different paths for the Unix root inode; i.e. '/', '/..', etc.
 */
extern const char *ext2_root_list[];

/*
 * List all files located under given directory
 */
__unused static void list_files(uint64_t dir_inum)
{
  struct inode *dir;
  struct dir_entry *dentry;
  uint64_t offset, len;
  char *name;
  struct buffer_dumper *bd = (void *) percpu_get(dumper);

  dir = inode_get(dir_inum);

  if (!(dentry = kmalloc(sizeof(*dentry)))) {
    panic("Error: cannot allocate memory %lu\n", sizeof(*dentry));
  }

  if (!(name = kmalloc(EXT2_FILENAME_LEN + 1))) {
    panic("Error: cannot allocate memory %lu\n", EXT2_FILENAME_LEN + 1);
  }

  for (offset = 0; ; offset += dentry->record_len) {
    len = file_read(dir, (char *)dentry, offset, sizeof(*dentry));

    if (len == 0) {
      break;
    }

    if (dentry->inode_num == 0) {
      continue;
    }

    if (!dir_entry_valid(dir, dentry, offset, len)) {
      bd->pr("Directory inode = %lu\n", dir->inum);
      panic("Invalid directory entry, check log!");
    }

    memcpy(name, dentry->filename, dentry->filename_len);
    name[dentry->filename_len] = '\0';
    bd->pr("File: '%s', inode = %lu\n", name, dentry->inode_num);
  }

  kfree(name);
  kfree(dentry);
  inode_put(dir);
}

__unused static void test_inodes(void)
{
  struct inode *inode, *inode2, *inode3;

  // All volume inodes!
  for (uint i = 1; i <= isb.sb->inodes_count; i++) {
    inode = inode_get(i);
    inode2 = inode_get(i);
    inode3 = inode_get(i);

    if (inode != inode2) {
      panic("Requesting the same inode #%d twice returned "
            "different ino images at 0x%lx, 0x%lx", i,
            inode, inode2);
    }

    if (inode2 != inode3) {
      panic("Requesting the same inode #%d thrice returned "
            "different ino images at 0x%lx, 0x%lx", i,
            inode2, inode3);
    }

    inode_dump(inode, "");
    inode_put(inode);
    inode_put(inode2);
    inode_put(inode3);
  }
}

__unused static void test_path_conversion(void)
{
  int64_t inum;
  struct path_translation *file;
  struct buffer_dumper *bd = (void *)percpu_get(dumper);

  /* Different forms of EXT2_ROOT_INODE */
  for (uint i = 0; ext2_root_list[i]; i++) {
    const char *path = ext2_root_list[i];
    inum = name_i(path);

    bd->pr("Path: '%s', Inode = %lu\n", path, inum);

    if (inum != EXT2_ROOT_INODE) {
      panic("_EXT2: Path '%s' returned invalid inode #%lu",
            path, inum);
    }
  }

  /* Custom files list, should be manually checked */
  for (uint i = 0; ext2_files_list[i].path; i++) {
    file = &ext2_files_list[i];
    file->absolute_inum = name_i(file->path);
    bd->pr("Path: '%s', Inode = %lu\n", file->path, file->absolute_inum);
  }

  /* Path file name length tests */
  char *path;

  if (!(path = kmalloc(EXT2_FILENAME_LEN + 4))) {
    panic("Error: cannot allocate memory %lu\n", EXT2_FILENAME_LEN + 4);
  }

  path[0] = '/';
  char *p = &path[1];

  for (int i = 0; i < EXT2_FILENAME_LEN + 1; i++) {
    p[i] = 'A';
  }

  p[EXT2_FILENAME_LEN + 1] = '\0';  // Should return -ENAMETOOLONG

  if ((inum = name_i(path)) < 0) {
    bd->pr("SUCCESS: '%s', Inode = %s\n", path, errno_to_str(inum));
  } else {
    bd->pr("FAILURE: '%s', Inode = %ld\n", path, inum);
  }

  for (int i = 0; i < EXT2_FILENAME_LEN; i++) {
    p[i] = 'B';
  }

  p[EXT2_FILENAME_LEN] = '\0';    // Should return -ENOENT
  inum = name_i(path);

  if ((inum = name_i(path)) < 0) {
    bd->pr("SUCCESS: '%s', Inode = %s\n", path, errno_to_str(inum));
  } else {
    bd->pr("FAILURE: '%s', Inode = %ld\n", path, inum);
  }
}

__unused static void test_file_reads(void)
{
  struct inode *inode;
  char *buf;
  int len;
  struct buffer_dumper *bd = (void *)percpu_get(dumper);

  assert(bd);
  print_uart("c%d t%lu fr start\n", percpu_get(apic_id), current->pid);

  if (!(buf = kmalloc(BUF_LEN))) {
    panic("Error: cannot allocate memory %lu\n", BUF_LEN);
  }

  for (uint i = 1; i <= isb.sb->inodes_count; i++) {
    bd->pr("Trying inode #%u: ", i);
    inode = inode_get(i);

    if (S_ISDIR(inode->mode)) {
      bd->pr("Directory!\n");
      inode_put(inode);
      continue;
    }

    if (inode->links_count == 0) {
      bd->pr("Free inode!\n");
      inode_put(inode);
      continue;
    }

    len = file_read(inode, buf, 0, BUF_LEN);
    inode_put(inode);

    if (len == 0) {
      bd->pr("No data!\n");
      continue;
    }

    bd->pr("It contains data:\n", i);
    printbuf(bd, buf, len);
    bd->pr("\n");
  }

  kfree(buf);

  print_uart("c%d t%lu fr end!\n", percpu_get(apic_id), current->pid);

  if (percpu_get(halt_thread_at_end)) {
    halt();
  }
}

__unused static void test_block_reads(void)
{
  char *buf;
  struct buffer_dumper *bd;

  bd = (void *)percpu_get(dumper);

  if (!(buf = kmalloc(BUF_LEN))) {
    panic("Error: cannot allocate memory %lu\n", BUF_LEN);
  }

  print_uart("c%d t%lu br start\n", percpu_get(apic_id), current->pid);

  /* All possible permumations: Burn, baby, Burn! */
  for (uint i = 0; i < isb.sb->blocks_count; i++) {
    for (uint off = 0; off < isb.block_size; off++) {
      for (uint len = 0; len <= (isb.block_size - off); len++) {
        bd->pr("Reading Block #%d, offset = %d, "
               "len = %d:\n", i, off, len);
        block_read(i, buf, off, len);
        printbuf(bd, buf, len);
      }
    }
  }

  kfree(buf);

  print_uart("c%d t%lu br end!\n", percpu_get(apic_id), current->pid);

  if (percpu_get(halt_thread_at_end)) {
    halt();
  }
}

/*
 * Assure -EEXIST on all existing Ext2 volume files
 *
 * TODO: Once we're able to pass parameters to kernel threads, let
 * this function (if the first param is true) act as an SMP fuzzer
 * instead by doing an infinite loop of existence checking.
 */
__unused static void test_file_existence(void)
{
  char *parent, *child;
  struct path_translation *file;
  struct inode *parent_ino;
  int64_t parent_inum, inum;
  struct buffer_dumper *bd;

  bd = (void *)percpu_get(dumper);

  print_uart("c%d t%ld ex start\n", percpu_get(apic_id), current->pid);

  if (!(parent = kmalloc(4096))) {
    panic("Error: cannot allocate memory %lu\n", 4096);
  }

  if (!(child = kmalloc(EXT2_FILENAME_LEN + 1))) {
    panic("Error: cannot allocate memory %lu\n", EXT2_FILENAME_LEN + 1);
  }

  for (uint j = 0; ext2_files_list[j].path; j++) {
    file = &ext2_files_list[j];
    bd->pr("Testing Path '%s':\n", file->path);
    path_get_parent(file->path, parent, child);
    bd->pr("Parent: '%s'\n", parent);
    bd->pr("Child: '%s'\n", child);
    parent_inum = (*parent == '\0') ?
                  (int64_t) current->working_dir : name_i(parent);
    parent_ino = inode_get(parent_inum);
    inum = file_new(parent_ino, child, EXT2_FT_REG_FILE);

    if (inum != -EEXIST) {
      panic("File with path '%s' already exists, but "
            "file_new allocated a new ino %u for it!",
            file->path, inum);
    }

    inode_put(parent_ino);
    bd->pr("Success: file creation returned -EEXIST\n");
  }

  kfree(parent);
  kfree(child);
  print_uart("c%d t%ld ex end\n", percpu_get(apic_id), current->pid);

  if (percpu_get(halt_thread_at_end)) {
    halt();
  }
}

__unused static int __vsnprint(char *buf, int size, const char *fmt, ...)
{
  va_list args;
  int n;

  va_start(args, fmt);
  n = vsnprint(buf, size - 1, fmt, args);
  va_end(args);
  buf[n] = 0;
  return n;
}

__unused static void test_file_creation(void)
{
  struct inode *parent_ino;
  int64_t inum;
  struct buffer_dumper *bd;

  bd = (void *)percpu_get(dumper);
  bd = &null_null_dumper;

  char prefix[64];
  __vsnprint(prefix, sizeof(prefix) - 1, "c%d_t%lu_",
             percpu_get(apic_id), current->pid);

  bd = &serial_char_dumper;

  /* Now just create a random set of regular files */
  char l1 = 'z', l2 = '~';
  int len = strlen(prefix);
  bool out = false;

  for (char p = 'a'; p <= 'z'; p++) {
    for (char ch = '0'; ch <= '~'; ch++) {
      prefix[len] = p;
      prefix[len + 1] = ch;
      prefix[len + 2] = '\0';
      bd->pr("Creating new file '%s': ", prefix);
      parent_ino = inode_get(EXT2_ROOT_INODE);
      inum = file_new(parent_ino, prefix, EXT2_FT_REG_FILE);

      if (inum <= 0 && inum != -EFBIG) {
        bd->pr("Returned %s", errno_to_str(inum));
        panic("File creation error; check log");
      }

      if (inum == -EFBIG) {
        l1 = p;
        l2 = ch;
        out = true;
      }

      inode_put(parent_ino);

      if (out) {
        bd->pr("Reached file size limit!");
        goto out;
      }

      bd->pr("Success!\n");
    }
  }

out:
  /* Assure -EEXIST on recreation of files created above */
  len = strlen(prefix) - 2;

  for (char p = 'a'; p <= l1; p++) {
    for (char ch = '0'; ch <= l2; ch++) {
      prefix[len] = p;
      prefix[len + 1] = ch;
      prefix[len + 2] = '\0';
      parent_ino = inode_get(EXT2_ROOT_INODE);
      inum = file_new(parent_ino, prefix, EXT2_FT_DIR);

      if (inum != -EEXIST)
        panic("File '%s' already exists, but file_new "
              "allocated a new ino %lu for it", prefix, inum);

      inode_put(parent_ino);
    }
  }

#if 0
  /* Test directories creation */
  char pname[4], ch;
  pname[0] = '/', pname[1] = 'A', pname[3] = '\0', ch = '0';

  for (int i = 0; i < 40; i++, ch++) {
    pname[2] = ch;
    bd->pr("Starting from root directory '/':\n");
    inum = EXT2_ROOT_INODE;

    /* For each dir, create a 50-level deep dir heiararchy!! */
    for (int i = 0; i < 50; i++) {
      bd->pr("Creating new sub-dir '%s': ", pname + 1);
      parent_ino = inode_get(inum);

      if ((inum = file_new(parent_ino, pname + 1, EXT2_FT_DIR)) <= 0) {
        bd->pr("Returned %ld", inum);
        panic("File creation error; check log");
      }

      inode_put(parent_ino);
      bd->pr("Success!\n");
    }

    bd->pr("\n");
  }

  bd->pr("Listing contents of folder /:\n");
  list_files(EXT2_ROOT_INODE);

  /* Test the contents of created directories contents */
  ch = '0';

  for (int i = 0; i < 40; i++, ch++) {
    pname[2] = ch;
    bd->pr("Listing contents of folder %s:\n", pname);
    list_files(name_i(pname));
    bd->pr("\n");
  }

  /* Boundary Case: what about files creation with long names? */
  char longname[EXT2_FILENAME_LEN + 1];
  memset(longname, 'a', EXT2_FILENAME_LEN);
  longname[EXT2_FILENAME_LEN] = '\0';
  parent_ino = inode_get(EXT2_ROOT_INODE);
  inum = file_new(parent_ino, longname, EXT2_FT_REG_FILE);
  bd->pr("Creating file '%s', returned %ld\n", longname, inum);

  if (inum > 0) {
    panic("Tried to create long file name of len %d, but it was "
          "accepted and inode %lu returned;  ENAMETOOLONG should"
          "'ve been returned!", EXT2_FILENAME_LEN, inum);
  }

  longname[EXT2_FILENAME_LEN - 1] = '\0';
  inum = file_new(parent_ino, longname, EXT2_FT_REG_FILE);
  bd->pr("Creating file '%s', returned %ld\n", longname, inum);

  if (inum < 0) {
    panic("Tried to create max possible len (%d) file name, but "
          "error %ld was returned!", EXT2_FILENAME_LEN - 1, inum);
  }

  inode_put(parent_ino);
#endif
}


static void test_ext2_up(void)
{
  __unused union super_block *sb;
  __unused struct inode *inode;
  __unused struct dir_entry *dentry;
  __unused struct unrolled_head head;
  __unused struct path_translation *file;
  __unused uint64_t len, block, nblocks, nfree;
  __unused int64_t ilen, inum, count, parent_inum;
  __unused char *buf, *buf2, *parent, *child;

  if (!(buf = kmalloc(BUF_LEN))) {
    panic("Error: cannot allocate memory %lu\n", BUF_LEN);
  }

  if (!(buf2 = kmalloc(BUF_LEN))) {
    panic("Error: cannot allocate memory %lu\n", BUF_LEN);
  }

  sb = isb.sb;

  ext2_debug_init(&null_null_dumper);
  struct buffer_dumper *bd = (void *)percpu_get(dumper);

  /* Extract the modified ext2 volume out of the virtual machine: */
  print_uart("Ramdisk start at: 0x%lx, with len = %ld\n", isb.buf,
             ramdisk_get_len());

  test_inodes();
  test_block_reads();
  test_file_reads();

#if TEST_DIR_ENTRIES

  /* Most of these fields are invalid, on purpose */
  if (!(dentry = kmalloc(sizeof(*dentry)))) {
    panic("Error: cannot allocate memory %lu\n", sizeof(*dentry));
  }

  dentry->inode_num = 0xffffffff;
  dentry->record_len = 3;
  dentry->filename_len = 5;
  dentry->file_type = 7;
  memcpy(dentry->filename, "testFile", 8);
  inode = inode_get(15);
  dir_entry_valid(inode, dentry, 10, 5);
  inode_put(inode);
  kfree(dentry);
#endif

  test_path_conversion();

  /*
   * Allocate all of the free system inodes (x), then deallocate half of
   * the allocated quantity (x/2).  Allocate the deallocated half again,
   * then deallocate half of it (x/4). Do so, till all the FS inodes are
   * allocated. This should hopefully provide good __serial__ test cases.
   *
   * This test must run on a single thread, __without__ any other code
   * allocating inodes behind our back.
   */
#if TEST_INODE_ALLOC_DEALLOC
  nfree = isb.sb->free_inodes_count;
  struct unrolled_head all_allocated;
  void *void_ino, *void_inum;
  bool first_run = true;
again:
  unrolled_init(&head, 64);

  if (first_run) {
    unrolled_init(&all_allocated, 64);
  }

  for (uint i = 0; i < nfree; i++) {
    inode = inode_alloc(EXT2_FT_REG_FILE);

    if (!inode) {
      panic("Reported free inodes count = %lu, but our "
            "%u-th allocation returned NULL!", nfree, i);
    }

    bd->pr("Returned inode = #%lu\n", inode->inum);
    unrolled_insert(&head, inode);

    if (first_run) {
      unrolled_insert(&all_allocated, (void *)inode->inum);
    }

    if (inode->links_count > 0 && inode->dtime != 0) {
      panic("Allocated used inode #%lu, its links count "
            "= %d!", inode->inum, inode->links_count);
    }
  }

  inode = inode_alloc(EXT2_FT_REG_FILE);

  if (inode) {
    // Boundary case
    panic("We've allocated all %lu inodes, how can a new "
          "allocation returns inode #%lu?", nfree, inode->inum);
  }

  print_uart("Success! All inodes now allocated; inode_alloc() got NULL!\n");

  /* Deallocate half of the allocated inodes */
  for (uint i = 0; i < nfree / 2; i++) {
    inode = unrolled_lookup(&head, i);
    assert(inode);

    bd->pr("Deallocating inode #%ld\n", inode->inum);
    unrolled_remove_key(&head, i);
    inode_mark_delete(inode);
    inode_put(inode);
  }

  if (isb.sb->free_inodes_count != nfree / 2) {
    panic("We've allocated all inodes, then deallocated %u "
          "of them. Nonetheless, reported num of free inos "
          "= %u instead of %u", nfree / 2, isb.sb->free_inodes_count,
          nfree / 2);
  }

  unrolled_for_each(&head, void_ino) {
    inode_put(void_ino);
  }
  nfree /= 2;
  unrolled_free(&head);

  if (nfree != 0) {
    bd->pr("Trying to allocate %u inodes again:\n", nfree);
    first_run = false;
    goto again;
  }

  print_uart("\n");
  print_uart("NOTE! All disk inodes are now allocated. Meanwhile, the ones "
             "we've manually allocated are not linked by any dir entries "
             "and have a dtime = 0. To make fsck happy, we'll deallocate "
             "all of those 'malformed' inodes now :-)\n\n");
  unrolled_for_each(&all_allocated, void_inum) {
    inode = inode_get((uint64_t) void_inum);
    print_uart("Deallocating inode #%lu\n", inode->inum);
    inode_mark_delete(inode);
    inode_put(inode);
  }
  print_uart("Done!\n");
#endif

#if TEST_BLOCK_ALLOC_DEALLOC
  nfree = isb.sb->free_blocks_count;
  count = 5;
bagain:
  unrolled_init(&head, 64);

  for (uint i = 0; i < nfree; i++) {
    block = block_alloc();

    if (block == 0) {
      panic("Reported free blocks count = %lu, but our "
            "%u-th allocation returned NULL!", nfree, i);
    }

    bd->pr("Returned block = %lu as free\n", block);
    unrolled_insert(&head, (void *)block);

    bd->pr("Verifying it's not really allocated: ");

    for (uint ino = 1; ino <= isb.sb->inodes_count; ino++) {
      inode = inode_get(ino);

      if (ino < sb->first_inode &&
          ino > EXT2_UNDELETE_DIR_INODE) {
        inode_put(inode);
        continue;
      }

      if (inode->links_count == 0 || inode->dtime != 0) {
        inode_put(inode);
        continue;
      }

      nblocks = CEIL(inode->size_low, isb.block_size);
      nblocks = MIN(nblocks, (uint64_t) EXT2_INO_NR_BLOCKS);

      for (uint ino_blk = 0; ino_blk < nblocks; ino_blk++) {
        if (inode->blocks[ino_blk] == 0) {
          continue;
        }

        if (inode->blocks[ino_blk] != block) {
          continue;
        }

        bd->pr("\nInode %lu contains that block!\n", ino);
        inode_dump(inode, "N/A");
        panic("Returned block %lu as free, but inode "
              "%lu contain that block!", block, ino);
      }

      inode_put(inode);
    }

    bd->pr("Success!\n\n", block);
  }

  block = block_alloc();

  if (block != 0) {
    // Boundary case
    panic("We've allocated all %lu blocks, how can a new "
          "allocation returns block #%lu?", nfree, block);
  }

  /* Deallocate all of the allocated blocks */
  for (uint i = 0; i < nfree; i++) {
    block = (uint64_t) unrolled_lookup(&head, i);
    assert(block != 0);

    bd->pr("Deallocating block %ld\n", block);
    block_dealloc(block);
  }

  if (isb.sb->free_blocks_count != nfree) {
    panic("We've allocated all blocks, then deallocated all of "
          "them. Nonetheless, reported num of free blocks = %u "
          "instead of %u", isb.sb->free_blocks_count, nfree);
  }

  unrolled_free(&head);
  count--;

  if (count != 0) {
    bd->pr("Trying to allocate %u blocks again:\n", nfree);
    goto bagain;
  }

#endif

#if TEST_FILE_WRITES
  int64_t last_file = -1;

  for (uint j = 0; ext2_files_list[j].path; j++) {
    file = &ext2_files_list[j];
    inum = name_i(file->path);
    assert(inum > 0);
    bd->pr("Writing to file '%s': ", file->path);

    if (last_file != -1) {
      bd->pr("Ignoring file!\n");
      continue;
    }

    inode = inode_get(inum);

    if (S_ISDIR(inode->mode) || S_ISLNK(inode->mode)) {
      inode_put(inode);
      bd->pr("Dir or a symlnk!\n");
      continue;
    }

    inode->mode &= ~S_IFMT;
    inode->mode |= S_IFREG;
    inode->dirty = true;
    memset32(buf, inum, BUF_LEN);

    for (uint offset = 0; offset < BUF_LEN * 3; offset += BUF_LEN) {
      ilen = file_write(inode, buf, offset, BUF_LEN);
      assert(ilen != 0);

      if (ilen == -ENOSPC) {
        bd->pr("Filled the entire disk!\n");
        bd->pr("Ignoring file!");
        last_file = j;
        inode_put(inode);
        goto out1;
      }

      if (ilen < 0) {
        bd->pr("%s\n", errno_to_str(ilen));
        continue;
      }

      assert(ilen == BUF_LEN);
      assert(inode->size_low >= offset + ilen);
      memset32(buf, ++inum, BUF_LEN);
    }

    assert(inode->size_low >= BUF_LEN * 2);
    inode_put(inode);
    bd->pr("Done!\n");
  }

out1:
  bd->pr("**** NOW TESTING THE WRITTEN DATA!\n");

  for (uint j = 0; ext2_files_list[j].path; j++) {
    file = &ext2_files_list[j];
    inum = name_i(file->path);
    assert(inum > 0);
    inode = inode_get(inum);
    bd->pr("Verifying file '%s': ", file->path);

    if (last_file != -1 && j >= last_file) {
      bd->pr("Ignoring file!\n");
      inode_put(inode);
      continue;
    }

    if (S_ISDIR(inode->mode) || S_ISLNK(inode->mode)) {
      bd->pr("Dir or a symlink!\n");
      inode_put(inode);
      continue;
    }

    memset32(buf2, inum, BUF_LEN);

    for (uint offset = 0; offset < BUF_LEN * 3; offset += BUF_LEN) {
      len = file_read(inode, buf, offset, BUF_LEN);

      if (len < BUF_LEN) {
        panic("We've written %d bytes to inode %lu, but "
              "returned only %d bytes on read", BUF_LEN,
              inode->inum, len);
      }

      if (memcmp(buf, buf2, BUF_LEN) != 0) {
        bd->pr("Data written differs from what's read!\n");
        bd->pr("We've written the following into file:\n");
        //        buf_hex_dump(buf2, BUF_LEN);
        bd->pr("But found the following when reading:\n");
        //        buf_hex_dump(buf, BUF_LEN);
        panic("Data corruption detected!");
      }

      memset32(buf2, ++inum, BUF_LEN);
    }

    inode_put(inode);
    bd->pr("Validated!\n");
  }

#endif

  test_file_existence();
  test_file_creation();

#if TEST_FILE_TRUNCATE

  for (uint i = 0; ext2_files_list[i].path; i++) {
    file = &ext2_files_list[i];
    inum = name_i(file->path);
    inode = inode_get(inum);
    bd->pr("Truncating file '%s': ", file->path);

    if (S_ISDIR(inode->mode)) {
      bd->pr("Directory!\n");
      inode_put(inode);
      continue;
    }

    inode_dump(inode, file->path);
    file_truncate(inode);

    assert(inode->size_low == 0);
    assert(inode->i512_blocks == 0);

    for (int i = 0; i < EXT2_INO_NR_BLOCKS; i++) {
      assert(inode->blocks[i] == 0);
    }

    inode_put(inode);
    bd->pr("\nSuccess!\n");
  }

#endif

#if TEST_FILE_DELETION

  if (!(parent = kmalloc(4096))) {
    panic("Error: cannot allocate memory %lu\n", 4096);
  }

  if (!(child = kmalloc(EXT2_FILENAME_LEN + 1))) {
    panic("Error: cannot allocate memory %lu\n", EXT2_FILENAME_LEN + 1);
  }

  for (uint i = 0; ext2_files_list[i].path; i++) {
    file = &ext2_files_list[i];
    bd->pr("Deleting file '%s'\n", file->path);
    inum = name_i(file->path);

    if (is_dir(inum) || is_symlink(inum)) {
      bd->pr("Directory or symlink!\n");
      continue;
    }

    path_get_parent(file->path, parent, child);
    print_uart("Parent: '%s'\n", parent);
    print_uart("Child: '%s'\n", child);
    parent_inum = (*parent == '\0') ?
                  (int64_t) current->working_dir : name_i(parent);

    if (parent_inum < 0) {
      bd->pr("FAILURE: Parent pathname resolution returned "
             "%s\n", errno_to_str(parent_inum));
      continue;
    }

    inode = inode_get(parent_inum);
    int ret = file_delete(inode, child);

    if (ret < 0) {
      bd->pr("FAILURE: Returned -%s\n", errno_to_str(ret));
    } else {
      bd->pr("Success!\n");
    }

    inode_put(inode);
  }

  kfree(child), kfree(parent);
#endif

  bd->pr("%s: Sucess!", __func__);

  if (bd->pr != printk) {
    printk("%s: Sucess!", __func__);
  }

  kfree(buf2);
  kfree(buf);
}

/*
 * File System test cases (II) - stressing the SMP side
 *
 * NOTE! Don't use lots of serial output sice it heavily serializes
 * the code, defeating our original purpose.
 */

/*
 * To test file-system code locks, run several hundred threads
 * of this function:
 *
 * TODO: Once semaphores are ready, don't quit, but sleep when no
 * more disk inodes are available.
 */
__noreturn static void test_alloc_dealloc(void)
{
  struct inode *inode;
  struct unrolled_head head;
  void *inode_ptr;
  bool complete = true;

  print_uart("c%d t%lu a start\n", percpu_get(apic_id), current->pid);
  unrolled_init(&head, 64);

  for (int i = 0; i < 100; i++) {
    inode = inode_alloc(EXT2_FT_REG_FILE);

    if (!inode) {
      complete = false;
      break;
    }

    unrolled_insert(&head, inode);
  }

  unrolled_for_each(&head, inode_ptr) {
    inode_mark_delete(inode_ptr);
    inode_put(inode_ptr);
  }
  print_uart("c%d t%lu a %s\n", percpu_get(apic_id), current->pid,
             (complete) ? "end!" : "no ino!");
  halt();
}

/*
 * Fuzz: Constantly Fetch and put inodes from disk, offering enough
 * SMP fuzz-testing against code accessing the file system.
 */
__noreturn static void smp_fuzz(void)
{
  struct inode *inode;

  while (true) {
    for (uint i = 1; i < isb.sb->inodes_count; i++) {
      inode = inode_get(i);
      inode_put(inode);
    }
  }
}

static bool test_ext2_smp(void)
{
  /* Avoid serialization - minimize serial port output */
  ext2_debug_init(&null_null_dumper);

  /* Each function runs in a thread context, let them
   * halt at the end */
  percpu_set(halt_thread_at_end, true);

  /* Extract the modified ext2 volume out of the virtual machine: */
  if (percpu_get(apic_id) == 0) {
    print_uart("Ramdisk start at: 0x%lx, with len = %ld\n", isb.buf,
               ramdisk_get_len());
  }

  for (int i = 0; i < 200; i++) {
    kthread_create(test_alloc_dealloc);
  }

  for (int i = 0; i < 200; i++) {
    kthread_create(test_file_reads);
  }

  /*
   * Below functions are already very heavy, don't
   * fork a huge number of them here. Better still,
   * run them as SMP fuzzers instead ...
   */
  //for (int i = 0; i < 20; i++)
  //  kthread_create(test_block_reads);
  //for (int i = 0; i < 5; i++)
  //  kthread_create(test_file_existence);

  /*
   * Don't run concurrent writers yet, inodes locks
   * are not yet implemented; file system state could
   * get corrupted.
   */
  //for (int i = 0; i < 5; i++)
  //  kthread_create(test_file_creation);

  /*
   * Some SMP fuzzers
   */
  for (int i = 0; i < 10; i++) {
    kthread_create(smp_fuzz);
  }

  return true;
}


bool test_ext2(void)
{
  test_ext2_up();
  test_ext2_smp();
  return true;
}
#else

bool test_ext2(void)
{
  return true;
}

#endif /* CONFIG_ARCH_X86 && CONFIG_OPTION_FS_EXT2 */

