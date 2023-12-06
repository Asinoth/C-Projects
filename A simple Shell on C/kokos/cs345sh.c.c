
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>

typedef struct process{
	
	pid_t pid;
	char **cmd;
	int completed,status,stopped,number_processed;
	struct process *next;
}process;

struct process *first = NULL;
int number_processed = 1;

char **split_a_line(char *string,char *delim);
char *read_a_line(void);
void print_command(char **command,pid_t child_pid);
char **RET_without_symbol(char **command_word);
int toString(char *a);

int run_a_command(char** command,int wait);
int isDaemon(char **arr);
void pipe_function(char **com_words, int pipes, int wait);

void print_process_table();

void child(char ** command_word);
void parent(char **command_word,pid_t pid,int wait,int pid_status);

void signal_CHLD(int signum);
void signal_TSTP(int signum);
void signal_CONT(int signum);

void put_job_in_bg(int pid);
void put_job_in_fg(int pid);


int main(void){
	
	/*used for breaking the command*/
	char *command = NULL;
	char *buf_cwd = NULL;
	char **command_word = NULL;
	int is_deamon = 0;
	int run =1;
	int pipes_number = 0;
	char ***commands_to_pipe;
	
	while(run){
		printf("\n%s@cs345sh%s/#",getlogin(),getcwd(buf_cwd,0));
		command = read_a_line();
		command_word = split_a_line(command," \t\n");
		
		if(command_word[0] == NULL){
			
			continue;
		}
		
		if(strcmp(command_word[0],"cd") == 0){
			
			if(command_word[1] == NULL){
				printf("\n Warning :No action taken: cd needs to be followed by a directory name\n");
			}else{
				if(chdir(command_word[1]) != 0 ){
					printf("\nCannot change directory it maybe doesn't exists\n");
				}
			}
			
		}else if(strcmp(command_word[0],"exit") == 0) {
			
			return 0;
			
		}else if(strcmp(command_word[0],"csd_jobs") == 0) {
	
			print_process_table();
			
		}else if(strcmp(command_word[0],"csd_fg") == 0) {
	
			put_job_in_fg(toString(command_word[1]));
			
		}else if(strcmp(command_word[0],"csd_bg") == 0) {
	
			put_job_in_bg(toString(command_word[1]));
			
		}else{
			/*check for deamons*/
			is_deamon = isDaemon(command_word);
			pipes_number = Are_There_Pipes(command_word);
			
			if(pipes_number > 0){
				
				/*Create pipes*/
				pipe_function(command_word,pipes_number,is_deamon);
				
			}else{
				
				/*No pipes do fork and execyte command*/
				run =0;
				run = run_a_command(command_word,is_deamon);
				
			}
		}
		
		/*job done by both parent and child*/
		
		free(command_word);
		free(command);
	}
	
	return 0;
}

/*function that prints all rpocess in backround with pid and time of being proccesed*/
void print_process_table(){
	
	int i ;
	struct process *p = first;
	while(p !=NULL){
		
		
		printf("Process PID : %d \n\tTime : %d\n\n" ,p->pid,p->number_processed);
		p = p->next;
	}
	
}

/*function that deletes for the table of backround process*/
void delete_from_list(pid_t pid){
	
	process *p = first;
	process *tmp = first; 
	if(p == 0){
		printf("empty list");
		return;
	}else{
		if(tmp->pid == pid){
			first = NULL;
			return;
		}
		tmp = p->next;
		while(tmp!=0){
			if(tmp->pid == pid){
				p->next = tmp->next;
				tmp->next = NULL;
				return;
			}
			tmp = tmp->next;
			p = p->next;
		}
		
	}
}

/*tried to handle all signals together
void signal_sent(int signum){
	
	if(SIGTSTP == signum){
		
		printf("SIGTSTP signal caught aka Ctrl+z\n");
		printf("\n\tProcess ID :%d\n",child_pid);
		waitpid(child_pid, &pid_status, WNOHANG);		
		
	}else if(SIGCONT == signum){
		
		printf("SIGCONT signal caught\n");
		printf("\n\tProcess ID :%d\n",child_pid);
		waitpid(child_pid,&pid_status, WUNTRACED);
		
	}else if(SIGCHLD == signum){
		
		printf("SIGCHLD signal caught\n");
		printf("Childe Terminated %d\n",child_pid);
		waitpid(child_pid,&pid_status, WUNTRACED);
	}
	
}
*/

/*places a job in the fg to be continued*/
void put_job_in_fg(int pid){
	printf("sending signal SIGCONT to pid : %d",pid);
	kill(pid,SIGCONT);
}

/*places a job in the bg to be suspended*/
void put_job_in_bg(int pid){
	printf("sending signal SIGTSTP to pid : %d",pid);
	kill(pid,SIGTSTP);
}

/*signal caught when the child terminates normally*/
void signal_CHLD(int signum){
	pid_t pid,wait_pid;
	int pid_status;
	signal(SIGCHLD,signal_CHLD);
	printf("Caught a CHLD signal\n");
	wait_pid = waitpid(pid, &pid_status, WUNTRACED);
	
}

/*a signal sent via parent to suspend a process*/
void signal_TSTP(int signum){
	pid_t pid,wait_pid;
	int pid_status;
	signal(SIGTSTP,signal_TSTP);
	printf("Caught a TSTP signal\n");
	wait_pid = waitpid(pid, &pid_status, WNOHANG);
}

/*a signal sent via parent to continue a job that was suspended*/
void signal_CONT(int signum){
	pid_t pid,wait_pid;
	int pid_status;
	signal(SIGCONT,signal_CONT);
	printf("caught a CONT signal");
	wait_pid = waitpid(pid, &pid_status, WUNTRACED);
}	

/*what job has the child to do*/
void child(char **command_word){
	
	signal(SIGCHLD,signal_CHLD);
	sleep(5);
	execvp(command_word[0],command_word);
	exit(EXIT_FAILURE);
	
}

/*conditions and things parent must be aware of during child execution*/
void parent(char **command_word,pid_t pid,int wait,int pid_status){
	pid_t wait_pid;
	
	signal(SIGCONT,signal_CONT);
	signal(SIGCHLD,signal_CHLD);
	signal(SIGTSTP,signal_TSTP);
	
	if(wait){
		printf("Background : \n");
		print_command(command_word,pid);
		if(first == NULL){
			first = malloc(sizeof(struct process));
			first->next = 0;
			first->pid = pid;
			first->number_processed = number_processed;
			first->status = pid_status;
			
			number_processed++;
		}else{
			struct process *p = first;
			while(p->next !=NULL){
				p = p->next;
			}
			p->next = malloc(sizeof(struct process));
			p->next->next = 0;
			p->next->pid = pid;
			p->next->number_processed = number_processed;
			p->next->status = pid_status;
			
			number_processed++;
		}
		kill(pid,SIGTSTP);
		/*wait_pid = waitpid(pid, &pid_status, WNOHANG);  wnohang or tstp ?*/
		printf("Child Terminated %d\n",wait_pid);
		delete_from_list(wait_pid);
	}else{
		wait_pid = waitpid(pid, &pid_status, WUNTRACED);
		delete_from_list(wait_pid);
		printf("Child Terminated %d\n",wait_pid);
	}
	
	
}

/*tranforms a sting into integer (used for the csd_bg and csd_fg) and return its value*/
int toString(char *a) {
	int c, sign, offset, n;

	if (a[0] == '-') {  // Handle negative integers
		sign = -1;
	}

	if (sign == -1) {  // Set starting position to convert
		offset = 1;
	}else {
		offset = 0;
	}

	n = 0;

	for (c = offset; a[c] != '\0'; c++) {
		n = n * 10 + a[c] - '0';
	}

	if (sign == -1) {
		n = -n;
	}

	return n;
}

/*reads a line from the terminal and return it as an array of chars (string)*/
char *read_a_line(void){
	int command_size = 512;
	int position = 0;
	char *command = malloc(sizeof(char) * command_size);
	int c;
	
	if(!command){
		printf("error malloc in function read_a_line\n");
		exit(EXIT_FAILURE);
	}
	
	while (1){
		
		c = getchar();
		
		/*at the end of the command when reached we have \n or eof*/
		if(c == EOF || c == '\n'){
			command[position] = '\0';
			return command;
		}else{
			command[position] = c;
		}
		position++;
		
		if(position >= command_size){
			command_size += command_size;
			command = realloc(command,command_size);
			if(!command){
				printf("error realloc in reading a line\n");
				exit(EXIT_FAILURE);
			}
		}
	}
}

/*it splits the sentence given via terminal whenever it finds a space or tab and 
 *returns an array of strings null terminated*/
char **split_a_line(char *string,char *delim) {
	
	int string_size = 64,position =0;
	char **tokens = malloc(string_size * ( sizeof(char *) ));
	char *token;
	
	if(!tokens){
		printf("error malloc in function split_a_line\n");
		exit(EXIT_FAILURE);
	}
	
	token = strtok(string,delim);
	
	while(token != NULL) {
		
		tokens[position] = token;
		position++;
		
		if(position >= string_size){
			
			string_size += string_size;
			tokens = realloc(tokens,string_size * ( sizeof(char *) ));
			
			if(!tokens){
				printf("error malloc in function split_a_line\n");
				exit(EXIT_FAILURE);
			}
			
		}
		
		token = strtok(NULL, delim);
		
	}
	tokens[position] = NULL;
	
	return tokens;
}

/*checking is there are pipes in our command if there are the number of them is returned 0 otherwise*/
int Are_There_Pipes(char **command_word){
	int position =0;
	int position_word =0;
	int pipes = 0;
	
	while(command_word[position] != NULL){
		position_word = 0;
		while(command_word[position][position_word] != '\0'){
			
			if(command_word[position][position_word] == '|'){
				pipes++;
			}
			position_word++;
		}
		position++;
		
	}
	return pipes;
}

/*if a command the user use has the & then that command is a deamon 0 if it is 1 otherwise*/
int isDaemon(char **command_word){
	int position =0;
	int position_word =0;
	
	while(command_word[position] != NULL){
		position_word = 0;
		while(command_word[position][position_word] != '\0'){
			
			position_word++;
		}
		
		position++;
		
	}
	if(command_word[position-1][position_word-1] == '&'){
		return 1;
	}else{
		return 0;
	}
}

/*whenever we find a deamon we must take the real command without the '&' or without the pipe '|'*/
char **RET_without_symbol(char **command_word){
	int position =0;
	int position_word =0;
	
	while(command_word[position] != NULL){
		position_word = 0;
		while(command_word[position][position_word] != '\0'){
			
			position_word++;
		}
		position++;
		
	}
	if(command_word[position-1][position_word-1] == '&'){
		command_word[position-1][position_word-1] = '\0';
	}
	
	return command_word;
}

/*a fork is done and 2 process born the one controlled by father and the other by child 
 *each of them producing diferent stuff*/
int run_a_command(char** command_word,int wait){
	
	pid_t child_pid,wait_pid;
	int pid_status;
	signal(SIGCHLD,signal_CHLD);
	
	if(wait == 1){
		command_word = RET_without_symbol(command_word);
	}
	
	child_pid = fork();
	
	if (child_pid != 0) {
		
		
		/*job done by Parent process*/
		printf("Child with pid : %d\n",child_pid);
		parent(command_word,child_pid,wait,pid_status);
		
		
	} else if (child_pid < 0) {
		/* Error in fork*/
		
		printf("\nError in forking\n");
		
	} else if(child_pid == 0){
		
		child(command_word);
		
	}
	
	return 1;
}

/*it prints the command given by the user and the pid of that command*/
void print_command(char **command_word,pid_t child_pid){
	
	char ** this = command_word;
	int position =0;
	printf("\tProcess Name :");
	while(this[position] != NULL){
		printf("%s",this[position]);
		position++;
	}
	printf("\n\tProcess ID :%d",child_pid);
	
}

/*pipes are being produced here. its a pipeline of commands the user 
 *typed seperated by | the output of the least significcant most left command
 *passes to the right till the final command which will produce our output in the stdout*/
void pipe_function(char **com_words, int pipes, int wait){
	pid_t pid;
	int pipefd[pipes][2];
	int pipenum;
	int i, k, tmp_words;
	int word_index;
	char ***tmp = NULL;
	int status;
	char **tmp1;
	
	tmp = malloc((pipes + 1) * sizeof(char **));
	for(i = 0; i <= pipes; ++i) tmp[i] = NULL;
	
	i = 0;
	tmp_words = 0;
	for(k = 0; com_words[k]; ++k) {
		
		/*antika8istw ta | me null kai grafw sto tmp mia mia tis entoles*/
		if( strcmp(com_words[k], "|") ) {
			tmp1 = realloc(tmp[i], (tmp_words + 1) * sizeof(char *));
			tmp[i] = tmp1;
			tmp[i][tmp_words] = malloc(strlen(com_words[k]) + 1);
			strcpy(tmp[i][tmp_words], com_words[k]);
			++tmp_words;
		}else {
			tmp1 = realloc(tmp[i], (tmp_words + 1) * sizeof(char *));
			tmp[i] = tmp1;
			tmp[i][tmp_words] = NULL;
			++i;
			tmp_words = 0;
		}
		
	}
	tmp1 = realloc(tmp[i], (tmp_words + 1) * sizeof(char *));
	tmp[i] = tmp1;
	tmp[i][tmp_words] = NULL;
	
	
	for(i = 0; i < pipes; ++i) pipe(pipefd[i]);
	
	
	pipenum = 0;
	for(i = 0; i <= pipes; ++i) {
		if( i && (i != pipes) ) ++pipenum;
		
		pid = fork();
		if(pid == -1) puts("cannot create child process");
		else if(!pid) {
			if(!i) {
				/*first command to execute*/
				close(pipefd[0][0]);
				close(fileno(stdout));
				dup(pipefd[0][1]);
				
			}else if(i == pipes) {
				/*last command to execute*/
				close(pipefd[pipes-1][1]);
				close(fileno(stdin));
				dup(pipefd[pipes-1][0]);
				close(pipefd[pipes-1][0]);
			}else {
				/*intermediate command to execute*/
				close(pipefd[pipenum-1][1]);
				close(fileno(stdin)); 
				dup(pipefd[pipenum-1][0]);
				close(pipefd[pipenum][0]);
				close(fileno(stdout));
				dup(pipefd[pipenum][1]);
			}
			
			child(tmp[i]);
		}else {
			if(i == pipes){
				close(pipefd[pipes-1][1]);
			}else if( i && (i != pipes) ) {
				close(pipefd[pipenum-1][1]);
			}
			
			parent(tmp[i],pid,wait,status);
			
		}
	}
	
	for(i = 0; i < pipes; ++i) {
		close(pipefd[i][0]);
		close(pipefd[i][1]);
	}

	for(i = 0; i <= pipes; ++i) {
		for(k = 0; tmp[i][k]; ++k){
			free(tmp[i][k]);
		}
		free(tmp[i]);
	}
	free(tmp);
}
