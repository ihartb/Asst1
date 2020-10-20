all:memgrind

mymalloc:mymalloc.c
	gcc -g mymalloc -o mymalloc.c

memgrind:mymalloc.c memgrind.c
	gcc -c -Wall -g mymalloc.c
	gcc -g -o memgrind memgrind.c mymalloc.c

clean:
	rm -f mymalloc.o memgrind
