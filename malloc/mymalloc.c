/* Memory allocation example
 * COMP3000, Alex Tronstanovsky, 100984702
 * Version: March 18, 2018
 * Tested on Ubuntu 16.04 (64-bit)
 * Code sources:
 * "A Malloc Tutorial" by Marwan Burelle
 * http://www.inf.udec.cl/~leo/Malloc_tutorial.pdf
 * "Implementing malloc" by Michael Saelee
 * http://moss.cs.iit.edu/cs351/slides/slides-malloc.pdf
 */

 /*
***NOTE***

The following code has been adapted for use from

"A Malloc Tutorial" by Marwan Burelle
* http://www.inf.udec.cl/~leo/Malloc_tutorial.pdf *

(Allowable as per assignment specification:

"You may reuse and adapt code returned by Google, but give credit to and cite
your sources.")
 */

#include "mymalloc.h"
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* base address of heap */
void * base = NULL;

/* Searching for a free segment
 */
 /*
 * Function : find_header
 * in/out   : last -> address of last visited segment when search finishes
 * in       : size -> size of the block of memory associated with the header
 */
static t_header find_header(t_header *last, size_t size) {
   t_header b = base; /* start from heap base address */
   /* first fit algorithm */
   while (b && !(b->free && b->size >= size)) {
      *last = b; /* save address of last visited segment */
      b = b->next;
   }
   /* if success, return segment address else NULL */
   return (b);
}

/* Extending the heap
 */
 /*
 * Function : extend_heap
 * in/out   : last -> address of last segment on heap
 * in       : size -> the required size of extension of heap
 */
static t_header extend_heap(t_header last , size_t s) {
   void * sb;
   t_header b; /* ptr to header being created */
   b = sbrk(0); /* get current address of break */
   /* increment the break */
   sb = sbrk(header_SIZE + s);
   /* the following verification is in accordance with Linux man pages */
   if (sb == (void *) -1) {
      /* failure! */
      return (NULL);
   }
   /* success! */
   b->size = s;
   b->next = NULL;
   b->prev = last;
   b->ptr = (char *)b + header_SIZE;
   if (last) {
      last ->next = b;
   }
   b->free = 0;
   return (b);
}

/* header splitting
 */
 /*
 * Function : split_header
 * in/out   : b -> block on heap to be split
 * in       : s -> size of new block split off
 */
void split_header(t_header b, size_t s) {
   t_header new; /* ptr to header being created */
   new = (t_header)( (char *)b->ptr + s ); /* addr of new header */
   new->size = b->size - s - header_SIZE ; /* size of new header */
   new->next = b->next;
   new->prev = b;
   new->free = 1;
   /* In the following, "header_SIZE" is in bytes. Using the type
    * cast "(char *)", the "void *" pointer "p" is converted to a
    * character pointer. Hence, the pointer subtraction is done in
    * the byte unit. The result is converted as a "t_header" pointer.
    */
   new->ptr = (char *)new + header_SIZE; /* addr of available mem in new header */
   b->size = s; /* update size of old header */
   b->next = new;
   if (new->next) {
      new->next->prev = new;
   }
}

/* Memory allocation function
 * size = number of bytes
 * returns pointer to allocated memory,
 * NULL in case of failure
 */
void * mymalloc(size_t size) {
   t_header b, last;
   size_t s;
   s = ALIGN(size); /* alignment of size */
   /* not first time */
   if (base) {
      /* find a header */
      last = base;
      b = find_header(&last, s);
      if (b) {
         /* can split? */
         if ((b->size - s) >= (header_SIZE + 8)) {
            split_header(b, s);
         }
         b->free = 0;
      } else {
         /* no fitting header, extend the heap */
         b = extend_heap(last, s);
         if (!b) return (NULL);
      }
   /* first time */
   } else {
      b = extend_heap(NULL, s);
      if (!b) return (NULL);
      /* set base address of heap */
      base = b;
   }
   return (b->ptr);
}

/* free function
 */
 /*
 * Function : myfree
 * in/out   : p -> address of block to be deallocated
 */
void myfree(void *p) {
  t_header b;
  if ( valid_addr (p))
  {
    b = get_header (p);
    b->free = 1;
    /* fusion with previous if possible */
    if(b->prev && b->prev ->free)
    b = fusion(b->prev );
    /* then fusion with next */
    if (b->next)
    fusion(b);
    else
    {
      /* free the end of the heap */
      if (b->prev)
      b->prev ->next = NULL;
      else
      /* No more block !*/
      base = NULL;
      brk(b);
    }
  }
}

/* Valid addr for free */
/*
* Function : valid_addr
* in/out   : p -> address of block to be deallocated
* Verifies that p points to an address on the heap which is correctly associated
* and alligned with/to a t_header
*/
int valid_addr (void *p)
{
  if (base)
  {
    if ( p>base && p<sbrk (0))
    {
      return (p == ( get_header (p))->ptr );
    }
  }
  return (0);
}

/*
* Function : fusion
* in/out   : b -> address of block to be fused with its next neighbour
* Fusion/coalescence occurs in the case when a block is freed, and either of its
* neighbours (next/prev) are also free
*/
t_header fusion(t_header b)
{
  if (b->next && b->next->free ){
    b->size += header_SIZE + b->next->size;
    b->next = b->next->next;
    if (b->next)
      b->next ->prev = b;
  }
  return (b);
}

/* Get the block from and addr */
/*
* Function : get_header
* in/out   : p -> address of block associated to header
*/
t_header get_header (void *p)
{
  char *tmp;
  tmp = p;
  return (p = tmp -= header_SIZE );
}
