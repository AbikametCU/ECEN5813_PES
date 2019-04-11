
cbuffer: ring.c ring.h
	gcc -o ring ring.c

unittest: ring_test.c ring.c test
	gcc -o test ring_test.c ring.c -lcunit
	./test


