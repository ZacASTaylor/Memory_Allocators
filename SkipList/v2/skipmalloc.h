#ifndef SKIPMALLOC_H
#define SKIPMALLOC_H

#include <stddef.h>

struct s_header {
   size_t           size; /* in bytes */
   int              free; /* 1=available; 0=not available */
   struct s_header *right; /* ptr to next header */
   struct s_header *left; /* ptr to previous header */
   struct s_header *up; /* ptr to next header */
   struct s_header *down; /* ptr to previous header */
   void *           ptr; /* addr of data part, after header */
};
typedef struct s_header * t_header;

typedef struct skiplist{
  s_header *head;
  s_header *tail;
  int numNodes;
  int height;
  int random;
} skiplist;

#define ALIGNMENT 8 // must be a power of 2
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))

/* header size */
# define header_SIZE (ALIGN(sizeof(struct s_header)))

/* malloc function
 */
void *   mymalloc   (size_t size );

/* free function
 */
void     myfree     (void *p);

/* get_header function
*/

t_header get_header (void *p);

/* fusion (coalescence) function
*/
t_header fusion     (t_header b);

/* valid address function
*/
int      valid_addr (void *p);

t_header* findEntry(size_t, skiplist *);



#endif
