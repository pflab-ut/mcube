/**
 * @file net/socket.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

#if !CONFIG_ARCH_SIM

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
  return 0;
}

#endif /* !CONFIG_ARCH_SIM */


