
target: main.c
	gcc -g -Wall -Werror -o determine main.c

test: test.c
	gcc -g -Wall -Werror -o test test.c