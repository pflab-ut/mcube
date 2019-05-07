/**
 * @file lib/unistd.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

#if !CONFIG_ARCH_SIM

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
