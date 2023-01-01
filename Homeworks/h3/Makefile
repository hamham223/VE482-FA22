all: ex2.o list.o sort.o
	gcc -o ex2 ex2.o list.o sort.o -Wall -Wextra -Werror -std=c11 -pedantic
debug:
	make clean
	gcc -o ex2 -D DEBUG *.c
mem:
	gcc -o ex2 *.c -D DEBUG -fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined 
list.o: list.c list.h
	gcc -o list.o -c list.c -Wall -Wextra -Werror -std=c11 -pedantic 
sort.o: sort.h sort.c
	gcc -o sort.o -c sort.c -Wall -Wextra -Werror -std=c11 -pedantic
ex2.o: ex2.c list.h
	gcc -o ex2.o -c ex2.c -Wall -Wextra -Werror -std=c11 -pedantic
clean:
	rm -f *.o ex2