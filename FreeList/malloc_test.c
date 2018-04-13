#include "malloc_freelist.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "linked_list.h"
#include <string.h>
#define MALLOC_SIZE (512 * 1024 * 1024)

static int rand_int(int n) {
	int limit = RAND_MAX - RAND_MAX % n;
	int rnd;

	do {
		rnd = rand();
	} while (rnd >= limit);
	return rnd % n;
}

void shuffle(int *array, int n) {
	int i, j, tmp;

	for (i = n - 1; i > 0; i--) {
		j = rand_int(i + 1);
		tmp = array[j];
		array[j] = array[i];
		array[i] = tmp;
	}
}

struct timespec m_start, m_end;

void setTimer(struct timespec* timer) {
	clock_gettime(CLOCK_REALTIME, timer);
}

double calculateElapsedTime()  {
	struct timespec temp;
	if ((m_end.tv_nsec - m_start.tv_nsec) < 0) {
		temp.tv_sec = m_end.tv_sec - m_start.tv_sec - 1;
		temp.tv_nsec = 1e9 + m_end.tv_nsec - m_start.tv_nsec;
	} else {
		temp.tv_sec = m_end.tv_sec - m_start.tv_sec;
		temp.tv_nsec = m_end.tv_nsec - m_start.tv_nsec;
	}

	const double time_sec = (double) temp.tv_sec;
	const double time_nsec = (double) temp.tv_nsec;
	const double time_msec = (time_sec * 1e3) + (time_nsec / 1e6);

	return time_msec;
}

int main(int argc, char *argv[])
{
	void * my_block = malloc(MALLOC_SIZE); // For host system demonstration, alloc 4MB block

	//Initialize our simple freelist with the space.
	//On our embedded platform, this will be the RAM address for your pool and the size you desire.
	 malloc_addblock(my_block, MALLOC_SIZE);


	int sizes[] = { 32, 64, 256, 512, 1024, 2048, 4096 };
	int N = 7;
	int i;
	long nOperations;
	nOperations = atoi(argv[1]);

	void * p[N][nOperations];

	float opsPerSec;
	float timePerOp;
	int memoryPeak;

	memoryPeak = 0;

	srand(time(NULL));
	int r, s;
	void * q[nOperations];
	void * t[nOperations/2];

	int rand_array[nOperations];

	for(int i = 0; i < nOperations; i++)
	{
		rand_array[i] = i;
	}
	shuffle(rand_array,nOperations);

	int randomByteSize = 0;
	randomByteSize = rand()%1024 + 1;

	setTimer(&m_start);
	for (int j=0; j<nOperations; j++)
	{

		q[j] = fl_malloc(randomByteSize);
		memoryPeak += sizeof(ll_t);
		randomByteSize = rand()%1024 + 1;
		r = rand() % 7;
	}
	for (int j=0; j<nOperations/2; j++)
	{

		fl_free(q[rand_array[j]]);
	}

	setTimer(&m_end);

	double elapsedTime;
	elapsedTime = calculateElapsedTime();
	opsPerSec = nOperations / elapsedTime;
	timePerOp = elapsedTime / nOperations;

		if(atoi(argv[2]) == 1){
 			fprintf(stdout, "%lf\n", elapsedTime);  
		}
		if(atoi(argv[2]) == 2){
 			fprintf(stdout, "%lf\n", opsPerSec);  
		}
		if(atoi(argv[2]) == 3){
 			fprintf(stdout, "%lf\n", timePerOp);  
		}


	memoryPeak = 0;

	free(my_block);
}
