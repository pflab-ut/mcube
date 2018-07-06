/**
 * @file drivers/net/socket.c
 *
 * @author Hiroyuki Chishiro
 */
#include <est/est.h>
#include <arch/arch.h>
#include <net/net.h>
#include <debug/debug.h>

#if CONFIG_ARCH_SIM


int kaccept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
	return accept(sockfd, addr, addrlen);
}

int kbind(int sockfd, const struct sockaddr *my_addr, socklen_t addrlen)
{
	return bind(sockfd, my_addr, addrlen);
}

int kconnect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	return connect(sockfd, addr, addrlen);
}


int klisten(int sockfd, int backlog)
{
	return listen(sockfd, backlog);
}

int ksocket(int domain, int type, int protocol)
{
	return socket(domain, type, protocol);
}



#else

int kaccept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
	return 0;
}

int kbind(int sockfd, const struct sockaddr *my_addr, socklen_t addrlen)
{
	return 0;
}

int kconnect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	return 0;
}


int klisten(int sockfd, int backlog)
{
	return 0;
}

int ksocket(int domain, int type, int protocol)
{
	return 0;
}

#endif /* CONFIG_ARCH_SIM */

