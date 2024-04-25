#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_types/_size_t.h>
#include <sys/_types/_ssize_t.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 2048
#define HOSTNAME "telehack.com"
#define PORT 23

char *
ip_by_hostname(const char *host) {
  struct hostent *he;
  struct in_addr **addr_list;

  if((he = gethostbyname(host)) == NULL) {
    perror("gethostbyname");
    return NULL;
  }

  addr_list = (struct in_addr **) he->h_addr_list;
  for(int i = 0; addr_list[i] != NULL; i++) {
    char *h = inet_ntoa(*addr_list[i]);
    if(h != NULL) {
      return h;
    }
  }

  return NULL;
}

int
main(int argc, char *argv[]) {
  if(argc != 3) {
    perror("./ascii-cli <font> <text>");
    return EXIT_FAILURE;
  }

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    perror("socket");
    return EXIT_FAILURE;
  }

  struct sockaddr_in serv_addr = {0};
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  char *addr = ip_by_hostname(HOSTNAME);
  if(addr == NULL) {
    perror("ip_by_hostname");
    close(sockfd);
    return EXIT_FAILURE;
  }

  if(inet_pton(PF_INET, addr, &serv_addr.sin_addr) < 0) {
    perror("inet_pton");
    close(sockfd);
    return EXIT_FAILURE;
  }

  if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    perror("connect");
    close(sockfd);
    return EXIT_FAILURE;
  }

  char buff[BUF_SIZE];
  memset(buff, 0, sizeof(buff));

  ssize_t numBytes;
  do {
    numBytes = recv(sockfd, buff, BUF_SIZE - 1, 0);

    if(numBytes == -1) {
      perror("recv");
      close(sockfd);
      return EXIT_FAILURE;
    }

    buff[numBytes] = '\0';

  } while(numBytes > 0 && buff[numBytes - 2] != '\n'
	  && buff[numBytes - 1] != '.');

  memset(buff, 0, sizeof(buff));

  if(snprintf(buff, BUF_SIZE, "figlet /%s %s\r\n", argv[1], argv[2]) <= 0) {
    perror("message error");
    close(sockfd);
    return EXIT_FAILURE;
  }

  if(send(sockfd, buff, BUF_SIZE, 0) < 0) {
    perror("send");
    close(sockfd);
    return EXIT_FAILURE;
  }

  memset(buff, 0, sizeof(buff));

  size_t len = 0;
  ssize_t r = 0;

  while((r = recv(sockfd, &buff[len], BUF_SIZE - len, 0)) > 0) {
    len += r;
  }

  buff[len] = '\0';
  printf("%s", buff);

  shutdown(sockfd, SHUT_RDWR);
  close(sockfd);

  return EXIT_SUCCESS;
}
