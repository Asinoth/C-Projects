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


int 
main(int argc, char **argv){
  char buffer[512];
  
  int sock;
  int accepted;
  int received;
  
  struct sockaddr_in sin;

  struct sockaddr client_addr;
  socklen_t client_addr_len;

  
  if((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
    perror("opening UDP socket");
    exit(EXIT_FAILURE);
  }
  
  memset(&sin, 0, sizeof(struct sockaddr_in));
  sin.sin_family = AF_INET;
  sin.sin_port = htons(6886);
  /* Bind to all available network interfaces */
  sin.sin_addr.s_addr = INADDR_ANY;

  if(bind(sock, (struct sockaddr *)&sin, sizeof(struct sockaddr_in)) == -1){
    perror("UDP bind");
    exit(EXIT_FAILURE);
  }


  while(1){
    memset(buffer, 0, 512);
    if( (received = read(sock, buffer, 511)) == -1){
      perror("UDP read");
      exit(EXIT_FAILURE);
    }
    buffer[received] = 0;
    printf("Received: %s\n", buffer);
    printf("Going to sleep for 5 secs... Like a boss!\n");
    sleep(5);
  }
  return 1;
}
 
