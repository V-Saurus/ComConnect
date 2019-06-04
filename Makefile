# Makefile for ComConnect

comcon: main.o
	gcc -o comcon main.o

main.o: main.c
	gcc -c main.c

clean:
	rm -f *.o comcon

