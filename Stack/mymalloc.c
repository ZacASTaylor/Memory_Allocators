/* Memory allocation example
 *  COMP3000, Michel Barbeau
 *  Version: March 5, 2018
 *  Tested on Ubuntu 16.04 (64-bit)
 *  Code sources:
 *  "A Malloc Tutorial" by Marwan Burelle
 *  http://www.inf.udec.cl/~leo/Malloc_tutorial.pdf
 *  "Implementing malloc" by Michael Saelee
 *  http://moss.cs.iit.edu/cs351/slides/slides-malloc.pdf
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

// Starting address
void * initial = NULL;

// added for stackAllocator
void * top = NULL;

void * mymalloc(size_t size) {
	
	if(initial == NULL)
		initial = sbrk(0);
	
	t_header b;

	b = sbrk(header_SIZE + size); // increment the break
	
	if (b == (void *) -1) { 
		printf("Break increment fail! Exiting.\n");
		exit(0) ; // unsuccessful break increment 	
	}
	
	
	b->size = size;
	b->ptr = (char *)b + header_SIZE;
	
	if(top != NULL){
		t_header oldTop = top;
		b->prev = (char *)oldTop->ptr - header_SIZE;
	}
	else{
		b->prev = initial;
	}
	
	top = b;
	
	if (!base){
		base = b; // set base on first call
	}
	
	return (b->ptr);
} 


void myfree(void *p){
	
	if (sbrk(0) == initial) {
		printf("Trying to free with no memory allocated.\n");
		return;
	}
	
	t_header b;
	
	t_header oldTop = top;
	
	top = oldTop->prev;
	
	if (!base){
		return; // set base on first call
	}

	b = sbrk((oldTop->size + header_SIZE)*-1); // decrement the break
	
	if (b == (void*) -1) { 
		printf("Break decrement fail! Exiting.\n");
		exit(0) ; // unsuccessful break decrement 
	}
}
