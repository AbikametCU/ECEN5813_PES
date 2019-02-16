CC=gcc
CFLAGS=-I
Project1: Project1Sprint1.c
	$(CC) -o Project1 Project1Sprint1.c
clean: 
	rm -rf *.o Project1
