#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct stack{
	char* id;
	int* stack;
	int height;
}tower_t;

typedef struct anoi_towers{
	tower_t left;
	tower_t mid;
	tower_t right;
	int disc_num;
}anoi_t;


void fault(char* str);
void display_game_state(anoi_t anoi);
void display_tower_item(anoi_t anoi, tower_t tower, int index);
void display_spaces_between_towers(anoi_t anoi, tower_t tower, int index);
void display_spaces_between_names(anoi_t anoi);
void display_base(anoi_t anoi);
int get_number_digits(int a_num);
void init_game(anoi_t* anoi, int disc_num);
void init_tower(tower_t* tower, int disc_num, char *id, int is_left);
int pop(tower_t* tower);
void push(tower_t* tower, int an_item);
int game_is_over (anoi_t anoi);
int is_full(tower_t tower, int disc_num);
int is_empty(tower_t tower);
void solve_anoi_problem(anoi_t* anoi);
void threesome(anoi_t* anoi);
void a_legal_move(anoi_t anoi, tower_t* source, tower_t* dest);

/*generate fault message and terminate*/
void fault(char* str){
	printf("%s\n",str);
	exit(1);
}


/*displays the table on the console*/
void display_game_state(anoi_t anoi){
	int i=0;

	/*display names*/
	display_spaces_between_names(anoi);
	printf("%s",  anoi.left.id);
	display_spaces_between_names(anoi);
	display_spaces_between_names(anoi);
	printf("%s",  anoi.mid.id);
	display_spaces_between_names(anoi);
	display_spaces_between_names(anoi);
	printf("%s",  anoi.right.id);
	printf("\n");

	/*display towers*/
	for(i=anoi.disc_num; i>0; --i){
		display_tower_item(anoi, anoi.left, i);
		display_tower_item(anoi, anoi.mid, i);
		display_tower_item(anoi, anoi.right, i);
		printf("\n");
	}

	display_base(anoi);
}

void display_spaces_between_names(anoi_t anoi){
        int i = 0;

        for(    i = 0;
                i < ( anoi.disc_num - strlen(anoi.right.id)/2 ) ;
                ++i     ){

                printf(" ");
        }
}

void display_base(anoi_t anoi){
	int i=0;
	for (i=0; i<((anoi.disc_num+anoi.disc_num + 1) *3); ++i){
		printf("-");
	}
	printf("\n");
}

/*displays a tower on the console*/
void display_tower_item(anoi_t anoi, tower_t tower, int index){
	int i=0, j=0;
	int digits_of_item = 0;
	int biggest_disc_digits = 0;

	digits_of_item = get_number_digits(tower.stack[index-1] );
	biggest_disc_digits = get_number_digits(anoi.disc_num );

	display_spaces_between_towers (anoi, tower, index);

	/*display tower*/
	if (index <= tower.height){
		printf(" ");
		for(i=0; i<(tower.stack[index-1]+tower.stack[index-1]-1)/digits_of_item; ++i){
			printf("%d", tower.stack[index-1]);
		}
		if (digits_of_item > 1){
			printf("%d", (tower.stack[index-1]/10));
		}
		printf(" ");
	}
	else{
		printf("|");
	}

	display_spaces_between_towers (anoi, tower, index);

}

void display_spaces_between_towers (anoi_t anoi, tower_t tower, int index){

	int i = 0;

        for(    i = 0;
                i < ( (((anoi.disc_num + anoi.disc_num -1)) -
                        (tower.stack[index-1] + tower.stack[index-1] -1)) / 2 ) ;
                ++i     ){
                printf(" ");
        }
}

int get_number_digits(int a_num){

	if (a_num < 10){
		return 1;
	}
	else if(a_num < 100){
		return 2;
	}
	else{
		fault("Problem in get_number_digits, a_num > 99!!!");
	}
}

/*initializes all the anoi towers*/
void init_game(anoi_t* anoi, int disc_num){

	init_tower(&anoi->left, disc_num, "Left", 1);
	init_tower(&anoi->mid, disc_num, "Middle", 0);
	init_tower(&anoi->right, disc_num, "Right", 0);
	anoi->disc_num = disc_num;
}

/*initialized a tower*/
void init_tower(tower_t* tower, int disc_num, char *id, int is_left){
	int i=0;

	tower->stack = (int *)malloc(disc_num*sizeof(int));
	tower->id = (char*)strdup(id);
	tower->height = 0;

	if (is_left){
		for(i=disc_num; i>0; --i){
			push(tower, i);
		}
	}
}

/*returns the value of the tower's top item*/
int top(tower_t tower){
	return tower.stack[tower.height-1];
}

/*deletes top tower item and returns its value*/
int pop(tower_t* tower){
	int ret=0;

	ret = tower->stack[--tower->height];
	tower->stack[tower->height] = 0;

	return ret;
}

/*adds arg item to top of the tower*/
void push(tower_t* tower, int an_item){
	tower->stack[tower->height++] = an_item;
}

/*return 1 if anoi towers problem is solved*/
int game_is_over (anoi_t anoi){

	if (!(is_full(anoi.right, anoi.disc_num) && is_empty(anoi.left) && is_empty(anoi.mid))){
/*		printf("right: %d, left: %d, mid: %d", anoi.right.height, anoi.left.height, anoi.mid.height); */
        	return 0;
	}
	else{
		return 1;
	}
}

/*returns 1 if tower is full*/
int is_full(tower_t tower, int disc_num){
	if(!(tower.height == disc_num)){
		return 0;
	}
	else{
		return 1;
	}
}

/*returns 1 if tower is empty*/
int is_empty(tower_t tower){
	if (!(tower.height == 0)){
		return 0;
	}
	else{
		return 1;
	}
}

/*solves the anoi problem*/
void solve_anoi_problem(anoi_t* anoi){

	threesome(anoi);
	solve_anoi_problem(anoi);
}

/*manages three move instrunctions depending on N (odd/even)*/
void threesome(anoi_t* anoi){

        if (anoi->disc_num % 2 == 0){
		a_legal_move(*anoi, &anoi->left, &anoi->mid);
		a_legal_move(*anoi, &anoi->left, &anoi->right);
		a_legal_move(*anoi, &anoi->mid, &anoi->right);
        }
        else if(anoi->disc_num % 2 == 1){
		a_legal_move(*anoi, &anoi->left,&anoi->right);
		a_legal_move(*anoi, &anoi->left,&anoi->mid);
		a_legal_move(*anoi, &anoi->mid,&anoi->right);
        }
        else{
                fault("Problem in threesome...how the hell did this happen!!!");
        }
}

/*manages the legal move from the two possible between two towers*/
void a_legal_move(anoi_t anoi, tower_t* one, tower_t* other){

	sleep(1);
        display_game_state(anoi);

        if ( game_is_over(anoi) ){
                fault("Anoi solved!!!\n");
        }
	else{
		if (top(*one) < top(*other)){
			printf("%s -> %s\n\n\n", one->id, other->id);
			push(other, pop(one));
		}
		else if (top(*one) > top(*other)){
			printf("%s -> %s\n\n\n", other->id, one->id);
	        	push(one, pop(other));
		}
	}
}

int main(int argc, char *argv[]){

        int N;
	anoi_t anoi;
        if ( (argc != 2) || (atoi(argv[1]) < 1) || (atoi(argv[1]) > 99)){
                fault("Usage: AnoiTowers <N>...N must be a positive number.");
        }
        else{
                N = atoi(argv[1]);
        }

	init_game(&anoi, N);
	solve_anoi_problem(&anoi);
        return 0;
}


