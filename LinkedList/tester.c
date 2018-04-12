/* Memory allocation tester program
* COMP3000, Michel Barbeau
* Version: March 1, 2018
*/
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "mymalloc.h"
// #include "tlpi_hdr.h"
// #include <sys/times.h>
#include <time.h>
#include <string.h>

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



void main(int argc, char *argv[]) {

  typedef enum	rbcolor
  {
    BLACK = 0,
    RED = 1
  }		t_rbcolor;

  typedef struct		s_metadata
  {
    size_t		size;
    size_t		free;
    struct s_metadata	*next;
    struct s_metadata	*prev;
  }			t_metadata;

  typedef size_t		t_key;
  typedef t_metadata	t_value;

  typedef struct		s_rbnode
  {
    size_t		size;
    size_t		free;
    t_metadata		*next;
    t_metadata		*prev;
    t_key			key;
    t_value		**tab_values;
    size_t		size_tab;
    size_t		nb_activ;
    t_rbcolor		color;
    struct s_rbnode	*left;
    struct s_rbnode	*right;
  }			t_rbnode;

  typedef struct		s_malloc
  {
    t_rbnode		*root_rbtree;
    t_metadata		*last_node;
    void			*end_in_page;
    void			*first_block;
    int			page_size;
    pthread_mutex_t	mutex;
    size_t		page_remaining;
  }			t_malloc;

  int sizes[] = { 32, 64, 256, 512, 1024, 2048, 4096 };
  int N = 7;
  int i;
  long nOperations;
  nOperations = atoi(argv[1]);

  int sizeof_rbnode = sizeof(t_rbnode);
  int sizeof_smalloc = sizeof(t_malloc);


  void * p[N][nOperations];

  float opsPerSec;
  float timePerOp;
  int memoryPeak;

  memoryPeak = 0;






  // for(int i = 0; i < N; i++)
  // {

  //   setTimer(&m_start);
  //   for (int j=0; j<nOperations; j++)
  //   {
  //     p[i][j] = mymalloc(sizes[i]);
  //     memoryPeak += sizes[i];
  //     memoryPeak += sizeof_rbnode;
  //   }
  //
  //   for (int j=0; j<nOperations; j++)
  //   {
  //     myfree(p[i][j]);
  //   }
  //   memoryPeak += sizeof_smalloc;
  //   setTimer(&m_end);
  //
  //   double elapsedTime;
  //   elapsedTime = calculateElapsedTime();
  //   opsPerSec = nOperations / elapsedTime;
  //   timePerOp = elapsedTime / nOperations;
  //
  //
 
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


  r = rand() % 7;
  s = rand() % nOperations;
  int randomByteSize = 0;
  randomByteSize = rand()%1024 + 1;

  setTimer(&m_start);
  for (int j=0; j<nOperations; j++)
  {

    q[j] = mymalloc(randomByteSize);
    memoryPeak += sizes[r];
    memoryPeak += sizeof_rbnode;
    randomByteSize = rand()%1024 + 1;
    r = rand() % 7;
  }

  for (int j=0; j<nOperations/2; j++)
  {
 
    myfree(q[rand_array[j]]);
    //s = rand() % nOperations/2;
  }

  // randomByteSize = rand()%4096 + 1;
	// for (int j=0; j<nOperations/2; j++)
	// {

	// 	t[j] = malloc(randomByteSize);
	// 	memoryPeak += randomByteSize;
	// 	//memoryPeak += sizeof_rbnode;
	// 	randomByteSize = rand()%4096 + 1;
	// 	r = rand() % 7;
	// }
  memoryPeak += sizeof_smalloc;
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





  return;
}
