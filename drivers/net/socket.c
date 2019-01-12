/**
 * @file drivers/net/socket.c
 *
 * @author Hiroyuki Chishiro
 */
#include <est/est.h>
#include <arch/arch.h>
#include <net/net.h>
#include <debug/debug.h>

#if !CONFIG_ARCH_SIM


int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
  return 0;
}

int bind(int sockfd, const struct sockaddr *my_addr, socklen_t addrlen)
{
  return 0;
}

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
  return 0;
}


int listen(int sockfd, int backlog)
{
  return 0;
}

int socket(int domain, int type, int protocol)
{
  return 0;
}

#endif /* !CONFIG_ARCH_SIM */

