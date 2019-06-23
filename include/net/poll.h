/**
 * @file include/net/poll.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_NET_POLL_H__
#define __MCUBE_NET_POLL_H__

#ifndef __ASSEMBLY__

#if !CONFIG_ARCH_SIM

/**
 * @def POLLIN
 * @brief There is data to read.
 */
#define POLLIN 0x0001

/**
 * @def POLLPRI
 * @brief There is some exceptional condition on the file descriptor.
 */
#define POLLPRI 0x0002

/**
 * @def POLLOUT
 * @brief Writing is now possible, though a write larger that the available space
 * in a socket or pipe will still block (unless O_NONBLOCK is set).
 */
#define POLLOUT 0x0004

/**
 * @def POLLERR
 * @brief Error condition (only returned in revents; ignored in events).
 * This bit is also set for a file descriptor referring to the write end of a pipe
 * when the read end has been closed.
 */
#define POLLERR 0x0008

/**
 * @def POLLHUP
 * @brief Hang up (only returned in revents; ignored in events).
 * Note that when reading from a channel such as a pipe or a stream socket,
 * this event merely indicates that the peer closed its end of the channel.
 * Subsequent reads from the channel will return 0 (end of file) only after
 * all outstanding data in the channel has been consumed.
 */
#define POLLHUP 0x0010

/**
 * @def POLLNVAL
 * @brief Invalid request: @a fd not open (only returned in revents; ignored in events).
 */
#define POLLNVAL 0x0020

/**
 * @struct pollfd
 * @brief Set of file descriptors to be polled.
 */
struct pollfd {
  /**
   * File descriptor.
   */
  int fd;

  /**
   * Requested events.
   */
  short events;

  /**
   * Returned events.
   */
  short revents;
};

/**
 * @typedef nfds_t
 * @brief Typedef of number of file descriptors.
 */
typedef unsigned long nfds_t;

/**
 * @fn int poll(struct pollfd *fds, nfds_t nfds, int timeout)
 * @brief wait for some event on a file descriptor.
 *
 * @param fds File descriptors.
 * @param nfds Number of file descriptors.
 * @param timeout Timeout [ms].
 */
int poll(struct pollfd *fds, nfds_t nfds, int timeout);

#endif /* !CONFIG_ARCH_SIM */

#endif /* __ASSEMBLY__ */

#endif	/* __MCUBE_NET_POLL_H__ */
