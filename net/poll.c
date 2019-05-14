/**
 * @file net/poll.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

#if !CONFIG_ARCH_SIM

int poll(__unused struct pollfd *fds, __unused nfds_t nfds,
         __unused int timeout)
{

  return 0;
}

#endif /* !CONFIG_ARCH_SIM */


