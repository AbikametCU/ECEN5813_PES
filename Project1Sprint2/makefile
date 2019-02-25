CC=gcc
CFLAGS=-I

Project1: Project1Sprint2.c
	$(CC) -o Project1S2 Project1Sprint2.c
clean: 
	rm -rf *.o Project1S2

test: Project1Sprint2.c project1S2TestScript
	./Project1S2 < project1S2TestScript > project1S2TestResult
	gedit project1S2TestResult
