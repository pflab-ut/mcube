/**
 * @file user/test/test_socket.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

#define UNIXDOMAIN_PATH "/tmp/server.sock"
#define MESSAGE "hello world"
#define SOCKET_BUFSIZE 256


static int socket_client(__unused int argc, __unused char *argv[])
{
  struct sockaddr_un addr;
  int srvfd;

  if ((srvfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
    printk("Error: socket()\n");
    return 1;
  }

  memset(&addr, 0, sizeof(struct sockaddr_un));
  addr.sun_family = AF_UNIX;
  strcpy(addr.sun_path, UNIXDOMAIN_PATH);

  if (connect(srvfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) < 0) {
    printk("Error: connect()\n");
    return 2;
  }

  if (write(srvfd, MESSAGE, strlen(MESSAGE)) < 0) {
    printk("Error: write()\n");
    return 3;
  }

  if (shutdown(srvfd, SHUT_RDWR) == -1) {
    printk("Error: shutdown()\n");
  }

  return 0;
}

static int socket_server(__unused int argc, __unused char *argv[])
{
  int clifd, lsnfd;
  struct sockaddr_un cliaddr, srvaddr;
  struct pollfd fds[] = {{0}};
  char recvbuf[SOCKET_BUFSIZE] = "";
  socklen_t addrlen;
  int len;

  if ((lsnfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
    printk("Error: socket\n");
    return 1;
  }

  printk("lsnfd = %d\n", lsnfd);

  unlink(UNIXDOMAIN_PATH);
  memset(&srvaddr, 0, sizeof(struct sockaddr_un));
  srvaddr.sun_family = AF_UNIX;
  strcpy(srvaddr.sun_path, UNIXDOMAIN_PATH);

  if (bind(lsnfd, (struct sockaddr *) &srvaddr, sizeof(struct sockaddr_un)) < 0) {
    printk("Error: bind()\n");
    return 2;
  }

  if (listen(lsnfd, 5) < 0) {
    printk("Error: listen()\n");
    return 3;
  }

  fds[0].fd = lsnfd;
  fds[0].events = POLLIN;

  poll(fds, 1, -1);

  if (fds[0].revents & POLLIN) {
    memset(&cliaddr, 0, sizeof(struct sockaddr_un));
    addrlen = sizeof(struct sockaddr_un);

    if ((clifd = accept(lsnfd, (struct sockaddr *) &cliaddr, &addrlen)) < 0) {
      printk("Error: accept()\n");
      return 4;
    }

    if ((len = read(clifd, recvbuf, sizeof(recvbuf))) == -1) {
      printk("Error: read()\n");
    }

    recvbuf[len] = 0;
    printk("%s\n", recvbuf);
  } else {
    printk("Error: POLLIN\n");
    return 5;
  }

  if (shutdown(clifd, SHUT_RDWR) == -1) {
    printk("Error: shutdown()\n");
  }

  if (shutdown(lsnfd, SHUT_RDWR) == -1) {
    printk("Error: shutdown()\n");
  }

  return 0;
}


static void print_socket_usage(char *exec)
{
  printk("Usage: %s server[client]\n", exec);
}

bool test_socket(int argc, char *argv[])
{
  for (int i = 0; i < argc; i++) {
    printk("argv[%d] = %s\n", i, argv[i]);
  }

  if (argc != 2) {
    print_socket_usage(argv[0]);
    return false;
  } else {
    if (strcmp(argv[1], "client") == 0) {
      socket_client(argc, argv);
    } else if (strcmp(argv[1], "server") == 0) {
      socket_server(argc, argv);
    } else {
      print_socket_usage(argv[0]);
      return false;
    }
  }

  return true;
}
