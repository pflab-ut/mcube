/**
 * @file net/socket.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

#if !CONFIG_ARCH_SIM


struct socket_struct sockets[SOMAXCONN] = {
  {
    .used = false,
    .passive_socket = false,
    .connect_id = -1,
    .addr = {.sun_family = -1, .sun_path = ""},
    .sendmsg = NULL,
    .recvmsg = NULL
  }
};

#if CONFIG_ARCH_AXIS
spinlock_t socket_lock;
#else
spinlock_t socket_lock = INIT_SPINLOCK;
#endif /* CONFIG_ARCH_AXIS */


int accept(int sockfd, __unused struct sockaddr *addr,
           __unused socklen_t *addrlen)
{
  int ret;

  if (sockfd >= 0 && sockfd < SOMAXCONN) {
    if (sockets[sockfd].passive_socket) {
      /* TODO: use poll() to wait for new socket. */
      while (true) {
        spin_lock(&socket_lock);

        if (sockets[sockfd].connect_id != -1) {
          spin_unlock(&socket_lock);
          break;
        }

        spin_unlock(&socket_lock);
        delay(1000);
      }

#if 1
      printk("accept sockets[%d].connect_id = %d\n", sockfd,
             sockets[sockfd].connect_id);
      ret = sockets[sockfd].connect_id;

      if ((sockets[sockfd].sendmsg = (char *) kmalloc(MSG_BUFSIZE)) == NULL) {
        ret = -1;
        goto out;
      }

      if ((sockets[sockfd].recvmsg = (char *) kmalloc(MSG_BUFSIZE)) == NULL) {
        ret = -1;
        goto out;
      }

#endif
    } else {
      /* sockfd is not an open file descriptor. */
      errno = EBADF;
      ret = -1;
    }
  } else {
    /* Socket is not listening for connections, or addrlen is invalid (e.g., is negative). */
    errno = EINVAL;
    ret = -1;
  }

out:
  return ret;
}

int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
  int ret;
  spin_lock(&socket_lock);

  if (addr->sa_family == AF_UNIX) {
    if (link(NULL, addr->sa_data) == -1) {
      /* The given address is already in use. */
      errno = EADDRINUSE;
      ret = -1;
    } else {
      if (sockfd >= 0 && sockfd < SOMAXCONN) {
        memmove(&sockets[sockfd].addr, addr, addrlen);
        ret = 0;
      } else {
        /* sockfd is not a valid file descriptor. */
        errno = EBADF;
        ret = -1;
      }
    }
  } else {
    errno = EINVAL;
    ret = -1;
  }

  spin_unlock(&socket_lock);
  return ret;
}

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
  int ret;
  spin_lock(&socket_lock);

  if (sockfd >= 0 && sockfd < SOMAXCONN) {
    for (int i = 0; i < SOMAXCONN; i++) {
      if (strcmp(sockets[i].addr.sun_path, addr->sa_data) == 0) {
        /* find file */
        /* connect both directions. */
        sockets[i].connect_id = sockfd;
        sockets[sockfd].connect_id = i;
        memmove(&sockets[sockfd].addr, addr, addrlen);
      }
    }

    if (sockets[sockfd].passive_socket) {
      /* Another socket is already listening on the same port. */
      errno = EADDRINUSE;
      ret = -1;
    } else {
      sockets[sockfd].passive_socket = true;
      ret = 0;
    }
  } else {
    /* The argument sockfd is not a valid file descriptor. */
    errno = EBADF;
    ret = -1;
  }

  spin_unlock(&socket_lock);
  return ret;
}

int listen(int sockfd, __unused int backlog)
{
  int ret;
  spin_lock(&socket_lock);

  if (sockfd >= 0 && sockfd < SOMAXCONN) {
    if (sockets[sockfd].passive_socket) {
      /* Another socket is already listening on the same port. */
      errno = EADDRINUSE;
      ret = -1;
    } else {
      sockets[sockfd].passive_socket = true;
      ret = 0;
    }
  } else {
    /* The argument sockfd is not a valid file descriptor. */
    errno = EBADF;
    ret = -1;
  }

  spin_unlock(&socket_lock);
  return ret;
}


int socket(int domain, int type, int protocol)
{
  int ret;
  spin_lock(&socket_lock);

  switch (domain) {
  case AF_UNIX:
    switch (type) {
    case SOCK_STREAM:
      switch (protocol) {
      case 0:
        ret = -1;

        /* check if remaining socket exists */
        for (int i = 0; i < SOMAXCONN; i++) {
          if (!sockets[i].used) {
            sockets[i].used = true;
            ret = i;
            break;
          }
        }

        break;

      default:
        ret = -1;
        /* Unknown protocol, or protocol family not available. */
        errno = EINVAL;
        break;
      }

      break;

    default:
      ret = -1;
      /* Unknown protocol, or protocol family not available. */
      errno = EINVAL;
      break;
    }

    break;

  default:
    ret = -1;
    /* Unknown protocol, or protocol family not available. */
    errno = EINVAL;
    break;
  }

  spin_unlock(&socket_lock);
  return ret;
}

int shutdown(__unused int sockfd, __unused int how)
{
  kfree(sockets[sockfd].recvmsg);
  kfree(sockets[sockfd].sendmsg);
  return 0;
}

ssize_t send(__unused int sockfd, __unused const void *buf, __unused size_t len,
             __unused int flags)
{
  return 0;
}

ssize_t recv(__unused int sockfd, __unused void *buf, __unused size_t len,
             __unused int flags)
{
  return 0;
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
    sockets[i].addr = (struct sockaddr_un) {
      .sun_family = AF_UNSPEC, .sun_path = ""
    };
    sockets[i].sendmsg = NULL;
    sockets[i].recvmsg = NULL;
  }

  //  pdebug_sockets();
}


#endif /* !CONFIG_ARCH_SIM */
