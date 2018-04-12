#include <string.h>
#include "poolAllocator.h"
/*
PMPA_STATIC_UNLESS_TESTING
__thread pmpa_memory_block *master_memory_block = NULL;

PMPA_STATIC_UNLESS_TESTING
__thread pmpa_memory_int master_memory_block_size = 0;
*/
//*master_memory_block = NULL;
//master_memory_block_size = 0;
pa_memory_block *master_memory_block      = NULL;
pa_memory_int    master_memory_block_size = 0;

void concat_sequential_blocks(pa_memory_block *memory_block, bool is_allocated)
{
	pa_memory_block *current_memory_block = memory_block;
	pa_memory_block *next_memory_block    = NULL;

	if (current_memory_block->allocated != is_allocated)
		return;

	while ( (next_memory_block = current_memory_block
                             + current_memory_block->size
                             + PA_MEMORY_BLOCK_HEADER_SIZE)
        &&
			    PA_POINTER_IS_IN_POOL(next_memory_block + PA_MEMORY_BLOCK_HEADER_SIZE)
        &&
			    (next_memory_block->allocated == is_allocated) )
          {
            current_memory_block->size += next_memory_block->size
            + PA_MEMORY_BLOCK_HEADER_SIZE;
          }
}

pa_memory_block *find_first_block(bool is_allocated, pa_memory_int min_size)
{
	pa_memory_block *memory_block = master_memory_block;

	while (PA_POINTER_IS_IN_POOL(memory_block + sizeof(pa_memory_block))) {
		/* If we're trying to find an block, then defragment the pool as we go along.
		 * This incurs a minor speed penalty, but not having to spend time
		 * iterating over a fragmented pool makes up for it. */
		if (is_allocated == false)
			concat_sequential_blocks(memory_block, is_allocated);

		if ( (memory_block->allocated == is_allocated)
      && (memory_block->size >= min_size) )
    {
			return memory_block;
		} else
    {
			memory_block += memory_block->size + PA_MEMORY_BLOCK_HEADER_SIZE;
		}
	}
	return NULL;
}

void split_block(pa_memory_block *memory_block, pa_memory_int size)
{
	pa_memory_block *second_memory_block          = memory_block
                                                + size
                                                + PA_MEMORY_BLOCK_HEADER_SIZE;
	pa_memory_block *original_second_memory_block = memory_block
                                                + memory_block->size
                                                + PA_MEMORY_BLOCK_HEADER_SIZE;
	pa_memory_int    original_memory_block_size   = memory_block->size;

	memory_block->allocated = false;

	/* We can't split this block if there's not enough room to create another one. */
	if ( PA_POINTER_IS_IN_RANGE((second_memory_block + PA_MEMORY_BLOCK_HEADER_SIZE),
                               0, original_second_memory_block)
      &&
	   ( PA_POINTER_IS_IN_POOL(second_memory_block + sizeof(pa_memory_block)) ) ) {
		memory_block->size = size;
		second_memory_block->size = original_memory_block_size - (size + PA_MEMORY_BLOCK_HEADER_SIZE);
		second_memory_block->allocated = false;
	}
}

/*
 * Externally accessible API functions.
 */

bool pa_init_thread(pa_memory_int size)
{
	if ( (master_memory_block = malloc(size)) ) {

		master_memory_block->size      = size - PA_MEMORY_BLOCK_HEADER_SIZE;
		master_memory_block->allocated = false;
    master_memory_block_size       = size;
		return true;
	} else {
		return false;
	}
}

bool pmpa_uninit_thread(void)
{
	master_memory_block_size = 0;
	free(master_memory_block);
	return true;
}

/*
 * Externally accessible C memory functions.
 */

void *pa_malloc(size_t size)
{
	pa_memory_block *memory_block = find_first_block(false, size);

	if (memory_block) {
		split_block(memory_block, size);
		memory_block->allocated = true;

		return &(memory_block->data);
	} else {
		return NULL;
	}
}

void pa_free(void *ptr)
{
	pa_memory_block *memory_block = ptr - PA_MEMORY_BLOCK_HEADER_SIZE;

	if (ptr == NULL)
		return;

	memory_block->allocated = false;
}
