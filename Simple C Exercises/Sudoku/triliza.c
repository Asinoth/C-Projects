#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*reads an integer from keyboard,
  Returns -1 if ir is negative number
  Further input is asked in case the input
  in < 1 or > 9 or character
*/
int read_int (int PlayerID){
        int num;
        char input[10];

        printf("Player %c: ", PlayerID);
        scanf("%s", &input);

        num = atoi(input);

        while (num < 1 || num > 9){
                num = read_int(PlayerID);
	}

        return num;
}

/*read input from player and add it to the table*/
char* player_action(char table[], int PlayerID){

	int Num;

	Num = read_int(PlayerID);
	/*can not rewrite an already filled box*/
	while ( table[Num-1] == 'x' || table[Num-1] == 'o'){
		Num = read_int(PlayerID);
	}

	/*player1 = x,  player2 = o*/
	if (PlayerID == 120)
		table[Num-1] = 'x';
	else
		table[Num-1] = 'o';

	return table;
}

/*display the table on the console*/
void display_table(char table[]){

	int i;

	for (i=0; i<9; i++){
		if (i % 3 == 0 && i){
			printf("\n-----------\n");
		}

		if ( (i + 1) % 3 )
			printf(" %c |", table[i] );
		else
			printf(" %c", table[i] );
	}
	printf("\n\n");
}

int have_winner (char table[]){

	int i;

	/*check rows*/
	for (i=0; i<9; i+=3){

		if (table[i] == table[i+1] && table[i+1] == table[i+2]
                                && ( table[i+1] == 'x' || table[i+1] == 'o' ) ){
			return 1;
		}
	}
	/*check columns*/
        for (i=0; i<3; i++){

                if (table[i] == table[i+3] && table[i+3] == table[i+6]
                                && ( table[i+3] == 'x' || table[i+3] == 'o' ) ){
			return 1;
		}
        }
	/* check diagonal */
	if (table[0] == table[4] && table[4] == table[8]
                                && ( table[0] == 'x' || table[0] == 'o' ) ){
		return 1;
	}
	else if (table[2] == table[4] && table[4] == table[6]
                                && ( table[2] == 'x' || table[2] == 'o' ) ){
		return 1;
	}
	return 0;
}

int main(int argc, char *argv[]){

        int i, LastId;
	char table[9] = {' '};


	for (i=0; (!have_winner(table)) && (i < 9); i++ ){

		display_table(table);

		if (i%2 == 0){
			player_action(table, 120);
			LastId = 120;
		}
		else{
			player_action(table, 111);
			LastId = 111;
		}
	}

	/*display the last player are the winner*/
	display_table(table);
	if (have_winner(table))
		printf("player %c wins!!!\n\n", LastId);
	else
		printf("It is a tie!!!\n\n");

        return 0;
}


