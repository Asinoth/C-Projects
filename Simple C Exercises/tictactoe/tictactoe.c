#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void display_table(char table[]);

void fault(char* str){
	printf("%s\n", str);
	exit(1);
}

/*reads an integer from keyboard,
  Returns -1 if ir is negative number
  Further input is asked in case the input
  is < 1 or > 9 or character
*/
int read_int (int PlayerID){
        int num;
        char input[10];

        printf("Player %c plays box: ", PlayerID);
        scanf("%s", &input);

        num = atoi(input);

        while (!(1 <= num && num <= 9) ){
                num = read_int(PlayerID);
	}

        return num;
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

int snatch_win(char table[], int index){

	if (index >= 9)
		return 0;

	if ( table[index] != 'x' && table[index] != 'o'){
        	table[index] = 'o';
/*		display_table(table);*/
      		if (have_winner (table)){
        		table[index] = ' ';
        		return index+1;
		}
        	table[index] = ' ';
	}

	return snatch_win(table, index+1);
}

int block_win(char table[], int index){

	if (index >= 9)
		return 0;

        if ( table[index] != 'x' && table[index] != 'o'){
                table[index] = 'x';
/*		display_table(table);*/
                if (have_winner (table)){
			table[index] = ' ';
                        return index+1;
                }
	        table[index] = ' ';
	}

        return block_win(table, index+1);
}

int computer_strategy(char table[], int current_turn){
	int  i=0, ret=0;

	if ( ret = snatch_win(table, 0) ) return ret;/*win*/
	else if ( ret = block_win(table, 0) ) return ret;/*not lose*/


	if (table[4] == ' ') return 5;

	else if (table[0] == ' ') return 1;
        else if (table[3] == ' ') return 4;
	else if (table[8] == ' ') return 9;
        else if (table[7] == ' ') return 8;
	else if (table[6] == ' ') return 7;
        else if (table[1] == ' ') return 2;
	else if (table[2] == ' ') return 3;
        else if (table[5] == ' ') return 6;


	fault("Problem in strategy function...");
}

/*read input from player and add it to the table*/
char* player_action(char table[], int PlayerID, int current_turn){

	int Num;
        if (PlayerID == 120){
                Num = read_int(PlayerID);
        }
        else{
                Num = computer_strategy(table, current_turn);
		printf("Computer plays box: %d \n", Num);
        }

	/*can not rewrite an already filled box*/
	if ( table[Num-1] == 'x' || table[Num-1] == 'o'){
		printf("Player %c has already played in box %d.\n", table[Num-1], Num);
		player_action(table, PlayerID, current_turn);
	}
	else{
		/*player1 = x,  player2 = o*/
		if (PlayerID == 120)
			table[Num-1] = 'x';
		else
			table[Num-1] = 'o';
	}

	return table;
}

int row_win(char table[], int i){

	if (i > 6)
		return 0;

	if (table[i] == table[i+1] && table[i+1] == table[i+2]
			        && ( table[i+1] == 'x' || table[i+1] == 'o' ) ){
                return 1;
        }

	return row_win(table, i+3);
}

int column_win(char table[], int i){

	if (i>2)
		return 0;

        if (table[i] == table[i+3] && table[i+3] == table[i+6]
                                && ( table[i+3] == 'x' || table[i+3] == 'o' ) ){
                return 1;
        }

        return column_win(table, i+1);
}

int diagonal_win(char table[]){

	if (table[0] == table[4] && table[4] == table[8]
                                && ( table[0] == 'x' || table[0] == 'o' ) ){
		return 1;
	}
	else if (table[2] == table[4] && table[4] == table[6]
                                && ( table[2] == 'x' || table[2] == 'o' ) ){
		return 1;
	}
	else{
		return 0;
	}
}

int have_winner (char table[]){

	if (row_win(table, 0))	return 1;/*check rows*/
	else if (column_win(table, 0)) return 1;/*check columns*/
	else if (diagonal_win(table)) return 1;/*check diagonal*/

	return 0;
}


int get_winner(int last_turn){

	if ( last_turn % 2 ){
		return 'X';
	}
	else if (!(last_turn % 2) ){
		return 'O';
	}
}

int exists_empty(char table[], int i){

	if (table[i] == ' ')
		return 1;

	if (i<9){
		return exists_empty(table, i+1);
	}

	return 0;
}

int play_tic_tac_toe(char table[], int turn){

	if (have_winner(table) || !exists_empty(table, 0))
		return turn;

	display_table(table);
	if (turn%2 == 0){
		table = player_action(table, 120, turn);
	}
	else{
		table = player_action(table, 111, turn);
	}

	play_tic_tac_toe(table, turn+1);
}

int main(int argc, char *argv[]){
        int winner = 0;
	char table[9] = {' ',' ',' ',' ',' ',' ',' ',' ',' '};

	winner = get_winner( play_tic_tac_toe(table, 0) );
	display_table(table);

	if (!have_winner(table))
		printf("It is a tie.\n");
	else
		printf("Player %c wins.\n", winner);

        return 0;
}


