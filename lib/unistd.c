/**
 * @file lib/unistd.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

#if !CONFIG_ARCH_SIM

struct file_struct files[NR_FILES] = {{0}};


int link(const char *oldpath, const char *newpath)
{
  if (oldpath) {
    errno = EINVAL;
    return -1;
  }

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
  uint8_t *p = (uint8_t *) buf;
  int index;

  if (count == 0) {
    return 0;
  }

  if (fd < 0) {
    return -1;
  }

  if (!buf) {
    return -2;
  }

  index = sockets[fd].connect_id;

  while (true) {
    spin_lock(&socket_lock);

    if (!ring_buf_empty(&sockets[index].msg)) {
      break;
    }

    spin_unlock(&socket_lock);
    delay(1000);
  }

  while (!ring_buf_empty(&sockets[index].msg)) {
    ring_buf_get(&sockets[index].msg, p++);
  }

  spin_unlock(&socket_lock);
  return p - (uint8_t *) buf;
}

ssize_t write(int fd, const void *buf, size_t count)
{
  const uint8_t *p = (uint8_t *) buf;
  printk("write() do\n");

  if (count == 0) {
    return 0;
  }

  if (fd < 0) {
    return -1;
  }

  if (!buf) {
    return -2;
  }

  printk("count = %d\n", count);
  spin_lock(&socket_lock);

  for (size_t i = 0; i < count; i++) {
#if 0
    printk("&sockets[%d].msg.buffer = 0x%lx p[%d] = %d\n",
           fd, sockets[sockets[fd].connect_id].msg.buffer, i, p[i]);
#endif
    ring_buf_put(&sockets[sockets[fd].connect_id].msg, p[i]);
  }

  spin_unlock(&socket_lock);

  return count;
}

int close(int fd)
{
  spin_lock(&socket_lock);
  printk("close(): fd = %d\n", fd);
  sockets[fd].used = false;
  sockets[fd].passive_socket = false;
  sockets[fd].connect_id = -1;
  sockets[fd].addr.sun_family = AF_UNSPEC;
  memset(sockets[fd].addr.sun_path, 0, sizeof(sockets[fd].addr.sun_path));

  // XXX: exception in x86.
  //  ring_buf_free(&sockets[sockets[fd].connect_id].msg);

  //  sockets[fd].msg = INIT_RING_BUF;
  ring_buf_init(&sockets[fd].msg, NULL, 0);
  spin_unlock(&socket_lock);

  return 0;
}


#endif /* !CONFIG_ARCH_SIM */
