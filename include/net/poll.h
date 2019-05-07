/**
 * @file include/net/poll.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_NET_POLL_H__
#define __MCUBE_NET_POLL_H__

#ifndef __ASSEMBLY__

#if !CONFIG_ARCH_SIM


#define POLLIN          0x0001
#define POLLPRI         0x0002
#define POLLOUT         0x0004
#define POLLERR         0x0008
#define POLLHUP         0x0010
#define POLLNVAL        0x0020

struct pollfd {
  int   fd;         /* file descriptor */
  short events;     /* requested events */
  short revents;    /* returned events */
};

typedef unsigned long int nfds_t;

int poll(struct pollfd *fds, nfds_t nfds, int timeout);

#endif /* CONFIG_ARCH_SIM */

#endif /* __ASSEMBLY__ */

#endif	/* __MCUBE_NET_POLL_H__ */
