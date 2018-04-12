/* Memory allocation tester program
 * COMP3000, Michel Barbeau
 * Version: March 1, 2018
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "mymalloc.h"
#include "tlpi_hdr.h"
#include <sys/times.h>
#include <time.h>

displayProcessTimes(const char *msg)
{
  struct   tms t;
  clock_t  clockTime;
  static long clockTicks = 0;
  if (msg != NULL)
  printf("%s", msg);
  if (clockTicks == 0) { /* Fetch clock ticks on first call */
    clockTicks = sysconf(_SC_CLK_TCK);
    if (clockTicks == -1)
    {
      printf("error");
    }
    //errExit("sysconf");
  }
  clockTime = clock();
  if (clockTime == -1)
  {
    printf("error");
  }
  //  errExit("clock");
  printf(" clock() returns: %ld clocks-per-sec (%.2f secs)\n",
  (long) clockTime, (double) clockTime / CLOCKS_PER_SEC);
  if (times(&t) == -1)
  {
    printf("error");
  }
  //errExit("times");
  printf(" times() yields: user CPU=%.2f; system CPU: %.2f\n",
  (double) t.tms_utime / clockTicks,
  (double) t.tms_stime / clockTicks);
}

void main(int argc, char *argv[]) {
   int sizes[] = { 1024, 2048, 4096 };
   int N = 3;
   int i;
   void * p[N];

   printf("\nCLOCKS_PER_SEC=%ld sysconf(_SC_CLK_TCK)=%ld\n\n",
   (long) CLOCKS_PER_SEC, sysconf(_SC_CLK_TCK));

   fprintf(stdout, "\n");
   fprintf(stdout, "Initial program break: %10p\n", sbrk(0));

   fprintf(stdout, "*** First test ***\n");
   printf("\n");
  displayProcessTimes("BEFORE FIRST TEST:\n");
  printf("\n");
   for (i = 0; i < N; i++) {
      fprintf(stderr, "Allocating %d bytes\n", sizes[i]);
      p[i] = mymalloc(sizes[i]);
      printf("\n");
    displayProcessTimes("ALLOCATING:");
    printf("\n");
      if (p[i]==NULL) {
         fprintf(stderr, "mymalloc failed!\n");
         exit(EXIT_FAILURE);
      }
      fprintf(stdout, "After allocation, program break is:  %10p\n", sbrk(0));
   }
   for (i = N-1; i >=0; i--) {
      fprintf(stderr, "Address being freed: %10p\n", p[i]);
      myfree(p[i]);
      printf("\n");
    displayProcessTimes("FREEING:");
    printf("\n");
      fprintf(stdout, "After freeing, program break is: %10p\n", sbrk(0));
   }

   fprintf(stdout, "*** 2nd test ***\n"); fflush(stdout);
   fprintf(stdout, "Testing freeing invalid address\n");
   myfree(NULL);
   printf("\n");
  displayProcessTimes("INVALID:");
  printf("\n");
   fprintf(stdout, "Program break is:  %10p\n", sbrk(0));

   fprintf(stdout, "*** 3rd test ***\n");
   for (i = 0; i < N; i++) {
      fprintf(stderr, "Allocating %d bytes\n", sizes[i]);
      p[i] = mymalloc(sizes[i]);
      if (p[i]==NULL) {
         fprintf(stderr, "mymalloc failed!\n");
         exit(EXIT_FAILURE);
      }
      fprintf(stdout, "After allocation, program break is:  %10p\n", sbrk(0));
   }
   for (i = 0; i < N; i++) {
      fprintf(stderr, "Address being freed: %10p\n", p[i]);
      myfree(p[i]);
      fprintf(stdout, "After freeing, program break is: %10p\n", sbrk(0));
   }

   fprintf(stdout, "*** 4th test ***\n");
   fprintf(stdout, "Final program break is:  %10p\n", sbrk(0));

   return;
}


/*
void main(){

	skiplist* s = malloc(sizeof(skiplist));

	s = skiplist_init(s);

	printHorizontal(s);


	char* x = s->head->key;

	printf("\n\n%s\n\n", x);
	printf("------\n");
	printf("======");

	put("ABC", 123, s);
	// printf("\n");
	// printHorizontal(s);
	// printf("\n");


}
*/
