#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <inttypes.h>

typedef uint64_t pa_memory_int;

typedef struct {
	pa_memory_int size;
	bool allocated;
	char data;
} pa_memory_block;
/*
extern PA_STATIC_UNLESS_TESTING __thread pa_memory_block *master_memory_block;
extern PA_STATIC_UNLESS_TESTING __thread pa_memory_int master_memory_block_size;
*/

//extern pa_memory_block *master_memory_block;
//extern pa_memory_int    master_memory_block_size;

bool pa_init_thread(pa_memory_int master_memory_block_size);
bool pa_uninit_thread(void);

void *pa_malloc  (size_t size);
//void pa_calloc  (size_t numElem, size_t elSize);
//void pa_realloc (void  *ptr,     size_t size);
void pa_free  (void  *ptr);

#define PA_MEMORY_BLOCK_HEADER_SIZE ( offsetof(pa_memory_block, data) )
#define PA_FIRST_VALID_ADDRESS_IN_POOL master_memory_block
#define PA_LAST_VALID_ADDRESS_IN_POOL (PA_FIRST_VALID_ADDRESS_IN_POOL + master_memory_block_size)
#define PA_POINTER_IS_IN_RANGE(a, b, c) ( ((a) < ((b) + (c))) && ((a) >=  (b)) )
#define PA_POINTER_IS_IN_POOL(a) PA_POINTER_IS_IN_RANGE (a, PA_FIRST_VALID_ADDRESS_IN_POOL, master_memory_block_size)
