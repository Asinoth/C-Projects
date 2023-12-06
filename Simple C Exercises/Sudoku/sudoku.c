
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 9
#define LENGTH_OF_BLOCK 3

/*displays a message and terminates programm*/
void fault(char *s){
        printf("%s\n",s);
        exit(1);
}

/*reads an integer from keyboard,
  Returns -1 if ir is negative number
  Further input is asked in case the input
  in < 1 or > 9 or character
*/
int read_int_from_file (FILE *file){
        int num, ret;
        char input[10];

        ret = fscanf(file, "%s", &input);
        if ( (!ret) || (ret == EOF) ) {
	        fault("\n\t Please check the format of your txt file!!!\nA 9x9 board of numbers needed.");
        }

        num = atoi(input);

        if (num < 1 || num > 9){
                fault("\n\t Please check the format of your txt file!!!\nOnly numbers between 1-9 allowed,");
        }

        return num;
}

/*Reads a txt file and creates a matrix structure*/
int ** init_table(char *TextFile){

        int **NewMatrix;
        FILE *fp;
        int i, j;

        if ( !(fp = fopen(TextFile, "r") ) ){
                printf("Error in %s.\n",TextFile);
                fault("Cannot open text file.");
        }

	printf("\n\t Attention!!!\n");
	printf("\t The program will read only the first 81 numbers,\n");
	printf("\t in case of more numbers\n");
        sleep(3);

	/*memory allocate*/
	NewMatrix = (int**) malloc (SIZE * sizeof(int *) );

        for (i=0; i<9; i++){
		/*memory allocate*/
		NewMatrix[i] = (int*) malloc (SIZE * sizeof(int) );

                for (j=0; j<SIZE; j++){
                        /*read next integer*/
			NewMatrix[i][j] = read_int_from_file(fp);
                }
        }

        if ( (i != SIZE) || (j != SIZE) )
        	fault("\n\t Please check the format of your txt file.");

        fclose(fp);

        return NewMatrix;
}

void display_table(int **table){

	int i, j;

	for (i=0; i<SIZE; i++){
		printf("\n---------------------------------------\n|");
		for (j=0; j<SIZE; j++){
			printf(" %d |", table[i][j]);
			if ((j+1) % 3 == 0 && j != SIZE -1) printf("|");
		}
		if ((i+1) % 3 == 0) printf("\n---------------------------------------");
	}
	printf("\n");
}

int contains_int(int *bag, int len,  int Num){

	int i=0;

	while (i < len ){
        	if (bag[i] == Num )
			return 1;

        	i++;
        }
	return 0;
}

int check_vertical(int ** table){
	int i, j, k, ret = 0;
	int *Num_bag;

	k = 0;
        for (i=0; i<SIZE; i++){

		/*mem allocate*/
		Num_bag = (int *) malloc (SIZE * sizeof(int) );

                for (j=0; j<SIZE; j++){

			/* if the next number is already inside the bag then we have an error*/
			if ( contains_int(Num_bag, k, table[j][i]) ){
				printf("Error at row: %d.\n", i+1);
				ret = 1;
				break;
			}

			/* add a number to the bag*/
			Num_bag[k++] = table[j][i];
                }
		k = 0;
		free(Num_bag);
        }
	return ret;
}

int check_horizontal(int ** table){
        int i, j, k, ret = 0;
        int *Num_bag;

        k = 0;
        for (i=0; i<SIZE; i++){

                /*mem allocate*/
                Num_bag = (int *) malloc (SIZE * sizeof(int) );

                for (j=0; j<SIZE; j++){

                        /* if the next number is already inside the bag then we have an error*/
                        if ( contains_int(Num_bag, k, table[i][j]) ){
                                printf("Error at line: %d.\n", i+1);
				ret = 1;
                                break;
                        }

                        /* add a number to the bag*/
                        Num_bag[k++] = table[i][j];
                }
                k = 0;
                free(Num_bag);
        }
	return ret;
}

int check_block (int **table, int line, int row){

	int i, j, k;
	int *Num_bag;
	/*mem allocate*/
        Num_bag = (int *) malloc (SIZE * sizeof(int) );

	for (i = line, k = 0; i< line+LENGTH_OF_BLOCK; i++){

	       for (j=row; j< row+LENGTH_OF_BLOCK; j++){
                        /* if the next number is already inside the bag then we have an error*/
                        if ( contains_int(Num_bag, k, table[i][j]) ){
                                return 1;
                        }

                        /* add a number to the bag*/
                        Num_bag[k++] = table[i][j];
	        }
	}
	free(Num_bag);
	return 0;
}

int check_all_blocks(int ** table){

	int i, j, ret =0;
	int error_in_box = 1;
        for (i=0; i<SIZE; i+=3){
                for (j=0; j<SIZE; j+=3){

			if ( check_block(table,i,j) ){
				printf("Error at box: %d.\n", error_in_box);
				ret = 1;
			}
			error_in_box++;
		}
        }

	return ret;
}

void sudoku_errors(int ** table){

	int error_flag;

	error_flag = check_vertical(table);
	error_flag += check_horizontal(table);
	error_flag += check_all_blocks(table);

	if (!error_flag){
		printf("\n\t No errors in this sudoku!!!\n");
	}

	return ;
}

int main(int argc, char *argv[]){

        int i, N;
	int **sudoku_matrix;

        /*check for the right paramaters*/
        if ( (argc != 2)  ){
                fault("The programm needs 1 parameter, the txt file to read.\n");
        }/*init the two matrices*/
        else{
		sudoku_matrix = init_table(argv[1]);
        }

	display_table(sudoku_matrix);

	sudoku_errors(sudoku_matrix);

        return 0;
}

