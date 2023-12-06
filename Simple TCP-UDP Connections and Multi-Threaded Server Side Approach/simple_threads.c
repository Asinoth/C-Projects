#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <pthread.h>


struct thread_param {
  int num;
  char *str;
};

/*
 * At this example you will notice,
 * the prints have a strange behaviour.
 * This is due to the preemption.
 * Try to use a pthread mutex,
 * in order the messages from the
 * printf to be normally printed.
 */

void *print_msg_1(void *param){
  struct thread_param *cast = (struct thread_param *)param;
  while(1){
    printf("%s %d\n", cast->str, cast->num);
    sleep(1);
  }
  pthread_exit(NULL);
}

void *print_msg_2(void *param){
  struct thread_param *cast = (struct thread_param *)param;
  while(1){
    printf("%s %d\n", cast->str, cast->num);
    sleep(2);
  }
  pthread_exit(NULL);
}

int
main(int argc, char **argv){
  int param;
  
  pthread_t thread1;
  pthread_t thread2;
  
  pthread_attr_t thread_1_attributes;
  pthread_attr_t thread_2_attributes;
  /* Initialize the attributes of the threads */
  pthread_attr_init(&thread_1_attributes);
  pthread_attr_init(&thread_2_attributes);
  /*Set the detache state to JOINABLE*/
  pthread_attr_setdetachstate(&thread_1_attributes, PTHREAD_CREATE_JOINABLE);
  pthread_attr_setdetachstate(&thread_2_attributes, PTHREAD_CREATE_JOINABLE);
  
  struct thread_param param1;
  struct thread_param param2;
  
  param1.num = 1;
  param1.str = "Printing from thread ";
  param2.num = 2;
  param2.str = "Printing from thread ";
  
  if( pthread_create(&thread1, &thread_1_attributes, &print_msg_1, (void *)&param1) != 0){
    perror("create thread 1");
    exit(EXIT_FAILURE);
  }
  if( pthread_create(&thread2, &thread_2_attributes, &print_msg_2, (void *)&param2) != 0){
    perror("create thread 2");
    exit(EXIT_FAILURE);
  }
  pause();
  return 1;
}

