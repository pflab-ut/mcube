/**
 * @file net/poll.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

#if !CONFIG_ARCH_SIM

int poll(struct pollfd *fds, nfds_t nfds,
         int timeout)
{
  /* TODO: implement */
  __uninitialized(fds);
  __uninitialized(nfds);
  __uninitialized(timeout);
  return 0;
}

#endif /* !CONFIG_ARCH_SIM */


