/**
 * @file include/x86/file.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_FILE_H__
#define __MCUBE_X86_FILE_H__

#ifndef __ASSEMBLY__

int sys_chdir(const char *path);
int sys_creat(const char *path, __unused mode_t mode);
int sys_open(const char *path, int flags, __unused mode_t mode);
int64_t sys_read(int fd, void *buf, uint64_t count);
int64_t sys_write(int fd, void *buf, uint64_t count);
int64_t sys_lseek(int fd, uint64_t offset, uint whence);
int sys_fstat(int fd, struct stat *buf);
int sys_stat(const char *path, struct stat *buf);
int sys_close(int fd);
int sys_unlink(const char *path);
int sys_link(const char *oldpath, const char *newpath);

/*
 * States for parsing a hierarchial Unix path
 */
enum parsing_state {
  START,      /* Start of line */
  SLASH,      /* Entry names separator */
  FILENAME,    /* Dir or reg file name */
  EOL,      /* End of line */
};


#define FSTATIC    extern
uint path_get_leaf(const char *path, mode_t *leaf_type);

struct test_file {
  uint64_t inum;    /* Inode# of the open()-ed file */
  int flags;    /* Flags passed  to open() call */
  spinlock_t lock;  /* ONLY FOR offset and refcount */
  uint64_t offset;  /* MAIN FIELD: File byte offset */
  int refcount;    /* Reference count; fork,dup,.. */
};

void file_run_tests(void);


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_FILE_H__ */
