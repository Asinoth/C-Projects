#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>

char **split_a_line(char *string,char *delim);
char *read_a_line(void);
int run_a_command(char** command,int wait);
int isDaemon(char **arr);
void print_command(char **command,pid_t child_pid);
char **RET_without_symbol(char **command_word);
void pipe_function(char **com_words, int pipes, int wait);

void signal_sent(int signum);

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

/*
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

int run_a_command(char** command_word,int wait){
	
	pid_t child_pid,wait_pid;
	int pid_status;
	
	if(wait == 1){
		command_word = RET_without_symbol(command_word);
	}
	
	child_pid = fork();
	
	if (child_pid != 0) {
		
		
		/*job done by Parent process*/
		
		if(wait == 1){
			printf("Background : \n");
			print_command(command_word,child_pid);
			wait_pid = waitpid(child_pid, &pid_status, WNOHANG);
		}else{
			wait_pid = waitpid(child_pid, &pid_status, WUNTRACED);
			printf("\n\nTELEIWSE TO PAIDI %d\n",wait_pid);
		}
		
	} else if (child_pid < 0) {
		/* Error in fork*/
		
		printf("\nError in forking\n");
		
	} else if(child_pid == 0){
		
		
		execvp(command_word[0],command_word);
		exit(EXIT_FAILURE);
		
	}
	
	return 1;
}

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
			
			execvp(tmp[i][0], tmp[i]);
		}else {
			if(i == pipes){
				close(pipefd[pipes-1][1]);
			}else if( i && (i != pipes) ) {
				close(pipefd[pipenum-1][1]);
			}
			if(wait) {
				
				printf("background process : %s\npid : %d\n", tmp[i][0], pid);
				waitpid(pid,&status,WNOHANG);
				
			}
			else {
				waitpid(pid, &status, WUNTRACED);
			}
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
