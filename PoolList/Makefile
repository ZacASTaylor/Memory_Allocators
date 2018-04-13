all: poolAllocator.o tester.o
	cc -o tester poolAllocator.o tester.o
poolAllocator.o: poolAllocator.c poolAllocator.h
	cc -c poolAllocator.c
tester.o: tester.c
	cc -c tester.c
clean:
	rm -f *.o *~ core *~ tester
