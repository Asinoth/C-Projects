#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>

void sighup(); /* routines child will call upon sigtrap */
void sigint();
void sigquit();

main()
{	 int pid;

	/* get child process */
	while(1){
		
		signal(SIGHUP,sighup); /* set function calls */
		signal(SIGTSTP,sigint);
		signal(SIGQUIT, sigquit);
		for(;;); /* loop for ever */
		printf("still here child");
		
		if ((pid = fork()) < 0) {
			perror("fork");
			exit(1);
		}
    
		if (pid == 0)
		{ /* child */
			
			 /* pid hold id of child */
			printf("\nPARENT: sending SIGHUP\n\n");
			kill(pid,SIGHUP);
			sleep(3); /* pause for 3 secs */
			printf("\nPARENT: sending SIGTSTP\n\n");
			kill(pid,SIGTSTP);
			sleep(3); /* pause for 3 secs */
			printf("\nPARENT: sending SIGQUIT\n\n");
			kill(pid,SIGQUIT);
			sleep(3);
			printf("still here father");
			
			
		}
		else /* parent */
		{ 	
			printf("still here child");
		}
		
		printf("still here both");
	}
}
void sighup()

{  signal(SIGHUP,sighup); /* reset signal */
   printf("CHILD: I have received a SIGHUP\n");
}

void sigint()

{  signal(SIGTSTP,sigint); /* reset signal */
   printf("CHILD: I have received a SIGINT\n");
}

void sigquit()

{ printf("My DADDY has Killed me!!!\n");
  exit(0);
}