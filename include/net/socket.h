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

typedef unsigned short sa_family_t;
typedef int socklen_t;

/*
 *      1003.1g requires sa_family_t and that sa_data is char.
 */

/**
 * @brief Socket address
 *
 * The sockaddr structure has socket address information.
 */
struct sockaddr {
  /** address family, AF_xxx. */
  sa_family_t sa_family;
  /** 14 bytes of protocol address. */
  char sa_data[14];
};

typedef struct sockaddr sockaddr;

/**
 * @brief Internet address
 *
 * The in_addr structure has definition of internet address.
 */
struct in_addr {
  /** Socket in address. */
  unsigned long s_addr;
};

typedef struct in_addr in_addr;

/**
 * @brief Socket address
 *
 * The sockaddr_in structure has internet socket address.
 */
struct sockaddr_in {
  /** Address family. */
  short sin_family;
  /** Port number. */
  unsigned short sin_port;
  /** Internet address. */
  struct in_addr sin_addr;
  /** Padding. */
  char sin_zero[8];
};

typedef struct sockaddr_in sockaddr_in;

/* Types of sockets.  */
enum socket_type {
  SOCK_STREAM = 1,              /* Sequenced, reliable, connection-based
																	 byte streams.  */
#define SOCK_STREAM SOCK_STREAM
  SOCK_DGRAM = 2,               /* Connectionless, unreliable datagrams
																	 of fixed maximum length.  */
#define SOCK_DGRAM SOCK_DGRAM
  SOCK_RAW = 3,                 /* Raw protocol interface.  */
#define SOCK_RAW SOCK_RAW
  SOCK_RDM = 4,                 /* Reliably-delivered messages.  */
#define SOCK_RDM SOCK_RDM
  SOCK_SEQPACKET = 5,           /* Sequenced, reliable, connection-based,
																	 datagrams of fixed maximum length.  */
#define SOCK_SEQPACKET SOCK_SEQPACKET
  SOCK_DCCP = 6,                /* Datagram Congestion Control Protocol.  */
#define SOCK_DCCP SOCK_DCCP
  SOCK_PACKET = 10,             /* Linux specific way of getting packets
																		 at the dev level.  For writing rarp and
																		 other similar things on the user level. */
#define SOCK_PACKET SOCK_PACKET

  /* Flags to be ORed into the type parameter of socket and socketpair and
  	 used for the flags parameter of paccept.  */

  SOCK_CLOEXEC = 02000000,      /* Atomically set close-on-exec flag for the
																	 new descriptor(s).  */
#define SOCK_CLOEXEC SOCK_CLOEXEC
  SOCK_NONBLOCK = 04000         /* Atomically mark descriptor(s) as
                                   non-blocking.  */
#define SOCK_NONBLOCK SOCK_NONBLOCK
};

typedef enum socket_type socket_type;

/* Protocol families.  */
#define PF_UNSPEC       0       /* Unspecified.  */
#define PF_LOCAL        1       /* Local to host (pipes and file-domain).  */
#define PF_UNIX         PF_LOCAL /* POSIX name for PF_LOCAL.  */
#define PF_FILE         PF_LOCAL /* Another non-standard name for PF_LOCAL.  */
#define PF_INET         2       /* IP protocol family.  */
#define PF_AX25         3       /* Amateur Radio AX.25.  */
#define PF_IPX          4       /* Novell Internet Protocol.  */
#define PF_APPLETALK    5       /* Appletalk DDP.  */
#define PF_NETROM       6       /* Amateur radio NetROM.  */
#define PF_BRIDGE       7       /* Multiprotocol bridge.  */
#define PF_ATMPVC       8       /* ATM PVCs.  */
#define PF_X25          9       /* Reserved for X.25 project.  */
#define PF_INET6        10      /* IP version 6.  */
#define PF_ROSE         11      /* Amateur Radio X.25 PLP.  */
#define PF_DECnet       12      /* Reserved for DECnet project.  */
#define PF_NETBEUI      13      /* Reserved for 802.2LLC project.  */
#define PF_SECURITY     14      /* Security callback pseudo AF.  */
#define PF_KEY          15      /* PF_KEY key management API.  */
#define PF_NETLINK      16
#define PF_ROUTE        PF_NETLINK /* Alias to emulate 4.4BSD.  */
#define PF_PACKET       17      /* Packet family.  */
#define PF_ASH          18      /* Ash.  */
#define PF_ECONET       19      /* Acorn Econet.  */
#define PF_ATMSVC       20      /* ATM SVCs.  */
#define PF_RDS          21      /* RDS sockets.  */
#define PF_SNA          22      /* Linux SNA Project */
#define PF_IRDA         23      /* IRDA sockets.  */
#define PF_PPPOX        24      /* PPPoX sockets.  */
#define PF_WANPIPE      25      /* Wanpipe API sockets.  */
#define PF_LLC          26      /* Linux LLC.  */
#define PF_CAN          29      /* Controller Area Network.  */
#define PF_TIPC         30      /* TIPC sockets.  */
#define PF_BLUETOOTH    31      /* Bluetooth sockets.  */
#define PF_IUCV         32      /* IUCV sockets.  */
#define PF_RXRPC        33      /* RxRPC sockets.  */
#define PF_ISDN         34      /* mISDN sockets.  */
#define PF_PHONET       35      /* Phonet sockets.  */
#define PF_IEEE802154   36      /* IEEE 802.15.4 sockets.  */
#define PF_MAX          37      /* For now..  */


extern int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
extern int bind(int sockfd, const struct sockaddr *my_addr, socklen_t addrlen);
extern int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
extern int listen(int sockfd, int backlog);
extern int socket(int domain, int type, int protocol);


#endif /* CONFIG_ARCH_SIM */

#endif /* __ASSEMBLY__ */

#endif	/* __MCUBE_NET_SOCKET_H__ */
