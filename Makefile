
all: determine.c knot.c
	gcc -g -Wall -o determine determine.c
	gcc -g -Wall -o -knot knot.c

determine: determine.c
	gcc -g -Wall -o determine determine.c

knot: knot.c
	gcc -g -Wall -o knot knot.c

clean:
	rm -f determine knot