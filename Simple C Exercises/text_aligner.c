#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct alligned_text{
	char** text;
	int current_line;
	int characters_per_line;
}alligned_text_t;

/*generate fault message and terminate*/
void fault(char* str){
        printf("%s\n",str);
        exit(1);
}

void fill_in_line_with_spaces(alligned_text_t *alligned_text){

	char* pch = NULL;
	char* spaced_line = NULL, * tmp_line = NULL;
	int characters_to_be_filled = 0, i=0, current_char=0, first_time_run=0;

	/*preperation*/
	characters_to_be_filled = alligned_text->characters_per_line - strlen(alligned_text->text[alligned_text->current_line]);
	spaced_line = (char*) malloc (alligned_text->characters_per_line * sizeof(char) );

	/* run through every character of the line and create a new line that has
	  an extra space character added for each space. */
	for (i=0; i < strlen(alligned_text->text[alligned_text->current_line]); ++i ){

		spaced_line[current_char++] = alligned_text->text[alligned_text->current_line][i];

		if (alligned_text->text[alligned_text->current_line][i] == ' ' && characters_to_be_filled > 0){
			spaced_line[current_char++] = ' ';
			--characters_to_be_filled;
		}
	}
	strcpy(alligned_text->text[alligned_text->current_line],spaced_line);

	if (strlen(alligned_text->text[alligned_text->current_line]) < alligned_text->characters_per_line){
		fill_in_line_with_spaces(alligned_text);
	}
	else{
		return ;
	}
}

void add_a_word_to_alligned_text(char* a_word, alligned_text_t *alligned_text){

	char** temp_text = NULL;
	/*word+space fits so add it to the line*/
	if (strlen(alligned_text->text[alligned_text->current_line]) == 0){
		strcpy(alligned_text->text[alligned_text->current_line], "");
		strcat(alligned_text->text[alligned_text->current_line], a_word);
	}
	else if ( (strlen(alligned_text->text[alligned_text->current_line]) + strlen(a_word) + 1) <= alligned_text->characters_per_line){  // <---------------- EDW TO BUG !!!
	/*	if (strlen(alligned_text->text[alligned_text->current_line]) != 0){
			/*add a space before the word as a seperator*/
		strcat(alligned_text->text[alligned_text->current_line], " ");
		strcat(alligned_text->text[alligned_text->current_line], a_word);
	}/*word+space do not fit so try again*/
	else{
		printf("\nline+keno+leksh: %d > chars_perLine: %d\n", ((strlen(alligned_text->text[alligned_text->current_line]) + strlen(a_word) + 1)),//test print
									alligned_text->characters_per_line);

		/*add some spaces*/
//		printf("before: %d\n", strlen(alligned_text->text[alligned_text->current_line]));
                fill_in_line_with_spaces(alligned_text);
//		printf("after: %d\n", strlen(alligned_text->text[alligned_text->current_line]));
		/*memory allocate for one more line and raise the current line by 1*/
		++alligned_text->current_line;
		temp_text = (char**) realloc (alligned_text->text, (alligned_text->current_line + 1) * sizeof(char*) );
		if (!temp_text){
			fault("Problem with realloc");
		}
		else{
			alligned_text->text = temp_text;
			temp_text = NULL;
		}

		alligned_text->text[alligned_text->current_line] = (char*) malloc ( alligned_text->characters_per_line * sizeof(char) );

		/*call again*/
		add_a_word_to_alligned_text(a_word, alligned_text);
	}
}

void manage_alignment_of_txt(FILE *fp, alligned_text_t* alligned_text){

	char* pch;
	char line[1000];

	while ( fgets ( line, sizeof(line), fp ) != NULL ){
		printf("\nnext lines length: %d\n", strlen(line) );
		pch = strtok (line," \t\r\n");
  		while (pch != NULL)
  		{
			if (alligned_text->characters_per_line >= strlen(pch)){
				add_a_word_to_alligned_text(pch, alligned_text);
			}
			else{
				printf("Try an argument larger that: %d\n", strlen(pch));
				fault("The maximum letters per line must be greater than the length of the largest word int the text!!!");
			}

    			pch = strtok (NULL, " \t\r\n");
  		}
		printf("\n");
        }
}

void display_alligned_text(alligned_text_t alligned_text){

	int i = 0;

	for (i=0; i<alligned_text.current_line+1; ++i){
		printf("%s\n",alligned_text.text[i] );
	}
}

int main(int argc, char **argv){

        FILE *fp = NULL;
	alligned_text_t alligned_text;
	int i=0;

        /*check for the right paramaters*/
        if ( argc != 3 ){
                fault("Usage: AlignText <somename.txt> <max letters per line> !!!");
        }
        else{
                /*read file*/
                if ( !(fp = fopen(argv[1], "r") ) ){
                        printf("Error in %s.\n",argv[1]);
                        fault("Cannot open the file.");
                }
		else{
			/*initialize alligned text*/
			alligned_text.current_line = 0;
			alligned_text.characters_per_line = atoi(argv[2]);
			alligned_text.text = (char**) malloc ( sizeof(char*));
			alligned_text.text[0] = (char*) malloc (sizeof(char) * alligned_text.characters_per_line);

			/*create the alligned text*/
			manage_alignment_of_txt(fp, &alligned_text);
			/*display alligned text*/
			display_alligned_text(alligned_text);
		}
	}


	return 0;
}
