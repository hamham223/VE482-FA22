hamster: parse.o main.o command.o
	gcc -o mumsh main.o parse.o command.o -Wall -Wextra -Werror -pedantic -Wno-unused-result -Wconversion
main.o: global.h command.h parse.h main.c
	gcc -c main.c -o main.o -Wall -Wextra -Werror -pedantic -Wno-unused-result -Wconversion
parse.o: parse.h global.h parse.c
	gcc -c parse.c -o parse.o -Wall -Wextra -Werror -pedantic -Wno-unused-result -Wconversion
command.o: command.h command.c
	gcc -c command.c -o command.o -Wall -Wextra -Werror -pedantic -Wno-unused-result -Wconversion
clean:
	rm -rf *.o *.out mumsh