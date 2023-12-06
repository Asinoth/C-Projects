#include <assert.h>
#include "sudoku.h"
#include <ctype.h>

#define SIZE 9


void sudoku_print(FILE *s, Grid_T g)
{
    int i,j;
    for (i = 0; i<SIZE; i++)
    {
        for (j = 0; j<SIZE; j++)
        {
            fprintf(s,"%d ",grid_read_value(g, i, j));
        }
        fprintf(s,"\n");
    }
    return;
}

/* diavasma puzzle, to input periexei 81 ari8mous > 0 kai < 9 */
/* xwrizontai me keno h newline( ka8e 9 ari8mous)*/
Grid_T sudoku_read(void)
{

    Grid_T puzzle;
    int n,i= 0, j= 0, counter=0;
    char c[2];/*pinaka giati h atoi pairnei string san orisma*/
    c[1] = '\0';
    while ((c[0]=getchar()) != EOF)
    {
        if (c[0]=='\n')
        {/*allagh seiras*/
            i++;
            j = 0;
            continue;
        }

        if ( isdigit((int)c[0]) )
        {
            n = atoi(c);
            if (n>SIZE || n<0){/*elegxos input*/
                printf("Error...all cells must be between 0 and 9\n");
                exit(EXIT_FAILURE);
            }
            grid_update_value(&puzzle,i,j,n);
            j++;
            counter++;
        }/*
        else if(c[0]==' ')
        {
            continue;
        }*/
    }
    if (counter != 81){/*elegxos input*/
        printf("Error...the cells' number must be 81\n");
        exit(EXIT_FAILURE);
    }
    return puzzle;
}


/* typwma twn error */
void print_errors(Grid_T g)
{
    int i, j, k, l, num;
    for (i = 0; i<SIZE; i++){
        for (j = 0; j<SIZE; j++){
            num = grid_read_value(g,i,j);
            if (num == 0)
                continue;
            k=0;
            while (k<SIZE)
            {
                if (num == grid_read_value(g, i, k) && k!=j)
                {/* gia grammes  */
                    printf("Error sunantame ton idio ari8mo se mia grammh: [%d , %d] , [%d , %d]\n",i,j,i,l);
                }
                if (num == grid_read_value(g, k, j) && k!=i)
                {/* gia seires */
                    printf("Error sunantame ton idio ari8mo se mia sthlh: [%d , %d] , [%d , %d]\n",i,j,k,j);
                }
                k++;
            }
            k = (i/3)*3;
            while (k <(i/3)*3+3)
            {
                l = (j/3)*3;
                while (l<(j/3)*3+3)
                {
                    if ( (((k!=i) && (l==j))||((k==i) && (l!=j)))/* gia ta 3x3 ypo tetragwna */
                            && num == grid_read_value(g, k, l))
                        printf("Error sunantame ton idio ari8mo se ena ypo-grind 3x3: [%d,%d] - [%d,%d]\n",i, j, k, l);
                    l++;
                }
                k++;
            }
        }
    }
}

/* epistrefei 1 an yparxei error alliws epistrefei 0 */
/* idios tropos me to panw */
int has_errors(Grid_T g)
{
    int i, j, k, l, num;
    int flag = 0;
    for (i = 0; i<SIZE; i++)
        for (j = 0; j<SIZE; j++){
            num = grid_read_value(g,i,j);
            if (num == 0)
                continue;
            for (k = 0; k<SIZE; k++){
                if (num == grid_read_value(g, i, k) && k!=j)
                    flag = 1;
                if (num == grid_read_value(g, k, j) && k!=i)
                    flag = 1;
            }
	    k = (i/3)*3;
            while (k < (i/3)*3+3)
	    {
		l = (j/3)*3;
                while (l < (j/3)*3+3)
		{
                    if ( (((k!=i) && (l==j))||((k==i) && (l!=j)))
                            && num == grid_read_value(g, k, l))
                        flag = 1;
		    l++;
		}
		k++;
	    }
        }
    return flag;
}

/*ekxwrhsh ths timhs n to i,j stoixeio */
static void sudoku_apply_choice(Grid_T *g, int i, int j, int n)
{
  grid_update_value(g, i, j, n);
  return;
}

/*sumplhrwsh dunatwn epilogwn */
void sudoku_init_choices(Grid_T *g)
{
    int i, j, k, l, m;
    grid_set_unique(g);
    for (i = 0; i<SIZE; i++)
    {/*choices set*/
        for (j = 0; j<SIZE; j++)
	{
            grid_set_count(g,i,j);
            grid_clear_choice(g,i,j,0);
            for (m = 1; m<=SIZE; m++)
                grid_set_choice(g, i, j, m);
        }
    }
    for (i = 0; i<SIZE; i++) {
        for (j = 0; j<SIZE; j++){
            m = grid_read_value(*g,i,j);
            if (m != 0)
	    {    /* an den einai adeio */
                grid_clear_count(g,i,j);
                for (m=1; m<SIZE + 1; m++)
                    grid_clear_choice(g, i, j, m);
                continue;
            } /* if cell is taken clear all choices */
            else
	    {    /* an einai adeio */
                for (k = 0; k<SIZE; k++){
                    m = grid_read_value(*g, k, j);
                    grid_remove_choice(g, i, j, m); /* remove olh th sthlh */
		    m = grid_read_value(*g, i, k);
                    grid_remove_choice(g, i, j, m); /* remove olh th grammh */
                }
		k = (i/3)*3;
                while (k < (i/3)*3+3)
		{
		    l = (j/3)*3;
                    while (l < (j/3)*3+3)
		    {
                        m = grid_read_value(*g, k, l);
                        grid_remove_choice(g, i, j, m);/* remove ola ta 3x3 upo tetragwna */
			l++;
                    }
		    k++;
		}
            }
        }
    }return;
}

/* epistrefei mia tuxaia epilogh apo ta cells pou exoun monadikh epilogh */
static int sudoku_try_next_random(Grid_T g, int *row, int *col)
{
    int i, j, k, n;
    int nchoice = SIZE + 1;

    for (i = 0; i < SIZE; i++)
    {
	for (j = 0; j < SIZE; j++)
	{
	    n = grid_read_count(g, i, j);
	    if (n > 0 && n < nchoice)
	        nchoice = n;
	}
    }
    /*twra to nchoice einai to mikrotero dunato */
    while (nchoice < SIZE + 1)
    {
	i = rand()%SIZE;
	j = rand()%SIZE;
	n = grid_read_count(g, i, j);/*pairnw random cells mexri na isxuei n == nchoice*/
	if (n == nchoice) {
	    while (1) {
		k = rand()%SIZE + 1;/* dinei random nums mexri vrei kapoion pou ontws uparxei*/
		if (grid_choice_is_valid(g, i, j, k))
		{/*ekxwrhsh timhs*/
		    *row = i;
		    *col = j;
		    return k;
		}
	    }
	}
    }
    return 0;/*alliws to afhnei 0*/
}

/* Remove value from grid */
static int sudoku_remove_choice(Grid_T *g, int i, int j, int n)
{/*removes mia choice kai kanei return ton ari8mo twn choices prin kanei remove*/
  int nchoice;

  nchoice = grid_read_count(*g, i, j);
  grid_remove_choice(g, i, j, n);

  return nchoice;
}

/*remove to n apo, thn r, thn c kai to upo-grind */
static void sudoku_eliminate_choice(Grid_T *g, int r, int c, int n)
{
  int k, l;
  for (k = 0; k < SIZE; k++)
  {
    grid_remove_choice(g, k, c, n);        /* diagrafh tou n apo th sthlh*/
    grid_remove_choice(g, r, k, n);        /* diagrafh tou n apo th grammh */
  }

  k = (r/3)*3;
  while (k < (r/3)*3+3)
  {
    l = (c/3)*3;
    while (l < (c/3)*3+3)
    {
      grid_remove_choice(g, k, l, n);      /* diagrafh tou n apo to ypo-grid*/
       l++;
    }
    k++;
  }

  return;
}

static Grid_T sudoku_generate_complete(void)
{
  Grid_T new_g;
  int ready_puzzle[SIZE*SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8, 9,
            	                  4, 5, 6, 7, 8, 9, 1, 2, 3,
      	     	              	  7, 8, 9, 1, 2, 3, 4, 5, 6,
                              	  2, 3, 4, 5, 6, 7, 8, 9, 1,
                              	  5, 6, 7, 8, 9, 1, 2, 3, 4,
                              	  8, 9, 1, 2, 3, 4, 5, 6, 7,
                              	  3, 4, 5, 6, 7, 8, 9, 1, 2,
                              	  6, 7, 8, 9, 1, 2, 3, 4, 5,
                              	  9, 1, 2, 3, 4, 5, 6, 7, 8 };
  int i, j, n, end;

  for (end =SIZE + 1; end!=0; end--)/*10 fores*/
  {
    for (i = 0; i < SIZE; i++)
    {
      for (j = 0; j < SIZE; j++)
      {/* ola ta cells pairnoun timh mhden*/
        grid_update_value(&new_g, i, j, 0);
      }
    }

    sudoku_init_choices(&new_g);/*set choices*/
    while ( (n = sudoku_try_next_random(new_g, &i, &j)) )/* to vhma eksartatai apo thn try_next*/
    {
      sudoku_remove_choice(&new_g, i, j, n);/*vgazei to n apo ta choices*/
      sudoku_apply_choice(&new_g, i, j, n);/*to ekxwrei sto cell*/
      sudoku_eliminate_choice(&new_g, i, j, n);/*kai to vgazei apo thn grammh, th sthlh k to upo-grid pou vriskete*/
    }
    if (sudoku_is_correct(new_g))
      return new_g;
  }

  n = 0;
  for (i = 0; i < SIZE; i++)
  {
    for (j = 0; j < SIZE; j++)
    {
      grid_update_value(&new_g, i, j, ready_puzzle[n++]);/* se ka8e cell vazei to ready_puzzle*/
    }
  }

  sudoku_init_choices(&new_g);/*reset*/
  return new_g;
}

/* metrhths twn mhdenikwn cells*/
int zero_counter(Grid_T g)
{
    int i,j,zeros = 0;
    for (i = 0; i<SIZE; i++)
        for (j = 0; j<SIZE; j++)
            if (grid_read_value(g,i,j) == 0)
                zeros++;
    return zeros;
}

/* lunei to puzzle g kai epistrefei th lush tou */
/* epistrefei ena apo ta lumena puzzles an uparxoun parapanw apo mia luseis.*/
Grid_T sudoku_solve(Grid_T g)
{
    int i, j, n, choice;
    Grid_T tmp, solution;

    while ( ( n = sudoku_try_next_random(g, &i, &j) ) )
    {
        choice = sudoku_remove_choice(&g, i, j, n);/*poses epiloges menoun*/
        if (choice != 1)/* an exoume kialles epiloges*/
	{
            grid_clear_unique(&g);/*unique == 0*/
            tmp = g;
            sudoku_apply_choice(&tmp, i, j, n);
            solution = sudoku_solve(tmp);/* anadromiko vhma*/
            if ( sudoku_is_correct(solution) )
                return solution;
        }
        else/*an ksemeiname*/
        {
            sudoku_apply_choice(&g, i, j, n);
            sudoku_eliminate_choice(&g, i, j, n);
        }

    }
    return g;
}

/* epistrefei 1 an einai monadiko to g */
int sudoku_solution_is_unique(Grid_T g)
{
    return (grid_read_unique(g));
}

/* dhmiourgei ena puzzle kai to epistrefei opou nels...*/
/* o ari8mos twn stoixeiwn pou den einai mhden */
/*an unique == 1 tote 8a prepei na epistrepsei monadiko puzzle*/
Grid_T sudoku_generate(int nelts, int unique)
{
    Grid_T puzzle, tmp;
    int i, j, Nums = 81;
    puzzle = sudoku_generate_complete();
    while (Nums != nelts)
    {
	j = rand()%SIZE;
        i = rand()%SIZE;

        if ( !grid_read_value(puzzle, i, j) )
            continue;           /* an value == 0 paso */
        grid_update_value(&tmp,i,j,0);
        sudoku_init_choices(&tmp);
        if (unique != 1)
        {   /* not unique */
            sudoku_init_choices(&puzzle);
            grid_update_value(&puzzle,i,j,0);
	    Nums--;
        }
        else
	{   /* unique */
	    tmp = puzzle;
            if (sudoku_solution_is_unique(sudoku_solve(tmp)))
	    {
                sudoku_init_choices(&puzzle);
                grid_update_value(&puzzle, i, j, 0);
		Nums--;
	    }
        }
    }
    return puzzle;
}


/* print ola ta arrors */
void sudoku_print_errors(Grid_T g)
{
    print_errors(g);
    return;
}

/* true an to g einai swsto */
int sudoku_is_correct(Grid_T g)
{
    int statement = has_errors(g) + zero_counter(g);
    if (statement==0)
        return 1;
    return 0;
}




int main(int argc, char **argv)
{
    Grid_T init, last;
    int n, flag = 1;
    srand(getpid());

    if (argc == 1)         /* sudoku */
    {
      init = sudoku_read();
      sudoku_print(stderr, init);
      sudoku_init_choices(&init);
      last = sudoku_solve(init);
      if (sudoku_is_correct(last))
      {
        if (sudoku_solution_is_unique(last))
          fprintf(stderr, "Exei monadikh lush:\n");
        else
          fprintf(stderr, "Exei lush:\n");
      }
      else
      {
        fprintf(stderr, "Den exei lush:\n");
      }
      sudoku_print(stdout, last);
      sudoku_print_errors(last);
    }
    else if (argc == 2)         /* elegxos an einai swsto */
    {
      if (argv[1][0] == '-' && argv[1][1] == 'c' && argv[1][2] == '\0')
      {
        init = sudoku_read();
        sudoku_print(stderr, init);
        if (sudoku_is_correct(init))
            fprintf(stderr, "H lush einai swsth.\n");
        else
          fprintf(stderr, "H lush den einai swsth.\n");
        sudoku_print_errors(init);
      }
    }
    else if (argc == 3)  /* -g */
    {
      if (argv[1][0] == '-' && argv[1][1] == 'g' && argv[1][2] == '\0')
      {
        n = 0;
        while (argv[2][n] != '\0')
          if (!isdigit((int)argv[2][n++]))
	  {
            flag = 0;
          }

        if (flag)
	{
          n = atoi(argv[2]);
          init = sudoku_generate(n, 0);
          sudoku_print(stdout, init);
        }
      }
    }
    else if (argc == 4)      /* -u */
    {
      if (argv[3][0] == '-' && argv[3][1] == 'u' && argv[3][2] == '\0')
	while (argv[2][n] != '\0')
          if (!isdigit((int)argv[2][n++]))
	  {
            flag = 0;
	  }

	if (flag)
        {
          n = atoi(argv[2]);
          init = sudoku_generate(n, 1);
          sudoku_print(stdout, init);
        }
    }
    else
    {
      fprintf(stderr,"la8os eisodos, exete tis parakatw epiloges:\nsudoku -c | -g nelts (-u)");
    }

  return 0;
}

