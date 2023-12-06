all:	triliza.o sudoku.o
	gcc triliza.o -o Triliza
	gcc sudoku.o -o Sudoku

triliza.o: triliza.c
	gcc -c triliza.c
sudoku.o: sudoku.c
	gcc -c sudoku.c

clean:
	rm -rf Triliza
	rm -rf Sudoku
	rm -rf *.o
	rm -rf *.out
