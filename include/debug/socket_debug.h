/**
 * @file include/debug/socket_debug.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_DEBUG_SOCKET_DEBUG_H__
#define __MCUBE_DEBUG_SOCKET_DEBUG_H__

#ifndef __ASSEMBLY__

#if CONFIG_OPTION_DEBUG

#if !CONFIG_ARCH_SIM

/**
 * @fn static inline void print_msg(uint8_t *buf)
 * @brief print message.
 *
 * @param buf Buffer.
 */
static inline void print_msg(uint8_t *buf)
{
  for (int i = 0; i < MSG_BUFSIZE; i++) {
    PDEBUG("%c", buf[i]);
  }
}


/**
 * @fn static inline void print_sockets(void)
 * @brief print sockets.
 */
static inline void print_sockets(void)
{
  for (int i = 0; i < SOMAXCONN; i++) {
    PDEBUG("sockets[%d]: used %d, passive_socket %d, connect_id %d\n"
           "             addr.sun_famliy 0x%x addr.sun_path %s\n",
           i, sockets[i].used, sockets[i].passive_socket, sockets[i].connect_id,
           sockets[i].addr.sun_family, sockets[i].addr.sun_path);
    PDEBUG("             msg: ");
    print_msg(sockets[i].msg.buffer);
    PDEBUG("\n");
  }
}


/**
 * @def pdebug_sockets(socket)
 * @brief socket Socket.
 */
#define pdebug_sockets(socket) do {             \
    PDEBUG_WHERE();                             \
    print_sockets(socket);                      \
  } while (0)


#endif /* !CONFIG_ARCH_SIM */

#else

/**
 * @def pdebug_sockets(socket)
 * @brief socket Socket.
 */
#define pdebug_sockets(socket)

#endif /* CONFIG_OPTION_DEBUG */

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_DEBUG_SOCKET_DEBUG_H__ */
