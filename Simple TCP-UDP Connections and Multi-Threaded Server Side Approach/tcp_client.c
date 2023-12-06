#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ifaddrs.h> 
#include <errno.h>
#include <sys/time.h>
#include <time.h>
#include <stddef.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h>

int main(int argc, char **argv){
  int sock;
  
  struct sockaddr *client_addr;
  socklen_t client_addr_len;

  if((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1){
    perror("opening TCP socket");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in sin;
  memset(&sin, 0, sizeof(struct sockaddr_in));
  sin.sin_family = AF_INET;
  /*Port that server listens at */
  sin.sin_port = htons(6886);
  /* The server's IP*/
  sin.sin_addr.s_addr = inet_addr("192.168.1.212");

  if(connect(sock, (struct sockaddr *)&sin, sizeof(struct sockaddr_in)) == -1){
    perror("tcp connect");
    exit(EXIT_FAILURE);
  }
  sleep(15);
  send(sock, "Hello Server!", 14, 0);
  close(sock);
  return 1;
}
