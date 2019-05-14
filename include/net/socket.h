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

/* Structure describing the address of an AF_LOCAL (aka AF_UNIX) socket.  */
struct sockaddr_un {
  sa_family_t sun_family;
  char sun_path[108];         /* Path name.  */
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
} __packed;

typedef struct sockaddr_in sockaddr_in;

/* Types of sockets.  */
enum socket_type {
  SOCK_STREAM = 1,              /* Sequenced, reliable, connection-based
																	 byte streams.  */
  SOCK_DGRAM = 2,               /* Connectionless, unreliable datagrams
																	 of fixed maximum length.  */
  SOCK_RAW = 3,                 /* Raw protocol interface.  */
  SOCK_RDM = 4,                 /* Reliably-delivered messages.  */
  SOCK_SEQPACKET = 5,           /* Sequenced, reliable, connection-based,
																	 datagrams of fixed maximum length.  */
  SOCK_DCCP = 6,                /* Datagram Congestion Control Protocol.  */
  SOCK_PACKET = 10,             /* Linux specific way of getting packets
                                   at the dev level.  For writing rarp and
                                   other similar things on the user level. */

  /* Flags to be ORed into the type parameter of socket and socketpair and
  	 used for the flags parameter of paccept.  */

  SOCK_CLOEXEC = 02000000,      /* Atomically set close-on-exec flag for the
																	 new descriptor(s).  */
  SOCK_NONBLOCK = 04000         /* Atomically mark descriptor(s) as
                                   non-blocking.  */
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

/* Address families.  */
#define AF_UNSPEC       PF_UNSPEC
#define AF_LOCAL        PF_LOCAL
#define AF_UNIX         PF_UNIX
#define AF_FILE         PF_FILE
#define AF_INET         PF_INET
#define AF_AX25         PF_AX25
#define AF_IPX          PF_IPX
#define AF_APPLETALK    PF_APPLETALK
#define AF_NETROM       PF_NETROM
#define AF_BRIDGE       PF_BRIDGE
#define AF_ATMPVC       PF_ATMPVC
#define AF_X25          PF_X25
#define AF_INET6        PF_INET6
#define AF_ROSE         PF_ROSE
#define AF_DECnet       PF_DECnet
#define AF_NETBEUI      PF_NETBEUI
#define AF_SECURITY     PF_SECURITY
#define AF_KEY          PF_KEY
#define AF_NETLINK      PF_NETLINK
#define AF_ROUTE        PF_ROUTE
#define AF_PACKET       PF_PACKET
#define AF_ASH          PF_ASH
#define AF_ECONET       PF_ECONET
#define AF_ATMSVC       PF_ATMSVC
#define AF_RDS          PF_RDS
#define AF_SNA          PF_SNA
#define AF_IRDA         PF_IRDA
#define AF_PPPOX        PF_PPPOX
#define AF_WANPIPE      PF_WANPIPE
#define AF_LLC          PF_LLC
#define AF_IB           PF_IB
#define AF_MPLS         PF_MPLS
#define AF_CAN          PF_CAN
#define AF_TIPC         PF_TIPC
#define AF_BLUETOOTH    PF_BLUETOOTH
#define AF_IUCV         PF_IUCV
#define AF_RXRPC        PF_RXRPC
#define AF_ISDN         PF_ISDN
#define AF_PHONET       PF_PHONET
#define AF_IEEE802154   PF_IEEE802154
#define AF_CAIF         PF_CAIF
#define AF_ALG          PF_ALG
#define AF_NFC          PF_NFC
#define AF_VSOCK        PF_VSOCK
#define AF_KCM          PF_KCM
#define AF_QIPCRTR      PF_QIPCRTR
#define AF_SMC          PF_SMC
#define AF_MAX          PF_MAX

/* Socket level values.  Others are defined in the appropriate headers.

   XXX These definitions also should go into the appropriate headers as
   far as they are available.  */
#define SOL_RAW         255
#define SOL_DECNET      261
#define SOL_X25         262
#define SOL_PACKET      263
#define SOL_ATM         264     /* ATM layer (cell level).  */
#define SOL_AAL         265     /* ATM Adaption Layer (packet level).  */
#define SOL_IRDA        266
#define SOL_NETBEUI     267
#define SOL_LLC         268
#define SOL_DCCP        269
#define SOL_NETLINK     270
#define SOL_TIPC        271
#define SOL_RXRPC       272
#define SOL_PPPOL2TP    273
#define SOL_BLUETOOTH   274
#define SOL_PNPIPE      275
#define SOL_RDS         276
#define SOL_IUCV        277
#define SOL_CAIF        278
#define SOL_ALG         279
#define SOL_NFC         280
#define SOL_KCM         281
#define SOL_TLS         282

/* Maximum queue length specifiable by listen.  */
#define SOMAXCONN       128

struct socket_struct {
  bool used;
  bool passive_socket;
  int connect_id;
  struct sockaddr_un addr;
  char *sendmsg;
  char *recvmsg;
};

#define NR_STDS 3

enum shutdown {
  SHUT_RD = 0,
  SHUT_WR,
  SHUT_RDWR
};

int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int bind(int sockfd, const struct sockaddr *my_addr, socklen_t addrlen);
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int listen(int sockfd, int backlog);
int socket(int domain, int type, int protocol);
int shutdown(int sockfd, int how);
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
ssize_t recv(int sockfd, void *buf, size_t len, int flags);

void init_socket(void);



#endif /* CONFIG_ARCH_SIM */

#endif /* __ASSEMBLY__ */

#endif	/* __MCUBE_NET_SOCKET_H__ */
