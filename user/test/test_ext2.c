/**
 * @file user/test/test_ext2.c
 *
 * @author Hiroyuki Chishiro
 */
/*
 * Copyright (C) 2012-2013 Ahmed S. Darwish <darwish.07@gmail.com>
 */

#include <mcube/mcube.h>

#if CONFIG_ARCH_X86_64 && CONFIG_OPTION_FS_EXT2


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
  for (uint i = 1; i <= imsb.sb->inodes_count; i++) {
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

__unused static void test_file_reads(__unused void *arg)
{
  struct inode *inode;
  char *buf;
  int len;
  struct buffer_dumper *bd = (void *)percpu_get(dumper);

  assert(bd);
  printk("c%d t%lu fr start\n", percpu_get(apic_id), get_current_process()->pid);

  if (!(buf = kmalloc(BUF_LEN))) {
    panic("Error: cannot allocate memory %lu\n", BUF_LEN);
  }

  for (uint i = 1; i <= imsb.sb->inodes_count; i++) {
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

  printk("c%d t%lu fr end!\n", percpu_get(apic_id), get_current_process()->pid);

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

  printk("c%d t%lu br start\n", percpu_get(apic_id), get_current_process()->pid);

  /* All possible permumations: Burn, baby, Burn! */
  for (uint i = 0; i < imsb.sb->blocks_count; i++) {
    for (uint off = 0; off < imsb.block_size; off++) {
      for (uint len = 0; len <= (imsb.block_size - off); len++) {
        bd->pr("Reading Block #%d, offset = %d, "
               "len = %d:\n", i, off, len);
        block_read(i, buf, off, len);
        printbuf(bd, buf, len);
      }
    }
  }

  kfree(buf);

  printk("c%d t%lu br end!\n", percpu_get(apic_id), get_current_process()->pid);

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

  printk("c%d t%ld ex start\n", percpu_get(apic_id), get_current_process()->pid);

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
                  (int64_t) get_current_process()->working_dir : name_i(parent);
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
  printk("c%d t%ld ex end\n", percpu_get(apic_id), get_current_process()->pid);

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

  bd = (void *) percpu_get(dumper);
  bd = &serial_null_dumper;

  char prefix[64];
  __vsnprint(prefix, sizeof(prefix) - 1, "c%d_t%lu_",
             percpu_get(apic_id), get_current_process()->pid);

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

  sb = imsb.sb;

  ext2_debug_init(&serial_null_dumper);
  struct buffer_dumper *bd = (void *)percpu_get(dumper);

  /* Extract the modified ext2 volume out of the virtual machine: */
  printk("Ramdisk start at: 0x%lx, with len = %ld\n", imsb.buf,
         ramdisk_get_len());

  test_inodes();
  test_block_reads();
  test_file_reads(NULL);

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
  nfree = imsb.sb->free_inodes_count;
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

  printk("Success! All inodes now allocated; inode_alloc() got NULL!\n");

  /* Deallocate half of the allocated inodes */
  for (uint i = 0; i < nfree / 2; i++) {
    inode = unrolled_lookup(&head, i);
    assert(inode);

    bd->pr("Deallocating inode #%ld\n", inode->inum);
    unrolled_remove_key(&head, i);
    inode_mark_delete(inode);
    inode_put(inode);
  }

  if (imsb.sb->free_inodes_count != nfree / 2) {
    panic("We've allocated all inodes, then deallocated %u "
          "of them. Nonetheless, reported num of free inos "
          "= %u instead of %u", nfree / 2, imsb.sb->free_inodes_count,
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

  printk("\n");
  printk("NOTE! All disk inodes are now allocated. Meanwhile, the ones "
         "we've manually allocated are not linked by any dir entries "
         "and have a dtime = 0. To make fsck happy, we'll deallocate "
         "all of those 'malformed' inodes now :-)\n\n");
  unrolled_for_each(&all_allocated, void_inum) {
    inode = inode_get((uint64_t) void_inum);
    printk("Deallocating inode #%lu\n", inode->inum);
    inode_mark_delete(inode);
    inode_put(inode);
  }
  printk("Done!\n");
#endif

#if TEST_BLOCK_ALLOC_DEALLOC
  nfree = imsb.sb->free_blocks_count;
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

    for (uint ino = 1; ino <= imsb.sb->inodes_count; ino++) {
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

      nblocks = CEIL(inode->size_low, imsb.block_size);
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

  if (imsb.sb->free_blocks_count != nfree) {
    panic("We've allocated all blocks, then deallocated all of "
          "them. Nonetheless, reported num of free blocks = %u "
          "instead of %u", imsb.sb->free_blocks_count, nfree);
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
    printk("Parent: '%s'\n", parent);
    printk("Child: '%s'\n", child);
    parent_inum = (*parent == '\0') ?
                  (int64_t) get_current_process()->working_dir : name_i(parent);

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
__noreturn static void test_alloc_dealloc(__unused void *arg)
{
  struct inode *inode;
  struct unrolled_head head;
  void *inode_ptr;
  bool complete = true;

  printk("c%d t%lu a start\n", percpu_get(apic_id), get_current_process()->pid);
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
  printk("c%d t%lu a %s\n", percpu_get(apic_id), get_current_process()->pid,
         (complete) ? "end!" : "no ino!");
  halt();
}

/*
 * Fuzz: Constantly Fetch and put inodes from disk, offering enough
 * SMP fuzz-testing against code accessing the file system.
 */
__noreturn static void smp_fuzz(__unused void *arg)
{
  struct inode *inode;

  while (true) {
    for (uint i = 1; i < imsb.sb->inodes_count; i++) {
      inode = inode_get(i);
      inode_put(inode);
    }
  }
}

static bool test_ext2_smp(void)
{
  /* Avoid serialization - minimize serial port output */
  ext2_debug_init(&serial_null_dumper);

  /* Each function runs in a thread context, let them
   * halt at the end */
  percpu_set(halt_thread_at_end, true);

  /* Extract the modified ext2 volume out of the virtual machine: */
  if (percpu_get(apic_id) == 0) {
    printk("Ramdisk start at: 0x%lx, with len = %ld\n", imsb.buf,
           ramdisk_get_len());
  }

  for (int i = 0; i < 200; i++) {
    kthread_create(test_alloc_dealloc, NULL, NULL);
  }

  for (int i = 0; i < 200; i++) {
    kthread_create(test_file_reads, NULL, NULL);
  }

  /*
   * Below functions are already very heavy, don't
   * fork a huge number of them here. Better still,
   * run them as SMP fuzzers instead ...
   */
  //for (int i = 0; i < 20; i++)
  //  kthread_create(test_block_reads, NULL, NULL);
  //for (int i = 0; i < 5; i++)
  //  kthread_create(test_file_existence, NULL, NULL);

  /*
   * Don't run concurrent writers yet, inodes locks
   * are not yet implemented; file system state could
   * get corrupted.
   */
  //for (int i = 0; i < 5; i++)
  //  kthread_create(test_file_creation, NULL, NULL);

  /*
   * Some SMP fuzzers
   */
  for (int i = 0; i < 10; i++) {
    kthread_create(smp_fuzz, NULL, NULL);
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

#endif /* CONFIG_ARCH_X86_64 && CONFIG_OPTION_FS_EXT2 */

