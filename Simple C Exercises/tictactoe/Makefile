all:	tictactoe.o anoiTower.o
	gcc100 tictactoe.o -o Tictactoe
	gcc100 anoiTower.o -o AnoiTower

tictactoe.o: tictactoe.c
	gcc100 -c -g tictactoe.c
anoiTower.o: anoiTower.c
	gcc100 -c anoiTower.c

clean:
	rm -rf Tictactoe
	rm -rf AnoiTower
	rm -rf *.o
	rm -rf *.out
