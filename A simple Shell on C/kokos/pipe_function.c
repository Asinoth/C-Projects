#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>


char ***pipe_function(char **command_word,int pipes,int wait){
	
	int commands = pipes +1;
	int the_command = 0;
	int pipe_number = 0;
	int position =0 ;
	int position_word = 0 ;
	int pipefds[pipes][2];
	
	char **command_to_execute[commands];

	while(command_word[position] != NULL){
		
		if(the_command < commands){
			
			command_to_execute[the_command][position] = command_word[position];
			while(command_word[position][position_word] != '\0'){
				
				if (command_word[position][position_word] == '|'){
					
					command_to_execute[the_command][position][position_word] = '\0';
					the_command++;
				}
				
			}
			
		}
		
	}
	return command_to_execute;
	
}