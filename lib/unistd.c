/**
 * @file lib/unistd.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

#if !CONFIG_ARCH_SIM

struct file_struct files[NR_FILES] = {{0}};

int link(__unused const char *oldpath, const char *newpath)
{
  for (int i = 0; i < NR_FILES; i++) {
    if (strcmp(newpath, files[i].pathname) == 0) {
      /* file exists. */
      errno = EEXIST;
      return -1;
    }
  }

  for (int i = 0; i < NR_FILES; i++) {
    if (strlen(files[i].pathname) == 0) {
      /* create new file. */
      strcpy(files[i].pathname, newpath);
      return 0;
    }
  }

  /* cannot create file. */
  errno = EDQUOT;
  return -1;
}

int unlink(const char *pathname)
{
  for (int i = 0; i < NR_FILES; i++) {
    if (strcmp(pathname, files[i].pathname) == 0) {
      memset(files[i].pathname, '\0', sizeof(pathname));
      break;
    }
  }

  return 0;
}

ssize_t read(int fd, void *buf, size_t count)
{
  if (count == 0) {
    return 0;
  }

  if (fd < 0) {
    return -1;
  }

  if (!buf) {
    return -2;
  }

  /* TODO: implement */
  return 1;
}

ssize_t write(int fd, const void *buf, size_t count)
{
  if (count == 0) {
    return 0;
  }

  if (fd < 0) {
    return -1;
  }

  if (!buf) {
    return -2;
  }

  /* TODO: implement */
  return 1;
}


#endif /* !CONFIG_ARCH_SIM */
