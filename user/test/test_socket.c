/**
 * @file user/test/test_socket.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

#define UNIXDOMAIN_PATH "/tmp/server.sock"
#define MESSAGE "hello world"
#define SOCK_BUFSIZE 256


static int socket_client(__unused int argc, __unused char *argv[])
{
  struct sockaddr_un addr;
  int srvfd;

  if ((srvfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
    printk("Error: socket()\n");
    return 1;
  }

  printk("srvfd = %d\n", srvfd);

  memset(&addr, 0, sizeof(struct sockaddr_un));
  addr.sun_family = AF_UNIX;
  strcpy(addr.sun_path, UNIXDOMAIN_PATH);

  if (connect(srvfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) < 0) {
    printk("Error: connect()\n");
    goto end_socket;
  }

  if (write(srvfd, MESSAGE, strlen(MESSAGE)) < 0) {
    printk("Error: write()\n");
    goto end_socket;
  }

end_socket:

  if (shutdown(srvfd, SHUT_RDWR) == -1) {
    printk("Error: shutdown() for socket()\n");
    return 2;
  }

  return 0;
}

static int socket_server(__unused int argc, __unused char *argv[])
{
  int clifd, lsnfd;
  struct sockaddr_un cliaddr, srvaddr;
  char recvbuf[SOCK_BUFSIZE] = "";
  socklen_t addrlen;
  int len;

  if ((lsnfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
    printk("Error: socket\n");
    return 1;
  }

  /* remove UNIXDOMAIN_PATH if it exists. */
  unlink(UNIXDOMAIN_PATH);

  memset(&srvaddr, 0, sizeof(struct sockaddr_un));
  srvaddr.sun_family = AF_UNIX;
  strcpy(srvaddr.sun_path, UNIXDOMAIN_PATH);

  if (bind(lsnfd, (struct sockaddr *) &srvaddr, sizeof(struct sockaddr_un)) < 0) {
    printk("Error: bind()\n");
    goto end_socket;
  }

  printk("listen()\n");

  if (listen(lsnfd, 1024) < 0) {
    printk("Error: listen()\n");
    goto end_socket;
  }

  printk("accept()\n");

  if ((clifd = accept(lsnfd, (struct sockaddr *) &cliaddr, &addrlen)) < 0) {
    printk("Error: accept()\n");
    goto end_socket;
  }

  printk("clifd = %d\n", clifd);
  printk("read()\n");

  if ((len = read(clifd, recvbuf, sizeof(recvbuf))) == -1) {
    printk("Error: read()\n");
    goto end_accept;
  }

  recvbuf[len] = 0;
  printk("%s\n", recvbuf);

end_accept:

  if (shutdown(clifd, SHUT_RDWR) == -1) {
    printk("Error: shutdown() for accept()\n");
  }

end_socket:

  if (shutdown(lsnfd, SHUT_RDWR) == -1) {
    printk("Error: shutdown() for socket()\n");
    return 2;
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
