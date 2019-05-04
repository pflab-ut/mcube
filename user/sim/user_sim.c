/**
 * @file user/sim/user_sim.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

int ap_main(void)
{
  unsigned long cpu = get_cpu_id();
  putchar(cpu + '0');
  print("ap_main()\n");
  print("get_cpu_id() = %lu\n", get_cpu_id());
  //  print("ap_main()\n");
  //  cpu_ids[cpu] = cpu;
  return 0;
}


#define PORT 49152
#define SOCKET_BUFSIZE 256

int socket_client(int argc, char *argv[])
{
  int s;
  struct sockaddr_in addr;
  struct hostent *hp;
  char buf[SOCKET_BUFSIZE];
  int ret;

  if (argc < 2) {
    printf("%s SERVER\n", argv[0]);
    exit(1);
  }

  if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(2);
  }

  memset((char *) &addr, 0, sizeof(addr));

  if ((hp = gethostbyname(argv[2])) == NULL) {
    perror("gethostbyname");
    exit(3);
  }

  bcopy(hp->h_addr, &addr.sin_addr, hp->h_length);
  addr.sin_family = AF_INET;
  addr.sin_port   = htons(PORT);

  if (connect(s, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
    perror("connect");
    exit(4);
  }

  if ((ret = read(s, buf, sizeof(buf))) == -1) {
    perror("read");
    exit(5);
  }

  printf("%s", buf);

  close(s);
  return 0;
}

int socket_server(__unused int argc, __unused char *argv[])
{
  int s1, s2;
  int ret;
  socklen_t len;
  struct sockaddr_in saddr, caddr;
  char buf[SOCKET_BUFSIZE];

  if ((s1 = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(1);
  }

  memset((char *) &saddr, 0, sizeof(saddr));
  saddr.sin_family      = AF_INET;
  saddr.sin_addr.s_addr = INADDR_ANY;
  saddr.sin_port        = htons(PORT);

  if (bind(s1, (struct sockaddr *) &saddr, sizeof(saddr)) < 0) {
    perror("bind");
    exit(2);
  }

  if (listen(s1, 1) < 0) {
    perror("listen");
    exit(3);
  }

  len = sizeof(caddr);

  if ((s2 = accept(s1, (struct sockaddr *) &caddr, &len)) < 0) {
    perror("accept");
    exit(4);
  }

  close(s1);

  strcpy(buf, "I'm a server.\n");

  if ((ret = write(s2, buf, sizeof(buf))) == -1) {
    perror("write");
    exit(5);
  }

  close(s2);

  return 0;
}

void print_socket_usage(char *exec)
{
  printk("Usage: %s server[client address]\n"
         "  server means socket server.\n"
         "  client means socket client.\n"
         "  address means network address (localhost, 127.0.0.1, etc.)\n", exec);
  exit(0);
}


int user_arch_main(__unused int argc, __unused char *argv[])
{
  for (int i = 0; i < argc; i++) {
    printk("argv[%d] = %s\n", i, argv[i]);
  }

  if (argc < 2) {
    print_socket_usage(argv[0]);
  } else {
    if (argc == 3 && strcmp(argv[1], "client") == 0) {
      socket_client(argc, argv);
    } else if (argc == 2 && strcmp(argv[1], "server") == 0) {
      socket_server(argc, argv);
    } else {
      print_socket_usage(argv[0]);
    }
  }

  return 0;
}
