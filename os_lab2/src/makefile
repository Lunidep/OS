start: prog.out child
	rm -r *.o
	./prog.out

prog.out: main.o string.o vector.o
	gcc -o prog.out main.o string.o vector.o -lm

child: child.o string.o
	gcc -o child child.o string.o -lm

play: prog.out child
	valgrind ./prog.out

main.o: main.c string.c vector.c

child.o: child.c string.c

.c.o: 
	gcc -pedantic -Wall -g -c $< -o $@
