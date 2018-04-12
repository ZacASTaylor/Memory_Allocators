/* Memory allocation example
 * COMP3000, Alex Tronstanovsky, 100984702
 * Version: March 18, 2018
 */

#ifndef MYALLOC_H
#define MYALLOC_H

#include <stddef.h>

/* header structure
 */
struct s_header {
   size_t           size; /* in bytes */
   int              free; /* 1=available; 0=not available */
   struct s_header *next; /* ptr to next header */
   struct s_header *prev; /* ptr to previous header */
   void *           ptr; /* addr of data part, after header */
};
typedef struct s_header * t_header;

/* segment must begin at an address on a multiple of alignment size,
   Linux requires 8-byte alignment */
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

#endif
