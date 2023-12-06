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
#include <pthread.h>


void *handle_tcp_connection(void *param){
  char buffer[512];
  int received;
  int sock = (int )param;
  printf("New connection accepted!\n");
    received = recv(sock, buffer, 511, 0);
    buffer[received] = 0;
    printf("Received from client: %s\n");
}

int 
main(int argc, char **argv){
  
  int sock;
  int accepted;
  int received;
  
  struct sockaddr_in sin;

  struct sockaddr client_addr;
  socklen_t client_addr_len;

  
  if((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1){
    perror("opening TCP socket");
    exit(EXIT_FAILURE);
  }
  
  memset(&sin, 0, sizeof(struct sockaddr_in));
  sin.sin_family = AF_INET;
  sin.sin_port = htons(6886);
  /* Bind to all available network interfaces */
  sin.sin_addr.s_addr = INADDR_ANY;

  if(bind(sock, (struct sockaddr *)&sin, sizeof(struct sockaddr_in)) == -1){
    perror("TCP bind");
    exit(EXIT_FAILURE);
  }

  if(listen(sock, 1000) == -1){
    perror("TCP listen");
    exit(EXIT_FAILURE);
  }

  /* Ok, a tricky part here. See man accept() for details */
  client_addr_len = sizeof(struct sockaddr);
  pthread_t *new_thread = (pthread_t *)malloc(sizeof(pthread_t));
  pthread_attr_t thread_attributes;
  /* Initialize the attributes of the threads */
  pthread_attr_init(&thread_attributes);
  /*Set the detache state to JOINABLE*/
  pthread_attr_setdetachstate(&thread_attributes, PTHREAD_CREATE_JOINABLE);
  
  while((accepted = accept(sock, &client_addr, &client_addr_len)) > 0 ){
    new_thread = (pthread_t *)malloc(sizeof(pthread_t));
    
    /*Create the thread and pass the socket discriptor*/
    if( pthread_create(new_thread, &thread_attributes, &handle_tcp_connection, (void *)accepted) != 0){
      perror("create thread");
      exit(EXIT_FAILURE);
    }
  }
  return 1;
}
