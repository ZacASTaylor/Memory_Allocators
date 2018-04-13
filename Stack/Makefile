all: mymalloc.o tester.o
	cc -o tester mymalloc.o tester.o
mymalloc.o: mymalloc.c mymalloc.h
	cc -c mymalloc.c
tester.o: tester.c
	cc -c tester.c
error_functions.o: error_functions.c error_functions.h
	cc -c error_functions.c

clean:
	rm -f *.o *~ core *~
