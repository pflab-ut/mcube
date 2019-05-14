/**
 * @file net/socket.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

#if !CONFIG_ARCH_SIM


static struct socket_struct sockets[SOMAXCONN] = {{false, false, {0, ""}}};

#if CONFIG_ARCH_AXIS
spinlock_t socket_lock;
#else
spinlock_t socket_lock = INIT_SPINLOCK;
#endif /* CONFIG_ARCH_AXIS */


int accept(int sockfd, __unused struct sockaddr *addr,
           __unused socklen_t *addrlen)
{
  int ret;
  spin_lock(&socket_lock);

  if (sockfd >= 0 && sockfd < SOMAXCONN) {
    if (sockets[sockfd].passive_socket) {
      /* TODO: use poll() to wait for new socket. */
      for (;;)
        ;

      ret = -1;

      /* check if remaining socket exists */
      for (int i = 0; i < SOMAXCONN; i++) {
        if (!sockets[i].used) {
          sockets[i].used = true;
          ret = i;
          break;
        }
      }

      if (ret == -1) {
        /* The system-wide limit on the total number of open files has been reached. */
        errno = ENFILE;
      }

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

  spin_unlock(&socket_lock);
  return ret;
}

int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
  int ret;
  spin_lock(&socket_lock);

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

  spin_lock(&socket_lock);
  return ret;
}

int connect(__unused int sockfd, __unused const struct sockaddr *addr,
            __unused socklen_t addrlen)
{

  return 0;
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

  spin_lock(&socket_lock);
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
  for (int i = 0; i < NR_STDS; i++) {
    sockets[i].used = true;
  }
}


#endif /* !CONFIG_ARCH_SIM */
