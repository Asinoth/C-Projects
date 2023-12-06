

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/times.h>


char **get_command_words(void);
char **free_command_buffer(char **);
void work_with_pipes(char **, int, int);
void redirect_to_file(char **, int);
void redirect_append_to_file(char **, int);
void redirect_from_file(char **, int);

struct tms st_buf, end_buf;
int time = 0;
clock_t st_time, end_time;

int main(void)
{
	char *cwd_buf = NULL;
	char **com_words = NULL;
	char **tmp_buf = NULL;
	pid_t pid;
	int status;
	
	while(1) {
		printf("%s@csd_sh %s# ", getlogin(), getcwd(cwd_buf, 0));
		free(cwd_buf);

		com_words = get_command_words();

		
		if(com_words) {
			if( !strcmp(com_words[0], "csdTime") ) {
				if(!com_words[1]) {
						puts("command not found");
						goto end;
				}
				
				char **tmp1, *tmp2;
				int i;
				for(i = 0; ;++i) {
					if(!com_words[i+1]) {
						tmp1 = realloc(com_words, (i + 1) * sizeof(char *));
						com_words = tmp1;
						com_words[i] = NULL;
						break;
					}
					else {
						tmp2 = realloc(com_words[i], strlen(com_words[i+1]) + 1);
						com_words[i] = tmp2;
						strcpy(com_words[i], com_words[i+1]);
					}
				}
				time = 1;
			}

			if( !strcmp(com_words[0], "exit") ) {
				free_command_buffer(com_words);
				return 0;
			}
			else if( !strcmp(com_words[0], "cd") ) {
				if(!com_words[1]) printf("command not found\n");
				else if( chdir(com_words[1]) )
					printf("cannot change directory to %s\n", com_words[1]);
			}
			else if ( !strcmp(com_words[0], "setenv") ) {
				if(!com_words[1]) printf("command not found\n");
				else if(!com_words[2]) printf("command not found\n");
				else if(com_words[3]) printf("command not found\n");
				else
					if( setenv(com_words[1], com_words[2], 1) )
						printf("cannot set enviroment variable\n");
			}
			else if( !strcmp(com_words[0], "unsetenv") ) {
				if(!com_words[1]) printf("command found\n");
				else if(com_words[2]) printf("command not found\n");
				else
					if( unsetenv(com_words[1]) )
						printf("cannot unset enviroment variable\n");
			}
			else {
				int i, wait_or_not = 1;
				int redToFile = 0, redFromFile = 0, redApToFile = 0;
				int pipes = 0;
				int k;
				
				for(i = 0; com_words[i]; ++i) {			/*check for & symbol*/
					if( !strcmp(com_words[i], "&") ) {
						if(!i || com_words[i+1]) {
							printf("command not found\n");
							goto end;
						}
						else if(time) {
							puts("cannot calculate time");
							puts("command not found");
							goto end;
						}
						else {
							com_words = realloc(com_words, (i + 1) * sizeof(char *));
							free(com_words[i]);
							com_words[i] = NULL;
							wait_or_not = 0;
							break;
						}
					}					/*check for > symbol*/
					else if( !strcmp(com_words[i], ">") ) {
						if(!i) {
							printf("command not found\n");
							goto end;
						}
						else if(!com_words[i+1]) {
							printf("command not found\n");
							goto end;
						}
						else if( com_words[i+2] && strcmp(com_words[i+2], "&") ) {
							printf("command not found\n");
							goto end;
						}

						char **tmp1;
						char *tmp2;
						for(k = 0; k < i; ++k) {
							tmp1 = realloc(tmp_buf, (k + 1) * sizeof(char *));
							tmp_buf = tmp1;
							tmp_buf[k] = NULL;
							tmp2 = realloc(tmp_buf[k], (strlen(com_words[k]) + 1) * sizeof(char));
							tmp_buf[k] = tmp2;
							strcpy(tmp_buf[k], com_words[k]);
						}
						tmp1 = realloc(tmp_buf, (k + 2) * sizeof(char *));
						tmp_buf = tmp1;
						tmp_buf[k] = NULL;
						tmp2 = realloc(tmp_buf[k], (strlen(com_words[k+1]) + 1) * sizeof(char));
						tmp_buf[k] = tmp2;
						strcpy(tmp_buf[k], com_words[k+1]);
						
						tmp_buf[k+1] = NULL;
						redToFile = 1;
					}						/*check for >> symbol*/
					else if( !strcmp(com_words[i], ">>") ) {
						if(!com_words[i+1]) {
							printf("command not found\n");
							goto end;
						}
						else if( com_words[i+2] && strcmp(com_words[i+2], "&") ) {
							printf("command not found\n");
							goto end;
						}

						char **tmp1;
						char *tmp2;
						for(k = 0; k < i; ++k) {
							tmp1 = realloc(tmp_buf, (k + 1) * sizeof(char *));
							tmp_buf = tmp1;
							tmp_buf[k] = NULL;
							tmp2 = realloc(tmp_buf[k], (strlen(com_words[k]) + 1) * sizeof(char));
							tmp_buf[k] = tmp2;
							strcpy(tmp_buf[k], com_words[k]);
						}
						tmp1 = realloc(tmp_buf, (k + 2) * sizeof(char *));
						tmp_buf = tmp1;
						tmp_buf[k] = NULL;
						tmp2 = realloc(tmp_buf[k], (strlen(com_words[k+1]) + 1) * sizeof(char));
						tmp_buf[k] = tmp2;
						strcpy(tmp_buf[k], com_words[k+1]);
						
						tmp_buf[k+1] = NULL;
						redApToFile = 1;
					}					/*check for < symbol*/
					else if( !strcmp(com_words[i], "<") ) {
						if(!i) {
							printf("command not found\n");
							goto end;
						}
						else if(!com_words[i+1]) {
							printf("command not found\n");
							goto end;
						}
						else if( com_words[i+2] && strcmp(com_words[i+2], "&") ) {
							printf("command not found\n");
							goto end;
						}

						char **tmp1;
						char *tmp2;
						for(k = 0; k < i; ++k) {
							tmp1 = realloc(tmp_buf, (k + 1) * sizeof(char *));
							tmp_buf = tmp1;
							tmp_buf[k] = NULL;
							tmp2 = realloc(tmp_buf[k], (strlen(com_words[k]) + 1) * sizeof(char));
							tmp_buf[k] = tmp2;
							strcpy(tmp_buf[k], com_words[k]);
						}
						tmp1 = realloc(tmp_buf, (k + 2) * sizeof(char *));
						tmp_buf = tmp1;
						tmp_buf[k] = NULL;
						tmp2 = realloc(tmp_buf[k], (strlen(com_words[k+1]) + 1) * sizeof(char));
						tmp_buf[k] = tmp2;
						strcpy(tmp_buf[k], com_words[k+1]);
						
						tmp_buf[k+1] = NULL;
						redFromFile = 1;
					}						/*check for | symbol*/
					else if( !strcmp(com_words[i], "|") ) {
						if(!i) {
							printf("command not found\n");
							goto end;
						}
						else if(!com_words[i+1]) {
							printf("command not found\n");
							goto end;
						}
						pipes++;
					}
				}

				if(pipes) work_with_pipes(com_words, pipes, wait_or_not);
				else {
					if(time) st_time = times(&st_buf);
					pid = fork();
					if(pid == -1) printf("cannot create child process\n");
					else if(!pid) {
						if(redToFile) redirect_to_file(tmp_buf, k);
						else if(redApToFile) redirect_append_to_file(tmp_buf, k);
						else if(redFromFile) redirect_from_file(tmp_buf, k);
						else if( execvp(com_words[0], com_words) ) {
							printf("command not found\n");
							exit(1);
						}
					}
					else {
						if(wait_or_not) {
							waitpid(pid, &status, 0);
							if(time) {
								end_time = times(&end_buf);
								printf("real = %lf --- ", ((double) end_time - st_time) / ((double) sysconf(_SC_CLK_TCK)));
								printf("user = %lf --- ", ((double) (end_buf.tms_cutime - st_buf.tms_cutime)) / ((double) sysconf(_SC_CLK_TCK)));
								printf("system = %lf\n", ((double) (end_buf.tms_cstime - st_buf.tms_cstime)) / ((double) sysconf(_SC_CLK_TCK)));
							}
						}
						else {
							printf("background process : %s\npid : %d\n", com_words[0], pid);
							sleep(1);
						}
					}
				}
			}

			time = 0;
			end: free_command_buffer(com_words);
			tmp_buf = free_command_buffer(tmp_buf);
		}
	}

	return 0;
}

/*function that handles redirect from file*/

void redirect_from_file(char **tmp_buf, int k)
{
	FILE *fd;
	
	if( !(fd = fopen(tmp_buf[k], "r")) ) {
		printf("cannot open file %s\n", tmp_buf[k]);
		exit(1);
	}

	if( close(fileno(stdin)) ) {
		puts("cannot close stdin file descriptor\n");
		exit(1);
	}
	
	if(dup2(fileno(fd), fileno(stdin)) == -1) {
		printf("cannot redirect input from file %s\n", tmp_buf[k]);
		exit(1);
	}
	
	close(fileno(fd));
	
	free(tmp_buf[k]);
	tmp_buf[k] = NULL;
	
	if( execvp(tmp_buf[0], tmp_buf) ) {
		puts("command not found");
		exit(1);
	}
}

/*function that handles redirect appending to file*/

void redirect_append_to_file(char **tmp_buf, int k)
{
	FILE *fd;
	int old_stdout;
	char *tmp;
	
	if( !(fd = fopen(tmp_buf[k], "a")) ) {
		printf("cannot open file %s\n", tmp_buf[k]);
		exit(1);
	}
	
	old_stdout = dup(fileno(stdout));
	if( close(fileno(stdout)) ) {
		puts("cannot close stdout file descriptor\n");
		exit(1);
	}
	
	if(dup2(fileno(fd), fileno(stdout)) == -1) {
		printf("cannot redirect output to file %s\n", tmp_buf[k]);
		exit(1);
	}
	
	close(fileno(fd));
	
	tmp = malloc(strlen(tmp_buf[k]) + 1);
	strcpy(tmp, tmp_buf[k]);
	
	free(tmp_buf[k]);
	tmp_buf[k] = NULL;
	
	if( execvp(tmp_buf[0], tmp_buf) ) {
		if(dup2(old_stdout, fileno(stdout)) == -1) {
			puts("cannot redirect output to stdout");
			exit(1);
		}
		puts("command not found");
		remove(tmp);
		exit(1);
	}
}


/*function that handles redirect writing to file*/

void redirect_to_file(char **tmp_buf, int k)
{
	FILE *fd;
	int old_stdout;
	char *tmp;
	
	if( !(fd = fopen(tmp_buf[k], "w+")) ) {
		printf("cannot open file %s\n", tmp_buf[k]);
		exit(1);
	}
	
	old_stdout = dup(fileno(stdout));
	if( close(fileno(stdout)) ) {
		puts("cannot close stdout file descriptor\n");
		exit(1);
	}
	
	if(dup2(fileno(fd), fileno(stdout)) == -1) {
		printf("cannot redirect output to file %s\n", tmp_buf[k]);
		exit(1);

	}
	
	close(fileno(fd));
	
	tmp = malloc(strlen(tmp_buf[k]) + 1);
	strcpy(tmp, tmp_buf[k]);
	
	free(tmp_buf[k]);
	tmp_buf[k] = NULL;
	
	if( execvp(tmp_buf[0], tmp_buf) ) {
		if(dup2(old_stdout, fileno(stdout)) == -1) {
			puts("cannot redirect output to stdout");
			exit(1);
		}
		puts("command not found");
		remove(tmp);
		exit(1);
	}
}

/*function that handles execution with pipes*/

void work_with_pipes(char **com_words, int pipes, int wait_or_not)
{
	pid_t pid;
	int pipefd[pipes][2];
	int pipenum;
	int i, k, tmp_words;
	int word_index;
	char ***tmp = NULL;
	int oldstdout;
	int status;
	char **tmp1;
	
	tmp = malloc((pipes + 1) * sizeof(char **));
	for(i = 0; i <= pipes; ++i) tmp[i] = NULL;
	
	i = 0;
	tmp_words = 0;
	for(k = 0; com_words[k]; ++k) {
		if( strcmp(com_words[k], "|") ) {
			tmp1 = realloc(tmp[i], (tmp_words + 1) * sizeof(char *));
			tmp[i] = tmp1;
			tmp[i][tmp_words] = malloc(strlen(com_words[k]) + 1);
			strcpy(tmp[i][tmp_words], com_words[k]);
			++tmp_words;
		}
		else {
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
	
	oldstdout = fileno(stdout);

	if(time) st_time = times(&st_buf);
	
	pipenum = 0;
	for(i = 0; i <= pipes; ++i) {
		if( i && (i != pipes) ) ++pipenum;
		
		pid = fork();
		if(pid == -1) puts("cannot create child process");
		else if(!pid) {
			if(!i) {
								/*first command to execute*/
				if( close(pipefd[0][0]) ) {
					puts("cannot close pipe");
					exit(1);
				}
				
				if( close(fileno(stdout)) ) {
					puts("cannot close stdout");
					exit(1);
				}
				
				if( dup2(pipefd[0][1], fileno(stdout)) == -1 ) {
					puts("cannot redirect stdout to pipe");
					exit(1);
				}
				
				if( close(pipefd[0][1]) ) {
					dup2(oldstdout, fileno(stdout));
					puts("cannot close pipe");
					exit(1);
				}
			}						/*last command to execute*/
			else if(i == pipes) {

				if( close(pipefd[pipes-1][1]) ) {
					puts("cannot close pipe");
					exit(1);
				}
				
				if( close(fileno(stdin)) ) {
					puts("cannot close stdin");
					exit(1);
				}
				
				if( dup2(pipefd[pipes-1][0], fileno(stdin)) == -1 ) {
					puts("cannot redirect stdin to pipe");
					exit(1);
				}
				
				if( close(pipefd[pipes-1][0]) ) {
					puts("cannot close pipe");
					exit(1);
				}
			}						/*intermediate command to execute*/
			else {
				if( close(pipefd[pipenum-1][1]) ) {
					puts("cannot close pipe");
					exit(1);
				}
				
				if( close(fileno(stdin)) ) {
					puts("cannot close stdin");
					exit(1);
				}
				
				if( dup2(pipefd[pipenum-1][0], fileno(stdin)) == -1 ) {
					puts("cannot redirect stdin to pipe");
					exit(1);
				}
				
				if( close(pipefd[pipenum][0]) ) {
					puts("cannot close pipe");
					exit(1);
				}
				
				if( close(fileno(stdout)) ) {
					puts("cannot close stdout");
					exit(1);
				}
				
				if( dup2(pipefd[pipenum][1], fileno(stdout)) == -1) {
					puts("cannot redirect stdout to pipe");
					exit(1);
				}
			}
			
			if ( execvp(tmp[i][0], tmp[i]) ) {
				if(dup2(oldstdout, fileno(stdout)) == -1) {
					puts("cannot redirect output to stdout");
					exit(1);
				}
				puts("command not found");
				exit(1);
			}
		}
		else {
			if(i == pipes) close(pipefd[pipes-1][1]);
			else if( i && (i != pipes) ) close(pipefd[pipenum-1][1]);

			if(wait_or_not) waitpid(pid, &status, 0);
			else {
				printf("background process : %s\npid : %d\n", tmp[i][0], pid);
				if(i == pipes) sleep(1);
			}
		}
	}
	
	if(time) {
		end_time = times(&end_buf);
		printf("real = %lf --- ", ((double) end_time - st_time) / ((double) sysconf(_SC_CLK_TCK)));
		printf("user = %lf --- ", ((double) (end_buf.tms_cutime - st_buf.tms_cutime)) / ((double) sysconf(_SC_CLK_TCK)));
		printf("system = %lf\n", ((double) (end_buf.tms_cstime - st_buf.tms_cstime)) / ((double) sysconf(_SC_CLK_TCK)));
	}
	
	for(i = 0; i < pipes; ++i) {
		close(pipefd[i][0]);
		close(pipefd[i][1]);
	}

	for(i = 0; i <= pipes; ++i) {
		for(k = 0; tmp[i][k]; ++k) free(tmp[i][k]);
		free(tmp[i]);
	}
	free(tmp);
}

/*parse input command*/

char **get_command_words(void)
{
	char ch;
	char **com_array = NULL;
	char **tmp1;
	char *tmp2;
	int word_index = 0;
	int char_index = 0;
	int check1 = 0;
	int check2 = 0;
	int check_to_check = 0;
	int append = 0;
	
	while( (ch = getchar()) != '\n' ) {
		if(ch == ' ' || ch == '\t') {
			if(com_array) {
				if(!check2) {
					tmp2 = realloc(com_array[word_index], (char_index + 1) * sizeof(char));
					com_array[word_index] = tmp2;
					com_array[word_index][char_index] = '\0';
					++word_index;
					char_index = 0;
					check1 = 0;
					check2 = 1;
					continue;
				}
				else {
					append = 0;
					continue;
				}
			}
			else continue;
		}
		
		else if(ch == '>' && append) {
			com_array[word_index-1][1] = ch;
			tmp2 = realloc(com_array[word_index-1], 3 * sizeof(char));
			com_array[word_index-1] = tmp2;
			com_array[word_index-1][2] = '\0';
			append = 0;
			continue;
		}
		else if( (ch == '&' || ch == '>' || ch == '<' || ch == '|') && com_array ) {
			if(ch == '>') append = 1;
			if(!check2) {
				tmp2 = realloc(com_array[word_index], (char_index + 1) * sizeof(char));
				com_array[word_index] = tmp2;
				com_array[word_index][char_index] = '\0';
				++word_index;
				char_index = 0;
			}
			tmp1 = realloc(com_array, (word_index + 1) * sizeof(char *));
			com_array = tmp1;
			com_array[word_index] = NULL;
			check_to_check = 1;
		}
		else if(!check1) {
			tmp1 = realloc(com_array, (word_index + 1) * sizeof(char *));
			com_array = tmp1;
			com_array[word_index] = NULL;
			append = 0;
		}
		tmp2 = realloc(com_array[word_index], (char_index + 1) * sizeof(char));
		com_array[word_index] = tmp2;
		com_array[word_index][char_index] = ch;
		++char_index;
		if(check_to_check) {
			tmp2 = realloc(com_array[word_index], (char_index + 1) * sizeof(char));
			com_array[word_index] = tmp2;
			com_array[word_index][char_index] = '\0';
			++word_index;
			char_index = 0;
			check1 = 0;
			check2 = 1;
			check_to_check = 0;
			if(ch != '>') append = 0;
		}
		else {
			check1 = 1;
			check2 = 0;
		}
	}
	
	if(!com_array) return NULL;
	
	if(!check2) {
		tmp2 = realloc(com_array[word_index], (char_index + 1) * sizeof(char));
		com_array[word_index] = tmp2;
		com_array[word_index][char_index] = '\0';
		++word_index;
	}
	
	tmp1 = realloc(com_array, (word_index + 1) * sizeof(char *));
	com_array = tmp1;
	com_array[word_index] = NULL;
	
	return com_array;
}


/*free command buffers*/

char **free_command_buffer(char **com_words)
{
	int i;
	
	if(!com_words) return;
	
	for(i = 0; com_words[i]; ++i) free(com_words[i]);
	free(com_words);
	return NULL;
}








