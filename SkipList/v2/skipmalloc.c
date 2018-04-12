#include "mymalloc.h"
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

   // while (b && !(b->free && b->size >= size)) {
   //    *last = b; /* save address of last visited segment */
   //    b = b->next;
   // }

   //.......

   /* if success, return segment address else NULL */
   return (b);
}

t_header* findEntry(size_t size, skiplist *list)
{
  t_header* p;

  p = list->head;

  //printf("%d\n",strcmp(p->right->key, key));
  while(true)
  {
    while( (strcmp(p->right->key, posInf) != 0) &&
    (size < p->size) )
    {
      p = p->right;
    }
    if(p->down != NULL)
    {
      p = p->down;
    }
    else
    {
      break;
    }
  }
  return p;
}


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
