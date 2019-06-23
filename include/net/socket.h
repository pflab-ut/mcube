/**
 * @file include/net/socket.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_NET_SOCKET_H__
#define __MCUBE_NET_SOCKET_H__

#ifndef __ASSEMBLY__

#if CONFIG_ARCH_SIM

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


#else

/**
 * @typedef sa_family_t
 * @brief Typedef of socket address family.
 */
typedef unsigned short sa_family_t;

/**
 * @typedef socklen_t
 * @brief Typedef of socket length.
 */
typedef int socklen_t;

/*
 *      1003.1g requires sa_family_t and that sa_data is char.
 */

/**
 * @struct sockaddr
 * @brief Socket address.
 *
 * The sockaddr structure has socket address information.
 */
struct sockaddr {
  /**
   *  address family, AF_xxx.
   */
  sa_family_t sa_family;

  /**
   * 14 bytes of protocol address.
   */
  char sa_data[14];
};

/**
 * @struct sockaddr_un
 * @brief Structure describing the address of an AF_LOCAL (aka AF_UNIX) socket.
 */
struct sockaddr_un {
  /**
   * AF_UNIX.
   */
  sa_family_t sun_family;

  /**
   * Pathname.
   */
  char sun_path[108];
};

/**
 * @typedef sockaddr
 * @brief Typedef of @struct sockaddr.
 */
typedef struct sockaddr sockaddr;

/**
 * @struct in_addr
 * @brief Internet address.
 *
 * The in_addr structure has definition of internet address.
 */
struct in_addr {
  /**
   * Socket in address.
   */
  unsigned long s_addr;
};

/**
 * @typedef in_addr
 * @brief Typedef of internet address.
 */
typedef struct in_addr in_addr;

/**
 * @struct sockaddr_in
 * @brief Socket address
 *
 * The sockaddr_in structure has internet socket address.
 */
struct sockaddr_in {
  /**
   * Address family.
   */
  short sin_family;

  /**
   * Port number.
   */
  unsigned short sin_port;

  /**
   * Internet address.
   */
  struct in_addr sin_addr;

  /**
   * Padding.
   */
  char sin_zero[8];
} __packed;

/**
 * @typedef sockaddr_in
 * @brief Typedef of @struct sockaddr_in.
 */
typedef struct sockaddr_in sockaddr_in;

/**
 * @enum socket_type
 * @brief Types of sockets.
 */
enum socket_type {
  /**
   * Sequenced, reliable, connection-based byte streams.
   */
  SOCK_STREAM = 1,

  /**
   * Connectionless, unreliable datagrams of fixed maximum length.
   */
  SOCK_DGRAM = 2,

  /**
   * Raw protocol interface.
   */
  SOCK_RAW = 3,

  /**
   * Reliably-delivered messages.
   */
  SOCK_RDM = 4,

  /**
   * Sequenced, reliable, connection-based, datagrams of fixed maximum length.
   */
  SOCK_SEQPACKET = 5,

  /**
   * Datagram Congestion Control Protocol.
   */
  SOCK_DCCP = 6,

  /**
   * Linux specific way of getting packets at the dev level.
   * For writing rarp and other similar things on the user level.
   */
  SOCK_PACKET = 10,

  /*
   * Flags to be ORed into the type parameter of socket and socketpair and
   * used for the flags parameter of paccept.
   */

  /**
   * Atomically set close-on-exec flag for the new descriptor(s).
   */
  SOCK_CLOEXEC = 02000000,

  /**
   * Atomically mark descriptor(s) as non-blocking.
   */
  SOCK_NONBLOCK = 04000
};

typedef enum socket_type socket_type;

/* Protocol families.  */

/**
 * @def PF_UNSPEC
 * @brief Unspecified.
 */
#define PF_UNSPEC 0

/**
 * @def PF_LOCAL
 * @brief Local to host (pipes and file-domain).
 */
#define PF_LOCAL 1

/**
 * @def PF_UNIX
 * @brief POSIX name for PF_LOCAL.
 */
#define PF_UNIX PF_LOCAL


/**
 * @def PF_FILE
 * @brief Another non-standard name for PF_LOCAL.
 */
#define PF_FILE PF_LOCAL

/**
 * @def PF_INET
 * @brief IP protocol family.
 */
#define PF_INET 2

/**
 * @def PF_AX25
 * @brief Amateur Radio AX.25.
 */
#define PF_AX25 3

/**
 * @def PF_IPX
 * @brief Novell Internet Protocol.
 */
#define PF_IPX 4

/**
 * @def PF_APPLETALK
 * @brief Appletalk DDP.
 */
#define PF_APPLETALK 5


/**
 * @def PF_NETROM
 * @brief Amateur radio NetROM.
 */
#define PF_NETROM 6

/**
 * @def PF_BRIDGE
 * @brief Multiprotocol bridge.
 */
#define PF_BRIDGE 7

/**
 * @def PF_ATMPVC
 * @brief ATM PVCs.
 */
#define PF_ATMPVC 8

/**
 * @def PF_X25
 * @brief Reserved for X.25 project.
 */
#define PF_X25 9

/**
 * @def PF_INET6
 * @brief IP version 6.
 */
#define PF_INET6 10

/**
 * @def PF_ROSE
 * @brief Amateur Radio X.25 PLP.
 */
#define PF_ROSE 11

/**
 * @def PF_DECnet
 * @brief Reserved for DECnet project.
 */
#define PF_DECnet 12

/**
 * @def PF_NETBEUI
 * @brief Reserved for 802.2LLC project.
 */
#define PF_NETBEUI 13

/**
 * @def PF_SECURITY
 * @brief Security callback pseudo AF.
 */
#define PF_SECURITY 14

/**
 * @def PF_KEY
 * @brief PF_KEY key management API.
 */
#define PF_KEY 15

/**
 * @def PF_NETLINK
 * @brief Kernel user interface device.
 */
#define PF_NETLINK 16

/**
 * @def PF_ROUTE
 * @brief Alias to emulate 4.4BSD.
 */
#define PF_ROUTE PF_NETLINK

/**
 * @def PF_PACKET
 * @brief Packet family.
 */
#define PF_PACKET 17

/**
 * @def PF_ASH
 * @brief Ash.
 */
#define PF_ASH 18

/**
 * @def PF_ECONET
 * @brief Acorn Econet.
 */
#define PF_ECONET 19

/**
 * @def PF_ATMSVC
 * @brief ATM SVCs.
 */
#define PF_ATMSVC 20

/**
 * @def PF_RDS
 * @brief RDS sockets.
 */
#define PF_RDS 21

/**
 * @def PF_SNA
 * @brief Linux SNA Project.
 */
#define PF_SNA 22

/**
 * @def PF_IRDA
 * @brief IRDA sockets.
 */
#define PF_IRDA 23

/**
 * @def PF_PPPOX
 * @brief PPPoX sockets.
 */
#define PF_PPPOX 24

/**
 * @def PF_WANPIPE
 * @brief Wanpipe API sockets.
 */
#define PF_WANPIPE 25

/**
 * @def PF_LLC
 * @brief Linux LLC.
 */
#define PF_LLC 26

/**
 * @def PF_CAN
 * @brief Controller Area Network.
 */
#define PF_CAN 29

/**
 * @def PF_TIPC
 * @brief TIPC sockets.
 */
#define PF_TIPC 30

/**
 * @def PF_BLUETOOTH
 * @brief Bluetooth sockets.
 */
#define PF_BLUETOOTH 31

/**
 * @def PF_IUCV
 * @brief IUCV sockets.
 */
#define PF_IUCV 32

/**
 * @def PF_RXRPC
 * @brief RxRPC sockets.
 */
#define PF_RXRPC 33

/**
 * @def PF_ISDN
 * @brief mISDN sockets.
 */
#define PF_ISDN 34

/**
 * @def PF_PHONET
 * @brief Phonet sockets.
 */
#define PF_PHONET 35

/**
 * @def PF_IEEE802154
 * @brief IEEE 802.15.4 sockets.
 */
#define PF_IEEE802154 36

/**
 * @def PF_MAX
 * @brief For now..
 */
#define PF_MAX 37



/* Address families.  */

/**
 * @def AF_UNSPEC
 * @brief Alias to PF_UNSPEC.
 */
#define AF_UNSPEC PF_UNSPEC

/**
 * @def AF_LOCAL
 * @brief Alias to PF_LOCAL.
 */
#define AF_LOCAL PF_LOCAL

/**
 * @def AF_UNIX
 * @brief Alias to PF_UNIX.
 */
#define AF_UNIX PF_UNIX

/**
 * @def AF_FILE
 * @brief Alias to PF_FILE.
 */
#define AF_FILE PF_FILE

/**
 * @def AF_INET
 * @brief Alias to PF_INET.
 */
#define AF_INET PF_INET

/**
 * @def AF_AX25
 * @brief Alias to PF_AX25.
 */
#define AF_AX25 PF_AX25

/**
 * @def AF_IPX
 * @brief Alias to PF_IPX.
 */
#define AF_IPX PF_IPX

/**
 * @def AF_APPLETALK
 * @brief Alias to PF_APPLETALK.
 */
#define AF_APPLETALK PF_APPLETALK

/**
 * @def AF_NETROM
 * @brief Alias to PF_NETROM.
 */
#define AF_NETROM PF_NETROM

/**
 * @def AF_BRIDGE
 * @brief Alias to PF_BRIDGE.
 */
#define AF_BRIDGE PF_BRIDGE

/**
 * @def AF_ATMPVC
 * @brief Alias to PF_ATMPVC.
 */
#define AF_ATMPVC PF_ATMPVC

/**
 * @def AF_X25
 * @brief Alias to PF_X25.
 */
#define AF_X25 PF_X25

/**
 * @def AF_INET6
 * @brief Alias to PF_INET6.
 */
#define AF_INET6 PF_INET6

/**
 * @def AF_ROSE
 * @brief Alias to PF_ROSE.
 */
#define AF_ROSE PF_ROSE

/**
 * @def AF_DECnet
 * @brief Alias to PF_DECnet.
 */
#define AF_DECnet PF_DECnet

/**
 * @def AF_NETBEUI
 * @brief Alias to PF_NETBEUI.
 */
#define AF_NETBEUI PF_NETBEUI

/**
 * @def AF_SECURITY
 * @brief PF_SECURITY.
 */
#define AF_SECURITY PF_SECURITY

/**
 * @def AF_KEY
 * @brief Alias to PF_KEY.
 */
#define AF_KEY PF_KEY

/**
 * @def AF_NETLINK
 * @brief Alias to PF_NETLINK.
 */
#define AF_NETLINK PF_NETLINK

/**
 * @def AF_ROUTE
 * @brief Alias to PF_ROUTE.
 */
#define AF_ROUTE PF_ROUTE

/**
 * @def AF_PACKET
 * @brief Alias to PF_PACKET.
 */
#define AF_PACKET PF_PACKET

/**
 * @def AF_ASH
 * @brief Alias to PF_ASH.
 */
#define AF_ASH PF_ASH

/**
 * @def AF_ECONET
 * @brief Alias to PF_ECONET.
 */
#define AF_ECONET PF_ECONET

/**
 * @def AF_ATMSVC
 * @brief Alias to PF_ATMSVC.
 */
#define AF_ATMSVC PF_ATMSVC

/**
 * @def AF_RDS
 * @brief Alias to PF_RDS.
 */
#define AF_RDS PF_RDS

/**
 * @def AF_SNA
 * @brief Alias to PF_SNA.
 */
#define AF_SNA PF_SNA

/**
 * @def AF_IRDA
 * @brief Alias to PF_IRDA.
 */
#define AF_IRDA PF_IRDA

/**
 * @def AF_PPPOX
 * @brief Alias to PF_PPPOX.
 */
#define AF_PPPOX PF_PPPOX

/**
 * @def AF_WANPIPE
 * @brief Alias to PF_WANPIPE.
 */
#define AF_WANPIPE PF_WANPIPE

/**
 * @def AF_LLC
 * @brief Alias to PF_LLC.
 */
#define AF_LLC PF_LLC

/**
 * @def AF_IB
 * @brief Alias to PF_IB.
 */
#define AF_IB PF_IB

/**
 * @def AF_MPLS
 * @brief Alias to PF_MPLS.
 */
#define AF_MPLS PF_MPLS

/**
 * @def AF_CAN
 * @brief Alias to PF_CAN.
 */
#define AF_CAN PF_CAN

/**
 * @def AF_TIPC
 * @brief Alias to PF_TIPC.
 */
#define AF_TIPC PF_TIPC

/**
 * @def AF_BLUETOOTH
 * @brief Alias to PF_BLUETOOTH.
 */
#define AF_BLUETOOTH PF_BLUETOOTH

/**
 * @def AF_IUCV
 * @brief Alias to PF_IUCV.
 */
#define AF_IUCV PF_IUCV

/**
 * @def AF_RXRPC
 * @brief Alias to PF_RXRPC.
 */
#define AF_RXRPC PF_RXRPC

/**
 * @def AF_ISDN
 * @brief Alias to PF_ISDN.
 */
#define AF_ISDN PF_ISDN

/**
 * @def AF_PHONET
 * @brief Alias to PF_PHONET.
 */
#define AF_PHONET PF_PHONET

/**
 * @def AF_IEEE802154
 * @brief Alias to PF_IEEE802154.
 */
#define AF_IEEE802154 PF_IEEE802154

/**
 * @def AF_CAIF
 * @brief Alias to PF_CAIF.
 */
#define AF_CAIF PF_CAIF

/**
 * @def AF_ALG
 * @brief Alias to PF_ALG.
 */
#define AF_ALG PF_ALG

/**
 * @def AF_NFC
 * @brief Alias to PF_NFC.
 */
#define AF_NFC PF_NFC

/**
 * @def AF_VSOCK
 * @brief Alias to PF_VSOCK.
 */
#define AF_VSOCK PF_VSOCK

/**
 * @def AF_KCM
 * @brief Alias to PF_KCM.
 */
#define AF_KCM PF_KCM

/**
 * @def AF_QIPCRTR
 * @brief Alias to PF_QIPCRTR.
 */
#define AF_QIPCRTR PF_QIPCRTR

/**
 * @def AF_SMC
 * @brief Alias to PF_SMC.
 */
#define AF_SMC PF_SMC

/**
 * @def AF_MAX
 * @brief Alias to PF_MAX.
 */
#define AF_MAX PF_MAX

/* Socket level values.
 * Others are defined in the appropriate headers.
 *
 * XXX: These definitions also should go into the appropriate headers as
 * far as they are available.
 */

/**
 * @def SOL_RAW
 * @brief RAW socket option level.
 */
#define SOL_RAW 255

/**
 * @def SOL_DECNET
 * @brief DECNET socket option level.
 */
#define SOL_DECNET 261

/**
 * @def SOL_X25
 * @brief X25 socket option level.
 */
#define SOL_X25 262

/**
 * @def SOL_PACKET
 * @brief PACKET socket option level.
 */
#define SOL_PACKET 263

/**
 * @def SOL_ATM
 * @brief ATM layer (ceil level) socket option level.
 */
#define SOL_ATM 264

/**
 * @def SOL_AAL
 * @brief ATM Adaption Layer (packet level) socket option level.
 */
#define SOL_AAL 265

/**
 * @def SOL_IRDA
 * @brief IRDA socket option level.
 */
#define SOL_IRDA 266

/**
 * @def SOL_NETBEUI
 * @brief NETBEUI socket option level.
 */
#define SOL_NETBEUI 267

/**
 * @def SOL_LLC
 * @brief LLC socket option level.
 */
#define SOL_LLC 268

/**
 * @def SOL_DCCP
 * @brief DCCP socket option level.
 */
#define SOL_DCCP 269

/**
 * @def SOL_NETLINK
 * @brief NETLINK socket option level.
 */
#define SOL_NETLINK 270

/**
 * @def SOL_TIPC
 * @brief TIPC socket option level.
 */
#define SOL_TIPC 271

/**
 * @def SOL_RXRPC
 * @brief RXRPC socket option level.
 */
#define SOL_RXRPC 272

/**
 * @def SOL_PPPOL2TP
 * @brief PPPOL socket option level.
 */
#define SOL_PPPOL2TP 273

/**
 * @def SOL_BLUETOOTH
 * @brief BLUETOOTH socket option level.
 */
#define SOL_BLUETOOTH 274

/**
 * @def SOL_PNPIPE
 * @brief PNPIPE socket option level.
 */
#define SOL_PNPIPE 275

/**
 * @def SOL_RDS
 * @brief RDS socket option level.
 */
#define SOL_RDS 276

/**
 * @def SOL_IUCV
 * @brief IUCV socket option level.
 */
#define SOL_IUCV 277

/**
 * @def SOL_CAIF
 * @brief CAIF socket option level.
 */
#define SOL_CAIF 278

/**
 * @def SOL_ALG
 * @brief ALG socket option level.
 */
#define SOL_ALG 279

/**
 * @def SOL_NFC
 * @brief NFC socket option level.
 */
#define SOL_NFC 280

/**
 * @def SOL_KCM
 * @brief KCM socket option level.
 */
#define SOL_KCM 281

/**
 * @def SOL_TLS
 * @brief TLS socket option level.
 */
#define SOL_TLS 282

/**
 * @def SOMAXCONN
 * @brief Maximum queue length specifiable by listen.
 */
#define SOMAXCONN 128

/**
 * @enum MSG
 * @brief Bits in the FLAGS argument to `send', `recv', et al.
 */
enum MSG {
  MSG_OOB		= 0x01,	/* Process out-of-band data.  */
  MSG_PEEK		= 0x02,	/* Peek at incoming messages.  */
  MSG_DONTROUTE	= 0x04,	/* Don't use local routing.  */
  /* DECnet uses a different name.  */
  MSG_TRYHARD		= MSG_DONTROUTE,
  MSG_CTRUNC		= 0x08,	/* Control data lost before delivery.  */
  MSG_PROXY		= 0x10,	/* Supply or ask second address.  */
  MSG_TRUNC		= 0x20,
  MSG_DONTWAIT	= 0x40, /* Nonblocking IO.  */
  MSG_EOR		= 0x80, /* End of record.  */
  MSG_WAITALL		= 0x100, /* Wait for a full request.  */
  MSG_FIN		= 0x200,
  MSG_SYN		= 0x400,
  MSG_CONFIRM		= 0x800, /* Confirm path validity.  */
  MSG_RST		= 0x1000,
  MSG_ERRQUEUE	= 0x2000, /* Fetch message from error queue.  */
  MSG_NOSIGNAL	= 0x4000, /* Do not generate SIGPIPE.  */
  MSG_MORE		= 0x8000,  /* Sender will send more.  */
  MSG_WAITFORONE	= 0x10000, /* Wait for at least one packet to return.*/
  MSG_BATCH		= 0x40000, /* sendmmsg: more messages coming.  */
  MSG_ZEROCOPY	= 0x4000000, /* Use user data in kernel path.  */
  MSG_FASTOPEN	= 0x20000000, /* Send data in TCP SYN.  */
  MSG_CMSG_CLOEXEC	= 0x40000000	/* Set close_on_exit for file
					   descriptor received through
					   SCM_RIGHTS.  */
};

/**
 * @struct socket_struct
 * @brief Socket structure.
 */
struct socket_struct {
  /**
   * Used.
   */
  bool used;

  /**
   * Passive socket.
   */
  bool passive_socket;

  /**
   * Connect ID.
   */
  int connect_id;

  /**
   * Domain.
   */
  int domain;

  /**
   * Type.
   */
  int type;

  /**
   * Protocol.
   */
  int protocol;

  /**
   * Address of an AF_LOCAL (aka AF_UNIX) socket.
   */
  struct sockaddr_un addr;

  /**
   * Message.
   */
  ring_buf_t msg;
};

/**
 * @def NR_STDS
 * @brief Number of standards.
 */
#define NR_STDS 3

/**
 * @def MSG_BUFSIZE
 * @brief Message buffer size.
 */
#define MSG_BUFSIZE 1024

/**
 * @enum shutdown
 * @brief Shutdown for socket.
 */
enum shutdown {
  SHUT_RD = 0,
  SHUT_WR,
  SHUT_RDWR
};


/**
 * @fn int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
 * @brief is used with connection-based socket types (SOCK_STREAM, SOCK_SEQPACKET).
 * It extracts the first connection request on the queue of pending connections for
 * the listening socket, sockfd, creates a new connected socket, and returns a new file
 * descriptor referring to that socket.
 * The newly created socket is not in the listening state.
 * The original socket sockfd is unaffected by this call.
 *
 * @param sockfd Socket file descriptor.
 * @param addr Address.
 * @param addrlen Address length.
 * @return On success, these system calls return a nonnegative integer that is a file descriptor
 * for the accepted socket.
 * On error, -1 is returned, and errno is set appropriately.
 */
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

/**
 * @fn int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
 * @brief assign the address specified by addr to the socket referred to by
 * the file descriptor @a sockfd.
 * @a addrlen specifies the size, in bytes, of the address structure pointed to by @a addr.
 * Traditionally, this operation is called "assigning a name to a socket".
 *
 * @param sockfd Socket descriptor.
 * @param addr Address.
 * @param addrlen Address length.
 * @retrun On success, zero is returned.
 * On error, -1 is returned, and errno is set appropriately.
 */
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

/**
 * @fn int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
 * @brief connect the socket referred to by the file descriptor @a sockfd to
 * the address specified by @a addr.
 *
 * @param sockfd Socket descriptor.
 * @param addr Address.
 * @param addrlen Address length.
 * @retrun If the connection or binding succeeds, zero is returned.
 * On error, -1 is returned, and errno is set appropriately.
 */
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

/**
 * @fn int listen(int sockfd, int backlog)
 * @brief mark the socket referred to by @a sockfd as a passive socket, that is,
 * as a socket that will be used to accept incoming connection requests using accept().
 *
 * @param sockfd Socket file descriptor.
 * @param backlog Maximum length to which the queue of pending connections for @a sockfd.
 * @return On success, zero is returned.
 * On error, -1 is returned, and errno is set appropriately.
 */
int listen(int sockfd, int backlog);

/**
 * @fn int socket(int domain, int type, int protocol)
 * @brief create an endpoint for communication and returns a file descriptor that refers to
 * that endpoint.
 * The file descriptor returned by a successful call will be the lowest-numbered
 * file descriptor not currently open for the process.
 *
 * @param domain Domain.
 * @param type Type.
 * @param protocol Protocol.
 * @return On success, a file descriptor for the new socket is returned.
 * On error, -1 is returned, and errno is set appropriately.
 */
int socket(int domain, int type, int protocol);

/**
 * @fn int shutdown(int sockfd, int how)
 * @brief cause all or part of a full-duplex connection on the socket associated
 * with @a sockfd to be shut down.
 * If @a how is @c SHUT_RD, further receptions will be disallowed.
 * If @a how is @c SHUT_WR, further transmissions will be disallowed.
 * If @a how is @c SHUT_RDWR, further receptions and transmissions will be disallowed.
 *
 * @param sockfd Socket file descriptor.
 * @param how How to shut down.
 * @return On success, zero is returned.
 * On error, -1 is returned, and errno is set appropriately.
 */
int shutdown(int sockfd, int how);

/**
 * @fn ssize_t send(int sockfd, const void *buf, size_t len, int flags)
 * @brief is used only when the socket is in a connected  state (so that the
 * intended recipient is known).
 * The only difference between send() and write() is the presence of flags.
 * With a zero flags argument, send() is equivalent to write().
 *
 * @param sockfd Socket file descriptor.
 * @param buf Buffer.
 * @param len Length.
 * @param flags Flags.
 * @return On success, these calls return the number of bytes sent.
 * On error, -1 is returned, and errno is set appropriately.
 */
ssize_t send(int sockfd, const void *buf, size_t len, int flags);

/**
 * @fn ssize_t recv(int sockfd, void *buf, size_t len, int flags)
 * @brief is used to receive messages from a socket.
 * It may be used to receive data on both connectionless and connection-oriented sockets.
 * This page first describes common features of all three system calls, and then describes the
 * differences between the calls. The only difference between recv() and read() is
 * the presence of flags.
 * With a zero flags argument, recv() is generally equivalent to read().
 *
 * @param sockfd Socket file descriptor.
 * @param buf Buffer.
 * @param len Length.
 * @param flags Flags.
 * @return On success, these calls return the number of bytes sent.
 * On error, -1 is returned, and errno is set appropriately.
 */
ssize_t recv(int sockfd, void *buf, size_t len, int flags);

/**
 * @fn void init_socket(void)
 * @brief initialize socket.
 */
void init_socket(void);

/**
 * @var sockets[SOMAXCONN]
 * @brief Array of socket structures.
 */
extern struct socket_struct sockets[SOMAXCONN];


#endif /* CONFIG_ARCH_SIM */

#endif /* __ASSEMBLY__ */

#endif	/* __MCUBE_NET_SOCKET_H__ */
