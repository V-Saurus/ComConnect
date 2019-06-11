# Makefile for ComConnect

comcon: main.o com_init.o send_get.o
	gcc -o comcon main.o com_init.o send_get.o

main.o: main.c
	gcc -c main.c

com_init.o: com_init.c
	gcc -c com_init.c

send_get.o: send_get.c
	gcc -c send_get.c

clean:
	rm -f *.o comcon

