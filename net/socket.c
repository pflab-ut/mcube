/**
 * @file net/socket.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

#if !CONFIG_ARCH_SIM

/* 0: stdin, 1: stdout, 2: stderr */
static int socket_id = 3;

int accept(__unused int sockfd, __unused struct sockaddr *addr,
           __unused socklen_t *addrlen)
{
  return 0;
}

int bind(__unused int sockfd, __unused const struct sockaddr *my_addr,
         __unused socklen_t addrlen)
{
  return 0;
}

int connect(__unused int sockfd, __unused const struct sockaddr *addr,
            __unused socklen_t addrlen)
{
  return 0;
}

int listen(__unused int sockfd, __unused int backlog)
{
  return 0;
}

int socket(__unused int domain, __unused int type, __unused int protocol)
{
  switch (domain) {
  case AF_UNIX:
    switch (type) {
    case SOCK_STREAM:
      return socket_id++;
      break;
    default:
      printk("Error: type %d is not implemented\n", type);
      break;
    }

    break;

  default:
    printk("Error: domain %d is not implemented\n", domain);
    break;
  }

  return -1;
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


#endif /* !CONFIG_ARCH_SIM */


