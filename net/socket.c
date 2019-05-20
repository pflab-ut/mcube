/**
 * @file net/socket.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

#if !CONFIG_ARCH_SIM

struct socket_struct sockets[SOMAXCONN];

#if CONFIG_ARCH_AXIS
spinlock_t socket_lock;
#else
spinlock_t socket_lock = INIT_SPINLOCK;
#endif /* CONFIG_ARCH_AXIS */


int accept(int sockfd, __unused struct sockaddr *addr,
           __unused socklen_t *addrlen)
{
  int ret = -1;

  spin_lock(&socket_lock);

  if (sockets[sockfd].type != SOCK_STREAM
      && sockets[sockfd].type != SOCK_SEQPACKET) {
    errno = EOPNOTSUPP;
    goto out;
  }

  if (sockfd < 0 || sockfd >= SOMAXCONN) {
    /* Socket is not listening for connections,
     * or addrlen is invalid (e.g., is negative). */
    errno = EINVAL;
    goto out;
  }

  /* TODO: use poll() to wait for new socket. */
  while (sockets[sockfd].connect_id == -1) {
    spin_unlock(&socket_lock);
    delay(1000);
    spin_lock(&socket_lock);
  }

  printk("accept(): sockets[%d].connect_id = %d\n", sockfd,
         sockets[sockfd].connect_id);
  ret = sockets[sockfd].connect_id;

  if (!(sockets[sockfd].msg.buffer = (uint8_t *) kmalloc(MSG_BUFSIZE))) {
    errno = ENOMEM;
    goto out;
  }

  sockets[ret].connect_id = sockfd;

out:
  spin_unlock(&socket_lock);
  return ret;
}

int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
  int ret = -1;
  spin_lock(&socket_lock);

  if (addr->sa_family != AF_UNIX) {
    errno = EINVAL;
    goto out;
  }

  if (link(NULL, addr->sa_data) == -1) {
    /* The given address is already in use. */
    errno = EADDRINUSE;
    goto out;
  }

  if (sockfd < 0 && sockfd >= SOMAXCONN) {
    /* sockfd is not a valid file descriptor. */
    errno = EBADF;
    goto out;
  }

  memmove(&sockets[sockfd].addr, addr, addrlen);
  ret = 0;
out:
  spin_unlock(&socket_lock);
  return ret;
}

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
  int ret = -1;
  spin_lock(&socket_lock);

  if (sockfd < 0 || sockfd >= SOMAXCONN) {
    /* The argument sockfd is not a valid file descriptor. */
    errno = EBADF;
    goto out;
  }

  if (sockets[sockfd].type != SOCK_STREAM
      && sockets[sockfd].type != SOCK_SEQPACKET) {
    errno = EOPNOTSUPP;
    goto out;
  }

  for (int i = 0; i < SOMAXCONN; i++) {
    if (strcmp(sockets[i].addr.sun_path, addr->sa_data) == 0) {
      /* find file */
      memmove(&sockets[sockfd].addr, addr, addrlen);
      ret = i;
      break;
    }
  }

  printk("connect(): ret = %d\n", ret);

  if (ret == -1) {
    errno = EADDRINUSE;
    goto out;
  }

  /* register connect id in server fd. */
  sockets[ret].connect_id = sockfd;

  /* wait until connect id is set in client fd. */
  while (sockets[sockfd].connect_id == -1) {
    spin_unlock(&socket_lock);
    delay(1000);
    spin_lock(&socket_lock);
  }

out:
  spin_unlock(&socket_lock);
  return ret;
}

int listen(int sockfd, __unused int backlog)
{
  int ret = -1;
  spin_lock(&socket_lock);

  if (sockfd < 0 || sockfd >= SOMAXCONN) {
    /* The argument sockfd is not a valid file descriptor. */
    errno = EBADF;
    goto out;
  }

  if (sockets[sockfd].passive_socket) {
    /* Another socket is already listening on the same port. */
    errno = EADDRINUSE;
    goto out;
  }

  sockets[sockfd].passive_socket = true;
  ret = 0;
out:
  spin_unlock(&socket_lock);
  return ret;
}


int socket(int domain, int type, int protocol)
{
  int ret = -1;
  spin_lock(&socket_lock);

  switch (domain) {
  case AF_UNIX:
    switch (type) {
    case SOCK_STREAM:
      switch (protocol) {
      case 0:

        /* check if remaining socket exists */
        for (int i = 0; i < SOMAXCONN; i++) {
          if (!sockets[i].used) {
            sockets[i].used = true;
            sockets[i].domain = AF_UNIX;
            sockets[i].type = SOCK_STREAM;
            sockets[i].protocol = 0;
            ret = i;
            break;
          }
        }

        break;

      default:
        /* Unknown protocol, or protocol family not available. */
        errno = EINVAL;
        break;
      }

      break;

    default:
      /* Unknown protocol, or protocol family not available. */
      errno = EINVAL;
      break;
    }

    break;

  default:
    /* Unknown protocol, or protocol family not available. */
    errno = EINVAL;
    break;
  }

  spin_unlock(&socket_lock);
  return ret;
}

int shutdown(int sockfd, __unused int how)
{
  return close(sockfd);
}

ssize_t send(int sockfd, const void *buf, size_t len, int flags)
{
  if (flags != 0) {
    printk("Error: cannot set flags 0x%x\n", flags);
    errno = EINVAL;
    return -1;
  }

  return write(sockfd, buf, len);
}

ssize_t recv(int sockfd, void *buf, size_t len, int flags)
{
  if (flags != 0) {
    printk("Error: cannot set flags 0x%x\n", flags);
    errno = EINVAL;
    return -1;
  }

  return read(sockfd, buf, len);
}

void init_socket(void)
{
  /* stdin, stdout, and stderr are used. */
  for (int i = 0; i < SOMAXCONN; i++) {
    if (i < NR_STDS) {
      sockets[i].used = true;
    } else {
      sockets[i].used = false;
    }

    sockets[i].passive_socket = false;
    sockets[i].connect_id = -1;
    sockets[i].domain = -1;
    sockets[i].type = -1;
    sockets[i].protocol = -1;
    sockets[i].addr = (struct sockaddr_un) {
      .sun_family = AF_UNSPEC, .sun_path = ""
    };
    sockets[i].msg = INIT_RING_BUF;
  }

  //  pdebug_sockets();
}


#endif /* !CONFIG_ARCH_SIM */
