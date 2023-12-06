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
  
  unsigned int i = 0;
  
  struct sockaddr *client_addr;
  socklen_t client_addr_len;

  if((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
    perror("opening UDP socket");
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
    perror("udp connect");
    exit(EXIT_FAILURE);
  }

  
  while(i < 10){
    printf("Look me, look me I do not block!!!\n");
    if( sendto(sock, "Hello Server!", 14, 0, (struct sockaddr *)&sin, sizeof(struct sockaddr)) == -1){
	perror("send status report");
	exit(EXIT_FAILURE);
    }
    i++;
    sleep(1);
  }
  close(sock);
  return 1;
}
 
