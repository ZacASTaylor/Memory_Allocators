all: malloc_freelist.o malloc_test.o
	cc -o tester malloc_freelist.o malloc_test.o
malloc_freelist.o: malloc_freelist.c malloc_freelist.h linked_list.h
	cc -c malloc_freelist.c
malloc_test.o: malloc_test.c malloc_freelist.h linked_list.h
	cc -c malloc_test.c
clean:
	rm -f *.o *~ core *~ tester
