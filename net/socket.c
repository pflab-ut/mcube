/**
 * @file net/socket.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

#if !CONFIG_ARCH_SIM


static struct socket_struct sockets[SOMAXCONN] = {{false, {0}}};


int accept(__unused int sockfd, __unused struct sockaddr *addr,
           __unused socklen_t *addrlen)
{
  return 0;
}

int bind(__unused int sockfd, __unused const struct sockaddr *addr,
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

int socket(int domain, int type, int protocol)
{
  int ret = -1;

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
            ret = i;
            break;
          }
        }

        break;

      default:
        printk("Error: protocol %d is not implemented\n", protocol);
        break;
      }

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
